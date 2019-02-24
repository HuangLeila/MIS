#pragma once
#include"Util.h"

class Contract;
class Material
{
private:
	int _id;
	static int _count;
	string _name;
	Contract * _contract;

public:
	Material(string name);

	void setId(int id) { _id = id; }
	int getId() { return _id; }

	void setName(string name) { _name = name; }
	string getName() { return _name; }

	void setContract(Contract * contract) { _contract = contract; }
	Contract * getContract() { return _contract; }

	void print();
};
