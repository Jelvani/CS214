#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*FUNCTION PROTOTYPES*/
void tokenize(char* input);
void printWord(char* input, int start, int end);
int findQuotes(char* input, int start);
enum token findCkeyword(char* input, int start);
enum token findCop(char *input, int start);
int findFloat(char *input, int start);
int findHex(char *input, int start);
int findOctal(char *input, int start);
int findDecimal(char *input, int start);
int findWord(char *input, int start);
int isoctal(char c);


/*DECLARATIONS*/

enum token{//token types
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
	AUTO,
	BREAK,
	CASE,
	CHAR,
	CONST,
	CONTINUE,
	DEFAULT,
	DO,
	DOUBLE,
	ELSE,
	ENUM,
	EXTERN,
	_FLOAT,
	FOR,
	GOTO,
	IF,
	INLINE,
	INT,
	LONG,
	REGISTER,
	RESTRICT,
	RETURN,
	SHORT,
	SIGNED,
	STATIC,
	STRUCT,
	SWITCH,
	TYPEDEF,
	UNION,
	UNSIGNED,
	VOID,
	VOLATILE,
	WHILE,
	_BOOL,
	_COMPLEX,
	_IMAGINARY,
	};

const char *c_op_vals[] = {//c operators to check against
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
	"auto",
	"break",
	"case",
	"char",
	"const",
	"continue",
	"default",
	"do",
	"double",
	"else",
	"enum",
	"extern",
	"float",
	"for",
	"goto",
	"if",
	"inline",
	"int",
	"long",
	"register",
	"restrict",
	"return",
	"short",
	"signed",
	"static",
	"struct",
	"switch",
	"typedef",
	"union",
	"unsigned",
	"void",
	"volatile",
	"while",
	"_Bool",
	"_Complex",
	"_Imaginary"
	};

const char *token_type[] = {//token type that is printed out
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
	"multiply/dereference operator",
	"single quotes"
	};

/*HELPER FUNCTIONS*/
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
	if(input[start]=='0'){
		return length;
	}
	while(isdigit(input[start+length])){
		length++;
	}
	return length;
}

int findOctal(char *input, int start){//returns number of chars in detected octal token
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
		if(input[start]=='0'){
			return length;
		}
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
			}else{//these elses return 0 because then there are no deimals after dot, or no dot exists
				return 0;
			}
		}else{
			return 0;
		}

		if(tolower(input[start+length])=='e'){//checks for exponent in scientific notation
			scnot++;
			if(input[start+length+scnot] == '-' || input[start+length+scnot] == '+' ){
				scnot++;
			}
		}
		if(isdigit(input[start+length+scnot])){//check again for decimals that make up exponent
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

int findQuotes(char* input, int start){//returns the index of the closing quote, and -1 if no quote detected, -2 if no closing quote detected

	char quote;
	int end = start;
	if(input[start]=='\''){
		quote = '\'';
	}else if(input[start]=='\"'){
		quote='\"';
	}else{
		return -1;
	}

	while(end<strlen(input)){
		end++;
		if(input[end]==quote) return (end-start+1);
	}
	return -2;

}


void printWord(char* input, int start, int end) {//prints a substring. input is the pointer to the begining of a string, and the starting and ending index to be printed. Functions returns on index out of bounds
	int length = strlen(input);
	if(start<0 || end>length){
		return;
	}
	printf("\"");
	for(int i = start; i < end; i++) {
		printf("%c", input[i]);
	}
	printf("\"");
}



/*
The tokenize functon will take a pointer to a string and print out all detected tokens and type
*/
void tokenize(char* input) {
	int start = 0;
	int length = strlen(input);
	enum token type = 0;
	enum token tempType = 0;
	int tokenLength = 0;//The temp variables are used to compare every token detected to see if it is the largest one, for greedy aspect of tokenizer
	int tempLength = 0;
	while(start < length){


		if(strncmp(&input[start],"/*",2)==0){//for extra credit, multi-line C comment detector. increases pointer index until closing comment found
			start+=2;
			while(strncmp(&input[start],"*/",2)!=0){
				if(start>length){
					printf("No closing multi-line C comment (*/) detected!\n");
					break;
				}
				start++;
			}
			start+=2;
			continue;
		}
		if(strncmp(&input[start],"//",2)==0){//for extra credit, single-line C comment detector. Ignores all text after '//' until newline terminator
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

		if(findQuotes(input,start)!=-1){//For extra credit, finds double and single quote pairs. Prints all content between 2 matching quotes as single token

			tokenLength = findQuotes(input,start);
			if(tokenLength==-2){
				printf("No closing quotes detected in string! Please modify string and retry.\n");
				break;
			}
			printf("quotes: ");
			printWord(input, start, start + tokenLength);
			printf("\n");
			start+=tokenLength;
			tokenLength=0;//reset length for finding largest token
			type=0;
			continue;

		}


		if(isspace(input[start])){//on whitespace, ignore and restart
			start++;
			continue;			
		}
		if(findCkeyword(input,start)!=-1){//Finds C keywords and sets token type and token length
			tempType = findCkeyword(input,start);
			tempLength = strlen(c_op_vals[tempType-6]);
			if(tempLength>tokenLength){
				type=tempType;
				tokenLength=tempLength;
			}
		}
		if(findCop(input,start)!=-1){//Finds C operators and sets token type and token length
			tempType = findCop(input,start);
			tempLength = strlen(c_op_vals[tempType-6]);
			if(tempLength>tokenLength){
				type=tempType;
				tokenLength=tempLength;
			}
		}if(isalpha(input[start])){//Finds alphanumerical token and sets token type and token length
			tempType = WORD;
			tempLength = findWord(input,start);
			if(tempLength>tokenLength){
				type=tempType;
				tokenLength=tempLength;
			}
		}
		if(tolower(input[start+1])=='x' && findHex(input,start+2)>0){//Finds hexadecimal tokens, and sets token type and token length
			tempType = HEX;
			tempLength = findHex(input,start+2)+2;
			if(tempLength>tokenLength){
				type=tempType;
				tokenLength=tempLength;
			}
		}else{//if no hex detected, checks for octal, decimal, or float. Chooses the token with the longest length
			int t1 = findOctal(input,start);
			int t2 = findDecimal(input,start);
			int t3 = findFloat(input,start);
			if(t1>=t2 && t1>=t3 && input[start] == '0'){
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

		if(tokenLength==0){//if encountered unkown character, ignore and continue
			start++;
			continue;

		}

		if(type < AUTO){//prints token type for all non-extra credit
			printf("%s: ", token_type[type]);
		}else{
			printf("c keyword: ");//else, type must be a C keyword from extra credit
		}
		printWord(input, start, start + tokenLength);
		printf("\n");
		start+=tokenLength;//increment index by length of printed out token
		tokenLength=0;//reset length and type for finding largest token
		type=0;
	}

}

int main(int argc, char *argv[]) {
	if(argc != 2){
		printf("Please pass 1 argument to this program\n");
		return EXIT_FAILURE;
	}
	tokenize(argv[1]);
	printf("Tokenizer has finished\n");
	return EXIT_SUCCESS;
}
