#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

// Internal
#include "Variable.h"
#include "Table.h"
#include "../scope.h"
#include "../stack.h"
#include "../codes.h"
#include "../OperationCode.h"
#include "../Compiler/ArgParser.h"

// External
#include <map>
#include <string>
#include <exception>

# define LOGFILE(A) false // No Log files.

class Environment {
private:
	byte *mOperationcodes;
	uint mOperationPointer;
	bool mDataDefinition;
	Stack<uint>	mStackPointer;
	Stack<uint>	mStack;
	Scope mScope;
	Stack<Scope*> mSizeScope;
	std::map<uint, uint> mFunctions;
	int mRefCounter;

private:
	Environment* mParent;
	Table mVariables;

public:
	Environment(OperationCode *rOpcode);
	Environment(ArgParser* rParser);	
	~Environment();
	int Execute();

private:
	void Operations();
	void PopStackingVariable(Variable *&rVar);
	void OperationPush();
	void OperationPushData();
	void OperationPop();
	void OperationCall();
	void OperationCallStd(uint rFuncID);
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
	void ArithmeticStack(void(Variable::*rOper)(const Variable&));
	void OperationAddInt();
	void OperationSubInt();
	void OperationMultiplyInt();
	void OperationDivdeInt();
	void OperationModulesInt();
	void ArithmeticInt(void(Variable::*rOper)(const int&));
	void OperationAddFloat();
	void OperationSubdivisionFloat();
	void SubdivisionMultiplyFloat();
	void OperationDivdeFloat();
	void ArithmeticFloat(void(Variable::*rOper)(const float&));
	void OperationPushScope();
	void OperationPopScope();
	void OperationJump();
	void OperationJe();
	void OperationJne();
	void OperationJg();
	void OperationJge();
	void OperationJl();
	void OperationJle();
	void CompareJumpStack(bool(Variable::*rCmp)(const Variable&)const);
	void OpJeInt();
	void OperationJneInt();
	void OperationJgInt();
	void OperationJgeInt();
	void OperationPJlInt();
	void OperationPJleInt();
	void CompareJumpInt(bool(Variable::*rCmp)(const int&)const);
	void OperationPJeF();
	void OperationpJneFloat();
	void OperationPJgFloat();
	void OperationpJgeFloat();
	void OperationPJlFloat();
	void OperationPJleFloat();
	void CompareJumpFloat(bool(Variable::*rCmp)(const float&)const);
	void OperationDataBegin();
	void OperationDataString();
	void OperationDataFunction();
	void OperationDataEnd();

	Scope* GetCurrentScope();

	Variable* GetVariableByID(uint rID);
	Variable*GetOperationCodeVariable();

	uint GetOperationCodeUint();

	int	GetOperationCodeInteger();

	float GetOperationCodeFloat();
};
#endif // ENVIRONMENT_H