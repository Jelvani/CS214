#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tokenizer.h"

int isoctal(char c){//returns 1 if char is octal, else 0
	if(c>='0' && c<='7'){
		return 1;
	}
	return 0;
}


/*
The following 'findX' functions take an input of the char pointer to the begining of the inputted string, and the current index
*/
int findWord(char *input, int start){//returns number of chars in detected word token
	int length = 0;
	while(isalnum(input[start+length])){
		length++;
	}
	return length;
}

int findDecimal(char *input, int start){//returns number of chars in detected decimal token
	int length = 0;
	while(isdigit(input[start+length])){
		length++;
	}
	return length;
}

int findOctal(char *input, int start){//retursn number of chars in detected octal token
	int length = 0;
	while(isoctal(input[start+length])){
		length++;
	}
	return length;
}

int findHex(char *input, int start){//returns number of chars in detected hexadecimal token
	int length = 0;
	while(isxdigit(input[start+length])){
		length++;
	}
	return length;
}

int findFloat(char *input, int start){//returns the number of chars in detected float token
	int length = 0;
	int scnot = 0;
		while(isdigit(input[start+length])){
			length++;
		}
		if(length==0){
			return length;
		}
		if(input[start+length]=='.'){//if detect dot, find all decimals again
			if(isdigit(input[start+length+1])){
				length++;
				while(isdigit(input[start+length])){
					length++;
				}
			}else{
				return 0;
			}
		}else{
			return 0;
		}

		if(tolower(input[start+length])=='e'){
			scnot++;
			if(input[start+length+scnot] == '-' || input[start+length+scnot] == '+' ){
				scnot++;
			}
		}
		if(isdigit(input[start+length+scnot])){
			length+=scnot;
			while(isdigit(input[start+length])){
				length++;
			}
		}

	return length;
}

enum token findCop(char *input, int start){//returns the type of C operator detected, and if none are detcted, returns a -1
	int previous = -1;//stores longest length operator
	int chosen = -1;
	for(int i = LEFTPARENTHESIS; i<=MULTIPLYOPERATOR; i++){ 
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

enum token findCkeyword(char* input, int start){
	int previous = -1;//stores longest length operator
	int chosen = -1;
	for(int i = AUTO; i<= _IMAGINARY; i++){ 
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

void printDEBUG(enum token type){//ised for debugging purposes 
	printf("%s\n",token_type[type]);
}

void printWord(char* input, int start, int end) {//prints a substring. input is the pointer to the begining of a string, and the starting and ending index to be printed. Functions returns on index out of bounds
	int length = strlen(input);
	if(start<0 || end>length){
		return;
	}

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
	enum token tempType = 0;
	int tokenLength = 0;
	int tempLength = 0;
	while(start < length){


		if(strncmp(&input[start],"/*",2)==0){//for multiline c comments
			start+=2;
			while(strncmp(&input[start],"*/",2)!=0){
				start++;
			}
			start+=2;
			continue;
		}
		if(strncmp(&input[start],"//",2)==0){//for singleline c comments
			start+=2;
			while(start < length){
				if(input[start]=='\n'){
					start++;
					break;
				}
				start++;

			}
			continue;
		}

		

		if(isspace(input[start])){//on whitespace, ignore and restart
			start++;
			continue;			
		}
		if(findCkeyword(input,start)!=-1){
			tempType = findCkeyword(input,start);
			tempLength = strlen(c_op_vals[tempType-6]);
			if(tempLength>tokenLength){
				type=tempType;
				tokenLength=tempLength;
			}
		}
		if(findCop(input,start)!=-1){//if found c op, increment tokelnegth
			tempType = findCop(input,start);
			tempLength = strlen(c_op_vals[tempType-6]);
			if(tempLength>tokenLength){
				type=tempType;
				tokenLength=tempLength;
			}
		}if(isalpha(input[start])){
			tempType = WORD;
			tempLength = findWord(input,start);
			if(tempLength>tokenLength){
				type=tempType;
				tokenLength=tempLength;
			}
		}
		if(tolower(input[start+1])=='x' && findHex(input,start+2)>0){
			tempType = HEX;
			tempLength = findHex(input,start+2)+2;
			if(tempLength>tokenLength){
				type=tempType;
				tokenLength=tempLength;
			}
		}else{//choose max length from octal, deicimal, or float
			int t1 = findOctal(input,start);
			int t2 = findDecimal(input,start);
			int t3 = findFloat(input,start);
			if(t1>=t2 && t1>=t3 && t1!=1 && input[start] == '0'){
				tempType = OCTAL;
				tempLength = t1;
			}else if(t2>=t3 && t2>=t3){
				tempType = DECIMAL;
				tempLength = t2;
			}else{
				tempType = FLOAT;
				tempLength = t3;
				}
			if(tempLength>tokenLength){
				type=tempType;
				tokenLength=tempLength;
			}
		}

		if(tokenLength==0){//if encountered unkown character
			start++;
			continue;

		}

		if(type < AUTO){
			printf("%s: ", token_type[type]);
		}else{
			printf("c keyword: ");
		}
		printWord(input, start, start + tokenLength);
		printf("\n");
		start+=tokenLength;
		tokenLength=0;//reset length for finding largest token
		type=0;
	}

}

int main(int argc, char *argv[]) {
	if(argc != 2){
		printf("Please pass 1 argument to this program\n");
		return EXIT_FAILURE;
	}
	tokenize(argv[1]);
	return EXIT_SUCCESS;
}
