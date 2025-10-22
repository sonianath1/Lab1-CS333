//Sonia Nath - cae-oxr.c



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

# define OPTIONS "edc:x:hD" // double colon means optional arg

static int KC_INDEX = 0; // index of key to keep track of ceasar_cihper 
static int KX_INDEX = 0; // index of key to keep track of xor cipher
static const int BUFFER = 1000;
static const int SHIFT = 32;


char cesar_encrypt(char _char, char* key, bool crypt);
unsigned char xor_encrypt(unsigned char _byte, char* key);



int main(int argc, char *argv[])
{
	char* c_key = NULL; // key for ceasar encryption
	char* x_key = NULL;
	bool crypt = true; // true means to encrypt, false means to decrypt 
	ssize_t br = 0;
	char buffer[BUFFER];

	{
		int opt = 0;

		while ((opt = getopt(argc, argv, OPTIONS)) != -1)
		{

			switch(opt)
			{
				case 'e':
					crypt = true; // encryption	
					break;

				case 'd':
					crypt = false; // decrpytion
					break;

				case 'c':
					if (optarg)
						c_key = optarg;
					break;
				case 'x':
					if (optarg)
						x_key = optarg;
					break;

				case 'h':
					printf("\t-e        encrypt text (this is the default)\n");
					printf("\t-d        decrypt text\n");
					printf("\t-c str    string to use for caesar cypher algorithm (default NULL)\n");
					printf("\tstr       string to use for xor cypher algorithm (default NULL)\n");
					printf("\t-D        enable diagnostic output\n");
					printf("\t-h        show this amazing help and exit\n");
					exit(EXIT_SUCCESS);	
					break;

				case 'D':
					// if i have time
					break;

				default:
					fprintf(stderr, "unknown option %c\n", opt);
					exit(EXIT_FAILURE);
					break;	
			}	
		}
	}


	while ((br = read(STDIN_FILENO, buffer, BUFFER)) != 0) 
	{
		int elements = br / sizeof(char);
		KC_INDEX = 0;
		KX_INDEX = 0;


		if (crypt) 
		{
			// first do cesar cipher
			for (int i = 0; i < elements; ++i)
				buffer[i] = cesar_encrypt(buffer[i], c_key, crypt);		

			// second do xor 	
			for (ssize_t i = 0; i < br; ++i)
				buffer[i] = xor_encrypt(buffer[i], x_key);	

		}
		else 
		{	
			// second do xor 	
			for (ssize_t i = 0; i < br; ++i)
				buffer[i] = xor_encrypt(buffer[i], x_key);	

			// first do cesar cipher
			for (int i = 0; i < elements; ++i)
				buffer[i] = cesar_encrypt(buffer[i], c_key, crypt);		
		}

		write(STDOUT_FILENO, buffer, br);
	}

	exit(EXIT_SUCCESS);
}


char cesar_encrypt(char _char, char* key, bool crypt)
{

	if (key) 
	{
		int k_len = strlen(key);
		int base = 32; // where ' ' is in ascii
		int range = 95; // the range of printable chars 
				
		if (isprint(_char))
		{
			int shift = key[KC_INDEX % k_len] - SHIFT;
			if (crypt)
				_char = base + ((_char - base + shift + range) % range);
//				_char += shift;
			else
				_char = base + ((_char - base -  shift + range) % range);
//				_char -= shift;

			++KC_INDEX;	
		}	
	}
	return _char;
}


unsigned char xor_encrypt(unsigned char _byte, char* key)
{
	if (key)
	{
		int k_len = strlen(key);

		_byte ^= key[KX_INDEX % k_len];
		++KX_INDEX;
	}

	return _byte;
}
