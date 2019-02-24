#pragma once
#include"Util.h"

class Machine;
class SubContract;
class Path
{
private:
	int _id;
	static int _count;
	int _useId;
	SubContract * _subContract;
	vector<int> _dayList;

	double _solnWeight;

public:
	Path();
	Path(SubContract * subContract);
	Path(SubContract * subContract, vector<int> dayList);

	void setId(int id) { _id = id; }
	int getId() { return _id; }

	void setUseId(int useId) { _useId = useId; }
	int getUseId() { return _useId; }

	void setSubContract(SubContract * subContract) { _subContract = subContract; }
	SubContract * getSubContract() { return _subContract; }

	void setDayList(vector<int> dayList) { _dayList = dayList; }
	vector<int> getDayList() { return _dayList; }

	Path * copyPath(vector<int> dayList);

	bool produceContain(Machine * machine, int t);
	bool inventoryContain(Machine * machine, int t);

	void setSolnWeight(double weight) { _solnWeight = weight; }
	double getSolnWeight() { return _solnWeight; }

	void print();
};
