//
// Created by brett on 06/02/18.
//

#ifndef MAZESOLVER_V2_MAZE_H
#define MAZESOLVER_V2_MAZE_H

#include <string>
#include "StackNode.h"
#include "Stack.h"

using namespace std;

class Maze {
private:
    char** emptyMaze;
    char** maze;

    int rows = 0;
    int cols = 0;

    std::string file;
    std::string dir = "mazes/";

    Point end;

    enum mazeDetails{ Free = ' ', Used = '#', Dead = 'D' };
    enum moves { East = 0, South = 1, West = 2, North = 3 };

public:
    explicit Maze(const string &fileName);
    void saveMazeToFile();

    void setMazeSize();
    void buildMazeArray();

    void Move(moves move, Stack *stack, Point *pos);
    void solve();
    bool checkMove(Point here, moves move);

    void printMaze();
    void printBothMazes();
    void clean();
};


#endif //MAZESOLVER_V2_MAZE_H
