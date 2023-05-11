#ifndef SRC_S21_FORMAT_H_
#define SRC_S21_FORMAT_H_

#include "../s21_string.h"

typedef enum s21_flags_values {
  LEFT_JUSTIFY = 1,
  FORCE_SIGN = 2,          //
  SPACE_BEFORE_VALUE = 4,  //
  SHARP = 8,               //
  ZEROES_INSTEAD_SPACES = 16,
  WIDTH = 32,
  PRECISION = 64  //
} s21_flags_values;

typedef struct s21_flags_t {
  char bitflags;
  size_t precision;
  size_t width;
} s21_flags_t;

#endif  // SRC_S21_FORMAT_H_
