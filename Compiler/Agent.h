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
};
#endif // AGENT_H