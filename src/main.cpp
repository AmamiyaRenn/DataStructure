#include <ctime>
#include "../inc/Structure.h"

using namespace std;

template <class T>
struct Increase
{
    void operator()(T &e) { e++; }
};

int main()
{
    int a[] = {2, -5, 3, 5, 0, -2, 1, 9, 4, 5};
    int b[] = {2, 6, 4, 5, 8};
#ifdef STACK_VECTOR
    Stack<int> s1(a, 10);
#else
    Stack<int> s1;
    for (int i = 0; i < 10; i++)
        s1.push(a[i]);
#endif
    cout << s1.pop() << endl;
    cout << s1.top() << endl;
    s1.push(-1);
    cout << s1.top() << endl;
    int size = s1.getSize();
    for (int i = 0; i != size; i++)
        cout << s1.pop() << " ";

    return 0;
}