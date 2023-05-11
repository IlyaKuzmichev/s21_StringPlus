#ifndef SRC_S21_STRING_FORMAT_FUNCTIONS_S21_SSCANF_H_
#define SRC_S21_STRING_FORMAT_FUNCTIONS_S21_SSCANF_H_

#include "../../s21_string.h"

// float functions

int f_sscan(float *value, const char **s, s21_size_t width);
int lf_sscan(double *value, const char **s, s21_size_t width);
int Lf_sscan(long double *value, const char **s, s21_size_t width);

// char functions

int c_sscan(char *value, const char **s, s21_size_t width);
int s_sscan(char *value, const char **s, s21_size_t width);
int lc_sscan(wchar_t *value, const char **s, s21_size_t width);
int ls_sscan(wchar_t *value, const char **s, s21_size_t width);

// integer functions

int hu_sscan(unsigned short *value, const char **s, s21_size_t width);
int u_sscan(unsigned int *value, const char **s, s21_size_t width);
int lu_sscan(unsigned long int *value, const char **s, s21_size_t width);

int hd_sscan(short *value, const char **s, s21_size_t width);
int d_sscan(int *value, const char **s, s21_size_t width);
int ld_sscan(long int *value, const char **s, s21_size_t width);

int hi_sscan(short *value, const char **s, s21_size_t width);
int i_sscan(int *value, const char **s, s21_size_t width);
int li_sscan(long int *value, const char **s, s21_size_t width);

int ho_sscan(unsigned short *value, const char **s, s21_size_t width);
int o_sscan(unsigned int *value, const char **s, s21_size_t width);
int lo_sscan(unsigned long int *value, const char **s, s21_size_t width);

int hx_sscan(unsigned short *value, const char **s, s21_size_t width);
int x_sscan(unsigned int *value, const char **s, s21_size_t width);
int lx_sscan(unsigned long int *value, const char **s, s21_size_t width);

// pointer function

int p_sscan(void **value, const char **s, s21_size_t width);

// utility functions

int n_sscan(int *value, s21_size_t length);
int percent_sscan(const char **str);

#endif  // SRC_S21_STRING_FORMAT_FUNCTIONS_S21_SSCANF_H_
