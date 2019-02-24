#include"Model.h"
#include"IO.h"

Model::Model(Schedule * schedule)
{
	_contractList = schedule->getContractList();
	_fullMachineList = schedule->getMachineList();
	for (int m = 0; m < _fullMachineList.size(); m++)
	{
		if (_fullMachineList[m]->getIsColdRoll())
		{
			_fullMachineList[m]->setUseId(_machineList.size());
			_machineList.push_back(_fullMachineList[m]);
		}
	}

	_materialList = schedule->getMaterialList();
	_pathMatrix = schedule->getPathMatrix();
	_subContractList = schedule->getSubContractList();

	_maxDayNum = (Util::endTime - Util::bgnTime) / (ADAYTIME);

	_model = IloModel(_env);
	_obj = IloAdd(_model, IloMinimize(_env));
	_solver = IloCplex(_model);
}

void Model::init()
{
	cout << "Begin Init Model\n";
	/*每条路径的产量*/
	//IloNumVarArray _x_p;
	_x_ip = NumVar2Matrix(_env, _subContractList.size());
	for (int i = 0; i < _subContractList.size(); i++)
	{
		_x_ip[i] = IloNumVarArray(_env);
		vector<Path *> pathList = _subContractList[i]->getPathList();
		for (int p = 0; p < pathList.size(); p++)
		{
			pathList[p]->setUseId(p);
			string varName = "X_I_" + to_string(_subContractList[i]->getId()) + "_" +
				to_string(_subContractList[i]->getContract()->getId()) +
				"_P_" + to_string(pathList[p]->getId()) + "_" + to_string(p);
			_x_ip[i].add(IloNumVar(_env, 0, pathList[p]->getSubContract()->getWeight(), ILOFLOAT, varName.c_str()));
			cout << varName << endl;
		}
		_model.add(_x_ip[i]);
	}

	/*每台机器不同材料每天初的额外供料*/
	//NumVar3Matrix _y_mct;
	_y_mct = NumVar3Matrix(_env, _machineList.size());
	for (int m = 0; m < _machineList.size(); m++)
	{
		_y_mct[m] = NumVar2Matrix(_env, _materialList.size());
		for (int c = 0; c < _materialList.size(); c++)
		{
			_y_mct[m][c] = IloNumVarArray(_env);
			for (int t = 0; t < _maxDayNum; t++)
			{
				string varName = "Y_M_" + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
				_y_mct[m][c].add(IloNumVar(_env, 0, _machineList[m]->getSafeInventory(), ILOFLOAT, varName.c_str()));
				cout << varName << endl;
			}
			_model.add(_y_mct[m][c]);
		}
	}

	/*每台机器不同材料每天初的ku库存，t=0时的库存为初始库存*/
	//NumVar3Matrix _z_mct;
	_z_mct = NumVar3Matrix(_env, _machineList.size());
	for (int m = 0; m < _machineList.size(); m++)
	{
		_z_mct[m] = NumVar2Matrix(_env, _materialList.size());
		for (int c = 0; c < _materialList.size(); c++)
		{
			_z_mct[m][c] = IloNumVarArray(_env);
			for (int t = 0; t < _maxDayNum; t++)
			{
				string varName = "Inventory_M_" + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
				if (t > 0)
				{
					_z_mct[m][c].add(IloNumVar(_env, 0, _machineList[m]->getSafeInventory(), ILOFLOAT, varName.c_str()));
				}
				else
				{
					_z_mct[m][c].add(IloNumVar(_env, 0, 0, ILOFLOAT, varName.c_str()));
				}
				cout << varName << endl;
			}
			_model.add(_z_mct[m][c]);
		}

		vector<pair<Material *, double>> initInventory = _machineList[m]->getInitInventory();
		for (int index = 0; index < initInventory.size(); index++)
		{
			_z_mct[m][initInventory[index].first->getId()][0].setBounds(initInventory[index].second, initInventory[index].second);
		}
	}

	/*每个合同欠量*/
	//IloNumVarArray _under_i;
	_under_i = IloNumVarArray(_env);
	for (int i = 0; i < _subContractList.size(); i++)
	{
		string varName = "UnDmd_I_" + to_string(_subContractList[i]->getId()) + "_" + to_string(_subContractList[i]->getContract()->getId());
		IloExpr expr(_env);
		_under_i.add(IloNumVar(_env, 0, _subContractList[i]->getWeight(), ILOFLOAT, varName.c_str()));
		cout << varName << endl;
	}
	_model.add(_under_i);

	/*每个合同的产量要求*/
	//IloRangeArray _demand_i;
	_demand_i = IloRangeArray(_env);
	for (int i = 0; i < _subContractList.size(); i++)
	{
		string consName = "Dmd_I_" + to_string(_subContractList[i]->getId()) + "_" + to_string(_subContractList[i]->getContract()->getId());
		IloExpr expr(_env);
		for (int p = 0; p < _x_ip[i].getSize(); p++)
		{
			expr += _x_ip[i][p];
		}
		expr += _under_i[i];
		_demand_i.add(IloRange(_env, _subContractList[i]->getWeight(), expr, INFINITY, consName.c_str()));
		cout << consName << endl;
	}
	_model.add(_demand_i);

	/*每台机器每天的产能约束*/
	//IloRangeArray _capacity_mt;
	_capacity_mt = IloRangeArray(_env);
	for (int m = 0; m < _machineList.size(); m++)
	{
		for (int t = 0; t < _maxDayNum; t++)
		{
			string consName = "Cap_M_" + to_string(m) + "_T_" + to_string(t);
			IloExpr expr(_env);
			for (int i = 0; i < _subContractList.size(); i++)
			{
				vector<Path *> pathList = _subContractList[i]->getPathList();
				for (int p = 0; p < pathList.size(); p++)
				{
					if (pathList[p]->produceContain(_machineList[m], t))
					{
						expr += _x_ip[i][p];
					}
				}
			}
			_capacity_mt.add(IloRange(_env, 0, expr, _machineList[m]->getYield(t), consName.c_str()));
			cout << consName << endl;
		}
	}
	_model.add(_capacity_mt);

	/*每台机器每个材料每天库存平衡*/
	//IloRangeArray _blc_mct;
	_blc_mct = IloRangeArray(_env);
	for (int m = 0; m < _machineList.size(); m++)
	{
		for (int c = 0; c < _materialList.size(); c++)
		{
			/*从第二天开始考虑*/
			for (int t = 1; t < _maxDayNum; t++)
			{
				string consName = "Blc_M_" + to_string(m) + "_C_" + to_string(c) + "_T_" + to_string(t);
				IloExpr expr(_env);
				for (int i = 0; i < _subContractList.size(); i++)
				{
					vector<Path *> pathList = _subContractList[i]->getPathList();
					for (int p = 0; p < pathList.size(); p++)
					{
						if (_subContractList[i]->getMaterial() == _materialList[c] &&
							pathList[p]->inventoryContain(_machineList[m], t))
						{
							expr += _x_ip[i][p];
						}
						if (_subContractList[i]->getMaterial() == _materialList[c] &&
							pathList[p]->produceContain(_machineList[m], t - 1))
						{
							expr -= _x_ip[i][p];
						}
					}				
				}

				expr += _z_mct[m][c][t - 1];
				expr += _y_mct[m][c][t - 1];
				expr -= _z_mct[m][c][t];

				_blc_mct.add(IloRange(_env, 0, expr, 0, consName.c_str()));
				cout << consName << endl;
			}
		}
	}
	_model.add(_blc_mct);

	/*每台机器每天总库存平衡*/
	//IloRangeArray _safeKeep_mt;
	_safeKeep_mt = IloRangeArray(_env);
	for (int m = 0; m < _materialList.size(); m++)
	{
		for (int t = 1; t < _maxDayNum; t++)
		{
			string consName = "SafeKeep_M_" + to_string(m) + "_T_" + to_string(t);
			IloExpr expr(_env);
			for (int c = 0; c < _materialList.size(); c++)
			{
				expr += _z_mct[m][c][t];
			}
			_safeKeep_mt.add(IloRange(_env, _machineList[m]->getSafeInventory(), expr, _machineList[m]->getSafeInventory(), consName.c_str()));
			cout << consName << endl;
		}
	}
	_model.add(_safeKeep_mt);

	IloExpr expr_obj(_env);
	for (int i = 0; i < _subContractList.size(); i++)
	{
		vector<Path *> pathList = _subContractList[i]->getPathList();
		for (int p = 0; p < pathList.size(); p++)
		{
			expr_obj += _x_ip[i][p] * 1;
		}
	}

	for (int m = 0; m < _machineList.size(); m++)
	{
		for (int c = 0; c < _materialList.size(); c++)
		{
			for (int t = 0; t < _maxDayNum; t++)
			{
				expr_obj += _y_mct[m][c][t] * 10;
			}
		}
	}

	for (int i = 0; i < _subContractList.size(); i++)
	{
		expr_obj += _under_i[i] * 100;
	}

	_obj.setExpr(expr_obj);
	expr_obj.end();
}

