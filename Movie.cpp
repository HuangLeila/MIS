#include"Movie.h"

Movie::Movie() 
{
}

Movie::Movie(int id,int uni_id, string title, string genre) 
{
	_id = id;
	_unified_id = uni_id;
	_title = title;
	_genre = genre;
	for (auto &tmp : _user)
		tmp = 0;
}

Movie::~Movie() 
{
}

void Movie::setRate(int u_id, int rate)
{
	if (u_id > 0 && rate >= 0)
	{
		_user[u_id - 1] = rate;
	}
	else
	{
		cerr << "* INVALID ARGUMENT: ILLEGAL USER_ID OR RATE *" << endl;
	}
}

void Movie::print(ostream &os) 
{
	os << "MovieId: " << _id << " " << "Movie Unified Id: " << _unified_id << " " << "Movie Title: " << _title << " " << "Movie Genre: " << _genre << endl;
	os << "THE USE COLUMN IS: " << endl;
	for (int i = 0; i < 10; ++i)
		os << _user[i] <<"\t";
	os << endl;
}

