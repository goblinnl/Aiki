// Internal
#include "Variable.h"
#include "../Mixer/MCommon.h"
#include "../Mixer/MString.h"

// External
#include <cstring>
#include <stdlib.h>
#include <assert.h>
#include <sstream>
#include <math.h>

Variable* Variable::CreateVariable(const char *rValue) {
	assert(rValue != NULL && strlen(rValue) != 0);

	Variable *variable = new Variable();
	variable->Set(rValue);
	return variable;
}

Variable::Type Variable::GetType(MString rString) {
	bool flt = false;

	for(int i = 0; i < (int)rString.Length(); i++) {
		if(rString[i] >= '0' && rString[i] <= '9') {
		}
		else if(rString[i] == '.') {
			if(flt) {
				return STRING;
			}

			flt = true;
		}
		else if(i && rString[i] == '-') {
			return STRING;
		}
		else {
			return STRING;
		}
	}

	if(flt) {
		return FLOAT;
	}
	else {
		return INT;
	}
}

Variable::Variable(int rID) {
	mType = UNDEFINED;

	mValueInteger = 0;
	mValueFloat = 0.0f;
	mValueString = NULL;

	mValueID = rID;
}

Variable::~Variable() {
	if(mValueString) {
		delete[] mValueString;
	}
}

Variable::Type Variable::GetType() const {
	return mType;
}

int Variable::GetID() const {
	return mValueID;
}

void Variable::Set(int rValue) {
	Clear();

	mType = INT;
	mValueInteger = rValue;
}

void Variable::Set(float rValue) {
	Clear();

	mType = FLOAT;
	mValueFloat = rValue;
}

void Variable::Set(const char *rValue) {
	Clear();

	Variable::Type t = GetType(rValue);
	if(t == INT) {
		Set(atoi(rValue));
		return;
	}
	else if(t == FLOAT) {
		Set((float)atof(rValue));
		return;
	}

	mType = STRING;

	if(mValueString != NULL) {
		delete[] mValueString;
	}

	mValueString = new char[strlen(rValue)];
	//strcpy(valueString, aValue);
	strcpy_s(mValueString, strlen(rValue), rValue);
}

void Variable::Undefine() {
	mType = UNDEFINED;
	mValueFloat = 0.f;
	mValueInteger = 0;
	mValueString = NULL;
}

int Variable::GetInteger() const {
	if(mType == INT) {
		return mValueInteger;
	}
	else if(mType == FLOAT) {
		return (int)mValueFloat;
	}
	else if(mType == STRING && mValueString) {
		return atoi(mValueString);
	}
	return 0;
}

float Variable::GetFloat() const {
	if(mType == FLOAT) {
		return mValueFloat;
	}
	else if(mType == INT) {
		return (float)mValueInteger;
	}
	else if(mType == STRING && mValueString) {
		return (float)strtod(mValueString, NULL);
	}

	return 0.0;
}

const char* Variable::GetString() const {
	if(mType == STRING) {
		return mValueString;
	}
	else if(mType == FLOAT) {
		std::stringstream ss;
		ss << mValueFloat;
		return ss.str().c_str();
	}
	else if(mType == INT) {
		std::stringstream ss;
		ss << mValueInteger;
		return ss.str().c_str();
	}
	return "NULL";
}

void Variable::operator=(const Variable &rVariable) {
	Variable::Type type = rVariable.GetType();

	if(type == INT) {
		Set(rVariable.GetInteger());
	}
	else if(type == FLOAT) {
		Set(rVariable.GetFloat());
	}
	else if(type == STRING) {
		Set(rVariable.GetString());
	}
	else {
		Clear();
	}
}

void Variable::operator+=(const Variable &rVariable) {
	if(mType == INT) {
		mValueInteger += rVariable.GetInteger();
	}
	else if(mType == FLOAT) {
		mValueFloat += rVariable.GetFloat();
	}
}

void Variable::operator+=(const int &rValue) {
	if(mType == INT) {
		mValueInteger += rValue;
	}
	else if(mType == FLOAT) {
		mValueFloat += rValue;
	}
	else if(mType == UNDEFINED) {
		Set(rValue);
	}
}

void Variable::operator+=(const float &rValue) {
	if(mType == FLOAT) {
		mValueFloat += rValue;
	}
	else if(mType == INT) {
		Set(rValue + (float)mValueInteger);
	}
	else if(mType == UNDEFINED) {
		Set(rValue);
	}
}

void Variable::operator-=(const Variable &rVariable) {
	if(mType == INT) {
		mValueInteger -= rVariable.GetInteger();
	}
	else if(mType == FLOAT) {
		mValueFloat -= rVariable.GetFloat();
	}
}

void Variable::operator-=(const int &rValue) {
	if(mType == INT) {
		mValueInteger -= rValue;
	}
	else if(mType == FLOAT) {
		mValueFloat -= rValue;
	}
	else if(mType == UNDEFINED) {
		Set(-rValue);
	}
}

void Variable::operator-=(const float &rValue) {
	if(mType == FLOAT) {
		mValueFloat -= rValue;
	}
	else if(mType == INT) {
		Set((float)mValueInteger - rValue);
	}
	else if(mType == UNDEFINED) {
		Set(-rValue);
	}
}

void Variable::operator*=(const Variable &rVariable) {
	if(mType == INT) {
		mValueInteger *= rVariable.GetInteger();
	}
	else if(mType == FLOAT) {
		mValueFloat *= rVariable.GetFloat();
	}
}

void Variable::operator*=(const int &rValue) {
	if(mType == INT) {
		mValueInteger *= rValue;
	}
	else if(mType == FLOAT) {
		mValueFloat *= (float)rValue;
	}
}

