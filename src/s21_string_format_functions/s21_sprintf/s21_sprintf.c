#include "s21_sprintf.h"

#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

#include "../../s21_string.h"
#include "../s21_format.h"

// Flags parsing stages enum

typedef enum s21_flags_stage_t {
  FLAGS_STAGE,
  WIDTH_STAGE,
  PRECISION_STAGE,
  LENGTH_STAGE,
  SPECIFIER_STAGE,
  ERROR_STAGE
} s21_flags_stage_t;

static const char *s21_sprintf_flags = "-+ #0";
static const char *s21_sprintf_length_descs = "hlL";
static const char *s21_sprintf_specifiers = "cdieEfgGosuxXpn%";

// Specifier processing functions
int process_specifier(char **str, const char **format, va_list *values,
                      int length);
int s21_sprint_value(char **str, va_list *values, s21_flags_t flags,
                     char length_modifier, char specifier, int length);
int get_value_representation(char **v, va_list *values, s21_flags_t flags,
                             char length_modifier, char specifier,
                             int main_length);
int get_signed_integer_representation(
    char **v, va_list *values, s21_flags_t flags, s21_size_t *length,
    char length_modifier,
    int (*shortf)(short, char **, s21_flags_t, s21_size_t *),
    int (*normalf)(int, char **, s21_flags_t, s21_size_t *),
    int (*longf)(long, char **, s21_flags_t, s21_size_t *));
int get_unsigned_integer_representation(
    char **v, va_list *values, s21_flags_t flags, s21_size_t *length,
    char length_modifier,
    int (*shortf)(unsigned short, char **, s21_flags_t, s21_size_t *),
    int (*normalf)(unsigned int, char **, s21_flags_t, s21_size_t *),
    int (*longf)(unsigned long, char **, s21_flags_t, s21_size_t *));
int get_double_representation(char **v, va_list *values, s21_flags_t flags,
                              s21_size_t *length, char length_modifier,
                              int (*normalf)(double, char **, s21_flags_t,
                                             s21_size_t *),
                              int (*longf)(long double, char **, s21_flags_t,
                                           s21_size_t *));

// Auxiliary functions
void add_flag(s21_flags_t *flags, char flag);
void add_width(s21_flags_t *flags, const char **spec, va_list *values);
void add_precision(s21_flags_t *flags, const char **spec, va_list *values);
s21_size_t s21_atosz(const char *s, const char **end);

int s21_sprintf(char *str, const char *format, ...) {
  va_list values;
  va_start(values, format);

  int length = 0;

  while (*format && length >= 0) {
    if ('%' == *format) {
      int result = process_specifier(&str, &format, &values, length);
      if (result >= 0) {
        length += result;
      } else {
        length = -1;
      }
    } else {
      *str = *format;
      ++str;
      ++length;
    }
    ++format;
  }

  *str = '\0';
  va_end(values);
  return length;
}

int process_specifier(char **str, const char **format, va_list *values,
                      int length) {
  const char *spec = *format + 1;
  int spec_length = 0;
  s21_flags_stage_t current_stage = FLAGS_STAGE;
  char length_modifier = 0;
  char specifier = 0;
  s21_flags_t flags = {0};
  while (current_stage != SPECIFIER_STAGE && current_stage != ERROR_STAGE) {
    const char *flag = s21_NULL;
    if (FLAGS_STAGE == current_stage &&
        s21_NULL != (flag = s21_strchr(s21_sprintf_flags, *spec))) {
      add_flag(&flags, *flag);
      ++spec;
    } else if (current_stage < WIDTH_STAGE &&
               ((*spec >= '1' && *spec <= '9') || '*' == *spec)) {
      current_stage = WIDTH_STAGE;
      add_width(&flags, &spec, values);
    } else if (current_stage < PRECISION_STAGE && '.' == *spec) {
      current_stage = PRECISION_STAGE;
      add_precision(&flags, &spec, values);
    } else if (current_stage < LENGTH_STAGE &&
               s21_NULL !=
                   (flag = s21_strchr(s21_sprintf_length_descs, *spec))) {
      current_stage = LENGTH_STAGE;
      length_modifier = *flag;
      ++spec;
    } else if (s21_NULL != (flag = s21_strchr(s21_sprintf_specifiers, *spec))) {
      current_stage = SPECIFIER_STAGE;
      specifier = *flag;
      ++spec;
    } else {
      current_stage = ERROR_STAGE;
    }
  }
  *format = spec - 1;
  if (ERROR_STAGE == current_stage) {
    spec_length = 1;
    **str = '%';
    ++(*str);
  } else {
    spec_length = s21_sprint_value(str, values, flags, length_modifier,
                                   specifier, length);
  }
  return spec_length;
}

