// Internal
#include "Environment.h"
#include "../Exception.h"
#include "../Functions.h"
#include "../Compiler/Function.h"

// External 
#include <stdlib.h>
#include <cstring>
#include <stdio.h>

Environment::Environment(OperationCode *aOpcode) {
	const vector<byte> opVec = aOpcode->GetBytecode();

	operationcodes = new byte[opVec.size()];
	copy(opVec.begin(), opVec.end(), stdext::checked_array_iterator<byte *>(operationcodes, opVec.size()));

	operationPointer = 0;
	dataDefinition = false;

	scope.AddItem(VAR_RETURN, new Variable(VAR_RETURN));
}

Environment::~Environment() {
	delete[] operationcodes;
}

int Environment::Execute() {
	while (operationcodes[operationPointer] != OP_EXIT) {
		Operations();
	}

	operationPointer++;
	int exitId = GetOperationCodeUint();

	if (exitId & (VAR_GLOBAL | VAR_LOCAL)) {
		Variable *var = GetVariableByID(exitId);
		exitId = var->GetInteger();
	}

	printf("Program terminated with status %i\n", exitId);
	return exitId;
}

void Environment::Operations() {
	byte op = operationcodes[operationPointer++];

	if (op >= 0xF0) {
		switch (op) {
			case OP_DATA_BEGIN:
				OperationDataBegin();	
				break;
			case OP_DATA_STRING: 
				OperationDataString();
				break;
			case OP_DATA_FUNC:
				OperationDataFunction();
				break;
			case OP_DATA_END:
				OperationDataEnd();
				break;
		}
	} else {
		if (dataDefinition) {
			throw InvalidOpException("Cannot perform common operations when defining data");
		}

		if (op <= 0x1F) {
			switch (op) {
				case OP_PUSH:
					OperationPush();
					break;

				case OP_PUSH_DATA:
					OperationPushData();
					break;

				case OP_POP:
					OperationPop();
					break;

				case OP_CALL:
					OperationCall();
					break;

				case OP_RET:
					OperationRet();
					break;

				case OP_ALLOC:
					OperationAlloc();
					break;

				case OP_POPMOV:
					OperationPopMove();
					break;

				case OP_MOV: 
					OperationMove();
					break;

				case OP_MOVI: 
					OperationMoveInt();
					break;

				case OP_MOVF:
					OperationMoveFloat();
					break;

				case OP_MOVS: 
					OperationMoveString();
					break;

				case OP_ADD: 
					OperationAdd();
					break;

				case OP_SUB: 
					OperationSub();
					break;

				case OP_MUL: 
					OperationMultiply();
					break;

				case OP_DIV:
					OperationDivide();
					break;

				case OP_MOD: 
					OpModulate();
					break;

				case OP_ADD_I:	
					OperationAddInt();		
					break;

				case OP_SUB_I:		
					OperationSubInt();		
					break;

				case OP_MUL_I:		
					OperationMultiplyInt();		
					break;

				case OP_DIV_I:	
					OperationDivdeInt();		
					break;

				case OP_MOD_I:		
					OperationModulesInt();		
					break;

				case OP_ADD_F:		
					OperationAddFloat();	
					break;

				case OP_SUB_F:	
					OperationSubdivisionFloat();	
					break;

				case OP_MUL_F:	
					SubdivisionMultiplyFloat();	
					break;

				case OP_DIV_F:	
					OperationDivdeFloat();	
					break;

				case OP_PUSH_SCOPE:
					OperationPushScope();
					break;

				case OP_POP_SCOPE:	
					OperationPopScope();
					break;
			}

		} else if (op <= 0x2F) {
			switch (op) {
				case OP_JMP:		
					OperationJump();
					break;

				case OP_JE:		
					OperationJe();		
					break;

				case OP_JNE:	
					OperationJne();		
					break;

				case OP_JG:		
					OperationJg();	
					break;

				case OP_JGE:	
					OperationJge();	
					break;

				case OP_JL:		
					OperationJl();		
					break;

				case OP_JLE:	
					OperationJle();	
					break;

				case OP_JE_I:
					OpJeInt();	
					break;

				case OP_JNE_I:
					OperationJneInt();
					break;

				case OP_JG_I:	
					OperationJgInt();
					break;

				case OP_JGE_I:	
					OperationJgeInt();	
					break;

				case OP_JL_I:	
					OperationPJlInt();	
					break;

				case OP_JLE_I:	
					OperationPJleInt();	
					break;

				case OP_JE_F:	
					OperationPJeF();	
					break;

				case OP_JNE_F:	
					OperationpJneFloat();
					break;

				case OP_JG_F:	
					OperationPJgFloat();
					break;

				case OP_JGE_F:	
					OperationpJgeFloat();
					break;

				case OP_JL_F:	
					OperationPJlFloat();	
					break;

				case OP_JLE_F:	
					OperationPJleFloat();
					break;
			}
		} 
	}
}

