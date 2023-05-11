#ifndef SRC_S21_STRING_FORMAT_FUNCTION_S21_SPRINTF_H_
#define SRC_S21_STRING_FORMAT_FUNCTION_S21_SPRINTF_H_

#include "../../s21_string.h"
#include "../s21_format.h"

// The sprintf function
int s21_sprintf(char *str, const char *format, ...);

// float functions

int f_sprint(double value, char **v, s21_flags_t flags, s21_size_t *length);
int Lf_sprint(long double value, char **v, s21_flags_t flags,
              s21_size_t *length);

int e_sprint(double value, char **v, s21_flags_t flags, s21_size_t *length);
int Le_sprint(long double value, char **v, s21_flags_t flags,
              s21_size_t *length);

int E_sprint(double value, char **v, s21_flags_t flags, s21_size_t *length);
int LE_sprint(long double value, char **v, s21_flags_t flags,
              s21_size_t *length);

int g_sprint(double value, char **v, s21_flags_t flags, s21_size_t *length);
int Lg_sprint(long double value, char **v, s21_flags_t flags,
              s21_size_t *length);

int G_sprint(double value, char **v, s21_flags_t flags, s21_size_t *length);
int LG_sprint(long double value, char **v, s21_flags_t flags,
              s21_size_t *length);

// char functions

int c_sprint(char value, char **v, s21_flags_t flags, s21_size_t *length);
int lc_sprint(wchar_t value, char **v, s21_flags_t flags, s21_size_t *length);

int s_sprint(char *value, char **v, s21_flags_t flags, s21_size_t *length);
int ls_sprint(wchar_t *value, char **v, s21_flags_t flags, s21_size_t *length);

// integer functions

int d_sprint(int value, char **v, s21_flags_t flags, s21_size_t *length);
int hd_sprint(short int value, char **v, s21_flags_t flags, s21_size_t *length);
int ld_sprint(long int value, char **v, s21_flags_t flags, s21_size_t *length);

int i_sprint(int value, char **v, s21_flags_t flags, s21_size_t *length);
int hi_sprint(short int value, char **v, s21_flags_t flags, s21_size_t *length);
int li_sprint(long int value, char **v, s21_flags_t flags, s21_size_t *length);

int u_sprint(unsigned int value, char **v, s21_flags_t flags,
             s21_size_t *length);
int hu_sprint(unsigned short int value, char **v, s21_flags_t flags,
              s21_size_t *length);
int lu_sprint(unsigned long int value, char **v, s21_flags_t flags,
              s21_size_t *length);

int o_sprint(unsigned int value, char **v, s21_flags_t flags,
             s21_size_t *length);
int ho_sprint(unsigned short int value, char **v, s21_flags_t flags,
              s21_size_t *length);
int lo_sprint(unsigned long int value, char **v, s21_flags_t flags,
              s21_size_t *length);

int x_sprint(unsigned int value, char **v, s21_flags_t flags,
             s21_size_t *length);
int hx_sprint(unsigned short int value, char **v, s21_flags_t flags,
              s21_size_t *length);
int lx_sprint(unsigned long int value, char **v, s21_flags_t flags,
              s21_size_t *length);

int X_sprint(unsigned int value, char **v, s21_flags_t flags,
             s21_size_t *length);
int hX_sprint(unsigned short int value, char **v, s21_flags_t flags,
              s21_size_t *length);
int lX_sprint(unsigned long int value, char **v, s21_flags_t flags,
              s21_size_t *length);

// pointer function

int p_sprint(void *value, char **v, s21_flags_t flags, s21_size_t *length);

// utility functions

int n_sprint(int *value, s21_size_t *length, int main_length);
int percent_sprint(char **v, s21_size_t *length);

#endif  // SRC_S21_STRING_FORMAT_FUNCTION_S21_SPRINTF_H_
