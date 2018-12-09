#pragma once
#ifndef _IO_H
#define _IO_H

#include"Schedule.h"

class IO {
private:
	static Schedule *_schedule;

public:
	IO();
	~IO();

	Schedule *getSchedule() { return _schedule; }

	void input();
	void readRatingMatrix();
	void readMovies();

};
#endif // !_IO_H
