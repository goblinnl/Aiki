#include "DataArgument.h"

DataArgument::DataArgument() : mFunction(NULL), mParser(NULL), mCount(0), mObject(NULL)
{

}

DataArgument::~DataArgument()
{
	Destroy();
}

void DataArgument::SetVarData(List<Variable*> *rVars)
{
	Destroy();

	for(int i = 0; i < rVars->GetCount() i++) {
		if(GetCount() + 1 == AK_ARGUMENT_SIZE) {
			throw TooManyArgumentException("Too many arguments within the call");
			return;
		}
		Variable* var = rVars->Get(i);
		AK_REFERENCE_INCREASE(var);
		mVars[mCount++] = var;
	}
}

void DataArgument::AddVar(Variable* rVar)
{
	if(GetCount() + 1 == AK_ARGUMENT_SIZE) {
		throw TooManyArgumentException("Too many arguments within the call");
		return;
	}

	AK_REFERENCE_INCREASE(rVar);
	mVars[mCount++] = rVar;

}

void DataArgument::Error(const MCString& rString)
{
	
}

void DataArgument::Error(const MCString& rString, const int& rLineNumber, const char* rFile)
{

}

int DataArgument::GetType(int rIndex)
{
	if(rIndex >= GetCount()) {
		return Variable::UNDEFINED;
	}
	return mVars[rIndex]->GetType();
}

bool DataArgument::CheckType(int rIndex, int rType)
{
	if(rIndex >= GetCount()) {
		return false;
	}

	return mVars[rIndex]->GetType() == rType;
}

bool DataArgument::CheckType(int rIndex, int rType, bool rThrowError)
{
	if(rIndex >= GetCount()) {
		if(rThrowError) {
			throw ArgumentExpectedException("Missing argument, expected: "+ Parse::ObjectToString(mParser, rType).mBuffer +" an argument at index: " + rIndex);
		}
	}

}

bool DataArgument::CheckObject(int rType)
{

}

bool DataArgument::CheckObject(int rType, bool rThrowError)
{

}

bool DataArgument::GetError()
{

}

void DataArgument::Destroy()
{

}

void DataArgument::SetObject(Variable* rVar)
{
	mObject = rVar;
}

const char* DataArgument::GetStringByIndex(int rIndex) const
{

}

double DataArgument::GetNumberByIndex(int rIndex) const
{

}

bool DataArgument::GetBoolByIndex(int rIndex) const
{

}

Table* DataArgument::GetTableByIndex(int rIndex) const
{

}

Function* DataArgument::GetFunctionByIndex(int rIndex) const
{

}

Function* DataArgument::GetFunction() const {

}

Variable* DataArgument::GetVariableByIndex(int rIndex) const
{

}

void* DataArgument::GetDataByIndex(int rIndex, int rOperator) const
{

}

void* DataArgument::GetObjectData() const
{

}

Variable* DataArgument::GetObject() const
{

}

ArgParser* DataArgument::GetParser() const
{

}

void DataArgument::SetCount(int rCount)
{

}

int DataArgument::GetCount() const
{

}

void DataArgument::SetFunction(Function* rFunction)
{
	mFunction = rFunction;
}

void DataArgument::SetParser(ArgParser* rParser)
{
	mParser = rParser;
}
