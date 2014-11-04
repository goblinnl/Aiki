#ifndef OPERATIONCODETEXT_H
#define OPERATIONCODETEXT_H

/* Operation Code Text
* Class for oncerting scrap bytecode to readable text
*/

// Internal
#include "Codes.hpp"

// External
#include <string>
#include <vector>
using namespace std;

class OperationCode;

class OperationCodeText {
public:
	void parse(OperationCode *aOperationCode);

private:
	vector<byte> operationCodes;

	int	parametersCount(byte aCode);
	string	getLieteral(byte aCode);
	string	getParameter(byte aCtx, int aOpcodeIdx, int aParamIdx);

	void* getDword(int aOpcodeIdx);
	string getUint(int aOpcodeIdx);
	string getInteger(int aOpcodeIdx);
	string getFloat(int aOpcodeIdx);
};

#endif // OPERATIONCODETEXT_H