#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

const int x[] = { 1, 2, 3, 4, 5 };
const int y[] = { 5, 4, 3, 2, 1};
const int y_wrong[] = { 5, 4, 3, 2};

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


int main(void) {
	unsigned long num;
	char input_buffer[64]; /* 64 chars is enough I guess */
  	char* input_end_char;  

	print_array(x, sizeof(x)/sizeof(x[0]));
	print_array(y, sizeof(y)/sizeof(y[0]));
    scalar( x, y, sizeof(x)/sizeof(x[0]), sizeof(y)/sizeof(y[0]) );

    print_array(x, sizeof(x)/sizeof(x[0]));
	print_array(y_wrong, sizeof(y_wrong)/sizeof(y_wrong[0]));
    scalar( x, y_wrong, sizeof(x)/sizeof(x[0]), sizeof(y_wrong)/sizeof(y_wrong[0]) );

    printf("Enter a number:\n");
   	scanf("%63s", input_buffer);


  	errno = 0;
  	num = strtoul((const char*) input_buffer, &input_end_char, 10);
  	if (errno == ERANGE || *input_end_char != '\0' || input_buffer[0] == '-') {
    	fprintf(stderr, "Input is not of the type \"unsigned long\" \n");
    	return 1;
  	}

    else if (is_prime(num) == 1) {
    	printf("%lu is prime!\n", num);
    }
    else {
    	printf("%lu is NOT prime.\n", num);
    }
    return 0;  
}