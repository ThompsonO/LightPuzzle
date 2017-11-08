#ifndef RANDPLACEMENT_H
#define RANDPLACEMENT_H

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

//Randomly Places Bulbs in valid locations
void place_bulbs(vector< vector<int> >& board);

//Makes a random solution with black square numbers being satisified
void valid_forced(vector< vector<int> >& board);

//Returns the fitness value
int fitness_eval(vector< vector<int> >& board);

//Returns true if the space is not adjacent to any black squares that have enough lights next to them
bool lit_up(int x, int y, vector< vector<int> >& board);

//Returns true if the black square has enough adjacent lights
bool adjacent_lights(int x, int y, vector< vector<int> >& board);

//Returns true if board is a valid solution
bool valid_board(vector< vector<int> >& board);

//Returns the fitness value ignoring the numbers on black squares
int numberless_fitness_eval(vector< vector<int> >& board);

//Recombines two parents into a child
vector< vector<int> > recombination(vector< vector<int> >& par, vector< vector<int> >& mar);

//Mutates child
void mutate(vector< vector<int> >& child);

//Gives fitness with a penalty function
int constraint_fitness(vector< vector<int> >& board);

//Returns the number of lit sells in the board
int lit_cells(vector< vector<int> >& board);

//Returns a number decremented each time there is a conflicting light
int conflicting_lights(vector< vector<int> >& board);

//Returns a number decremented each time there is a black box violation
int cell_violations(vector< vector<int> >& board);

//Returns a vector with the rankings of solutions based on dominance
vector<int> domination(vector<int>& lit, vector<int>& lights, vector<int>& black);

//#include "randplacement.cpp"

#endif