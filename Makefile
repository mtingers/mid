
all:
	cc -Wall -O2 mid.c -o mid
debug:
	cc -Wall -O2 -DDEBUG=1 mid.c -o mid

test:
	cd tests && bash test_1.sh
