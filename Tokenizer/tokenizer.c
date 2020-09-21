#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum token{delimiter, word, integer, decimal, octal, hexadecimal, floating, C_op, left_brack, right_brack};

const char *token_type[] = {
	"delimiter",
	"word",
	"integer",
	"decimal",
	"octal",
	"hexadecimal",
	"floating",
	"C_op",
	"left bracket",
	"right bracket"
};

struct t_index{//Contains data of where in string token is
	enum token type;
	char* value;
};

enum token getType(char* input, int start){//will return the token type from current index
	if(input[start] == ' ' || input[start] == 92) //92 is backslash '\'
		return delimiter;
	if(isalpha(input[start]))
		return word;
	if(isdigit(input[start]))
		return integer;
	if(input[start] == '[')
		return left_brack;
	if(input[start] == ']')
		return right_brack;
}

void printOutput(char* input, int start, int end) {
	if(getType(input, start) == 0 || start > strlen(input)) {
		return;
	}
	printf("%s: ", token_type[getType(input, start)]); 
	for(int i = start; i < end; i++) {
		printf("%c", input[i]);
	}
	printf("\n");
}

void tokenize(char* input) {
	int length = strlen(input);
	int start = 0; //first token must have index 0
	int end = 0;
	printf("length of string: %d\n\n", length);
	for(int i = 0; input[i]!='\0'; i++) {
		if(getType(input, i) == 0) {
			if(input[i + 1] == 't' || input[i + 1] == 'v' || input[i + 1] == 'f' || input[i + 1] == 'n' || input[i + 1] == 'r') {
				i++;
			} else {
				start = i + 1;
			}
		} else {
			int currentValue = getType(input, start);
			for(int j = start; j < length; j++) {
				if(getType(input, j) == currentValue)
					end = j + 1;
				else
					break;
			}
			printOutput(input, start, end);
			goto end;
		}
		end: start = end;
	}
}

int main(int argc, char *argv[]) {
	tokenize(argv[1]);
	printf("\n");
	return 0;
}
