#include <ctime>
#include "inc/Structure.h"

using namespace std;

int main()
{
    int a[] = {4, 5, 6, 7, 8};
    int b[] = {2, 3, 4, 5};
    SeqList<int> s1(a, 5), s2(b, 4);
    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    s1.insert(2, 2);
    cout << "s1.insert(1, 2): " << s1 << endl;
    s1.remove(4);
    cout << "s1.remove(5): " << s1 << endl;
    int c[] = {-1, 2, 3, 7, -4, -5, 6, 7};
    SeqList<int> s3(c, 8);
    s3.move_all_neg_aheadOf_nonneg();
    cout << "s3.move_all_neg_aheadOf_nonneg(): " << s3 << endl;
    SeqList<int> s4(c, 8);
    s4.remove_all_maximum();
    cout << "s4.remove_all_maximum(): " << s4 << endl;

    return 0;
}