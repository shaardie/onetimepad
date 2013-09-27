
all: onetimepad.c crypt.o generate.o
	gcc -Wall -lssl -lcrypto onetimepad.c crypt.o generate.o -o onetimepad
	cp onetimepad.1.gz /usr/share/man/man1/
crypt.o: crypt.c crypt.h
	gcc -Wall crypt.c -c -o crypt.o

generate.o: generate.c generate.h
	gcc -Wall generate.c -c -o generate.o

test: all
	valgrind --leak-check=full ./onetimepad

clean:
	rm -f onetimepad crypt.o generate.o /usr/share/man/man1/onetimepad.1.gz
