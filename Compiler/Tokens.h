#ifndef TOKENS_H
#define TOKENS_H

// External
#include <string>
#include <list>

struct Token {
	enum Type {
		INVALID = 0x80000000,
		UNDEFINED = 0x00000100,
		OPERATOR = 0x00000200,
		OPERATOR_ARIT = 0x00000201,
		OPERATOR_COMP = 0x00000202,
		OPERATOR_ASSIGN = 0x00000204,
		VARIABLE_STRING = 0x00080000,
		VARIABLE_INT = 0x00100000,
		VARIABLE_FLOAT = 0x00200000,
		VARIABLE_FUNCTION = 0x00400000,
		PARANTH_BEG	= 0x00000400,
		PARANTH_END	= 0x00000800,
		RESERVED = 0x00001000,
		BRACKET_BEG	= 0x00002000,
		BRACKET_END = 0x00004000,	
		VALUE = 0x00040000,
		SEMICOLON = 0x00008000,
		DOT	= 0x00010000,
		COMMA = 0x00020000,		
	};

	static std::string getStringValue(Token::Type aType);
	Token() { aType = INVALID; }
	Token(std::string aToken, Type t = INVALID) {
		token = aToken;
		aType = t;
	}

	std::string	token;
	Type aType;
};

typedef std::list<Token*>::iterator TokenIterertor;

class Tokens {
	public:
		Tokens();
		void generateTokens(std::string aFile);
		void setPointer(TokenIterertor aFront);	
		Token* popIfExists(Token::Type aType);
		Token* popExpected(Token::Type aType);
		Token* popNext();
		Token* checkNext();
		TokenIterertor getFirstIterator();
		TokenIterertor getPointer();
		bool isMore();
	

	private:
		std::list<Token*> tokens;
		TokenIterertor pointer;
		bool getToken(std::ifstream &aFile);
		bool getOperator(std::ifstream &aFile);
		bool getSpecialChar(std::ifstream &aFile);
		bool getWord(std::ifstream &aFile);
		bool peekOperator(std::ifstream &aFile, char aContext=0);	
		bool reservedWord(std::string aWord);
		bool isValidName(std::string aName);
		bool blockNextChar(std::ifstream &aFile, bool aNumerical);
		bool reachedEnd(std::ifstream &aFile);
		void seekNextToken(std::ifstream &aFile);
		void skipLine(std::ifstream &aFile);	
		void determineOperator(Token* aToken);
};

#endif // TOKENS_H