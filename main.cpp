#include "inc/Set.h"

using namespace std;

int main()
{
    int a[] = {1, 4, 2, 6, 8};
    int b[] = {2, 5, 3, 6};
    Set<int> s1(a, 5), s2(b, 4);
    cout << "Set s1: " << s1 << endl;
    cout << "Set s2: " << s2 << endl;
    cout << "Set s1 + s2: " << s1 + s2 << endl;
    cout << "Set s1 - s2: " << s1 - s2 << endl;
    cout << "Set s1 intersect with s2: " << s1.intersect(s2) << endl;

    return 0;
}