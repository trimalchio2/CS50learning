#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char argv[])
{
    char plaintext[] = get_string("plaintext: ");
    for(int i = 0; i < strlen(plaintext); i++)
    {
        if (plaintext[i] >= 'a' && plaintext[i] <= 'z')
        {
        int shift = (plaintext[i] - 97 + atoi(argv[1])) % 26;
        printf("%c", shift + 97);
        }
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
}