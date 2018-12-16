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

	pair<int, float> recommendTimes_AvgScore;

	vector<pair<int,double>> recommendList;//recommend user

public:
	Movie();
	Movie(int id,int uni_id, string title, string genre);
	~Movie();
	array<double,USERNUMBER> _user;

	int getId() { return _id; }
	int getUniId() { return _unified_id; }
	void setRate(int, int);
	void print(ostream &os = cout);

	string getTitle() { return _title; }

	void getRecommendUsers(vector<int> *watcher, vector<int> *noWatchers);
	
	void setRecommendList(vector<pair<int,double>> *rL) { recommendList = *rL; }
	vector<pair<int,double>> getRecommendList() { return recommendList; }

	void setRecommendTimes_AvgScore(pair<int, float> ts) { recommendTimes_AvgScore = ts; }
	pair<int, float> getRecommendTimes_AvgScore() { return recommendTimes_AvgScore; }
};




#endif // MOVIE_H