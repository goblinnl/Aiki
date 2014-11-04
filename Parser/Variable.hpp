#ifndef VARIABLE_H
#define VARIABLE_H

// External
#include <string>

class Variable {
	public:
		enum Type {
			UNDEFINED,
			INT,
			FLOAT,
			STRING,
			OBJECT,
		};

		static Variable* createVariable(const char *aValue);
		static Type	getType(std::string aString);


		Variable(int aID = 0);
		~Variable();

		Type getType() const;
		int getID() const;
		int getInteger() const;
		float getFloat() const;
		const char*	getString() const;

		void set(int);
		void set(float);
		void set(const char*);
		void undefine();

		void operator=(const Variable &aVariable);
		void operator+=(const Variable &aVariable);
		void operator+=(const int &aVariable);
		void operator+=(const float &aVariable);
		void operator-=(const Variable &aVariable);
		void operator-=(const int &aVariable);
		void operator-=(const float &aVariable);
		void operator*=(const Variable &aVariable);
		void operator*=(const int &aVariable);
		void operator*=(const float &aVariable);
		void operator/=(const Variable &aVariable);
		void operator/=(const int &aVariable);
		void operator/=(const float &aVariable);
		void operator%=(const Variable &aVariable);
		void operator%=(const int &aVariable);
		bool operator>(const Variable &aVariable) const;
		bool operator>(const int &aVariable) const;
		bool operator>(const float &aVariable) const;
		bool operator>=(const Variable &aVariable) const;
		bool operator>=(const int &aVariable) const;
		bool operator>=(const float &aVariable) const;	
		bool operator<(const Variable &aVariable) const;
		bool operator<(const int &aVariable) const;
		bool operator<(const float &aVariable) const;
		bool operator<=(const Variable &aVariable) const;
		bool operator<=(const int &aVariable) const;
		bool operator<=(const float &aVariable) const;
		bool operator==(const Variable &aVariable) const;
		bool operator==(const int &aVariable) const;
		bool operator==(const float &aVariable) const;
		bool operator!=(const Variable &aVariable) const;
		bool operator!=(const int &aVariable) const;
		bool operator!=(const float &aVariable) const;

	private:
		Type type;
		int valueInteger;
		float valueFloat;
		char* valueString;
		int valueID;

		bool convert(const char *aString);
		void clear();

		enum commandPrompResults {
			UNDEFCMP = 0x01,
			GREATER = 0x02,
			LESS = 0x04,
			EQUAL = 0x08,
			NOTQUAL = 0x10,
		};

		commandPrompResults CompareWith(const Variable &aVariable) const;
		commandPrompResults CompareWithInt(const int &aVariable) const;
		commandPrompResults CompareWithFloat(const float &aVariable) const;
		commandPrompResults CompareWithString(const char *aVariable) const;
};

#endif // VARIABLE_H