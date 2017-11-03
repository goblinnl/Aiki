
// Internal
#include "Tokens.h"
#include "../Exception.h"
#include "../Parser/Variable.h"

// External
#include <fstream>

MCString Token::GetStringValue(Token::Type rType) {
	switch(rType) {
	case UNDEFINED: return "UNDEFINED";
	case OPERATOR: return "OPERATOR";
	case PARANTH_BEG: return "PARANTH_BEG";
	case PARANTH_END: return "PARANTH_END";
	case RESERVED: return "RESERVED";
	case BRACKET_BEG: return "BRACKET_BEG";
	case BRACKET_END: return "BRACKET_END";
	case SEMICOLON: return "SEMICOLON";
	case DOT: return "DOT";
	case COMMA: return "COMMA";
	case VARIABLE_FUNCTION: return "VARFUNC";
	case VALUE:	return "VALUE";
	case VARIABLE_STRING: return "VAL_STRING";
	case VARIABLE_INT: return "VAL_INT";
	case VARIABLE_FLOAT: return "VAL_FLOAT";
	case INVALID: return "INVALID";
	}
	return "???";
}

Tokens::Tokens() {
	pointer = tokens.begin();
}

void Tokens::GenerateTokens(MCString rFileName) {
	std::ifstream file;
	file.open(rFileName);
	if(!file.good()) {
		throw FileNotFoundException("File couldn't be found: " + rFileName);
	}

	while(GetToken(file) && !ReachedEnd(file));

	pointer = tokens.begin();
}

bool Tokens::IsMore() {
	return tokens.size() > 0
		&& pointer != tokens.end();
}


Token* Tokens::PopIfExists(Token::Type rType) {
	Token *token = NULL;

	if((*pointer)->mType & rType) {
		token = *pointer;
		pointer = tokens.erase(pointer);
	}

	return token;
}

Token* Tokens::PopExpected(Token::Type rType) {
	Token *token = PopIfExists(rType);

	if(token) {
		return token;
	}

	// All hell is loose - RELEASE THE KRAKEN!
	token = PopNext();
	throw InvalidTokenException("Expected '" + Token::GetStringValue(rType) + "', got: '" + (token ? Token::GetStringValue(token->mType) : "NULL") + "'.");
}

Token* Tokens::PopNext() {
	Token *token = *pointer;
	pointer = tokens.erase(pointer);
	return token;
}

Token* Tokens::CheckNext() {
	return *pointer;
}

TokenIterertor Tokens::GetFirstIterator() {
	return tokens.begin();
}

TokenIterertor Tokens::GetPointer() {
	return pointer;
}

void Tokens::SetPointer(TokenIterertor aIterator) {
	pointer = aIterator;
}

bool Tokens::GetToken(std::ifstream &rFile) {
	if(!rFile.good()) return false;

	Token token;
	SeekNextToken(rFile);

	if(GetSpecialChar(rFile)) {
		return true;
	}

	if(GetOperator(rFile)) {
		return true;
	}

	if(GetWord(rFile)) {
		return true;
	}

	return false;
}

bool Tokens::GetOperator(std::ifstream &rFile) {
	MCString str;
	char lastChar = 0;

	while(PeekOperator(rFile, lastChar) && str.Length() <= 1) {
		lastChar = rFile.get();
		str += lastChar;
	}

	if(str.Length()) {
		Token *t = new Token(str, Token::OPERATOR);
		tokens.push_back(t);

		DetermineOperator(t);

		return true;
	}
	return false;
}

bool Tokens::GetSpecialChar(std::ifstream &rFile) {
	char ch = rFile.peek();
	MCString str;
	str = ch;

	Token *t = new Token(str);

	switch(ch) {
	case '(':
		t->mType = Token::PARANTH_BEG;
		break;

	case ')':
		t->mType = Token::PARANTH_END;
		break;

	case '{':
		t->mType = Token::BRACKET_BEG;
		break;

	case '}':
		t->mType = Token::BRACKET_END;
		break;

	case ';':
		t->mType = Token::SEMICOLON;
		break;

	case '.':
		t->mType = Token::DOT;
		break;

	case ',':
		t->mType = Token::COMMA;
		break;

	default:
		delete t;
		t = NULL;
		return false;
	}

	tokens.push_back(t);
	rFile.get();
	return true;
}

