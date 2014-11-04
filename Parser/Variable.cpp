// Internal
#include "Variable.hpp"

// External
#include <cstring>
#include <stdlib.h>
#include <assert.h>
#include <sstream>
#include <math.h>

Variable* Variable::createVariable(const char *aValue) {
	assert(aValue != NULL && strlen(aValue) != 0);

	Variable *variable = new Variable();
	variable->set(aValue);
	return variable;
}

Variable::Type Variable::getType(std::string aString) {
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

Variable::Type Variable::getType() const {
	return type;
}

int Variable::getID() const {
	return valueID;
}

void Variable::set(int aValue) {
	clear();

	type = INT;
	valueInteger = aValue;
}

void Variable::set(float aValue) {
	clear();

	type = FLOAT;
	valueFloat = aValue;
}

void Variable::set(const char *aValue) {
	clear();

	Variable::Type t = getType(aValue);
	if (t == INT) {
		set(atoi(aValue));
		return;
	} else if (t == FLOAT) {
		set((float)atof(aValue));
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

void Variable::undefine() {
	type = UNDEFINED;
	valueFloat = 0.f;
	valueInteger = 0;
	valueString = NULL;
}

int Variable::getInteger() const {
	if (type == INT) {
		return valueInteger;
	} else if (type == FLOAT) {
		return (int)valueFloat;
	} else if (type == STRING && valueString) {
		return atoi(valueString);
	}
	return 0;
}

float Variable::getFloat() const {
	if (type == FLOAT) {
		return valueFloat;
	} else if (type == INT) {
		return valueInteger;
	} else if (type == STRING && valueString) {
		return strtod(valueString, NULL);
	}

	return 0.0;
}

const char* Variable::getString() const {
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
	Variable::Type type = aVariable.getType();

	if (type == INT) {
		set(aVariable.getInteger());
	} else if (type == FLOAT) {
		set(aVariable.getFloat());
	} else if (type == STRING) {
		set(aVariable.getString());
	} else {
		clear();
	}
}

void Variable::operator+=(const Variable &aVariable) {
	if (type == INT) {
		valueInteger += aVariable.getInteger();
	} else if (type == FLOAT) {
		valueFloat += aVariable.getFloat();
	}
}

void Variable::operator+=(const int &aValue) {
	if (type == INT) {
		valueInteger += aValue;
	} else if (type == FLOAT) {
		valueFloat += aValue;
	} else if (type == UNDEFINED) {
		set(aValue);
	}
}

void Variable::operator+=(const float &aValue) {
	if (type == FLOAT) {
		valueFloat += aValue;
	} else if (type == INT) {
		set(aValue + (float)valueInteger);
	} else if (type == UNDEFINED) {
		set(aValue);
	}
}

void Variable::operator-=(const Variable &aVariable) {
	if (type == INT) {
		valueInteger -= aVariable.getInteger();
	} else if (type == FLOAT) {
		valueFloat -= aVariable.getFloat();
	}
}

void Variable::operator-=(const int &aValue) {
	if (type == INT) {
		valueInteger -= aValue;
	} else if (type == FLOAT) {
		valueFloat -= aValue;
	} else if (type == UNDEFINED) {
		set(-aValue);
	}
}

void Variable::operator-=(const float &aValue) {
	if (type == FLOAT) {
		valueFloat -= aValue;
	} else if (type == INT) {
		set((float)valueInteger - aValue);
	} else if (type == UNDEFINED) {
		set(-aValue);
	}
}

void Variable::operator*=(const Variable &aVariable) {
	if (type == INT) {
		valueInteger *= aVariable.getInteger();
	} else if (type == FLOAT) {
		valueFloat *= aVariable.getFloat();
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
		set((float)valueInteger * aValue);
	}
}

void Variable::operator/=(const Variable &aVariable) {
	if (aVariable.getFloat() == 0.f || !aVariable.getInteger()) {
		throw "DIVISION BY ZERO";
	}

	if (type == INT) {
		valueInteger /= aVariable.getInteger();
	} else if (type == FLOAT) {
		valueFloat /= aVariable.getFloat();
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
		set((float)valueInteger / aValue);
	}
}

void Variable::operator%=(const Variable &aVariable) {
	set(getInteger() % aVariable.getInteger());
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

void Variable::clear() {
	type = UNDEFINED;

	if (valueString) {
		delete[] valueString;
	}

	valueInteger = 0;
	valueFloat = 0.0;
}

Variable::commandPrompResults Variable::CompareWith(const Variable &aVariable) const {
	Variable::Type type = aVariable.getType();
	if (type == FLOAT || type == FLOAT) {
		return CompareWithFloat(aVariable.getFloat());
	} else if (type == INT && type == INT) {
		return CompareWithInt(aVariable.getInteger());
	} else if (type == STRING || type == STRING) {
		return CompareWithString(aVariable.getString());
	}

	return UNDEFCMP;
}

Variable::commandPrompResults Variable::CompareWithInt(const int &aValue) const {
	int tVal = getInteger();

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
	float tVal = getFloat();

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
	const char *tVal = getString();

	if (strcmp(tVal, aValue) == 0) {
		return EQUAL;
	}

	return NOTQUAL;
}