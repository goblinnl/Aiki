#include "Parser.h"
#include "Expression.h"
#include "Function.h"
#include "IntermediateOper.h"
#include "../Functions.h"

uint Parser::functionID = 0;
uint Parser::stdFunctionID = 0;
uint Parser::VariableID = 0;


Parser::Parser(string aFile, bool aMainFile) {
	inputFile = aFile;
	isMainFile = aMainFile;

	tokens = new Tokens();
	interMedOperCode = new OperationCode();
}

Parser::~Parser() {
	delete tokens;
	delete interMedOperCode;
}


bool Parser::ParseFile() {
	try {
		tokens->GenerateTokens(inputFile);
	} catch (exception e) {
		printf("Failed to parse file '%s':\n", inputFile.c_str());
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
	} catch (exception e) {
		printf("Failed to build fragments from tokens:\n");
		printf("%s\n", e.what());
		return false;
	}

	try {
		if (!BuildIntermediates()) {
			return false;
		}
	} catch (exception e) {
		printf("Failed to build intermediates:\n");
		printf("%s\n", e.what());
		return false;
	}

	try {
		if (!BuildBytecode()) {
			return false;
		}
	} catch (exception e) {
		printf("Failed to build bytecode:\n");
		printf("%s\n", e.what());
		return false;
	}

	return true;
}

OperationCode* Parser::GetOpcodes() {
	return interMedOperCode;
}


void Parser::PushScope() {
	stackScope.Push(new CompileScope);
}

void Parser::PopScope() {
	delete stackScope.Pop();
}

bool Parser::IsInLocalScope() {
	return stackScope.Size() > 0;
}


void Parser::PushNestedScope() {
	if (stackScope.Size()) {
		stackScope.Peek()->PushNestedScope();
	} else {
		compileScope.PushNestedScope();
	}
}

void Parser::PopNestedScope() {
	if (stackScope.Size()) {
		stackScope.Peek()->PopNestedScope();
	} else {
		compileScope.PopNestedScope();
	}
}


uint Parser::RegisterVariable(string aName) {
	CompileScope *scope = NULL;
	uint id = 0;

	if (stackScope.Size()) {
		scope = stackScope.Peek();
		id = VAR_LOCAL | (stackScope.Peek()->GetVarCounter());
	} else {
		scope = &compileScope;
		id = VAR_GLOBAL | (++VariableID);
	}

	string *n = new string;
	*n = aName;

	scope->AddItem(id, n);
	return id;
}

uint Parser::GetVariableID(string aName) {
	if (!aName.length()) {
		throw runtime_error("No Variable ID");
	}

	uint id = 0;

	if (stackScope.Size()) {
		id = stackScope.Peek()->GetItemID(&aName);
		if (id) { 
			return id;
		}
	}

	id = compileScope.GetItemID(&aName);
	
	if (id == 0) {
		throw VarNotDefinedException("Variable is not defined in the current scope: " + aName);
	}

	return id;
}



uint Parser::RegisterFunction(FunctionSignature aFuncSign) {
	list<FunctionSignature>::iterator it;
	for (it = functionSignatureList.begin(); it != functionSignatureList.end(); it++) {
		if (it->GetName() == aFuncSign.GetName()) {
			throw FuncAlreadyDefinedException("Function " + aFuncSign.GetName() + " is already defined");
		}
	}

	aFuncSign.SetID(++functionID);
	functionSignatureList.push_back(aFuncSign);

	return functionID;
}

uint Parser::RegisterStdFunction(FunctionSignature aFuncSign) {
	list<FunctionSignature>::iterator it;
	for (it = functionSignatureList.begin(); it != functionSignatureList.end(); it++) {
		if (it->GetName() == aFuncSign.GetName()) {
			throw FuncAlreadyDefinedException("Function " + aFuncSign.GetName() + " is already defined");
		}
	}

	aFuncSign.SetID(++stdFunctionID | FUNC_STD);
	functionSignatureList.push_back(aFuncSign);

	return aFuncSign.GetID();
}

uint Parser::GetFunctionID(string aName) {
	list<FunctionSignature>::iterator it;
	for (it = functionSignatureList.begin(); it != functionSignatureList.end(); it++) {
		if (it->GetName() == aName) {
			return it->GetID();
		}
	}

	throw FuncNotDefinedException("Function is undefined: " + aName);
	return 0;
}

FunctionSignature Parser::GetFunctionSignature(string aFuncName) {
	list<FunctionSignature>::iterator it;
	for (it = functionSignatureList.begin(); it != functionSignatureList.end(); it++) {
		if (it->GetName() == aFuncName) {
			return *it;
		}
	}

	throw  FuncNotDefinedException("Function " + aFuncName + " not defined");
}

