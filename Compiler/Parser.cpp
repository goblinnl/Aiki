#include "Parser.hpp"
#include "Expression.hpp"
#include "Function.hpp"
#include "IntermediateOper.hpp"
#include "../Functions.hpp"

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


bool Parser::parseFile() {
	try {
		tokens->generateTokens(inputFile);
	} catch (exception e) {
		printf("Failed to parse file '%s':\n", inputFile.c_str());
		printf("%s\n", e.what());
		return false;
	}

	return true;
}

bool Parser::compileTokens() {
	AikiStd::registerFunctions(this);

	try {
		if (!buildFragments()) {
			return false;
		}
	} catch (exception e) {
		printf("Failed to build fragments from tokens:\n");
		printf("%s\n", e.what());
		return false;
	}

	try {
		if (!buildIntermediates()) {
			return false;
		}
	} catch (exception e) {
		printf("Failed to build intermediates:\n");
		printf("%s\n", e.what());
		return false;
	}

	try {
		if (!buildBytecode()) {
			return false;
		}
	} catch (exception e) {
		printf("Failed to build bytecode:\n");
		printf("%s\n", e.what());
		return false;
	}

	return true;
}

OperationCode* Parser::getOpcodes() {
	return interMedOperCode;
}


void Parser::pushScope() {
	stackScope.push(new CompileScope);
}

void Parser::popScope() {
	delete stackScope.pop();
}

bool Parser::isInLocalScope() {
	return stackScope.Size() > 0;
}


void Parser::pushNestedScope() {
	if (stackScope.Size()) {
		stackScope.peek()->pushNestedScope();
	} else {
		compileScope.pushNestedScope();
	}
}

void Parser::popNestedScope() {
	if (stackScope.Size()) {
		stackScope.peek()->popNestedScope();
	} else {
		compileScope.popNestedScope();
	}
}


uint Parser::registerVariable(string aName) {
	CompileScope *scope = NULL;
	uint id = 0;

	if (stackScope.Size()) {
		scope = stackScope.peek();
		id = VAR_LOCAL | (stackScope.peek()->getVarCounter());
	} else {
		scope = &compileScope;
		id = VAR_GLOBAL | (++VariableID);
	}

	string *n = new string;
	*n = aName;

	scope->addItem(id, n);
	return id;
}

uint Parser::getVariableID(string aName) {
	if (!aName.length()) {
		throw runtime_error("No Variable ID");
	}

	uint id = 0;

	if (stackScope.Size()) {
		id = stackScope.peek()->getItemID(&aName);
		if (id) { 
			return id;
		}
	}

	id = compileScope.getItemID(&aName);
	
	if (id == 0) {
		throw VarNotDefinedException("Variable is not defined in the current scope: " + aName);
	}

	return id;
}



uint Parser::registerFunction(FunctionSignature aFuncSign) {
	list<FunctionSignature>::iterator it;
	for (it = functionSignatureList.begin(); it != functionSignatureList.end(); it++) {
		if (it->getName() == aFuncSign.getName()) {
			throw FuncAlreadyDefinedException("Function " + aFuncSign.getName() + " is already defined");
		}
	}

	aFuncSign.setID(++functionID);
	functionSignatureList.push_back(aFuncSign);

	return functionID;
}

uint Parser::registerStdFunction(FunctionSignature aFuncSign) {
	list<FunctionSignature>::iterator it;
	for (it = functionSignatureList.begin(); it != functionSignatureList.end(); it++) {
		if (it->getName() == aFuncSign.getName()) {
			throw FuncAlreadyDefinedException("Function " + aFuncSign.getName() + " is already defined");
		}
	}

	aFuncSign.setID(++stdFunctionID | FUNC_STD);
	functionSignatureList.push_back(aFuncSign);

	return aFuncSign.getID();
}

uint Parser::getFunctionID(string aName) {
	list<FunctionSignature>::iterator it;
	for (it = functionSignatureList.begin(); it != functionSignatureList.end(); it++) {
		if (it->getName() == aName) {
			return it->getID();
		}
	}

	throw FuncNotDefinedException("Function is undefined: " + aName);
	return 0;
}

FunctionSignature Parser::getFunctionSignature(string aFuncName) {
	list<FunctionSignature>::iterator it;
	for (it = functionSignatureList.begin(); it != functionSignatureList.end(); it++) {
		if (it->getName() == aFuncName) {
			return *it;
		}
	}

	throw  FuncNotDefinedException("Function " + aFuncName + " not defined");
}

