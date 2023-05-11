#include <wchar.h>

#include "../../s21_string.h"
#include "../s21_format.h"
#include "s21_sprintf.h"

#define ERROR_MEMORY 1

void write_string(char* value, char** v, s21_flags_t flags,
                  s21_size_t value_length) {
  if (flags.bitflags & WIDTH && value_length < flags.width) {
    char filler = ' ';
    if (flags.bitflags & ZEROES_INSTEAD_SPACES &&
        !(flags.bitflags & LEFT_JUSTIFY)) {
      filler = '0';
    }
    for (s21_size_t i = 0; i < flags.width; i++) {
      *(*v + i) = filler;
    }
    if (flags.bitflags & LEFT_JUSTIFY) {
      s21_memcpy(*v, value, value_length);
    } else {
      char* s = *v;
      s += flags.width - value_length;
      s21_strncpy(s, value, value_length);
    }
  } else {
    s21_strncpy(*v, value, value_length);
  }
}

int c_sprint(char value, char** v, s21_flags_t flags, s21_size_t* length) {
  int error_code = 0;

  if (flags.bitflags & WIDTH && flags.width > 1) {
    *length = flags.width;
  } else {
    *length = 1;
  }
  *v = (char*)calloc(*length + 1, sizeof(char));
  char charstr[2] = {0};
  charstr[0] = value;
  if (*v) {
    write_string(charstr, v, flags, 1);
  } else {
    error_code = ERROR_MEMORY;
  }
  return error_code;
}

int lc_sprint(wchar_t value, char** v, s21_flags_t flags, s21_size_t* length) {
  int error_code = 0;

  char* mbsbuffer = (char*)calloc(sizeof(wchar_t) + 1, sizeof(char));

  if (mbsbuffer) {
    wchar_t value_str[2] = {value, '\0'};
    s21_size_t value_length =
        wcstombs(mbsbuffer, value_str, sizeof(wchar_t) + 1);

    if (flags.bitflags & WIDTH && flags.width > value_length) {
      *length = flags.width;
    } else {
      *length = value_length;
    }

    *v = (char*)calloc(*length + 1, sizeof(char));
    if (*v) {
      write_string(mbsbuffer, v, flags, value_length);
    } else {
      error_code = ERROR_MEMORY;
    }
    free(mbsbuffer);
  } else {
    error_code = ERROR_MEMORY;
  }
  return error_code;
}

int s_sprint(char* value, char** v, s21_flags_t flags, s21_size_t* length) {
  int error_code = 0;
  s21_size_t value_length = s21_strlen(value);
  if (flags.bitflags & PRECISION && flags.precision < value_length) {
    value_length = flags.precision;
  }
  if (flags.bitflags & WIDTH && flags.width > value_length) {
    *length = flags.width;
  } else {
    *length = value_length;
  }

  *v = (char*)calloc(*length + 1, sizeof(char));
  if (*v) {
    write_string(value, v, flags, value_length);
  } else {
    error_code = ERROR_MEMORY;
  }

  return error_code;
}

int ls_sprint(wchar_t* value, char** v, s21_flags_t flags, s21_size_t* length) {
  int error_code = 0;
  char* mbsbuffer =
      (char*)calloc(wcslen(value) * sizeof(wchar_t) + 1, sizeof(char));

  if (mbsbuffer) {
    s21_size_t value_length =
        wcstombs(mbsbuffer, value, wcslen(value) * sizeof(wchar_t) + 1);
    if (flags.bitflags & PRECISION && flags.precision < value_length) {
      value_length = flags.precision;
    }
    if (flags.bitflags & WIDTH && flags.width > value_length) {
      *length = flags.width;
    } else {
      *length = value_length;
    }
    *v = (char*)calloc(*length + 1, sizeof(char));
    if (*v) {
      write_string(mbsbuffer, v, flags, value_length);
    } else {
      error_code = ERROR_MEMORY;
    }
    free(mbsbuffer);
  } else {
    error_code = ERROR_MEMORY;
  }
  return error_code;
}
