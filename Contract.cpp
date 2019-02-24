#include"Contract.h"
#include"Material.h"
#include"Machine.h"
#include"SubContract.h"

int Contract::_count = 0;
Contract::Contract(string name, Material * material) :
	_name(name), _material(material)
{
	_id = _count;
	_count++;
}

void Contract::calcuUnderWeightList()
{
	_underWeightList.resize(_machineList.size());
	for (int m = 0; m < _machineList.size(); m++)
	{
		if (!_machineList[m]->getIsColdRoll())
		{
			_underWeightList[m] = 0;
		}
		else
		{
			_underWeightList[m] = 0;
			for (int i = 0; i < _subContractList.size(); i++)
			{
				if (_subContractList[i]->contain(_machineList[m]))
				{
					_underWeightList[m] += _subContractList[i]->getUnderWeight();
				}
			}
		}
	}
}

void Contract::print()
{
	cout << "#Contract" << _id;
	cout << "\tName: " << _name;
	cout << "\tMaterial: " << _material->getName() << "(" << _material->getId() << ")";
	cout << "\n #MachineList:";
	for (int m = 0; m < _machineList.size(); m++)
	{
		cout << " " << _machineList[m]->getName() << "(" << _machineList[m]->getId() << ")";
	}
	cout << "\n #PositionList:";
	for (int m = 0; m < _machineList.size(); m++)
	{
		cout << " " << _machineList[m]->getPosition();
	}
	cout << "\n #WeightList:";
	for (int i = 0; i < _weightList.size(); i++)
	{
		cout << " " << _weightList[i];
	}
	cout << "\n #InventoryList:";
	for (int i = 0; i < _inventoryList.size(); i++)
	{
		cout << " " << _inventoryList[i];
	}
	cout << "\n #PlanList:";
	for (int i = 0; i < _planList.size(); i++)
	{
		cout << " " << _planList[i];
	}
	cout << "\n #LockList:";
	for (int i = 0; i < _lockList.size(); i++)
	{
		cout << " " << _lockList[i];
	}
	cout << "\n-----------------------------------------------------------------------------------------\n";

}
