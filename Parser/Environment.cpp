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
	const vector<byte> opVec = aOpcode->getBytecode();

	operationcodes = new byte[opVec.size()];
	copy(opVec.begin(), opVec.end(), operationcodes);

	operationPointer = 0;
	dataDefinition = false;

	scope.addItem(VAR_RETURN, new Variable(VAR_RETURN));
}

Environment::~Environment() {
	delete[] operationcodes;
}

int Environment::Execute() {
	while (operationcodes[operationPointer] != OP_EXIT) {
		operations();
	}

	operationPointer++;
	int exitId = getOperationCodeUint();

	if (exitId & (VAR_GLOBAL | VAR_LOCAL)) {
		Variable *var = getVariableByID(exitId);
		exitId = var->getInteger();
	}

	printf("Program terminated with status %i\n", exitId);
	return exitId;
}

void Environment::operations() {
	byte op = operationcodes[operationPointer++];

	if (op >= 0xF0) {
		switch (op) {
			case OP_DATA_BEGIN:
				operationDataBegin();	
				break;
			case OP_DATA_STRING: 
				operationDataString();
				break;
			case OP_DATA_FUNC:
				operationDataFunction();
				break;
			case OP_DATA_END:
				operationDataEnd();
				break;
		}
	} else {
		if (dataDefinition) {
			throw InvalidOpException("Cannot perform common operations when defining data");
		}

		if (op <= 0x1F) {
			switch (op) {
				case OP_PUSH:
					operationPush();
					break;

				case OP_PUSH_DATA:
					operationPushData();
					break;

				case OP_POP:
					operationPop();
					break;

				case OP_CALL:
					operationCall();
					break;

				case OP_RET:
					operationRet();
					break;

				case OP_ALLOC:
					operationAlloc();
					break;

				case OP_POPMOV:
					operationPopMove();
					break;

				case OP_MOV: 
					operationMove();
					break;

				case OP_MOVI: 
					operationMoveInt();
					break;

				case OP_MOVF:
					operationMoveFloat();
					break;

				case OP_MOVS: 
					operationMoveString();
					break;

				case OP_ADD: 
					operationAdd();
					break;

				case OP_SUB: 
					operationSub();
					break;

				case OP_MUL: 
					operationMultiply();
					break;

				case OP_DIV:
					operationDivide();
					break;

				case OP_MOD: 
					opModulate();
					break;

				case OP_ADD_I:	
					operationAddInt();		
					break;

				case OP_SUB_I:		
					operationSubInt();		
					break;

				case OP_MUL_I:		
					operationMultiplyInt();		
					break;

				case OP_DIV_I:	
					operationDivdeInt();		
					break;

				case OP_MOD_I:		
					operationModulesInt();		
					break;

				case OP_ADD_F:		
					operationAddFloat();	
					break;

				case OP_SUB_F:	
					operationSubdivisionFloat();	
					break;

				case OP_MUL_F:	
					subdivisionMultiplyFloat();	
					break;

				case OP_DIV_F:	
					OperationDivdeFloat();	
					break;

				case OP_PUSH_SCOPE:
					operationPushScope();
					break;

				case OP_POP_SCOPE:	
					operationPopScope();
					break;
			}

		} else if (op <= 0x2F) {
			switch (op) {
				case OP_JMP:		
					operationJump();
					break;

				case OP_JE:		
					operationJe();		
					break;

				case OP_JNE:	
					operationJne();		
					break;

				case OP_JG:		
					operationJg();	
					break;

				case OP_JGE:	
					operationJge();	
					break;

				case OP_JL:		
					operationJl();		
					break;

				case OP_JLE:	
					operationJle();	
					break;

				case OP_JE_I:
					opJeInt();	
					break;

				case OP_JNE_I:
					operationJneInt();
					break;

				case OP_JG_I:	
					operationJgInt();
					break;

				case OP_JGE_I:	
					operationJgeInt();	
					break;

				case OP_JL_I:	
					operationPJlInt();	
					break;

				case OP_JLE_I:	
					operationPJleInt();	
					break;

				case OP_JE_F:	
					operationPJeF();	
					break;

				case OP_JNE_F:	
					operationpJneFloat();
					break;

				case OP_JG_F:	
					operationPJgFloat();
					break;

				case OP_JGE_F:	
					operationpJgeFloat();
					break;

				case OP_JL_F:	
					operationPJlFloat();	
					break;

				case OP_JLE_F:	
					operationPJleFloat();
					break;
			}
		} 
	}
}

Variable* Environment::getVariableByID(uint aID) {
	Variable *ret = NULL;

	if (aID & VAR_GLOBAL) {
		ret = scope.getVar(aID);
	} else if (aID & VAR_LOCAL) {
		if (sizeScope.Size()) {
			ret = sizeScope.peek()->getVar(aID);
		}
	}

	return ret;
}

void Environment::popStackingVariable(Variable *&aVar) {
	uint varID = 0;

	varID = stackPointer.pop();
	aVar = getVariableByID(varID);
}


int Environment::getOperationCodeInteger() {
	byte *bytePtr = &((byte*)operationcodes)[operationPointer];

	operationPointer += 4;

	return *(int*)bytePtr;
}

