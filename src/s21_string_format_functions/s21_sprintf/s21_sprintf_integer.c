#include "../../s21_string.h"
#include "../s21_format.h"
#include "s21_sprintf.h"

// основание числа
typedef enum {
  BINARY = 2,
  OCTAL = 8,
  DECIMAL = 10,
  HEXADECIMAL = 16,
} Base;

// тип буквы в 16 сс
typedef enum {
  LOW = 0,
  UP = 1,
} Case;

// тип числа
typedef enum {
  UNSIGNED = 0,
  SIGNED = 1,
} Sign;

// возвращает указатель на указатель с готовой строкой
// строка заканчивается \0
// length - длина строки вместе с \0

typedef struct {
  Base base;
  Case caser;
  Sign sign;
  s21_size_t world_len;
  s21_size_t full_len;
  int zero;
  int spaces;
  int prefix;
} Meta;

// возвращает положительное число в виде строки по нужному основанию сс
int s21_itoa(char *str, long long int number, int len, Base base, int upper);
// вычисляет длину числа в виде строки по определенному основанию
int get_len(long long int number, Base base);
// целое число в строку по нужному основанию
int integer_sprint(long long int value, char **v, s21_flags_t flags,
                   s21_size_t *length, Base base, int upper, int sign);
// добавление префикса #
char *add_prefix(char *str, s21_flags_t flag, Base base, int zero, int upper);
// добавляет знаки или их замену
char *add_sign(char *str, s21_flags_t flags, long int value);
//  добавляет пробелы для ширины
char *add_spaces(char *str, int spaces);
// создает строку
int make_string(char **v, Meta meta, s21_flags_t flags, long long int value,
                s21_size_t *len);

int s21_itoa(char *str, long long int number, int len, Base base, int upper) {
  const char nums[] = "0123456789abcdef";
  str[len--] = '\0';

  if (number == 0) {
    str[len] = '0';
  } else {
    number = (number < 0) ? (-number) : number;
    for (; number > 0; number /= base) {
      str[len] = nums[number % base];

      if (upper && (str[len] >= 'a' && str[len] <= 'f')) {
        str[len] += ('A' - 'a');
      }
      len--;
    }
  }

  return 0;
}

int get_len(long long int number, Base base) {
  int len = 0;
  if (number == 0) {
    len = 1;
  } else {
    number = (number < 0) ? (-number) : number;
    for (; number > 0; number /= base, len++) {
    }
  }
  return len;
}

char *add_prefix(char *str, s21_flags_t flags, Base base, int zero, int upper) {
  if (flags.bitflags & SHARP && base == HEXADECIMAL && upper == 0) {
    *str = '0';
    str++;
    *str = 'x';
    str++;
  }
  if (flags.bitflags & SHARP && base == HEXADECIMAL && upper == 1) {
    *str = '0';
    str++;
    *str = 'X';
    str++;
  }

  for (int i = 0; i < zero; i++) {
    *str = '0';
    str++;
  }
  return str;
}

char *add_spaces(char *str, int spaces) {
  for (int i = 0; i < spaces; i++) {
    *str = ' ';
    str++;
  }
  return str;
}

char *add_sign(char *str, s21_flags_t flags, long int value) {
  if (flags.bitflags & FORCE_SIGN && value >= 0) {
    *str = '+';
    str++;
  } else if (flags.bitflags & SPACE_BEFORE_VALUE && value >= 0) {
    *str = ' ';
    str++;
  }

  if (value < 0) {
    *str = '-';
    str++;
  }
  return str;
}

int make_string(char **v, Meta meta, s21_flags_t flags, long long int value,
                s21_size_t *len) {
  int error = 0;
  *v = (char *)calloc(meta.full_len + 1, sizeof(char));
  if (*v == s21_NULL) {
    error = 1;
  }

  if (!error) {
    char *copy = *v;

    if (!(flags.bitflags & LEFT_JUSTIFY)) {
      copy = add_spaces(copy, meta.spaces);
    }

    if (meta.sign) {
      copy = add_sign(copy, flags, value);
    }

    copy = add_prefix(copy, flags, meta.base, meta.zero, meta.caser);
    s21_itoa(copy, value, meta.world_len, meta.base, meta.caser);
    copy += meta.world_len;

    if (flags.bitflags & LEFT_JUSTIFY) {
      add_spaces(copy, meta.spaces);
    }

    *len = meta.full_len;
  }
  return error;
}

