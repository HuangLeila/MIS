#include"Machine.h"
#include"Material.h"


int Machine::_count = 0;
Machine::Machine(string name, int position) :
	_name(name), _position(position)
{
	_id = _count;
	_count++;
	if (_position >= 8)
	{
		_isColdRoll = true;
	}
	else
	{
		_isColdRoll = false;
	}
}

void Machine::initSolnMatrix(vector<Material *> materialList, int maxDayNum)
{
	_solnInventoryMatrix.resize(materialList.size());
	_solnYieldMatrix.resize(materialList.size());
	_solnSupplyMatrix.resize(materialList.size());

	for (int c = 0; c < materialList.size(); c++)
	{
		_solnInventoryMatrix[c].resize(maxDayNum);
		_solnYieldMatrix[c].resize(maxDayNum);
		_solnSupplyMatrix[c].resize(maxDayNum);
		for (int t = 0; t < maxDayNum; t++)
		{
			_solnInventoryMatrix[c][t] = 0;
			_solnYieldMatrix[c][t] = 0;
			_solnSupplyMatrix[c][t] = 0;
		}
	}
}

double Machine::getSolnInventoryPerDay(int t)
{
	double totalInventory = 0;
	for (int c = 0; c < _solnInventoryMatrix.size(); c++)
	{
		totalInventory += _solnInventoryMatrix[c][t];
	}
	return totalInventory;
}

double Machine::getSolnYieldPerDay(int t)
{
	double totalYield = 0;
	for (int c = 0; c < _solnYieldMatrix.size(); c++)
	{
		totalYield += _solnYieldMatrix[c][t];
	}
	return totalYield;
}

double Machine::getSolnSupplyPerDay(int t)
{
	double totalSupply = 0;
	for (int c = 0; c < _solnSupplyMatrix.size(); c++)
	{
		totalSupply += _solnSupplyMatrix[c][t];
	}
	return totalSupply;
}

void Machine::print()
{
	cout << "#Machine" << _id;
	cout << "\tName: " << _name;
	cout << "\tPosition: " << _position;
	cout << "\tIsColdRoll: " << _isColdRoll;
	cout << "\tSafeInventory: " << _safeInventory;
	cout << "\n #InitInventoryList:";
	for (int i = 0; i < _initInventoryList.size(); i++)
	{
		cout << " " << _initInventoryList[i].second << "(" << _initInventoryList[i].first->getName() << ")";
	}
	cout << "\n #YieldList:";
	for (int i = 0; i < _yieldList.size(); i++)
	{
		cout << " " << _yieldList[i];
	}
	cout << "\n-----------------------------------------------------------------------------------------\n";

}