// Internal
#include "Variable.h"

// External
#include <cstring>
#include <stdlib.h>
#include <assert.h>
#include <sstream>
#include <math.h>

Variable* Variable::CreateVariable(const char *aValue) {
	assert(aValue != NULL && strlen(aValue) != 0);

	Variable *variable = new Variable();
	variable->Set(aValue);
	return variable;
}

Variable::Type Variable::GetType(std::string aString) {
	bool flt = false;

	for (int i=0; i < (int)aString.length(); i++) {
		if (aString[i] >= '0' && aString[i] <= '9') {
		} else if (aString[i] == '.') {
			if (flt) {
				return STRING;
			} 

			flt = true;
		} else if (i && aString[i] == '-') {
			return STRING;
		}  else {
			return STRING;
		}
	}

	if (flt) {
		return FLOAT;
	} else {
		return INT;
	}
}

Variable::Variable(int aID) {
	type = UNDEFINED;

	valueInteger = 0;
	valueFloat = 0.0f;
	valueString = NULL;

	valueID = aID;
}

Variable::~Variable() {
	if (valueString) {
		delete[] valueString;
	}	
}

Variable::Type Variable::GetType() const {
	return type;
}

int Variable::GetID() const {
	return valueID;
}

void Variable::Set(int aValue) {
	Clear();

	type = INT;
	valueInteger = aValue;
}

void Variable::Set(float aValue) {
	Clear();

	type = FLOAT;
	valueFloat = aValue;
}

void Variable::Set(const char *aValue) {
	Clear();

	Variable::Type t = GetType(aValue);
	if (t == INT) {
		Set(atoi(aValue));
		return;
	} else if (t == FLOAT) {
		Set((float)atof(aValue));
		return;
	}

	type = STRING;

	if (valueString != NULL) {
		delete[] valueString;
	}

	valueString = new char[strlen(aValue)];
	//strcpy(valueString, aValue);
	strcpy_s(valueString, strlen(aValue), aValue);
}

void Variable::Undefine() {
	type = UNDEFINED;
	valueFloat = 0.f;
	valueInteger = 0;
	valueString = NULL;
}

int Variable::GetInteger() const {
	if (type == INT) {
		return valueInteger;
	} else if (type == FLOAT) {
		return (int)valueFloat;
	} else if (type == STRING && valueString) {
		return atoi(valueString);
	}
	return 0;
}

float Variable::GetFloat() const {
	if (type == FLOAT) {
		return valueFloat;
	} else if (type == INT) {
		return (float)valueInteger;
	} else if (type == STRING && valueString) {
		return (float)strtod(valueString, NULL);
	}

	return 0.0;
}

const char* Variable::GetString() const {
	if (type == STRING) {
		return valueString;
	} else if (type == FLOAT) {
		std::stringstream ss;
		ss << valueFloat;
		return ss.str().c_str();
	} else if (type == INT) {
		std::stringstream ss;
		ss << valueInteger;
		return ss.str().c_str();
	}
	return "NULL";
}

void Variable::operator=(const Variable &aVariable) {
	Variable::Type type = aVariable.GetType();

	if (type == INT) {
		Set(aVariable.GetInteger());
	} else if (type == FLOAT) {
		Set(aVariable.GetFloat());
	} else if (type == STRING) {
		Set(aVariable.GetString());
	} else {
		Clear();
	}
}

void Variable::operator+=(const Variable &aVariable) {
	if (type == INT) {
		valueInteger += aVariable.GetInteger();
	} else if (type == FLOAT) {
		valueFloat += aVariable.GetFloat();
	}
}

void Variable::operator+=(const int &aValue) {
	if (type == INT) {
		valueInteger += aValue;
	} else if (type == FLOAT) {
		valueFloat += aValue;
	} else if (type == UNDEFINED) {
		Set(aValue);
	}
}

void Variable::operator+=(const float &aValue) {
	if (type == FLOAT) {
		valueFloat += aValue;
	} else if (type == INT) {
		Set(aValue + (float)valueInteger);
	} else if (type == UNDEFINED) {
		Set(aValue);
	}
}

void Variable::operator-=(const Variable &aVariable) {
	if (type == INT) {
		valueInteger -= aVariable.GetInteger();
	} else if (type == FLOAT) {
		valueFloat -= aVariable.GetFloat();
	}
}

void Variable::operator-=(const int &aValue) {
	if (type == INT) {
		valueInteger -= aValue;
	} else if (type == FLOAT) {
		valueFloat -= aValue;
	} else if (type == UNDEFINED) {
		Set(-aValue);
	}
}

void Variable::operator-=(const float &aValue) {
	if (type == FLOAT) {
		valueFloat -= aValue;
	} else if (type == INT) {
		Set((float)valueInteger - aValue);
	} else if (type == UNDEFINED) {
		Set(-aValue);
	}
}

void Variable::operator*=(const Variable &aVariable) {
	if (type == INT) {
		valueInteger *= aVariable.GetInteger();
	} else if (type == FLOAT) {
		valueFloat *= aVariable.GetFloat();
	}
}

void Variable::operator*=(const int &aValue) {
	if (type == INT) {
		valueInteger *= aValue;
	} else if (type == FLOAT) {
		valueFloat *= (float)aValue;
	} 
}

void Variable::operator*=(const float &aValue) {
	if (type == FLOAT) {
		valueFloat *= aValue;
	} else if (type == INT) {
		Set((float)valueInteger * aValue);
	}
}

