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