uint Environment::getOperationCodeUint() {
	byte *bytePtr = &((byte*)operationcodes)[operationPointer];

	operationPointer += 4;

	return *(uint*)bytePtr;
}

float Environment::getOperationCodeFloat() {
	byte *bytePtr = &((byte*)operationcodes)[operationPointer];

	operationPointer += 4;

	return *(float*)bytePtr;
}

Variable* Environment::getOperationCodeVariable() {
	uint id = getOperationCodeUint();
	return getVariableByID(id);
}


Scope* Environment::getCurrentScope() {
	if (!sizeScope.Size()) {
		return &scope;
	} 
		
	return sizeScope.peek();
}


void Environment::operationPush() {
	uint varId = getOperationCodeUint();
	stackPointer.push(varId);

	LOGFILE(("Pushing var %x\n", varId));
}

void Environment::operationPushData() {
	// Needs to be done Someday
}

void Environment::operationPop() {
	stackPointer.pop();
}

void Environment::operationCall() {
	uint funcID = getOperationCodeUint();

	if (funcID & FUNC_STD) {
		operationCallStd(funcID);
	} else {
		uint funcPos = functions[funcID];

		stack.push(operationPointer);
		operationPointer = funcPos;

		Scope *localScope = new Scope();
		sizeScope.push(localScope);
	}

	LOGFILE(("Calling %x\n", funcID));
}

void Environment::operationCallStd(uint aFuncID) {
	int pCount = AikiStd::getParameterCount(aFuncID);

	Variable **params = new Variable*[pCount];
	for (int i=0; i < pCount; i++) {
		popStackingVariable(params[i]);
	}

	Variable *retVal = AikiStd::callStdFunc(aFuncID, pCount, params);

	if (retVal) {
		*getVariableByID(VAR_RETURN) = *retVal;
	} else {
		getVariableByID(VAR_RETURN)->undefine();
	}

	delete[] params;
}

void Environment::operationRet() {
	Scope *localScope = NULL;
	int retID = 0;

	retID = getOperationCodeUint();
	Variable *retSrc = NULL;
	if ((retSrc = getVariableByID(retID))) {
		Variable *retDst = getVariableByID(VAR_RETURN);
		*retDst = *retSrc;
		stackPointer.push(VAR_RETURN);
	} else {
		stackPointer.push(0);
	}

	localScope = sizeScope.pop();
	delete localScope;

	operationPointer = stack.pop();

	LOGFILE(("Returning to %u\n", operationPointer));
}

void Environment::operationAlloc() {
	uint varID = getOperationCodeUint();

	if (varID & VAR_GLOBAL) {
		if (!scope.itemExists(varID)) {
			scope.addItem(varID, new Variable(varID));
		}
	} else {
		if (sizeScope.Size()) {
			if (!sizeScope.peek()->itemExists(varID)) {
				sizeScope.peek()->addItem(varID, new Variable(varID));
			}
		}
	}

	LOGFILE(("Allocating %x\n", varID));
}

void Environment::operationPopMove() {
	Variable *source = NULL;
	Variable *dest = NULL;

	popStackingVariable(source);

	dest = getOperationCodeVariable();

	if (source != NULL) {
		*dest = *source;
	} else {
		dest->undefine();
	}
}

void Environment::operationMove() {
	Variable *source = NULL;
	Variable *dest = NULL;

	dest = getOperationCodeVariable();
	source = getOperationCodeVariable();

	*dest = *source;
}

void Environment::operationMoveInt() {
	Variable *dest = NULL;
	int literal = 0;

	dest = getOperationCodeVariable();
	literal = getOperationCodeInteger();
	
	dest->set(literal);

	LOGFILE(("Var %x = %i\n", dest->getID(), literal));
}

void Environment::operationMoveFloat() {
	Variable *dest = NULL;
	float literal = 0;

	dest = getOperationCodeVariable();
	literal = getOperationCodeFloat();

	dest->set(literal);

	LOGFILE(("Var %x = %f\n", dest->getID(), literal));
}

void Environment::operationMoveString() {
	// Needs to be done someday :)
}

void Environment::operationAdd() {
	arithmeticStack(&Variable::operator+=);
}

void Environment::operationSub() {
	arithmeticStack(&Variable::operator-=);
}

void Environment::operationMultiply() {
	arithmeticStack(&Variable::operator*=);
}

void Environment::operationDivide() {
	arithmeticStack(&Variable::operator/=);
}

void Environment::opModulate() {
	arithmeticStack(&Variable::operator%=);
}

void Environment::arithmeticStack(void(Variable::*aOper)(const Variable&)) {
	Variable *left = NULL;
	Variable *right = NULL;

	popStackingVariable(right);
	popStackingVariable(left);

	(*left.*aOper)(*right);
	stackPointer.push(left->getID());
}

void Environment::operationAddInt() {
	arithmeticInt(&Variable::operator+=);
}

void Environment::operationSubInt() {
	arithmeticInt(&Variable::operator-=);
}

void Environment::operationMultiplyInt() {
	arithmeticInt(&Variable::operator*=);
}

