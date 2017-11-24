#ifndef AGENT_H
#define AGENT_H

// Internal
#include "../Exception.h"
#include "../Singleton.h"
#include "../Stack.h"
#include "../Codes.h"

class Agent;
typedef Singleton<Agent> gAgent;

class Agent {
public:
	Agent();
	~Agent();

	Variable* GetAO(Stack* rStack, Variable* rVar0, Variable* rVar1, int rCode);

	void Duplicate(Stack* rStack);
	void IncreaseIndex(Stack* rStack);
	void GetSize(Stack* rStack);
	void GetIndex(Stack* rStack);
	void SetIndex(Stack* rStack);
	void Set(Stack* rStack);
	void SetLenght(Stack* rStack);
	void New(Stack* rStack);
	void Call(Stack* rStack);
	void CallThis(Stack* rStack);
	void PostIncrement(Stack* rStack);
	void PreIncrement(Stack* rStack);
	void Pop(Stack* rStack);
	void Delete(Stack* rStack);
	void StackNull(Stack* rStack);
	void StackBool(Stack* rStack);
	void StackNumber(Stack* rStack);
	void StackTable(Stack* rStack);
	void StackString(Stack* rStack);
	void StackFunction(Stack* rStack);
	void JumpPS(Stack* rStack);
	void Jump(Stack* rStack);
	void Get(Stack* rStack);
	void GetLenght(Stack* rStack);
	void StringTable(Stack* rStack);
	void CurrentLine(Stack* rStack);
	void GlobalAndLocals(Stack* rStack);

private:	
	void AddNumber(Variable* rRet, const Variable* rVar0, const Variable* rVar1);
	void AddString(Variable* rRet, const Variable* rVar0, const Variable* rVar1);
	void AddNumberToString(Variable* rRet, const Variable* rVar0, const Variable* rVar1);
	void AddStringToNumber(Variable* rRet, const Variable* rVar0, const Variable* rVar1);

	// Arithmetic operators 
	void Add(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void Substract(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void Divide(ArgParser* rParser, Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void DivideString(ArgParser* rParser, Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void Multiply(ArgParser* rParser, Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void MultiplyString(ArgParser* rParser, Variable& rRet, const Variable* rNumber, const Variable* rStr);
	void Equal(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void GreaterThan(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void GreaterOrEqual(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void LessThan(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void LessOrEqual(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void Modulo(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void And(Variable& rRet, Variable* rVar0, Variable* rVar1);
	void Or(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void BitwiseLeft(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void BitwiseRight(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void BitwiseOr(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void BitwiseAnd(Variable& rRet, const Variable* rVar0, const Variable* rVar1);
	void NotEqual(Variable& rRet, const Variable* rVar0, const Variable* rVar1);

};
#endif // AGENT_H