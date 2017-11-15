#ifndef PARSE_H
#define PARSE_H

#include "Parse.h"

#include "Variable.h"
#include "../Global.h"
#include "../Codes.h"
#include "../Data/DataInstance.h"
#include "../Singleton.h"

class Parse;
typedef Singleton<Parse> gParse;

class Parse {
public:
	Parse();
	~Parse();

	Variable* ToVar(ArgParser* rParser, bool rValue);
	Variable* ToVar(ArgParser* rParser, double rValue);
	Variable* ToVar(ArgParser* rParser, int rValue);
	Variable* ToVar(ArgParser* rParser, long long rValue);
	Variable* ToVar(ArgParser* rParser, float rValue);
	Variable* ToVar(ArgParser* rParser, const MCString &rValue);
	Variable* ToVar(ArgParser* rParser, const char* rValue);
	Variable* ToVar(ArgParser* rParser, Table* rValue);
	Variable* ToVar(ArgParser* rParser, const MCString &rKey, AKFunction &rValue);
	Variable* ToVar(ArgParser* rParser, const char* rKey, AKFunction rValue);

	bool SetMember(ArgParser* rParser, Variable* rMember, Variable* rValue, const MCString &rKey);
	Variable* MemberToVar(ArgParser* rParser, Variable* rMember);

	void* NumberToPtr(ArgParser* rParser, double rVal);
	void* NumberToPtr(double rVal);
	void* BoolToPtr(bool rVal);
	void* StringToPtr(const MCString &rVal);

	MCString ObjectToString(Variable* rVal);
	MCString ObjectToString(int rType);
	MCString ObjectToString(ArgParser* rParser, int rType);
	MCString OperatorToString(int rOperator);

	MCString VarToString(Variable* rVar);
	MCString VarToString(ArgParser* rParser, Variable* rVar);
};
#endif // PARSE_H