#include"IO.h"
#include"Util.h"

Schedule *IO::_schedule = NULL;

IO::IO() 
{
	_schedule = new Schedule();//dynamic memory allocation initialization
}

IO::~IO()
{
}

void IO::input()
{
	cout << "========================* INPUT MOVIES DATA *======================================" << endl;
	readMovies();
	cout << "========================* MOVIES DATA INPUT COMPLETED *======================================" << endl;
	cout << "========================* INPUT USERS DATA *======================================" << endl;
	readRatingMatrix();
	cout << "========================* USER DATA INPUT COMPLETED *======================================" << endl;
}

void IO::readMovies()
{
	map<int, int> mapid_UniId;
	vector<Movie *> movieList;

	string fileName = Util::INPUTPATH + "movies.csv";
	cout << "*Read movies from file : " << fileName << endl;
	ifstream infile(fileName);

	cout << (infile.is_open() ? "*The movies.csv file is open*" : "*movies.csv Opening failed*") << endl;
	
	int id = 0;
	int uni_id;
	string title;
	string genre;
	
	string buf;
	char *token;
	char *temp;

	/*if (infile.good())
	{
		getline(infile, buf);
		cout << buf << endl;

		getline(infile, buf);
		cout << buf << endl;
	}*/

	try
	{
		if (infile.good())
		{
			getline(infile, buf);//first row: title
			
			while (getline(infile, buf))
			{
				token = strtok_s((char*)buf.c_str(), ",", &temp);
				uni_id = atoi(token);

				token = strtok_s(NULL, ",", &temp);
				title = string(token);

				token = strtok_s(NULL, ",", &temp);
				genre = string(token);

				Movie *movie = new Movie(id,uni_id, title, genre);
				movieList.push_back(movie);

				mapid_UniId.insert(pair<int,int>(uni_id,id));

				id += 1;
			}
		}

		_schedule->setMovieList(movieList);
		_schedule->setMap_UnifiedId_Id(mapid_UniId);
		infile.close();

		cout << "The size of movieList is :" << _schedule->getMovieList().size() << endl;
		cout << "The size of map is :" << mapid_UniId.size() << endl;

		/*for (auto c:movieList)
			c->print();*/
		/*for (auto iter=mapid_UniId.begin();iter!=mapid_UniId.end();++iter)
		{
			cout << iter->first << " " << iter->second << endl;
		}*/

	}
	catch (exception &e) 
	{
		cerr << "* MOVIES.CSV LINE PARTITION BY COMMA FAILED !*" << endl;
	}
}

void IO::readRatingMatrix() 
{
	vector<User *> userList;
	vector<Movie *> movieList;
	

	//cout << "The size of movieList is :"<<movieList.size() << endl;// the difference between sizeof and .size() fucntion

	movieList = _schedule->getMovieList();
	cout << (movieList.empty() ? "The assignement of movieList failed" : "The assignement of movieList finished") << endl;
	map<int, int> IdMap = _schedule->getMap_UnifiedId_Id();
	cout << (movieList.empty() ? "The assignement of IdMap failed" : "The assignement of IdMap finished") << endl;

	string fileName = Util::INPUTPATH + "ratings.csv";
	cout << "*Read ratings from file : " << fileName << endl;
	ifstream infile(fileName, ifstream::in);
	
	cout << (infile.is_open() ? "The ratings.csv file is open" : "ratings.csv Opening failed") << endl;
	
	int id;//userId
	int m_u_id;//movieId:unified_id
	int m_id;//movieId
	int rating;
	string dipose_timestamp;

	string buf;
	char *token;
	char *temp;

	if (infile.good()) 
	{
		
		getline(infile, buf);//first row: title
		
		while (getline(infile, buf)) 
		{
			token = strtok_s((char*)buf.c_str(), ",", &temp);
			id = atoi(token);

			token = strtok_s(NULL, ",", &temp);
			m_u_id = atoi(token);

			token = strtok_s(NULL, ",", &temp);
			rating = atof(token);

			token = strtok_s(NULL, ",", &temp);
			dipose_timestamp = string(token);

			auto iter = IdMap.find(m_u_id);
			m_id = iter->second;

			// input row by row: initialize userList
			if (!userList.empty()) 
			{
				if ((*(userList.end()-1))->getID()<id)
				{
					User * user = new User(id);
					user->setRate(m_id, rating);
					userList.push_back(user);
				}
				else
				{
					userList[id - 1]->setRate(m_id, rating);
				}
			}
			else//add the first user into userList
			{
				User * user = new User(id);
				user->setRate(m_id, rating);
				userList.push_back(user);
			}

			//input column by column: revise movieList
			if (!movieList.empty())
			{
				movieList[m_id]->setRate(id,rating);
			}
			else
			{
				cerr << "* INPUT MOVIELIST FAILED *" << endl;
				break;
			}
		}
	}
	
	_schedule->setUserList(userList);
	_schedule->setMovieList(movieList);
	
	infile.close();

	/*cout << "====================PRINT FIRST 10 USER ROW====================" << endl;

	for (int i = 0; i < 10; ++i)
		userList[i]->print();

	cout << "====================PRINT FIRST 10 MOVIE COLUMN====================" << endl;

	for (int i = 0; i < 10; ++i)
		_schedule->getMovieList()[i]->print();*/
	cout << " * The total number of User is : " << _schedule->getUserList().size() << endl;
	cout << " * The total number of Movie is : " << _schedule->getMovieList().size() << endl;
}