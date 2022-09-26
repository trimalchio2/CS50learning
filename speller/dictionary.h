// Declares a dictionary's functionality
// ensure that clang will only compile it once

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// Prototypes
//const means you won't be able to change them, accidentally or otherwise!

bool check(const char *word);
unsigned int hash(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);
void printdict(int prefix);

#endif // DICTIONARY_H
