#pragma once
#pragma once

#include<iostream>
#include<stdio.h>
#include <algorithm>
#include<string>
#include<ctime>
#include<vector>
#include<stack>
#include<fstream>
#include<ilcplex/ilocplex.h>
#include<process.h>
#include<random>
#include<array>
#include<map>
#include <iomanip>
#include<math.h>

using namespace std;

#define EPSILON 0.00001
#define MAXNUM 99999999
#define ADAYTIME 24*3600

class Util
{

public:
	static string INPUTPATH;
	static string OUTPUTPATH;
	static time_t bgnTime;
	/*终止日期第二天一早上*/
	static time_t endTime;
};

inline time_t makeTime(int yr, int mon, int day, int hr = 0, int min = 0, int sec = 0)
{
	struct tm timeinfo;
	timeinfo.tm_year = yr - 1900;
	timeinfo.tm_mon = mon - 1;
	timeinfo.tm_mday = day;
	timeinfo.tm_hour = hr;
	timeinfo.tm_min = min;
	timeinfo.tm_sec = sec;
	return mktime(&timeinfo);
}