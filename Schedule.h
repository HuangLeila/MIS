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
	map<int, int> _movie_map_UnifiedId_Id;
	map<int, int> _user_map_UnifiedId_Id;

public:
	Schedule();
	~Schedule();

	void setMovieMap_UnifiedId_Id(map<int, int> map) { _movie_map_UnifiedId_Id = map; }
	map<int, int> getMovieMap_UnifiedId_Id() { return _movie_map_UnifiedId_Id; }

	void setUserMap_UnifiedId_Id(map<int, int> map) { _user_map_UnifiedId_Id = map; }
	map<int, int> getUserMap_UnifiedId_Id() { return _user_map_UnifiedId_Id; }

	void setUserList(vector<User *> userList) { _userList = userList; }
	vector<User *> getUserList() { return _userList; }

	void setMovieList(vector<Movie *> movieList) { _movieList = movieList; }
	vector<Movie *> getMovieList() { return _movieList; }

	void getRecommendList();
};
#endif // !SCHEDULE_H

