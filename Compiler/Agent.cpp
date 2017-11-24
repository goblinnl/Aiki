#include "Agent.h"

Agent::Agent()
{

}

Agent::~Agent()
{

}

Variable* Agent::GetAO(Stack* rStack, Variable* rVar0, Variable* rVar1, int rCode)
{
	Variable* temp;

	if(rVar0->GetType() > Variable::DATA || rVar1->GetType() > Variable::DATA) {
		DataInstance* inst;

		if(rVar0->GetType() > Variable::DATA) {
			inst = (DataInstance*)rVar0->GetData();
		}
		else {
			inst = (DataInstance*)rVar1->GetData();
		}

		AKFunction func = inst->GetOperator(rCode);
		if(func != nullptr) {
			DataArgument arg;
			arg.AddVar(rVar0);
			arg.AddVar(rVar1);
			arg.SetParser(rStack->GetParser());

			temp = func(rStack->GetParser(), &arg);

			if(temp == NULL) {
				temp = rStack->GetParser()->GetUndefinedVar();
			}
			else {
				rStack->GetParser()->GetGarbageCollector().RegisterVariable(temp);
			}
			return temp;
		}
	}

	Variable* obj = NULL;
	if(rVar0->GetType() == Variable::TABLE && rVar0->GetTable()->Get("__TYPE")->GetType() != Variable::UNDEFINED) {
		obj = rVar0;
	}

	if(rVar1->GetType() == Variable::TABLE && rVar1->GetTable()->Get("__TYPE")->GetType() != Variable::UNDEFINED) {
		obj = rVar1;
	}

	if(obj != NULL) {
		Variable* temp = obj->GetTable()->Get(MCString::Format("__%s", gParse::Instance()->OperatorToString(rCode).mBuffer));
	
		if(temp->GetType() == Variable::UNDEFINED) {
			throw InvalidOpException(MCString::Format("Invalid arithmetic operator, %s %s %s", gParse::Instance()->ObjectToString(rVar0).mBuffer, gParse::Instance()->OperatorToString(rCode).mBuffer, gParse::Instance()->ObjectToString(rVar1).mBuffer));
			return rStack->GetParser()->GetUndefinedVar();
		}

		if(temp->GetType() == Variable::FUNCTION) {
			throw InvalidOpException(MCString::Format("Invalid arithmetic operator, %s %s %s", gParse::Instance()->ObjectToString(rVar0).mBuffer, gParse::Instance()->OperatorToString(rCode).mBuffer, gParse::Instance()->ObjectToString(rVar1).mBuffer));
			return rStack->GetParser()->GetUndefinedVar();
		}

		DataArgument arg;
		arg.AddVar(rVar0);
		arg.AddVar(rVar1);
		arg.SetParser(rStack->GetParser());

		return temp->GetFunction()->Bind(&arg, obj);		
	}

	temp = rStack->GetParser()->GetGarbageCollector().GetPooledVariable();

	// Arithmetic operations
	if(rCode == AO_ADD) { Add(*temp, rVar0, rVar1); }
	if(rCode == AO_SUBTRACT) { Substract(*temp, rVar0, rVar1); }
	if(rCode == AO_DIVIDE) { Divide(rStack->GetParser(),*temp, rVar0, rVar1); }
	if(rCode == AO_MULTIPLY) { Multiply(rStack->GetParser(),*temp, rVar0, rVar1); }
	if(rCode == AO_NOT_EQUAL) { NotEqual(*temp, rVar0, rVar1); }
	if(rCode == AO_EQUAL) { Equal(*temp, rVar0, rVar1); }
	if(rCode == AO_GREATER_THAN) { GreaterThan(*temp, rVar0, rVar1); }
	if(rCode == AO_GREATER_OR_EQUAL) { GreaterOrEqual(*temp, rVar0, rVar1); }
	if(rCode == AO_LESS_THAN) { LessThan(*temp, rVar0, rVar1); }
	if(rCode == AO_LESS_OR_EQUAL) { LessOrEqual(*temp, rVar0, rVar1); }
	if(rCode == AO_MODULO) { Modulo(*temp, rVar0, rVar1); }
	if(rCode == AO_AND) { And(*temp, rVar0, rVar1); }
	if(rCode == AO_OR) { Or(*temp, rVar0, rVar1); }
	if(rCode == AO_BITWISE_LEFT) { BitwiseLeft(*temp, rVar0, rVar1); }
	if(rCode == AO_BITWISE_RIGHT) { BitwiseRight(*temp, rVar0, rVar1); }
	if(rCode == AO_BITWISE_OR) { BitwiseOr(*temp, rVar0, rVar1); }
	if(rCode == AO_BITWISE_AND) { BitwiseAnd(*temp, rVar0, rVar1); }
	

	if(temp->GetType() == Variable::UNDEFINED) {
		throw InvalidOpException(MCString::Format("Invalid arithmetic function: See %s(%s) %s %s(%s)", gParse::Instance()->ObjectToString(rVar0).mBuffer, gParse::Instance()->VarToString(rStack->GetParser(), rVar0).mBuffer, gParse::Instance()->OperatorToString(rCode).mBuffer, gParse::Instance()->ObjectToString(rVar1).mBuffer, gParse::Instance()->VarToString(rStack->GetParser(), rVar1).mBuffer));
	}

	return temp;
}

