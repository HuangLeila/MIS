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
	if (u_id >= 0 && rate >= 0)
	{
		_user[u_id ] = rate;
	}
	else
	{
		cerr << "* INVALID ARGUMENT: ILLEGAL USER_ID OR RATE *" << endl;
	}
}

void Movie::print(ostream &os) 
{
	os << "MovieId: " << _id << " " << "Movie Unified Id: " << _unified_id << " " << "Movie Title: " << _title << " " << "Movie Genre: " << _genre << endl;
	/*os << "THE USE COLUMN IS: " << endl;
	for (int i = 0; i < 10; ++i)
		os << _user[i] <<"\t";
	os << endl;*/

	os << " The recommend list is : " << endl;
	for (auto i : recommendList)
		os << " The recommend user Id is " << i.first << "\t" << "The cos coefficient is " << i.second << endl;
	os << " The recommended times is: " << getRecommendTimes_AvgScore().first << "\t" << " the avg score is: " << getRecommendTimes_AvgScore().second << endl;
	os << endl << endl;
}

void Movie::getRecommendUsers(vector<int> *watcher, vector<int> *noWatchers)
{
	for (int i=0;i<_user.size();++i)
	{
		if (_user[i] > 0.0)
		{
			watcher->push_back(i);// i : user Id who've watched this movie (not unified ID)
		}
		else
			noWatchers->push_back(i);
	}

	/*cout << " * The Movie : " << _title << " watchers include : " << endl;
	for (auto u : _watchers)
	{
		cout << u << "\t";
	}
	cout<<endl;
	cout << " * The Movie : " << _title << "  nnon-watchers include : " << endl;
	for (auto u : _noWatchers)
	{
		cout << u << "\t";
	}
	cout << endl;*/
}

