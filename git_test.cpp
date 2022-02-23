#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
   cout << "master" << endl;
   cout << "master:--> freature_dev_branch " << endl;
   cout << "master:--> freature_dev_branch:-->dev_bug_1 " << endl;
   cout << "master:--> FN_1234 " << endl;
   cout << "master:--> back inside master" << endl;
   cout << "master:--> FN_1234 inside again" << endl;
   cout << "master:--> add a line from master to be rebased in fn1234" << endl;
   cout << "first PR2" << endl;
   cout << "first PR3" << endl;
   std::vector<int> v = {1,2,3,4,5,6,7,8,9,10,11,12};
   int i = 4;
    for (auto x : v)
    {
	    i--;

            cout << x << " ";
	if(i==0)
	{
        cout << endl;
	i=4;
	}
    }
    cout << "try thist" << endl;
    i = 4;
    std::stringstream ss;
    for (auto x : v)
    {
	    i--;

            ss << x << " ";
	if(i==0)
	{
        cout << ss.str() << endl;
	i=4;
	ss.str("");
	}
    }
}
