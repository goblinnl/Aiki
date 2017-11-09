#ifndef CODES_H
#define CODES_H

/*
*	 Variables getting stored into a lookup-table in the Environment class.
*	There may only be (2^28)-1 variables allocated
*/
#define VAR_GLOBAL 		0x80000000
#define VAR_LOCAL		0x40000000
#define VAR_RETURN		0xFFFFFFFF
#define FUNC_STD		0x20000000
#define OP_PUSH			0x01  
#define OP_PUSH_DATA	0x02	
#define OP_CALL			0x03	
#define OP_RET			0x04
#define OP_ALLOC		0x05
#define OP_POPMOV		0x06 //PARAM (*)
#define OP_EXIT			0x07	
#define OP_MOV			0x08 // PARAM 2  (var id)
#define OP_MOVI			0x09 // PARAM 2  (int literal)
#define OP_MOVF			0x0A // PARAM 2  (float literal)
#define OP_MOVS			0x0B // PARAM 2  (string-data id)
#define OP_POP 			0x0C	

// Arithmetic operations are performed:
//		b = pop()
// 		a = pop()
// 		a = a (operator) b
// 		push(a)
#define OP_ADD					0x0D
#define OP_SUB					0x0E
#define OP_MUL					0x0F
#define OP_DIV 					0x10
#define OP_MOD					0x11

// Arithmetic literal operations are performed:
//		a = pop()
//		a = a (operator) opcode_literal
//		push(a)
#define OP_ADD_I				0x12	// PARAM
#define OP_SUB_I				0x13	// PARAM
#define OP_MUL_I				0x14	// PARAM
#define OP_DIV_I				0x15	// PARAM
#define OP_MOD_I				0x16	// PARAM

#define OP_ADD_F				0x17	// PARAM
#define OP_SUB_F				0x18	// PARAM
#define OP_MUL_F				0x19	// PARAM
#define OP_DIV_F				0x1A	// PARAM

// Nested scopes are scopes within a scope, for
// instance the body of a loop or an if-statement.
// Variables allocated after a call to PUSH_SCOPE
// will be deallocated on the next POP_SCOPE call.
#define	OP_PUSH_SCOPE			0x1E
#define OP_POP_SCOPE			0x1F

/***** COMPARISON AND JUMPS *****
* Start: 		0x20
* Finish:		0x3F
*
* Comparisons are performed in the same order
* as arithmetics. This is a pseudo-comparison
* between the two last elements on the stack:
*
* b = pop()
* a = pop()
* if (a >= b) { stuff() }
*
* (*)	Expects an INT, followed by an UINT.
* (**)	Expects a FLOAT, followed by an UINT
*****/
#define OP_JMP					0x20	// PARAM
#define OP_JE					0x21	// PARAM
#define OP_JNE					0x22	// PARAM
#define OP_JG					0x23	// PARAM
#define OP_JGE					0x24	// PARAM
#define OP_JL					0x25	// PARAM
#define OP_JLE					0x26	// PARAM

#define OP_JE_I					0x27	// PARAM (*)
#define OP_JNE_I				0x28	// PARAM (*)
#define OP_JG_I					0x29	// PARAM (*)
#define OP_JGE_I				0x2A	// PARAM (*)
#define OP_JL_I					0x2B	// PARAM (*)
#define OP_JLE_I				0x2C	// PARAM (*)

#define OP_JE_F					0x2D	// PARAM  (**)
#define OP_JNE_F				0x2E	// PARAM  (**)
#define OP_JG_F					0x2F	// PARAM  (**)
#define OP_JGE_F				0x30	// PARAM  (**)
#define OP_JL_F					0x31	// PARAM  (**)
#define OP_JLE_F				0x32	// PARAM  (**)

/***** DATA DEFINITIONS *****
* Start: 		0xF0
* Finish: 		0xFF
*
* (*) 	Requires four bytes (string length),
* 		followed by a NULL-terminated string.
* (**)	Required an int defining the ID of the
*		function, followed by an int containing
*		the byte offset of the first instruction
*		in the function.
*****/
#define OP_DATA_BEGIN			0xF0
#define OP_DATA_STRING			0xF1 	// PARAM (*)
#define OP_DATA_FUNC			0xF2	// PARAM (**)
#define OP_DATA_END 			0xFF


