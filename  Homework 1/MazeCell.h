//
// Created by Sinem Kaya on 5.11.2023.
//
#ifndef UNTITLED20_MAZECELL_H
#define UNTITLED20_MAZECELL_H

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "MyStack.h"

using namespace std;

class MazeCell {
public:
    int xCor; //corresponding values
    int yCor;
    bool visited_in_create = false; //to track if we visited a cell while implementing the maze
    bool visited_in_path = false; //to track if we visited a cell while solving the maze
    int upper_wall;
    int lower_wall;
    int right_wall;
    int left_wall;


    bool operator!=(const MazeCell& other) const
    {
        return this->xCor != other.xCor || this->yCor != other.yCor;
    }

    MazeCell();
    MazeCell(int x, int y);
};

string pos_move_path (int x_coordinate, int y_coordinate, int xupBound, int yupBound, vector<vector<MazeCell>>& maze_vector);
string pos_move_string (int x_coordinate, int y_coordinate, int xupBound, int yupBound, vector<vector<MazeCell>> & maze_vector);
void MazeGenerate(int row_number, int column_number, vector<vector<MazeCell>> & maze_filled);
void print_Maze (ofstream& output, vector<vector<MazeCell>>& maze_vector);
void print_Path(ofstream& output, Node <MazeCell> * temp);
void implementMaze (vector<vector<vector<MazeCell>>> & Entire_Mazes, int K_num, int N_num, int M_num);
void create_Maze(MyStack <MazeCell> & stack_maze, vector <vector <vector<MazeCell>>> & Entire_Mazes, int exitpoint_X, int exitpoint_Y, int Maze_ID, int xupbound, int yupbound);

#endif //UNTITLED20_MAZECELL_H


