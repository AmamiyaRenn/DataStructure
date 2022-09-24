#pragma once

#include <sstream>
#include "../inc/Structure.h"

using namespace std;

class Maze
{
public:
    Maze(Rank x_end = 8, Rank y_end = 8)
        : x_end(x_end), y_end(y_end){};
    void solution(Rank row, Rank column)
    {
        if (maze[row][column])
            return;
        else if (row == x_end && column == y_end)
        {
            ss << "(" << row << ", " << column << ")"
               << "\n";
            solved = true;
            return;
        }
        else if (solved)
            return;

        ss << "(" << row << ", " << column << ")"
           << "\n";
        maze[row][column] = true;
        solution(row, column + 1);
        solution(row + 1, column);
        solution(row, column - 1);
        solution(row - 1, column);
        maze[row][column] = false;
    }
    string ans() { return ss.str(); }

private:
    const static Rank maze_size = 10;
    bool maze[maze_size][maze_size] =
        {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
         {1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
         {1, 0, 0, 1, 1, 0, 0, 1, 0, 1},
         {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
         {1, 0, 1, 1, 0, 1, 1, 1, 0, 1},
         {1, 0, 0, 1, 0, 0, 1, 0, 1, 1},
         {1, 1, 0, 0, 1, 0, 0, 0, 0, 1},
         {1, 1, 1, 0, 0, 1, 1, 1, 0, 1},
         {1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
    Rank x_end, y_end;
    bool solved = false;
    stringstream ss;
};