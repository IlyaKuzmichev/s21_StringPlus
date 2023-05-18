#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "s21_string.h"
#define LEN 512

void to_upper_single() {
  const char str1[] = "English, motherfucker, DO YOU SPEAK IT?!123450";
  char *result = (char *)s21_to_upper(str1);
  char *expected_result = "ENGLISH, MOTHERFUCKER, DO YOU SPEAK IT?!123450";
  printf("%s\n%s\n", result, expected_result);
  if (result) {
    free(result);
  }
}

void to_lower_single() {
  const char str1[] = "123450English, motherfucker, DO YOU SPEAK IT?!";
  char *result = (char *)s21_to_lower(str1);
  char *expected_result = "123450english, motherfucker, do you speak it?!";
  printf("%s\n%s\n", result, expected_result);
  if (result) {
    free(result);
  }
}

void insert_multy() {
  struct input_data {
    const char *str1;
    const char *str2;
    s21_size_t index;
  };
  struct input_data cases[] = {{"I'm a poor boy", " just", 3},
                               {"I need no", " sympathy", 9},
                               {"easy go", "Beacuse I'm easy come, ", 0},
                               {"I need no", " sympathy", 19},
                               {"I need no", "", 0},
                               {"I need no", "", 6},
                               {"I need no", "", 19},
                               {"", "", 0},
                               {"", " ", 0},
                               {" ", "", 0},
                               {" ", " ", 0}};
  s21_size_t n = sizeof(cases) / sizeof(cases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    char *result =
        (char *)s21_insert(cases[i].str1, cases[i].str2, cases[i].index);
    if (result) {
      printf("Case %llu:\t%s\n", i, result);
      free(result);
    }
  }
}

void trim_multy() {
  struct input_data {
    const char *str1;
    const char *str2;
  };
  struct input_data cases[] = {
      {"Floor! Let the bodies hit the Floor!", "Floor !"},
      {"p p p Poker face", "p "},
      {"Oh no no no", "no "},
      {"Oh no no no", "yes "},
      {"aaa", "a"},
      {" ", ""},
      {"", " "},
      {"      ", "       "}};
  s21_size_t n = sizeof(cases) / sizeof(cases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char *result = (char *)s21_trim(cases[i].str1, cases[i].str2);
    if (result) {
      printf("Case %llu:\t%s\n", i, result);
      free(result);
    }
  }
}

