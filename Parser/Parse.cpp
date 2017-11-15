#include "Parse.h"

Parse::Parse()
{

}

Parse::~Parse()
{

}

Variable* Parse::ToVar(ArgParser* rParser, bool rValue)
{
	Variable* var = rParser->GetGarbageCollector().GetPooledVariable();
	var->SetType(Variable::BOOL);
	var->SetData(BoolToPtr(rValue));

	return var;
}

Variable* Parse::ToVar(ArgParser* rParser, double rValue)
{
	Variable* var = rParser->GetGarbageCollector().GetPooledVariable();
	var->SetType(Variable::NUMBER);
	var->SetData(NumberToPtr(rParser, rValue));

	return var;
}

Variable* Parse::ToVar(ArgParser* rParser, int rValue)
{
	Variable* var = rParser->GetGarbageCollector().GetPooledVariable();
	var->SetType(Variable::NUMBER);
	var->SetData(NumberToPtr(rParser, rValue));

	return var;
}

Variable* Parse::ToVar(ArgParser* rParser, long long rValue)
{
	Variable* var = rParser->GetGarbageCollector().GetPooledVariable();
	var->SetType(Variable::NUMBER);
	var->SetData(NumberToPtr(rParser, rValue));

	return var;
}

Variable* Parse::ToVar(ArgParser* rParser, float rValue)
{
	Variable* var = rParser->GetGarbageCollector().GetPooledVariable();
	var->SetType(Variable::NUMBER);
	var->SetData(NumberToPtr(rParser, rValue));

	return var;
}

Variable* Parse::ToVar(ArgParser* rParser, const MCString &rValue)
{
	Variable* var = rParser->GetGarbageCollector().GetPooledVariable();
	var->SetType(Variable::STRING);
	var->SetData(StringToPtr(rValue));

	return var;
}

Variable* Parse::ToVar(ArgParser* rParser, const char* rValue)
{
	Variable* var = rParser->GetGarbageCollector().GetPooledVariable();
	var->SetType(Variable::STRING);
	var->SetData(StringToPtr(rValue));

	return var;
}

Variable* Parse::ToVar(ArgParser* rParser, Table* rValue)
{
	Variable* var = rParser->GetGarbageCollector().GetPooledVariable();
	var->SetType(Variable::TABLE);
	var->SetData(rValue);

	return var;
}

Variable* Parse::ToVar(ArgParser* rParser, const MCString &rKey, AKFunction &rValue)
{
	Function* func = new Function(rParser);
	func->mCppFunc = rValue;
	func->mFilename = "C++";
	func->mIsCpp = true;
	func->mName = rKey;

	Variable* var = rParser->GetGarbageCollector().GetPooledVariable();
	var->SetType(Variable::FUNCTION);
	var->SetData(func);

	return var;
}

Variable* Parse::ToVar(ArgParser* rParser, const char* rKey, AKFunction rValue)
{
	Function* func = new Function(rParser);
	func->mCppFunc = rValue;
	func->mFilename = "C++";
	func->mIsCpp = true;
	func->mName = rKey;

	Variable* var = rParser->GetGarbageCollector().GetPooledVariable();
	var->SetType(Variable::FUNCTION);
	var->SetData(func);

	return var;
}

bool Parse::SetMember(ArgParser* rParser, Variable* rMember, Variable* rValue, const MCString &rKey)
{
	DataInstance* data = (DataInstance*)rMember->GetData();

	if(data->GetData()->mSet != nullptr) {
		data->GetData()->mSet(rParser, data->GetPointer(), rValue);
		return true;
	}

	if(data->GetData()->GetTypeID() == Variable::BOOL && rValue->GetType() == Variable::BOOL) {	*(bool*)data->GetPointer() = rValue->GetData() != nullptr; }
	if(data->GetData()->GetTypeID() == Variable::BYTE && rValue->GetType() == Variable::NUMBER) { *(byte*)data->GetPointer() = (byte)*(double*)rValue->GetData(); }
	if(data->GetData()->GetTypeID() == Variable::INT && rValue->GetType() == Variable::NUMBER) { *(int*)data->GetPointer() = (int)*(double*)rValue->GetData(); }
	if(data->GetData()->GetTypeID() == Variable::SHORT && rValue->GetType() == Variable::NUMBER) { *(short*)data->GetPointer() = (short)*(double*)rValue->GetData(); }
	if(data->GetData()->GetTypeID() == Variable::FLOAT && rValue->GetType() == Variable::NUMBER) { *(float*)data->GetPointer() = (float)*(double*)rValue->GetData(); }
	if(data->GetData()->GetTypeID() == Variable::DOUBLE && rValue->GetType() == Variable::NUMBER) { *(double*)data->GetPointer() = *(double*)rValue->GetData(); }
	if(data->GetData()->GetTypeID() == Variable::LONG && rValue->GetType() == Variable::NUMBER) { *(long long*)data->GetPointer() = (long long)*(double*)rValue->GetData(); }
	if(rMember->GetType() == rValue->GetType() && rValue->GetType() == Variable::DATA) { memcpy(data->GetPointer(), ((DataInstance*)rValue->GetData())->GetPointer(), data->GetData()->GetTypeSize()); }
	
	if(data->GetPointer() != nullptr) {
		return true;
	}

	return false;
}

