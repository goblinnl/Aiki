#ifndef DATA_ARGUMENT_H
#define DATA_ARGUMENT_H

// Internal
#include "../Mixer/MCString.h"
#include "../Parser/Variable.h"
#include "../Parser/Table.h"
#include "../Exception.h"

class DataArgument {
private:
	int mCount;
	Variable* mObject;
	ArgParser* mParser;
	Variable* mVars[AK_ARGUMENT_SIZE];
	Function* mFunction;

public:
	DataArgument();
	~DataArgument();

	void SetVarData(List<Variable*> *rVars);
	void AddVar(Variable* rVar);

	void Error(const MCString& rString);
	void Error(const MCString& rString, const int& rLineNumber, const char* rFile);

	int GetType(int rIndex);
	bool CheckType(int rIndex, int rType);
	bool CheckType(int rIndex, int rType, bool rThrowError);

	bool CheckObject(int rType);
	bool CheckObject(int rType, bool rThrowError);

	bool GetError();
	void Destroy();
	void SetObject(Variable* rVar);

	const char* GetStringByIndex(int rIndex) const;
	double GetNumberByIndex(int rIndex) const;
	bool GetBoolByIndex(int rIndex) const;
	Table* GetTableByIndex(int rIndex) const;
	Function* GetFunctionByIndex(int rIndex) const;
	Function* GetFunction() const;
	Variable* GetVariableByIndex(int rIndex) const;
	void* GetDataByIndex(int rIndex, int rOperator) const;
	void* GetObjectData() const;

	Variable* GetObject() const;
	ArgParser* GetParser() const;
	
	void SetCount(int rCount);
	void SetFunction(Function* rFunction);
	void SetParser(ArgParser* rParser);
	int GetCount() const;
};
#endif // DATA_ARGUMENT_H