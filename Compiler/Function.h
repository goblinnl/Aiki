#ifndef FUNCTION_H
#define FUNCTION_H

// External
#include <stdlib.h>

// Internal
#include "Fragment.h"
#include "tokens.h"
#include "../Mixer/MCString.h"
#include "../Parser/Variable.h"
#include "../Parser/List.h"
#include "../Parser/ArgumentData.h"
#include "../Parser/Environment.h"

class Expression;
class PositionReference;

class Function {
public:
	Variable** mFixedLocalVars;
	Variable** mStringTableVars;

	MCString* mFixedLocalKeys;
	MCString* mStringTable;
	Variable* mCurrentObject;
	Function* mReferenceFunc;
	ArgParser* mParser;
	Function* mParent;
	Environment* mEnv;
	byte* mCode;
	List<Function*> mRefChilds;
	
	int* mFixedLocalTypes;
	bool* mFixedLocalRef;
	
	int mFixedLocalCount;
	int mCodeSize;
	int mCodeOffset;
	int mSouceFileLine;
	int mStringTableCount;
	
	bool mForceReturn;
	bool mIsCpp;

	MCString mName;
	MCString mFilename;

	List<MCString> mParameters;
	AKFunction mCppFunc;

public:
	Function(ArgParser* rParser);
	~Function();

	Variable* Bind();
	Variable* Bind(Variable* rObject);
	Variable* Bind(ArgumentData* rArgs);
	Variable* Bind(ArgumentData* rArgs, Variable* rObject);
	Variable* Bind(List<Variable*> *rArgs);
	Variable* Bind(List<Variable*> *rArgs, Variable* rObject);
	Variable* InternalBind(ExecuteData* rData);

	void SetRef(Function* rFunc, int rEntryPoint);
	void SetEnv(Environment* rEnv);
	void SetVar(Variable* rVar, int rIndex);
	void SetVar(MCString rKey, Variable* rVar, bool rIsGlobal);

	Function* GetRev();
	Environment* GetEnv();
};

class FunctionCall : public Fragment {
public:
	FunctionCall(Token *rFuncToken);
	void ParseFragment(Tokens * rTokens, ArgParser *rParser);
	void ProvideIntermediates(OperationCode *rOpcode, ArgParser *rParser);
	MCString	GetString();

protected:
	Token::Type		mDelimter;
	Token			*mFunctionToken;
	std::list<Expression*>	mParameters;
	void HandleParameters(OperationCode *rOpcode, ArgParser *rParser);
};

class FunctionDefinition : public Fragment {
protected:
	std::list<Token*> mParameter;
	PositionReference *mPositionReference;
	uint mFunctionID;

public:
	FunctionDefinition();
	PositionReference* GetPositionReference();

	void ParseFragment(Tokens *aTokens, ArgParser *rParser);
	void ProvideIntermediates(OperationCode *rOpcode, ArgParser *rParser);

	uint GetID();

	static bool	IsFunctionDefinition(Tokens *rTokens);

};

class FunctionTail : public Fragment {
public:
	FunctionTail();
	void ParseFragment(Tokens *rTokens, ArgParser *rParser);
	void ProvideIntermediates(OperationCode *rOpcode, ArgParser *rParser);
	PositionReference*	GetPositionReference();

private:
	PositionReference *mPosRef;
};

class FunctionSignature {
private:
	MCString mSignatureName;
	int	mParameterCount;
	uint mFunctionID;

public:
	FunctionSignature(MCString rName, int rParams);
	MCString GetName();
	int GetParameterCount();

	uint GetID();
	void SetID(uint rID);
};
#endif  // FUNCTION_H