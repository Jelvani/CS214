#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


enum token{word, decimal, octal, hexadecimal, floating, C_op};

struct t_index{//Contains data of where in string token is
	enum token type;
	char* value;
};

enum token getType(char* input, int index){//will return the token type from current index



}

void tokenize(char* input) {
	int length = strlen(input);
	int start = 0; //first token must have index 0

	for(int i = 0; input[i]!='\0'; i++) {
		if(input[i] == ' ') {
			i++;
		} else if(input[i] == 92 && i < length) { //checks for the '\' character
			if(input[i + 1] == 't' || input[i + 1] == 'v' || input[i + 1] == 'f' || input[i + 1] == 'n' || input[i + 1] == 'r') {
				i += 2;
			}
		} else {
			printf("%c", input[i]);
		}
	}
}

int main(int argc, char *argv[]) {
	tokenize(argv[1]);
	return 0;
}
