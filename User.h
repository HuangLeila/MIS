#pragma once
#ifndef USER_H
#define USE_H

#include"Util.h"

class User {
public:
	User();
	User(int);
	~User();
	
	int getID() { return _id; }

	//int getUnifiedID() { return _unifiedId; }
	
	void setRate(int, int);
	float _similarity(User*);

	array<double, MOVIENUMBER> _getMovies() { return _movies; }
	
	void print();

private:
	int _id;
	//int _unifiedId;
	array<double,MOVIENUMBER> _movies;
	float _getNorm();
};
#endif//!USE_H