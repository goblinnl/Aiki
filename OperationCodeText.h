#ifndef OPERATIONCODETEXT_H
#define OPERATIONCODETEXT_H

/* Operation Code Text
* Class for concerting scrap byte code to readable text
*/

// Internal
#include "Mixer/MCommon.h"
#include "Mixer/MString.h"
#include "Codes.h"

// External
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

	MString GetUint(int rOpcodeIdx);
	MString GetInteger(int rOpcodeIdx);
	MString GetFloat(int rOpcodeIdx);
	MString	GetLieteral(byte rCode);
	MString	GetParameter(byte rCtx, int rOpcodeIdx, int rParamIdx);
};
#endif // OPERATIONCODETEXT_H