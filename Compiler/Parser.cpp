#include "Parser.h"
#include "Expression.h"
#include "Function.h"
#include "IntermediateOper.h"
#include "../Functions.h"

uint Parser::mFunctionID = 0;
uint Parser::mStdFunctionID = 0;
uint Parser::mVariableID = 0;


Parser::Parser(std::string rFile, bool rMainFile) {
	mInputFile = rFile;
	mIsMainFile = rMainFile;

	mTokens = new Tokens();
	mInterMedOperCode = new OperationCode();
}

Parser::~Parser() {
	delete mTokens;
	delete mInterMedOperCode;
}


bool Parser::ParseFile() {
	try {
		mTokens->GenerateTokens(mInputFile);
	}
	catch(std::exception e) {
		printf("Failed to parse file '%s':\n", mInputFile.c_str());
		printf("%s\n", e.what());
		return false;
	}

	return true;
}

bool Parser::CompileTokens() {
	AikiStd::RegisterFunctions(this);

	try {
		if (!BuildFragments()) {
			return false;
		}
	}
	catch(std::exception e) {
		printf("Failed to build fragments from tokens:\n");
		printf("%s\n", e.what());
		return false;
	}

	try {
		if (!BuildIntermediates()) {
			return false;
		}
	}
	catch(std::exception e) {
		printf("Failed to build intermediates:\n");
		printf("%s\n", e.what());
		return false;
	}

	try {
		if (!BuildBytecode()) {
			return false;
		}
	}
	catch(std::exception e) {
		printf("Failed to build bytecode:\n");
		printf("%s\n", e.what());
		return false;
	}

	return true;
}

OperationCode* Parser::GetOpcodes() {
	return mInterMedOperCode;
}


void Parser::PushScope() {
	mStackScope.Push(new CompileScope);
}

void Parser::PopScope() {
	delete mStackScope.Pop();
}

bool Parser::IsInLocalScope() {
	return mStackScope.Size() > 0;
}


void Parser::PushNestedScope() {
	if (mStackScope.Size()) {
		mStackScope.Peek()->PushNestedScope();
	} else {
		mCompileScope.PushNestedScope();
	}
}

void Parser::PopNestedScope() {
	if (mStackScope.Size()) {
		mStackScope.Peek()->PopNestedScope();
	} else {
		mCompileScope.PopNestedScope();
	}
}


uint Parser::RegisterVariable(std::string rName) {
	CompileScope *scope = NULL;
	uint id = 0;

	if (mStackScope.Size()) {
		scope = mStackScope.Peek();
		id = VAR_LOCAL | (mStackScope.Peek()->GetVarCounter());
	} else {
		scope = &mCompileScope;
		id = VAR_GLOBAL | (++mVariableID);
	}

	std::string *n = new std::string;
	*n = rName;

	scope->AddItem(id, n);
	return id;
}

uint Parser::GetVariableID(std::string rName) {
	if(!rName.length()) {
		throw std::runtime_error("No Variable ID");
	}

	uint id = 0;

	if (mStackScope.Size()) {
		id = mStackScope.Peek()->GetItemID(&rName);
		if (id) { 
			return id;
		}
	}

	id = mCompileScope.GetItemID(&rName);
	
	if (id == 0) {
		throw VarNotDefinedException("Variable is not defined in the current scope: " + rName);
	}

	return id;
}



uint Parser::RegisterFunction(FunctionSignature rFuncSign) {
	std::list<FunctionSignature>::iterator it;
	for (it = mFunctionSignatureList.begin(); it != mFunctionSignatureList.end(); it++) {
		if(it->GetName() == rFuncSign.GetName()) {
			throw FuncAlreadyDefinedException("Function " + rFuncSign.GetName() + " is already defined");
		}
	}

	rFuncSign.SetID(++mFunctionID);
	mFunctionSignatureList.push_back(rFuncSign);

	return mFunctionID;
}

uint Parser::RegisterStdFunction(FunctionSignature rFuncSign) {
	std::list<FunctionSignature>::iterator it;
	for (it = mFunctionSignatureList.begin(); it != mFunctionSignatureList.end(); it++) {
		if(it->GetName() == rFuncSign.GetName()) {
			throw FuncAlreadyDefinedException("Function " + rFuncSign.GetName() + " is already defined");
		}
	}

	rFuncSign.SetID(++mStdFunctionID | FUNC_STD);
	mFunctionSignatureList.push_back(rFuncSign);

	return rFuncSign.GetID();
}

uint Parser::GetFunctionID(std::string rName) {
	std::list<FunctionSignature>::iterator it;
	for (it = mFunctionSignatureList.begin(); it != mFunctionSignatureList.end(); it++) {
		if(it->GetName() == rName) {
			return it->GetID();
		}
	}

	throw FuncNotDefinedException("Function is undefined: " + rName);
	return 0;
}

