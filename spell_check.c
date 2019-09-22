#include <stdio.h>
#include "dictionary.h"
//#include "dictionary.c"
//#include "spell.c"

int main(int argc, char **argv)
{
	// Hash table is an array of linked lists.
	node* hashtable[HASH_SIZE];
	printf("%s\n", argv[0]);
	
	// if (load_dictionary("wordlist.txt", hashtable))
	if (load_dictionary(argv[2], hashtable))
	{
		printf("Dictionary loaded successfully\n");
	}
	else 
	{
		printf("Dictionary failed to load\n");
	}
	
	char* misspelled[MAX_MISSPELLED];
	FILE * fp;
	
	// fp = fopen("test.txt", "r");
	fp = fopen(argv[1], "r");
	
	int num_misspelled = check_words(fp, hashtable, misspelled);
	printf("%d misspelled words.\n", num_misspelled);
	
	for (int i = 0; i < num_misspelled; i++)
	{
		printf("%s\n", misspelled[i]);
	}
	
	fclose(fp);
	return 0;
}

