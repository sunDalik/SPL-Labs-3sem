#include <stdio.h>
#include <ctype.h>

char alphabet[26] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

void print_alphabet() {
    printf("\n");
    char *alpha = alphabet;
    while (*alpha) {
        printf("%c ", *alpha);
        alpha++;
    }
    printf("\n");
}

void invert_case() {
    for (int i = 0; i < 26; ++i) {
        alphabet[i] = (islower(alphabet[i])) ? toupper(alphabet[i]) : tolower(alphabet[i]);
    }
}

void swap_alphabet() {
    int i = 0, j = 25;
    char c;
    while (i < j) {
        c = alphabet[i];
        alphabet[i] = alphabet[j];
        alphabet[j] = c;
        i++;
        j--;
    }
}

int count_uppercase() {
    int upper = 0;
    for (int i = 0; i < 26; ++i)
        if (alphabet[i] <= 'Z') upper++;
    return upper;
}