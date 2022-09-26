// Implements a dictionary's functionality
//testing sample ./speller50 texts/lalaland.txt > staff.txt

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    //check the prefix
    unsigned int prefix = 0;
    int number = 0;
    bool failure = true;
    prefix = hash(word);
    node *tmp = table[prefix];
    while(tmp != NULL && failure)
    {
        number = 0;
        failure = false;
        for (int i = 0; tmp->word[i] != '\0'; i++)
        {
            if((tmp->word[i] - word[i]) != 0 && (tmp->word[i] - word[i]) != 32)
            {
                failure = true;
                tmp = tmp->next;
                break;
            }
            number++;
        }
        if(!failure)
        {
            if (word[number] != '\0')
            {
                failure = true;
                tmp = tmp->next;
            }
        }
    }
    if (!failure)
    {
        return true;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    int stt = 0;
    int prefix = 0;
    node *tmp[N];
    node *n;
    FILE *dict = fopen(dictionary, "r");
    if (dict != NULL)
    {
        for (int i = 0; i < N; i++)
        {
            table[i] = NULL;
            tmp[i] = NULL;
        }
        char w;
        while (fread(&w, sizeof(char), 1, dict))
        {
            //check the set index
            if (stt == 0)
            {
                n = malloc(sizeof(node));
            }
            // Check the alphabetical characters
            if (isalpha(w) || (w == '\'' && stt > 0))
            {
                // Append character to word
                n->word[stt] = w;
                stt++;
            }
            else if (stt > 0)
            {
                n->word[stt] = '\0';
                n->next = NULL;
                prefix = hash(n->word);
                if (table[prefix] == NULL)
                {
                    table[prefix] = n;
                    tmp[prefix] = table[prefix];
                }
                else
                {
                    tmp[prefix]->next = n;
                    tmp[prefix] = tmp[prefix]->next;
                }
                stt = 0;
            }
        }
        fclose(dict);
        return true;
    }
    fclose(dict);
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    unsigned int n = 0;
    for (int i = 0; i < N; i++)
    {
        for (node *tmp = table[i]; tmp != NULL; tmp = tmp->next)
        {
            n ++;
        }
    }
    return n;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    bool success = true;
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
        if (table[i] != NULL)
        {
            success = false;
            break;
        }
    }
    return success;
}

void printdict(int prefix)
{
    for (node *rec = table[prefix]; rec != NULL; rec = rec->next)
    {
        for (int i = 0; rec->word[i] != '\0'; i++)
        {
            printf("%c", rec->word[i]);
        }
        printf("\n");
    }
}
