// Internal
#include "OperationCodeText.h"
#include "OperationCode.h"

// External
#include <sstream>
#include <iomanip>
#include <iostream>

void OperationCodeText::parse(OperationCode *aOperationCode) {
	operationCodes = aOperationCode->getBytecode();

	for ( int i = 0; i < (int)operationCodes.size(); i++ ) {
		if (i && i % 8 == 0) {
			std::printf("\n");
		}

		std::printf("%2x  ", operationCodes[i]);
	}
	std::printf("\n\n");


	stringstream ss;
	int idx = 0;

	while ( idx < (int)operationCodes.size() ) 
	{
		ss << "[";
		ss << setw(5) << setfill(' ') << idx;
		ss << "]\t";

		byte code = operationCodes[idx];
		ss << getLieteral(code);
		ss << "\t\t";

		int params = parametersCount(code);
		idx++;

		for (int i=0; i<params; i++) {
			if (i) {
				ss << ", ";
			}

			ss << getParameter(code, idx, i);
			idx += 4;
		}

		ss << "\n";
	}

	std::printf("%s\n", ss.str().c_str());

}


int OperationCodeText::parametersCount(byte aCode) {
	switch (aCode) {
		/* Operations with 0 parameters */
		case OP_POP:

		case OP_ADD:
		case OP_SUB:
		case OP_DIV:
		case OP_MUL:

		case OP_PUSH_SCOPE:
		case OP_POP_SCOPE:

		case OP_DATA_BEGIN:
		case OP_DATA_END:
			return 0;

		/* Operations with 1 parameter */
		case OP_PUSH:
		case OP_PUSH_DATA:
		case OP_CALL:
		case OP_RET:
		case OP_ALLOC:
		case OP_POPMOV:
		case OP_EXIT:

		case OP_ADD_I:
		case OP_SUB_I:
		case OP_MUL_I:
		case OP_DIV_I:
		case OP_MOD_I:

		case OP_ADD_F:
		case OP_SUB_F:
		case OP_MUL_F:
		case OP_DIV_F:

		case OP_JMP:
		case OP_JE:
		case OP_JNE:
		case OP_JG:
		case OP_JL:
		case OP_JLE:
			return 1;

		/* Operations with 2 parameters*/
		case OP_MOV:
		case OP_MOVI:
		case OP_MOVF:
		case OP_MOVS:
		case OP_DATA_FUNC:

		case OP_JE_I:
		case OP_JNE_I:
		case OP_JG_I:
		case OP_JL_I:
		case OP_JLE_I:

		case OP_JE_F:
		case OP_JNE_F:
		case OP_JG_F:
		case OP_JL_F:
		case OP_JLE_F:
			return 2;

		/* TODO: */
		case OP_DATA_STRING:
			return -1;
	}
	
	return 0;
}

string OperationCodeText::getLieteral(byte code) {
	switch (code) {
		/* Basics */
		case OP_PUSH:			return "PUSH";
		case OP_PUSH_DATA:		return "PUSH_DATA";
		case OP_POP:			return "POP";
		case OP_CALL:			return "CALL";
		case OP_RET:			return "RET";
		case OP_ALLOC:			return "ALLOC";
		case OP_POPMOV:			return "POPMOV";
		case OP_EXIT:			return "EXIT";
		case OP_MOV:			return "MOV";
		case OP_MOVI:			return "MOV_I";
		case OP_MOVF:			return "MOV_F";
		case OP_MOVS:			return "MOV_S";

		case OP_PUSH_SCOPE:		return "PUSH_SCOPE";
		case OP_POP_SCOPE:		return "POP_SCOPE";

		/* Arithmetics */
		case OP_ADD:			return "ADD";
		case OP_SUB:			return "SUB";
		case OP_DIV:			return "DIV";
		case OP_MUL:			return "MUL";

		case OP_ADD_I:			return "ADD_I";
		case OP_SUB_I:			return "SUB_I";
		case OP_MUL_I:			return "MUL_I";
		case OP_DIV_I:			return "DIV_I";
		case OP_MOD_I:			return "MOD_I";

		case OP_ADD_F:			return "ADD_F";
		case OP_SUB_F:			return "SUB_F";
		case OP_MUL_F:			return "MUL_F";
		case OP_DIV_F:			return "DIV_F";

		/* Comparisons */
		case OP_JMP:			return "JMP";
		case OP_JE:				return "JE";
		case OP_JNE:			return "JNE";
		case OP_JG:				return "JG";
		case OP_JL:				return "JL";
		case OP_JLE:			return "JLE";

		case OP_JE_I:			return "JE_I";
		case OP_JNE_I:			return "JNE_I";
		case OP_JG_I:			return "JG_I";
		case OP_JL_I:			return "JL_I";
		case OP_JLE_I:			return "JLE_I";

		case OP_JE_F:			return "JE_F";
		case OP_JNE_F:			return "JNE_F";
		case OP_JG_F:			return "JG_F";
		case OP_JL_F:			return "JL_F";
		case OP_JLE_F:			return "JLE_F";

		/* Data definitions */
		case OP_DATA_BEGIN:		return "DATA_BEGIN";
		case OP_DATA_END:		return "DATA_END";
		case OP_DATA_FUNC:		return "DATA_FUNC";
		case OP_DATA_STRING:	return "DATA_STRING";
	}

	char s[32];
	sprintf_s(s, "UNKOWN(%2x)", code);
	return (string)s;
}

