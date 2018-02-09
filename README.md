# MazeSolver



C++ console application that will solve a text maze using a *stack* data structure and *backtracking*.





# The Assumptions



The first thing that needs to be understood is the way a maze is parsed and generated. A couple of things are considered to be known before any parsing or solving:

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

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                                                  |

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+-+

|                                                  

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+



+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

##############################                    |

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+#+-+-+-+-+-+-+-+-+-+-+

|                            ######################

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

```



# The Structure



Before building an algorithm to solve the maze, the first thing I needed to do was come up with a way to read the text file directly into a data structure that makes sense for reading each character out of a grid.



A two-dimensional array makes sense for storing the character array.



To dynamically allocate the space required to store the text file containing the maze, I first had to come up with a way to calculate the size of the maze.

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



We now have access to a maze\[row\]\[col\] variable that is accessible throughout the entire Maze.cpp file.



## The Point Structure



The last part of designing the structure of the application included coming up with a simple way of storing information about the location inside of the grid. My solution for this was to create a new struct located inside of the StackNode.h header file.



```c

struct Point {

public:

    int col {0};

    int row {1};



    Point() = default;

    Point(int row, int col) : col(row), row(col) {}

    bool equals(Point p);

};

```



This has a few advantages...

- We can now Push a Point directly to our Stack when we need to add a new position to the maze.

- It is easier to read and organize our location data now that it's encapsulated in its own structure.

- We can create a method to compare points directly to each other.



At this point, I was ready to figure out a way to actually solve the maze.



# Solving The Maze



To solve this maze, there are a couple of things that need to be taken into account:

- The solution to the maze must contain only one path.

- No recursion may be used to solve the maze.

- A Custom Stack is used to solve the maze.



With that out of the way. The first step I took was thinking of a logical way to solve a maze in general.



*Pseudocode*

```

while (current != end) {

    check move east:

        move east;

        

    check move south:

        move south;

        

    check move west:

        move west;

        

    check move north:

        move north;

        

    check stack empty:

        set current -> deadend;

        current = top of stack;

        pop stack;

}

```



From this logic, we can confirm that a specified move is possible, if there is no possible move, it means we have reached a dead-end, so we can undo (Pop) from our Stack until a move is valid. Whenever a Pop() is performed, we also set that location in the maze to a Deadend character (in this case, I used a 'D').



The implemented version of the pseudocode above looks like:

```c

void Maze::solve() {

    Stack pos;

    while (!here.equals(end)) {

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

            maze[here.row][here.col] = Dead;

            here = pos.getTop();

            pos.Pop();

            continue;

        }

    }

}

```



Here is a visual representation of a maze being solved.



*Before solve():*

```

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                                                  |

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+-+

|                                                  

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

```



*After solve():*

```

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

##############################DDDDDDDDDDDDDDDDDDDD|

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+#+-+-+-+-+-+-+-+-+-+-+

|                            ######################

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

```



*After clean():*

```

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

##############################                    |

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+#+-+-+-+-+-+-+-+-+-+-+

|                            ######################

+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

```



### Contact

- If you have any questions, feel free to contact me @ theycallmevotum@gmail.com