FunctionSignature Parser::getFunctionSignature(uint aFuncID) {
	list<FunctionSignature>::iterator it;
	for (it = functionSignatureList.begin(); it != functionSignatureList.end(); it++) {
		if (it->getID() == aFuncID) {
			return *it;
		}
	}

	string errorMsg;
	errorMsg += "Function with ID '";
	errorMsg += aFuncID;
	errorMsg += "' not defined";
	throw  FuncNotDefinedException(errorMsg);
}


bool Parser::buildFragments() {
	pushFragmentTail(fragmentList.end());
	addHeader();
	fragmentFunctionDef = fragmentList.insert(fragmentList.end(), NULL);

	int stackDepth = 0;
	bool inFunction = false;

	while (tokens->isMore()) {
		Token *next = tokens->checkNext();

		Statement *statement = Statement::createStatement(tokens, this);
		if (statement) {
			addFragment(statement);
		} else if (FunctionDefinition::isFunctionDefinition(tokens)) {
			FunctionDefinition *fdef = new FunctionDefinition();
			fdef->parseFragment(tokens, this);


			pushFragmentTail(fragmentFunctionDef);
			addFragment(fdef);
			addFunctionData(fdef);

			delete tokens->popExpected(Token::BRACKET_BEG);

			if (inFunction) {
				throw SyntaxErrorException("BuildExcpetion");
			}

			stackDepth++;
			inFunction = true;
			pushScope();

		} else if (next->aType == Token::BRACKET_BEG) {
			stackDepth++;
			pushNestedScope();
			delete tokens->popNext();
		} else if (next->aType == Token::BRACKET_END) {
			if (--stackDepth == 0) {
				if (inFunction) {
					popScope();
					inFunction = false;

					FunctionTail *tail = new FunctionTail();
					addFragment(tail);
					popFragmentTail();

					tail->getPositionReference()->addInquirer(headerStart);
				} else {
					throw InvalidTokenException("Unexpected }");
				}
			} else {
				popNestedScope();
			}
			delete tokens->popNext();
		} else {
			throw InvalidTokenException("Unexpected '" + next->token + "'!");
		}
	}

	return true;
}

bool Parser::buildIntermediates() {
	list<Fragment*>::iterator it;
	for (it = fragmentList.begin(); it != fragmentList.end(); it++) {
		if (!*it) continue;
		(*it)->provideIntermediates(interMedOperCode, this);
	}

	// Add a final exit-statement
	uint zero = 0;
	interMedOperCode->addInterop(new ByteOperation(OP_EXIT));
	interMedOperCode->addInterop(new DwordOperation(&zero));

	return true;
}

bool Parser::buildBytecode() {
	return interMedOperCode->buildBytecodeFromIntermediates();
}


void Parser::addFragment(Fragment *aFragment) {
	fragmentList.insert(fragmentTailStack.peek(), aFragment);
}

void Parser::pushFragmentTail(FragmentIter aIter) {
	fragmentTailStack.push(aIter);
}

void Parser::popFragmentTail() {
	fragmentTailStack.pop();
}


void Parser::addHeader() {
	if (interMedOperCode->length() != 0) {
		throw InternalErrorException();
	}
	
	interMedOperCode->addInterop(new ByteOperation(OP_DATA_BEGIN));
	headerEnd = interMedOperCode->addInterop(new ByteOperation(OP_DATA_END));
	headerStart = new PositionInquirer();

	interMedOperCode->addInterop(new ByteOperation(OP_JMP));
	interMedOperCode->addInterop(headerStart);

	PositionReference *posRef = new PositionReference();
	posRef->addInquirer(headerStart);
	interMedOperCode->addInterop(posRef);
}	

void Parser::addFunctionData(FunctionDefinition *aFuncDef) {
	interMedOperCode->pushTail(headerEnd);

	uint funcId = aFuncDef->getID();

	interMedOperCode->addInterop(new ByteOperation(OP_DATA_FUNC));
	interMedOperCode->addInterop(new DwordOperation(&funcId));

	// Request the final position for later
	PositionInquirer *posInq = new PositionInquirer();
	aFuncDef->getPositionReference()->addInquirer(posInq);
	interMedOperCode->addInterop(posInq);

	interMedOperCode->popTail();
}
