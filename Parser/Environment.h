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
		void  Operations();
		Variable* GetVariableByID(uint aID);
		void PopStackingVariable(Variable *&aVar);
		int	GetOperationCodeInteger();
		uint GetOperationCodeUint();
		float GetOperationCodeFloat();
		Variable*GetOperationCodeVariable();
		Scope* GetCurrentScope();
		void OperationPush();
		void OperationPushData();
		void OperationPop();
		void OperationCall();
		void OperationCallStd(uint funcId);
		void OperationRet();
		void OperationAlloc();
		void OperationPopMove();
		void OperationMove();
		void OperationMoveInt();
		void OperationMoveFloat();
		void OperationMoveString();
		void OperationAdd();
		void OperationSub();
		void OperationMultiply();
		void OperationDivide();
		void OpModulate();
		void ArithmeticStack(void(Variable::*oper)(const Variable&));
		void OperationAddInt();
		void OperationSubInt();
		void OperationMultiplyInt();
		void OperationDivdeInt();
		void OperationModulesInt();
		void ArithmeticInt(void(Variable::*oper)(const int&));
		void OperationAddFloat();
		void OperationSubdivisionFloat();
		void SubdivisionMultiplyFloat();
		void OperationDivdeFloat();
		void ArithmeticFloat(void(Variable::*oper)(const float&));
		void OperationPushScope();
		void OperationPopScope();
		void OperationJump();
		void OperationJe();
		void OperationJne();
		void OperationJg();
		void OperationJge();
		void OperationJl();
		void OperationJle();
		void CompareJumpStack(bool(Variable::*cmp)(const Variable&)const);
		void OpJeInt();
		void OperationJneInt();
		void OperationJgInt();
		void OperationJgeInt();
		void OperationPJlInt();
		void OperationPJleInt();
		void CompareJumpInt(bool(Variable::*cmp)(const int&)const);
		void OperationPJeF();
		void OperationpJneFloat();
		void OperationPJgFloat();
		void OperationpJgeFloat();
		void OperationPJlFloat();
		void OperationPJleFloat();
		void CompareJumpFloat(bool(Variable::*cmp)(const float&)const);
		void OperationDataBegin();
		void OperationDataString();
		void OperationDataFunction();
		void OperationDataEnd();
};
#endif // ENVIRONMENT_H