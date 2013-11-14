LIBS=-lcrypto `libgcrypt-config --cflags --libs`

all: onetimepad.c crypt.o generate.o import.o twofish.o key.o
	gcc -Wall onetimepad.c crypt.o generate.o import.o twofish.o key.o -o onetimepad ${LIBS}

key.o: key.c key.h
	gcc -Wall key.c -c -o key.o

install: all 
	cp onetimepad.1 /usr/share/man/man1/
	cp onetimepad /usr/bin/

crypt.o: crypt.c crypt.h 
	gcc -Wall crypt.c -c -o crypt.o

twofish.o: twofish.c twofish.h 
	gcc -Wall twofish.c -c -o twofish.o

generate.o: generate.c generate.h
	gcc -Wall generate.c -c -o generate.o

import.o: import.c import.h 
	gcc -Wall import.c -c -o import.o

clean:
	rm -f onetimepad *.o

uninstall:
	rm -f /usr/share/man/man1/onetimepad.1 /usr/bin/onetimepad
	rm -f /usr/bin/onetimepad

test: all
	dd if=/dev/urandom of=inputfile count=10 bs=1M
	./onetimepad -r   generate 50000 keyfile 
	cp keyfile keyfile2
	./onetimepad -s 2 import keyfile
	mv keyfile2 keyfile
	./onetimepad -s 1 import keyfile
	./onetimepad      encrypt inputfile keyfile.public encfile.1
	./onetimepad      encrypt inputfile keyfile.public encfile.2
	./onetimepad      encrypt inputfile keyfile.public encfile.3
	./onetimepad      encrypt inputfile keyfile.public encfile.4
	./onetimepad -k   decrypt encfile.1 keyfile plainfile.1.a
	./onetimepad      decrypt encfile.1 keyfile plainfile.1.b
	./onetimepad      decrypt encfile.1 keyfile plainfile.1.c
	./onetimepad      decrypt encfile.3 keyfile plainfile.3
	./onetimepad      decrypt encfile.2 keyfile plainfile.2
	./onetimepad      decrypt encfile.4 keyfile plainfile.4
	@diff inputfile plainfile.1.a && echo "  decrypted file 1.a: ok" || echo "  decrypted file 1.a: error"
	@diff inputfile plainfile.1.b && echo "  decrypted file 1.b: ok" || echo "  decrypted file 1.b: error"
	@diff inputfile plainfile.1.c && echo "  decrypted file 1.c: error" || echo "  decrypted file 1.c: ok (should differ)"
	@diff inputfile plainfile.2 && echo "  decrypted file 2: ok" || echo "  decrypted file 2: error"
	@diff inputfile plainfile.3 && echo "  decrypted file 3: ok" || echo "  decrypted file 3: error"
	@diff inputfile plainfile.4 && echo "  decrypted file 4: ok" || echo "  decrypted file 4: error"
	@echo cleanup…
	@rm  inputfile keyfile keyfile.public plainfile.* encfile.*
