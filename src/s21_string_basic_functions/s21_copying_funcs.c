#include "../s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *d = (char *)dest;
  const char *s = (const char *)src;
  for (; n != 0; d++, s++, n--) {
    (*d) = (*s);
  }
  return dest;
}

void *s21_memmove(void *dest, const void *src, s21_size_t n) {
  char *d = (char *)dest;
  const char *s = (const char *)src;
  if (src < dest) {
    for (; n != 0; n--) {
      *(d + n - 1) = *(s + n - 1);
    }
  } else {
    for (; n != 0; d++, s++, n--) {
      *d = *s;
    }
  }
  return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  char *s = (char *)str;
  for (; n != 0; n--, s++) {
    *s = c;
  }
  return str;
}

char *s21_strcpy(char *dest, const char *src) {
  char *copy_dest = dest;
  for (; *src; src++, copy_dest++) {
    *copy_dest = *src;
  }
  *copy_dest = '\0';
  return dest;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *copy_dest = dest;
  if (n) {
    for (; *src && n; src++, copy_dest++, n--) {
      *copy_dest = *src;
    }
  }
  for (; n; n--, copy_dest++) {
    *copy_dest = '\0';
  }
  return dest;
}
