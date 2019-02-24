#pragma once
#include"Util.h"

class Path;
class Machine;
class Material;
class Contract;
class SubContract
{
private:
	int _id;
	static int _count;
	vector<Machine *> _machineList;
	double _weight;
	Material * _material;
	Contract * _contract;
	vector<Path *> _pathList;

	double _underWeight;

public:
	SubContract(vector<Machine *> machineList, double weight, Material * material, Contract * contract);

	void setId(int id) { _id = id; }
	int getId() { return _id; }

	void setMachineList(vector<Machine *> machineList) { _machineList = machineList; }
	vector<Machine *> getMachineList() { return _machineList; }

	void setWeigth(double weight) { _weight = weight; }
	double getWeight() { return _weight; }

	void setMaterial(Material * material) { _material = material; }
	Material * getMaterial() { return _material; }

	void setContract(Contract * contract) { _contract = contract; }
	Contract * getContract() { return _contract; }

	void setPathList(vector<Path *> pathList) { _pathList = pathList; }
	void pushPath(Path * path) { _pathList.push_back(path); }
	vector<Path *> getPathList() { return _pathList; }

	void setUnderWeight(double underWeight) { _underWeight = underWeight; }
	double getUnderWeight() { return _underWeight; }

	bool contain(Machine * machine);

	void print();

};
