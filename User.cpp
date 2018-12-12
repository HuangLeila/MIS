#include"User.h"

User::User() //default constructor
{
	_id = 0;
	for (auto &tmp : _movies)
		tmp = 0;
}

User::User(int id) //alternative constructor
{
	_id = id;
	//_unifiedId = uni_id;
	for (auto &tmp : _movies)
		tmp = 0;
}

User::~User() //destructor 
{
}

float User::_getNorm() {
	int sum = 0;
	for (auto temp : _movies)
		sum += temp*temp;
	return sqrt(sum);
}

void User::setRate(int m_id, int rating)
{
	if(m_id>=0 && rating>=0)
	{
		_movies[m_id] = rating;
	}
	else
	{
		cerr << "* MOVIE_ID OR RATING IS ILLEGAL *" << endl;
	}
}

float User::_similarity(User* user1) 
{
	float sim = 0.0;
	
	for (int i = 0; i < MOVIENUMBER; ++i) 
		sim += _movies[i] * user1->_movies[i];
	
	sim = sim / (_getNorm()*user1->_getNorm());
	
	return sim;
}

void User::print()
{
	cout << "The useId is : " << _id << endl;
	cout << " This row of rating matrix is : " << endl;
	/*for (auto tmp : _movies)
	{
		cout << tmp << endl;
	}*/

	for (int i = 0; i < 10; ++i)
	{
		cout << _movies[i] << "\t";
	}
	cout << endl;
}