string OperationCodeText::getParameter(byte ctx, int bytecodeIdx, int paramIdx) {
	switch (ctx) {
		/* Operations with 1 parameter */
		case OP_PUSH:
		case OP_PUSH_DATA:
		case OP_CALL:
		case OP_RET:
		case OP_ALLOC:
		case OP_POPMOV:
		case OP_EXIT:

		case OP_JMP:
		case OP_JE:
		case OP_JNE:
		case OP_JG:
		case OP_JL:
		case OP_JLE:
			return getUint(bytecodeIdx);

		case OP_ADD_I:
		case OP_SUB_I:
		case OP_MUL_I:
		case OP_DIV_I:
		case OP_MOD_I:
			return getInteger(bytecodeIdx);

		case OP_ADD_F:
		case OP_SUB_F:
		case OP_MUL_F:
		case OP_DIV_F:
			return getFloat(bytecodeIdx);

			/* Operations with 2 parameters*/
		case OP_MOV:
			return getUint(bytecodeIdx);

		case OP_MOVI:
			if (!paramIdx) {
				return getUint(bytecodeIdx);
			} else {
				return getInteger(bytecodeIdx);
			}

		case OP_MOVF:
			if (!paramIdx) {
				return getUint(bytecodeIdx);
			} else {
				return getFloat(bytecodeIdx);
			}

		case OP_MOVS:
			return getUint(bytecodeIdx);

		case OP_DATA_FUNC:
			return getUint(bytecodeIdx);


		case OP_JE_I:
		case OP_JNE_I:
		case OP_JG_I:
		case OP_JL_I:
		case OP_JLE_I:
			if (!paramIdx)  return getInteger(bytecodeIdx);
			return getUint(bytecodeIdx);

		case OP_JE_F:
		case OP_JNE_F:
		case OP_JG_F:
		case OP_JL_F:
		case OP_JLE_F:
			if (!paramIdx)  return getFloat(bytecodeIdx);
			return getUint(bytecodeIdx);


		/* TODO: */
		case OP_DATA_STRING:
			return "WHAT";
	}

	return "{?}";
}


void* OperationCodeText::getDword(int opcodeIdx) {
	void *dword = malloc(4);

	return dword;
}

string OperationCodeText::getUint(int opcodeIdx) {
	stringstream ss;

	uint val = *(uint*)(&operationCodes[opcodeIdx]);

	if (val & VAR_GLOBAL) {
		val &= ~VAR_GLOBAL;
		ss << "G_";
	} else if (val & VAR_LOCAL) {
		val &= ~VAR_LOCAL;
		ss << "L_";
	} else if (val & FUNC_STD) {
		val &= ~FUNC_STD;
		ss << "STD_";
	}

	ss << val;

	return ss.str();
}

string OperationCodeText::getInteger(int opcodeIdx) {
	stringstream ss;

	int val = *(int*)(&operationCodes[opcodeIdx]);
	ss << val;

	return ss.str();
}

string OperationCodeText::getFloat(int opcodeIdx) {
	stringstream ss;

	float val = *(float*)(&operationCodes[opcodeIdx]);
	ss << val;

	return ss.str();
}