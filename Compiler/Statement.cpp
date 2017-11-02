#include "Statement.h"
#include "IntermediateOper.h"
#include "Expression.h"
#include "tokens.h"
#include "parser.h"

#include "../stack.h"
#include "../Exception.h"


Statement::Statement() {

}

Statement* Statement::CreateStatement(Tokens *rTokens, Parser *rParser) {
	Statement *stmt = NULL;

	const Token *token = rTokens->CheckNext();

	if (token->mType == Token::RESERVED) {
		std::string s = token->mToken;

		if (s == "var") {
			stmt = new AssignStatement();
		} else if (s == "return") {
			stmt = new ReturnStatement();
		} else if (s == "if") {
			stmt = new IfStatement();
		}
	}
	else if(token->mType == Token::VARIABLE_FUNCTION) {
		stmt = new AssignStatement();
	}

	if (stmt) {
		stmt->ParseFragment(rTokens, rParser);
	}

	return stmt;
}

void IfStatement::ParseFragment(Tokens *rTokens, Parser *rParser) {
	mOperators = NULL;
	mExpression = NULL;
	mAssignee = NULL;

	delete rTokens->PopExpected(Token::RESERVED);
	delete rTokens->PopExpected(Token::PARANTH_BEG);

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

void IfStatement::ProvideIntermediates(OperationCode *rOpcode, Parser *rParser) {
	throw NotImplementedException("If statements are not implemented");
}

void AssignStatement::ParseFragment(Tokens *rTokens, Parser *rParser) {
	mAlloc = false;
	mOperators = NULL;
	mExpression = NULL;
	mAssignee = NULL;

	// Check for allocation
	if(rTokens->CheckNext()->mToken == "var") {
		mAlloc = true;
		delete rTokens->PopExpected(Token::RESERVED);

		mAssignee = rTokens->PopExpected(Token::VARIABLE_FUNCTION);
		mOperators = rTokens->PopIfExists(Token::OPERATOR);
	} else {
		TokenIterertor it = rTokens->GetPointer();
		if ((*it++)->mType == Token::VARIABLE_FUNCTION &&
			(*it)->mType == Token::OPERATOR) {
			mAssignee = rTokens->PopExpected(Token::VARIABLE_FUNCTION);
			mOperators = rTokens->PopExpected(Token::OPERATOR);
		}
	}

	// Parse the expression
	mExpression = new Expression();
	mExpression->ParseFragment(rTokens, rParser);
}

void AssignStatement::ProvideIntermediates(OperationCode *rOpcode, Parser *rParser) {
	uint varID = 0;

	if (mAlloc) {
		varID = SetVariable(rParser, mAssignee->mToken);
		AllocateVariable(rOpcode, varID);
	} else if (mAssignee) {
		varID = GetVariableID(rParser, mAssignee->mToken);
	}

	mExpression->ProvideIntermediates(rOpcode, rParser);

	if (varID && mOperators) {
		HandleOperator(rOpcode, varID);
	}
}

void AssignStatement::HandleOperator(OperationCode *rOpcode, uint rVarID) {
	std::string s = mOperators->mToken;
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
		rOpcode->AddInterop(new ByteOperation(OP_POPMOV));
		rOpcode->AddInterop(new DwordOperation(&rVarID));
		return;
	} 
	else {
		throw NotImplementedException("Operator not implemented: " + s);
	}
	
	rOpcode->AddInterop(new ByteOperation(OP_PUSH));
	rOpcode->AddInterop(new DwordOperation(&rVarID));
	rOpcode->AddInterop(new ByteOperation(operation));
	rOpcode->AddInterop(new ByteOperation(OP_POPMOV));
	rOpcode->AddInterop(new DwordOperation(&rVarID));
}

void ReturnStatement::ParseFragment(Tokens *rTokens, Parser *rParser) {
	delete rTokens->PopExpected(Token::RESERVED);

	if(rTokens->CheckNext()->mType == Token::SEMICOLON) {
		mExpression = NULL;
		delete rTokens->PopNext();
	}
	else {
		mExpression = new Expression;
		mExpression->ParseFragment(rTokens, rParser);
	}
}

void ReturnStatement::ProvideIntermediates(OperationCode *rOpcode, Parser *rParser) {
	if(!rParser->IsInLocalScope()) {
		throw InvalidTokenException("Unexpected return");
	}


	if (mExpression) {
		uint varID = rParser->RegisterVariable("");
		AllocateVariable(rOpcode, varID);

		mExpression->ProvideIntermediates(rOpcode, rParser);

		rOpcode->AddInterop(new ByteOperation(OP_POPMOV));
		rOpcode->AddInterop(new DwordOperation(&varID));

		rOpcode->AddInterop(new ByteOperation(OP_RET));
		rOpcode->AddInterop(new DwordOperation(&varID));
	}
	else {
		uint zero = 0;
		rOpcode->AddInterop(new ByteOperation(OP_RET));
		rOpcode->AddInterop(new DwordOperation(&zero));
	}
}