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

    return 0;
}