#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


//TODO create header for all definitions
enum token{
	DELIMETER,
	WORD,
	DECIMAL,
	OCTAL,
	HEX,
	FLOAT,
	LEFTPARENTHESIS,//(
	RIGHTPARENTHESIS,//)
	LEFTBRACKET,//[
	RIGHTBRACKET,//]
	STRUCTMEMBER,//.
	STRUCTPOINTER,//->
	SIZEOF,//sizeof
	COMMA,//,
	NEGATE,//!
	ONESCOMPLEMENT,//~
	SHIFTRIGHT,//>>
	SHIFTLEFT,//<<
	BITWISEXOR,//^
	BITWISEOR,//|
	INCREMENT,//++
	DECREMENT,//--
	ADDITION,//+
	DIVISION,///
	LOGICALOR,//||
	LOGICALAND,//&&
	CONDITIONALTRUE,//?
	CONDITIONALFALSE,//:
	EQUALITYTEST,//==
	INEQUALITYTEST,//!=
	LESSTHANTEST,//<
	GREATERTHANTEST,//>
	LESSTHANEQUAL,//<=
	GREATERTHANEQUAL,//>=
	ASSIGNMENT,//=
	PLUSEQUALS,//+=
	MINUSEQUALS,//-=
	TIMESEQUALS,//*=
	DIVIDEEQUALS,///=
	MODEQUALS,//%=
	SHIFTRIGHTEQUALS,//>>=
	SHIFTLEFTEQUALS,//<<=
	BITWISEANDEQUALS,//&=
	BITWISEXOREQUALS,//^=
	BITWISEOREQUALS,//|=
	ADDRESSOPERATOR,//&
	MINUSOPERATOR,//-
	MULTIPLYOPERATOR,//*
	};

const char *c_op_vals[] = {
	"(",
	")",
	"[",
	"]",
	".",
	"->",
	"sizeof",
	",",
	"!",
	"~",
	">>",
	"<<",
	"^",
	"|",
	"++",
	"--",
	"+",
	"/",
	"||",
	"&&",
	"?",
	":",
	"==",
	"!=",
	"<",
	">",
	"<=",
	">=",
	"=",
	"+=",
	"-=",
	"*=",
	"/=",
	"%=",
	">>=",
	"<<=",
	"&=",
	"^=",
	"|=",
	"&",
	"-",
	"*",
};
const char *token_type[] = {
	"delimiter",
	"word",
	"decimal integer",
	"octal integer",
	"hexadecimal integer",
	"float",
	"left parenthesis",
	"right parenthesis",
	"left bracket",
	"right bracket",
	"structure member",
	"structure pointer",
	"sizeof",
	"comma",
	"negate",
	"1s complement",
	"shift right",
	"shift left",
	"bitwise XOR",
	"bitwise OR",
	"increment",
	"decrement",
	"addition",
	"division",
	"logical OR",
	"logical AND",
	"conditional true",
	"conditional false",
	"equality test",
	"inequality test",
	"less than test",
	"greater than test",
	"less than or equal test",
	"greater than or equal test",
	"assignment",
	"plus equals",
	"minus equals",
	"times equals",
	"divide equals",
	"mod equals",
	"shift right equals",
	"shift left equals",
	"bitwise AND equals",
	"bitwise XOR equals",
	"bitwise OR equals",
	"AND/address operator",
	"minus/subtract operator",
	"multiply/dereference operator"
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
		return DELIMETER;
	}else if((input[start]=='0' && tolower(input[start+1])=='x' && isxdigit(input[start+2])) || (isxdigit(input[start]) && currentType==HEX)){//returns if current char is hexadecimal
		return HEX;
	}else if((isalpha(input[start]) && isalnum(input[start+1])) || (isalnum(input[start] && currentType==WORD))){//returns if current char is part of a word
		return WORD;
	}else if((input[start]=='0' && isoctal(input[start+1])) || (isoctal(input[start]) && currentType==OCTAL)){//returns if current type is part of octal
		return OCTAL;
	}else if(isdigit(input[start])){ //returns if current char is part of a decimal integer
		return DECIMAL;
	}else if(input[start] == ('.' || 'e' || '-') && currentType == (DECIMAL || FLOAT)){ //returns floating point
		return FLOAT;
	}else{//here the c operators begin
		int i = 0;
		for(i = LEFTPARENTHESIS; i<=MULTIPLYOPERATOR; i++){ 
			int length = strlen(c_op_vals[i-6]);
			if(strncmp(input, c_op_vals[i-6],length)==0){
				return i;
			}
		}

	/*
		switch(input[start]){
			case '(':
				return LEFTPARENTHESIS;
			case ')':
				return RIGHTPARENTHESIS;
			case '[':
				return LEFTBRACKET;
			case ']':
				return RIGHTPARENTHESIS:
			case '.':
				return STRUCTMEMBER;
			case '-':
				switch(input[start+1]){//seoncd switch for 2 character c ops starting with '-'
					case '>':
						return STRUCTPOINTER;
					case '-':
						return DECREMENT;
					case '=':
						return MINUSEQUALS;
					default:
						return MINUSOPERATOR;
				}
			case 's':
				if(strncmp(input[start],"sizeof",6)==0){
					return SIZEOF;
				}
				break;
			case ',':
				return COMMA;
			case '!':
				return NEGATE;
			case '~':
				return 1SCOMPLEMENT;
			case '>':
				if(strncmp(input[start],">>",2)==0){
					return SHIFTRIGHT;
				}else if(strncmp(input[start],">=",2)==0){
					return GREATERTHANEQUAL;
				}else if(strncmp(input[start],">>=",3)==0){
					return SHIFTRIGHTEQUALS;
				}else{
					return GREATERTHANTEST;
				}
			case '<':
				if(strncmp(input[start],"<<",2)==0){
					return SHIFTLEFT;
				}else if(strncmp(input[start],"<=",2)==0){
					return LESSTHANEQUAL;
				}else if(strncmp(input[start],"<<=",3)==0){
					return SHIFTLEFTEQUALS;
				}else{
					return LESSTHANTEST;
				}
			case '^':
				return BITWISEXOR;
			case ''
		}*/
	}
	return -1;

}

void printDEBUG(enum token type){
	printf("%s\n",token_type[type]);
}
void printOutput(char* input, int start, int end) {
	/*
	if(getType(input, start,NONE) == 0 || start > strlen(input)) {
		return;
	}
	printf("%s: ", token_type[getType(input, start,NONE)]); 
	for(int i = start; i < end; i++) {
		printf("%c", input[i]);
	}
	printf("\n");*/
}

void tokenize(char* input) {
	/*
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
	*/
}

int main(int argc, char *argv[]) {
	//tokenize(argv[1]);
	printDEBUG(getType(argv[1],0,-1));
	
	return 0;
}