Variable* Environment::GetVariableByID(uint aID) {
	Variable *ret = NULL;

	if (aID & VAR_GLOBAL) {
		ret = scope.GetVar(aID);
	} else if (aID & VAR_LOCAL) {
		if (sizeScope.Size()) {
			ret = sizeScope.Peek()->GetVar(aID);
		}
	}

	return ret;
}

void Environment::PopStackingVariable(Variable *&aVar) {
	uint varID = 0;

	varID = stackPointer.Pop();
	aVar = GetVariableByID(varID);
}


int Environment::GetOperationCodeInteger() {
	byte *bytePtr = &((byte*)operationcodes)[operationPointer];

	operationPointer += 4;

	return *(int*)bytePtr;
}

uint Environment::GetOperationCodeUint() {
	byte *bytePtr = &((byte*)operationcodes)[operationPointer];

	operationPointer += 4;

	return *(uint*)bytePtr;
}

float Environment::GetOperationCodeFloat() {
	byte *bytePtr = &((byte*)operationcodes)[operationPointer];

	operationPointer += 4;

	return *(float*)bytePtr;
}

Variable* Environment::GetOperationCodeVariable() {
	uint id = GetOperationCodeUint();
	return GetVariableByID(id);
}


Scope* Environment::GetCurrentScope() {
	if (!sizeScope.Size()) {
		return &scope;
	} 
		
	return sizeScope.Peek();
}


void Environment::OperationPush() {
	uint varId = GetOperationCodeUint();
	stackPointer.Push(varId);

	LOGFILE(("Pushing var %x\n", varId));
}

void Environment::OperationPushData() {
	// Needs to be done Someday
}

void Environment::OperationPop() {
	stackPointer.Pop();
}

void Environment::OperationCall() {
	uint funcID = GetOperationCodeUint();

	if (funcID & FUNC_STD) {
		OperationCallStd(funcID);
	} else {
		uint funcPos = functions[funcID];

		stack.Push(operationPointer);
		operationPointer = funcPos;

		Scope *localScope = new Scope();
		sizeScope.Push(localScope);
	}

	LOGFILE(("Calling %x\n", funcID));
}

void Environment::OperationCallStd(uint aFuncID) {
	int pCount = AikiStd::GetParameterCount(aFuncID);

	Variable **params = new Variable*[pCount];
	for (int i=0; i < pCount; i++) {
		PopStackingVariable(params[i]);
	}

	Variable *retVal = AikiStd::CallStdFunc(aFuncID, pCount, params);

	if (retVal) {
		*GetVariableByID(VAR_RETURN) = *retVal;
	} else {
		GetVariableByID(VAR_RETURN)->Undefine();
	}

	delete[] params;
}

void Environment::OperationRet() {
	Scope *localScope = NULL;
	int retID = 0;

	retID = GetOperationCodeUint();
	Variable *retSrc = NULL;
	if ((retSrc = GetVariableByID(retID))) {
		Variable *retDst = GetVariableByID(VAR_RETURN);
		*retDst = *retSrc;
		stackPointer.Push(VAR_RETURN);
	} else {
		stackPointer.Push(0);
	}

	localScope = sizeScope.Pop();
	delete localScope;

	operationPointer = stack.Pop();

	LOGFILE(("Returning to %u\n", operationPointer));
}

