#include <math.h>
#include <stdio.h>

#include "../../s21_string.h"
#include "../s21_format.h"
#include "s21_sprintf.h"

#define EPS 10e-47
#define EPSI(x) powl(10., -1. * (x))

/* ===    Cores   === */
int Lf_core(long double value, char **v, s21_flags_t flags, s21_size_t *length,
            const int G);
int Le_core(long double value, char **v, s21_flags_t flags, s21_size_t *length,
            const int E, const int G);
int Lg_core(long double value, char **v, s21_flags_t flags, s21_size_t *length,
            const int E);

/* ===   Helpers  === */
int e_string(long double *value, char **e_str, s21_size_t precision,
             const int E);
int get_exponent(long double *value, s21_size_t precision);
char *int_to_str(long double integer, s21_size_t *length);
char *dec_proc(long double decimal, s21_size_t precision, s21_size_t *length,
               s21_flags_t flags, const int G);
char *dec_to_str(long double decimal, s21_size_t precision, s21_size_t *length,
                 s21_flags_t flags, const int G);
void reverse_string(char **str);
int get_d_digit(long double *value, s21_size_t precision);
int calloc_str(char **str, s21_size_t size);
void compose_str(char **v, const int precision, char *pad_str, char *sign_str,
                 char *e_str, char *int_str, char *dec_str, s21_flags_t flags,
                 const int e, const int G);
char *fill_pad_str(s21_size_t *str_len, s21_flags_t flags);

/* ===  Wrappers  === */

int f_sprint(double value, char **v, s21_flags_t flags, s21_size_t *length) {
  return Lf_core(value, v, flags, length, 0);
}

int Lf_sprint(long double value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return Lf_core(value, v, flags, length, 0);
}

int e_sprint(double value, char **v, s21_flags_t flags, s21_size_t *length) {
  return Le_core(value, v, flags, length, 0, 0);
}

int Le_sprint(long double value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return Le_core(value, v, flags, length, 0, 0);
}

int E_sprint(double value, char **v, s21_flags_t flags, s21_size_t *length) {
  return Le_core(value, v, flags, length, 1, 0);
}

int LE_sprint(long double value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return Le_core(value, v, flags, length, 1, 0);
}

int g_sprint(double value, char **v, s21_flags_t flags, s21_size_t *length) {
  return Lg_core(value, v, flags, length, 0);
}

int Lg_sprint(long double value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return Lg_core(value, v, flags, length, 0);
}

int G_sprint(double value, char **v, s21_flags_t flags, s21_size_t *length) {
  return Lg_core(value, v, flags, length, 1);
}

int LG_sprint(long double value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return Lg_core(value, v, flags, length, 1);
}

/* ===    Cores   === */

int Lf_core(long double value, char **v, s21_flags_t flags, s21_size_t *length,
            const int G) {
  int fail = 0;
  char *sign_str = value < -EPS ? "-" : "+";
  char *int_str = s21_NULL, *dec_str = s21_NULL;
  s21_size_t str_length = 0;
  s21_size_t precision = flags.bitflags & PRECISION ? flags.precision : 6;
  long double integer = 0., decimal = 0.;

  value = value < -EPS ? value * -1. : value;
  value = roundl((value * powl(10, precision))) / powl(10, precision);
  decimal = modfl(value, &integer);
  decimal = roundl((decimal * powl(10, precision))) / powl(10, precision);

  if (s21_strchr(sign_str, '-') || flags.bitflags & FORCE_SIGN ||
      flags.bitflags & SPACE_BEFORE_VALUE)
    ++str_length;

  int_str = int_to_str(integer, &str_length);
  dec_str = dec_proc(decimal, precision, &str_length, flags, G);

  char *pad_str = fill_pad_str(&str_length, flags);
  int G_ = G && decimal < EPSI(precision);
  if (calloc_str(v, str_length + 1) && !fail) {
    compose_str(v, precision, pad_str, sign_str, s21_NULL, int_str, dec_str,
                flags, 0, G_);
    *length += str_length;
    if (int_str && s21_strcmp(int_str, "0")) free(int_str);
    if (dec_str && s21_strcmp(dec_str, "")) free(dec_str);
    if (pad_str) free(pad_str);
  } else
    fail = 1;

  return fail;
}

