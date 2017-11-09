#include "Data.h"

Data::Data(ArgParser* rParser) : mParser(rParser), mDestroy(false), mName(""), mOffset(0),
mTypeID(0), mTypeSize(0), mGet(nullptr), mSet(nullptr), mInheritFrom(nullptr), mStats(new Table(rParser))
{
	memset(mOverrideOperator, 0, sizeof(mOverrideOperator));
}

Data::~Data()
{
	while(mMembers.GetCount() > 0) {
		delete mMembers.RemoveAt(0);
	}

	if(mStats != nullptr) {
		delete mStats;
	}
}

Data* Data::Copy()
{
	Data* data = new Data(mParser);

	data->mName = mName;
	data->mOffset = mOffset;
	data->mTypeID = mTypeID;
	data->mTypeSize = mTypeSize;
	data->mGet = mGet;
	data->mSet = mSet;
	data->mConstructor = mConstructor;
	data->mDestructor = mDestructor;

	for(int i = 0; i < mMembers.GetCount(); i++) {
		data->mMembers.Add(mMembers[i]->Copy());
	}

	for(int i = 0; i < mFunctions.Count(); i++) {
		data->mFunctions.Add(mFunctions.GetKeyByIndex(i), mFunctions.GetValueByIndex(i));
	}

	for(int i = 0; i < sizeof(data->mOverrideOperator) / sizeof(AKFunction); i++) {
		data->mOverrideOperator[i] = mOverrideOperator[i];
	}

	return data;
}

void Data::RegisterMember(MCString rName, int rOffset, int rType)
{
	Data* data = nullptr;
	for(int i = 0; i < mParser->GetRegisteredDataTypes().GetCount(); i++) {
		if(mParser->GetRegisteredDataTypes()[i]->mTypeID == rType) {
			data = mParser->GetRegisteredDataTypes()[i]->Copy();
			break;
		}
	}

	if(data == NULL) {
		if(rType > Variable::Type::BOOL) {
			throw ObjectTypeException("Unknown Object Type");
		}
		data = new Data(mParser);
	}
	data->mParser = mParser;
	data->mOffset = mOffset;
	data->mTypeID = mTypeID;
	data->mName = rName;

	mMembers.Add(data);
}

void Data::RegisterMember(MCString rName, int rOffset, int rType, AKSetFunction rSet, AKGetFunction rGet)
{
	Data* data = NULL;
	for(int i = 0; i < mParser->GetRegisteredDataTypes().GetCount(); i++) {
		if(mParser->GetRegisteredDataTypes()[i]->mTypeID == rType) {
			data = mParser->GetRegisteredDataTypes()[i]->Copy();
			break;
		}
	}

	if(data == NULL) {
		if(rType > Variable::Type::BOOL) {
			throw ObjectTypeException("Unknown Object Type");
		}
		data = new Data(mParser);
	}

	data->mParser = mParser;
	data->mOffset = mOffset;
	data->mTypeID = mTypeID;
	data->mName = mName;
	data->mGet = mGet;
	data->mSet = mSet;

	mMembers.Add(data);
}

void Data::RegisterFunction(MCString rName, AKFunction rFunction)
{
	if(mFunctions.HasKey(rName)) {
		throw FuncAlreadyDefinedException("Function Already Defined!");
	}
	mFunctions.Add(rName, rFunction);
}

void Data::RegisterOperator(int rCode, AKFunction rFunction)
{
	mOverrideOperator[AK_FUNCTION_TO_INDEX(rCode)] = rFunction;
}

void Data::RegisterStaticFunction(MCString rName, AKFunction rFunction, MCString rFile /*= __FILE__*/, int rLineNumber /*= __LINE__*/)
{
	Function* temp = new Function(mParser);
	temp->mCppFunc = rFunction;
	temp->mFilename = rFile;
	temp->mIsCpp = true;
	temp->mName = rName;
	temp->mSouceFileLine = rLineNumber;

	Variable* var = mParser->mGarbageCollector.GetPooledVariable();
	var->SetType(Variable::FUNCTION);
}

void Data::RegisterStaticMember(MCString rName, Variable* rVar)
{
	mStats->Set(rName, rVar);
}

ArgParser* Data::GetParser() const
{
	return mParser;
}

Data* Data::GetInheritFrom() const
{
	return mInheritFrom;
}

MCDictionary<MCString, AKFunction> Data::GetFunctions() const
{
	return mFunctions;
}
