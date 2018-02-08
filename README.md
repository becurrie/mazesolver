# MazeSolver

C++ console application that will solve a text maze using a *stack* data structure and *backtracking*.


# The Assumptions

The first thing that needs to be understood is the way in which a maze is parsed and generated. A couple of things are considered to be known before any parsing or solving:
- The maze will always be in a **row** x **col** grid.
- The maze will not always be the same size.
- The entry point in the maze will always be at top left-hand corner (0, 1).
- The exit point will always be at bottom right-hand (rows - 1, cols - 2).

# The Maze

The maze itself is comprised of a bunch of characters inside of a text file.
- Walls in the maze: ('+', '-', '|').
- Empty Pathway in the maze: (' ').
- Used Pathway in the maze: ('#').

A good way to generate these mazes is to use the [Delorie](http://www.delorie.com/game-room/mazes/genmaze.cgi) maze generator that allows for you to create a maze with any row x col values you choose.

#### Example Solution
```
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                               
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
###############################
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

# The Structure

Before building an algorithm to solve the maze, the first thing I needed to do was come up with a way to read the text file directly into a data structure that makes sense for reading individual characters out of a grid.

A two-dimensional array makes sense for storing the character array.

Two dynamically allocate the space required to store the text file containing the maze, I first had to come up with a way to calculate the size of the maze.
```c
void Maze::setMazeSize() {
    ifstream f(dir + file);
    
    int tempCols = 0;
    while (!f.eof()) {
        rows += 1;
        string line;
        getline(f, line);
        
        for (char c : line) {
            if (c != '\r')
                tempCols += 1;
        }
        
        if (cols == 0)
            cols = tempCols;
    }
    f.close();
```

- Because we are working with grids, we know that the columns of a maze will not differ after the first row columns are calculated. so we set the cols once in the loop. Each row or (!f.eof()) will increment our rows by one.

Once the **rows** and **cols** variables have been set, we can begin dynamically allocating the space required to store our new maze.

```c
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
```

- This function begins by setting our maze variable: ``char** maze;`` to a new char*[rows]
- Then we will loop through each row and allocate our cols variable into our maze variable:
```c
 for (int i = 0; i < rows; i++) {
        maze[i] = new char[cols];
}
```
