#include"IO.h"
#include"Contract.h"
#include"Machine.h"
#include"Material.h"
#include"SubContract.h"
#include"Schedule.h"


IO::IO(Schedule * schedule):
	_schedule(schedule)
{

}

void IO::readContractList()
{
	vector<Contract *> contractList;
	vector<Machine *> machineList;
	vector<Material *> materialList;
	string fileName = Util::INPUTPATH + "Contract.csv";
	cout << "* Read contracts form file:" << fileName << endl;
	ifstream file(fileName.c_str());

	string buf;
	char * token;
	char * tmp;

	getline(file, buf);

	while (getline(file, buf))
	{
		/*�������*/
		token = strtok_s((char *)buf.c_str(), ",", &tmp);
		int id = atoi(token);
		/*�������*/
		token = strtok_s(NULL, ",", &tmp);
		string materialName = string(token);
		/*����λ*/
		token = strtok_s(NULL, ",", &tmp);
		int position = atoi(token);
		/*Ƿ��*/
		token = strtok_s(NULL, ",", &tmp);
		double weight = atof(token);
		/*ǰ���ڿ���*/
		token = strtok_s(NULL, ",", &tmp);
		double inventory = atof(token);
		/*�ƻ���*/
		token = strtok_s(NULL, ",", &tmp);
		double plan = atof(token);
		/*������*/
		token = strtok_s(NULL, ",", &tmp);
		double lock = atof(token);
		/*˵��*/
		token = strtok_s(NULL, ",", &tmp);
		/*��������*/
		token = strtok_s(NULL, ",", &tmp);
		string machineName = string(token);
		/*����*/
		token = strtok_s(NULL, ",", &tmp);
		string contractName = string(token);

		Machine * machine = NULL;
		for (int m = 0; m < machineList.size(); m++)
		{
			if (machineList[m]->getName() == machineName && machineList[m]->getPosition() == position)
			{
				machine = machineList[m];
				break;
			}
		}
		if (machine == NULL)
		{
			machine = new Machine(machineName, position);
			machineList.push_back(machine);
		}

		Material * material = NULL;
		for (int c = 0; c < materialList.size(); c++)
		{
			if (materialList[c]->getName() == materialName)
			{
				material = materialList[c];
				break;
			}
		}
		if (material == NULL)
		{
			material = new Material(materialName);
			materialList.push_back(material);
		}
		
		Contract * contract = NULL;
		for (int i = 0; i < contractList.size(); i++)
		{
			if (contractList[i]->getName() == contractName && contractList[i]->getMaterial() == material)
			{
				contract = contractList[i];
				break;
			}
		}

		if (contract == NULL)
		{
			contract = new Contract(contractName, material);
			contractList.push_back(contract);
			material->setContract(contract);
		}

		contract->pushInventory(inventory);
		contract->pushLock(lock);
		contract->pushMachine(machine);
		contract->pushPlan(plan);
		contract->pushWeight(weight);
	}

	file.close();

	_schedule->setContractList(contractList);
	_schedule->setMachineList(machineList);
	_schedule->setMaterialList(materialList);

	cout << "#Size of contracts is: " << contractList.size() << endl;
	cout << "#Size of machines is: " << machineList.size() << endl;
	cout << "#Size of materials is " << materialList.size() << endl;

	cout << "The contracts are: \n";
	for (int i = 0; i < contractList.size(); i++)
	{
		contractList[i]->print();
	}

	cout << "The machines are: \n";
	for (int m = 0; m < machineList.size(); m++)
	{
		machineList[m]->print();
	}

	cout << "The materials are:\n";
	for (int c = 0; c < materialList.size(); c++)
	{
		materialList[c]->print();
	}

}

void IO::readMachineList()
{
	vector<Machine *> machineList = _schedule->getMachineList();
	string fileName = Util::INPUTPATH + "Machine.csv";
	cout << "* Read machines form file:" << fileName << endl;
	ifstream file(fileName.c_str());

	string buf;
	char * token;
	char * tmp;

	getline(file, buf);

	int maxDayTime = (Util::endTime - Util::bgnTime) / (ADAYTIME);

	while (getline(file, buf))
	{
		/*��������*/
		token = strtok_s((char *)buf.c_str(), ",", &tmp);
		string machineName = string(token);
		/*��ȫ���*/
		token = strtok_s(NULL, ",", &tmp);
		double safeKeep = atof(token);
		Machine * machine = NULL;
		for (int m = 0; m < machineList.size(); m++)
		{
			if (machineList[m]->getName() == machineName)
			{
				machine = machineList[m];
				break;
			}
		}

		machine->setSafeInventory(safeKeep);

		/*ÿ��������*/
		for (int t = 0; t < maxDayTime; t++)
		{
			token = strtok_s(NULL, ",", &tmp);
			double yield = atof(token);
			machine->pushYield(yield);
		}
	}

	file.close();

	cout << "The datail machines are: \n";
	for (int m = 0; m < machineList.size(); m++)
	{
		machineList[m]->print();
	}
}

