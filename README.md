# CS214 Projects

## Tokenizer
Tokenizer will take in 1 argument through the command-line specified as a string and tokenize the input.

### Usage
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

### Supported Tokens
***
Tokenizer currently supports the following token types:

Token Type  | Example
------------- | -------------
Word  | hi
Decimal Integer  |  123|
Octal Integer | 07345
Hexadecimal Integer | 0xa367f
Floating Point | 2.12444e+13
C Operator | Check 'n1256.pdf'
C Keywords | Check 'n1256.pdf'
C Single-line and Multi-line comments (ignores these) | /*balhblah*/ and //blahblah
Quote detection | "all one token" and 'all one token'

***
## ++Malloc

A custom implementation of the `Malloc` and `free` functions provided in the C standard library.

### Usage
 To use `mymalloc.c` simply link your exectuable against this library, and include the `mymalloc.h` header for defeniition and necessary macros.

 To test the runtimes of our program type:
 ```sh
make
```
in `CS214/Malloc`.
you can run our example usage program `memgrind` by executing:
```
./memgrind
```