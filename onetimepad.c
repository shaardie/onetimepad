#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypt.h"
#include "header.h"
#include "generate.h"
#include "import.h"
#include <getopt.h>
#include "twofish.h"

/* usage print a help */
void usage(const char* prgm) {
	printf( "\nUsage: %s [options] command [command options]\n\n"
			"-- Commands: ----\n"
			"\tgenerate        -- Generate a new keyfile\n"
			"\timport          -- Import a keyfile\n"
			"\tencrypt         -- Encrypt a file\n"
			"\tdecrypt         -- Decrypt a file\n\n"
			"-- Options ----\n"
			"\t-h              -- Show this help\n"
			"\t-r              -- Toggle reseed of OpenSSL PRNG (default: reseed)\n"
			"\t-k              -- Toggle if the key part used for decryption shall be\n"
			"\t                   kept to enable a second decryption. (default: overwrite)\n\n"
			"\t-l              -- Change Library to use for encryption and random number generation to Gcrypt library\n"
			"\t-q              -- Use very strong pseudo random number generator\n"
			"\t-s importstatus -- Set status for import:\n"
			"\t                   1 to import key as decrypt key\n"
			"\t                   2 to import key as encrypt key\n"
			"For more informations read the man page.\n\n"
			,prgm);
}

/* Read in arguments an transfer them
 * to encrypt, decrypt, generate and import */
int main(int argc, char *argv[]) {

	/* Set default values 
	 * Default values:
	 * - Reseed
	 * - Do not keep keys 
	 * - use openssl 
	 * - use gcrypt random quality strong
	 * - Set import status decrypt and encrypt */
	config_t config = { 1, 0, USE_OPENSSL, GCRY_STRONG_RANDOM, STATUS_UNUSED_KEY };

	/* Read in options */
	int c;
	while ((c = getopt (argc, argv, "rklqs:h")) != -1) {
		switch (c) {
			case 'r': 
				config.reseed = !config.reseed;
				break;
			case 'k': 
				config.keep_key = !config.keep_key;
				break;
			case 'l':
				config.cryptlib = USE_LIBGCRYPT;
				break;
			case 'q':
				config.random_quality = GCRY_VERY_STRONG_RANDOM;
				break;
			case 's':
				config.get_status =  atoi(optarg);
				break;
			case 'h':
				usage(*argv);
				return 0;
		}
   }

	/* Show help if there is no command  */
	if (argc - optind < 1) {
		usage(*argv);
		return 0;
	}

	/* Go through the commands */
	const char* cmd = argv[optind];

	/* Case: generate */
	if (!strcmp("generate", cmd)) {
		if (argc - optind == 3) {
			return generate( &config, atol(argv[optind+1]), argv[optind+2]);
		}
		printf("Command options: generate [size (kb)] [keyfile]\n");
		return 0;
	}

	/* Case: import */
	if (!strcmp("import", cmd)) {
		if (argc - optind == 2) {
			return import( &config, argv[optind+1]);
		}
		printf("Command options: import [keyfile]\n");
		return 0;
	}

	/* Case: encrypt */
	if (!strcmp("encrypt", cmd)) {
		if (argc - optind == 4) {
			return encrypt(&config, argv[optind+1], argv[optind+2], argv[optind+3]);
		}
		printf("Command options: encrypt [infile] [keyfile] [outfile]\n");
		return 0;
	}

	/* Case: decrypt */
	if (!strcmp("decrypt", cmd)) {
		if (argc - optind == 4) {
			return decrypt(&config, argv[optind+1], argv[optind+2], argv[optind+3]);
		}
		printf("Command options: decrypt [infile] [keyfile] [outfile]\n");
		return 0;
	}

	/* Case: default */
	usage(*argv);
	return 0;
}
