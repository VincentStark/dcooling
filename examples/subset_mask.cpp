#include <string>
#include <iostream>

using namespace std;

/*  set count
abc = 000
ab = 001
a c = 010
a = 011
bc = 100
b = 101
c = 110
= 111
*/
void recursivefunction(string set, long long count)
{
  int i =0;
  bool firstelement = true;
  string temp;

  while (i < set.size()) {
    if (!(count>>i & 1)) { // test first bit
      if (!firstelement) // if not the first element in subset, produce a comma
        temp = set.substr(set.size()-1-i,1) + ","+temp;
      else {
        temp = set.substr(set.size()-1-i,1);
        firstelement = false;
      }
    }
    ++i;
  }

  //if(count) // if not the first subset, produce a comma
  //  cout << ",[" << temp << "]";
  //else
  //  cout << "[" << temp << "]";

  ++count;

  if(count% (1<<set.size()) ==0) // bailout condition, 2^setsize is number of subsets
    return;

  recursivefunction(set,count);
}

int main(int argc, char* argv[])
{
  string set;
  cout << "enter the characters of the set:" << endl;
  cin >> set;
  recursivefunction(set,0);
  cout << endl;
  return 0;
}
