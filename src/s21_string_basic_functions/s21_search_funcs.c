#include "../s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  const unsigned char *walker = (const unsigned char *)str;
  void *result = s21_NULL;
  while (n && !result) {
    if ((unsigned char)c == *walker) {
      result = (void *)walker;
    }
    --n;
    ++walker;
  }
  return result;
}

char *s21_strchr(const char *str, int c) {
  char *result = s21_NULL;
  while (*str && *str != (char)c) {
    str++;
  }
  if (*str == (char)c) {
    result = (char *)str;
  }
  return result;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  s21_size_t n = s21_strlen(str2);
  char *result = s21_NULL;
  while (*str1 && !result) {
    for (s21_size_t i = 0; i < n && !result; i++) {
      if (*str1 == str2[i]) {
        result = (char *)str1;
      }
    }
    ++str1;
  }
  return result;
}

char *s21_strrchr(const char *str, int c) {
  char *result = s21_NULL;
  while (*str) {
    if (*str == (char)c) {
      result = (char *)str;
    }
    ++str;
  }
  if (*str == (char)c) {
    result = (char *)str;
  }
  return result;
}

char *s21_strstr(const char *haystack, const char *needle) {
  char *occurence = s21_NULL;
  s21_size_t haystack_len = s21_strlen(haystack);
  s21_size_t needle_len = s21_strlen(needle);
  s21_size_t haystack_shift = 0;
  s21_size_t needle_shift = 0;
  s21_size_t coincidence_counter = 0;
  while (haystack_len >= needle_len &&
         haystack_len - haystack_shift >= needle_len - coincidence_counter &&
         coincidence_counter != needle_len) {
    if (haystack[haystack_shift] == needle[needle_shift]) {
      ++coincidence_counter;
      ++needle_shift;
    } else {
      needle_shift = 0;
      coincidence_counter = (haystack[haystack_shift] == needle[needle_shift]);
    }
    ++haystack_shift;
  }
  if (coincidence_counter == needle_len) {
    occurence = (char *)haystack + haystack_shift - needle_len;
  }
  return occurence;
}
