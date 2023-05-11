#include "../s21_string.h"

s21_size_t s21_strlen(const char *str) {
  s21_size_t len = 0;
  for (; *(str + len); ++len)
    ;
  return len;
}

s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t len = 0;
  int match = 1;
  while (match && *(str1 + len)) {
    match = 0;
    for (s21_size_t i = 0; *(str2 + i); ++i) {
      if (*(str1 + len) == *(str2 + i)) {
        match = 1;
        ++len;
        break;
      }
    }
  }
  return len;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t len = 0;
  int match = 0;
  while (!match && *(str1 + len)) {
    for (s21_size_t i = 0; !match && *(str2 + i); ++i) {
      if (*(str1 + len) == *(str2 + i)) match = 1;
    }
    if (!match) ++len;
  }
  return len;
}
