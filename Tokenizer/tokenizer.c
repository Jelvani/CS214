#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


enum token{word, decimal, octal, hexadecimal, floating, C_op};

struct t_index{//Contains data of where in string token is
	enum token type;
	char* value;
};

int main(int argc, char *argv[]) {

	return 0;
}


enum token getType(char* input, int index){//will return the token type from current index



}
void tokenize(char* input){
	int length = 0;
	length = strlen(input);
	int start = 0; //first token must have index 0
	int i;
	for(i = 0; input[i]!='\0'; i++){
		if()
	}
	return 0;
}
