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

void FunctionCall::ParseFragment(Tokens * aTokens, Parser * aParser) {
	delete aTokens->PopExpected(Token::PARANTH_BEG);

	if (aTokens->CheckNext()->aType == Token::PARANTH_END) {
		delete aTokens->PopNext();
	} else {
		while (aTokens->CheckNext()->aType != Token::PARANTH_END) {
			Expression *expr = new Expression(true);
			expr->ParseFragment(aTokens, aParser);
			parameters.push_back(expr);

			if (Token *t = aTokens->PopIfExists(Token::COMMA)) {
				delete t;
			}
		}

		delete aTokens->PopExpected(Token::PARANTH_END);
	}


	FunctionSignature funcSign = aParser->GetFunctionSignature(functionToken->token);
	if (funcSign.GetParameterCount() != parameters.size()) {
		stringstream ss;
		ss << "Function " << funcSign.GetName() << ": "
			<< "Expected " << funcSign.GetParameterCount() << " parameters, "
			<< "received " << parameters.size();
		throw InvalidArgumentException(ss.str());
	}
}

string FunctionCall::GetString() {
	string ret = functionToken->token;

	ret += "(";

	list<Expression*>::iterator it;
	int i = 0;
	for (it = parameters.begin(); it != parameters.end(); it++) {
		if (i++) ret += ", ";
		ret += (*it)->GettingString();
	}

	ret += ") ";

	return ret;
}


void FunctionCall::ProvideIntermediates(OperationCode *aOpcode, Parser *aParser) {
	HandleParameters(aOpcode, aParser);

	uint funcID = aParser->GetFunctionID(functionToken->token);

	aOpcode->AddInterop(new ByteOperation(OP_CALL));
	aOpcode->AddInterop(new DwordOperation(&funcID));
}

void FunctionCall::HandleParameters(OperationCode *aOpcode, Parser *aParser) {
	list<Expression*>::reverse_iterator it;

	for (it=parameters.rbegin(); it != parameters.rend(); it++) {
		(*it)->ProvideIntermediates(aOpcode, aParser);
	}
}



/***** FunctionDefinition *****/
bool FunctionDefinition::IsFunctionDefinition(Tokens *aTokens) {
	return aTokens->CheckNext()->token == "func";
}

FunctionDefinition::FunctionDefinition() {
	functionID = 0;
}

PositionReference* FunctionDefinition::GetPositionReference() {
	return positionReference;
}


void FunctionDefinition::ParseFragment(Tokens *aTokens, Parser *aParser) {
	// Ensure that this really is a function
	Token *token = aTokens->PopExpected(Token::RESERVED);
	if (token->token != "func") {
		throw InvalidTokenException("Expected 'function', got: " + token->token);
	}
	delete token;

	// Get the function name
	Token *funcName = aTokens->PopExpected(Token::VARIABLE_FUNCTION);

	// Delete the opening parentheses
	delete aTokens->PopExpected(Token::PARANTH_BEG);

	// Get the parameters
	while (aTokens->CheckNext()->token == "var") {
		delete aTokens->PopExpected(Token::RESERVED);

		Token *param = aTokens->PopExpected(Token::VARIABLE_FUNCTION);
		parameter.push_back(param);

		Token *comma = aTokens->PopIfExists(Token::COMMA);
		if (comma) {
			delete comma;
		}
	}

	// Register the function
	FunctionSignature sig(funcName->token, parameter.size());
	functionID = aParser->RegisterFunction(sig);

	delete funcName;
	delete aTokens->PopExpected(Token::PARANTH_END);

	if (aTokens->CheckNext()->aType != Token::BRACKET_BEG) {
		throw InvalidTokenException("Expected {");
	}

	positionReference = new PositionReference();
}

void FunctionDefinition::ProvideIntermediates(OperationCode *aOpcode, Parser *aParser) {
	aParser->PushScope();
	aOpcode->AddInterop(positionReference);

	for (TokenIterertor it = parameter.begin(); it != parameter.end(); it++) {
		uint varID = aParser->RegisterVariable((*it)->token);

	
		AllocateVariable(aOpcode, varID);
		aOpcode->AddInterop(new ByteOperation(OP_POPMOV));
		aOpcode->AddInterop(new DwordOperation(&varID));
	}
}

uint FunctionDefinition::GetID() {
	return functionID;
}


FunctionTail::FunctionTail() {
	posRef = new PositionReference();
}

void FunctionTail::ParseFragment(Tokens *aTokens, Parser *aParser) {

}


void FunctionTail::ProvideIntermediates(OperationCode *aOpcode, Parser *aParser) {
	aParser->PopScope();
	uint zero = 0;

	aOpcode->AddInterop(new ByteOperation(OP_RET));
	aOpcode->AddInterop(new DwordOperation(&zero));
	aOpcode->AddInterop(posRef);
}

PositionReference* FunctionTail::GetPositionReference() {
	return posRef;
}


/***** FunctionSignature *****/
FunctionSignature::FunctionSignature(string aName, int aParamCount) {
	signatureName = aName;
	parameterCount = aParamCount;
	functionID = 0;
}

string FunctionSignature::GetName() {
	return signatureName;
}

int FunctionSignature::GetParameterCount() {
	return parameterCount;
}

uint FunctionSignature::GetID() {
	return functionID;
}

void FunctionSignature::SetID(uint aID) {
	functionID = aID;
}