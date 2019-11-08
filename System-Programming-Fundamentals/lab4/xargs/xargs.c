#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_SIZE 256
#define MAX_RESULT_SIZE 512

int main(int argc, char *argv[]) {	
    char* input_command = (char*)malloc(MAX_COMMAND_SIZE * sizeof(char));
    char* progr_arg = (char*)malloc(MAX_COMMAND_SIZE * sizeof(char));
	
    for (int i = 1; i < argc; i++) {
       strcat(input_command, argv[i]);
       strcat(input_command, " ");
    }
   
    while (scanf("%s", progr_arg) != EOF) {
        char* result = (char*)malloc(MAX_RESULT_SIZE * sizeof(char));
        strcpy(result, input_command);
        strcat(result, " ");
        strcat(result, progr_arg);
        system(result);
    }
	
    return 0;
}
