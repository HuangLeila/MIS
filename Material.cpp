#include"Material.h"
#include"Contract.h"

int Material::_count = 0;
Material::Material(string name) :
	_name(name)
{
	_id = _count;
	_count++;
}

void Material::print()
{
	cout << "#Material" << _id;
	cout << "\tName: " << _name;
	cout << "\tContract: " << _contract->getName() << "(" << _contract->getId() << ")";
	cout << endl;
}