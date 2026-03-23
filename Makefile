all:
	clang main.c -lpthread -o main
check_leaks:
	valgrind --leak-check=full ./main