#include "../s21_string.h"

void *s21_to_upper(const char *str) {
  char *result_str = s21_NULL;
  s21_size_t len = str ? s21_strlen(str) : 0;
  if (str) result_str = (char *)calloc((len + 1), sizeof(char));
  if (result_str && len) {
    for (s21_size_t i = 0; *(str + i); ++i)
      result_str[i] = (str[i] > 96 && str[i] < 123) ? str[i] - 32 : str[i];
  }

  return result_str;
}

void *s21_to_lower(const char *str) {
  char *result_str = s21_NULL;
  s21_size_t len = str ? s21_strlen(str) : 0;
  if (str) result_str = (char *)calloc((len + 1), sizeof(char));
  if (result_str && len) {
    for (s21_size_t i = 0; *(str + i); ++i)
      result_str[i] = (str[i] > 64 && str[i] < 91) ? str[i] + 32 : str[i];
  }

  return result_str;
}

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *result_str = s21_NULL;
  s21_size_t len_src = src ? s21_strlen(src) : 0;
  s21_size_t len_str = str ? s21_strlen(str) : 0;
  if (start_index <= len_src && src && str) {
    result_str = (char *)calloc(len_src + len_str + 1, sizeof(char));
    if (result_str) {
      for (s21_size_t i = 0, j = 0, l = 0; i < len_src + len_str;) {
        if (i == start_index && *(str + j))
          for (; *(str + j); ++j) result_str[i++] = str[j];
        else
          result_str[i++] = src[l++];
      }
    }
  }
  return result_str;
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *result_str = s21_NULL;
  s21_size_t len = src ? s21_strlen(src) : 0;
  if (src) result_str = (char *)calloc((len + 1), sizeof(char));
  if (result_str && trim_chars) {
    s21_size_t i = 0, j = 0;
    for (; i < len && s21_strchr(trim_chars, src[i]); ++i)
      ;
    for (; i < len; ++j) result_str[j] = src[i++];

    for (; j != 0 && s21_strchr(trim_chars, result_str[--j]) && j < len;)
      result_str[j] = '\0';
  }
  return result_str;
}
