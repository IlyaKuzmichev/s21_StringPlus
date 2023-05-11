#include "s21_sscanf.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

#define ERROR_CODE 1
#define VALUE_WRITTEN 0
#define VALUE_WRITTEN_BUT 2
#define VALUE_SUPPRESSION -1

typedef enum s21_flags_stage_t {
  WIDTH_STAGE,
  LENGTH_STAGE,
  SPECIFIER_STAGE,
  ERROR_STAGE
} s21_flags_stage_t;

static s21_size_t length = 0;

static const char *s21_sscanf_length_descs = "hlL";
static const char *s21_sscanf_specifiers = "cdieEfgGosuxXpn%";

int s21_sscanf(const char *str, const char *format, ...);
int specifier_sscan(const char **str, const char **format, va_list *values,
                    int *fields_quantity);
int add_width_sscan(s21_size_t *width_spec, const char **spec);
int s21_atobus(const char **str);
int s21_sscan_value(const char **str, va_list *values, char length_modifier,
                    char specifier, s21_size_t width_spec,
                    int *flag_suppression, int *fields_quantity);

int s21_sscanf(const char *str, const char *format, ...) {
  va_list values;
  va_start(values, format);
  int fields_quantity = 0;
  int exit_flag = 1;

  length = 0;
  if (*str == EOF || 0 == *str) {
    fields_quantity = EOF;
  }
  while (*format && exit_flag && fields_quantity >= 0) {
    while (' ' == *format || '\t' == *format || '\n' == *format) {
      format++;
      while (*str && (' ' == *str || '\t' == *str || '\n' == *str)) {
        str++;
        length++;
      }
    }
    if ('%' == *format) {
      int result = specifier_sscan(&str, &format, &values, &fields_quantity);
      if (result % 2 == 0 && -1 != fields_quantity) {
        fields_quantity++;
        if (result == VALUE_WRITTEN_BUT) exit_flag = 0;
      } else if (result == VALUE_SUPPRESSION) {
      } else if (result == ERROR_CODE) {
        exit_flag = 0;
      }
    } else if (*str == *format) {
      str++;
      format++;
      length++;
    } else {
      exit_flag = 0;
    }
  }

  va_end(values);
  return fields_quantity;
}

int specifier_sscan(const char **str, const char **format, va_list *values,
                    int *fields_quantity) {
  const char *spec = *format + 1;
  int result = 0;
  s21_flags_stage_t current_stage = WIDTH_STAGE;
  char length_modifier = 0;
  char specifier = 0;
  s21_size_t width_spec = 0;
  int flag_suppression = 0;

  while (current_stage != SPECIFIER_STAGE && current_stage != ERROR_STAGE) {
    const char *flag = s21_NULL;
    if (current_stage == WIDTH_STAGE &&
        ((*spec >= '0' && *spec <= '9') || '*' == *spec)) {
      int temp = add_width_sscan(&width_spec, &spec);
      if (!flag_suppression) {
        flag_suppression = temp;
      }
    } else if ((current_stage < LENGTH_STAGE) &&
               s21_NULL !=
                   (flag = s21_strchr(s21_sscanf_length_descs, *spec))) {
      current_stage = LENGTH_STAGE;
      length_modifier = *flag;
      spec++;
    } else if (s21_NULL != (flag = s21_strchr(s21_sscanf_specifiers, *spec))) {
      current_stage = SPECIFIER_STAGE;
      specifier = *flag;
      ++spec;
    } else {
      current_stage = ERROR_STAGE;
    }
  }
  *format = spec;
  if (current_stage == ERROR_STAGE) {
    result = ERROR_CODE;
  } else {
    result = s21_sscan_value(str, values, length_modifier, specifier,
                             width_spec, &flag_suppression, fields_quantity);
    if (result == VALUE_WRITTEN && flag_suppression == VALUE_SUPPRESSION) {
      result = VALUE_SUPPRESSION;
    }
  }
  return result;
}

int add_width_sscan(s21_size_t *width_spec, const char **spec) {
  int flag_suppression = 0;
  if ('*' == **spec) {
    flag_suppression = VALUE_SUPPRESSION;
    (*spec)++;
  } else {
    *width_spec = s21_atobus(spec);
  }
  return flag_suppression;
}

int s21_atobus(const char **str) {
  int result = 0;
  while ('0' <= **str && **str <= '9') {
    result = 10 * result + **str - '0';
    (*str)++;
  }
  return result;
}

