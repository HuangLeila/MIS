#include"Path.h"
#include"SubContract.h"

int Path::_count = 0;
Path::Path()
{
	_id = _count;
	_count++;
}

Path::Path(SubContract * subContract) :
	_subContract(subContract)
{
	_id = _count;
	_count++;
}

Path::Path(SubContract * subContract, vector<int> dayList) :
	_subContract(subContract), _dayList(dayList)
{
	_id = _count;
	_count++;
}

Path * Path::copyPath(vector<int> dayList)
{
	Path * newPath = new Path();
	newPath->setSubContract(_subContract);
	newPath->setDayList(dayList);

	return newPath;
}


bool Path::produceContain(Machine * machine, int t)
{
	vector<Machine *> machineList = _subContract->getMachineList();
	for (int m = 0; m < machineList.size(); m++)
	{
		if (machineList[m] == machine && _dayList[m] == t)
		{
			return true;
		}
	}
	return false;
}
bool Path::inventoryContain(Machine * machine, int t)
{
	/*昨天在前序机器上生产，今天作为当前机器的库存*/
	vector<Machine *> machineList = _subContract->getMachineList();
	for (int m = 0; m < machineList.size(); m++)
	{
		if (machineList[m] == machine)
		{
			if (m > 0 && _dayList[m - 1] == t - 1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

void Path::print()
{
	cout << "#Path" << _id;
	cout << "\tSubContract: " << _subContract->getId();
	cout << "\n #DayList:";
	for (int i = 0; i < _dayList.size(); i++)
	{
		cout << " " << _dayList[i];
	}
	cout << "\n-------------------------------------------------------------------------------------------------\n";
}