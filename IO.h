#pragma once
#include"Util.h"

class Machine;
class Material;
class Contract;
class Schedule;
class SubContract;
class IO
{
private:
	Schedule * _schedule;
public:
	IO(Schedule * schedule);
	void readContractList();
	void readMachineList();
	void readSubContract();

	void setSchedule(Schedule * schedule) { _schedule = schedule; }
	Schedule * getSchedule() { return _schedule; }

	static void outputMachine(vector<Machine *> machineList, vector<Material *> materials, int maxDayNum);
	static void outputContract(vector<Contract *> contractList);
};
