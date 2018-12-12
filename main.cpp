#include"IO.h"
#include"Movie.h"
#include<memory>

int main(int argc,char **argv) 
{
	clock_t start;
	start = clock();
	//try
	//{
	//	IO *io = new IO();//dynamic memory allocation initialization
	//	io->readMovies();
	//}
	//catch (exception &e)
	//{
	//	cerr << "IO initialization failed !" << endl;
	//}
	
	/*vector<User *> users;

	User *user = new User(1);
	user->setRate(1, 5);
	user->setRate(5, 6);
	
	user->print();

	users.push_back(user);

	cout << "* Check whether the subscript would out of range *" << endl;
	cout<< (*(users.end() - 1))->getID() << endl;
	user->print();*/

	try
	{
		IO *io = new IO();//dynamic memory allocation initialization
		io->input();

		clock_t inputTime;
		inputTime = clock();
		cout << " Time for input is: " << (inputTime - start) / CLK_TCK << " seconds" << endl;

		cout << " After Read Files *" << endl;

		Schedule *schedule = io->getSchedule();
		schedule->getRecommendList();

		clock_t totalTime;
		totalTime = clock();
		cout << " RunTime is: " << (totalTime - start) / CLK_TCK << " seconds" << endl;

	}
	catch (exception &e)
	{
		cerr << "IO initialization failed !" << endl;
	}

	/*vector<Movie *> movieList;
	Movie *movie = new Movie(1, "title", "comedy");
	movieList.push_back(movie);
	movieList[0]->setRate(1, 5);
	movieList[0]->print();*/
	
	system("pause");
}