void Variable::operator/=(const Variable &aVariable) {
	if (aVariable.GetFloat() == 0.f || !aVariable.GetInteger()) {
		throw "DIVISION BY ZERO";
	}

	if (type == INT) {
		valueInteger /= aVariable.GetInteger();
	} else if (type == FLOAT) {
		valueFloat /= aVariable.GetFloat();
	}
}

void Variable::operator/=(const int &aValue) {
	if (type == INT) {
		valueInteger /= aValue;
	} else if (type == FLOAT) {
		valueFloat /= (float)aValue;
	}
}

void Variable::operator/=(const float &aValue) {
	if (type == FLOAT) {
		valueFloat /= aValue;
	} else if (type == INT) {
		Set((float)valueInteger / aValue);
	}
}

void Variable::operator%=(const Variable &aVariable) {
	Set(GetInteger() % aVariable.GetInteger());
}

void Variable::operator%=(const int &aVariable) {
	if (type == INT) {
		valueInteger %= aVariable;
	}
}

bool Variable::operator>(const Variable &aVariable) const {
	commandPrompResults res = CompareWith(aVariable);
	return res == GREATER;
}

bool Variable::operator>(const int &aValue) const {
	commandPrompResults res = CompareWithInt(aValue);
	return res == GREATER;
}

bool Variable::operator>(const float &aValue) const {
	commandPrompResults res = CompareWithFloat(aValue);
	return res == GREATER;
}

bool Variable::operator>=(const Variable &aVariable) const {
	commandPrompResults res = CompareWith(aVariable);
	return (res == GREATER || res == EQUAL);
}

bool Variable::operator>=(const int &aValue) const {
	commandPrompResults res = CompareWithInt(aValue);
	return (res == GREATER || res == EQUAL);
}

bool Variable::operator>=(const float &aValue) const {
	commandPrompResults res = CompareWithFloat(aValue);
	return (res == GREATER || res == EQUAL);
}

bool Variable::operator<(const Variable &aVariable) const {
	commandPrompResults res = CompareWith(aVariable);
	return res == LESS;
}

bool Variable::operator<(const int &aValue) const {
	commandPrompResults res = CompareWithInt(aValue);
	return res == LESS;
}

bool Variable::operator<(const float &aValue) const {
	commandPrompResults res = CompareWithFloat(aValue);
	return res == LESS;
}

bool Variable::operator<=(const Variable &aVariable) const {
	commandPrompResults res = CompareWith(aVariable);
	return (res == LESS || res == EQUAL);
}

bool Variable::operator<=(const int &aValue) const {
	commandPrompResults res = CompareWithInt(aValue);
	return (res == LESS || res == EQUAL);
}

bool Variable::operator<=(const float &aValue) const {
	commandPrompResults res = CompareWithFloat(aValue);
	return (res == LESS || res == EQUAL);
}

bool Variable::operator==(const Variable &aVariable) const {
	commandPrompResults res = CompareWith(aVariable);
	return res == EQUAL;
}

bool Variable::operator==(const int &aValue) const {
	commandPrompResults res = CompareWithInt(aValue);
	return res == EQUAL;
}

bool Variable::operator==(const float &aValue) const {
	commandPrompResults res = CompareWithFloat(aValue);
	return res == EQUAL;
}

bool Variable::operator!=(const Variable &aVariable) const {
	commandPrompResults res = CompareWith(aVariable);
	return res != EQUAL;
}

bool Variable::operator!=(const int &val) const {
	commandPrompResults res = CompareWithInt(val);
	return res != EQUAL;
}

bool Variable::operator!=(const float &aValue) const {
	commandPrompResults res = CompareWithFloat(aValue);
	return res != EQUAL;
}

void Variable::Clear() {
	type = UNDEFINED;

	if (valueString) {
		delete[] valueString;
	}

	valueInteger = 0;
	valueFloat = 0.0;
}

Variable::commandPrompResults Variable::CompareWith(const Variable &aVariable) const {
	Variable::Type type = aVariable.GetType();
	if (type == FLOAT || type == FLOAT) {
		return CompareWithFloat(aVariable.GetFloat());
	} else if (type == INT && type == INT) {
		return CompareWithInt(aVariable.GetInteger());
	} else if (type == STRING || type == STRING) {
		return CompareWithString(aVariable.GetString());
	}

	return UNDEFCMP;
}

Variable::commandPrompResults Variable::CompareWithInt(const int &aValue) const {
	int tVal = GetInteger();

	if (tVal < aValue) {
		return LESS;
	} else if (tVal > aValue) {
		return GREATER;
	} else if (tVal == aValue) {
		return EQUAL;
	}

	return UNDEFCMP;
}

Variable::commandPrompResults Variable::CompareWithFloat(const float &aValue) const {
	float tVal = GetFloat();

	if ((float)fabs((float)tVal-aValue) < 0.0000001f) {
		return EQUAL;
	} if (tVal < aValue) {
		return LESS;
	} else if (tVal > aValue) {
		return GREATER;
	}

	return UNDEFCMP;
}

Variable::commandPrompResults Variable::CompareWithString(const char *aValue) const {
	const char *tVal = GetString();

	if (strcmp(tVal, aValue) == 0) {
		return EQUAL;
	}

	return NOTQUAL;
}