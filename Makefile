OS := $(shell uname)

THREADS_LIB := -lpthread
ifeq ($(OS), Minix)
	THREADS_LIB := -lmthread
endif

all:
	clang main.c $(THREADS_LIB) -o main
check_leaks:
	valgrind --leak-check=full ./main