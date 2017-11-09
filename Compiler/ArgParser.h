#ifndef PARSER_H
#define PARSER_H

// Internal
#include "tokens.h"
#include "statement.h"
#include "Stack.h"
#include "GarbageCollector.h"
#include "../Parser/List.h"
#include "../Data/Data.h"
#include "../Global.h"
#include "../Scope.h"
#include "../Exception.h"
#include "../OperationCode.h"
#include "../Mixer/MCommon.h"
#include "../Mixer/MCString.h"

class Statement;
class FunctionDefinition;
class PositionInquirer;
class FunctionSignature;

typedef std::list<Fragment*>::iterator FragmentIter;

class ArgParser {
private:
	static uint	mFunctionID;
	static uint	mStdFunctionID;
	static uint	mVariableID;

	int mStackIndex;

	MCString mInputFile;
	FILE* mFile;
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
	List<Data*> mRegisteredDataTypes;
	Stack mCurrentStack[AK_CALLSTACK_SIZE];
	GarbageCollector mGarbageCollector;

public:
	ArgParser();
	ArgParser(MCString rFile, bool rMainFile);
	~ArgParser();

	bool ParseFile();
	bool OpenFile(const char* rFilename, const char* rMode);

	unsigned long SizeFile();
	unsigned long LocationFile();

	bool CompileTokens();
	bool IsInLocalScope();

	void CloseFile();
	void WriteFile(const void* rPointer, unsigned long rLenght);
	void* ReadFile(unsigned long rLenght);
	const void* ReadFileToEnd();

	void PushScope();
	void PopScope();
	void PushNestedScope();
	void PopNestedScope();
	
	int GetCurrentStackSize() const;

	void Seek(unsigned long rPos, int rOrigin);

	uint RegisterVariable(MCString rName);
	uint GetVariableID(MCString rName);
	uint RegisterFunction(FunctionSignature rFuncSign);
	uint RegisterStdFunction(FunctionSignature rFuncSign);
	uint GetFunctionID(MCString rName);

	FunctionSignature GetFunctionSignature(MCString rFunctionName);
	FunctionSignature GetFunctionSignature(uint rFunctionID);

	GarbageCollector GetGarbageCollector() const;

	OperationCode*	GetOpcodes();

	List<Data*> GetRegisteredDataTypes() const;

	Function* GetCurrentFunction() const;

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