void Agent::Duplicate(Stack* rStack)
{
	rStack->Push(rStack->GetStack()[rStack->GetPosition() - 1]);
}

void Agent::IncreaseIndex(Stack* rStack)
{
	Variable* data = rStack->Pop();
	Variable* temp = rStack->GetStack()[rStack->GetPosition() - 1];

	bool isKey = rStack->GetByteParser()->ReadBool();

	MCString key = isKey ? rStack->GetByteParser()->ReadString() : MCString::Format("%d", temp->GetTable()->mCount);
	if(temp->GetType() == Variable::TABLE) {
		temp->GetTable()->Set(key, data);
		return;
	}

	if(temp->GetType() > Variable::DATA) {
		DataInstance* ins = (DataInstance*)temp->GetData();
		AK_REFERENCE_INCREASE(temp);
		AK_REFERENCE_INCREASE(data);
		ins->SetIndex(temp, rStack->GetParser()->GetGarbageCollector().RegisterVariable(gParse::Instance()->ToVar(rStack->GetParser(), key)), data);
		AK_REFERENCE_DECREASE(temp);
		AK_REFERENCE_DECREASE(data);
		return;
	}

	throw InvalidIndexException(MCString::Format("Cannot add an index on a %s", gParse::Instance()->ObjectToString(rStack->GetParser(), temp->GetType()).mBuffer));
}

void Agent::GetSize(Stack* rStack)
{
	Variable* temp = rStack->Pop();
	
	if(temp->GetType() == Variable::TABLE) {
		rStack->Push(rStack->GetParser()->GetGarbageCollector().RegisterVariable(gParse::Instance()->ToVar(rStack->GetParser(), temp->GetString().Length())));
		return;
	}

	if(temp->GetType() > Variable::DATA) {
		DataInstance* data = (DataInstance*)temp->GetData();
		AKFunction func = data->GetOperator(AO_GET_COUNTER);
		if(func != nullptr) {
			DataArgument arg;
			arg.SetParser(rStack->GetParser());
			arg.SetFunction(rStack->GetFunction());
			arg.SetObject(temp);

			AK_REFERENCE_INCREASE(temp);
			Variable* var = func(rStack->GetParser(), &arg);
			AK_REFERENCE_INCREASE(temp);

			arg.Destroy();

			if(var == NULL) {
				var = rStack->GetParser()->GetUndefinedVar();
			}
			else {
				rStack->GetParser()->GetGarbageCollector().RegisterVariable(var);
			}
			rStack->Push(var);
			return;
		}
	}

	throw InvalidIndexException(MCString::Format("Cannot read size of %s",gParse::Instance()->ObjectToString(rStack->GetParser(), temp->GetType()).mBuffer));
}

void Agent::GetIndex(Stack* rStack)
{
	Variable* key = rStack->Pop();
	Variable* op = rStack->Pop();

	if(op->GetType() == Variable::TABLE) {
		rStack->Push(op->GetTable()->Get(key->ToString(rStack->GetParser())));
		return;
	}

	if(op->GetType() == Variable::STRING) {
		if(key->GetType() == Variable::NUMBER) {
			rStack->Push(gParse::Instance()->ToVar(rStack->GetParser(), MCString(op->GetString()[(int)key->GetNumber()])));
			return;
		}

		throw InvalidIndexException(MCString::Format("Unable to index a string with %s", gParse::Instance()->ObjectToString(rStack->GetParser(), key->GetType()).mBuffer));
		return;
	}

	if(op->GetType() > Variable::DATA) {
		DataInstance* inst = (DataInstance*)op->GetData();

		AK_REFERENCE_INCREASE(key);
		AK_REFERENCE_INCREASE(op);
		rStack->Push(inst->GetIndex(op, key));
		AK_REFERENCE_DECREASE(key);
		AK_REFERENCE_DECREASE(op);
	}

	if(op->GetType() == Variable::CLASS) {
		Function* temp = (Function*)op->GetData();
		if(temp->mIsCpp) {
			Data* data = rStack->GetParser()->GetRegisteredData(temp->mName);
			rStack->Push(data->GetStats()->Get(key->ToString(rStack->GetParser())));
		} // TODO: Implement Classes

		return;
	}

	throw InvalidIndexException(MCString::Format("No index at position %s", gParse::Instance()->ObjectToString(rStack->GetParser(), op->GetType()).mBuffer));
}

