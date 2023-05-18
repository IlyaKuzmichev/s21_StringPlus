CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -c -g

TESTFLAGS:=
ifeq ($(shell uname -s),Linux)
	TESTFLAGS += -lm -lrt -lsubunit
endif

TESTFLAGS += -lcheck -lpthread -pthread

GCOVFLAGS = -fprofile-arcs -ftest-coverage

HEADERS=s21_string.h
SOURCES=$(addprefix s21_string_basic_functions/,s21_comparison_functions.c s21_copying_funcs.c s21_search_funcs.c s21_calculation_functions.c s21_additional_functions.c s21_special_processing_funcs.c)
OBJECTS=$(SOURCES:.c=.o)
TESTS_SOURCES=test.c

VALGRIND_SOURCES=valgrind_test.c
VALGRIND_EXEC=valgrind_test.out


FORMAT_HEADERS=s21_string_format_functions/s21_format.h

SPRINT_HEADERS=s21_string_format_functions/s21_sprintf/s21_sprintf.h
SPRINT_SOURCES=$(addprefix s21_string_format_functions/s21_sprintf/,s21_sprintf_double.c s21_sprintf.c s21_sprintf_char.c s21_sprintf_integer.c s21_sprintf_util_specs.c) 
SPRINT_OBJECTS=$(SPRINT_SOURCES:.c=.o)

SSCAN_HEADERS=s21_string_format_functions/s21_sscanf/s21_sscanf.h
SSCAN_SOURCES=$(addprefix s21_string_format_functions/s21_sscanf/,s21_sscanf.c s21_sscanf_char.c s21_sscanf_double.c s21_sscanf_integer.c)
SSCAN_OBJECTS=$(SSCAN_SOURCES:.c=.o)

GCDA_FILES=$(SPRINT_SOURCES:.c=.gcda) $(SOURCES:.c=.gcda)
GCNO_FILES=$(SPRINT_SOURCES:.c=.gcno) $(SOURCES:.c=.gcno)
GCOV_FILES=$(SPRINT_SOURCES:.c=.gcov) $(SOURCES:.c=.gcov)

FILES_TO_CHECK=$(SOURCES) $(HEADERS) $(VALGRIND_SOURCES) $(TESTS_SOURCES) $(FORMAT_HEADERS) $(SPRINT_HEADERS) $(SPRINT_SOURCES) $(SSCAN_HEADERS) $(SSCAN_SOURCES) $(TESTS_SOURCES)


.PHONY: test rebuild clean stylecheck fmt
.SECONDARY: $(OBJECTS)


all: s21_string.a


%.out: %.o $(OBJECTS)
	$(CC) $^ -o $@
	
%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@ 

s21_string.a:  $(OBJECTS) $(SPRINT_OBJECTS) $(SSCAN_OBJECTS)
	ar rcs s21_string.a $^
	ranlib s21_string.a

gcov_report: test
	lcov -t test.out -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html

report_nolcov:
	gcov *gcno > coverage.txt
	open coverage.txt

test: test_s21_string.a
	$(CC) -g $(TESTS_SOURCES) $(TESTFLAGS) $(GCOVFLAGS) -L. s21_string.a -o test.out
	./test.out

test_s21_string.a: all_functions
	ar rcs s21_string.a *.o
	ranlib s21_string.a

all_functions: $(SOURCES) $(SPRINT_SOURCES) $(SSCAN_SOURCES)
	$(CC) $(CFLAGS) $^ $(GCOVFLAGS)

rebuild: clean all

valgrinder: s21_string.a
	gcc -o $(VALGRIND_EXEC) -Wall -Werror -Wextra $(VALGRIND_SOURCES) s21_string.a
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --num-callers=20 --track-fds=yes ./$(VALGRIND_EXEC)

clean: 
	@rm -rf  $(OBJECTS) $(SPRINT_OBJECTS) $(SSCAN_OBJECTS) $(GCDA_FILES) $(GCNO_FILES) $(GCOV_FILES) 
	@rm -rf report/ s21_string.a *.gcda *.gcno *.gcov *.out *.info *.o coverage.txt
	
stylecheck: 
	@clang-format -style=Google -n $(FILES_TO_CHECK)
	
fmt: .clang-format
	@clang-format -style=Google -n $(FILES_TO_CHECK)

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem $(FILES_TO_CHECK)