Variable* Parse::MemberToVar(ArgParser* rParser, Variable* rMember)
{
	DataInstance* data = (DataInstance*)rMember->GetData();
	if(data->GetData()->mGet != nullptr) {
		Variable* var = data->GetData()->mGet(rParser, data->GetPointer());

		if(var == nullptr) {
			var = rParser->GetUndefinedVar();
		}
		else {
			rParser->GetGarbageCollector().RegisterVariable(var);
		}

		return var;
	}

	if(rMember->GetType() > Variable::BOOL) {
		return rMember;
	}

	Variable* var = rParser->GetGarbageCollector().GetPooledVariable();
	var->SetIsCpp(rMember->GetIsCpp());
	var->SetData(rMember->GetData());
	var->SetType(rMember->GetType());
	var->SetDestroyOnDestructor(false);

	switch(data->GetData()->GetTypeID()) {
		case Variable::BOOL:
			var->SetData(Parse::BoolToPtr(*(bool*)data->GetPointer()));
			var->SetType(Variable::BOOL);
			break;		
		case Variable::DOUBLE:
			var->SetData(Parse::NumberToPtr(rParser, *(double*)data->GetPointer()));
			var->SetType(Variable::NUMBER);
			break;
		case Variable::BYTE:
			var->SetData(Parse::NumberToPtr(rParser, (double)*(byte*)data->GetPointer()));
			var->SetType(Variable::NUMBER);
			break;
		case Variable::INT:
			var->SetData(Parse::NumberToPtr(rParser, (double)*(int*)data->GetPointer()));
			var->SetType(Variable::NUMBER);
			break;
		case Variable::SHORT:
			var->SetData(Parse::NumberToPtr(rParser, (double)*(short*)data->GetPointer()));
			var->SetType(Variable::NUMBER);
			break;
		case Variable::FLOAT:
			var->SetData(Parse::NumberToPtr(rParser, (double)*(float*)data->GetPointer()));
			var->SetType(Variable::NUMBER);
			break;
		case Variable::LONG:
			var->SetData(Parse::NumberToPtr(rParser, (double)*(long long*)data->GetPointer()));
			var->SetType(Variable::NUMBER);
			break;
	}

	return var;
}

void* Parse::NumberToPtr(ArgParser* rParser, double rVal)
{
	double* pdouble = rParser->GetGarbageCollector().mPoolNumber.GetNext();
	if(pdouble != nullptr) {
		*pdouble = rVal;
		return pdouble;
	}
	return new double(rVal);
}

void* Parse::NumberToPtr(double rVal)
{
	return new double(rVal);
}

void* Parse::BoolToPtr(bool rVal)
{
	return (void*)(rVal ? 1 : 0);
}

void* Parse::StringToPtr(const MCString &rVal)
{
	return new MCString(rVal);
}

MCString Parse::ObjectToString(Variable* rVal)
{
	if(rVal == nullptr) {
		return "Undefined";
	}
	
	if(rVal->GetType() == Variable::BOOL) { return "Bool"; }
	if(rVal->GetType() == Variable::CLASS) { return "Class";  }
	if(rVal->GetType() == Variable::FUNCTION) { return "Function";  }
	if(rVal->GetType() == Variable::UNDEFINED) { return "Undefined"; }
	if(rVal->GetType() == Variable::NUMBER) { return "Number";  }
	if(rVal->GetType() == Variable::STRING) { return "String"; }
	if(rVal->GetType() == Variable::TABLE) { 
		
		if(rVal->GetTable()->Get("__TYPE")->GetType() != Variable::UNDEFINED) {
			return rVal->GetTable()->Get("__TYPE")->GetString();
		}

		return "Table";
	}

	return ((DataInstance*)rVal->GetData())->GetData()->GetName();
}

MCString Parse::ObjectToString(int rType)
{
	if(rType == Variable::BOOL) { return "Bool"; }
	if(rType == Variable::NUMBER) { return "Number"; }
	if(rType == Variable::STRING) { return "String"; }
	if(rType == Variable::UNDEFINED) { return "Undefined"; }
	if(rType == Variable::FUNCTION) { return "Function"; }
	if(rType == Variable::TABLE) { return "Table"; }
	if(rType == Variable::CLASS) { return "Class"; }

	return "Data";
}

