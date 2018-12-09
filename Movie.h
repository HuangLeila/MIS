#pragma once
#ifndef MOVIE_H
#define MOVIE_H

#include"Util.h"

class User;

class Movie {
private:
	int _id;
	int _unified_id;
	string _title;
	string _genre;
public:
	Movie();
	Movie(int id,int uni_id, string title, string genre);
	~Movie();
	array<double,USERNUMBER> _user;

	int getId() { return _id; }
	int getUniId() { return _unified_id; }
	void setRate(int, int);
	void print(ostream &os = cout);


};


#endif // MOVIE_H