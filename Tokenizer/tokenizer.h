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
	_SIZEOF,
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
	"sizeof",
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
	"multiply/dereference operator",
	"single quotes",
};
