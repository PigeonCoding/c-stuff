main: main.c
	clang ./main.c -g -Wall -Wextra -Wswitch-enum  -o main
main-run: main
	./main

http: http-test.c
	clang ./http-test.c -g -Wall -Wextra -Wswitch-enum  -o http-test
http-run: http
	./http-test