void Model::solveLP()
{
	string name = Util::OUTPUTPATH + string("model")  + string(".lp");
	cout << "output:" << name << endl;
	_solver.exportModel(name.c_str());

	//_solver.setParam(IloCplex::RootAlg, IloCplex::Barrier);
	_solver.solve();

	_solver.out() << "Solution status:" << _solver.getStatus() << endl;
	if (_solver.getStatus() == IloCplex::Infeasible)
	{
		exit(0);
	}

	_solver.out() << "Optimal value:" << _solver.getObjValue() << endl;
}

void Model::output()
{
	for (int m = 0; m < _machineList.size(); m++)
	{
		_machineList[m]->initSolnMatrix(_materialList, _maxDayNum);
	}

	for (int m = 0; m < _machineList.size(); m++)
	{
		for (int c = 0; c < _materialList.size(); c++)
		{
			for (int t = 0; t < _maxDayNum; t++)
			{
				IloNum y_soln = _solver.getValue(_y_mct[m][c][t]);
				cout << _y_mct[m][c][t].getName() << " = " << y_soln << endl;
				if (y_soln > EPSILON)
				{
					_machineList[m]->setSolnSupply(c, t, y_soln);
				}
			}
		}
	}

	for (int m = 0; m < _machineList.size(); m++)
	{
		for (int c = 0; c < _materialList.size(); c++)
		{
			for (int t = 0; t < _maxDayNum; t++)
			{
				IloNum z_soln = _solver.getValue(_z_mct[m][c][t]);
				cout << _z_mct[m][c][t].getName() << " = " << z_soln << endl;
				if (z_soln > EPSILON)
				{
					_machineList[m]->setSolnInventory(c, t, z_soln);
				}
			}
		}
	}

	for (int i = 0; i < _subContractList.size(); i++)
	{
		vector<Machine *> machines = _subContractList[i]->getMachineList();
		Material * material = _subContractList[i]->getMaterial();
		vector<Path *> pathList = _subContractList[i]->getPathList();
		for (int p = 0; p < pathList.size(); p++)
		{
			vector<int> dayNums = pathList[p]->getDayList();
			IloNum x_soln = _solver.getValue(_x_ip[i][p]);
			cout << _x_ip[i][p].getName() << " = " << x_soln << endl;
			if (x_soln > EPSILON)
			{
				pathList[p]->setSolnWeight(x_soln);
				for (int mm = 0; mm < machines.size(); mm++)
				{
					machines[mm]->addSolnYield(material->getId(), dayNums[mm], x_soln);
				}
			}
			else
			{
				pathList[p]->setSolnWeight(0);
			}
		}
	}

	for (int i = 0; i < _subContractList.size(); i++)
	{
		IloNum soln_under_i = _solver.getValue(_under_i[i]);
		cout << _under_i[i].getName() << " = " << soln_under_i << endl;
		if (soln_under_i > EPSILON)
		{
			_subContractList[i]->setUnderWeight(soln_under_i);
		}
		else
		{
			_subContractList[i]->setUnderWeight(0);
		}
	}

	IO::outputMachine(_machineList, _materialList, _maxDayNum);
	
	for (int i = 0; i < _contractList.size(); i++)
	{
		_contractList[i]->calcuUnderWeightList();
	}
	IO::outputContract(_contractList);

}