void Variable::operator*=(const float &rValue) {
	if(mType == FLOAT) {
		mValueFloat *= rValue;
	}
	else if(mType == INT) {
		Set((float)mValueInteger * rValue);
	}
}

void Variable::operator/=(const Variable &rVariable) {
	if(rVariable.GetFloat() == 0.f || !rVariable.GetInteger()) {
		throw "DIVISION BY ZERO";
	}

	if(mType == INT) {
		mValueInteger /= rVariable.GetInteger();
	}
	else if(mType == FLOAT) {
		mValueFloat /= rVariable.GetFloat();
	}
}

void Variable::operator/=(const int &rValue) {
	if(mType == INT) {
		mValueInteger /= rValue;
	}
	else if(mType == FLOAT) {
		mValueFloat /= (float)rValue;
	}
}

void Variable::operator/=(const float &rValue) {
	if(mType == FLOAT) {
		mValueFloat /= rValue;
	}
	else if(mType == INT) {
		Set((float)mValueInteger / rValue);
	}
}

void Variable::operator%=(const Variable &rVariable) {
	Set(GetInteger() % rVariable.GetInteger());
}

void Variable::operator%=(const int &rVariable) {
	if(mType == INT) {
		mValueInteger %= rVariable;
	}
}

bool Variable::operator>(const Variable &rVariable) const {
	commandPrompResults res = CompareWith(rVariable);
	return res == GREATER;
}

bool Variable::operator>(const int &rValue) const {
	commandPrompResults res = CompareWithInt(rValue);
	return res == GREATER;
}

bool Variable::operator>(const float &rValue) const {
	commandPrompResults res = CompareWithFloat(rValue);
	return res == GREATER;
}

bool Variable::operator>=(const Variable &rVariable) const {
	commandPrompResults res = CompareWith(rVariable);
	return (res == GREATER || res == EQUAL);
}

bool Variable::operator>=(const int &rValue) const {
	commandPrompResults res = CompareWithInt(rValue);
	return (res == GREATER || res == EQUAL);
}

bool Variable::operator>=(const float &rValue) const {
	commandPrompResults res = CompareWithFloat(rValue);
	return (res == GREATER || res == EQUAL);
}

bool Variable::operator<(const Variable &rVariable) const {
	commandPrompResults res = CompareWith(rVariable);
	return res == LESS;
}

bool Variable::operator<(const int &rValue) const {
	commandPrompResults res = CompareWithInt(rValue);
	return res == LESS;
}

bool Variable::operator<(const float &rValue) const {
	commandPrompResults res = CompareWithFloat(rValue);
	return res == LESS;
}

bool Variable::operator<=(const Variable &rVariable) const {
	commandPrompResults res = CompareWith(rVariable);
	return (res == LESS || res == EQUAL);
}

bool Variable::operator<=(const int &rValue) const {
	commandPrompResults res = CompareWithInt(rValue);
	return (res == LESS || res == EQUAL);
}

bool Variable::operator<=(const float &rValue) const {
	commandPrompResults res = CompareWithFloat(rValue);
	return (res == LESS || res == EQUAL);
}

bool Variable::operator==(const Variable &rVariable) const {
	commandPrompResults res = CompareWith(rVariable);
	return res == EQUAL;
}

bool Variable::operator==(const int &rValue) const {
	commandPrompResults res = CompareWithInt(rValue);
	return res == EQUAL;
}

bool Variable::operator==(const float &rValue) const {
	commandPrompResults res = CompareWithFloat(rValue);
	return res == EQUAL;
}

bool Variable::operator!=(const Variable &rVariable) const {
	commandPrompResults res = CompareWith(rVariable);
	return res != EQUAL;
}

bool Variable::operator!=(const int &rValue) const {
	commandPrompResults res = CompareWithInt(rValue);
	return res != EQUAL;
}

bool Variable::operator!=(const float &rValue) const {
	commandPrompResults res = CompareWithFloat(rValue);
	return res != EQUAL;
}

void Variable::Clear() {
	mType = UNDEFINED;

	if(mValueString) {
		delete[] mValueString;
	}

	mValueInteger = 0;
	mValueFloat = 0.0;
}

Variable::commandPrompResults Variable::CompareWith(const Variable &rVariable) const {
	Variable::Type type = rVariable.GetType();
	if(type == FLOAT || type == FLOAT) {
		return CompareWithFloat(rVariable.GetFloat());
	}
	else if(type == INT && type == INT) {
		return CompareWithInt(rVariable.GetInteger());
	}
	else if(type == STRING || type == STRING) {
		return CompareWithString(rVariable.GetString());
	}

	return UNDEFCMP;
}

Variable::commandPrompResults Variable::CompareWithInt(const int &rValue) const {
	int tVal = GetInteger();

	if(tVal < rValue) {
		return LESS;
	}
	else if(tVal > rValue) {
		return GREATER;
	}
	else if(tVal == rValue) {
		return EQUAL;
	}

	return UNDEFCMP;
}

Variable::commandPrompResults Variable::CompareWithFloat(const float &rValue) const {
	float tVal = GetFloat();

	if((float)fabs((float)tVal - rValue) < 0.0000001f) {
		return EQUAL;
	}

	if(tVal < rValue) {
		return LESS;
	}
	else if(tVal > rValue) {
		return GREATER;
	}

	return UNDEFCMP;
}

Variable::commandPrompResults Variable::CompareWithString(const char *rValue) const {
	const char *tVal = GetString();

	if(strcmp(tVal, rValue) == 0) {
		return EQUAL;
	}

	return NOTQUAL;
}