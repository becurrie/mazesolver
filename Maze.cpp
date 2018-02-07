//
// Created by brett on 06/02/18.
//

#include <fstream>
#include <iostream>
#include "Maze.h"

/**
 * Explicit Maze Constructor function that will create the grid representing the maze.
 * @param fileName string Name of file being read.
 */
Maze::Maze(const string &fileName) {
    // Before doing anything, must verify that the file provided is a valid text file.
    ifstream f(dir + fileName);
    if (!f.good()) {
        cout << "File: " << fileName << " is inaccessible or doesn't exist..." << endl << endl;
        return;
    }

    // Set filename string property.
    file = fileName;

    // Set the rows and cols properties from maze file.
    setMazeSize();

    // Set the current maze's end Point.
    end = Point(cols - 1, rows - 2);

    // Finally, build up the dynamic 2d char array using the newly determined
    // rows and cols variables.
    buildMazeArray();

    solve();
}

/**
 * Constructor helper function to determine the size of the Maze being created by
 * retrieving the number of rows and columns.
 */
void Maze::setMazeSize() {
    // Determine size of the maze being solved.
    ifstream f(dir + file);

    // Temporary columns int to store the number of columns found throughout loop,
    // this number is incremented by number of cols in each row, it will only be set
    // to the cols property once to ensure proper columns.
    int tempCols = 0;
    while (!f.eof()) {
        rows += 1;
        string line;
        getline(f, line);

        // Loop through each character in the current line (column) as long as the character isn't a '\r'.
        for (char c : line) {
            if (c != '\r')
                tempCols += 1;
        }

        // Because the maze will always be a grid, we can set the cols once and know
        // that each row will have the same number of columns.
        if (cols == 0)
            cols = tempCols;
    }
    // Finally, close the ifstream object.
    f.close();
}

/**
 * Constructor helper function to build up a 2D char array containing the contents of the maze file
 * begin inputted. By first creating a pointer to an array of chars[rows], then looping through each row
 * and creating another pointer to an array of chars[cols].
 */
void Maze::buildMazeArray() {
    maze = new char*[rows];
    emptyMaze = new char*[rows];
    for (int i = 0; i < rows; i++) {
        maze[i] = new char[cols];
        emptyMaze[i] = new char[cols];
    }

    ifstream f(dir + file);
    while (!f.eof()) {
        for (int i = 0; i < rows; i++) {
            string line;
            getline(f, line);
            for (int x = 0; x < cols; x++) {
                maze[i][x] = line[x];
                emptyMaze[i][x] = line[x];
            }
        }
    }
    f.close();
}

/**
 * Print the maze directly to the console.
 * Note: This will print the solved/being solved maze to the console, not to be mistaken
 * for the emptyMaze that holds an empty unsolved version of the maze.
 */
void Maze::printMaze() {
    for (int i = 0; i < rows; i++) {
        for (int c = 0; c < cols; c++) {
            std::cout << maze[i][c];
        }
        std::cout << endl;
    }
    std::cout << endl << endl;
}

/**
 * Prettier print function to display both mazes side by side for comparison.
 */
void Maze::printBothMazes() {
    cout << "Printing Maze: " << file << "..." << endl;
    for (int i = 0; i < rows; i++) {
        for (int c = 0; c < cols; c++) {
            cout << emptyMaze[i][c];
        }
        cout << "  ^  ";
        for (int c = 0; c < cols; c++) {
            cout << maze[i][c];
        }
        cout << endl;
    }
    cout << endl << endl;
}

/**
 * Check that a move is valid by seeing if the spot North, West, South, Eat from the current Point here
 * is equal to the moves Free character.
 * @param here The current Point position in the maze.
 * @param move Enum for coordinate (North, West, South, East).
 * @return True or False if the proposed move is valid.
 */
bool Maze::checkMove(Point here, moves move) {
    switch (move) {
        case North:
            return (maze[here.row - 1][here.col] == Free);
        case East:
            return (maze[here.row][here.col + 1] == Free);
        case South:
            return (maze[here.row + 1][here.col] == Free);
        case West:
            return (maze[here.row][here.col - 1] == Free);
    }
}

/**
 * Maze move method to move the current Point position to the new location specified by the moves move.
 * The new position will be pushed to the Stack.
 * @param move Enum for coordinate (North, West, South, East).
 * @param stack The current Maze Stack.
 * @param pos The current Point position.
 */
void Maze::Move(Maze::moves move, Stack *stack, Point *pos) {
    stack->Push(*pos);
    maze[pos->row][pos->col] = Used;

    // Check move being performed using a switch to determine the row or col to increment or decrement.
    switch (move) {
        case North:
            pos->row -= 1;
            break;
        case East:
            pos->col += 1;
            break;
        case South:
            pos->row += 1;
            break;
        case West:
            pos->col -= 1;
            break;
    }
}

/**
 * Using a Stack and two dimensional character array. Solve the current maze by checking if a coordinate
 * is valid (Free) and Push this Point(row, col) to the Stack. If a dead end is ever encountered. Pop back until
 * a new coordinate is valid. This is done until the Point here col and row are equal to the end Point.
 */
void Maze::solve() {
    // Create the Stack that will hold the information about moves made inside of the maze.
    Stack pos;

    // Create a new Point here that holds the starting position inside of the maze,
    // (by default, this is set to row = 1, col = 0.
    Point here;

    std::cout << endl << maze[end.row][end.col] << endl << endl;

    while (!here.equals(end)) {

        // Check if a move is valid and make that move.
        // Order: East -> South -> West -> North.
        if (checkMove(here, East)) {
            Move(East, &pos, &here);
            continue;
        }
        if (checkMove(here, South)) {
            Move(South, &pos, &here);;
            continue;
        }
        if (checkMove(here, West)) {
            Move(West, &pos, &here);
            continue;
        }
        if (checkMove(here, North)) {
            Move(North, &pos, &here);
            continue;
        }

        // Only go here if no moves are valid at the moment, must check Stack to ensure it isn't empty,
        // then begin Popping from the stack and setting these dead ends to the mazeDetails Dead character.
        // This will happen indefinitely as long as no move is possible from the current Point here's coords.
        if (!pos.empty()) {
            // Set the current position in maze character to Dead.
            maze[here.row][here.col] = Dead;

            // Set current position to the top of the Stack.
            here = pos.getTop();

            // Lastly, Pop from the Stack to step backwards (backtrace) in maze.
            pos.Pop();
            continue;
        }
    }

    // Maze has been solved at this point, set the end point to the proper character.
    maze[end.row][end.col] = Used;
    clean();
    printBothMazes();

    saveMazeToFile();
}

/**
 * Clean up the current maze 2d array by iterating through each character in the array
 * and removing any instances of the enum Dead that come up, by default Dead = 'D'.
 */
void Maze::clean() {
    for (int i = 0; i < rows; i++) {
        for (int x = 0; x < cols; x++) {
            if (maze[i][x] == 'D') {
                maze[i][x] = ' ';
            }
        }
    }
}

/**
 * Save the maze into a new directory called 'solved' with the same name as the input file.
 */
void Maze::saveMazeToFile() {
    ofstream out;
    std::string outPath = "solved/" + file;
    out.open(outPath);

    for (int i = 0; i < rows; i++) {
        for (int x = 0; x < cols; x++) {
            out << maze[i][x];
        }
        out << "\r\n";
    }
    out.close();
}
