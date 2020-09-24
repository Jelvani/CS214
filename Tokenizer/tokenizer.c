#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tokenizer.h"

struct tokens{//Contains data of where in string token is
	enum token type;
	char* value;
	struct tokens* next;
};


int isoctal(char c){//returns if char is octal or not (0-7)
	if(c>='0' && c<='7'){
		return 1;
	}
	return 0;
}

int findWord(char *input, int start){//returns num of chars in detected word token
	int length = 0;
	while(isalnum(input[start+length])){
		length++;
	}
	return length;
}

int findDecimal(char *input, int start){//returns num of chars in detected decimal token
	int length = 0;
	while(isdigit(input[start+length])){
		length++;
	}
	return length;
}

int findOctal(char *input, int start){//retursn num of chars in detected octal token
	int length = 0;
	while(isoctal(input[start+length])){
		length++;
	}
	return length;
}

int findHex(char *input, int start){//returns num of chars in detected hex token
	int length = 0;
	while(isxdigit(input[start+length])){
		length++;
	}
	return length;
}

int findFloat(char *input, int start){//find num of chars in detected float token
	int length = 0;
	while((isdigit(input[start+length])) || (input[start+length] == '.') || (strncmp(&input[start+length],"e+",2) == 0) || (strncmp(&input[start]+length,"e-",2) == 0)){
		if((strncmp(&input[start+length],"e+",2) == 0) || (strncmp(&input[start]+length,"e-",2) == 0)){
			length+=2;
		}else{
			length++;
		}
	}
	return length;
}

enum token findCop(char *input, int start){//returns type C operator detected, or -1 if none
	int i = 0;
	int previous = -1;//stores longest length operator
	int chosen = -1;
	for(i = LEFTPARENTHESIS; i<=MULTIPLYOPERATOR; i++){ 
		int length = strlen(c_op_vals[i-6]);
		if(strncmp(&input[start], c_op_vals[i-6],length)==0){
			if(previous<length){
				previous = length;
				chosen = i;
			}
		}
	}
	return chosen;
}

void printDEBUG(enum token type){
	printf("%s\n",token_type[type]);
}

void printWord(char* input, int start, int end) {
	for(int i = start; i < end; i++) {
		printf("%c", input[i]);
	}
}


/*
The tokenize functon will take a pointer to a string and print out all detected tokens and type
*/
void tokenize(char* input) {
	int i =0;
	int start = 0;
	int end = 1;
	int length = strlen(input);
	enum token type = 0;
	int tokenLength;
	while(start < length){
		if(isspace(input[start])){
			start++;
			continue;
		}else if(findCop(input,start)!=-1){
			type = findCop(input,start);
			tokenLength = strlen(c_op_vals[type-6]);
		}else if(isalpha(input[start])){
			type = WORD;
			tokenLength = findWord(input,start);	
		}else{
			if(tolower(input[start+1])=='x' && findHex(input,start+2)>0){
				type = HEX;
				tokenLength = findHex(input,start+2)+2;
			}else{//choose max length from octal, deicimal, or float
				int t1 = findOctal(input,start);
				int t2 = findDecimal(input,start);
				int t3 = findFloat(input,start);
				if(t1>=t2 && t1>=t3 && t1!=1 && input[start] == '0'){
					type = OCTAL;
					tokenLength = t1;
				}else if(t2>=t3 && t2>=t3){
					type = DECIMAL;
					tokenLength = t2;
				}else{
					type = FLOAT;
					tokenLength = t3;
				}
			}

		}

		printf("%s: ", token_type[type]);
		printWord(input, start, start + tokenLength);
		printf("\n");
		//printDEBUG(type);
		start+=tokenLength;
	}
}

int main(int argc, char *argv[]) {
	if(argc<2){
		printf("Please pass 1 argument to this program\n");
		return EXIT_FAILURE;
	}
	tokenize(argv[1]);
	return EXIT_SUCCESS;
}