bool Tokens::GetWord(std::ifstream &rFile) {
	std::string str;
	if(rFile.peek() == '\"') {
		str = rFile.get();
		char ch = rFile.peek();

		while(ch != '\"' && !rFile.eof()) {
			str += rFile.get();
			ch = rFile.peek();
		}
		str += rFile.get();
	}
	else {
		bool numerical = (rFile.peek() >= '0' && rFile.peek() <= '9');

		while(!BlockNextChar(rFile, numerical) && !rFile.eof()) {
			char ch = rFile.get();
			str += ch;
		}
	}

	if(str.length()) {
		Token *t = new Token(str.c_str());

		if(ReservedWord(str.c_str())) {
			t->mType = Token::RESERVED;
		}
		else if(str[0] == '\"') {
			t->mType = Token::VARIABLE_STRING;
		}
		else {
			Variable::Type vType = Variable::GetType(str.c_str());
			if(vType == Variable::INT) {
				t->mType = Token::VARIABLE_INT;
			}
			else if(vType == Variable::FLOAT) {
				t->mType = Token::VARIABLE_FLOAT;
			}
			else {
				if(IsValidName(str.c_str())) {
					t->mType = Token::VARIABLE_FUNCTION;
				}
				else {
					t->mType = Token::INVALID;
				}
			}
		}

		tokens.push_back(t);
		return true;
	}

	return false;
}

bool Tokens::PeekOperator(std::ifstream &rFile, char rContext) {
	char ch = rFile.peek();

	switch(ch) {
	case '+':
	case '-':
	case '&':
	case '|':
	{
		if(rContext == ch || !rContext) {
			return true;
		}
		return false;
	}

	case '*':
	case '/':
	case '<':
	case '>':
	{
		return rContext == 0;
	}

	case '=':
	{
		if(rContext == '&' || rContext == '|') {
			return false;
		}
		return true;
	}

	case '!':
	{
		if(rContext == 0) {
			return true;
		}
		return false;
	}
	}

	return false;
}

void Tokens::DetermineOperator(Token *rToken) {
	MCString str = rToken->mToken;

	switch(str[0]) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
		if(str.Length() == 1) {
			rToken->mType = Token::OPERATOR_ARIT;
		}
		else {
			if(str[1] == '=') {
				rToken->mType = Token::OPERATOR_ASSIGN;
			}
			else if(str[1] == str[0]) {
				if(str[0] == '+' || str[0] == '-') {
					rToken->mType = Token::OPERATOR_ASSIGN;
					throw NotImplementedException("Operator ++ and -- are not implemented yet");
				}
				else {
					throw InvalidTokenException("Token not supported: " + rToken->mToken);
				}
			}
		}
		return;

	case '=':
		if(str.Length() == 1) {
			rToken->mType = Token::OPERATOR_ASSIGN;
		}
		else if(str[0] == str[1]) {
			rToken->mType = Token::OPERATOR_COMP;
		}
		return;

	case '>':
	case '<':
		rToken->mType = Token::OPERATOR_COMP;
		return;
	}
}

bool Tokens::ReservedWord(MCString rStr) {
	if(rStr == "class" || rStr == "for" || rStr == "if" || rStr == "else" || rStr == "while" || rStr == "func" || rStr == "var" || rStr == "return" || rStr == "include") {
		return true;
	}
	return false;
}

bool Tokens::IsValidName(MCString rName) {
	if(!rName.Length()) {
		return false;
	}

	if(rName[0] >= '0' && rName[0] <= '9') {
		return false;
	}

	for(int i = 0; i < rName.Length(); i++) {
		char c = rName[i];

		if((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && (c < '0' || c > '9') && (c != '_')) {
			return false;
		}
	}

	return true;
}

bool Tokens::BlockNextChar(std::ifstream &rFile, bool rNumericalContext) {
	char ch = rFile.peek();

	switch(ch) {
	case '(':
	case ')':
	case '{':
	case '}':
	case ';':

	case ' ':
	case '\n':
	case '\t':
	case '#':

	case '-':
	case '+':
	case '*':
	case '/':
	case '=':
	case '!':

	case '<':
	case '>':
	case '\"':
	case ',':
		return true;

	case '.':
		return !rNumericalContext;
	}
	return false;
}

void Tokens::SeekNextToken(std::ifstream &rFile) {
	char ch = rFile.peek();
	while(ch == '#') {
		SkipLine(rFile);
		ch = rFile.peek();
	}

	while(ch == ' ' || ch == '\n' || ch == '\t') {
		rFile.get();
		ch = rFile.peek();

		while(ch == '#') {
			SkipLine(rFile);
			ch = rFile.peek();
		}
	}
}

void Tokens::SkipLine(std::ifstream &rFile) {
	MCString waste;
	std::getline(rFile, (std::string)waste);
}

bool Tokens::ReachedEnd(std::ifstream &rFile) {
	int next = rFile.peek();
	return next == std::char_traits<char>::eof();
}