int s21_sscan_value(const char **str, va_list *values, char length_modifier,
                    char specifier, s21_size_t width_spec,
                    int *flag_suppression, int *fields_quantity) {
  int result = ERROR_CODE;
  void *value_pointer = s21_NULL;
  long double suppression_value = 0;

  if ('%' != specifier && *flag_suppression != VALUE_SUPPRESSION) {
    value_pointer = va_arg(*values, void *);
  } else {
    value_pointer = &suppression_value;
  }
  if ('c' != specifier) {
    while (**str && (' ' == **str || '\t' == **str || '\n' == **str)) {
      (*str)++;
    }
    if (!(**str) && !(*fields_quantity) && 'n' != specifier) {
      *fields_quantity = -1;
    }
  }
  const char *temp = *str;
  if ('c' == specifier && **str) {
    if (VALUE_SUPPRESSION == *flag_suppression && width_spec) {
      for (s21_size_t i = 0; i < width_spec; i++) (*str)++;
    } else if ('l' == length_modifier) {
      result = lc_sscan((wchar_t *)value_pointer, str, width_spec);
    } else {
      result = c_sscan((char *)value_pointer, str, width_spec);
    }
  } else if ('d' == specifier && **str) {
    if ('h' == length_modifier) {
      result = hd_sscan((short int *)value_pointer, str, width_spec);
    } else if ('l' == length_modifier) {
      result = ld_sscan((long int *)value_pointer, str, width_spec);
    } else {
      result = d_sscan((int *)value_pointer, str, width_spec);
    }
  } else if ('i' == specifier && **str) {
    if ('h' == length_modifier) {
      result = hi_sscan((short int *)value_pointer, str, width_spec);
    } else if ('l' == length_modifier) {
      result = li_sscan((long int *)value_pointer, str, width_spec);
    } else {
      result = i_sscan((int *)value_pointer, str, width_spec);
    }
  } else if (('e' == specifier || 'E' == specifier || 'f' == specifier ||
              'g' == specifier || 'G' == specifier) &&
             **str) {
    if ('L' == length_modifier) {
      result = Lf_sscan((long double *)value_pointer, str, width_spec);
    } else if ('l' == length_modifier) {
      result = lf_sscan((double *)value_pointer, str, width_spec);
    } else {
      result = f_sscan((float *)value_pointer, str, width_spec);
    }
  } else if ('s' == specifier && **str) {
    if (VALUE_SUPPRESSION == *flag_suppression) {
      char infinite = width_spec == 0;
      while (**str && !isspace(**str) && (infinite || width_spec > 0)) {
        (*str)++;
        width_spec--;
      }
    } else if ('l' == length_modifier && **str) {
      result = ls_sscan((wchar_t *)value_pointer, str, width_spec);
    } else {
      result = s_sscan((char *)value_pointer, str, width_spec);
    }
  } else if (('x' == specifier || 'X' == specifier) && **str) {
    if ('h' == length_modifier) {
      result = hx_sscan((unsigned short int *)value_pointer, str, width_spec);
    } else if ('l' == length_modifier) {
      result = lx_sscan((unsigned long int *)value_pointer, str, width_spec);
    } else {
      result = x_sscan((unsigned int *)value_pointer, str, width_spec);
    }
  } else if ('u' == specifier && **str) {
    if ('h' == length_modifier) {
      result = hu_sscan((unsigned short int *)value_pointer, str, width_spec);
    } else if ('l' == length_modifier) {
      result = lu_sscan((unsigned long int *)value_pointer, str, width_spec);
    } else {
      result = u_sscan((unsigned int *)value_pointer, str, width_spec);
    }
  } else if ('o' == specifier && **str) {
    if ('h' == length_modifier) {
      result = ho_sscan((unsigned short int *)value_pointer, str, width_spec);
    } else if ('l' == length_modifier) {
      result = lo_sscan((unsigned long int *)value_pointer, str, width_spec);
    } else {
      result = o_sscan((unsigned int *)value_pointer, str, width_spec);
    }
  } else if ('p' == specifier && **str) {
    result = p_sscan((void **)value_pointer, str, width_spec);
  } else if ('n' == specifier) {
    *flag_suppression = VALUE_SUPPRESSION;
    result = n_sscan((int *)value_pointer, length);
  } else if ('%' == specifier && **str) {
    result = percent_sscan(str);
  }
  length += (s21_size_t)(*str - temp);

  return result;
}

int n_sscan(int *value, s21_size_t length) {
  *value = (int)length;
  return 0;
}

int percent_sscan(const char **str) {
  int result = VALUE_SUPPRESSION;
  if ('%' == **str) {
    (*str)++;
  } else {
    result = ERROR_CODE;
  }
  return result;
}