void sprintf_c_multy() {
  struct input_data {
    const char *format;
    const char c1;
    const char c2;
    const char c3;
  };
  struct input_data_precision {
    const char *format;
    const char c1;
    const int pr;
    const char c2;
  };
  struct input_data testcases[] = {
      {"%c%c%c", 'a', 'b', 'c'},         {"%c%c%c", '\t', ' ', '\n'},
      {"%%%c%0c%%%c", 'a', 'b', 'c'},    {"%4c%05c%6c", 'a', 'b', 'c'},
      {"%1c%02c%1c", 'a', 'd', 'g'},     {"%-4c%-5c%-6c", 'a', 'b', 'c'},
      {"%-4.3c%5.2c%-6c", 'a', 'b', 'c'}};
  struct input_data_precision testcases_pr[] = {
      {"%04c%*c", 'a', 4, 'c'},     {"%1c%.*c", 'a', 4, 'd'},
      {"%04c%*c", 'a', 1, 'b'},     {"%-6c%.*c", 'a', 1, 'b'},
      {"%02.2c%*.0c", 'a', 5, 'd'}, {"%-5c%5.*c", 'a', 5, 'b'},
      {"%5c%*.5c", 'a', -5, 'd'},   {"%-2c%5.*c", 'a', 0, 'b'},
  };
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].c1, testcases[i].c2,
                testcases[i].c3);
    sprintf(expected_result, testcases[i].format, testcases[i].c1,
            testcases[i].c2, testcases[i].c3);
    printf("%s\n%s\n", result, expected_result);
  }
  s21_size_t m = sizeof(testcases_pr) / sizeof(testcases_pr[0]);
  for (s21_size_t i = 0; i < m; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases_pr[i].format, testcases_pr[i].c1,
                testcases_pr[i].pr, testcases_pr[i].c2);
    sprintf(expected_result, testcases_pr[i].format, testcases_pr[i].c1,
            testcases_pr[i].pr, testcases_pr[i].c2);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_s_single() {
  struct input_data {
    const char *format;
    const char *s;
  };
  struct input_data testcases[] = {
      {"%s", "Aboba string"},    {"%s", "  \t "},
      {"%%%s", "Aboba string"},  {"%040.2s", "Aboba string"},
      {"%-40s", "Aboba string"}, {"%-040.40s", "Aboba string"}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].s);
    sprintf(expected_result, testcases[i].format, testcases[i].s);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_ls_single() {
  struct input_data {
    const char *format;
    const wchar_t *s;
  };
  struct input_data testcases[] = {{"%ls", L"Мы адепты кириллицы"},
                                   {"%ls", L"  \t "},
                                   {"%%%ls", L"हिंदी में पाठ"},
                                   {"%040ls", L"Косово је Србија"},
                                   {"%-40.2ls", L"Ez bi kurdî dinivîsim"},
                                   {"%40.40ls", L"日本"}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].s);
    sprintf(expected_result, testcases[i].format, testcases[i].s);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_dioxu_long() {
  struct input_data {
    const char *format;
    const int n1;
    const int n2;
    const int n3;
  };
  struct input_data testcases[] = {
      {"%d%i%o", 2147483647, -2147483648, 999999999},
      {"%%%d%d%d", 2147483647, -2147483648, 999999999},
      {"%4o%3i%2u", 2147483647, -2147483648, 999999999},
      {"%1d%1d%1d", 2147483647, -0, -2147483648},
      {"%04d%*d", 2147483647, 3, -2147483648},
      {"%+4x%5.*d", 2147483647, 3, -2147483648},
      {"%+05u%1o%+1d", 2147483647, 0, -2147483648},
      {"% -d%*d", 2147483647, 20, -2147483648},
      {"%-4x%-2.*d", 2147483647, 20, -2147483648},
      {"%+01u%+5.5d% d", 2147483647, -2147483648, -2147483648},
      {"% -o%*d", 2147483647, 0, -2147483648},
      {"%-4o%+5.*d", 2147483647, 0, -2147483648},
      {"%+.5x%-5.5d% d", 2147483647, 1234567890, 123456},
      {"% -.5u%*d", 2147483647, 5, -2147483648},
      {"%.3o%+3.*d", 2147483647, 7, -2147483648},
      {"%x%i%d", 2147483647, 0, -2147483648},
      {"%%%u%d%i", 2147483647, 0, -2147483648},
      {"%4d%3i%2i", 2147483647, 1234567890, -2147483648},
      {"%1i%1i%1i", 2147483647, -222, -2147483648},
      {"%04i%*d", 2147483647, 2, -2147483648},
      {"%+4d%5.*i", 2147483647, 2, -2147483648},
      {"%+05i%1d%+1i", 2147483647, -222, -2147483648},
      {"% -x%*i", 2147483647, 0, -2147483648},
      {"%-4i%-2.*d", 2147483647, 0, -2147483648}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].n1, testcases[i].n2,
                testcases[i].n3);
    sprintf(expected_result, testcases[i].format, testcases[i].n1,
            testcases[i].n2, testcases[i].n3);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_hoxu_multy() {
  struct input_data {
    const char *format;
    const short int n1;
    const short int n2;
    const short int n3;
  };
  struct input_data testcases[] = {{"%ho%ho%#ho", 1, 9, 15},
                                   {"%hu%hu%#hu", 1, 9, 15},
                                   {"%hx%hx%#hx", 1, 15, 16},
                                   {"%hX%hX%#hX", 1, 15, 16},
                                   {"%4ho%3hu%#2hx", 100, 234, 5678},
                                   {"%1hX%1ho%1hu", 111, 222, 333},
                                   {"%04hx%*hX", 99, 2, 99},
                                   {"%+4ho%5.*hu", 1, 5, 100},
                                   {"%+05hx%#1hX%+1ho", 111, 222, 333},
                                   {"% -hu%*hx", 1, 2, 15},
                                   {"%#-4hX%-2.*ho", 14, 0, 0},
                                   {"%#+01hu%+5.5hx% X", 111, 222, 333},
                                   {"% -ho%*hu", 11, 5, 3},
                                   {"%-4hx%+5.*hX", 1, 7, 5678},
                                   {"%+.5ho%-5.5hu%# hx", 111, 222, 1236},
                                   {"% -.5hX%*ho", 21, 5, 21},
                                   {"%.3hu%+3.*hx", 12345, 7, 0},
                                   {"%#hX%#ho%hu", 21, 21, 21},
                                   {"%%%hx%hX%ho", 21, 21, 21},
                                   {"%4hu%3hx%2hX", 21221, 21121, 21221},
                                   {"%1ho%1hu%1hx", 111, 222, 333},
                                   {"%04hX%*ho", 11, 2, 123},
                                   {"%+4hu%5.*hx", 404, 0, 0},
                                   {"%+05hX%1ho%+1hu", 111, 222, 0},
                                   {"%# -hx%*hX", 13, 2, 14},
                                   {"%-4ho%-#2.*hu", 64, 2, 5678}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].n1, testcases[i].n2,
                testcases[i].n3);
    sprintf(expected_result, testcases[i].format, testcases[i].n1,
            testcases[i].n2, testcases[i].n3);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_loxu_multy() {
  struct input_data {
    const char *format;
    const long int n1;
    const long int n2;
    const long int n3;
  };
  struct input_data testcases[] = {{"%lo%lo%#lo", 1, 9, 15},
                                   {"%lu%lu%#lu", 1, 9, 15},
                                   {"%lx%lx%#lx", 1, 15, 16},
                                   {"%lX%lX%#lX", 1, 15, 16},
                                   {"%4lo%3lu%#2lx", 100, 234, 5678},
                                   {"%1lX%1lo%1lu", 111, 222, 333},
                                   {"%04lx%*lX", 99, 2, 99},
                                   {"%+4lo%+50.*lu", 1, 5, 100},
                                   {"%+05lx%#1lX%+1lo", 111, 222, 333},
                                   {"% -lu%*lx", 1, 2, 15},
                                   {"%#-4lX%-0.*lo", 14, 0, 0},
                                   {"%#+01lu%+5.5lx% X", 111, 222, 333},
                                   {"% -lo%*lu", 11, 5, 3},
                                   {"%-4lx%+5.*lX", 1, 7, 5678},
                                   {"%+.5lo%-5.0lu%# lx", 111, 222, 123456},
                                   {"% -.5lX%*lo", 21, 5, 21},
                                   {"%.3lu%+3.*lx", 12345, 7, 0},
                                   {"%#lX%#lo%lu", 21, 21, 21},
                                   {"%%%lx%lX%lo", 21, 21, 21},
                                   {"%4lu%3lx% 2lX", 212121, 212121, 212121},
                                   {"%1lo%1lu% 10lx", 111, 222, 333},
                                   {"%04lX%*lo", 11, 2, 123},
                                   {"%+4lu%5.*lx", 404, 0, 0},
                                   {"%+05lX%1lo%+1lu", 111, 222, 0},
                                   {"%# -lx%*lX", 13, 2, 14},
                                   {"%-4lo%-#2.*lu", 64, 2, 5678}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].n1, testcases[i].n2,
                testcases[i].n3);
    sprintf(expected_result, testcases[i].format, testcases[i].n1,
            testcases[i].n2, testcases[i].n3);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_f_single() {
  struct input_data {
    const char *format;
    const double n;
  };
  struct input_data testcases[] = {{"%f", 1.23},          {"%%%f", -1.23},
                                   {"%4f", 1.23},         {"%4f", 12.3456},
                                   {"%04f", 1.23},        {"%+4f", 1.23},
                                   {"%+05f", 1.23},       {"% .2f", 12.3456},
                                   {"%-4f", 1.23},        {"%+5.5f", 1.23},
                                   {"% 5f", -1.23},       {"%.5f", 1.23},
                                   {"%+10.5f", 1.234445}, {"%-10.5f", 123},
                                   {"%.3f", 1.23456},     {"%#f", 1},
                                   {"%%%#f", -123},       {"%#.3f", 1.23},
                                   {"%+#f", 12.3456},     {"%0#f", 1.23},
                                   {"%#-4.1f", 123},      {"%+#05.1f", 1.23},
                                   {"% #f", 123},         {"%-# 7f", 12.3}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].n);
    sprintf(expected_result, testcases[i].format, testcases[i].n);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_Lf_single() {
  struct input_data {
    const char *format;
    const long double n;
  };
  struct input_data testcases[] = {{"%Lf", 1.23},          {"%%%Lf", -1.23},
                                   {"%4Lf", 1.23},         {"%1.1Lf", 12.3456},
                                   {"%04Lf", 1.23},        {"%+4.0Lf", 1.23},
                                   {"%+05Lf", 1.23},       {"% .2Lf", 12.3456},
                                   {"%-4Lf", 1.23},        {"%+5.5Lf", 1.23},
                                   {"% 5Lf", -1.23},       {"%.5Lf", 1.23},
                                   {"%+10.5Lf", 1.234445}, {"%-10.5Lf", 123},
                                   {"%.3Lf", 1.23456},     {"%#Lf", 1},
                                   {"%%%#Lf", -123},       {"%#.3Lf", 0},
                                   {"%+#Lf", 12.3456},     {"%0#Lf", 0},
                                   {"%#-4.1Lf", 123},      {"%+#05.1Lf", 0},
                                   {"% #Lf", 123},         {"%-# 7.0Lf", 0}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].n);
    sprintf(expected_result, testcases[i].format, testcases[i].n);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_e_single() {
  struct input_data {
    const char *format;
    const double n;
  };
  struct input_data testcases[] = {{"%e", 1.23},          {"%%%e", -1.23},
                                   {"%4e", 1.23},         {"%4e", 12.3456},
                                   {"%04E", 1.23},        {"%+4e", 1.23},
                                   {"%+05E", 1.23},       {"% e", 12.3456},
                                   {"%-4E", 1.23},        {"%+5.0e", 1.23},
                                   {"% 5E", -1.23},       {"%.5e", 1.23},
                                   {"%+10.2E", 1.234545}, {"%-10.5e", 123},
                                   {"%.3E", 1.23456},     {"%#e", 1},
                                   {"%%%#E", -123},       {"%#.1e", 1.2876},
                                   {"%+#E", 12.3456},     {"%0#e", 1.23},
                                   {"%#-4E", 123},        {"%+#05e", 1.23},
                                   {"% #E", 123},         {"%-# 7e", 12.3}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].n);
    sprintf(expected_result, testcases[i].format, testcases[i].n);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_LE_single() {
  struct input_data {
    const char *format;
    const long double n;
  };
  struct input_data testcases[] = {{"%Le", 1.23},          {"%%%Le", -1.23},
                                   {"%4Le", 1.23},         {"%4Le", 12.3456},
                                   {"%04LE", 1.23},        {"%+4Le", 1.23},
                                   {"%+05LE", 1.23},       {"% Le", 12.3456},
                                   {"%-4LE", 1.23},        {"%+5.0Le", 1.23},
                                   {"% 5LE", -1.23},       {"%.5Le", 1.23},
                                   {"%+10.2LE", 1.234545}, {"%-10.5Le", 123},
                                   {"%.3LE", 1.23456},     {"%#Le", 1},
                                   {"%%%#LE", -123},       {"%#.1Le", 1.2876},
                                   {"%+#LE", 12.3456},     {"%0#Le", 1.23},
                                   {"%#-4LE", 123},        {"%+#05Le", 1.23},
                                   {"% #LE", 123},         {"%-# 7Le", 12.3}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].n);
    sprintf(expected_result, testcases[i].format, testcases[i].n);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_g_single() {
  struct input_data {
    const char *format;
    const double n;
  };
  struct input_data testcases[] = {{"%g", 1.23},          {"%%%g", -1.23},
                                   {"%4g", 1.23},         {"%4g", 12.3456},
                                   {"%04g", 1.23},        {"%+4G", 1.23},
                                   {"%+09g", 9000.00},    {"% G", 12.3456},
                                   {"%-4g", 1.23},        {"%+5.5G", 1.23},
                                   {"% 5g", -1.23},       {"%.5G", 1.23},
                                   {"%+10.5g", 1.234445}, {"%-10.5G", 12300000},
                                   {"%.3g", 1.23456},     {"%#G", 1},
                                   {"%%%#g", -123},       {"%#G", 1.10000},
                                   {"%+#g", 12.3456},     {"%0#G", 1.23},
                                   {"%#-4g", 123},        {"%+#05G", 1.23},
                                   {"% #g", 123},         {"%-# 7G", 120000}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].n);
    sprintf(expected_result, testcases[i].format, testcases[i].n);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_LG_single() {
  struct input_data {
    const char *format;
    const long double n;
  };
  struct input_data testcases[] = {
      {"%Lg", 1.23},          {"%%%Lg", -1.23},
      {"%4Lg", 1.23},         {"%4Lg", 12.3456},
      {"% 04Lg", 0},          {"%+4LG", 1.23},
      {"%+09Lg", 9000.00},    {"% LG", 12.3456},
      {"% -4.4Lg", 1.23},     {"%+5.5LG", 1.23},
      {"% 5Lg", -1.23},       {"%.5LG", 1.23},
      {"%+10.5Lg", 1.234445}, {"%-10.5LG", 12300000},
      {"%4.0Lg", 0},          {"%#LG", 0},
      {"%%%#Lg", -123},       {"% #LG", 1.10000},
      {"%+#Lg", 12.3456},     {"%0#LG", 1.23},
      {"%#-4Lg", 123},        {"%+#05LG", 1.23},
      {"% #Lg", 123},         {"%-# 7LG", 120000}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].n);
    sprintf(expected_result, testcases[i].format, testcases[i].n);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_feg_single_long() {
  struct input_data {
    const char *format;
    const double n;
  };
  struct input_data testcases[] = {
      {"%f", -340282346638528859811704183484516925440.0000000000000000},
      {"%%%f", 340282346638528859811704183484516925440.0000000000000000},
      {"%4e", -340282346638528859811704183484516925440.0000000000000000},
      {"%4e", 340282346638528859811704183484516925440.0000000000000000},
      {"%04g", -340282346638528859811704183484516925440.0000000000000000},
      {"%+4g", 340282346638528859811704183484516925440.0000000000000000},
      {"%+05f", 340282346638528859811704183484516925439.999999999999},
      {"% .2f", -340282346638528859811704183484516925439.899999999999},
      {"%-4e", 340282346638528859811704183484516925439.999999999999},
      {"%+5.5e", -340282346638528859811704183484516925439.999999999999},
      {"% 5g", 340282346638528859811704183484516925439.999999999999},
      {"%.5g", -340282346638528859811704183484516925439.999999999999},
      {"%+50.10f", 340282346638528859811704183484516925440.0000000000000000},
      {"%-10.0f", -340282346638528859811704183484516925439.444999999999},
      {"%.0e", 340282346638528859811704183484516925439.9999999999},
      {"%#e", -340282346638528859811704183484516925439.999999999999},
      {"%%%#g", 340282346638528859811704183484516925440.0000000000000000},
      {"%50.15g", 000000000000000000000000.0000000000000000},
      {"%+#f", 340282346638528859811704183484516925440.0000000000000000},
      {"%0#f", -340282346638528859811704183484516925439.999999999999},
      {"%#-50.10e", 340282346638528859811704183484516925440.0000000000000000},
      {"%+#05.0e", -340282346638528859811704183484516925439.899999999999},
      {"% #g", 340282346638528859811704183484516925440.0000000000000000},
      {"%-# 7g", -340282346638528859811704183484516925439.999999999999}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    s21_sprintf(result, testcases[i].format, testcases[i].n);
    sprintf(expected_result, testcases[i].format, testcases[i].n);
    printf("%s\n%s\n", result, expected_result);
  }
}

