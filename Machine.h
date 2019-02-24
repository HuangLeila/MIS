#pragma once
#include"Util.h"

class Material;
class Machine
{
private:
	int _id;
	static int _count;
	int _useId;
	string _name;
	int _position;
	bool _isColdRoll;
	vector<pair<Material*, double>> _initInventoryList;
	double _safeInventory;
	vector<double> _yieldList;

	vector<vector<double>> _solnInventoryMatrix;
	vector<vector<double>> _solnYieldMatrix;
	vector<vector<double>> _solnSupplyMatrix;

public:
	Machine(string name, int position);

	void setId(int id) { _id = id; }
	int getId() { return _id; }

	void setUseId(int useId) { _useId = useId; }
	int getUseId() { return _useId; }

	void setName(string name) { _name = name; }
	string getName() { return _name; }

	void setPosition(int position) { _position = position; }
	int getPosition() { return _position; }

	void setIsColdRoll(bool b) { _isColdRoll = b; }
	bool getIsColdRoll() { return _isColdRoll; }

	void setSafeInventory(double safeInventory) { _safeInventory = safeInventory; }
	double getSafeInventory() { return _safeInventory; }

	void setInitInventoryList(vector<pair<Material*, double>> initInventoryList) { _initInventoryList = initInventoryList; }
	void pushInitInventory(Material * material, double weight) { _initInventoryList.push_back(make_pair(material, weight)); }
	vector<pair<Material *, double>> getInitInventory() { return _initInventoryList; }

	void setYieldList(vector<double> yieldList) { _yieldList = yieldList; }
	void pushYield(double yield) { _yieldList.push_back(yield); }
	vector<double> getYieldList() { return _yieldList; }
	double getYield(int t) { return _yieldList[t]; }

	void initSolnMatrix(vector<Material *> materialList, int maxDayNum);
	void setSolnInventory(int c, int t, double inventory) { _solnInventoryMatrix[c][t] = inventory; }
	void setSolnYield(int c, int t, double yield) { _solnYieldMatrix[c][t] = yield; }
	void addSolnYield(int c, int t, double addYield) { _solnYieldMatrix[c][t] += addYield; }
	void setSolnSupply(int c, int t, double supply) { _solnSupplyMatrix[c][t] = supply; }
	double getSolnInventory(int c, int t) { return _solnInventoryMatrix[c][t]; }
	double getSolnYield(int c, int t) { return _solnYieldMatrix[c][t]; }
	double getSolnSupply(int c, int t) { return _solnSupplyMatrix[c][t]; }
	double getSolnInventoryPerDay(int t);
	double getSolnYieldPerDay(int t);
	double getSolnSupplyPerDay(int t);

	void print();
};
