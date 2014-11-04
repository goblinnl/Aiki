#include "Statement.hpp"
#include "IntermediateOper.hpp"
#include "Expression.hpp"
#include "tokens.hpp"
#include "parser.hpp"

#include "../stack.hpp"
#include "../Exception.hpp"


Statement::Statement() {

}

Statement* Statement::createStatement(Tokens *aTokens, Parser *aParser) {
	Statement *stmt = NULL;

	const Token *token = aTokens->checkNext();

	if (token->aType == Token::RESERVED) {
		string s = token->token;

		if (s == "var") {
			stmt = new AssignStatement();
		} else if (s == "return") {
			stmt = new ReturnStatement();
		} else if (s == "if") {
			stmt = new IfStatement();
		}
	} else if (token->aType == Token::VARIABLE_FUNCTION) {
		stmt = new AssignStatement();
	}

	if (stmt) {
		stmt->parseFragment(aTokens, aParser);
	}

	return stmt;
}

void IfStatement::parseFragment(Tokens *aTokens, Parser *aParser) {
	delete aTokens->popExpected(Token::RESERVED);
	delete aTokens->popExpected(Token::PARANTH_BEG);

	throw NotImplementedException("If statements are not implemented");
}

void IfStatement::provideIntermediates(OperationCode *aOpcode, Parser *aParser) {
	throw NotImplementedException("If statements are not implemented");
}

void AssignStatement::parseFragment(Tokens *aTokens, Parser *aParser) {
	alloc = false;
	operators = NULL;
	expression = NULL;
	assignee = NULL;

	// Check for allocation
	if (aTokens->checkNext()->token == "var") {
		alloc = true;
		delete aTokens->popExpected(Token::RESERVED);

		assignee = aTokens->popExpected(Token::VARIABLE_FUNCTION);
		operators = aTokens->popIfExists(Token::OPERATOR);
	} else {
		TokenIterertor it = aTokens->getPointer();
		if ((*it++)->aType == Token::VARIABLE_FUNCTION &&
			(*it)->aType == Token::OPERATOR) {
				assignee = aTokens->popExpected(Token::VARIABLE_FUNCTION);
				operators = aTokens->popExpected(Token::OPERATOR);
		}
	}

	// Parse the expression
	expression = new Expression();
	expression->parseFragment(aTokens, aParser);
}

void AssignStatement::provideIntermediates(OperationCode *aOpcode, Parser *aParser) {
	uint varID = 0;

	if (alloc) {
		varID = setVariable(aParser, assignee->token);
		allocateVariable(aOpcode, varID);
	} else if (assignee) {
		varID = getVariableID(aParser, assignee->token);
	}

	expression->provideIntermediates(aOpcode, aParser);

	if (varID && operators) {
		handleOperator(aOpcode, varID);
	}
}

void AssignStatement::handleOperator(OperationCode *aOpcode, uint aVarID) {
	string s = operators->token;
	byte operation = 0;

	if (s == "+=") {
		operation = OP_ADD;
	} else if (s == "-=") {
		operation = OP_SUB;
	} else if (s == "/=") {
		operation = OP_DIV;
	} else if (s == "*=") {
		operation = OP_MUL;
	} else if (s == "%=") {
		operation = OP_MOD;
	} else if (s == "=") {
		aOpcode->addInterop(new ByteOperation(OP_POPMOV));
		aOpcode->addInterop(new DwordOperation(&aVarID));
		return;
	} else {
		throw NotImplementedException("Operator not implemented: " + s);
	}
	
	aOpcode->addInterop(new ByteOperation(OP_PUSH));
	aOpcode->addInterop(new DwordOperation(&aVarID));
	aOpcode->addInterop(new ByteOperation(operation));
	aOpcode->addInterop(new ByteOperation(OP_POPMOV));
	aOpcode->addInterop(new DwordOperation(&aVarID));
}

void ReturnStatement::parseFragment(Tokens *aTokens, Parser *aParser) {
	delete aTokens->popExpected(Token::RESERVED);

	if (aTokens->checkNext()->aType == Token::SEMICOLON) {
		expression = NULL;
		delete aTokens->popNext();
	} else {
		expression = new Expression;
		expression->parseFragment(aTokens, aParser);
	}
}

void ReturnStatement::provideIntermediates(OperationCode *aOpcode, Parser *aParser) {
	if (!aParser->isInLocalScope()) {
		throw InvalidTokenException("Unexpected return");
	}


	if (expression) {
		uint varID = aParser->registerVariable("");
		allocateVariable(aOpcode, varID);

		expression->provideIntermediates(aOpcode, aParser);

		aOpcode->addInterop(new ByteOperation(OP_POPMOV));
		aOpcode->addInterop(new DwordOperation(&varID));

		aOpcode->addInterop(new ByteOperation(OP_RET));
		aOpcode->addInterop(new DwordOperation(&varID));
	} else {
		uint zero = 0;
		aOpcode->addInterop(new ByteOperation(OP_RET));
		aOpcode->addInterop(new DwordOperation(&zero));
	}
}