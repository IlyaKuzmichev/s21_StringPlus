#include <ctype.h>
#include <wchar.h>

#include "s21_sscanf.h"

enum char_scan_error { OK, MEMORY_ERROR };

int c_sscan(char *value, const char **s, s21_size_t width) {
  if (0 == width) {
    ++width;
  }
  while (width && **s) {
    *value = **s;
    ++value;
    ++(*s);
    --width;
  }
  return OK;
}

int s_sscan(char *value, const char **s, s21_size_t width) {
  char infiniteWidth = 0 == width;
  while ((infiniteWidth || width) && **s && !isspace(**s)) {
    *value = **s;
    ++value;
    ++(*s);
    --width;
  }
  *value = '\0';
  return OK;
}

int lc_sscan(wchar_t *value, const char **s, s21_size_t width) {
  if (0 == width) {
    ++width;
  }
  size_t s_size = s21_strlen(*s);
  mbstate_t state;
  s21_memset(&state, 0, sizeof(state));
  while (width && **s) {
    size_t wchar_size = mbrtowc(value, *s, s_size, &state);
    s_size -= wchar_size;
    *s += wchar_size;
    ++value;
    --width;
  }
  return OK;
}

int ls_sscan(wchar_t *value, const char **s, s21_size_t width) {
  char infiniteWidth = 0 == width;
  size_t s_size = s21_strlen(*s);
  mbstate_t state;
  s21_memset(&state, 0, sizeof(state));
  while ((infiniteWidth || width) && **s && !isspace(**s)) {
    size_t wchar_size = mbrtowc(value, *s, s_size, &state);
    s_size -= wchar_size;
    *s += wchar_size;
    ++value;
    --width;
  }
  *value = '\0';
  return OK;
}
