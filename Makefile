LIBS=-lcrypto `libgcrypt-config --cflags --libs`
FLAGS=-g -Wall

all: onetimepad.c crypt.o generate.o import.o twofish.o key.o
	 gcc ${FLAGS} onetimepad.c crypt.o generate.o import.o twofish.o key.o -o onetimepad ${LIBS}

key.o: key.c key.h
	 gcc ${FLAGS} key.c -c -o key.o

install: all 
	cp onetimepad.1 /usr/share/man/man1/
	cp onetimepad /usr/bin/

crypt.o: crypt.c crypt.h 
	 gcc ${FLAGS} crypt.c -c -o crypt.o

twofish.o: twofish.c twofish.h 
	 gcc ${FLAGS} twofish.c -c -o twofish.o

generate.o: generate.c generate.h
	 gcc ${FLAGS} generate.c -c -o generate.o

import.o: import.c import.h 
	 gcc ${FLAGS} import.c -c -o import.o

clean:
	rm -f onetimepad *.o

uninstall:
	rm -f /usr/share/man/man1/onetimepad.1 /usr/bin/onetimepad
	rm -f /usr/bin/onetimepad

test: all
	dd if=/dev/urandom of=inputfile count=10 bs=1M &> /dev/null
	./onetimepad -r   generate 60000 kf-dec
	cp kf-dec kf-enc
	echo 123 | ./onetimepad import-enc kf-enc > /dev/null
	echo 234 | ./onetimepad import-dec kf-dec > /dev/null
	echo 123 | ./onetimepad encrypt inputfile kf-enc.public encfile.1 > /dev/null
	echo 123 | ./onetimepad encrypt inputfile kf-enc.public encfile.2 > /dev/null
	echo 123 | ./onetimepad encrypt inputfile kf-enc.public encfile.3 > /dev/null
	echo 123 | ./onetimepad encrypt inputfile kf-enc.public encfile.4 > /dev/null
	echo xyz | ./onetimepad encrypt inputfile kf-enc.public encfile.5 > /dev/null
	echo 234 | ./onetimepad decrypt encfile.1 kf-dec plainfile.1.a > /dev/null
	echo 234 | ./onetimepad decrypt encfile.1 kf-dec plainfile.1.b > /dev/null
	echo xyz | ./onetimepad decrypt encfile.1 kf-dec plainfile.1.c > /dev/null
	echo 234 | ./onetimepad decrypt encfile.3 kf-dec plainfile.3 > /dev/null
	echo 234 | ./onetimepad decrypt encfile.2 kf-dec plainfile.2 > /dev/null
	echo 234 | ./onetimepad decrypt encfile.4 kf-dec plainfile.4 > /dev/null
	echo 234 | ./onetimepad decrypt encfile.5 kf-dec plainfile.5 > /dev/null
	@diff inputfile plainfile.1.a && echo "  decrypted file 1: ok" || echo "  decrypted file 1: error"
	@diff inputfile plainfile.1.b && echo "  decrypted file 1: ok" || echo "  decrypted file 1: error"
	@diff inputfile plainfile.1.c && echo "  decrypted file 1.c: error" || echo "  decrypted file 1.c: ok (should differ)"
	@diff inputfile plainfile.2   && echo "  decrypted file 2: ok" || echo "  decrypted file 2: error"
	@diff inputfile plainfile.3   && echo "  decrypted file 3: ok" || echo "  decrypted file 3: error"
	@diff inputfile plainfile.4   && echo "  decrypted file 4: ok" || echo "  decrypted file 4: error"
	@diff inputfile plainfile.5   && echo "  decrypted file 1.c: error" || echo "  decrypted file 5: ok (should differ)"
	@echo cleanup…
	@rm  inputfile kf-enc.public kf-dec plainfile.* encfile.*
