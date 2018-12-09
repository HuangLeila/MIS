#pragma once
#ifndef SCHEDULE_H
#define SCHEDULE_H

#include"Movie.h"
#include"User.h"
#include"Util.h"


class Schedule 
{
private:
	vector<User *> _userList;
	vector<Movie *> _movieList;
	map<int, int> map_UnifiedId_Id;

public:
	Schedule();
	~Schedule();

	void setMap_UnifiedId_Id(map<int, int> map) { map_UnifiedId_Id = map; }
	map<int, int> getMap_UnifiedId_Id() { return map_UnifiedId_Id; }

	void setUserList(vector<User *> userList) { _userList = userList; }
	vector<User *> getUserList() { return _userList; }

	void setMovieList(vector<Movie *> movieList) { _movieList = movieList; }
	vector<Movie *> getMovieList() { return _movieList; }
};
#endif // !SCHEDULE_H

