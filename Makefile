OS := $(shell uname)
ifeq ($(OS), "") # uname failed, try something else
	OS != uname
endif

THREADS_LIB := -lpthread
ifeq ($(OS), Minix)
	THREADS_LIB := -lmthread
endif

all:
	clang main.c -lpthread -o main
check_leaks:
	valgrind --leak-check=full ./main