#include "../s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int result = 0;
  if (n != 0) {
    const unsigned char *c1 = str1, *c2 = str2;
    while (n-- != 0)
      if (*c1++ != *c2++) {
        result = *--c1 - *--c2;
        break;
      }
  }
  return result;
}

int s21_strcmp(const char *str1, const char *str2) {
  int result = 0;
  do {
    result = *str1 - *str2;
    if (!*str1) break;
  } while (*str1++ == *str2++);
  return result;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int result = 0;
  if (n != 0) {
    while (n-- != 0)
      if (*str1++ != *str2++) {
        result = *--str1 - *--str2;
        break;
      }
  }
  return result;
}
