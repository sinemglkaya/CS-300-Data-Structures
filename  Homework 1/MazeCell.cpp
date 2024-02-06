//
// Created by Sinem Kaya on 5.11.2023.
//
#include "MazeCell.h"
#include "randgen.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "MyStack.h"

using namespace std;

// Implement the MazeCell constructor

MazeCell::MazeCell() {
    xCor = 0;
    yCor = 0;
    upper_wall = 0;
    lower_wall = 0;
    right_wall = 0;
    left_wall = 0;
}


MazeCell::MazeCell(int x, int y)
{
    this->xCor = x;
    this->yCor = y;
    upper_wall = 1;
    lower_wall = 1;
    right_wall = 1;
    left_wall = 1;
}


string pos_move_path (int x_coordinate, int y_coordinate, int xupBound, int yupBound, vector<vector<MazeCell>>& maze_vector)
{
    // Function to determine possible moves in the "path" context and return them as a string
    string result1 = "";

    if (x_coordinate + 1 <= xupBound && (maze_vector[y_coordinate][x_coordinate + 1].left_wall == 0 && maze_vector[y_coordinate][x_coordinate + 1].visited_in_path == false)) {

        result1 += 'R';
    }

    if ((y_coordinate + 1 <= yupBound)  && (maze_vector[y_coordinate + 1][x_coordinate].lower_wall == 0 && maze_vector[y_coordinate+1][x_coordinate].visited_in_path == false)){
            result1 += 'U';
    }
    if ( (x_coordinate - 1 >= 0)  && (maze_vector[y_coordinate][x_coordinate - 1].right_wall == 0 && maze_vector[y_coordinate][x_coordinate - 1].visited_in_path == false))
    {
            result1 += 'L';
    }
    if ((y_coordinate - 1 >= 0) && (maze_vector[y_coordinate - 1][x_coordinate].upper_wall == 0 && maze_vector[y_coordinate-1][x_coordinate].visited_in_path == false))
    {
            result1 += 'D';
    }
    return result1;
}

string pos_move_string (int x_coordinate, int y_coordinate, int xupBound, int yupBound, vector<vector<MazeCell>> & maze_vector)
{
    // Function to determine possible moves in the "string" context and return them as a string
    string result2 = "";

    if ((x_coordinate + 1 <= xupBound) && (maze_vector[y_coordinate][x_coordinate+1].visited_in_create == false) )
    {
        result2 += 'R';
    }
    if ((y_coordinate + 1 <= yupBound) && (maze_vector[y_coordinate+1][x_coordinate].visited_in_create == false)) {
        result2 += 'U';
    }
    if ((x_coordinate - 1 >= 0) && (maze_vector[y_coordinate][x_coordinate-1].visited_in_create == false)) {
        result2 += 'L';
    }
    if ((y_coordinate - 1 >= 0) && (maze_vector[y_coordinate-1][x_coordinate].visited_in_create == false)) {
        result2 += 'D';
    }
    return result2;
}

void MazeGenerate(int row_number, int column_number, vector<vector<MazeCell>> &maze_filled) {
    // Function to generate a maze and fill it in a 2D vector
    maze_filled.reserve(row_number);  // Reserve space for rows to minimize reallocation

    for (int row1 = 0; row1 < row_number; row1++) {
        vector<MazeCell> myVector;
        myVector.reserve(column_number);  // Reserve space for columns to minimize reallocation

        for (int col1 = 0; col1 < column_number; col1++) {
            myVector.emplace_back(col1, row1);
        }

        maze_filled.push_back(std::move(myVector));  // Move myVector into maze_filled
    }
}

void print_Maze (ofstream& output, vector<vector<MazeCell>>& maze_vector)
{
    // Function to print the maze details to an output stream
    for (int k = 0;k < maze_vector[0].size();k++) {
        for (int i = 0;i < maze_vector.size();i++) {
            output << "x=" << k << " y=" << i << " l = "<<maze_vector[i][k].left_wall
                <<" r="<< maze_vector[i][k].right_wall <<" u=" <<
                maze_vector[i][k].upper_wall <<" d="<< maze_vector[i][k].lower_wall <<endl;
        }
    }
}

void print_Path(ofstream& output, Node<MazeCell>* temp) {
    // Function to print the path details to an output stream using a linked list

    //check if the temp is null
    if (temp != nullptr) {
        print_Path(output, temp->next);
        output << temp->value.xCor << " " << temp->value.yCor << endl;
    }
}

