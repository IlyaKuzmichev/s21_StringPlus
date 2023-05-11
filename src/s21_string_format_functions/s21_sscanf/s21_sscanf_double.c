#include <math.h>

#include "s21_sscanf.h"

int double_sscan(long double *value, const char **s, s21_size_t width);

char *read_sign(char *str, s21_size_t *width, int *is_sign);
int is_not_valid_symbol(char symbol, char prev, int is_mantissa);
void s21_atoi(long double *value, char str);
void make_value(long double *value, int is_sign, int is_mantissa,
                long double int_part, long double fract_part,
                long double exp_part, int preceding_zeroes);

char *read_sign(char *str, s21_size_t *width, int *is_sign) {
  if (*str == '-') {
    *is_sign = 1;
    str++;
    (*width)--;
  } else if (*str == '+') {
    str++;
    (*width)--;
  }
  return str;
}

int is_not_valid_symbol(char symbol, char prev, int is_mantissa) {
  int is_valid = 0;
  if ((symbol >= '0' && symbol <= '9') || symbol == '.' || symbol == 'e' ||
      symbol == 'E' || symbol == '+' || symbol == '-') {
    if ((symbol == 'e' || symbol == 'E' || symbol == '.') &&
        !(prev >= '0' && prev <= '9')) {
      is_valid = 1;
    }
    if ((symbol == '+' || symbol == '-') && !(prev == 'e' || prev == 'E')) {
      is_valid = 1;
    }
    if (is_mantissa && symbol == '.') {
      is_valid = 2;
    }
    if (is_mantissa > 1 && (symbol == 'e' || symbol == 'E')) {
      is_valid = 2;
    }
  } else {
    is_valid = 1;
  }
  return is_valid;
}

void s21_atoi(long double *value, char str) {
  *value *= 10;
  *value += (str - '0');
}

void make_value(long double *value, int is_sign, int is_mantissa,
                long double int_part, long double fract_part,
                long double exp_part, int preceding_zeroes) {
  long double result = int_part;
  while ((int)fract_part != 0) {
    fract_part /= 10;
  }
  fract_part /= pow(10, (double)preceding_zeroes);
  result += fract_part;
  if (is_mantissa == 2) {
    result *= pow(10, (double)exp_part);
  } else if (is_mantissa == 3) {
    result /= pow(10, (double)exp_part);
  }

  if (is_sign) {
    result *= -1;
  }
  *value = result;
}

int double_sscan(long double *value, const char **s, s21_size_t width) {
  int is_parse_int = 0;
  int is_parse_fract = 0;
  int is_sign = 0, is_mantissa = 0;  // 1- . 2 + e 3 -e
  width = (width == 0) ? (s21_size_t)-1 : width;
  char *str = (char *)*s;
  long double int_part = 0, fract_part = 0, exp_part = 0;
  int preceding_zeroes = 0;

  str = read_sign(str, &width, &is_sign);
  char prev = '0';

  while (width != 0 && !is_not_valid_symbol(*str, prev, is_mantissa)) {
    switch (*str) {
      case '.':
        is_mantissa = 1;
        break;
      case 'e':
      case 'E':
        is_mantissa = 2;
        break;
      case '+':
        break;
      default:
        if (!is_mantissa) {
          s21_atoi(&int_part, *str);
          is_parse_int = 1;
        } else if (is_mantissa == 1) {
          if (fract_part == 0 && *str == '0') {
            preceding_zeroes++;
          } else {
            s21_atoi(&fract_part, *str);
            is_parse_fract = 1;
          }
        } else {
          if (*str == '-') {
            is_mantissa = 3;
          } else if (!(*str == '0' && exp_part == 0)) {
            s21_atoi(&exp_part, *str);
            is_parse_fract = 1;
          }
        }
    }

    prev = *str;
    str++;
    width--;
  }
  *s = str;
  make_value(value, is_sign, is_mantissa, int_part, fract_part, exp_part,
             preceding_zeroes);
  int err = 1;
  if (is_parse_int || is_parse_fract) {
    err = 0;
  }
  if ((is_not_valid_symbol(*str, prev, is_mantissa) == 2 && *str != '.') ||
      ((is_mantissa == 2 || is_mantissa == 3) && exp_part == 0)) {
    err = 2;
  }
  if ((is_mantissa == 2 || is_mantissa == 3) && !is_parse_fract &&
      !is_parse_int) {
    err = 1;
  }
  return err;
}

int f_sscan(float *value, const char **s, s21_size_t width) {
  long double temp;
  int err = double_sscan(&temp, s, width);
  *value = (float)temp;
  return err;
}

int lf_sscan(double *value, const char **s, s21_size_t width) {
  long double temp;
  int err = double_sscan(&temp, s, width);
  *value = (double)temp;
  return err;
}

int Lf_sscan(long double *value, const char **s, s21_size_t width) {
  return double_sscan(value, s, width);
}
