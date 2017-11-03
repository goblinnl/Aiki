#ifndef TOKENS_H
#define TOKENS_H

// Internal
#include "../Mixer/MCommon.h"
#include "../Mixer/MString.h"

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
		PARANTH_BEG = 0x00000400,
		PARANTH_END = 0x00000800,
		RESERVED = 0x00001000,
		BRACKET_BEG = 0x00002000,
		BRACKET_END = 0x00004000,
		VALUE = 0x00040000,
		SEMICOLON = 0x00008000,
		DOT = 0x00010000,
		COMMA = 0x00020000,
	};

	static MString GetStringValue(Token::Type rType);

	Token() {
		mType = INVALID;
	}
	Token(MString rToken, Type rType = INVALID) {
		mToken = rToken;
		mType = rType;
	}

	MString	mToken;
	Type mType;
};
typedef std::list<Token*>::iterator TokenIterertor;

class Tokens {
private:
	std::list<Token*> tokens;
	TokenIterertor pointer;

public:
	Tokens();
	Token* PopIfExists(Token::Type rType);
	Token* PopExpected(Token::Type rType);
	Token* PopNext();
	Token* CheckNext();
	TokenIterertor GetFirstIterator();
	TokenIterertor GetPointer();

	void GenerateTokens(MString rFile);
	void SetPointer(TokenIterertor rFront);

	bool IsMore();

private:
	bool GetToken(std::ifstream &rFile);
	bool GetOperator(std::ifstream &rFile);
	bool GetSpecialChar(std::ifstream &rFile);
	bool GetWord(std::ifstream &rFile);
	bool PeekOperator(std::ifstream &rFile, char rContext = 0);
	bool ReservedWord(MString rWord);
	bool IsValidName(MString rName);
	bool BlockNextChar(std::ifstream &rFile, bool rNumerical);
	bool ReachedEnd(std::ifstream &rFile);

	void SeekNextToken(std::ifstream &rFile);
	void SkipLine(std::ifstream &rFile);
	void DetermineOperator(Token* rToken);
};
#endif // TOKENS_H