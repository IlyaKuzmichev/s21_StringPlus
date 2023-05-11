#include "../../s21_string.h"
#include "../s21_format.h"
#include "s21_sprintf.h"

int n_sprint(int *value, s21_size_t *length, int main_length) {
  *length = 0;
  *value = main_length;
  return 0;
}

int percent_sprint(char **v, s21_size_t *length) {
  int result = 0;
  *v = calloc(2, sizeof(char));
  if (*v) {
    s21_strcpy(*v, "%");
    *length = 1;
  } else {
    result = -1;
  }
  return result;
}
