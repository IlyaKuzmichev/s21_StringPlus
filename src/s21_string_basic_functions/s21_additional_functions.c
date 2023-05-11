#include "../s21_string.h"

#define BUFF_SIZE 128

char *s21_strcat(char *dest, const char *src) {
  char *temp = dest + s21_strlen(dest);

  for (; *src; src++) {
    *temp = *src;
    temp++;
  }
  *temp = '\0';
  return dest;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *temp = dest + s21_strlen(dest);

  for (; (*src) && (n--); src++, temp++) {
    *temp = *src;
  }
  *temp = '\0';
  return dest;
}

char *s21_strerror(int errnum) {
  static char *strerr[] = s21_error;
  static char s21_undef[BUFF_SIZE] = {'\0'};
  char *res = s21_NULL;

  if (errnum < 0 || errnum >= S21_ERRLIST_SIZE) {
    s21_sprintf(s21_undef, "Unknown error: %d", errnum);
    res = s21_undef;
  } else {
    res = strerr[errnum];
  }
  return res;
}

int is_delim(char sym, const char *delim) {
  int delim_flag = 0;

  while (!delim_flag && (*delim != '\0')) {
    if (sym == *delim) {
      delim_flag = 1;
    }
    delim++;
  }
  return delim_flag;
}

char *s21_strtok(char *str, const char *delim) {
  static char *next_string;
  char *return_string;
  if (!str) {
    str = next_string;
  }
  while ((*str != '\0') && is_delim(*str, delim)) {
    str++;
  }
  if (*str != '\0') {
    return_string = str;
    while (*str != '\0' && !is_delim(*str, delim)) {
      if (*(str + 1) == '\0') {
        next_string = str + 1;
      }
      if (is_delim(*(str + 1), delim)) {
        *(str + 1) = '\0';
        next_string = str + 2;
      }
      str++;
    }
  } else {
    return_string = s21_NULL;
  }
  return return_string;
}
