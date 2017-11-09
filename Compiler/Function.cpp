#include "Function.h"
#include "Expression.h"
#include "IntermediateOper.h"
#include "ArgParser.h"

#include "../stack.h"
#include "../Exception.h"

#include <sstream>


FunctionCall::FunctionCall(Token *rFuncToken) {
	mFunctionToken = rFuncToken;
}

void FunctionCall::ParseFragment(Tokens * rTokens, ArgParser * rParser) {
	delete rTokens->PopExpected(Token::PARANTH_BEG);

	if(rTokens->CheckNext()->mType == Token::PARANTH_END) {
		delete rTokens->PopNext();
	} else {
		while(rTokens->CheckNext()->mType != Token::PARANTH_END) {
			Expression *expr = new Expression(true);
			expr->ParseFragment(rTokens, rParser);
			mParameters.push_back(expr);

			if(Token *t = rTokens->PopIfExists(Token::COMMA)) {
				delete t;
			}
		}

		delete rTokens->PopExpected(Token::PARANTH_END);
	}

	FunctionSignature funcSign = rParser->GetFunctionSignature(mFunctionToken->mToken);

	if (funcSign.GetParameterCount() != mParameters.size()) {
		std::stringstream ss;
		ss << "Function " << funcSign.GetName() << ": "
			<< "Expected " << funcSign.GetParameterCount() << " parameters, "
			<< "received " << mParameters.size();
		throw InvalidArgumentException((MCString)ss.str().c_str());
	}
}

MCString FunctionCall::GetString() {
	MCString ret = mFunctionToken->mToken;

	ret += "(";

	std::list<Expression*>::iterator it;
	int i = 0;
	for (it = mParameters.begin(); it != mParameters.end(); it++) {
		if (i++) ret += ", ";
		ret += (*it)->GettingString();
	}

	ret += ") ";

	return ret;
}


void FunctionCall::ProvideIntermediates(OperationCode *rOpcode, ArgParser *rParser) {
	HandleParameters(rOpcode, rParser);

	uint funcID = rParser->GetFunctionID(mFunctionToken->mToken);

	rOpcode->AddInterop(new ByteOperation(OP_CALL));
	rOpcode->AddInterop(new DwordOperation(&funcID));
}

void FunctionCall::HandleParameters(OperationCode *rOpcode, ArgParser *rParser) {
	std::list<Expression*>::reverse_iterator it;

	for (it=mParameters.rbegin(); it != mParameters.rend(); it++) {
		(*it)->ProvideIntermediates(rOpcode, rParser);
	}
}

/***** FunctionDefinition *****/
bool FunctionDefinition::IsFunctionDefinition(Tokens *rTokens) {
	return rTokens->CheckNext()->mToken == "func";
}

FunctionDefinition::FunctionDefinition() {
	mFunctionID = 0;
}

PositionReference* FunctionDefinition::GetPositionReference() {
	return mPositionReference;
}

void FunctionDefinition::ParseFragment(Tokens *rTokens, ArgParser *rParser) {
	// Ensure that this really is a function
	Token *token = rTokens->PopExpected(Token::RESERVED);
	if(token->mToken != "func") {
		throw InvalidTokenException("Expected 'function', got: " + token->mToken);
	}
	delete token;

	// Get the function name
	Token *funcName = rTokens->PopExpected(Token::VARIABLE_FUNCTION);

	// Delete the opening parentheses
	delete rTokens->PopExpected(Token::PARANTH_BEG);

	// Get the parameters
	while(rTokens->CheckNext()->mToken == "var") {
		delete rTokens->PopExpected(Token::RESERVED);

		Token *param = rTokens->PopExpected(Token::VARIABLE_FUNCTION);
		mParameter.push_back(param);

		Token *comma = rTokens->PopIfExists(Token::COMMA);
		if (comma) {
			delete comma;
		}
	}

	// Register the function
	FunctionSignature sig(funcName->mToken, mParameter.size());
	mFunctionID = rParser->RegisterFunction(sig);

	delete funcName;
	delete rTokens->PopExpected(Token::PARANTH_END);

	if(rTokens->CheckNext()->mType != Token::BRACKET_BEG) {
		throw InvalidTokenException("Expected {");
	}

	mPositionReference = new PositionReference();
}

void FunctionDefinition::ProvideIntermediates(OperationCode *rOpcode, ArgParser *rParser) {
	rParser->PushScope();
	rOpcode->AddInterop(mPositionReference);

	for (TokenIterertor it = mParameter.begin(); it != mParameter.end(); it++) {
		uint varID = rParser->RegisterVariable((*it)->mToken);

	
		AllocateVariable(rOpcode, varID);
		rOpcode->AddInterop(new ByteOperation(OP_POPMOV));
		rOpcode->AddInterop(new DwordOperation(&varID));
	}
}

uint FunctionDefinition::GetID() {
	return mFunctionID;
}

FunctionTail::FunctionTail() {
	mPosRef = new PositionReference();
}