void Environment::operationDivdeInt() {
	arithmeticInt(&Variable::operator/=);
}

void Environment::operationModulesInt() {
	arithmeticInt(&Variable::operator%=);
}

void Environment::arithmeticInt(void(Variable::*aOper)(const int&)) {
	Variable *var = NULL;
	int literal = 0;

	popStackingVariable(var);
	literal = getOperationCodeInteger();

	(*var.*aOper)(literal);
	stackPointer.push(var->getID());
}

void Environment::operationAddFloat() {
	arithmeticFloat(&Variable::operator+=);
}

void Environment::operationSubdivisionFloat() {
	arithmeticFloat(&Variable::operator-=);
}

void Environment::subdivisionMultiplyFloat() {
	arithmeticFloat(&Variable::operator*=);
}

void Environment::OperationDivdeFloat() {
	arithmeticFloat(&Variable::operator/=);
}

void Environment::arithmeticFloat(void(Variable::*aOper)(const float&)) {
	Variable *var = NULL;
	float literal = 0.f;

	popStackingVariable(var);
	literal = getOperationCodeFloat();

	(*var.*aOper)(literal);
	stackPointer.push(var->getID());
}

void Environment::operationPushScope() {
	Scope *scope = getCurrentScope();
	scope->pushNestedScope();
}

void Environment::operationPopScope() {
	Scope *scope = getCurrentScope();
	scope->popNestedScope();
}

void Environment::operationJump() {
	int dest = getOperationCodeUint();
	operationPointer = dest;

	LOGFILE(("Jumping to %i\n", dest));
}

void Environment::operationJe() {
	LOGFILE(("Comparing ==\n"));
	compareJumpStack(&Variable::operator==);
}

void Environment::operationJne() {
	LOGFILE(("Comparing !=\n"));
	compareJumpStack(&Variable::operator!=);
}

void Environment::operationJg() {
	LOGFILE(("Comparing >\n"));
	compareJumpStack(&Variable::operator>);
}

void Environment::operationJge() {
	LOGFILE(("Comparing >=\n"));
	compareJumpStack(&Variable::operator>=);
}

void Environment::operationJl() {
	LOGFILE(("Comparing <\n"));
	compareJumpStack(&Variable::operator<);
}

void Environment::operationJle() {
	LOGFILE(("Comparing <=\n"));
	compareJumpStack(&Variable::operator<=);
}

void Environment::compareJumpStack(bool(Variable::*aCmp)(const Variable&)const) {
	Variable *left = NULL;
	Variable *right = NULL;

	popStackingVariable(right);
	popStackingVariable(left);

	if ((left->*aCmp)(*right)) {
		operationJump();
	} else {
		operationPointer += 4;
	}
}

void Environment::opJeInt() {
	compareJumpInt(&Variable::operator==);
}

void Environment::operationJneInt() {
	compareJumpInt(&Variable::operator!=);
}

void Environment::operationJgInt() {
	compareJumpInt(&Variable::operator>);
}

void Environment::operationJgeInt() {
	compareJumpInt(&Variable::operator>=);
}

void Environment::operationPJlInt() {
	compareJumpInt(&Variable::operator<);
}

void Environment::operationPJleInt() {
	compareJumpInt(&Variable::operator<=);
}

void Environment::compareJumpInt(bool(Variable::*aCmp)(const int&)const) {
	Variable *var = NULL;
	int literal = 0;

	popStackingVariable(var);
	literal = getOperationCodeInteger();

	if ((*var.*aCmp)(literal)) {
		operationJump();
	} else {
		operationPointer += 4;
	}
}

void Environment::operationPJeF() {
	compareJumpInt(&Variable::operator==);
}

void Environment::operationpJneFloat() {
	compareJumpInt(&Variable::operator!=);
}

void Environment::operationPJgFloat() {
	compareJumpInt(&Variable::operator>);
}

void Environment::operationpJgeFloat() {
	compareJumpInt(&Variable::operator>=);
}

void Environment::operationPJlFloat() {
	compareJumpInt(&Variable::operator<);
}

void Environment::operationPJleFloat() {
	compareJumpInt(&Variable::operator<=);
}

void Environment::compareJumpFloat(bool(Variable::*aCmp)(const float&)const) {
	Variable *var = NULL;
	float literal = 0.f;

	popStackingVariable(var);
	literal = getOperationCodeFloat();

	if ((*var.*aCmp)(literal)) {
		operationJump();
	} else {
		operationPointer += 4;
	}
}

void Environment::operationDataBegin() {
	if (dataDefinition) {
		throw InvalidOpException("Already defining data");
	}
	dataDefinition = true;
}

void Environment::operationDataString() {
	// Needs to be done someday
}

void Environment::operationDataFunction() {
	int funcId = 0;
	int funcPos = 0;

	funcId = getOperationCodeUint();
	funcPos = getOperationCodeUint();

	functions[funcId] = funcPos;
}

void Environment::operationDataEnd() {
	if (!dataDefinition) {
		throw InvalidOpException("Was not defining data");
	}

	dataDefinition = false;
}