#include <stdio.h>
#include "dictionary.h"
//#include "dictionary.c"
//#include "spell.c"

int main(int argc, char **argv)
{
	// Hash table is an array of linked lists.
	node* hashtable[HASH_SIZE];
	printf("%s\n", argv[0]);
	
	if (load_dictionary("wordlist.txt", hashtable))
	{
		printf("Dictionary loaded successfully\n");
	}
	else 
	{
		printf("Dictionary failed to load\n");
	}
	
	char* misspelled[MAX_MISSPELLED];
	FILE * fp;
	
	fp = fopen("test.txt", "r");
	
	int num_misspelled = check_words(fp, hashtable, misspelled);
	printf("%d misspelled words.\n", num_misspelled);
	
	for (int i = 0; i < num_misspelled; i++)
	{
		printf("%s\n", misspelled[i]);
	}
	
	/*
	char word1[10] = "Cat";
	char word2[10] = "drdrdrdr";
	
	if(check_word(word1, hashtable))
	{
		printf("This word spelled correctly: ");
		puts(word1);
	}
	else
	{
		printf("Something went wrong!\n");
	}
	
	if(check_word(word2, hashtable))
	{
		printf("Something went wrong!\n");
	}
	else
	{
		printf("This word not found: ");
		puts(word2);
	}
	*/
	fclose(fp);
	return 0;
}