void Agent::SetIndex(Stack* rStack)
{
	Variable* key = rStack->Pop();
	Variable* op = rStack->Pop();
	Variable* value = rStack->Pop();

	if(op->GetType() == Variable::TABLE) {
		if(op->GetType() == Variable::UNDEFINED) {
			op->GetTable()->Delete(key->ToString(rStack->GetParser()));
		}
		else {
			op->GetTable()->Set(key->ToString(rStack->GetParser()), value);
		}
	}

	if(op->GetType() > Variable::DATA) {
		DataInstance* datainst = (DataInstance*)op->GetData();

		AK_REFERENCE_INCREASE(key);
		AK_REFERENCE_INCREASE(op);
		AK_REFERENCE_INCREASE(value);
		datainst->SetIndex(op, key, value);
		AK_REFERENCE_DECREASE(key);
		AK_REFERENCE_DECREASE(op);
		AK_REFERENCE_DECREASE(value);

		return;
	}

	if(op->GetType() == Variable::CLASS) {
		Function* temp = (Function*)op->GetData();
		if(temp->mIsCpp) {
			Data* data = rStack->GetParser()->GetRegisteredData(temp->mName);

			if(value->GetType() == Variable::UNDEFINED) {
				data->GetStats()->Delete(key->ToString(rStack->GetParser()));
			}
			else {
				data->GetStats()->Set(key->ToString(rStack->GetParser()), value);
			}
 		} // TODO: Implement Classes
		return;
	}

	throw InvalidIndexException(MCString::Format("Unable to create an index at position %s", gParse::Instance()->ObjectToString(rStack->GetParser(), op->GetType()).mBuffer));
}

void Agent::Set(Stack* rStack)
{
	MCString name = rStack->GetByteParser()->ReadString();
	Variable* temp = rStack->Pop();
	temp->GetFunction()->SetVar(name, temp, true);

	// Add for debugger
	// TODO: Add debugger functionality
	if(temp->GetType() == Variable::FUNCTION) {
		temp->GetFunction()->mName = name;
	}
}

void Agent::SetLenght(Stack* rStack)
{
	Variable* var = rStack->Pop();
	int index = rStack->GetByteParser()->ReadInt();
	rStack->GetFunction()->SetVar(var, index);

	// Add for debugger
	// TODO: Add debugger functionality
	if(var->GetType() == Variable::FUNCTION) {
		var->GetFunction()->mName = rStack->GetFunction()->mFixedLocalKeys[index];
	}
}

void Agent::New(Stack* rStack)
{
	Variable* obj = rStack->Pop();

	if(obj->GetType() == Variable::UNDEFINED) {
		throw NullPointerException("Unable to instantiate null");
		return;
	}

	if(obj->GetType() != Variable::CLASS) {
		throw NullPointerException("Instance is not a class");
		return;
	}

	int paramters = rStack->GetByteParser()->ReadInt();
	DataArgument args;
	args.SetParser(rStack->GetParser());
	for(int i = 0; i < paramters; i++) {
		args.AddVar(rStack->Pop);
	}

	Function* function = (Function*)obj->GetData();
	if(function->mIsCpp) {
		Data* data = rStack->GetParser()->GetRegisteredData(function->mName);

		if(data == NULL) {
			throw NullPointerException("Unknown class type");
			return;
		}

		if(data->mConstructor == NULL) {
			throw NullPointerException("No constructor initialized, set a constructor");
			return;
		}

		Variable* var = rStack->GetParser()->GetGarbageCollector().GetPooledVariable();
		var->SetType((Variable::Type)data->GetTypeID());
		var->SetData(nullptr);

		void* ptr = data->mConstructor(rStack->GetParser(), &args, var);
		if(ptr == NULL) {
			rStack->Push(rStack->GetParser()->GetUndefinedVar());
		}
		else {
			DataInstance* inst = new DataInstance();
			inst->SetData(data);
			inst->SetPointer(ptr);
			inst->SetDestructor(true);

			var->SetData(inst);
			rStack->Push(var);
		}

		return;
	}

	// TODO: Implement AIKI Classes
	throw NullPointerException("Classes are not yet being supported");
}

