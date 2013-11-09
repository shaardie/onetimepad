#include <stdio.h>
#include <stdint.h>
#include <gcrypt.h>

int twofish_encrypt( char * in,
		uint64_t inlen,
		char * key ) {
	
	/* define error and handler */
	gcry_error_t error;
	gcry_cipher_hd_t hd;

	/* open cipher context */
	error = gcry_cipher_open (&hd,
			GCRY_CIPHER_TWOFISH,
			GCRY_CIPHER_MODE_CBC,
			GCRY_CIPHER_SECURE);

	if (error) {
		fprintf(stderr, "Could not open cipher context.");
		return 1;
	}

	/* set key */
	error = gcry_cipher_setkey(hd,
			key,
			32);
	
	if (error) {
		fprintf(stderr, "Could not set key.");
		return 1;
	}

	/* encrypt */
	error = gcry_cipher_encrypt (hd,
			in,
			inlen,
			NULL,
			0);

	if (error) {
		fprintf(stderr, "Could not encrypt.");
		return 1;
	}

	/* close cipher context */
	gcry_cipher_close(hd); 

	return 0;
}

int twofish_decrypt( char * in,
		uint64_t inlen,
		char * key ) {
	
	/* define error and handler */
	gcry_error_t error;
	gcry_cipher_hd_t hd;

	/* open cipher context */
	error = gcry_cipher_open (&hd,
			GCRY_CIPHER_TWOFISH,
			GCRY_CIPHER_MODE_CBC,
			GCRY_CIPHER_SECURE);

	if (error) {
		fprintf(stderr, "Could not open cipher context.");
		return 1;
	}

	/* set key */
	error = gcry_cipher_setkey(hd,
			key,
			32);
	
	if (error) {
		fprintf(stderr, "Could not set key.");
		return 1;
	}

	/* encrypt */
	error = gcry_cipher_decrypt (hd,
			in,
			inlen,
			NULL,
			0);

	if (error) {
		fprintf(stderr, "Could not decrypt.");
		return 1;
	}

	/* close cipher context */
	gcry_cipher_close(hd); 

	return 0;
}