void Environment::OperationAlloc() {
	uint varID = GetOperationCodeUint();

	if (varID & VAR_GLOBAL) {
		if (!scope.ItemExists(varID)) {
			scope.AddItem(varID, new Variable(varID));
		}
	} else {
		if (sizeScope.Size()) {
			if (!sizeScope.Peek()->ItemExists(varID)) {
				sizeScope.Peek()->AddItem(varID, new Variable(varID));
			}
		}
	}

	LOGFILE(("Allocating %x\n", varID));
}

void Environment::OperationPopMove() {
	Variable *source = NULL;
	Variable *dest = NULL;

	PopStackingVariable(source);

	dest = GetOperationCodeVariable();

	if (source != NULL) {
		*dest = *source;
	} else {
		dest->Undefine();
	}
}

void Environment::OperationMove() {
	Variable *source = NULL;
	Variable *dest = NULL;

	dest = GetOperationCodeVariable();
	source = GetOperationCodeVariable();

	*dest = *source;
}

void Environment::OperationMoveInt() {
	Variable *dest = NULL;
	int literal = 0;

	dest = GetOperationCodeVariable();
	literal = GetOperationCodeInteger();
	
	dest->Set(literal);

	LOGFILE(("Var %x = %i\n", dest->GetID(), literal));
}

void Environment::OperationMoveFloat() {
	Variable *dest = NULL;
	float literal = 0;

	dest = GetOperationCodeVariable();
	literal = GetOperationCodeFloat();

	dest->Set(literal);

	LOGFILE(("Var %x = %f\n", dest->GetID(), literal));
}

void Environment::OperationMoveString() {
	// Needs to be done someday :)
}

void Environment::OperationAdd() {
	ArithmeticStack(&Variable::operator+=);
}

void Environment::OperationSub() {
	ArithmeticStack(&Variable::operator-=);
}

void Environment::OperationMultiply() {
	ArithmeticStack(&Variable::operator*=);
}

void Environment::OperationDivide() {
	ArithmeticStack(&Variable::operator/=);
}

void Environment::OpModulate() {
	ArithmeticStack(&Variable::operator%=);
}

void Environment::ArithmeticStack(void(Variable::*aOper)(const Variable&)) {
	Variable *left = NULL;
	Variable *right = NULL;

	PopStackingVariable(right);
	PopStackingVariable(left);

	(*left.*aOper)(*right);
	stackPointer.Push(left->GetID());
}

void Environment::OperationAddInt() {
	ArithmeticInt(&Variable::operator+=);
}

void Environment::OperationSubInt() {
	ArithmeticInt(&Variable::operator-=);
}

void Environment::OperationMultiplyInt() {
	ArithmeticInt(&Variable::operator*=);
}

void Environment::OperationDivdeInt() {
	ArithmeticInt(&Variable::operator/=);
}

void Environment::OperationModulesInt() {
	ArithmeticInt(&Variable::operator%=);
}

void Environment::ArithmeticInt(void(Variable::*aOper)(const int&)) {
	Variable *var = NULL;
	int literal = 0;

	PopStackingVariable(var);
	literal = GetOperationCodeInteger();

	(*var.*aOper)(literal);
	stackPointer.Push(var->GetID());
}

void Environment::OperationAddFloat() {
	ArithmeticFloat(&Variable::operator+=);
}

void Environment::OperationSubdivisionFloat() {
	ArithmeticFloat(&Variable::operator-=);
}

void Environment::SubdivisionMultiplyFloat() {
	ArithmeticFloat(&Variable::operator*=);
}

void Environment::OperationDivdeFloat() {
	ArithmeticFloat(&Variable::operator/=);
}

void Environment::ArithmeticFloat(void(Variable::*aOper)(const float&)) {
	Variable *var = NULL;
	float literal = 0.f;

	PopStackingVariable(var);
	literal = GetOperationCodeFloat();

	(*var.*aOper)(literal);
	stackPointer.Push(var->GetID());
}