void Agent::Call(Stack* rStack)
{
	DataArgument* arg = rStack->GetParser()->GetGarbageCollector().mPoolArgument.GetNext();
	if(arg == NULL) {
		arg = new DataArgument();
	}

	arg->SetParser(rStack->GetParser());
	arg->SetFunction(rStack->GetFunction());

	int counter = rStack->GetByteParser()->ReadInt();
	for(int i = 0; i < counter; i++){
		arg->AddVar(rStack->Pop());
	}

	Variable* var = rStack->Pop();
	if(var->GetType() == Variable::FUNCTION) {
		AK_REFERENCE_INCREASE(var);
		rStack->Push(var->GetFunction()->Bind(arg));
		AK_REFERENCE_DECREASE(var);

		arg->Destroy();
		rStack->GetParser()->GetGarbageCollector().mPoolArgument.Free(arg);
		return;
	}

	if(var->GetType() > Variable::DATA) {
		DataInstance* data = (DataInstance*)var->GetData();
		AKFunction op = data->GetOperator(AO_CALL);
		if(op != nullptr) {
			arg->SetObject(var);

			AK_REFERENCE_INCREASE(var);
			Variable* temp = op(rStack->GetParser(), arg);
			AK_REFERENCE_DECREASE(var);

			arg->Destroy();

			if(temp == NULL) {
				temp = rStack->GetParser()->GetUndefinedVar();
			}
			else {
				rStack->GetParser()->GetGarbageCollector().RegisterVariable(temp);
			}

			rStack->Push(temp);
			arg->Destroy();
			rStack->GetParser()->GetGarbageCollector().mPoolArgument.Free(arg);
			return;
		}
	}

	arg->Destroy();
	rStack->GetParser()->GetGarbageCollector().mPoolArgument.Free(arg);

	throw StackFunctionException(MCString::Format("Unable to call %s", gParse::Instance()->ObjectToString(rStack->GetParser(), var->GetType()).mBuffer).mBuffer);
}

void Agent::CallThis(Stack* rStack)
{
	DataArgument* arg = rStack->GetParser()->GetGarbageCollector().mPoolArgument.GetNext();
	if(arg == NULL) {
		arg = new DataArgument();
	}

	arg->SetParser(rStack->GetParser());

	int counter = rStack->GetByteParser()->ReadInt();
	for(int i = 0; i < counter; i++) {
		arg->AddVar(rStack->Pop());
	}

	Variable* var = rStack->Pop();
	Variable* obj = rStack->Pop();
	arg->SetFunction(rStack->GetFunction());

	if(var->GetType() == Variable::FUNCTION) {
		AK_REFERENCE_INCREASE(var);
		AK_REFERENCE_INCREASE(obj);
		rStack->Push(var->GetFunction()->Bind(arg, obj));
		AK_REFERENCE_DECREASE(obj);
		AK_REFERENCE_INCREASE(var);

		arg->Destroy();
		rStack->GetParser()->GetGarbageCollector().mPoolArgument.Free(arg);
		return;
	}

	if(var->GetType() > Variable::DATA) {
		DataInstance* ins = (DataInstance*)var->GetData();
		AKFunction func = ins->GetOperator(AO_CALL);
		if(func != nullptr) {
			AK_REFERENCE_INCREASE(var);
			AK_REFERENCE_INCREASE(obj);
			Variable* temp = func(rStack->GetParser(), arg);
			AK_REFERENCE_DECREASE(obj);
			AK_REFERENCE_DECREASE(var);

			arg->Destroy();

			if(temp == NULL)
			{
				temp = rStack->GetParser()->GetUndefinedVar();
			}
			else {
				rStack->GetParser()->GetGarbageCollector().RegisterVariable(temp);
			}

			rStack->Push(temp);
			arg->Destroy();
			rStack->GetParser()->GetGarbageCollector().mPoolArgument.Free(arg);
			return;
		}
	}
	arg->Destroy();
	rStack->GetParser()->GetGarbageCollector().mPoolArgument.Free(arg);
	throw StackFunctionException(MCString::Format("Unable to call %s", gParse::Instance()->ObjectToString(rStack->GetParser(), var->GetType()).mBuffer).mBuffer);
}

void Agent::PostIncrement(Stack* rStack)
{
	Variable* temp = rStack->Pop();

	if(temp->GetType() != Variable::NUMBER) {
		throw NonNumberException("Cannot increase a non-number");
		return;
	}

	double number = (*(double*)temp->GetData())++;	
	temp->SetData(*(void**)&number);
	rStack->Push(rStack->GetParser()->GetGarbageCollector().RegisterVariable(gParse::Instance()->ToVar(rStack->GetParser(), temp->GetNumber() - 1)));
}

