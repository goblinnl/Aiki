#include "Function.h"
#include "Expression.h"
#include "IntermediateOper.h"
#include "parser.h"

#include "../stack.h"
#include "../Exception.h"

#include <sstream>


FunctionCall::FunctionCall(Token *aFuncToken) {
	functionToken = aFuncToken;
}

void FunctionCall::parseFragment(Tokens * aTokens, Parser * aParser) {
	delete aTokens->popExpected(Token::PARANTH_BEG);

	if (aTokens->checkNext()->aType == Token::PARANTH_END) {
		delete aTokens->popNext();
	} else {
		while (aTokens->checkNext()->aType != Token::PARANTH_END) {
			Expression *expr = new Expression(true);
			expr->parseFragment(aTokens, aParser);
			parameters.push_back(expr);

			if (Token *t = aTokens->popIfExists(Token::COMMA)) {
				delete t;
			}
		}

		delete aTokens->popExpected(Token::PARANTH_END);
	}


	FunctionSignature funcSign = aParser->getFunctionSignature(functionToken->token);
	if (funcSign.getParameterCount() != parameters.size()) {
		stringstream ss;
		ss << "Function " << funcSign.getName() << ": "
			<< "Expected " << funcSign.getParameterCount() << " parameters, "
			<< "received " << parameters.size();
		throw InvalidArgumentException(ss.str());
	}
}

string FunctionCall::getString() {
	string ret = functionToken->token;

	ret += "(";

	list<Expression*>::iterator it;
	int i = 0;
	for (it = parameters.begin(); it != parameters.end(); it++) {
		if (i++) ret += ", ";
		ret += (*it)->gettingString();
	}

	ret += ") ";

	return ret;
}


void FunctionCall::provideIntermediates(OperationCode *aOpcode, Parser *aParser) {
	handleParameters(aOpcode, aParser);

	uint funcID = aParser->getFunctionID(functionToken->token);

	aOpcode->addInterop(new ByteOperation(OP_CALL));
	aOpcode->addInterop(new DwordOperation(&funcID));
}

void FunctionCall::handleParameters(OperationCode *aOpcode, Parser *aParser) {
	list<Expression*>::reverse_iterator it;

	for (it=parameters.rbegin(); it != parameters.rend(); it++) {
		(*it)->provideIntermediates(aOpcode, aParser);
	}
}



/***** FunctionDefinition *****/
bool FunctionDefinition::isFunctionDefinition(Tokens *aTokens) {
	return aTokens->checkNext()->token == "func";
}

FunctionDefinition::FunctionDefinition() {
	functionID = 0;
}

PositionReference* FunctionDefinition::getPositionReference() {
	return positionReference;
}


void FunctionDefinition::parseFragment(Tokens *aTokens, Parser *aParser) {
	// Ensure that this really is a function
	Token *token = aTokens->popExpected(Token::RESERVED);
	if (token->token != "func") {
		throw InvalidTokenException("Expected 'function', got: " + token->token);
	}
	delete token;

	// Get the function name
	Token *funcName = aTokens->popExpected(Token::VARIABLE_FUNCTION);

	// Delete the opening parentheses
	delete aTokens->popExpected(Token::PARANTH_BEG);

	// Get the parameters
	while (aTokens->checkNext()->token == "var") {
		delete aTokens->popExpected(Token::RESERVED);

		Token *param = aTokens->popExpected(Token::VARIABLE_FUNCTION);
		parameter.push_back(param);

		Token *comma = aTokens->popIfExists(Token::COMMA);
		if (comma) {
			delete comma;
		}
	}

	// Register the function
	FunctionSignature sig(funcName->token, parameter.size());
	functionID = aParser->registerFunction(sig);

	delete funcName;
	delete aTokens->popExpected(Token::PARANTH_END);

	if (aTokens->checkNext()->aType != Token::BRACKET_BEG) {
		throw InvalidTokenException("Expected {");
	}

	positionReference = new PositionReference();
}

void FunctionDefinition::provideIntermediates(OperationCode *aOpcode, Parser *aParser) {
	aParser->pushScope();
	aOpcode->addInterop(positionReference);

	for (TokenIterertor it = parameter.begin(); it != parameter.end(); it++) {
		uint varID = aParser->registerVariable((*it)->token);

	
		allocateVariable(aOpcode, varID);
		aOpcode->addInterop(new ByteOperation(OP_POPMOV));
		aOpcode->addInterop(new DwordOperation(&varID));
	}
}

uint FunctionDefinition::getID() {
	return functionID;
}


FunctionTail::FunctionTail() {
	posRef = new PositionReference();
}

void FunctionTail::parseFragment(Tokens *aTokens, Parser *aParser) {

}


void FunctionTail::provideIntermediates(OperationCode *aOpcode, Parser *aParser) {
	aParser->popScope();
	uint zero = 0;

	aOpcode->addInterop(new ByteOperation(OP_RET));
	aOpcode->addInterop(new DwordOperation(&zero));
	aOpcode->addInterop(posRef);
}

PositionReference* FunctionTail::getPositionReference() {
	return posRef;
}


/***** FunctionSignature *****/
FunctionSignature::FunctionSignature(string aName, int aParamCount) {
	signatureName = aName;
	parameterCount = aParamCount;
	functionID = 0;
}

string FunctionSignature::getName() {
	return signatureName;
}

int FunctionSignature::getParameterCount() {
	return parameterCount;
}

uint FunctionSignature::getID() {
	return functionID;
}

void FunctionSignature::setID(uint aID) {
	functionID = aID;
}