void IO::readSubContract()
{
	vector<SubContract *> subcontracList;

	string fileName(Util::INPUTPATH + "ColdRollContract.csv");
	cout << "* Read contract order from file: " << fileName << endl;

	ifstream file(fileName);
	cout << (file.good() ? "File is open" : "Openning file failed") << endl;

	string buf;
	char * token;
	char * tmp;

	string material, explanation, techRoute, machine;
	int id(0), position;
	double weight, frontStock, plannedVolume, blockedVolume;
	//bool isColdRoll;

	getline(file, buf);

	while (getline(file, buf))
	{
		vector<Machine *> machineList;
		vector<double> weightList;

		token = strtok_s((char *)buf.c_str(), ",", &tmp);
		material = string(token);
		Material *mater = new Material(material);


		token = strtok_s(NULL, ",", &tmp);
		position = atoi(token);

		token = strtok_s(NULL, ",", &tmp);
		weight = atof(token);
		weightList.push_back(weight);

		token = strtok_s(NULL, ",", &tmp);
		//frontStock = atof(token);

		token = strtok_s(NULL, ",", &tmp);
		//plannedVolume = atof(token);

		token = strtok_s(NULL, ",", &tmp);
		//blockedVolume = atof(token);

		token = strtok_s(NULL, ",", &tmp);
		explanation = string(token);

		token = strtok_s(NULL, ",", &tmp);
		techRoute = string(token);


		token = strtok_s(NULL, ",", &tmp);
		machine = string(token);

		Machine tempMachine(machine, position);
		Machine *mach = &tempMachine;
		machineList.push_back(mach);

		SubContract *subcontract = new SubContract(machineList, weight, mater, NULL);

		subcontracList.push_back(subcontract);


	}

	//_schedule->setSubContractList(subcontracList);
	file.close();

}

void IO::outputMachine(vector<Machine *> machineList, vector<Material *> materialList, int maxDayNum)
{
	fstream resultFile;
	string fname = Util::OUTPUTPATH + "ResultMachine" + ".csv";
	cout << endl << "* output result:" << fname << endl;
	resultFile.open(fname, ios::out);
	if (!resultFile.is_open())
	{
		cout << "output error:fail to open :" << fname << endl;
		return;
	}

	string head = "��������,�������,����,�������";
	for (int t = 0; t < maxDayNum; t++)
	{
		head = head + "," + "Day" + to_string(t);
	}
	resultFile << head << endl;

	for (int m = 0; m < machineList.size(); m++)
	{
		for (int c = 0; c < materialList.size(); c++)
		{
			resultFile << machineList[m]->getName() << ",";
			resultFile << "����,";
			resultFile << materialList[c]->getContract()->getName() << ",";
			resultFile << materialList[c]->getName() << ",";			
			for (int t = 0; t < maxDayNum; t++)
			{
				resultFile << machineList[m]->getSolnSupply(c, t) << ",";
			}
			resultFile << endl;
		}
		resultFile << ",,,�ϼ�,";
		for (int t = 0; t < maxDayNum; t++)
		{
			resultFile << machineList[m]->getSolnSupplyPerDay(t) << ",";
		}
		resultFile << endl;


		for (int c = 0; c < materialList.size(); c++)
		{
			resultFile << machineList[m]->getName() << ",";
			resultFile << "���,";
			resultFile << materialList[c]->getContract()->getName() << ",";
			resultFile << materialList[c]->getName() << ",";
			for (int t = 0; t < maxDayNum; t++)
			{
				resultFile << machineList[m]->getSolnInventory(c, t) << ",";
			}
			resultFile << endl;
		}
		resultFile << ",,,�ϼ�,";
		for (int t = 0; t < maxDayNum; t++)
		{
			resultFile << machineList[m]->getSolnInventoryPerDay(t) << ",";
		}
		resultFile << endl;


		for (int c = 0; c < materialList.size(); c++)
		{
			resultFile << machineList[m]->getName() << ",";
			resultFile << "����,";
			resultFile << materialList[c]->getContract()->getName() << ",";
			resultFile << materialList[c]->getName() << ",";
			for (int t = 0; t < maxDayNum; t++)
			{
				resultFile << machineList[m]->getSolnYield(c, t) << ",";
			}
			resultFile << endl;
		}
		resultFile << ",,,�ϼ�,";
		for (int t = 0; t < maxDayNum; t++)
		{
			resultFile << machineList[m]->getSolnYieldPerDay(t) << ",";
		}
		resultFile << endl;
	}

}


void IO::outputContract(vector<Contract *> contractList)
{
	fstream resultFile;
	string fname = Util::OUTPUTPATH + "ResultContract" + ".csv";
	cout << endl << "* output result:" << fname << endl;
	resultFile.open(fname, ios::out);
	if (!resultFile.is_open())
	{
		cout << "output error:fail to open :" << fname << endl;
		return;
	}

	string head = "����,�������,��������,δ�����";
	resultFile << head << endl;

	for (int i = 0; i < contractList.size(); i++)
	{
		vector<Machine *> machineList = contractList[i]->getMachineList();
		vector<double> underWeightList = contractList[i]->getUnderWeightList();

		for (int m = 0; m < machineList.size(); m++)
		{
			if (machineList[m]->getIsColdRoll())
			{
				resultFile << contractList[i]->getName() << ",";
				resultFile << contractList[i]->getMaterial()->getName() << ",";
				resultFile << machineList[m]->getName() << ",";
				resultFile << underWeightList[m] << endl;
			}
		}
	}
}