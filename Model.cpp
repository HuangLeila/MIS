#include"Model.h"
#include"IO.h"

Model::Model(Schedule *schedule)
{
	_contractList = schedule->getContractList();
	_fullMachineList = schedule->getMachineList();
	for (int m = 0; m < _fullMachineList.size(); ++m)
	{
		if (_fullMachineList[m]->getIsColdRoll())
		{
			_fullMachineList[m]->setUseId(_machineList.size());//	
			_machineList.push_back(_fullMachineList[m]);
		}
	}

	_materialList = schedule->getMaterialList();
	_subContractList = schedule->getSubContractList();

	_incompatibleMachineList = schedule->getIncompatibleMachineList();
	_switchPointList = schedule->getSwitchPointList();
	_BCmachineList = schedule->getBCmachineList();

	_maxDayNum = (Util::endTime - Util::bgnTime) / (ADAYTIME);

	_model = IloModel(_env);//
	_solver = IloCplex(_model);
	_obj = IloAdd(_model, IloMinimize(_env));//

}

void Model::init()
{
	cout << "Begin init model\n";
	

	//产量
	_x_mct = NumVar3Matrix(_env, _machineList.size());
	for (int m = 0; m < _machineList.size(); ++m)
	{
		_x_mct[m] = NumVar2Matrix(_env, _contractList.size());
		for (int c = 0; c < _contractList.size(); ++c)
		{
			_x_mct[m][c] = IloNumVarArray(_env);
			if (_contractList[c]->contain(_machineList[m]))
			{
				for (int t = 0; t < _maxDayNum; ++t)
				{
					string varName = "X_M_" + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
					_x_mct[m][c].add(IloNumVar(_env, 0, _machineList[m]->getYield(t), ILOFLOAT, varName.c_str()));
					cout << varName << endl;
				}
			}
			else
			{
				for (int t = 0; t < _maxDayNum; ++t)
				{
					string varName = "X_M_" + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
					_x_mct[m][c].add(IloNumVar(_env, 0, 0, ILOFLOAT, varName.c_str()));
					cout << varName << endl;
				}
			}
			_model.add(_x_mct[m][c]);
		}
	}

	//是否生产指定流向
	_p_mct = NumVar3Matrix(_env, _machineList.size());
	for (int m = 0; m < _machineList.size(); ++m)
	{
		_p_mct[m] = NumVar2Matrix(_env, _contractList.size());
		for (int c = 0; c < _contractList.size(); ++c)
		{
			_p_mct[m][c] = IloNumVarArray(_env);
			if (_contractList[c]->contain(_machineList[m]))
			{
				for (int t = 0; t < _maxDayNum; ++t)
				{
					string varName = "Produce_M_" + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
					_p_mct[m][c].add(IloNumVar(_env, 0, 1, ILOBOOL, varName.c_str()));
					cout << varName << endl;
				}
			}
			else
			{
				for (int t = 0; t < _maxDayNum; ++t)
				{
					string varName = "Produce_M_" + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
					_p_mct[m][c].add(IloNumVar(_env, 0, 0, ILOBOOL, varName.c_str()));
					cout << varName << endl;
				}
			}
			_model.add(_p_mct[m][c]);
		}
	}
	
	//是否生产
	_p_mt = NumVar2Matrix(_env, _machineList.size());
	for (int m = 0; m < _machineList.size(); ++m)
	{
		_p_mt[m] = IloNumVarArray(_env);
		for (int t = 0; t < _maxDayNum; ++t)
		{
			string varName = "Produce_M_" + to_string(m) + "_T_" + to_string(t);
			_p_mt[m].add(IloNumVar(_env, 0, 1, ILOBOOL, varName.c_str()));
			cout << varName << endl;
		}
		_model.add(_p_mt[m]);
	}
	for (int m = 0; m < _machineList.size(); ++m)
	{
		if (!_machineList[m]->getIsInIncmptList())
		{
			for (int t = 0; t < _maxDayNum; ++t)
			{
				_p_mt[m][t].setBounds(1, 1);
			}
		}
	}
	
	//外部供料
	_y_mct = NumVar3Matrix(_env, _machineList.size());
	for (int m = 0; m < _machineList.size(); ++m)
	{
		_y_mct[m] = NumVar2Matrix(_env, _contractList.size());
		for (int c = 0; c < _contractList.size(); ++c)
		{
			_y_mct[m][c] = IloNumVarArray(_env);
			if (_contractList[c]->contain(_machineList[m]))
			{
				for (int t = 0; t < _maxDayNum; ++t)
				{
					string varName = string("Y_M_") + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
					_y_mct[m][c].add(IloNumVar(_env, 0, _machineList[m]->getSafeInventory(), ILOFLOAT, varName.c_str()));
					cout << varName << endl;
				}
			}
			else
			{
				for (int t = 0; t < _maxDayNum; ++t)
				{
					string varName = string("Y_M_") + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
					_y_mct[m][c].add(IloNumVar(_env, 0, 0, ILOFLOAT, varName.c_str()));
					cout << varName << endl;
				}
			}
			
			_model.add(_y_mct[m][c]);
		}
	}

	//库存
	_z_mct = NumVar3Matrix(_env, _machineList.size());
	for (int m = 0; m < _machineList.size(); ++m)
	{
		_z_mct[m] = NumVar2Matrix(_env, _contractList.size());
		for (int c = 0; c < _contractList.size(); ++c)
		{
			_z_mct[m][c] = IloNumVarArray(_env);
			if (_contractList[c]->contain(_machineList[m]))
			{
				for (int t = 0; t < _maxDayNum; ++t)
				{
					string varName = string("Inventory_M_") + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
					//
					if (t > 0)
					{
						_z_mct[m][c].add(IloNumVar(_env, 0, _machineList[m]->getSafeInventory(), ILOFLOAT, varName.c_str()));
					}
					else
					{
						//
						_z_mct[m][c].add(IloNumVar(_env, 0, 0, ILOFLOAT, varName.c_str()));
					}
					cout << varName << endl;
				}
			}
			else
			{
				for (int t = 0; t < _maxDayNum; ++t)
				{
					string varName = string("Inventory_M_") + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
					//
					_z_mct[m][c].add(IloNumVar(_env, 0, 0, ILOFLOAT, varName.c_str()));
					cout << varName << endl;
				}
			}
			_model.add(_z_mct[m][c]);
		}
		//
		vector<pair<Contract *, double> > initInventory = _machineList[m]->getInitInventory();
		for (int index = 0; index < initInventory.size(); ++index)
		{
			_z_mct[m][initInventory[index].first->getId()][0].setBounds(initInventory[index].second, initInventory[index].second);
		}
	}

	_under_mc = NumVar2Matrix(_env, _machineList.size());
	for (int m = 0; m < _machineList.size(); ++m)
	{
		_under_mc[m] = IloNumVarArray(_env);
		for (int c = 0; c < _contractList.size(); ++c)
		{
			string varName = "UnDmd_M_" + to_string(m) + "_C_" + to_string(c);
			_under_mc[m].add(IloNumVar(_env, 0, _contractList[c]->getWeight(_machineList[m]), ILOFLOAT, varName.c_str()));
			cout << varName << endl;
		}
		_model.add(_under_mc[m]);
	}
	
	_surplus_mc = NumVar2Matrix(_env,_machineList.size());
	for (int m = 0; m < _machineList.size(); ++m)
	{
		_surplus_mc[m] = IloNumVarArray(_env);
		for (int c = 0; c < _contractList.size(); ++c)
		{
			if (_contractList[c]->contain(_machineList[m]))
			{
				string varName = "Surp_M_" + to_string(m) + "_C_" + to_string(c);
				//upperBound->INFINITY?
				_surplus_mc[m].add(IloNumVar(_env, 0, INFINITY, ILOFLOAT, varName.c_str()));
				cout << varName << endl;
			}
			else
			{
				string varName = "Surp_M_" + to_string(m) + "_C_" + to_string(c);
				_surplus_mc[m].add(IloNumVar(_env, 0, 0, ILOFLOAT, varName.c_str()));
				cout << varName << endl;
			}
		}
		_model.add(_surplus_mc[m]);
	}

	//是否生产主供流向
	_pB_mt = NumVar2Matrix(_env, _BCmachineList.size());
	for (int m = 0; m < _BCmachineList.size(); ++m)
	{
		_pB_mt[m] = IloNumVarArray(_env);
		for (int t = 0; t < _maxDayNum; ++t)
		{
			string varName = "pB_M_" + to_string(_BCmachineList[m]->getUseId()) + "_T_" + to_string(t);
			_pB_mt[m].add(IloNumVar(_env, 0, 1, ILOBOOL, varName.c_str()));
			cout << varName << endl;
		}
		_model.add(_pB_mt[m]);
	}
	
	//m机组t天是否生产C
	_produce_mct = IloRangeArray(_env);
	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int c = 0; c < _contractList.size(); ++c)
		{
			for(int t = 0; t < _maxDayNum; ++t)
			{
				string consName = "ifProduce_M_" + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
				IloExpr expr(_env);
				expr += _p_mct[m][c][t] * BIGM;
				expr -= _x_mct[m][c][t];
				_produce_mct.add(IloRange(_env, 0, expr, INFINITY, consName.c_str()));
				cout << consName << endl;
			}
		}
	}
	_model.add(_produce_mct);

	//机组当日是否生产
	_produce_mt = IloRangeArray(_env);
	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int t = 0; t < _maxDayNum; ++t)
		{
			string consName = "ifProduce_M_" + to_string(m) + "_T_" + to_string(t);
			IloExpr expr(_env);
			for (int c = 0; c < _contractList.size(); ++c)
			{
				expr -= _p_mct[m][c][t];
			}
			expr += _p_mt[m][t] * BIGM;
			_produce_mt.add(IloRange(_env, 0, expr, INFINITY, consName.c_str()));
			cout << consName << endl;
		}
	}
	_model.add(_produce_mt);

	//拆分机组每日只有一个可以生产
	_producePattern = IloRangeArray(_env);
	for (int i = 0; i < _incompatibleMachineList.size(); ++i)
	{
		vector<Machine *> groupMachine = _incompatibleMachineList[i];
		for (int t = 0; t < _maxDayNum; ++t)
		{
			string consName = "ProducePattern_G_" + to_string(i) + "_T_" + to_string(t);
			IloExpr expr(_env);
			for (int m = 0; m < groupMachine.size(); ++m)
			{
				expr += _p_mt[groupMachine[m]->getUseId()][t];
			}
			_producePattern.add(IloRange(_env, 1, expr, 1, consName.c_str()));
			cout << consName << endl;
		}		
	}
	_model.add(_producePattern);

	//是否生产主供模式
	_produceB_mt = IloRangeArray(_env);
	for (int m = 0; m < _BCmachineList.size(); ++m)
	{
		for (int t = 0; t < _maxDayNum; ++t)
		{
			string consName = "ifProduceB_M_" + to_string(_BCmachineList[m]->getUseId()) + "_T_" + to_string(t);
			IloExpr expr(_env);
			vector<Machine *> Bmachines = _BCmachineList[m]->getBMachineList();
			for (int mm = 0; mm < Bmachines.size(); ++mm)
			{
				Contract *contract = new Contract();
				if (_BCmachineList[m]->getContract(Bmachines[mm], contract))
				{
					contract = _BCmachineList[m]->getCon(Bmachines[mm]);
					expr -= _p_mct[Bmachines[mm]->getUseId()][contract->getId()][t];
				}
			}
			expr += _pB_mt[m][t] * BIGM;
			_produceB_mt.add(IloRange(_env, 0, expr, INFINITY, consName.c_str()));
			cout << consName << endl;
		}
	}
	_model.add(_produceB_mt);

	//满足需求
	_demand_mc = IloRangeArray(_env);
	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int c = 0; c < _contractList.size(); ++c)
		{
			string consName = "Dmd_M_" + to_string(m) + "_C_" + to_string(c);
			IloExpr expr(_env);
			//the upper bound of the day should be the due date
			for (int t = 0; t < _maxDayNum; ++t)
			{
				expr += _x_mct[m][c][t];
			}
			expr += _under_mc[m][c];
			expr -= _surplus_mc[m][c];
			_demand_mc.add(IloRange(_env, _contractList[c]->getWeight(_machineList[m]), 
				expr, _contractList[c]->getWeight(_machineList[m]), consName.c_str()));
			cout << consName << endl;
		}
	}
	_model.add(_demand_mc);
	
	//机组每日必须满产
	_capacity_mt = IloRangeArray(_env);
	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int t = 1; t < _maxDayNum; ++t)
		{
			string consName = "Cap_M_" + to_string(m) + "_T_" + to_string(t);
			IloExpr expr(_env);
			//对于拆分机组如果生产必须满产
			expr += _p_mt[m][t] * _machineList[m]->getYield(t);
			for (int c = 0; c < _contractList.size(); ++c)
			{
				expr -= _x_mct[m][c][t];
			}
			_capacity_mt.add(IloRange(_env, 0, expr, 0, consName.c_str()));
			cout << consName << endl;	
		}
	}
	_model.add(_capacity_mt);

	//库存平衡
	_blc_mct = IloRangeArray(_env);
	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int c = 0; c < _contractList.size(); ++c)
		{//for(int t=0;t<_maxDayNum;++t)
			for (int t = 1; t < _maxDayNum; ++t)
			{
				string consName = "Blc_M_" + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
				IloExpr expr(_env);
				vector<pair<Machine *,Contract *> > preMachineList = _machineList[m]->getPreorderMachineList();
				if (!preMachineList.empty())
				{
					for (int mm = 0; mm < preMachineList.size(); ++mm)
					{
						if (_contractList[c]->getName() == preMachineList[mm].second->getName())
						{
							int index = preMachineList[mm].first->getUseId();
							cout << index << endl;
							expr += _x_mct[index][c][t - 1];
						}
					}
				}
				expr += _y_mct[m][c][t - 1];
				expr -= _x_mct[m][c][t - 1];
				expr += _z_mct[m][c][t - 1];
				expr -= _z_mct[m][c][t];

				_blc_mct.add(IloRange(_env, 0, expr, 0, consName.c_str()));
				cout << consName << endl;
			}
		}
	}
	_model.add(_blc_mct);

	//库存上下限
	_safeKeep_mt = IloRangeArray(_env);
	for (int m = 0; m < _machineList.size(); ++m)
	{//for(int t=0;t<_maxDayNum;++t
		for (int t = 1; t < _maxDayNum; ++t)
		{
			string consName = "SafeKeep_M_" + to_string(m) + "_T_" + to_string(t);
			IloExpr expr(_env);
			for (int c = 0; c < _contractList.size(); ++c)
			{
				expr += _z_mct[m][c][t];
			}
			//
			_safeKeep_mt.add(IloRange(_env, (_machineList[m]->getSafeInventory()*STOCKLOWERRANGE), expr, _machineList[m]->getSafeInventory()*STOCKUPPERRANGE, consName.c_str()));
			cout << consName << endl;
		}
	}
	_model.add(_safeKeep_mt);

	_switch = IloRangeArray(_env);
	for (int s = 0; s < _switchPointList.size(); ++s)
	{
		Machine *machine = _switchPointList[s]->getMachine();
		Contract *contract = _switchPointList[s]->getContract();
		double weight = _switchPointList[s]->getWeight();

		//切换后有欠量要求的切换点，例如C008，C122机组
		if (weight > 0)
		{
			string consName = "Switch_SP_" + to_string(s);
			IloExpr expr(_env);
			for (int t = _switchPointList[s]->getDayIndex(); t < _switchPointList[s]->getEndDay(); ++t)
			{
				expr += _x_mct[machine->getUseId()][contract->getId()][t];
			}
			//除了规定的生产期间，其余时间不可生产该流向
			for (int t = 0; t < _switchPointList[s]->getDayIndex(); ++t)
			{
				_p_mct[machine->getUseId()][contract->getId()][t].setBounds(0, 0);
			}
			for (int t = _switchPointList[s]->getEndDay(); t < _maxDayNum; ++t)
			{
				_p_mct[machine->getUseId()][contract->getId()][t].setBounds(0, 0);
			}
			_switch.add(IloRange(_env, weight, expr, INFINITY, consName.c_str()));
			cout << consName << endl;
		}
		//切换后没有产量要求的切换点，例如122机组，switch处输入的weight=0
		else
		{
			string consName = "Switch_SP_" + to_string(s);
			/*int indicator = 0;
			for (int t = _switchPointList[s]->getDayIndex(); t < _maxDayNum; ++t)
			{
				if (indicator == 0)
				{
					_p_mct[machine->getUseId()][contract->getId()][t].setBounds(1, 1);
					indicator += 1;
				}
				else
				{
					_p_mct[machine->getUseId()][contract->getId()][t].setBounds(0, 0);
					indicator -= 1;
				}
			}*/
			for (int t = _switchPointList[s]->getDayIndex(); t < _maxDayNum; ++t)
			{
				_p_mct[machine->getUseId()][contract->getId()][t].setBounds(1, 1);
			}
		}
	}
	_model.add(_switch);

	//产量小于库存
	_produceInvtry_mct = IloRangeArray(_env);
	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int c = 0; c < _contractList.size(); ++c)
		{
			for (int t = 0; t < _maxDayNum; ++t)
			{
				string consName = "ProduceInvtry_M_" + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
				IloExpr expr(_env);
				expr += _z_mct[m][c][t];
				expr -= _x_mct[m][c][t];
				_produceInvtry_mct.add(IloRange(_env, 0, expr, INFINITY, consName.c_str()));
				cout << consName << endl;
			}
		}
	}
	_model.add(_produceInvtry_mct);

	_delayTime = IloRangeArray(_env);
	for (int m = 0; m < _machineList.size(); ++m)
	{
		if (_machineList[m]->getIsCRM())
		{
			vector<Contract *> contractList = _machineList[m]->getContractList();
			for (int i = 0; i < contractList.size(); ++i)
			{
				string consName = "DelayTm_M_" + to_string(_machineList[m]->getUseId()) + "_C_" + to_string(contractList[i]->getId());
				IloExpr expr(_env);
				for (int t = 0; t < Util::HotRollDelayTm; ++t)
				{
					expr += _y_mct[_machineList[m]->getUseId()][contractList[i]->getId()][t];
				}
				_delayTime.add(IloRange(_env, 0, expr, 0, consName.c_str()));

				expr.clear();

				if (Util::HotRollDelayTm + Util::SteelMakeDelayTm <= _maxDayNum)
				{
					string consName = "DelayTm_M_" + to_string(_machineList[m]->getUseId()) + "_C_" + to_string(contractList[i]->getId())+"HotRoll";
					for (int t = Util::HotRollDelayTm; t < Util::HotRollDelayTm + Util::SteelMakeDelayTm; ++t)
					{
						expr+= _y_mct[_machineList[m]->getUseId()][contractList[i]->getId()][t];
					}
					_delayTime.add(IloRange(_env, 0, expr, contractList[i]->getPreWeightList()[0], consName.c_str()));
				}

				expr.clear();

				if (Util::HotRollDelayTm + Util::SteelMakeDelayTm + Util::MaterialApplyDelayTm <= _maxDayNum)
				{
					string consName = "DelayTm_M_" + to_string(_machineList[m]->getUseId()) + "_C_" + to_string(contractList[i]->getId()) + "SteelMake";
					for (int t = Util::HotRollDelayTm+Util::SteelMakeDelayTm; t < Util::HotRollDelayTm + Util::SteelMakeDelayTm+Util::MaterialApplyDelayTm; ++t)
					{
						expr += _y_mct[_machineList[m]->getUseId()][contractList[i]->getId()][t];
					}
					_delayTime.add(IloRange(_env, 0, expr, contractList[i]->getPreWeightList()[1], consName.c_str()));
				}

				expr.clear();

				if (Util::HotRollDelayTm + Util::SteelMakeDelayTm + Util::MaterialApplyDelayTm < _maxDayNum)
				{
					string consName = "DelayTm_M_" + to_string(_machineList[m]->getUseId()) + "_C_" + to_string(contractList[i]->getId()) + "MaterialApply";
					for (int t = Util::HotRollDelayTm + Util::SteelMakeDelayTm + Util::MaterialApplyDelayTm; t < _maxDayNum; ++t)
					{
						expr += _y_mct[_machineList[m]->getUseId()][contractList[i]->getId()][t];
					}
					_delayTime.add(IloRange(_env, 0, expr, contractList[i]->getPreWeightList()[2], consName.c_str()));
				}
			}
		}
	}
	_model.add(_delayTime);

	_BC_mt = IloRangeArray(_env);
	for (int m = 0; m < _BCmachineList.size(); ++m)
	{
		for (int t = 0; t < _maxDayNum - 1; ++t)
		{
			IloExpr expr(_env);
			string consName = "BC_M_" + to_string(_BCmachineList[m]->getUseId()) + "_T_" + to_string(t + 1);
			vector<Machine *> machines = _BCmachineList[m]->getBMachineList();
			//
			Contract *contract = NULL;
			for (int mm = 0; mm < machines.size(); ++mm)
			{
				if (_BCmachineList[m]->getContract(machines[mm], contract))
				{
					contract = _BCmachineList[m]->getCon(machines[mm]);
					expr -= _z_mct[machines[mm]->getUseId()][contract->getId()][t];
				}
			}
			expr += _BCmachineList[m]->getBInventory();
			expr += (1 - _pB_mt[m][t + 1])*BIGM;
			_BC_mt.add(IloRange(_env, 0, expr, INFINITY, consName.c_str()));
			cout << consName << endl;
		}
	}
	_model.add(_BC_mt);


	IloExpr expr_obj(_env);

	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int c = 0; c < _contractList.size(); ++c)
		{
			for (int t = 0; t < _maxDayNum; ++t)
			{
				expr_obj += _x_mct[m][c][t] * 1;
				expr_obj += _p_mct[m][c][t] * 50;
			}
		}
	}

	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int c = 0; c < _contractList.size(); ++c)
		{
			for (int t = 0; t < _maxDayNum; ++t)
			{
				if (_machineList[m]->getIsCRM())
				{
					expr_obj += _y_mct[m][c][t] * 5;
				}
				else
				{
					expr_obj += _y_mct[m][c][t] * 500;
				}
			}
		}
	}

	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int c = 0; c < _contractList.size(); ++c)
		{
			expr_obj += _under_mc[m][c] * 100;
			expr_obj += _surplus_mc[m][c] * 100;
		}
	}
	
	//
	_obj.setExpr(expr_obj);
	expr_obj.end();
}

