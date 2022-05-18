#include <map>
#include <iostream>
#include <string>
using namespace std;

int main()
{
    map<string, int> mymap;

    mymap["aa"]++;
    mymap["bb"]++;
    mymap["bb"]++;

    for(auto i : mymap)
    {
        cout << i.first << "  " << i.second << endl;
    }
}