#include"SubContract.h"
#include"Contract.h"
#include"Material.h"
#include"Machine.h"

int SubContract::_count = 0;
SubContract::SubContract(vector<Machine *> machineList, double weight, Material * material, Contract * contract) :
	_machineList(machineList), _weight(weight), _material(material), _contract(contract)
{
	_id = _count;
	_count++;
}

bool SubContract::contain(Machine * machine)
{
	for (int m = 0; m < _machineList.size(); m++)
	{
		if (machine == _machineList[m])
		{
			return true;
		}
	}
	return false;
}

void SubContract::print()
{
	cout << "#SubContract" << _id;
	cout << "\tContract: " << _contract->getName() << "(" << _contract->getId() << ")";
	cout << "\tMaterial: " << _material->getName() << "(" << _material->getId() << ")";
	cout << "\tWeight: " << _weight;
	cout << "\n #MachineList:";
	for (int m = 0; m < _machineList.size(); m++)
	{
		cout << " " << _machineList[m]->getName() << "(" << _machineList[m]->getId() << ")";
	}
	cout << "\n-----------------------------------------------------------------------------------------\n";
}