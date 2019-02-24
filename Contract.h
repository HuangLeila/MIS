#pragma once
#include"Util.h"

class Machine;
class SubContract;
class Material;
class Contract
{
private:
	int _id;
	static int _count;
	string _name;//����
	vector<Machine *> _machineList;//����
	vector<double> _weightList;//Ƿ��
	Material * _material;//�������
	vector<SubContract *> _subContractList;//�Ӻ�ͬ�������ٲ�
	vector<double> _inventoryList;//���
	vector<double> _planList;//�ƻ���
	vector<double> _lockList;//������

	vector<double> _underWeightList;

public:
	Contract(string name, Material * material);

	void setId(int id) { _id = id; }
	int getId() { return _id; }

	void setName(string name) { _name = name; }
	string getName() { return _name; }

	void setMachineList(vector<Machine *> machineList) { _machineList = machineList; }
	void pushMachine(Machine * machine) { _machineList.push_back(machine); }
	vector<Machine *> getMachineList() { return _machineList; }

	void setWeightList(vector<double> weightList) { _weightList = weightList; }
	void pushWeight(double weight) { _weightList.push_back(weight); }
	vector<double> getWeightList() { return _weightList; }

	void setInventoryList(vector<double> inventoryList) { _inventoryList = inventoryList; }
	void pushInventory(double inventory) { _inventoryList.push_back(inventory); }
	vector<double> getInventoryList() { return _inventoryList; }

	void setPlanList(vector<double> planList) { _planList = planList; }
	void pushPlan(double plan) { _planList.push_back(plan); }
	vector<double> getPlanList() { return _planList; }

	void setLockList(vector<double> lockList) { _lockList = lockList; }
	void pushLock(double lock) { _lockList.push_back(lock); }
	vector<double> getLockList() { return _lockList; }

	void setMaterial(Material * material) { _material = material; }
	Material * getMaterial() { return _material; }

	void setSubContractList(vector<SubContract *> subContractList) { _subContractList = subContractList; }
	void pushSubContract(SubContract * subContract) { _subContractList.push_back(subContract); }
	vector<SubContract *> getSubContractList() { return _subContractList; }

	void calcuUnderWeightList();
	vector<double> getUnderWeightList() { return _underWeightList; }

	void print();

};
