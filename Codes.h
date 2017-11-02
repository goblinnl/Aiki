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

/***** TYPE DEFINITIONS *****
* Common type definitions.
*****/
typedef unsigned		uint;
typedef unsigned char	byte;

#endif