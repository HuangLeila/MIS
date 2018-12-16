#include"Schedule.h"

Schedule::Schedule()
{
	_recommendMovies = new vector<pair<int, int>>;
}

Schedule::~Schedule() 
{
}

void Schedule::getRecommendList()
{
	//vector<pair<int, int>> *recommendStatis = new vector<pair<int, int>>;//count recommended times of movies

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

			int t=0;//recommended times
			float score = 0.0;

			m->getRecommendUsers(watchers, noWatchers);
			if (!watchers->empty() || !noWatchers->empty())
			{
				for (auto i : *noWatchers)
				{
					double sum_similarity = 0.0;
					
					for (auto w : *watchers)
					{
						sum_similarity+= _userList[i]->_similarity(_userList[w]);
					}

					//standardized recommend coefficient
					double sum=0.0;//BUG: un-initialized local variable

					for (auto w : *watchers)
					{
						sum += _userList[i]->_similarity(_userList[w])/sum_similarity*(*m)._user[w];
					}


					if (sum > 3.5)
					{
						pair<int, double> p;
						p.first = i;//useId
						p.second = sum;//cos coefficient
						rL->push_back(p);
						t += 1;
						score += sum;
					}
				}
				m->setRecommendList(rL);
				//recommendStatis->push_back(pair<int, int>(m->getUniId(), rL->size()));
				
				m->setRecommendTimes_AvgScore(pair<int,float>(t,(score>0?score/t:0)));
				
				m->print();

			}
			else
			{
				cout << " * getRecommandUser Failed * " << endl;
				break;
			}

			delete watchers, noWatchers, rL;
		}
	}
	else
		cerr << " * Input Users or Movies failed *" << endl;

	//delete recommendStatis;
}

vector<pair<int,int>> Schedule::getRecommendMovies()
{
	if (!_movieList.empty())
	{
		for (auto m : _movieList)
		{
			if (!m->getRecommendList().empty())
				_recommendMovies->push_back(pair<int,int>(m->getId(), m->getRecommendTimes_AvgScore().first));
		}
		
		for (auto i : *_recommendMovies)
		{
			cout << " The Id of recommended movie is: " << _movieList[i.first]->getUniId() << "\t" << " The title of movie is: " << _movieList[i.first]->getTitle() << "\t" <<
				" has been recommended times: " << i.second << "\t" << " the avg score is: " << _movieList[i.first]->getRecommendTimes_AvgScore().second << endl;
		}
	}
	
	
	return *_recommendMovies;
}

void Schedule::selectSort(vector<pair<int, int>> &vec)
{

}