int Le_core(long double value, char **v, s21_flags_t flags, s21_size_t *length,
            const int E, const int G) {
  int fail = 0;
  char *sign_str = value < -EPS ? "-" : "+";
  char *e_str = s21_NULL, *int_str = s21_NULL, *dec_str = s21_NULL;
  s21_size_t str_length = 0;
  s21_size_t precision = flags.bitflags & PRECISION ? flags.precision : 6;
  long double integer = 0., decimal = 0.;

  // value = roundl((value * powl(10, precision))) / powl(10, precision);
  value = value < -EPS ? value * -1. : value;
  str_length += e_string(&value, &e_str, precision, E);
  decimal = modfl(value, &integer);
  decimal = roundl((decimal * powl(10, precision))) / powl(10, precision);

  if (s21_strchr(sign_str, '-') || flags.bitflags & FORCE_SIGN ||
      flags.bitflags & SPACE_BEFORE_VALUE)
    ++str_length;

  int_str = int_to_str(integer, &str_length);
  dec_str = dec_proc(decimal, precision, &str_length, flags, G);

  char *pad_str = fill_pad_str(&str_length, flags);
  int G_ = G && decimal < EPSI(precision);
  if (calloc_str(v, str_length + 1) && !fail) {
    compose_str(v, precision, pad_str, sign_str, e_str, int_str, dec_str, flags,
                1, G_);
    *length += str_length;
    if (int_str && s21_strcmp(int_str, "0")) free(int_str);
    if (dec_str && s21_strcmp(dec_str, "")) free(dec_str);
    if (e_str) free(e_str);
    if (pad_str) free(pad_str);
  } else
    fail = 1;

  return fail;
}

int Lg_core(long double value, char **v, s21_flags_t flags, s21_size_t *length,
            const int E) {
  s21_size_t precision = flags.bitflags & PRECISION
                             ? (flags.precision >= 1 ? flags.precision : 1)
                             : 6;
  if (!(flags.bitflags & PRECISION)) flags.bitflags += 64;
  int result = 0;

  long double value_e = value;
  // value_e = roundl((value_e * powl(10, precision - 1))) / powl(10, precision
  // - 1);
  value_e = value_e < -EPS ? value_e * -1. : value_e;
  int expo = get_exponent(&value_e, precision - 1);
  expo = value_e < 1. ? expo * -1. : expo;

  if (expo >= -4 && expo < (int)precision) {
    s21_flags_t g_flags = {flags.bitflags, precision - 1 - expo, flags.width};
    result = Lf_core(value, v, g_flags, length, 1);
  } else {
    s21_flags_t g_flags = {flags.bitflags, precision - 1, flags.width};
    result = Le_core(value, v, g_flags, length, E, 1);
  }

  return result;
}

/* ===   Helpers  === */

int e_string(long double *value, char **e_str, s21_size_t precision,
             const int E) {
  int sign_str = *value < 1. ? 0 : 1;
  sign_str = *value < EPS ? 1 : sign_str;
  int exponent = get_exponent(value, precision);

  s21_size_t e_str_l = exponent < 100 ? 4 : 3 + (int)log10(exponent);
  *e_str = (char *)calloc(e_str_l + 1, sizeof(char));
  (*e_str)[0] = E ? 'E' : 'e';
  (*e_str)[1] = sign_str ? '+' : '-';

  if (exponent > 9) {
    s21_size_t dummy = 0;
    char *e_pwr = int_to_str(exponent, &dummy);
    s21_strcat(*e_str, e_pwr);
    free(e_pwr);
  } else {
    (*e_str)[2] = '0';
    (*e_str)[3] = exponent + 48;
  }
  return e_str_l;
}

int get_exponent(long double *value, s21_size_t precision) {
  int exponent = 0;

  for (; (*value < 1. || *value >= 10.) && *value > EPS; ++exponent)
    *value = *value < 1. ? *value * 10. : *value / 10.;
  *value = roundl((*value * powl(10, precision))) / powl(10, precision);

  return exponent;
}

char *int_to_str(long double integer, s21_size_t *length) {
  char *int_str = s21_NULL;
  if (integer > 0.9) {
    s21_size_t str_len = log10l(integer) + 1;
    int_str = (char *)calloc(str_len + 1, sizeof(char));
    long double divider = 10.;
    for (s21_size_t i = 0; i < str_len; ++i, ++*length) {
      int_str[i] = (int)(fmodl(integer, divider) / (divider / 10.)) + 48;
      divider *= 10.;
    }
    reverse_string(&int_str);
  } else {
    int_str = "0";
    ++*length;
  }
  return int_str;
}

