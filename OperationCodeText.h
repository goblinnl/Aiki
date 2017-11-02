#ifndef OPERATIONCODETEXT_H
#define OPERATIONCODETEXT_H

/* Operation Code Text
* Class for oncerting scrap bytecode to readable text
*/

// Internal
#include "Codes.h"

// External
#include <string>
#include <vector>
using namespace std;

class OperationCode;

class OperationCodeText {
public:
	void Parse(OperationCode *aOperationCode);

private:
	vector<byte> operationCodes;

	int	ParametersCount(byte aCode);
	string	GetLieteral(byte aCode);
	string	GetParameter(byte aCtx, int aOpcodeIdx, int aParamIdx);

	void* GetDword(int aOpcodeIdx);
	string GetUint(int aOpcodeIdx);
	string GetInteger(int aOpcodeIdx);
	string GetFloat(int aOpcodeIdx);
};

#endif // OPERATIONCODETEXT_H