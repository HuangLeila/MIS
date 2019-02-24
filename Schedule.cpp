#include"Schedule.h"
#include"Util.h"

Schedule::Schedule()
{

}

void Schedule::splitContracts()
{
	cout << "#Begin split contracts\n";
	/*Contract * contract1 = _contractList[0];
	vector<Machine *> machineList1 = { contract1->getMachineList().back() };
	Material * material1 = contract1->getMaterial();
	SubContract * sub1 = new SubContract(machineList1, 670, material1, contract1);
	_subContractList.push_back(sub1);
	contract1->pushSubContract(sub1);

	Contract * contract2 = _contractList[1];
	vector<Machine *> machineList2 = { contract2->getMachineList()[4] };
	Material * material2 = contract2->getMaterial();
	SubContract * sub2 = new SubContract(machineList2, 80.073, material2, contract2);
	_subContractList.push_back(sub2);

	vector<Machine *> machineList3 = { contract2->getMachineList()[5] };
	SubContract * sub3 = new SubContract(machineList3, 70.703, material2, contract2);
	_subContractList.push_back(sub3);
	contract2->pushSubContract(sub2);
	contract2->pushSubContract(sub3);*/

	for (int i = 0; i < _contractList.size(); i++)
	{
		vector<Machine *> machines = _contractList[i]->getMachineList();
		Material * material = _contractList[i]->getMaterial();
		vector<double> weightList = _contractList[i]->getWeightList();
		for (int mm = 0; mm < machines.size(); mm++)
		{
			if (machines[mm]->getIsColdRoll())
			{
				vector<Machine *> subMachines = { machines[mm] };
				SubContract * newSub = new SubContract(subMachines, weightList[mm], material, _contractList[i]);
				_subContractList.push_back(newSub);
				_contractList[i]->pushSubContract(newSub);
			}
		}
	}

	cout << "The subcontracts are:\n";
	for (int i = 0; i < _subContractList.size(); i++)
	{
		_subContractList[i]->print();
	}
}

void Schedule::calculateInitInventory()
{
	cout << "#Begin calculate init inventory\n";
	for (int i = 0; i < _contractList.size(); i++)
	{
		vector<Machine *> machines = _contractList[i]->getMachineList();
		Material * material = _contractList[i]->getMaterial();
		vector<double> inventoryList = _contractList[i]->getInventoryList();
		double coldInventory = 0;
		for (int m = 0; m < machines.size(); m++)
		{
			if (machines[m]->getPosition() == 5 || machines[m]->getPosition() == 6)
			{
				coldInventory += inventoryList[m];
			}
			else if (machines[m]->getPosition() == 8)
			{
				coldInventory += inventoryList[m];
				machines[m]->pushInitInventory(material, coldInventory);
			}
			else if (machines[m]->getPosition() > 8)
			{
				machines[m]->pushInitInventory(material, inventoryList[m]);
			}
		}
	}

	cout << "The detail detail machines are:\n";
	for (int m = 0; m < _machineList.size(); m++)
	{
		_machineList[m]->print();
	}
}

void Schedule::generatePathList()
{
	int maxDayNum = (Util::endTime - Util::bgnTime) / (ADAYTIME);
	int maxMachineNum = 0;
	for (int i = 0; i < _subContractList.size(); i++)
	{
		if (_subContractList[i]->getMachineList().size() > maxMachineNum)
		{
			maxMachineNum = _subContractList[i]->getMachineList().size();
		}
	}

	vector<vector<vector<int>>> combineMatrixs;
	for (int i = 1; i <= maxMachineNum; i++)
	{
		vector<vector<int>> combineLsit;
		genCombineList(combineLsit, maxDayNum, i, i);
		combineMatrixs.push_back(combineLsit);
	}

	for (int i = 0; i < _subContractList.size(); i++)
	{
		vector<Path *> pathList;
		int num = _subContractList[i]->getMachineList().size();
		vector<vector<int>> combineList = combineMatrixs[num - 1];

		for (int k = 0; k < combineList.size(); k++)
		{
			vector<int> dayList;
			for (int t = 0; t < maxDayNum; t++)
			{
				if (combineList[k][t] > 0)
				{
					dayList.push_back(t);
				}
			}
			Path * newPath = new Path(_subContractList[i], dayList);
			pathList.push_back(newPath);
			_subContractList[i]->pushPath(newPath);
		}
		_pathMatrixs.push_back(pathList);
	}

	for (int i = 0; i < _subContractList.size(); i++)
	{
		for (int p = 0; p < _pathMatrixs[i].size(); p++)
		{
			_pathMatrixs[i][p]->print();
		}
		cout << "\n";
	}
}

void Schedule::genCombineList(vector<vector<int>> & combineLists, int totalNum, int lowerBound, int upperBound)
{
	for (int i = lowerBound; i <= upperBound; i++)
	{
		genCombineList(combineLists, totalNum, i);
	}
}

void Schedule::genCombineList(vector<vector<int>> & combineLists, int totalNum, int maxNum)
{
	vector<int> combineList;
	combineList.resize(totalNum);
	int currCompNum = 0;
	int index = 0;
	while (true/*index <= _freeCompList.size() - maxCompNum + 1 || currCompNum > 0*/)
	{
		while (combineList[index] < 1 && currCompNum < maxNum)
		{
			combineList[index]++;
			currCompNum++;
			if (index < totalNum - 1)
			{
				index++;
			}
			else
			{
				break;
			}
		}

		vector<int> newCombineList = combineList;
		combineLists.push_back(newCombineList);

		if (combineList[totalNum - 1] > 0)
		{
			int currIndex = index;
			while (combineList[currIndex] > 0 && currIndex > totalNum - maxNum)
			{
				combineList[currIndex]--;
				currCompNum--;
				currIndex--;
			}
		}

		for (int i = index; i >= 0; i--)
		{
			if (combineList[i] > 0)
			{
				combineList[i]--;
				currCompNum--;
				index = i;
				break;
			}
		}
		index++;

		if (index == totalNum - maxNum + 1 && currCompNum == 0)
		{
			break;
		}
	}
}
