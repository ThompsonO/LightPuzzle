/*
  Oliver Thompson
  Homework 1
  board.h
*/

#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

extern const int BZERO;
extern const int BONE;
extern const int BTWO;
extern const int BTHREE;
extern const int NUMBERED;
extern const int BLANK;
extern const int UNLIT;
extern const int LIGHT;
extern const int LIT;

//Reads in a board from a file
void read_board(string puzzleFile, vector< vector<int> >& board);

//Generates a random board
void generate_board(int xsize, int ysize, vector< vector<int> >& board);

//Checks if a square is valid for a black square
bool blackValid(int x, int y, int num, int xmax, int ymax, vector< vector<int> >& board);

//Checks that a black square has no more than the maximum adjacent black squares
bool AdjacentMaxed(int x, int y, int xmax, int ymax, vector< vector<int> >& board);

//#include "board.cpp"

#endif