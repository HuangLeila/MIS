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

	/*ÿ��·���Ĳ���*/
	NumVar2Matrix _x_ip;

	/*ÿ̨������ͬ����ÿ����Ķ��⹩��*/
	NumVar3Matrix _y_mct;

	/*ÿ̨������ͬ����ÿ�����ku��棬t=0ʱ�Ŀ��Ϊ��ʼ���*/
	NumVar3Matrix _z_mct;

	/*ÿ����ͬǷ��*/
	IloNumVarArray _under_i;

	/*ÿ����ͬ�Ĳ���Ҫ��*/
	IloRangeArray _demand_i;

	/*ÿ̨����ÿ��Ĳ���Լ��*/
	IloRangeArray _capacity_mt;

	/*ÿ̨����ÿ������ÿ����ƽ��*/
	IloRangeArray _blc_mct;

	/*ÿ̨����ÿ���ܿ��ƽ��*/
	IloRangeArray _safeKeep_mt;

public:
	Model(Schedule * schedule);

	void init();
	void solveLP();
	void output();
};
