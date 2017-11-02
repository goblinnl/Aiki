#ifndef OPERATIONCODETEXT_H
#define OPERATIONCODETEXT_H

/* Operation Code Text
* Class for concerting scrap byte code to readable text
*/

// Internal
#include "Codes.h"

// External
#include <string>
#include <vector>

class OperationCode;

class OperationCodeText {
private:
	std::vector<byte> mOperationCodes;

public:
	void Parse(OperationCode *rOperationCode);

private:
	void* GetDword(int rOpcodeIdx);

	int	ParametersCount(byte rCode);

	std::string GetUint(int rOpcodeIdx);
	std::string GetInteger(int rOpcodeIdx);
	std::string GetFloat(int rOpcodeIdx);
	std::string	GetLieteral(byte rCode);
	std::string	GetParameter(byte rCtx, int rOpcodeIdx, int rParamIdx);
};
#endif // OPERATIONCODETEXT_H