void Environment::OperationPushScope() {
	Scope *scope = GetCurrentScope();
	scope->PushNestedScope();
}

void Environment::OperationPopScope() {
	Scope *scope = GetCurrentScope();
	scope->PopNestedScope();
}

void Environment::OperationJump() {
	int dest = GetOperationCodeUint();
	operationPointer = dest;

	LOGFILE(("Jumping to %i\n", dest));
}

void Environment::OperationJe() {
	LOGFILE(("Comparing ==\n"));
	CompareJumpStack(&Variable::operator==);
}

void Environment::OperationJne() {
	LOGFILE(("Comparing !=\n"));
	CompareJumpStack(&Variable::operator!=);
}

void Environment::OperationJg() {
	LOGFILE(("Comparing >\n"));
	CompareJumpStack(&Variable::operator>);
}

void Environment::OperationJge() {
	LOGFILE(("Comparing >=\n"));
	CompareJumpStack(&Variable::operator>=);
}

void Environment::OperationJl() {
	LOGFILE(("Comparing <\n"));
	CompareJumpStack(&Variable::operator<);
}

void Environment::OperationJle() {
	LOGFILE(("Comparing <=\n"));
	CompareJumpStack(&Variable::operator<=);
}

void Environment::CompareJumpStack(bool(Variable::*aCmp)(const Variable&)const) {
	Variable *left = NULL;
	Variable *right = NULL;

	PopStackingVariable(right);
	PopStackingVariable(left);

	if ((left->*aCmp)(*right)) {
		OperationJump();
	} else {
		operationPointer += 4;
	}
}

void Environment::OpJeInt() {
	CompareJumpInt(&Variable::operator==);
}

void Environment::OperationJneInt() {
	CompareJumpInt(&Variable::operator!=);
}

void Environment::OperationJgInt() {
	CompareJumpInt(&Variable::operator>);
}

void Environment::OperationJgeInt() {
	CompareJumpInt(&Variable::operator>=);
}

void Environment::OperationPJlInt() {
	CompareJumpInt(&Variable::operator<);
}

void Environment::OperationPJleInt() {
	CompareJumpInt(&Variable::operator<=);
}

void Environment::CompareJumpInt(bool(Variable::*aCmp)(const int&)const) {
	Variable *var = NULL;
	int literal = 0;

	PopStackingVariable(var);
	literal = GetOperationCodeInteger();

	if ((*var.*aCmp)(literal)) {
		OperationJump();
	} else {
		operationPointer += 4;
	}
}

void Environment::OperationPJeF() {
	CompareJumpInt(&Variable::operator==);
}

void Environment::OperationpJneFloat() {
	CompareJumpInt(&Variable::operator!=);
}

void Environment::OperationPJgFloat() {
	CompareJumpInt(&Variable::operator>);
}

void Environment::OperationpJgeFloat() {
	CompareJumpInt(&Variable::operator>=);
}

void Environment::OperationPJlFloat() {
	CompareJumpInt(&Variable::operator<);
}

void Environment::OperationPJleFloat() {
	CompareJumpInt(&Variable::operator<=);
}

void Environment::CompareJumpFloat(bool(Variable::*aCmp)(const float&)const) {
	Variable *var = NULL;
	float literal = 0.f;

	PopStackingVariable(var);
	literal = GetOperationCodeFloat();

	if ((*var.*aCmp)(literal)) {
		OperationJump();
	} else {
		operationPointer += 4;
	}
}

void Environment::OperationDataBegin() {
	if (dataDefinition) {
		throw InvalidOpException("Already defining data");
	}
	dataDefinition = true;
}

void Environment::OperationDataString() {
	// Needs to be done someday
}

void Environment::OperationDataFunction() {
	int funcId = 0;
	int funcPos = 0;

	funcId = GetOperationCodeUint();
	funcPos = GetOperationCodeUint();

	functions[funcId] = funcPos;
}

void Environment::OperationDataEnd() {
	if (!dataDefinition) {
		throw InvalidOpException("Was not defining data");
	}

	dataDefinition = false;
}