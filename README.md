# s21_StringPlus

### Own Implementation of the string.h library with additions.

### This is a team project developed in collaboration with tabathae, ivarstet and lorenttr, also students of School 21 in Moscow.

## Implementation

* The program was developed and implemented on MacOs Big Sur Version 11.6.6 and repeats the behavior of the standard `string.h` library functions and `sprintf, sscanf` from stdio.h
* You may build static library by running Makefile with `make` or `make s21_string.a` and use in your own projects in C language
* To run unit tests check.h library needs to be installed, target `make test`, it builds another library for testing with flags of coverage
* You may check test's coverage, for `make gcov_report` you must have the `lcov` utility installed
* To check for memory leaks i recomend to use linux OS with valgrind utility. Run `make valgrinder` on it


