#include <stdio.h>
#include <stdlib.h>



enum token{word, decimal, octal, hexadecimal, floating, C_op};

struct t_index{//Contains data of where in string token is
	int start;
	int end;
	enum token type;
}

int main(int argc, char *argv[]) {

	return 0;
}

int* tokenize
