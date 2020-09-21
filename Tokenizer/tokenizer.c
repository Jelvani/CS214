#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


//TODO create header for all definitions
enum token{
	NONE,
	delimiter,
	word,
	decimal,
	octal,
	hexadecimal,
	floating,
	leftparanthese,
	rightparanthese,
	leftbrace,
	rightbrace,
	structmember,
	structpointer,
	address,
	minus,
	negate,
	complement,
	increment,
	decrement,
	multiply,
	divide,
	modulus,
	add,
	shiftright,
	shiftleft,
	lessthan,
	greaterthan,
	leq,
	geq,
	equals,
	notequals,
	bitand,
	bitxor,
	bitor,
	logicand,
	logicor
	};

const char *token_type[] = {
	"delimiter",
	"word",
	"decimal",
	"octal",
	"hexadecimal",
	"floating",
	"leftbrace",
	"rightbrace"
};

struct t_index{//Contains data of where in string token is
	enum token type;
	char* value;
};


int isoctal(char c){//returns if char is octal or not (0-7)
	if(c>='0' && c<='7'){
		return 1;
	}
	return 0;
}

//TODO, can decimal,hex,octal, be only 1 character long? ex. '3', '0x', '0', ...\
//TODO, check for pointer positions when looking ahead in getType()
enum token getType(char* input, int start, enum token currentType){//will return the token type from current index, order of if statements is also important to logic
	if(isspace(input[start])){ //check for whitespace delimiter
		return delimiter;
	}else if((input[start]=='0' && tolower(input[start+1])=='x' && isxdigit(input[start+2])) || (isxdigit(input[start]) && currentType==hexadecimal)){//returns if current char is hexadecimal
		return hexadecimal;
	}else if((isalpha(input[start]) && isalnum(input[start+1])) || (isalnum(input[start] && currentType==word))){//returns if current char is part of a word
		return word;
	}else if((input[start]=='0' && isoctal(input[start+1])) || (isoctal(input[start]) && currentType==octal)){//returns if current type is part of octal
		return octal;
	}else if(isdigit(input[start]) && (input[start] != '0' || currentType==decimal)){ //returns if current char is part of a decimal integer
		return decimal;
	}
	return -1;

}

void printOutput(char* input, int start, int end) {
	if(getType(input, start,NONE) == 0 || start > strlen(input)) {
		return;
	}
	printf("%s: ", token_type[getType(input, start,NONE)]); 
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
		if(getType(input, i,NONE) == 0) {
			if(input[i + 1] == 't' || input[i + 1] == 'v' || input[i + 1] == 'f' || input[i + 1] == 'n' || input[i + 1] == 'r') {
				i++;
			} else {
				start = i + 1;
			}
		} else {
			int currentValue = getType(input, start,NONE);
			for(int j = start; j < length; j++) {
				if(getType(input, j,NONE) == currentValue)
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
	//tokenize(argv[1]);
	printf("%d\n",isoctal('f'));

	printf("%d\n",getType(argv[1],0,NONE));
	printf("\n");
	return 0;
}
