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

typedef list<Fragment*>::iterator FragmentIter;

class Parser {
	public:
		Parser(string aFile, bool aMainFile);
		~Parser();

		bool ParseFile();
		bool CompileTokens();
		OperationCode*	GetOpcodes();
		void PushScope();
		void PopScope();
		bool IsInLocalScope();

		void PushNestedScope();
		void PopNestedScope();

		uint RegisterVariable(string aName);
		uint GetVariableID(string aName);

		uint RegisterFunction(FunctionSignature aFuncSign);
		uint RegisterStdFunction(FunctionSignature aFuncSign);
		uint GetFunctionID(string aName);
		FunctionSignature GetFunctionSignature(string aFunctionName);
		FunctionSignature GetFunctionSignature(uint aFunctionID);

	private:
		static uint	functionID;
		static uint	stdFunctionID;
		static uint	VariableID;

		string inputFile;
		bool isMainFile;
		Tokens *tokens;

		OperationCode *interMedOperCode;
		CompileScope compileScope;
		Stack<CompileScope*> stackScope;
		list<Fragment*> fragmentList;
		list<FunctionSignature> functionSignatureList;

		bool BuildFragments();
		bool BuildIntermediates();
		bool BuildBytecode();
		void AddFragment(Fragment *aFragment);
		void PushFragmentTail(FragmentIter aTail);
		void PopFragmentTail();
		void AddHeader();
		void AddFunctionData(FunctionDefinition *aFuncDef);

		Stack<FragmentIter> fragmentTailStack;
		FragmentIter fragmentFunctionDef;
		InteropIter headerEnd;
		PositionInquirer *headerStart;
};

#endif // PARSER_H