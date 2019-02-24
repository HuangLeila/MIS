#pragma once
#include"Util.h"
#include"Contract.h"
#include"Machine.h"
#include"Material.h"
#include"Path.h"
#include"SubContract.h"

class Schedule
{
private:
	vector<Contract *> _contractList;
	vector<Machine *> _machineList;
	vector<Material *> _materialList;
	vector<vector<Path *>> _pathMatrixs;
	vector<SubContract *> _subContractList;

public:
	Schedule();

	void splitContracts();
	void calculateInitInventory();

	void setContractList(vector<Contract *> contractList) { _contractList = contractList; }
	vector<Contract *> getContractList() { return _contractList; }

	void setMachineList(vector<Machine *> machineList) { _machineList = machineList; }
	vector<Machine *> getMachineList() { return _machineList; }

	void setMaterialList(vector<Material *> materialLsit) { _materialList = materialLsit; }
	vector<Material *> getMaterialList() { return _materialList; }

	void setPathMatrix(vector<vector<Path *>> pathMatrix) { _pathMatrixs = pathMatrix; }
	vector<vector<Path *>> getPathMatrix() { return _pathMatrixs; }
	vector<Path *> getPathList(int id) { return _pathMatrixs[id]; }

	void setSubContractList(vector<SubContract *> subContractList) { _subContractList = subContractList; }
	vector<SubContract *> getSubContractList() { return _subContractList; }

	void generatePathList();

	static void genCombineList(vector<vector<int> > & combineLists, int totalNum, int lowerBound, int upperBound);
	static void genCombineList(vector<vector<int> > & combineLists, int totalNum, int maxNum);

};