void add_flag(s21_flags_t *flags, char flag) {
  if ('-' == flag) {
    flags->bitflags |= LEFT_JUSTIFY;
  } else if ('+' == flag) {
    flags->bitflags |= FORCE_SIGN;
  } else if (' ' == flag) {
    flags->bitflags |= SPACE_BEFORE_VALUE;
  } else if ('#' == flag) {
    flags->bitflags |= SHARP;
  } else if ('0' == flag) {
    flags->bitflags |= ZEROES_INSTEAD_SPACES;
  }
}

void add_width(s21_flags_t *flags, const char **spec, va_list *values) {
  flags->bitflags |= WIDTH;
  if ('*' == **spec) {
    int arg = va_arg(*values, int);
    if (arg < 0) {
      flags->bitflags |= LEFT_JUSTIFY;
      arg = -arg;
    }
    flags->width = (s21_size_t)arg;
    ++(*spec);
  } else {
    const char *end = *spec;
    flags->width = s21_atosz(*spec, &end);
    *spec = end;
  }
}

void add_precision(s21_flags_t *flags, const char **spec, va_list *values) {
  flags->bitflags |= PRECISION;
  ++(*spec);
  if ('*' == **spec) {
    flags->precision = va_arg(*values, s21_size_t);
    ++(*spec);
  } else if (**spec >= '0' && **spec <= '9') {
    const char *end = *spec;
    flags->precision = s21_atosz(*spec, &end);
    *spec = end;
  } else {
    flags->precision = 0;
  }
}

s21_size_t s21_atosz(const char *s, const char **end) {
  s21_size_t result = 0;
  while ('0' <= *s && *s <= '9') {
    result = 10 * result + *s - '0';
    ++s;
  }
  *end = s;
  return result;
}

int s21_sprint_value(char **str, va_list *values, s21_flags_t flags,
                     char length_modifier, char specifier, int length) {
  char *v = s21_NULL;
  int result = get_value_representation(&v, values, flags, length_modifier,
                                        specifier, length);
  if (result >= 0) {
    if (v && result > 0) {
      s21_strcpy(*str, v);
      (*str) += result;
    }
    if (v) {
      free(v);
    }
  }
  return result;
}

