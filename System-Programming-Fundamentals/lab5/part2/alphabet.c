#include <stdio.h>
#include <ctype.h>

char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                     'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

void print_alphabet() {
    printf("\n");
    for (int i = 0; i < 26; ++i)
        printf("%c ", alphabet[i]);
    printf("\n");
}

void invert_case() {
    for (int i = 0; i < 26; ++i)
        alphabet[i] = islower(alphabet[i]) ? toupper(alphabet[i]) : tolower(alphabet[i]);
}

void swap_alphabet() {
    int i = 0, j = 25;
    while (i < j) {
        char temp = alphabet[i];
        alphabet[i] = alphabet[j];
        alphabet[j] = temp;
        i++;
        j--;
    }
}

int count_uppercase() {
    int upper = 0;
    for (int i = 0; i < 26; ++i)
        if (isupper(alphabet[i])) upper++;
    return upper;
}