void Agent::PreIncrement(Stack* rStack)
{
	Variable* temp = rStack->Pop();
	if(temp->GetType() != Variable::NUMBER) {
		throw NonNumberException("Cannot increase a non-number");
		return;
	}
	double number = (*(double*)temp->GetData())++;
	temp->SetData(*(void**)&number);
	rStack->Push(temp);
}

void Agent::Pop(Stack* rStack)
{
	rStack->Pop();
}

void Agent::Delete(Stack* rStack)
{
	rStack->Pop()->SetUndefined();
}

void Agent::StackNull(Stack* rStack)
{
	rStack->Push(rStack->GetParser()->GetUndefinedVar());
}

void Agent::StackBool(Stack* rStack)
{
	rStack->Push(rStack->GetByteParser()->ReadBool ? rStack->GetParser()->GetDefaultTrue() : rStack->GetParser()->GetDefaultFalse());
}

void Agent::StackNumber(Stack* rStack)
{
	rStack->Push(rStack->GetParser()->GetGarbageCollector().RegisterVariable(gParse::Instance()->ToVar(rStack->GetParser(), rStack->GetByteParser()->ReadDouble())));
}

void Agent::StackTable(Stack* rStack)
{
	rStack->Push(rStack->GetParser()->GetGarbageCollector().RegisterVariable(gParse::Instance()->ToVar(rStack->GetParser(), new Table(rStack->GetParser()))));
}

void Agent::StackString(Stack* rStack)
{
	rStack->Push(rStack->GetParser()->GetGarbageCollector().RegisterVariable(gParse::Instance()->ToVar(rStack->GetParser(), rStack->GetByteParser()->ReadString())));
}

void Agent::StackFunction(Stack* rStack)
{
	MCString fname = rStack->GetByteParser()->ReadString();

	Function* func = new Function(rStack->GetParser());
	func->mName = "Function";
	func->mFilename = fname;
	func->mParent = rStack->GetFunction();
	func->SetEnv(rStack->GetFunction()->GetEnv());

	Variable* var = rStack->GetParser()->GetGarbageCollector().GetPooledVariable();
	var->SetData(func);
	var->SetType(Variable::FUNCTION);

	List<MCString>* prms = rStack->GetByteParser()->ReadStrings();
	for(int i = 0; i < prms->GetCount(); i++) {
		func->mParameters.Add(MCString(prms->Get(i)));
	}

	int size = rStack->GetByteParser()->ReadInt();

	func->SetRef(rStack->GetFunction(), rStack->GetByteParser()->GetPosition());
	int position = rStack->GetByteParser()->GetPosition();
	position += size;
	rStack->GetByteParser()->SetPosition(position);

	int count = rStack->GetByteParser()->ReadInt();
	func->mFixedLocalCount = count;
	func->mFixedLocalVars = new Variable*[count];
	func->mFixedLocalKeys = new MCString[count];
	func->mFixedLocalTypes = new int[count];
	func->mFixedLocalRef = new bool[count];

	for(int i = 0; i < count; i++) {
		func->mFixedLocalKeys[i] = rStack->GetByteParser()->ReadString();
		func->mFixedLocalRef[i] = rStack->GetByteParser()->ReadByte() == 1;
		func->mFixedLocalTypes[i] = rStack->GetByteParser()->ReadInt();
		func->mFixedLocalVars[i] = NULL;
	}

	delete prms;

	rStack->Push(var);
}

void Agent::JumpPS(Stack* rStack)
{
	Variable var = *rStack->Pop();
	int pos = rStack->GetByteParser()->ReadInt();
	
	if(!var.GetVarIsTrue(var)) {
		rStack->GetByteParser()->SetPosition(pos);
	}
}

void Agent::Jump(Stack* rStack)
{
	rStack->GetByteParser()->SetPosition(rStack->GetByteParser()->ReadInt());
}

void Agent::Get(Stack* rStack)
{
	rStack->Push(rStack->GetFunction()->GetVariable(rStack->GetByteParser()->ReadString()));
}

void Agent::GetLenght(Stack* rStack)
{
	Variable* var = rStack->GetFunction()->mFixedLocalVars[rStack->GetByteParser()->ReadInt()];
	rStack->Push(var != NULL ? var : rStack->GetParser()->GetUndefinedVar());
}

