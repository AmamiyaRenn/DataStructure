#include <ctime>
#include <sstream>
#include "../inc/Structure.h"
#include "app/Maze.h"

using namespace std;

int main()
{
    Maze maze;
    maze.solution(1, 1);
    cout << maze.ans() << endl;

    QueueCircular<char> q(4);
    q.enqueue('a');
    q.enqueue('b');
    q.enqueue('c');
    char x;
    q.dequeue(x);
    cout << x << " " << q.front() << " " << endl;

    return 0;
}