int integer_sprint(long long int value, char **v, s21_flags_t flags,
                   s21_size_t *length, Base base, int upper, int sign) {
  int error = 0;
  if (flags.bitflags & PRECISION && flags.precision == 0 && value == 0) {
    if (flags.bitflags & WIDTH) {
      *length = flags.width;
      *v = (char *)calloc(flags.width + 1, sizeof(char));
      add_spaces(*v, flags.width);
    } else {
      *length = 0;
    }
  } else {
    s21_size_t len = get_len(value, base);
    Meta meta = {base, upper, sign, len, len, 0, 0, 0};

    if (value == 0 && flags.bitflags & SHARP) {
      flags.bitflags ^= SHARP;
    }

    if (flags.bitflags & SHARP) {
      if (base == HEXADECIMAL) {
        meta.prefix += 2;
      }
      if (base == OCTAL) {
        meta.zero++;
      }
    }

    if (sign) {
      if (flags.bitflags & FORCE_SIGN || flags.bitflags & SPACE_BEFORE_VALUE ||
          value < 0) {
        meta.prefix += 1;
      }
    }

    if ((flags.bitflags & PRECISION) && (meta.world_len < flags.precision)) {
      meta.zero = flags.precision - meta.world_len;
    }

    meta.full_len = meta.zero + meta.prefix + meta.world_len;

    if ((flags.bitflags & WIDTH) && (flags.width > meta.full_len)) {
      if (flags.bitflags & ZEROES_INSTEAD_SPACES) {
        meta.zero += flags.width - meta.full_len;
      } else {
        meta.spaces += flags.width - meta.full_len;
      }
      meta.full_len += flags.width - meta.full_len;
    }

    error = make_string(v, meta, flags, value, length);
  }
  return error;
}

int d_sprint(int value, char **v, s21_flags_t flags, s21_size_t *length) {
  return integer_sprint(value, v, flags, length, DECIMAL, LOW, SIGNED);
}

int hd_sprint(short int value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return integer_sprint(value, v, flags, length, DECIMAL, LOW, SIGNED);
}

int ld_sprint(long int value, char **v, s21_flags_t flags, s21_size_t *length) {
  return integer_sprint(value, v, flags, length, DECIMAL, LOW, SIGNED);
}

int u_sprint(unsigned int value, char **v, s21_flags_t flags,
             s21_size_t *length) {
  return integer_sprint(value, v, flags, length, DECIMAL, LOW, UNSIGNED);
}

int hu_sprint(unsigned short int value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return integer_sprint(value, v, flags, length, DECIMAL, LOW, UNSIGNED);
}

int lu_sprint(unsigned long int value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return integer_sprint(value, v, flags, length, DECIMAL, LOW, UNSIGNED);
}

int o_sprint(unsigned int value, char **v, s21_flags_t flags,
             s21_size_t *length) {
  return integer_sprint(value, v, flags, length, OCTAL, LOW, UNSIGNED);
}

int ho_sprint(unsigned short int value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return integer_sprint(value, v, flags, length, OCTAL, LOW, UNSIGNED);
}

int lo_sprint(unsigned long int value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return integer_sprint(value, v, flags, length, OCTAL, LOW, UNSIGNED);
}

int x_sprint(unsigned int value, char **v, s21_flags_t flags,
             s21_size_t *length) {
  return integer_sprint(value, v, flags, length, HEXADECIMAL, LOW, UNSIGNED);
}

int hx_sprint(unsigned short int value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return integer_sprint(value, v, flags, length, HEXADECIMAL, LOW, UNSIGNED);
}

int lx_sprint(unsigned long int value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return integer_sprint(value, v, flags, length, HEXADECIMAL, LOW, UNSIGNED);
}

int X_sprint(unsigned int value, char **v, s21_flags_t flags,
             s21_size_t *length) {
  return integer_sprint(value, v, flags, length, HEXADECIMAL, UP, UNSIGNED);
}

int hX_sprint(unsigned short int value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return integer_sprint(value, v, flags, length, HEXADECIMAL, UP, UNSIGNED);
}

int lX_sprint(unsigned long int value, char **v, s21_flags_t flags,
              s21_size_t *length) {
  return integer_sprint(value, v, flags, length, HEXADECIMAL, UP, UNSIGNED);
}

int p_sprint(void *value, char **v, s21_flags_t flags, s21_size_t *length) {
  int res;
  if (value) {
    flags.bitflags |= SHARP;
    res = integer_sprint((long)value, v, flags, length, HEXADECIMAL, LOW,
                         UNSIGNED);
  } else {
    *v = (char *)calloc(4, sizeof(char));
    if (*v) {
      *(*v) = '0';
      *(*v + 1) = 'x';
      *(*v + 2) = '0';
      *(*v + 3) = '\0';
      *length = 3;
      res = 0;
    } else {
      res = 1;
    }
  }
  return res;
}
