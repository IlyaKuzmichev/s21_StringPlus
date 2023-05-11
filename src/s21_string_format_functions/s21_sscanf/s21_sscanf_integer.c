#include <math.h>
#include <stdio.h>

#include "s21_sscanf.h"

/*   ===                  cores                 ===   */
int core_d_sscan(long int *value, const char **s, s21_size_t width);
int core_o_sscan(long int *value, const char **s, s21_size_t width);
int core_x_sscan(long int *value, const char **s, s21_size_t width);

/*   ===          decimal integer core          ===   */

int core_d_sscan(long int *value, const char **s, s21_size_t width) {
  s21_size_t i = 0;
  width = width == 0 ? i - 1 : width;
  int fail = 0, pwr = 0, sgn = 1;
  if (**s == '-' || **s == '+') {
    sgn = *(*s)++ == '-' ? -1 : 1;
    --width;
  }

  while (*(*s + i) > 47 && *(*s + i) < 58 && i < width) ++i;

  fail = i == 0 ? 1 : 0;
  while (i-- > 0 && !fail) *value += (*(*s + i) - 48) * pow(10, pwr++) * sgn;

  *s = *s + pwr;
  return fail;
}

/*   ===           octal integer core           ===   */

int core_o_sscan(long int *value, const char **s, s21_size_t width) {
  s21_size_t i = 0;
  width = width == 0 ? i - 1 : width;
  int fail = 0, pwr = 0, sgn = 1;
  if (**s == '-' || **s == '+') {
    sgn = *(*s)++ == '-' ? -1 : 1;
    --width;
  }

  while (*(*s + i) > 47 && *(*s + i) < 56 && i < width) ++i;

  fail = i == 0 ? 1 : 0;
  while (i-- > 0 && !fail) *value += (*(*s + i) - 48) * pow(8, pwr++) * sgn;

  *s = *s + pwr;
  return fail;
}

/*   ===        hexadecimal integer core        ===   */

int core_x_sscan(long int *value, const char **s, s21_size_t width) {
  s21_size_t i = 0;
  width = width == 0 ? i - 1 : width;
  int fail = 0, pwr = 0, sgn = 1;
  if (**s == '-' || **s == '+') {
    sgn = *(*s)++ == '-' ? -1 : 1;
    --width;
  }
  if (**s == '0' && (*(*s + 1) == 'x' || *(*s + 1) == 'X')) {
    *s = *s + 2;
    width = width < 2 ? 0 : width - 2;
    *value = 0;
    fail = 1;
  }

  while (((*(*s + i) > 47 && *(*s + i) < 58) ||
          (*(*s + i) > 64 && *(*s + i) < 71) ||
          (*(*s + i) > 96 && *(*s + i) < 103)) &&
         i < width)
    ++i;

  fail = (i == 0) ? fail + 1 : 0;
  while (i-- > 0 && !fail) {
    int n = *(*s + i) < 58 ? *(*s + i) - 48
                           : (*(*s + i) < 71 ? *(*s + i) - 55 : *(*s + i) - 87);
    *value += n * (long long)pow(16, pwr++) * sgn;
  }
  *s = *s + pwr;
  return fail;
}

/*   ===        unsigned decimal integer        ===   */

int hu_sscan(unsigned short *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = core_d_sscan(&tmp_value, s, width);
  *value = (unsigned short)tmp_value;

  return fail;
}

int u_sscan(unsigned int *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = core_d_sscan(&tmp_value, s, width);
  *value = (unsigned int)tmp_value;

  return fail;
}

int lu_sscan(unsigned long int *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = core_d_sscan(&tmp_value, s, width);
  *value = (unsigned long)tmp_value;

  return fail;
}

/*   ===    optionally signed decimal integer   ===   */

int hd_sscan(short *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = core_d_sscan(&tmp_value, s, width);
  *value = (short)tmp_value;

  return fail;
}

int d_sscan(int *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = core_d_sscan(&tmp_value, s, width);
  *value = (int)tmp_value;

  return fail;
}

int ld_sscan(long int *value, const char **s, s21_size_t width) {
  return core_d_sscan(value, s, width);
}

/*   ===       optionally signed integer        ===   */

int hi_sscan(short *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = li_sscan(&tmp_value, s, width);
  *value = (short)tmp_value;

  return fail;
}

int i_sscan(int *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = li_sscan(&tmp_value, s, width);
  *value = (int)tmp_value;

  return fail;
}

int li_sscan(long int *value, const char **s, s21_size_t width) {
  int fail = 0, sgn = 0;
  if (**s == '-' || **s == '+') {
    sgn = 1;
    ++*s;
  }
  if (**s == '0' && (*(*s + 1) == 'x' || *(*s + 1) == 'X')) {
    *s -= sgn;
    fail = core_x_sscan(value, s, width);
  } else if (**s == '0') {
    *s -= sgn;
    fail = core_o_sscan(value, s, width);
  } else if (**s > 47 && **s < 58) {
    *s -= sgn;
    fail = core_d_sscan(value, s, width);
  } else
    fail = 1;

  return fail;
}

/*   ===              octal integer             ===   */

int ho_sscan(unsigned short *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = core_o_sscan(&tmp_value, s, width);
  *value = (unsigned short)tmp_value;

  return fail;
}

int o_sscan(unsigned int *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = core_o_sscan(&tmp_value, s, width);
  *value = (unsigned int)tmp_value;

  return fail;
}

int lo_sscan(unsigned long int *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = core_o_sscan(&tmp_value, s, width);
  *value = (unsigned long int)tmp_value;

  return fail;
}

/*   ===           hexadecimal integer          ===   */

int hx_sscan(unsigned short *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = core_x_sscan(&tmp_value, s, width);
  *value = (unsigned short)tmp_value;

  return fail;
}

int x_sscan(unsigned int *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = core_x_sscan(&tmp_value, s, width);
  *value = (unsigned int)tmp_value;

  return fail;
}

int lx_sscan(unsigned long int *value, const char **s, s21_size_t width) {
  long int tmp_value = 0;
  int fail = core_x_sscan(&tmp_value, s, width);
  *value = (unsigned long int)tmp_value;

  return fail;
}

/*   ===          unsigned int pointer          ===   */

int p_sscan(void **value, const char **s, s21_size_t width) {
  s21_size_t i = 0;
  unsigned long long tmp = 0;
  width = width == 0 ? i - 1 : width;
  int fail = 0, pwr = 0, sgn = 1;
  if (**s == '-' || **s == '+') {
    sgn = *(*s)++ == '-' ? -1 : 1;
    --width;
  }
  if (**s == '0' && (*(*s + 1) == 'x' || *(*s + 1) == 'X')) {
    *s = *s + 2;
    width = width < 2 ? 0 : width - 2;
    *value = 0;
    fail = 1;
  }

  while (((*(*s + i) > 47 && *(*s + i) < 58) ||
          (*(*s + i) > 64 && *(*s + i) < 71) ||
          (*(*s + i) > 96 && *(*s + i) < 103)) &&
         i < width)
    ++i;

  fail = (i == 0) ? fail + 1 : 0;
  while (i-- > 0 && !fail) {
    int n = *(*s + i) < 58 ? *(*s + i) - 48
                           : (*(*s + i) < 71 ? *(*s + i) - 55 : *(*s + i) - 87);
    long long sum = n * (long long)pow(16, pwr++) * sgn;
    if ((unsigned long long)(tmp + sum) < tmp * sgn) {
      tmp = (unsigned long long)~0;
      fail = 2;
    } else {
      tmp += sum;
    }
  }
  *value = (void *)tmp;
  *s = *s + pwr;
  return fail;
}
