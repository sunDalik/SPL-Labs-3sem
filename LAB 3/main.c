#include <stdio.h>
#include <stdlib.h>

const int x[] = { 1, 2, 3, 4, 5 };
const int y[] = { 5, 4, 3, 2, 1};

int scalar(const int x[], const int y[], size_t xsz, size_t ysz) {
    size_t i; 
    int res = 0;
    if (xsz != ysz) {
    	puts("Error! Arrays must be of a same size!");
    	return 0;
    }
    for (i = 0; i < xsz; i++) {
        res += x[i] * y[i];
	}
	printf("The scalar product is: %d\n", res);
    return res;
}

void print_array(const int x[], size_t sz) {
	int i;
	printf("{ ");
	for (i = 0; i < sz; i++) {
		printf("%d", x[i]);
		if (i < sz - 1) {
			printf("%s", ", ");
		}
	}
	printf(" }\n");
}

int is_prime(unsigned long num) {
	int i;
	int is_prime = 1;
	if (num == 0 || num == 1) {
		return 0;
	}
	for(i = 2; i*i <= num; i++) {
		if (num % i == 0) {
			is_prime = 0;
			break;
		}
	}
	return is_prime;
}

void check_if_long(const char string[], size_t stringsize) {
	if (stringsize < 20) {
		//ok
	}
	else if (stringsize > 20) {
		//error
	}
	else if (stringsize == 20) {
		//symbol-wise checking
	}
}


int main(void) {
	unsigned long num;
	print_array(x, sizeof(x)/sizeof(x[0]));
	print_array(y, sizeof(y)/sizeof(y[0]));
    scalar( x, y, sizeof(x)/sizeof(x[0]), sizeof(y)/sizeof(y[0]) );

    printf("Enter a number:\n");
    if (scanf("%lu", &num) == 0) {
    	printf("%s\n", "Not a Number!");
    }

    else if (is_prime(num) == 1) {
    	printf("%lu is prime!\n", num);
    }
    else {
    	printf("%lu is NOT prime.\n", num);
    }
    return 0;
}