void Agent::StringTable(Stack* rStack)
{
	int newPos = rStack->GetByteParser()->ReadInt();
	int oldPos = rStack->GetByteParser()->GetPosition();

	rStack->GetByteParser()->SetPosition(newPos);

	int index = rStack->GetByteParser()->ReadShort();
	rStack->GetFunction()->mStringTableCount = index;
	rStack->GetFunction()->mStringTableVars = new Variable*[index];
	rStack->GetFunction()->mStringTable = new MCString[index];

	for(int i = 0; i < index; i++) {
		int lenght = rStack->GetByteParser()->ReadShort();
		byte* buffer = rStack->GetByteParser()->ReadBytes(lenght);

		char* dest = new char[lenght + 1];
		memcpy(dest, buffer, lenght);
		dest[lenght] = 0;
		
		rStack->GetFunction()->mStringTable[i].mBuffer = dest;
		rStack->GetFunction()->mStringTableVars[i] = rStack->GetParser()->GetGarbageCollector().RegisterVariable(gParse::Instance()->ToVar(rStack->GetParser(), MCString(dest)));
		
		AK_REFERENCE_INCREASE(rStack->GetFunction()->mStringTableVars[i]);
		delete[] buffer;
	}

	rStack->GetByteParser()->SetPosition(oldPos);
	rStack->GetByteParser()->SetStringTable(rStack->GetFunction()->mStringTable);

}

void Agent::CurrentLine(Stack* rStack)
{
	rStack->GetFunction()->mSouceFileLine = rStack->GetByteParser()->ReadInt();
	// TODO: Enable Debugger Mode with update to step through
	//rStack->GetParser()->GetDubug()->Update();
}

void Agent::GlobalAndLocals(Stack* rStack)
{
	int newPos = rStack->GetByteParser()->ReadInt();
	int oldPos = rStack->GetByteParser()->GetPosition();

	rStack->GetByteParser()->SetPosition(newPos);

	int index = rStack->GetByteParser()->ReadInt();
	rStack->GetFunction()->mFixedLocalCount = index;
	rStack->GetFunction()->mFixedLocalVars = new Variable*[index];
	rStack->GetFunction()->mFixedLocalKeys = new MCString[index];

	for(int i = 0; i < index; i++) {
		MCString key = rStack->GetByteParser()->ReadString();
		
		rStack->GetFunction()->mFixedLocalKeys[i] = key;
		rStack->GetFunction()->mFixedLocalVars[i] = NULL;
	}

	rStack->GetByteParser()->SetPosition(oldPos);
}

void Agent::AddNumber(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	rRet.SetType(Variable::NUMBER);
	rRet.SetData(gParse::Instance()->NumberToPtr(*(double*)rVar0->GetData() + *(double*)rVar1->GetData()));
}

void Agent::AddString(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	rRet.SetType(Variable::STRING);
	rRet.SetData(gParse::Instance()->StringToPtr(*(MCString*)rVar1->GetData() + *(MCString*)rVar0->GetData()));	
}

void Agent::AddNumberToString(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	rRet.SetType(Variable::STRING);
	rRet.SetData(gParse::Instance()->StringToPtr(*(MCString*)rVar1->GetData() + MCString(*(double*)rVar0->GetData())));
	
}

void Agent::AddStringToNumber(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	rRet.SetType(Variable::STRING);
	rRet.SetData(gParse::Instance()->StringToPtr(MCString::Format("%f%s", *(double*)rVar0->GetData(), ((MCString*)rVar1->GetData())->mBuffer)));
}

void Agent::DivideString(ArgParser* rParser, Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	MCString str = *(MCString*)rVar0->GetData();
	Table* table = new Table(rParser);

	int lenght = str.Length();
	int strip = (int)(lenght / *(double*)rVar1->GetData());

	rRet.SetType(Variable::TABLE);
	rRet.SetData(table);

	if(strip <= 0) {
		return;
	}

	for(int i = 0; i < lenght; i += strip) {
		table->Set(MCString((double)i), gParse::Instance()->ToVar(rParser, str.Substring(i, lenght - 1 < strip ? lenght - i : strip)));
	}
}

void Agent::MultiplyString(ArgParser* rParser, Variable& rRet, const Variable* rNumber, const Variable* rStr)
{
	MCString strAdd(*(MCString*)rStr->GetData());
	MCString* str = new MCString();
	int lenght = (int)*(double*)rNumber->GetData();

	if(lenght <= 0) {
		rRet.SetType(Variable::STRING);
		rRet.SetData(gParse::Instance()->StringToPtr(""));
		return;
	}

	int size = strAdd.Length();
	int max = lenght * size + (int)((*(double*)rNumber->GetData() - lenght) * size);
	char* ptr = new char[max + 1];

	str->mBuffer = ptr;

	for(int i = 0; i < lenght; i++) {
		memcpy(ptr + i * size, strAdd.mBuffer, size);
	}

	if(*(double*)rNumber->GetData() - lenght > 0) {
		memcpy(ptr + lenght * size, strAdd.mBuffer, (int)((*(double*)rNumber->GetData() - lenght) * size));
	}

	ptr[max] = NULL;

	rRet.SetType(Variable::NUMBER);
	rRet.SetData(str);
}

