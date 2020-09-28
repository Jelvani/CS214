# Tokenizer
Tokenizer will take in 1 argument through the command-line specified as a string and tokenize the input.

## Usage
***
To compile `Tokenizer.c` run the following command:
```sh
gcc -o tokenizer tokenizer.c
```

Alternatively, you can just use `Make` to compile the program through the included makefile:
```sh
make
```
You can run the program by:
```sh
./tokenizer "You string goes here"
```

## Supported Tokens
***
Tokenizer currently supports the following token types:

Token Type  | Values
------------- | -------------
Word  | hi
Decimal Integer  |  123|
Octal Integer | 07345
Hexadecimal Integer | 0xa367f
Floating Point | 2.12444e+13
C Operator | `(`, `)`, `[`, `]`, `.`, `->`, `sizeof`,<br>`,`, `!`, `~`, `>>`, `<<`, `^`, `|`, `++`, `--`,<br>`+`, `/`, `||`, `&&`, `?`, `:`, `==`, `!=`,<br>`<`, `>`, `<=`, `>=`, `=`, `+=`, `-=`, `*=`,<br>`/=`, `%=`, `>>=`, `<<=`, `&=`, `^=`, `|=`, `&`, `=`, `*`



Below is a state machine diagram for token detection
![FSM](fsm.svg)