int get_value_representation(char **v, va_list *values, s21_flags_t flags,
                             char length_modifier, char specifier,
                             int main_length) {
  int error = 0;
  s21_size_t length = 0;
  if ('c' == specifier) {
    if ('l' == length_modifier) {
      wchar_t value = va_arg(*values, wchar_t);
      error = lc_sprint(value, v, flags, &length);
    } else {
      char value = va_arg(*values, int);
      error = c_sprint(value, v, flags, &length);
    }
  } else if ('d' == specifier || 'i' == specifier) {
    error = get_signed_integer_representation(v, values, flags, &length,
                                              length_modifier, hd_sprint,
                                              d_sprint, ld_sprint);
  } else if ('e' == specifier) {
    error = get_double_representation(v, values, flags, &length,
                                      length_modifier, e_sprint, Le_sprint);
  } else if ('E' == specifier) {
    error = get_double_representation(v, values, flags, &length,
                                      length_modifier, E_sprint, LE_sprint);
  } else if ('f' == specifier) {
    error = get_double_representation(v, values, flags, &length,
                                      length_modifier, f_sprint, Lf_sprint);
  } else if ('g' == specifier) {
    error = get_double_representation(v, values, flags, &length,
                                      length_modifier, g_sprint, Lg_sprint);
  } else if ('G' == specifier) {
    error = get_double_representation(v, values, flags, &length,
                                      length_modifier, G_sprint, LG_sprint);
  } else if ('o' == specifier) {
    error = get_unsigned_integer_representation(v, values, flags, &length,
                                                length_modifier, ho_sprint,
                                                o_sprint, lo_sprint);
  } else if ('s' == specifier) {
    if ('l' == length_modifier) {
      wchar_t *value = va_arg(*values, wchar_t *);
      error = ls_sprint(value, v, flags, &length);
    } else {
      char *value = va_arg(*values, char *);
      error = s_sprint(value, v, flags, &length);
    }
  } else if ('u' == specifier) {
    error = get_unsigned_integer_representation(v, values, flags, &length,
                                                length_modifier, hu_sprint,
                                                u_sprint, lu_sprint);
  } else if ('x' == specifier) {
    error = get_unsigned_integer_representation(v, values, flags, &length,
                                                length_modifier, hx_sprint,
                                                x_sprint, lx_sprint);
  } else if ('X' == specifier) {
    error = get_unsigned_integer_representation(v, values, flags, &length,
                                                length_modifier, hX_sprint,
                                                X_sprint, lX_sprint);
  } else if ('p' == specifier) {
    void *value = va_arg(*values, void *);
    error = p_sprint(value, v, flags, &length);
  } else if ('n' == specifier) {
    int *value = va_arg(*values, int *);
    error = n_sprint(value, &length, main_length);
  } else if ('%' == specifier) {
    error = percent_sprint(v, &length);
  }
  return error ? -1 : (int)length;
}

int get_signed_integer_representation(
    char **v, va_list *values, s21_flags_t flags, s21_size_t *length,
    char length_modifier,
    int (*shortf)(short, char **, s21_flags_t, s21_size_t *),
    int (*normalf)(int, char **, s21_flags_t, s21_size_t *),
    int (*longf)(long, char **, s21_flags_t, s21_size_t *)) {
  int error = 0;
  if ('l' == length_modifier) {
    long value = va_arg(*values, long);
    error = longf(value, v, flags, length);
  } else if ('h' == length_modifier) {
    short value = va_arg(*values, int);
    error = shortf(value, v, flags, length);
  } else {
    int value = va_arg(*values, int);
    error = normalf(value, v, flags, length);
  }
  return error;
}

int get_unsigned_integer_representation(
    char **v, va_list *values, s21_flags_t flags, s21_size_t *length,
    char length_modifier,
    int (*shortf)(unsigned short, char **, s21_flags_t, s21_size_t *),
    int (*normalf)(unsigned int, char **, s21_flags_t, s21_size_t *),
    int (*longf)(unsigned long, char **, s21_flags_t, s21_size_t *)) {
  int error = 0;
  if ('l' == length_modifier) {
    unsigned long value = va_arg(*values, unsigned long);
    error = longf(value, v, flags, length);
  } else if ('h' == length_modifier) {
    unsigned short value = va_arg(*values, unsigned int);
    error = shortf(value, v, flags, length);
  } else {
    unsigned int value = va_arg(*values, unsigned int);
    error = normalf(value, v, flags, length);
  }
  return error;
}

int get_double_representation(char **v, va_list *values, s21_flags_t flags,
                              s21_size_t *length, char length_modifier,
                              int (*normalf)(double, char **, s21_flags_t,
                                             s21_size_t *),
                              int (*longf)(long double, char **, s21_flags_t,
                                           s21_size_t *)) {
  int error = 0;
  if ('L' == length_modifier) {
    long double value = va_arg(*values, long double);
    error = longf(value, v, flags, length);
  } else {
    double value = va_arg(*values, double);
    error = normalf(value, v, flags, length);
  }
  return error;
}
