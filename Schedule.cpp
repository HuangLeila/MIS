#include"Schedule.h"

Schedule::Schedule()
{
}

Schedule::~Schedule() 
{
}

void Schedule::getRecommendList()
{
	vector<pair<int, int>> *recommendStatis = new vector<pair<int, int>>;//count recommended times of movies

	if (!_userList.empty() && !_movieList.empty())
	{
		for (auto m : _movieList)
		{
			//BUG ?
			//vector<int> *watchers;
			//vector<int> *noWatchers;
			//vector<pair<int, double>> *rL ;//temporary recommend list

			vector<int> *watchers=new vector<int>();
			vector<int> *noWatchers=new vector<int>();
			vector<pair<int,double>> *rL=new vector<pair<int, double>>();//temporary recommend list

			m->getRecommendUsers(watchers, noWatchers);
			if (!watchers->empty() && !noWatchers->empty())
			{
				for (auto i : *noWatchers)
				{
					double sum_similarity = 0.0;
					
					for (auto w : *watchers)
					{
						sum_similarity+= _userList[i - 1]->_similarity(_userList[w - 1]);
					}

					//standardized recommend coefficient
					double sum=0.0;//BUG: un-initialized local variable

					for (auto w : *watchers)
					{
						sum += _userList[i - 1]->_similarity(_userList[w - 1])/sum_similarity*(*m)._user[w-1];
					}


					if (sum > 3.5)
					{
						pair<int, double> p;
						p.first = i;//useId
						p.second = sum;//cos coefficient
						rL->push_back(p);
					}
				}
				m->setRecommendList(rL);
				recommendStatis->push_back(pair<int, int>(m->getUniId(), rL->size()));
				
				if(m->getId()<10)
					m->print();				

			}
			else
			{
				cerr << " * getRecommandUser Failed * " << endl;
				break;
			}

			delete watchers, noWatchers, rL;
		}
	}
	else
		cerr << " * Input Users or Movies failed *" << endl;

	delete recommendStatis;
}