void reverse_string(char **str) {
  s21_size_t len = s21_strlen(*str);
  for (s21_size_t i = 0, j = len - 1; i < j; ++i, --j) {
    char tmp = (*str)[i];
    (*str)[i] = (*str)[j];
    (*str)[j] = tmp;
  }
}

char *dec_proc(long double decimal, s21_size_t precision,
               s21_size_t *str_length, s21_flags_t flags, const int G) {
  char *dec_str = s21_NULL;
  int G_ = G && decimal < EPSI(precision);
  if (G_ && !(flags.bitflags & SHARP)) {
    dec_str = "";
  } else if (precision > 0) {
    ++*str_length;
    return dec_to_str(decimal, precision, str_length, flags, G);
  } else if (flags.bitflags & SHARP) {
    dec_str = "";
    ++*str_length;
  }

  return dec_str;
}

char *dec_to_str(long double decimal, s21_size_t precision, s21_size_t *length,
                 s21_flags_t flags, const int G) {
  char *dec_str = (char *)calloc(256, sizeof(char));
  s21_size_t i = 0;
  if (decimal > EPS)
    for (; decimal > EPS * powl(10., i) && i < precision; ++i, ++*length)
      dec_str[i] = get_d_digit(&decimal, precision) + 48;
  else {
    dec_str[i++] = '0';
    ++*length;
  }
  if (!G || (G && flags.bitflags & SHARP))
    for (; i < precision; ++i, ++*length) dec_str[i] = '0';
  return dec_str;
}

int get_d_digit(long double *value, s21_size_t precision) {
  long double integer = 0.;
  *value *= 10.;
  *value = modfl(*value, &integer);
  *value = roundl((*value * powl(10, precision))) / powl(10, precision);
  return (int)integer;
}

int calloc_str(char **str, s21_size_t size) {
  int success = 1;
  *str = (char *)calloc(sizeof(char), size);
  if (*str == s21_NULL) success = 0;
  return success;
}

void compose_str(char **v, const int precision, char *pad_str, char *sign_str,
                 char *e_str, char *int_str, char *dec_str, s21_flags_t flags,
                 const int e, const int G) {
  int right = pad_str && !(flags.bitflags & LEFT_JUSTIFY);

  if (!(flags.bitflags & ZEROES_INSTEAD_SPACES) && right)
    s21_strcat(*v, pad_str);

  if (s21_strchr(sign_str, '-') || flags.bitflags & FORCE_SIGN) {
    s21_strcat(*v, sign_str);
    if (flags.bitflags & ZEROES_INSTEAD_SPACES && right)
      s21_strcat(*v, pad_str);
  } else if (flags.bitflags & SPACE_BEFORE_VALUE) {
    s21_strcat(*v, " ");
    if (flags.bitflags & ZEROES_INSTEAD_SPACES && right)
      s21_strcat(*v, pad_str);
  } else if (flags.bitflags & ZEROES_INSTEAD_SPACES && right)
    s21_strcat(*v, pad_str);

  s21_strcat(*v, int_str);
  if (!G || flags.bitflags & SHARP)
    if (precision > 0 || flags.bitflags & SHARP) {
      s21_strcat(*v, ".");
      s21_strcat(*v, dec_str);
    }
  if (e) s21_strcat(*v, e_str);
  if (pad_str && flags.bitflags & LEFT_JUSTIFY) s21_strcat(*v, pad_str);
}

char *fill_pad_str(s21_size_t *str_len, s21_flags_t flags) {
  char *pad_str = s21_NULL;
  if (flags.bitflags & WIDTH && flags.width > *str_len) {
    s21_size_t pad_len = flags.width - *str_len;
    *str_len += pad_len;
    calloc_str(&pad_str, pad_len + 1);

    for (s21_size_t i = 0; i < pad_len; ++i)
      if (flags.bitflags & ZEROES_INSTEAD_SPACES)
        pad_str[i] = '0';
      else
        pad_str[i] = ' ';
  }
  return pad_str;
}