MCString Parse::ObjectToString(ArgParser* rParser, int rType)
{
	if(rType == Variable::BOOL) { return "Bool"; }
	if(rType == Variable::NUMBER) { return "Number"; }
	if(rType == Variable::STRING) { return "String"; }
	if(rType == Variable::UNDEFINED) { return "Undefined"; }
	if(rType == Variable::FUNCTION) { return "Function"; }
	if(rType == Variable::TABLE) { return "Table"; }
	if(rType == Variable::CLASS) { return "Class"; }

	for(int i = 0; i < rParser->GetRegisteredDataTypes().GetCount(); i++) {
		if(rParser->GetRegisteredDataTypes()[i]->GetTypeID == rType) {
			return rParser->GetRegisteredDataTypes[i]->GetName();
		}
	}

	return "Data";
}

MCString Parse::OperatorToString(int rOperator)
{
	if(rOperator == AO_ADD) { return "Add"; }
	if(rOperator == AO_AND) { return "And"; }
	if(rOperator == AO_DIVIDE) { return "Divide"; }
	if(rOperator == AO_EQUAL) { return "Equal"; }
	if(rOperator == AO_GREATER_OR_EQUAL) { return "GreaterOrEqual"; }
	if(rOperator == AO_GREATER_THAN) { return "GreaterThan"; }
	if(rOperator == AO_LESS_OR_EQUAL) { return "LessOrEqual"; }
	if(rOperator == AO_LESS_THAN) { return "LessThan"; }
	if(rOperator == AO_MODULO) { return "Modulo"; }
	if(rOperator == AO_MULTIPLY) { return "Multiply"; }
	if(rOperator == AO_NOT_EQUAL) { return "NotEqual"; }
	if(rOperator == AO_OR) { return "Or"; }
	if(rOperator == AO_SUBTRACT) { return "Subtract"; }
	if(rOperator == AO_BITWISE_AND) { return "BitwiseAnd"; }
	if(rOperator == AO_BITWISE_OR) { return "BitwiseOr"; }
	if(rOperator == AO_BITWISE_LEFT) { return "BitwiseLeft"; }
	if(rOperator == AO_BITWISE_RIGHT) { return "BitwiseRight"; }

	return "Unknown";
}

MCString Parse::VarToString(Variable* rVar)
{

	if(rVar->GetType() == Variable::STRING) { return rVar->GetString(); }
	if(rVar->GetType() == Variable::NUMBER) { return MCString::Format("%d", (int)rVar->GetNumber()); }
	if(rVar->GetType() == Variable::FUNCTION) { return "Function"; }
	if(rVar->GetType() == Variable::BOOL) { return rVar->GetBool() ? "True" : "False"; }
	if(rVar->GetType() == Variable::UNDEFINED) { return "Undefined"; }
	if(rVar->GetType() == Variable::TABLE) {
		if(rVar->GetTable()->Get("__TYPE")->GetType() != Variable::UNDEFINED){
			return MCString::Format("Class{%s}", rVar->GetTable()->Get("__TYPE")->GetString().mBuffer);
		}

		return MCString::Format("{table count=%d", rVar->GetTable()->mCount);
	}

	return MCString::Format("Class{%s", ((DataInstance*)rVar->GetData())->GetData()->GetName().mBuffer);
}

MCString Parse::VarToString(ArgParser* rParser, Variable* rVar)
{
	if(rVar->GetType() == Variable::STRING) { return rVar->GetString(); }
	if(rVar->GetType() == Variable::NUMBER) { return MCString::Format("%d", (int)rVar->GetNumber()); }
	if(rVar->GetType() == Variable::FUNCTION) { return "function"; }
	if(rVar->GetType() == Variable::BOOL) { return rVar->GetBool() ? "true" : "false"; }
	if(rVar->GetType() == Variable::UNDEFINED) { return "undefined"; }
	if(rVar->GetType() == Variable::TABLE) { 
		if(rVar->GetTable()->Get("__TYPE")->GetType() != Variable::UNDEFINED) {
			return MCString::Format("class{%s", rVar->GetTable()->Get("__TYPE")->GetString().mBuffer);
		}

		return MCString::Format("{table count=%d", rVar->GetTable()->mCount);	
	}

	DataInstance* data = (DataInstance*)rVar->GetData();
	AKFunction func = data->GetOperator(AO_TO_STRING);
	if(func != nullptr) {
		DataArgument args;
		args.SetObject(rVar);
		AK_REFERENCE_INCREASE(rVar);
		Variable* temp = func(rParser, &args);
		AK_REFERENCE_DECREASE(rVar);

		if(temp == nullptr) {
			return "UNDEFINED";
		}
		else {
			rParser->GetGarbageCollector().RegisterVariable(temp);
		}

		return temp->GetString();
	}

	return MCString::Format("Class{%s}", ((DataInstance*)rVar->GetData)->GetData()->GetName().mBuffer);
}