#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

// Internal
#include "Variable.h"
#include "../scope.h"
#include "../stack.h"
#include "../codes.h"
#include "../OperationCode.h"

// External
#include <map>
#include <string>
#include <exception>

# define LOGFILE(A) false // No Log files.

class Environment {
	public:
		Environment(OperationCode *aOpcode);
		~Environment();
		int Execute();
	private:
		byte *operationcodes;
		uint operationPointer;
		bool dataDefinition;
		Stack<uint>	stackPointer;
		Stack<uint>	stack;
		Scope scope;
		Stack<Scope*> sizeScope;
		std::map<uint,uint> functions;
		void  operations();
		Variable* getVariableByID(uint aID);
		void popStackingVariable(Variable *&aVar);
		int	getOperationCodeInteger();
		uint getOperationCodeUint();
		float getOperationCodeFloat();
		Variable*getOperationCodeVariable();
		Scope* getCurrentScope();
		void operationPush();
		void operationPushData();
		void operationPop();
		void operationCall();
		void operationCallStd(uint funcId);
		void operationRet();
		void operationAlloc();
		void operationPopMove();
		void operationMove();
		void operationMoveInt();
		void operationMoveFloat();
		void operationMoveString();
		void operationAdd();
		void operationSub();
		void operationMultiply();
		void operationDivide();
		void opModulate();
		void arithmeticStack(void(Variable::*oper)(const Variable&));
		void operationAddInt();
		void operationSubInt();
		void operationMultiplyInt();
		void operationDivdeInt();
		void operationModulesInt();
		void arithmeticInt(void(Variable::*oper)(const int&));
		void operationAddFloat();
		void operationSubdivisionFloat();
		void subdivisionMultiplyFloat();
		void OperationDivdeFloat();
		void arithmeticFloat(void(Variable::*oper)(const float&));
		void operationPushScope();
		void operationPopScope();
		void operationJump();
		void operationJe();
		void operationJne();
		void operationJg();
		void operationJge();
		void operationJl();
		void operationJle();
		void compareJumpStack(bool(Variable::*cmp)(const Variable&)const);
		void opJeInt();
		void operationJneInt();
		void operationJgInt();
		void operationJgeInt();
		void operationPJlInt();
		void operationPJleInt();
		void compareJumpInt(bool(Variable::*cmp)(const int&)const);
		void operationPJeF();
		void operationpJneFloat();
		void operationPJgFloat();
		void operationpJgeFloat();
		void operationPJlFloat();
		void operationPJleFloat();
		void compareJumpFloat(bool(Variable::*cmp)(const float&)const);
		void operationDataBegin();
		void operationDataString();
		void operationDataFunction();
		void operationDataEnd();
};
#endif // ENVIRONMENT_H