void FunctionTail::ParseFragment(Tokens *rTokens, ArgParser *rParser) {

}

void FunctionTail::ProvideIntermediates(OperationCode *rOpcode, ArgParser *rParser) {
	rParser->PopScope();
	uint zero = 0;

	rOpcode->AddInterop(new ByteOperation(OP_RET));
	rOpcode->AddInterop(new DwordOperation(&zero));
	rOpcode->AddInterop(mPosRef);
}

PositionReference* FunctionTail::GetPositionReference() {
	return mPosRef;
}

/***** FunctionSignature *****/
FunctionSignature::FunctionSignature(MCString rName, int rParamCount) {
	mSignatureName = rName;
	mParameterCount = rParamCount;
	mFunctionID = 0;
}

MCString FunctionSignature::GetName() {
	return mSignatureName;
}

int FunctionSignature::GetParameterCount() {
	return mParameterCount;
}

uint FunctionSignature::GetID() {
	return mFunctionID;
}

void FunctionSignature::SetID(uint rID) {
	mFunctionID = rID;
}

Function::Function(ArgParser* rParser) : mEnv(nullptr), mStringTableCount(0), mStringTableVars(NULL), 
mFixedLocalTypes(NULL), mFixedLocalRef(NULL), mReferenceFunc(NULL), mSouceFileLine(-1), mCodeOffset(0),
mFixedLocalCount(0), mCodeSize(0), mCode(NULL), mForceReturn(false), mFixedLocalVars(NULL), mFixedLocalKeys(NULL),
mIsCpp(false), mParent(NULL), mParser(rParser), mCppFunc(NULL), mCurrentObject(NULL), mStringTable(NULL)
{

}

Function::~Function()
{
	SetEnv(nullptr);

	if(mReferenceFunc != NULL) {
		for(int i = 0; i < mReferenceFunc->mRefChilds.mCount; i++) {
			if(mReferenceFunc->mRefChilds[i] == this) {
				mReferenceFunc->mRefChilds.RemoveAt(i);
				break;
			}
		}

		mCode = NULL;
		mStringTable = NULL;
		mStringTableVars = NULL;
	}
	else {
		Function* reference = NULL;
		for(int i = 0; i < mRefChilds.mCount; i++) {
			Function* child = mRefChilds[i];

			if(reference == NULL) {
				reference = child;

				reference->mReferenceFunc = NULL;
				mCode = NULL;
				mStringTable = NULL;
				mStringTableVars = NULL;
			}
			else{
				child->mReferenceFunc = reference;
				reference->mRefChilds.Add(child);
			}
		}
	}

	if(mFixedLocalVars != NULL) {
		for(int i = 0; i < mFixedLocalCount; i++) {
			if(mFixedLocalVars[i] != NULL) {
				AK_REFERENCE_DECREASE(mFixedLocalVars[i]);
			}
		}

		delete[] mFixedLocalVars;
	}

	if(mFixedLocalKeys != NULL) {
		delete[] mFixedLocalKeys;
	}

	if(mFixedLocalRef != NULL) {
		delete[] mFixedLocalRef;
	}

	if(mFixedLocalTypes != NULL) {
		delete[] mFixedLocalTypes;
	}

	if(mCode != NULL) {
		delete[] mCode;
	}


}

Variable* Function::Bind()
{
	ArgumentData data;
	data.mParser = mParser;
	data.mCaller = this;
	data.mCount = 0;

	Variable* temp = Bind(&data);
	return temp;
}

Variable* Function::Bind(Variable* rObject)
{
	Variable* object = mCurrentObject;
	mCurrentObject = rObject;

	ArgumentData data;
	data.mParser = mParser;
	data.mCaller = this;

	Variable* temp = Bind(&data);
	mCurrentObject = object;
	
	return temp;
}

Variable* Function::Bind(ArgumentData* rArgs)
{
	if(mParser->GetCurrentStackSize() >= 1048576) {
		throw StackFunctionException((MCString)mName.mBuffer);
		// TODO: Turn this on when debugging
		//return GetDefaultVarNull();
	}

	mForceReturn = false;

	if(mCurrentObject != NULL) {
		mParser->mGarbageCollector.RegisterVariable(mCurrentObject);
	}
}

Variable* Function::Bind(ArgumentData* rArgs, Variable* rObject)
{

}

Variable* Function::Bind(List<Variable*> *rArgs)
{

}

Variable* Function::Bind(List<Variable*> *rArgs, Variable* rObject)
{

}

Variable* Function::InternalBind(ExecuteData* rData)
{

}

void Function::SetRef(Function* rFunc, int rEntryPoint)
{

}

void Function::SetEnv(Environment* rEnv)
{

}

void Function::SetVar(Variable* rVar, int rIndex)
{

}

void Function::SetVar(MCString rKey, Variable* rVar, bool rIsGlobal)
{

}

Function* Function::GetRev()
{

}

Environment* Function::GetEnv()
{

}
