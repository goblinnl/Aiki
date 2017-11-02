#include "Statement.h"
#include "IntermediateOper.h"
#include "Expression.h"
#include "tokens.h"
#include "parser.h"

#include "../stack.h"
#include "../Exception.h"


Statement::Statement() {

}

Statement* Statement::CreateStatement(Tokens *aTokens, Parser *aParser) {
	Statement *stmt = NULL;

	const Token *token = aTokens->CheckNext();

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
		stmt->ParseFragment(aTokens, aParser);
	}

	return stmt;
}

void IfStatement::ParseFragment(Tokens *aTokens, Parser *aParser) {
	operators = NULL;
	expression = NULL;
	assignee = NULL;

	delete aTokens->PopExpected(Token::RESERVED);
	delete aTokens->PopExpected(Token::PARANTH_BEG);

	// Check of allocation
	//if(aTokens->checkNext()->token == "if") {
	//	alloc = true;
	//	delete aTokens->popExpected(Token::RESERVED);

		//assignee = aTokens->popExpected(Token::
	//}

	// Parse the expression
	//expression = new Expression();
	//expression->ParseFragment(aTokens, aParser);

	throw NotImplementedException("If statements are not implemented");
}

void IfStatement::ProvideIntermediates(OperationCode *aOpcode, Parser *aParser) {
	throw NotImplementedException("If statements are not implemented");
}

void AssignStatement::ParseFragment(Tokens *aTokens, Parser *aParser) {
	alloc = false;
	operators = NULL;
	expression = NULL;
	assignee = NULL;

	// Check for allocation
	if (aTokens->CheckNext()->token == "var") {
		alloc = true;
		delete aTokens->PopExpected(Token::RESERVED);

		assignee = aTokens->PopExpected(Token::VARIABLE_FUNCTION);
		operators = aTokens->PopIfExists(Token::OPERATOR);
	} else {
		TokenIterertor it = aTokens->GetPointer();
		if ((*it++)->aType == Token::VARIABLE_FUNCTION &&
			(*it)->aType == Token::OPERATOR) {
				assignee = aTokens->PopExpected(Token::VARIABLE_FUNCTION);
				operators = aTokens->PopExpected(Token::OPERATOR);
		}
	}

	// Parse the expression
	expression = new Expression();
	expression->ParseFragment(aTokens, aParser);
}

void AssignStatement::ProvideIntermediates(OperationCode *aOpcode, Parser *aParser) {
	uint varID = 0;

	if (alloc) {
		varID = SetVariable(aParser, assignee->token);
		AllocateVariable(aOpcode, varID);
	} else if (assignee) {
		varID = GetVariableID(aParser, assignee->token);
	}

	expression->ProvideIntermediates(aOpcode, aParser);

	if (varID && operators) {
		HandleOperator(aOpcode, varID);
	}
}

void AssignStatement::HandleOperator(OperationCode *aOpcode, uint aVarID) {
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
		aOpcode->AddInterop(new ByteOperation(OP_POPMOV));
		aOpcode->AddInterop(new DwordOperation(&aVarID));
		return;
	} else {
		throw NotImplementedException("Operator not implemented: " + s);
	}
	
	aOpcode->AddInterop(new ByteOperation(OP_PUSH));
	aOpcode->AddInterop(new DwordOperation(&aVarID));
	aOpcode->AddInterop(new ByteOperation(operation));
	aOpcode->AddInterop(new ByteOperation(OP_POPMOV));
	aOpcode->AddInterop(new DwordOperation(&aVarID));
}

void ReturnStatement::ParseFragment(Tokens *aTokens, Parser *aParser) {
	delete aTokens->PopExpected(Token::RESERVED);

	if (aTokens->CheckNext()->aType == Token::SEMICOLON) {
		expression = NULL;
		delete aTokens->PopNext();
	} else {
		expression = new Expression;
		expression->ParseFragment(aTokens, aParser);
	}
}

void ReturnStatement::ProvideIntermediates(OperationCode *aOpcode, Parser *aParser) {
	if (!aParser->IsInLocalScope()) {
		throw InvalidTokenException("Unexpected return");
	}


	if (expression) {
		uint varID = aParser->RegisterVariable("");
		AllocateVariable(aOpcode, varID);

		expression->ProvideIntermediates(aOpcode, aParser);

		aOpcode->AddInterop(new ByteOperation(OP_POPMOV));
		aOpcode->AddInterop(new DwordOperation(&varID));

		aOpcode->AddInterop(new ByteOperation(OP_RET));
		aOpcode->AddInterop(new DwordOperation(&varID));
	} else {
		uint zero = 0;
		aOpcode->AddInterop(new ByteOperation(OP_RET));
		aOpcode->AddInterop(new DwordOperation(&zero));
	}
}