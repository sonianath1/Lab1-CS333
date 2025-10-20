//Sonia Nath - cae-oxr.c



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

# define OPTIONS "edc:x:hD" // double colon means optional arg

static int K_INDEX = 0; // index of key to keep track of
static const int SHIFT = 32;

char* cesar_encrypt(char* key, char* text); // calls cesar cipher then xor
					    //char* decrypt(char key[], char text[]); // calls xor then cesar cipher




int main(int argc, char *argv[])
{
	char* key = NULL; // key for ceasar encryption
			  //	char* x_key = NULL; // key for xor excryption
	char* text = NULL; // text to encrypt or decrpyt
	bool crypt = true; // true means to encrypt, false means to decrypt 


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
				case 'x':
					if (optarg)
					{
						//						x_key = malloc(100 * sizeof(char));
						//						strcpy(x_key, optarg);
						key = optarg;
					}
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

	text = malloc(100 * sizeof(char));	
	scanf("%s", text);

	if (crypt)
		text = cesar_encrypt(key, text);
	//	else
	//		text = decrypt(x_key, text);	

	printf("\nHere is your text: %s\n", text);	

	//	if (c_key) free(c_key);
	//	if (x_key) free(x_key);
	if (text) free(text);

	exit(EXIT_SUCCESS);
}

char* cesar_encrypt(char* key, char* text) // calls cesar cipher 
{
	if (key)
	{
		int t_len = strlen(text);
		int k_len = strlen(key);

		for (int i = 0; i < t_len; ++i)
		{
			// add a thing to make sure it doesnt go out of bound for shifitng the text. ex: z becomes a printable char.
			if (isprint(text[i]))
			{
				int shift = key[K_INDEX % k_len] - SHIFT;
				text[i] += shift;
				++K_INDEX;
			}
		}	
	}
	return text; // returns text unaffected if key DNE
}