/**** DATA FUNCTIONS *****
*
* Replace compiler functions with own data functions
* this is to speed up the parsing processing
*
*/
#define EF_END					0
#define EF_SET_VAR				1
#define EF_GET_VAR				2
#define EF_EXC_FUNCTION			3
#define EF_IF					4
#define EF_MERGE_START			5
#define EF_MERGE_END			6
#define EF_STRING				7
#define EF_NUMBER				8
#define EF_TABLE				9
#define EF_RETURN				14
#define EF_L_SET_VAR			15
#define EF_ELSE_IF				24
#define EF_ELSE					25
#define EF_WHILE				26
#define EF_BOOL					27
#define EF_TABLE_INDEX			28
#define EF_TABLE_GET_INDEX		29
#define EF_BREAK				30
#define EF_FUNC					33
#define EF_FOR_EACH				34
#define EF_LOOP					35
#define EF_CLASS				36
#define EF_NEW					37
#define EF_MCNEW				38
#define EF_FOR					39
#define EF_POST_INCREASE		40
#define EF_POST_DECREASE		41
#define EF_PRE_INCREASE			42
#define EF_PRE_DECREASE			43
#define EF_CONTINUE				200
#define EF_ANON_FUNCTION		201
#define EF_ENUM					202
#define EF_GET_COUNT			203
#define EF_GOTO					204
#define EF_REWIND				205
#define EF_DELETE				206
#define EF_GLOBAL_LOCALS		207
#define EF_STRING_TABLE			250
#define EF_CURRENT_LINE			251
#define EF_SKIP					252

/*
*	 Variables getting stored into a lookup-table in the Environment class.
*	There may only be (2^28)-1 variables allocated
*/
#define OP_END_SCOPE			0
#define OP_SET					1
#define OP_SET_L				2
#define OP_GET					3
#define OP_GET_L				4
#define OP_CALL					5
#define OP_CALL_THIS			6
#define OP_SET_INDEX			7
#define OP_GET_INDEX			8
#define OP_ADD_INDEX			9
#define OP_ADD_NEW				10
#define OP_GET_COUNTER			11

/***** COMPARISON AND JUMPS *****
*
*
*****/
#define OP_JUMP					12
#define OP_JUMP_NT				13
#define OP_RETURN				14
#define OP_DELETE				15
#define OP_STACK_NUMBER			16
#define OP_STACK_STRING			17
#define OP_STACK_TABLE			18
#define OP_STACK_BOOL			19
#define OP_STACK_NULL			20
#define OP_STACK_FUNCTION		21
#define OP_POP					22
#define OP_DUPLICATE			23
#define OP_PRE_INCREASE			24
#define OP_PRE_DECREASE			25
#define OP_POST_INCREASE		26
#define OP_POST_DECREASE		27

// Arithmetic operations are performed:
//		b = pop()
// 		a = pop()
// 		a = a (operator) b
// 		push(a)
#define AO_START				28
#define AO_ADD					29
#define	AO_SUBSTRACT			30
#define AO_DIVIDE				31
#define AO_MULTIPLY				32
#define AO_AND					33
#define AO_OR					34
#define AO_GREATER_THAN 		35
#define AO_LESS_THAN			36
#define AO_GREATER_OR_EQUAL		37
#define AO_LESS_OR_EQUAL		38
#define AO_EQUAL				39
#define AO_NOT_EQUAL			40
#define AO_BITWISE_LEFT			41
#define AO_BITWISE_RIGHT		42
#define AO_BITWISE_OR			43
#define AO_BITWISE_AND			44
#define AO_BITWISE_MODULO		45
#define AO_BITWISE_CALL			46
#define AO_SET_INDEX			47
#define AO_GET_INDEX			48
#define AO_GET_COUNTER			49
#define AO_TO_STRING			50
#define AO_END					51

/***** CUSTOM OPERATORS *****
*
*
*/
#define CO_GLOBAL_LOCALS		52
#define CO_STRING_TABLE			53
#define CO_CURRENT_LINE			54
#define CO_SKIP					55


/***** TYPE DEFINITIONS *****
* Common type definitions.
*****/
typedef unsigned		uint;
typedef unsigned char	byte;

#endif