void implementMaze(vector<vector<vector<MazeCell>>> & Entire_Mazes, int K_num, int N_num, int M_num)
{
    // Function to generate multiple mazes and store them in a vector of vectors
    int k = 0;
    while (k < K_num)
    {
        // Generate a maze
        vector<vector<MazeCell>> maze_vector;
        MazeGenerate(M_num, N_num,maze_vector);

        MyStack<MazeCell> stack_maze;
        //create a stackmaze to store MazeCell elements

        // Start at a random point in the maze

        int xupBound = maze_vector[0].size() - 1;
        int yupBound = maze_vector.size() - 1;
        maze_vector[0][0].visited_in_create = true;
        stack_maze.push(maze_vector[0][0]);

        int cellnum = M_num * N_num;
        int i = 0;

        while(i < cellnum -1)
        {
            bool isUsed = false;
            string move1 = pos_move_string(stack_maze.top()->value.xCor, stack_maze.top()->value.yCor, xupBound, yupBound,maze_vector);
            int move_length = move1.length();

            if (move_length == 0) {

                while (pos_move_string(stack_maze.top()->value.xCor, stack_maze.top()->value.yCor, xupBound, yupBound,maze_vector).length() == 0)
                {
                    //if there is no moves for top, keep doing pop
                    stack_maze.pop();
                }

                isUsed = true;
            }

            if (isUsed) {
                move1 = pos_move_string(stack_maze.top()->value.xCor, stack_maze.top()->value.yCor, xupBound, yupBound,maze_vector);
                move_length = move1.length();

            }

            RandGen random;

            int randomIndex = random.RandInt(0, move_length - 1);
            char Move_random = move1[randomIndex];

            int new_X = 0;
            int new_Y = 0;

            // store the xcor and ycor values in an integer
            int currentXCor = stack_maze.top()->value.xCor;
            int currentYCor = stack_maze.top()->value.yCor;

            switch (Move_random) {
                case 'R':
                    // Going Right
                    new_X = currentXCor + 1;
                    new_Y = currentYCor;
                    stack_maze.top()->value.right_wall = 0;
                    maze_vector[new_Y][new_X].left_wall = 0;
                    break;

                case 'L':
                    // Going Left
                    new_X = currentXCor - 1;
                    new_Y = currentYCor;
                    stack_maze.top()->value.left_wall = 0;
                    maze_vector[new_Y][new_X].right_wall = 0;
                    break;

                case 'U':
                    // Going Up
                    new_X = currentXCor;
                    new_Y = currentYCor + 1;
                    stack_maze.top()->value.upper_wall = 0;
                    maze_vector[new_Y][new_X].lower_wall = 0;
                    break;

                case 'D':
                    // Going Down
                    new_X = currentXCor;
                    new_Y = currentYCor - 1;
                    stack_maze.top()->value.lower_wall = 0;
                    maze_vector[new_Y][new_X].upper_wall = 0;
                    break;

                    // Handle other cases if needed
                default:
                    // Handle unexpected input
                    break;
            }
            //setting the cell which has been visited
            maze_vector[new_Y][new_X].visited_in_create = true;
            //pushing onto stack
            stack_maze.push(maze_vector[new_Y][new_X]);
            i++;
        }

        string file_name2 = "maze_" + to_string(k+1) + ".txt";
        ofstream output;
        output.open(file_name2);

        output << M_num << " " << N_num << endl;

        print_Maze(output, maze_vector);

        Entire_Mazes.push_back(maze_vector);

        //emptying stack, freeing memory

        while (!stack_maze.isEmpty())
        {
            stack_maze.pop();
        }
        k++;
    }
}

void create_Maze(MyStack <MazeCell> & stack_maze, vector <vector <vector<MazeCell>>> & Entire_Mazes, int exitpoint_X, int exitpoint_Y, int Maze_ID, int xupbound, int yupbound)
{

    do
    {
        // Calculate the possible moves for the current cell
        string move2 = pos_move_path(stack_maze.top()-> value.xCor,stack_maze.top()-> value.yCor, xupbound, yupbound,Entire_Mazes[Maze_ID - 1]);
        int move2_Length = move2.length(); //length of the integer
        bool flag1 = false;
        // Check if there are no possible moves
        if (move2_Length == 0) {
            do {
                stack_maze.pop();
            } while (!stack_maze.isEmpty() && pos_move_path(stack_maze.top()-> value.xCor,stack_maze.top()-> value.yCor, xupbound, yupbound,Entire_Mazes[Maze_ID - 1]).length() == 0);
            flag1 = true;
        }

        // If there are no possible moves and a backtrack has occurred, recompute the moves
        if (flag1 == true)
        {
            move2 = pos_move_path(stack_maze.top()-> value.xCor,stack_maze.top()-> value.yCor, xupbound, yupbound,Entire_Mazes[Maze_ID - 1]);
            move2_Length = move2.length();
        }
        // If there are no possible moves, continue to the next iteration
        if (move2_Length == 0) {
            continue;
        }

        // Choose a random move from the available moves
        RandGen random;
        int randomIndex = random.RandInt(move2_Length); // RandInt is inclusive of 0 and exclusive of max
        char random_Move = move2[randomIndex];
        int new_X = 0;
        int new_Y = 0;
        int currentXCor = stack_maze.top()->value.xCor;
        int currentYCor = stack_maze.top()->value.yCor;
        // Update the new position based on the chosen move
        switch (random_Move)
        {
            case 'L': // Going left
            {
                new_X = currentXCor - 1;
                new_Y = currentYCor;
            }
                break;
            case 'R': // Going right
                {
                    new_X = currentXCor + 1;
                    new_Y = currentYCor;
                }
                break;
            case 'D': // Going down
            {
                new_X = currentXCor;
                new_Y = currentYCor - 1;
            }
                break;
            case 'U': // Going up
                {
                    new_X = currentXCor;
                    new_Y = currentYCor + 1;
                }
                break;
        }
        // Mark the new cell as visited in the path and update the stack
        Entire_Mazes[Maze_ID - 1][new_Y][new_X].visited_in_path = true;
        stack_maze.push(Entire_Mazes[Maze_ID - 1][new_Y][new_X]);
    }while (stack_maze.top()->value != Entire_Mazes[Maze_ID - 1][exitpoint_Y][exitpoint_X]);
}





