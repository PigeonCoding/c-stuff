main-build: main.c
	clang ./main.c -g -Wall -Wextra -Wswitch-enum -o build/main
main-run: main-build
	./build/main

http-build: http-test.c
	clang ./http-test.c -g -Wall -Wextra -Wswitch-enum -o build/http-test
http-run: http-build
	./build/http-test