FunctionSignature Parser::GetFunctionSignature(std::string rFuncName) {
	std::list<FunctionSignature>::iterator it;
	for (it = mFunctionSignatureList.begin(); it != mFunctionSignatureList.end(); it++) {
		if(it->GetName() == rFuncName) {
			return *it;
		}
	}

	throw  FuncNotDefinedException("Function " + rFuncName + " not defined");
}

FunctionSignature Parser::GetFunctionSignature(uint rFuncID) {
	std::list<FunctionSignature>::iterator it;
	for (it = mFunctionSignatureList.begin(); it != mFunctionSignatureList.end(); it++) {
		if(it->GetID() == rFuncID) {
			return *it;
		}
	}

	std::string errorMsg;
	errorMsg += "Function with ID '";
	errorMsg += rFuncID;
	errorMsg += "' not defined";
	throw  FuncNotDefinedException(errorMsg);
}


bool Parser::BuildFragments() {
	PushFragmentTail(mFragmentList.end());
	AddHeader();
	mFragmentFunctionDef = mFragmentList.insert(mFragmentList.end(), NULL);

	int stackDepth = 0;
	bool inFunction = false;

	while (mTokens->IsMore()) {
		Token *next = mTokens->CheckNext();

		Statement *statement = Statement::CreateStatement(mTokens, this);
		if (statement) {
			AddFragment(statement);
		} else if (FunctionDefinition::IsFunctionDefinition(mTokens)) {
			FunctionDefinition *fdef = new FunctionDefinition();
			fdef->ParseFragment(mTokens, this);


			PushFragmentTail(mFragmentFunctionDef);
			AddFragment(fdef);
			AddFunctionData(fdef);

			delete mTokens->PopExpected(Token::BRACKET_BEG);

			if (inFunction) {
				throw SyntaxErrorException("BuildExcpetion");
			}

			stackDepth++;
			inFunction = true;
			PushScope();

		} else if (next->mType == Token::BRACKET_BEG) {
			stackDepth++;
			PushNestedScope();
			delete mTokens->PopNext();
		}
		else if(next->mType == Token::BRACKET_END) {
			if (--stackDepth == 0) {
				if (inFunction) {
					PopScope();
					inFunction = false;

					FunctionTail *tail = new FunctionTail();
					AddFragment(tail);
					PopFragmentTail();

					tail->GetPositionReference()->AddInquirer(mHeaderStart);
				} else {
					throw InvalidTokenException("Unexpected }");
				}
			} else {
				PopNestedScope();
			}
			delete mTokens->PopNext();
		} else {
			throw InvalidTokenException("Unexpected '" + next->mToken + "'!");
		}
	}

	return true;
}

bool Parser::BuildIntermediates() {
	std::list<Fragment*>::iterator it;
	for (it = mFragmentList.begin(); it != mFragmentList.end(); it++) {
		if (!*it) continue;
		(*it)->ProvideIntermediates(mInterMedOperCode, this);
	}

	// Add a final exit-statement
	uint zero = 0;
	mInterMedOperCode->AddInterop(new ByteOperation(OP_EXIT));
	mInterMedOperCode->AddInterop(new DwordOperation(&zero));

	return true;
}

bool Parser::BuildBytecode() {
	return mInterMedOperCode->BuildBytecodeFromIntermediates();
}


void Parser::AddFragment(Fragment *rFragment) {
	mFragmentList.insert(mFragmentTailStack.Peek(), rFragment);
}

void Parser::PushFragmentTail(FragmentIter rIter) {
	mFragmentTailStack.Push(rIter);
}

void Parser::PopFragmentTail() {
	mFragmentTailStack.Pop();
}


void Parser::AddHeader() {
	if (mInterMedOperCode->Length() != 0) {
		throw InternalErrorException();
	}
	
	mInterMedOperCode->AddInterop(new ByteOperation(OP_DATA_BEGIN));
	mHeaderEnd = mInterMedOperCode->AddInterop(new ByteOperation(OP_DATA_END));
	mHeaderStart = new PositionInquirer();

	mInterMedOperCode->AddInterop(new ByteOperation(OP_JMP));
	mInterMedOperCode->AddInterop(mHeaderStart);

	PositionReference *posRef = new PositionReference();
	posRef->AddInquirer(mHeaderStart);
	mInterMedOperCode->AddInterop(posRef);
}	

void Parser::AddFunctionData(FunctionDefinition *rFuncDef) {
	mInterMedOperCode->PushTail(mHeaderEnd);

	uint funcId = rFuncDef->GetID();

	mInterMedOperCode->AddInterop(new ByteOperation(OP_DATA_FUNC));
	mInterMedOperCode->AddInterop(new DwordOperation(&funcId));

	// Request the final position for later
	PositionInquirer *posInq = new PositionInquirer();
	rFuncDef->GetPositionReference()->AddInquirer(posInq);
	mInterMedOperCode->AddInterop(posInq);

	mInterMedOperCode->PopTail();
}
