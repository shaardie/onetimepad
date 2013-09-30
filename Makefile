all: onetimepad.c crypt.o generate.o
	gcc -Wall onetimepad.c crypt.o generate.o -o onetimepad -lcrypto

install: all 
	cp onetimepad.1.gz /usr/share/man/man1/
	cp onetimepad /usr/bin/

crypt.o: crypt.c crypt.h
	gcc -Wall crypt.c -c -o crypt.o

generate.o: generate.c generate.h
	gcc -Wall generate.c -c -o generate.o

clean:
	rm -f onetimepad crypt.o generate.o 

uninstall:
	rm /usr/share/man/man1/onetimepad.1.gz /usr/bin/onetimepad