// TODO: Make it 1 function instead of 2 separate functions
void Agent::Add(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	if(rVar0->GetType() == Variable::NUMBER) {
		if(rVar1->GetType() == Variable::NUMBER) { AddNumber(rRet, rVar0, rVar1); }
		if(rVar1->GetType() == Variable::STRING) { AddStringToNumber(rRet, rVar0, rVar1); }
	}

	if(rVar0->GetType() == Variable::STRING) {
		if(rVar1->GetType() == Variable::NUMBER) { AddString(rRet, rVar0, rVar1); }
		if(rVar1->GetType() == Variable::STRING) { AddNumberToString(rRet, rVar1, rVar0); }
	}
}

void Agent::Substract(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	if(rVar0->GetType() == Variable::NUMBER && rVar1->GetType() == Variable::NUMBER) {		
		rRet.SetType(Variable::NUMBER);
		rRet.SetData(gParse::Instance()->NumberToPtr(*(double*)rVar0->GetData() - *(double*)rVar1->GetData()));
	}
}

void Agent::Divide(ArgParser* rParser, Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	if(rVar0->GetType() == Variable::NUMBER && rVar1->GetType() == Variable::NUMBER) {		
		rRet.SetType(Variable::NUMBER);
		rRet.SetData(gParse::Instance()->NumberToPtr(*(double*)rVar0->GetData() / *(double*)rVar1->GetData()));
	}

	if(rVar0->GetType() == Variable::STRING && rVar1->GetType() == Variable::NUMBER) {
		DivideString(rParser, rRet, rVar0, rVar1);
	}
}

void Agent::Multiply(ArgParser* rParser, Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	if(rVar0->GetType() == Variable::NUMBER) {
		if(rVar1->GetType() == Variable::NUMBER) {
			rRet.SetType(Variable::NUMBER);
			rRet.SetData(gParse::Instance()->NumberToPtr(*(double*)rVar0->GetData() * *(double*)rVar1->GetData()));
		}

		if(rVar1->GetType() == Variable::STRING) {
			MultiplyString(rParser, rRet, rVar0, rVar1);
		}			
	}

	if(rVar0->GetType() == Variable::STRING && rVar1->GetType() == Variable::NUMBER) {
		MultiplyString(rParser,rRet, rVar1, rVar0);
	}	
}

void Agent::Equal(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	rRet.SetType(Variable::BOOL);

	if(rVar0->GetType() == Variable::BOOL && rVar1->GetType() == Variable::UNDEFINED && rVar0->GetData() == NULL) {
		rRet.SetData(gParse::Instance()->BoolToPtr(true));
	}
	else if(rVar1->GetType() == Variable::BOOL && rVar0->GetType() == Variable::UNDEFINED && rVar1->GetData() != NULL) {
		rRet.SetData(gParse::Instance()->BoolToPtr(true));
	}
	else if(rVar0->GetType() != rVar1->GetType()){
		rRet.SetData(gParse::Instance()->BoolToPtr(false));
	}
	else if(rVar0->GetType() == Variable::BOOL){
		rRet.SetData(gParse::Instance()->BoolToPtr(rVar0->GetData() == rVar1->GetData()));
	}
	else if(rVar0->GetType() == Variable::NUMBER){
		rRet.SetData(gParse::Instance()->BoolToPtr(*(double*)rVar0->GetData() == *(double*)rVar1->GetData()));
	}
	else if(rVar0->GetType() == Variable::STRING){
		rRet.SetData(gParse::Instance()->BoolToPtr(*(MCString*)rVar0->GetData() == *(MCString*)rVar1->GetData()));
	}
	else {
		rRet.SetData(gParse::Instance()->BoolToPtr(rVar0->GetData() == rVar1->GetData()));	
	}
}

void Agent::GreaterThan(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	rRet.SetType(Variable::BOOL);
	if(rVar0->GetType() != rVar1->GetType() || rVar0->GetType() != Variable::NUMBER) {
		rRet.SetData(gParse::Instance()->BoolToPtr(false));
	}
	else {
		rRet.SetData(gParse::Instance()->BoolToPtr(*(double*)rVar0->GetData() >= *(double*)rVar1->GetData));
	}
}

void Agent::GreaterOrEqual(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	rRet.SetType(Variable::BOOL);
	if(rVar0->GetType() != rVar1->GetType() || rVar0->GetType() != Variable::NUMBER) {
		rRet.SetData(gParse::Instance()->BoolToPtr(false));
	}
	else {
		rRet.SetData(gParse::Instance()->BoolToPtr(*(double*)rVar0->GetData() >= *(double*)rVar1->GetData()));
	}
}

