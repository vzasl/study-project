#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
//#include "dictionary.c"

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
	// initialize hashtable[] with NULL
	for(int i = 0; i < HASH_SIZE; i++)
	{
        hashtable[i] = NULL;
    } 
	
	// open the dictiuonary file
	FILE * dict;
	
	dict = fopen(dictionary_file, "r");
	
	// check if the dictionary file opened properly
	if (dict == NULL) 
	{
		fclose(dict);
		return false;
	}
	
	char buffer[LENGTH+1];
	
	// loop through the lines of dictionary file
	// read each word into the buffer
	
	while (fscanf(dict, "%s", buffer) > 0)
	{
		// Allocate memory for a new node.
        node* new_node = malloc(sizeof(node));
        // Set node's next pointer to NULL.
        new_node->next = NULL;
        
        // copy buffer into the word of node
        strcpy(new_node->word, buffer);
        
        // get the hash value of copied word
        int bucket = hash_function(buffer);
        
        // If it's the first node being added to the bucket, replace 
        // the NULL value with the new node.
        if (hashtable[bucket] == NULL)
        {
            hashtable[bucket] = new_node;
        }
		
		// if not, set new node's pointer to the first node in the linked list
        // 
        else
        {
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }
		
	}
	
	fclose(dict);
	
	return true;
}

bool check_word(const char* word, hashmap_t hashtable[])
{
	
	char stripped_word[strlen(word)];
	strcpy(stripped_word, word);
	
	// set int bucket to the output of hash_function(word)
	int bucket = hash_function(stripped_word);
	
	// set hashmap_t cursor equal to hashtable[bucket]
	node* cursor = hashtable[bucket];
	
	while (cursor != NULL)
    {
        if (strcmp(stripped_word, cursor->word) == 0)
        {
			return true;
        }
        cursor = cursor->next;
    }
    
    for(int i = 0; stripped_word[i]; i++)
    {
		stripped_word[i] = tolower(stripped_word[i]);
	}
	
	bucket = hash_function(stripped_word);
	
	
	cursor = hashtable[bucket];
	
	while (cursor != NULL)
    {
        if (strcmp(stripped_word, cursor->word) == 0)
        {
			return true;
        }
        cursor = cursor->next;
    }
	
	
	
	return false;
}


int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
	int num_misspelled = 0;
	char line[1000];
	

	while (fgets(line, 1000, fp))
	{
		
		// read the line
		int i, j;
		char *found, *word;
		char *string = strdup(line);
		
		while ((found = strsep(&string, " ")) != NULL)
		{
			word = strdup(found);
        
			for(i = 0; word[i] != '\0'; ++i)
			{
				while (!( (word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= '0' && word[i] <= '9') || word[i] == '\0' || word[i] == '\'') )
				{
					for(j = i; word[j] != '\0'; ++j)
					{
						word[j] = word[j+1];
					}
					word[j] = '\0';
				}
			}
		
        
			if(!check_word(word, hashtable))
			{
				// append word to misspelled
				misspelled[num_misspelled] = word;
				
				// increment num_misspelled
				num_misspelled++;
			}
			
		}
		
	}

	return num_misspelled;
}