void sprintf_hard() {
  struct input_data {
    const char *format;
    const char c;
    const int d;
    const unsigned long int ul_i;
    const char *s;
    const char *p;
    const int o_i;
    const short int hx_i;
    const double f;
    const long double L_e;
    const long double L_g;
  };
  char *test_p = "abcdef";
  struct input_data testcases[] = {
      {"Hard %c rock %d hard %lu %nrock %s hard %p rock %o hard %hx rock %f "
       "hard "
       "%Le rock %Lg!!!",
       'z', 1, 1, "xyz", &test_p[0], 1, 1, 1., 1., 1.},
      {"Hard   %4c rock %5d hard %1lu %nrock %2s hard %p rock %% %5o hard "
       "%05hx  %%%%rock %5f hard "
       "%05Le \trock %+Lg!!!",
       'z', 1, 12345, "xyz", &test_p[1], 1, 1, 1., 1., 1.},
      {"Hard %-4c rock %-5d hard %5lu %nrock %-5s hard %p rock %-4o hard %-3hx "
       "rock %-2f hard "
       "%-1Le rock %-10Lg!!!",
       'z', 1, 1, "xyz", &test_p[2], 1, 1, 1., 1., 210000.00},
      {"Hard %1c rock % +04d hard % 04.4lu %nrock %10s hard %p rock % #04o "
       "hard "
       "% #04.4hx rock % #f hard "
       "% #-5Le rock %+-#5.2Lg!!!",
       'z', 1, 1, "xyz", &test_p[4], 1, 1, 1., 1., 1.245}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);

  for (s21_size_t i = 0; i < n; i++) {
    char result[LEN];
    char expected_result[LEN];
    int n1;
    int n2;
    s21_sprintf(result, testcases[i].format, testcases[i].c, testcases[i].d,
                testcases[i].ul_i, &n1, testcases[i].s, testcases[i].p,
                testcases[i].o_i, testcases[i].hx_i, testcases[i].f,
                testcases[i].L_e, testcases[i].L_g);
    sprintf(expected_result, testcases[i].format, testcases[i].c,
            testcases[i].d, testcases[i].ul_i, &n2, testcases[i].s,
            testcases[i].p, testcases[i].o_i, testcases[i].hx_i, testcases[i].f,
            testcases[i].L_e, testcases[i].L_g);
    printf("%s\n%s\n", result, expected_result);
  }
}

int main(void) {
  setlocale(LC_ALL, "");

  to_upper_single();
  to_lower_single();
  insert_multy();
  trim_multy();
  sprintf_c_multy();
  sprintf_s_single();
  // sprintf_ls_single();
  sprintf_dioxu_long();
  sprintf_hoxu_multy();
  sprintf_loxu_multy();
  sprintf_f_single();
  sprintf_Lf_single();
  sprintf_e_single();
  sprintf_LE_single();
  sprintf_g_single();
  sprintf_LG_single();
  sprintf_feg_single_long();
  sprintf_hard();

  return 0;
}
