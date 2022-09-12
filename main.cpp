#include <ctime>
#include "inc/Structure.h"

using namespace std;

template <class T>
struct Increase
{
    void operator()(T &e) { e++; }
};

int main()
{
    int a[] = {4, 5, 6, 7, 8};
    int b[] = {2, 3, 4, 5};
    Vector<int> v1(a, 5), v2(b, 4);
    cout << v1 << '\n'
         << v2 << endl;
    v1.traverse(Increase<int>());
    cout << v1 << endl;

    return 0;
}