all: onetimepad.c crypt.o generate.o
	gcc -Wall onetimepad.c crypt.o generate.o -o onetimepad `libgcrypt-config --cflags --libs`

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

test: all
	dd if=/dev/urandom of=inputfile count=10 bs=1M
	./onetimepad generate 50000 keyfile
	./onetimepad encrypt inputfile keyfile encfile.1
	./onetimepad encrypt inputfile keyfile encfile.2
	./onetimepad encrypt inputfile keyfile encfile.3
	./onetimepad encrypt inputfile keyfile encfile.4
	./onetimepad decrypt encfile.1 keyfile.public plainfile.1
	./onetimepad decrypt encfile.3 keyfile.public plainfile.3
	./onetimepad decrypt encfile.2 keyfile.public plainfile.2
	./onetimepad decrypt encfile.4 keyfile.public plainfile.4
	@diff inputfile plainfile.1 && echo "  decrypted file 1: ok" || echo "  decrypted file 1: error"
	@diff inputfile plainfile.2 && echo "  decrypted file 2: ok" || echo "  decrypted file 2: error"
	@diff inputfile plainfile.3 && echo "  decrypted file 3: ok" || echo "  decrypted file 3: error"
	@diff inputfile plainfile.4 && echo "  decrypted file 4: ok" || echo "  decrypted file 4: error"
	@echo cleanup…
	@rm inputfile keyfile plainfile.* encfile.*
