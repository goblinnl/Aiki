#ifndef PARSER_H
#define PARSER_H

// Internal
#include "tokens.hpp"
#include "statement.hpp"
#include "../OperationCode.hpp"
#include "../scope.hpp"

class Statement;
class FunctionDefinition;
class PositionInquirer;
class FunctionSignature;

typedef list<Fragment*>::iterator FragmentIter;

class Parser {
	public:
		Parser(string aFile, bool aMainFile);
		~Parser();

		bool parseFile();
		bool compileTokens();
		OperationCode*	getOpcodes();
		void pushScope();
		void popScope();
		bool isInLocalScope();

		void pushNestedScope();
		void popNestedScope();

		uint registerVariable(string aName);
		uint getVariableID(string aName);

		uint registerFunction(FunctionSignature aFuncSign);
		uint registerStdFunction(FunctionSignature aFuncSign);
		uint getFunctionID(string aName);
		FunctionSignature getFunctionSignature(string aFunctionName);
		FunctionSignature getFunctionSignature(uint aFunctionID);

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

		bool buildFragments();
		bool buildIntermediates();
		bool buildBytecode();
		void addFragment(Fragment *aFragment);
		void pushFragmentTail(FragmentIter aTail);
		void popFragmentTail();
		void addHeader();
		void addFunctionData(FunctionDefinition *aFuncDef);

		Stack<FragmentIter> fragmentTailStack;
		FragmentIter fragmentFunctionDef;
		InteropIter headerEnd;
		PositionInquirer *headerStart;
};

#endif // PARSER_H