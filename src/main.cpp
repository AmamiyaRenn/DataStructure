#include <ctime>
#include <sstream>
#include "../inc/Structure.h"

using namespace std;

int main()
{
    const char a[] = "I really want to stay at your house, really", b[] = "really stay", c[] = "really";
    Vector<char> P(b, strlen(b)), T(a, strlen(a));
    cout << question2(P, T) << endl;
    Vector<char> C(c, strlen(c));
    cout << question3(C, T) << endl;

    return 0;
}