void Model::solveLp()
{
	string name = Util::OUTPUTPATH + string("Model") + string(".lp");
	cout << "output: " << name << endl;
	//
	_solver.exportModel(name.c_str());

	_solver.solve();
	//
	_solver.out() << "Solution status:" << _solver.getStatus() << endl;
	if (_solver.getStatus() == IloCplex::Infeasible)
	{
		exit(0);
	}

	_solver.out() << "Optimal Value: " << _solver.getObjValue() << endl;
}

void Model::output()
{
	//
	for (int m = 0; m < _machineList.size(); ++m)
	{
		_machineList[m]->initSolnMatrix(_materialList, _maxDayNum);
	}

	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int c = 0; c < _materialList.size(); ++c)
		{
			for (int t = 0; t < _maxDayNum; ++t)
			{
				IloNum y_soln = _solver.getValue(_y_mct[m][c][t]);
				cout << _y_mct[m][c][t].getName() << "=" << y_soln << endl;
				if (y_soln > EPSILON)
				{
					_machineList[m]->setSolnSupply(c, t, y_soln);
				}
			}
		}
	}

	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int c = 0; c < _materialList.size(); ++c)
		{
			for (int t = 0; t < _maxDayNum; ++t)
			{
				IloNum z_soln = _solver.getValue(_z_mct[m][c][t]);
				cout << _z_mct[m][c][t].getName() << "=" << z_soln << endl;
				if (z_soln > EPSILON)
				{
					_machineList[m]->setSolnInventory(c, t, z_soln);
				}
			}
		}
	}

	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int c = 0; c < _contractList.size(); ++c)
		{
			for (int t = 0; t < _maxDayNum; ++t)
			{
				IloNum x_soln = _solver.getValue(_x_mct[m][c][t]);
				cout << _x_mct[m][c][t].getName() << "=" << x_soln << endl;
				if (x_soln > EPSILON)
				{
					_machineList[m]->setSolnYield(c, t, x_soln);
				}
			}
		}
	}

	for (int i = 0; i < _contractList.size(); ++i)
	{
		_contractList[i]->initSolnWeight();
	}

	for (int m = 0; m < _machineList.size(); ++m)
	{
		for (int c = 0; c < _contractList.size(); ++c)
		{
			IloNum under_soln = _solver.getValue(_under_mc[m][c]);
			cout << _under_mc[m][c].getName() << "=" << under_soln << endl;
			if (under_soln > EPSILON)
			{
				_contractList[c]->pushUnderWeight(_machineList[m], under_soln);
			}

			IloNum surplus_soln = _solver.getValue(_surplus_mc[m][c]);
			cout << _surplus_mc[m][c].getName() << "=" << surplus_soln << endl;
			if (surplus_soln > EPSILON)
			{
				_contractList[c]->pushSurplusWeight(_machineList[m], surplus_soln);
			}
		}
	}

	IO::outputMachine(_machineList, _materialList, _maxDayNum);
	IO::outputContract(_contractList);
}