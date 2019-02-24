#include"Util.h"
#include"Contract.h"
#include"IO.h"
#include"Machine.h"
#include"Model.h"
#include"Material.h"
#include"Path.h"
#include"Schedule.h"
#include"SubContract.h"

void main()
{
	Schedule * schedule = new Schedule();
	IO * io = new IO(schedule);
	io->readContractList();
	io->readMachineList();

	schedule->splitContracts();
	schedule->calculateInitInventory();
	schedule->generatePathList();

	Model * model = new Model(schedule);
	model->init();
	model->solveLP();
	model->output();
}