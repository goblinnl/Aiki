
// Internal
#include "Tokens.hpp"
#include "../Exception.hpp"
#include "../Parser/Variable.hpp"

// External
#include <fstream>

string Token::getStringValue(Token::Type aType) {
	switch (aType) {
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

void Tokens::generateTokens(string aFileName) {
	ifstream file;
	file.open(aFileName.c_str());
	if (!file.good()) {
		throw FileNotFoundException("File couldn't be found: " + (string)aFileName);		
	}

	while (getToken(file) && !reachedEnd(file));

	pointer = tokens.begin();
}

bool Tokens::isMore() {
	return tokens.size() > 0 
		&& pointer != tokens.end();
}


Token* Tokens::popIfExists(Token::Type aType) {
	Token *token = NULL;

	if ((*pointer)->aType & aType) {
		token = *pointer;
		pointer = tokens.erase(pointer);
	}

	return token;
}

Token* Tokens::popExpected(Token::Type aType) {
	Token *token = popIfExists(aType);

	if (token) { 
		return token;
	}

	// All hell is loose - RELEASE THE KRAKEN!
	token = popNext();
	throw InvalidTokenException("Expected '"+ Token::getStringValue(aType) + "', got: '" + (token ?Token::getStringValue(token->aType):"NULL") + "'.");
}

Token* Tokens::popNext() {
	Token *token = *pointer;
	pointer = tokens.erase(pointer);
	return token;
}

Token* Tokens::checkNext() {
	return *pointer;
}

TokenIterertor Tokens::getFirstIterator() {
	return tokens.begin(); 
}

TokenIterertor Tokens::getPointer() {
	return pointer;
}

void Tokens::setPointer(TokenIterertor aIterator) {
	pointer = aIterator;
}

bool Tokens::getToken(ifstream &aFile) {
	if (!aFile.good()) return false;

	Token token;
	seekNextToken(aFile);

	if (getSpecialChar(aFile)) {
		return true;
	}

	if (getOperator(aFile)) {
		return true;
	}

	if (getWord(aFile)) {
		return true;
	}

	return false;
}

bool Tokens::getOperator(ifstream &aFile) {
	string str;
	char lastChar = 0;

	while (peekOperator(aFile, lastChar) && str.length() <= 1) {
		lastChar = aFile.get();
		str += lastChar;
	}

	if (str.length()) {
		Token *t = new Token(str, Token::OPERATOR);
		tokens.push_back(t);

		determineOperator(t);

		return true;
	}
	return false;
}

bool Tokens::getSpecialChar(ifstream &aFile) {
	char ch = aFile.peek();
	string str;
	str = ch;

	Token *t = new Token(str);

	switch (ch) {
		case '(':
			t->aType = Token::PARANTH_BEG;
			break;

		case ')':
			t->aType = Token::PARANTH_END;
			break;

		case '{':
			t->aType = Token::BRACKET_BEG;
			break;

		case '}':
			t->aType = Token::BRACKET_END;
			break;

		case ';':
			t->aType = Token::SEMICOLON;
			break;

		case '.':
			t->aType = Token::DOT;
			break;

		case ',':
			t->aType = Token::COMMA;
			break;

		default:
			delete t;
			t = NULL;
			return false;
	}

	tokens.push_back(t);
	aFile.get();
	return true;
}

bool Tokens::getWord(ifstream &aFile) {
	string str;
	if (aFile.peek() == '\"') {
		str = aFile.get();
		char ch = aFile.peek();
		
		while (ch != '\"' && !aFile.eof()) {
			str += aFile.get();
			ch = aFile.peek();
		}
		str += aFile.get();
	} else {
		bool numerical = (aFile.peek() >= '0' && aFile.peek() <= '9');

		while (!blockNextChar(aFile, numerical) && !aFile.eof()) {
			char ch = aFile.get();
			str += ch;
		}
	}

	if (str.length()) {
		Token *t = new Token(str);

		if (reservedWord(str)) {
			t->aType = Token::RESERVED;
		} else if (str[0] == '\"') {
			t->aType = Token::VARIABLE_STRING;
		} else {
			Variable::Type vType = Variable::getType(str);
			if (vType == Variable::INT) {
				t->aType = Token::VARIABLE_INT;
			} else if (vType == Variable::FLOAT) {
				t->aType = Token::VARIABLE_FLOAT;
			} else {
				if (isValidName(str)) {
					t->aType = Token::VARIABLE_FUNCTION;
				} else {
					t->aType = Token::INVALID;
				}
			}
		}

		tokens.push_back(t);
		return true;
	}

	return false;
}

bool Tokens::peekOperator(ifstream &aFile, char aContext) {
	char ch = aFile.peek();
	
	switch (ch) {
	case '+':
	case '-':
	case '&':
	case '|':
		{
			if (aContext == ch || !aContext) {
				return true;
			}
			return false;
		}

	case '*':
	case '/':
	case '<':
	case '>':
		{
			return aContext == 0;
		}

	case '=':
		{
			if (aContext == '&' || aContext == '|') {
				return false;
			}
			return true;
		}

	case '!':
		{
			if (aContext == 0) {
				return true;
			}
			return false;
		}
	}

	return false;
}

void Tokens::determineOperator(Token *aToken) {
	string str = aToken->token;

	switch (str[0]) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
			if (str.length() == 1) {
				aToken->aType = Token::OPERATOR_ARIT;
			} else {
				if (str[1] == '=') {
					aToken->aType = Token::OPERATOR_ASSIGN;
				} else if (str[1] == str[0]) {
					if (str[0] == '+' || str[0] == '-') {
						aToken->aType = Token::OPERATOR_ASSIGN;
						throw NotImplementedException("Operator ++ and -- are not implemented yet");
					} else {
						throw InvalidTokenException("Token not supported: " + aToken->token);
					}
				}
			}
			return;

		case '=':
			if (str.length() == 1) {
				aToken->aType = Token::OPERATOR_ASSIGN;
			} else if (str[0] == str[1]) {
				aToken->aType = Token::OPERATOR_COMP;
			}
			return;

		case '>':
		case '<':
			aToken->aType = Token::OPERATOR_COMP;
			return;
	}
}

bool Tokens::reservedWord(string aStr) {
	if (aStr == "class" || aStr == "for" || aStr == "if" || aStr == "else" || aStr == "while" || aStr == "func" || aStr == "var" || aStr == "return" || aStr == "include"){
		return true;
	}
	return false;
}

bool Tokens::isValidName(string aName) {
	if (!aName.length()) { 
		return false;
	}

	if (aName[0] >= '0' && aName[0] <= '9') {
		return false;
	}

	for (int i = 0; i < aName.length(); i++) {
		char c = aName[i];

		if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && (c < '0' || c > '9') && (c != '_')) {
			return false;
		}
	}

	return true;
}

bool Tokens::blockNextChar(ifstream &aFile, bool aNumericalContext) {
	char ch = aFile.peek();

	switch (ch) {
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
			return !aNumericalContext;
	}
	return false;
}

void Tokens::seekNextToken(ifstream &aFile) {
	char ch = aFile.peek();
	while (ch == '#') {
		skipLine(aFile);
		ch = aFile.peek();
	}

	while (ch == ' ' || ch == '\n' || ch == '\t') {
		aFile.get();
		ch = aFile.peek();

		while (ch == '#') {
			skipLine(aFile);
			ch = aFile.peek();
		}
	}
}

void Tokens::skipLine(ifstream &aFile) {
	string waste;
	std::getline(aFile, waste);
}

bool Tokens::reachedEnd(ifstream &aFile) {
	int next = aFile.peek();
	return next == char_traits<char>::eof();
}