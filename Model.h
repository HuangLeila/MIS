#pragma once
#include"Schedule.h"

typedef IloArray<IloArray<IloNumVarArray>> NumVar3Matrix;
typedef IloArray<IloArray<IloNumArray>> Num3Matrix;

typedef IloArray<IloNumVarArray> NumVar2Matrix;
typedef IloArray<IloNumArray> Num2Matrix;

class Model
{
private:
	vector<Contract *> _contractList;
	vector<Machine *> _fullMachineList;
	vector<Machine *> _machineList;
	vector<Material *> _materialList;
	vector<vector<Path *>> _pathMatrix;
	vector<SubContract *> _subContractList;

	int _maxDayNum;

	IloEnv _env;
	IloCplex _solver;
	IloObjective _obj;
	IloModel _model;

	/*每条路径的产量*/
	NumVar2Matrix _x_ip;

	/*每台机器不同材料每天初的额外供料*/
	NumVar3Matrix _y_mct;

	/*每台机器不同材料每天初的ku库存，t=0时的库存为初始库存*/
	NumVar3Matrix _z_mct;

	/*每个合同欠量*/
	IloNumVarArray _under_i;

	/*每个合同的产量要求*/
	IloRangeArray _demand_i;

	/*每台机器每天的产能约束*/
	IloRangeArray _capacity_mt;

	/*每台机器每个材料每天库存平衡*/
	IloRangeArray _blc_mct;

	/*每台机器每天总库存平衡*/
	IloRangeArray _safeKeep_mt;

public:
	Model(Schedule * schedule);

	void init();
	void solveLP();
	void output();
};
