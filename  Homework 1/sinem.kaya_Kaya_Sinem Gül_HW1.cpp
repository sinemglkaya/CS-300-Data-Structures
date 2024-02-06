#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "MazeCell.h"
//#include "MazeCell.cpp"
#include "MyStack.h"
#include "MyStack.cpp"
#include "randgen.h"


using namespace std;


int main()
{
    int K_num, M_num, N_num;
    cout << "Enter the number of mazes: ";
    cin >> K_num;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M_num >> N_num;

    //to store all mazes, they will be created
    vector <vector <vector<MazeCell>>> Entire_Mazes;
    //use implement function to implement mazes
    implementMaze(Entire_Mazes, K_num, N_num, M_num);
    //To find a path, starting with a fresh stack

    cout << "All mazes are generated.\n" << endl;

    int Maze_ID = 0;
    cout << "Enter a maze ID between 1 to " << K_num << " inclusive to find a path: ";
    cin >> Maze_ID;

    int enter_xcor = 0, enter_ycor = 0;
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> enter_xcor >> enter_ycor;

    int exitpoint_X = 0, exitpoint_Y = 0;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> exitpoint_X >> exitpoint_Y;

    MyStack <MazeCell> stack_maze;

    //pushing enter_xcor and  enter_ycor to stack
    stack_maze.push(Entire_Mazes[Maze_ID - 1][enter_ycor][enter_xcor]);
    Entire_Mazes[Maze_ID - 1][enter_ycor][enter_xcor].visited_in_path = true;

    //boundries to create maze
    const int xupbound = Entire_Mazes[Maze_ID - 1][0].size() - 1;
    const int yupbound = Entire_Mazes[Maze_ID - 1].size() - 1;


    create_Maze(stack_maze, Entire_Mazes, exitpoint_X, exitpoint_Y, Maze_ID, xupbound, yupbound);
    ofstream finalPath;
    string name_path = "maze_" + to_string(Maze_ID) + "_path_" + to_string(enter_xcor)
                          + "_" + to_string(enter_ycor) + "_" + to_string(exitpoint_X)
                          + "_" + to_string(exitpoint_Y) + ".txt";

    finalPath.open(name_path);
    // we need to write on the txt file

    Node <MazeCell> * temp = stack_maze.top();
    print_Path(finalPath, temp);

    //emptying stack, freeing memory
    while (!stack_maze.isEmpty())
    {
        stack_maze.pop();
    }

    return 0;
}



