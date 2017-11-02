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

	static std::string GetStringValue(Token::Type aType);
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
		void GenerateTokens(std::string aFile);
		void SetPointer(TokenIterertor aFront);	
		Token* PopIfExists(Token::Type aType);
		Token* PopExpected(Token::Type aType);
		Token* PopNext();
		Token* CheckNext();
		TokenIterertor GetFirstIterator();
		TokenIterertor GetPointer();
		bool IsMore();
	

	private:
		std::list<Token*> tokens;
		TokenIterertor pointer;
		bool GetToken(std::ifstream &aFile);
		bool GetOperator(std::ifstream &aFile);
		bool GetSpecialChar(std::ifstream &aFile);
		bool GetWord(std::ifstream &aFile);
		bool PeekOperator(std::ifstream &aFile, char aContext=0);	
		bool ReservedWord(std::string aWord);
		bool IsValidName(std::string aName);
		bool BlockNextChar(std::ifstream &aFile, bool aNumerical);
		bool ReachedEnd(std::ifstream &aFile);
		void SeekNextToken(std::ifstream &aFile);
		void SkipLine(std::ifstream &aFile);	
		void DetermineOperator(Token* aToken);
};

#endif // TOKENS_H