FunctionSignature Parser::GetFunctionSignature(uint aFuncID) {
	list<FunctionSignature>::iterator it;
	for (it = functionSignatureList.begin(); it != functionSignatureList.end(); it++) {
		if (it->GetID() == aFuncID) {
			return *it;
		}
	}

	string errorMsg;
	errorMsg += "Function with ID '";
	errorMsg += aFuncID;
	errorMsg += "' not defined";
	throw  FuncNotDefinedException(errorMsg);
}


bool Parser::BuildFragments() {
	PushFragmentTail(fragmentList.end());
	AddHeader();
	fragmentFunctionDef = fragmentList.insert(fragmentList.end(), NULL);

	int stackDepth = 0;
	bool inFunction = false;

	while (tokens->IsMore()) {
		Token *next = tokens->CheckNext();

		Statement *statement = Statement::CreateStatement(tokens, this);
		if (statement) {
			AddFragment(statement);
		} else if (FunctionDefinition::IsFunctionDefinition(tokens)) {
			FunctionDefinition *fdef = new FunctionDefinition();
			fdef->ParseFragment(tokens, this);


			PushFragmentTail(fragmentFunctionDef);
			AddFragment(fdef);
			AddFunctionData(fdef);

			delete tokens->PopExpected(Token::BRACKET_BEG);

			if (inFunction) {
				throw SyntaxErrorException("BuildExcpetion");
			}

			stackDepth++;
			inFunction = true;
			PushScope();

		} else if (next->aType == Token::BRACKET_BEG) {
			stackDepth++;
			PushNestedScope();
			delete tokens->PopNext();
		} else if (next->aType == Token::BRACKET_END) {
			if (--stackDepth == 0) {
				if (inFunction) {
					PopScope();
					inFunction = false;

					FunctionTail *tail = new FunctionTail();
					AddFragment(tail);
					PopFragmentTail();

					tail->GetPositionReference()->AddInquirer(headerStart);
				} else {
					throw InvalidTokenException("Unexpected }");
				}
			} else {
				PopNestedScope();
			}
			delete tokens->PopNext();
		} else {
			throw InvalidTokenException("Unexpected '" + next->token + "'!");
		}
	}

	return true;
}

bool Parser::BuildIntermediates() {
	list<Fragment*>::iterator it;
	for (it = fragmentList.begin(); it != fragmentList.end(); it++) {
		if (!*it) continue;
		(*it)->ProvideIntermediates(interMedOperCode, this);
	}

	// Add a final exit-statement
	uint zero = 0;
	interMedOperCode->AddInterop(new ByteOperation(OP_EXIT));
	interMedOperCode->AddInterop(new DwordOperation(&zero));

	return true;
}

bool Parser::BuildBytecode() {
	return interMedOperCode->BuildBytecodeFromIntermediates();
}


void Parser::AddFragment(Fragment *aFragment) {
	fragmentList.insert(fragmentTailStack.Peek(), aFragment);
}

void Parser::PushFragmentTail(FragmentIter aIter) {
	fragmentTailStack.Push(aIter);
}

void Parser::PopFragmentTail() {
	fragmentTailStack.Pop();
}


void Parser::AddHeader() {
	if (interMedOperCode->Length() != 0) {
		throw InternalErrorException();
	}
	
	interMedOperCode->AddInterop(new ByteOperation(OP_DATA_BEGIN));
	headerEnd = interMedOperCode->AddInterop(new ByteOperation(OP_DATA_END));
	headerStart = new PositionInquirer();

	interMedOperCode->AddInterop(new ByteOperation(OP_JMP));
	interMedOperCode->AddInterop(headerStart);

	PositionReference *posRef = new PositionReference();
	posRef->AddInquirer(headerStart);
	interMedOperCode->AddInterop(posRef);
}	

void Parser::AddFunctionData(FunctionDefinition *aFuncDef) {
	interMedOperCode->PushTail(headerEnd);

	uint funcId = aFuncDef->GetID();

	interMedOperCode->AddInterop(new ByteOperation(OP_DATA_FUNC));
	interMedOperCode->AddInterop(new DwordOperation(&funcId));

	// Request the final position for later
	PositionInquirer *posInq = new PositionInquirer();
	aFuncDef->GetPositionReference()->AddInquirer(posInq);
	interMedOperCode->AddInterop(posInq);

	interMedOperCode->PopTail();
}
