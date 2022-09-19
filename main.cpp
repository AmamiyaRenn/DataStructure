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
    int a[] = {2, -5, 3, 5, 0, -2, 1, 9, 4, 5};
    int b[] = {2, 6, 4, 5, 8};
    List<int> l1;
    for (int i = 0; i < 10; i++)
        l1.insertAsLast(a[i]);
    cout << l1 << endl;
    l1.deduplicate();
    cout << l1 << endl;
    l1.insertionSort(l1.first(), l1.getSize());
    cout << l1 << endl;

    List<int> l2;
    for (int i = 0; i < 5; i++)
    {
        l2.insertAsLast(b[i]);
        l1.insertAsLast(b[i]);
    }
    cout << l2 << endl;
    cout << l1 << endl;
    l1.deduplicate();
    l1.insertionSort(l1.first(), l1.getSize());
    cout << l1 << endl;

    int c[] = {0, 1, 2, 2, 2, 5, 6, 6, 8, 9};
    List<int> l3;
    for (int i = 0; i < 10; i++)
        l3.insertAsLast(c[i]);
    l3.uniquify();
    cout << l3 << endl;

    return 0;
}