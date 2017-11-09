#ifndef DATA_H
#define DATA_H

// Internal
#include "../Exception.h"
#include "../Parser/Table.h"
#include "../Parser/List.h"
#include "../Mixer/MCString.h"
#include "../Mixer/MCDictionary.h"

class Data {
private:
	int mOffset;
	int mTypeSize;
	int mTypeID;
	bool mDestroy;

	MCString mName;
	ArgParser* mParser;
	List<Data*> mMembers;
	Table* mStats;
	Data* mInheritFrom;
	MCDictionary<MCString, AKFunction> mFunctions;

public:
	AKFunction mOverrideOperator[(int)AO_END - (int)AO_START - 1];

	AKSetFunction mSet;
	AKGetFunction mGet;

	AKFunctionConstructor mConstructor;
	AKFunctionDeconstructor mDestructor;

public:
	Data(ArgParser* rParser);
	~Data();

	Data* Copy();
	Data* GetInheritFrom() const;
	ArgParser* GetParser() const;
	MCDictionary<MCString, AKFunction> GetFunctions() const;

	void RegisterMember(MCString rName, int rOffset, int rType);
	void RegisterMember(MCString rName, int rOffset, int rType, AKSetFunction rSet, AKGetFunction rGet);
	void RegisterFunction(MCString rName, AKFunction rFunction);
	void RegisterOperator(int rCode, AKFunction rFunction);

	void RegisterStaticFunction(MCString rName, AKFunction rFunction, MCString rFile = __FILE__, int rLineNumber = __LINE__);
	void RegisterStaticMember(MCString rName, Variable* rVar);
};

#endif // DATA_H