void Agent::LessThan(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	rRet.SetType(Variable::BOOL);
	if(rVar0->GetType() != rVar1->GetType() || rVar0->GetType() != Variable::NUMBER) {
		rRet.SetData(gParse::Instance()->BoolToPtr(false));
	}
	else {
		rRet.SetData(gParse::Instance()->BoolToPtr(*(double*)rVar0->GetData() < *(double*)rVar1->GetData()));
	}

}

void Agent::LessOrEqual(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	rRet.SetType(Variable::BOOL);
	if(rVar0->GetType() != rVar1->GetType() || rVar0->GetType() != Variable::NUMBER) {
		rRet.SetData(gParse::Instance()->BoolToPtr(false));
	}
	else {
		rRet.SetData(gParse::Instance()->BoolToPtr(*(double*)rVar0->GetData() <= *(double*)rVar1->GetData()));
	}
}

void Agent::Modulo(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	if(rVar0->GetType() != Variable::NUMBER && rVar1->GetType() == Variable::NUMBER) {
		rRet.SetType(Variable::NUMBER);
		rRet.SetData(gParse::Instance()->NumberToPtr(fmod((double)*(double*)rVar0->GetData(), *(double*)rVar1->GetData())));
	}

	if(rVar0->GetType() == Variable::STRING && rVar1->GetType() == Variable::TABLE) {
		rRet.SetType(Variable::STRING);

		Table* table = rVar1->GetTable();
		MCString str;

		char* offset = ((MCString*)rVar1->GetData())->mBuffer;
		char* offsetOld = offset;

		int pos = table->GetNextIndex(0);
		do {
			offset = strstr(offset, "%");

			if(offset != NULL) {
				if(offsetOld != offset && offset[-1] == '\\') {
					offset++;
				} 
				else {
					if(pos == -1) {
						throw InvalidIndexException("Table contains not enough indexes to replace all % characters");
					}

					str.AppendToBuffer(offsetOld, offset - offsetOld);
					str += table->GetByIndex(pos)->ToString();

					offset++;
					offsetOld = offset;

					pos = table->GetNextIndex(++pos);
				}
			}
			else {
				str.AppendToBuffer(offsetOld, strlen(offsetOld));
			}

		} while(offset != NULL);

		rRet.SetData(gParse::Instance()->StringToPtr(str.Replace("\\%", "%")));
	}
}

void Agent::And(Variable& rRet, Variable* rVar0, Variable* rVar1)
{
	rRet.SetType(Variable::BOOL);
	rRet.SetData(gParse::Instance()->BoolToPtr(rRet.GetVarIsTrue(*rVar0) && rRet.GetVarIsTrue(*rVar1)));
}

void Agent::Or(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	rRet.SetType(Variable::BOOL);
	rRet.SetData(gParse::Instance()->BoolToPtr(rRet.GetVarIsTrue(*rVar0) || rRet.GetVarIsTrue(*rVar1)));
}

void Agent::BitwiseLeft(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	if(rVar0->GetType() == Variable::NUMBER && rVar1->GetType() == Variable::NUMBER) {
		rRet.SetType(Variable::NUMBER);
		rRet.SetData(gParse::Instance()->NumberToPtr((int)* (double*)rVar0->GetData() << (int)* (double*)rVar1->GetData));
	}
}

void Agent::BitwiseRight(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	if(rVar0->GetType() == Variable::NUMBER && rVar1->GetType() == Variable::NUMBER) {
		rRet.SetType(Variable::NUMBER);
		rRet.SetData(gParse::Instance()->NumberToPtr((int)* (double*)rVar0->GetData() >> (int)* (double*)rVar1->GetData));
	}
}

void Agent::BitwiseOr(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	if(rVar0->GetType() == Variable::NUMBER && rVar1->GetType() == Variable::NUMBER) {
		rRet.SetType(Variable::NUMBER);
		rRet.SetData(gParse::Instance()->NumberToPtr((int)* (double*)rVar0->GetData() | (int)* (double*)rVar1->GetData));
	}
}

void Agent::BitwiseAnd(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	if(rVar0->GetType() == Variable::NUMBER && rVar1->GetType() == Variable::NUMBER) {
		rRet.SetType(Variable::NUMBER);
		rRet.SetData(gParse::Instance()->NumberToPtr((int)* (double*)rVar0->GetData() & (int)* (double*)rVar1->GetData));
	}
}

void Agent::NotEqual(Variable& rRet, const Variable* rVar0, const Variable* rVar1)
{
	rRet.SetData(gParse::Instance()->BoolToPtr(rRet.GetData() == NULL));
}