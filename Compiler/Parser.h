#ifndef PARSER_H
#define PARSER_H

// Internal
#include "tokens.h"
#include "statement.h"
#include "../OperationCode.h"
#include "../scope.h"

class Statement;
class FunctionDefinition;
class PositionInquirer;
class FunctionSignature;

typedef std::list<Fragment*>::iterator FragmentIter;

class Parser {
private:
	static uint	mFunctionID;
	static uint	mStdFunctionID;
	static uint	mVariableID;

	std::string mInputFile;
	bool mIsMainFile;

	Tokens *mTokens;
	OperationCode *mInterMedOperCode;
	CompileScope mCompileScope;
	Stack<CompileScope*> mStackScope;
	std::list<Fragment*> mFragmentList;
	std::list<FunctionSignature> mFunctionSignatureList;
	Stack<FragmentIter> mFragmentTailStack;
	FragmentIter mFragmentFunctionDef;
	InteropIter mHeaderEnd;
	PositionInquirer *mHeaderStart;

public:
	Parser(std::string rFile, bool rMainFile);
	~Parser();

	bool ParseFile();
	bool CompileTokens();
	bool IsInLocalScope();

	void PushScope();
	void PopScope();
	void PushNestedScope();
	void PopNestedScope();

	uint RegisterVariable(std::string rName);
	uint GetVariableID(std::string rName);
	uint RegisterFunction(FunctionSignature rFuncSign);
	uint RegisterStdFunction(FunctionSignature rFuncSign);
	uint GetFunctionID(std::string rName);

	FunctionSignature GetFunctionSignature(std::string rFunctionName);
	FunctionSignature GetFunctionSignature(uint rFunctionID);

	OperationCode*	GetOpcodes();

private:
	bool BuildFragments();
	bool BuildIntermediates();
	bool BuildBytecode();
	void AddFragment(Fragment *rFragment);
	void PushFragmentTail(FragmentIter rTail);
	void PopFragmentTail();
	void AddHeader();
	void AddFunctionData(FunctionDefinition *rFuncDef);

};
#endif // PARSER_H