/*
  Oliver Thompson
  Homework 1
  board.cpp
*/

#include "board.h"

//Reads in a board from file
void read_board(string puzzleFile, vector< vector<int> >& board)
{
  int xmax, ymax, tempx, tempy, blackNum;
  string temp, numGetter;
  ifstream puzzle;
  stringstream converter;
  stringstream puzzleName;
  string puzzleTest;

  puzzleName << puzzleFile;
  puzzleName >> puzzleTest;

  //Opens the puzzle file
  puzzle.open(puzzleTest.c_str(), ifstream::in);

  //Gets xmax
  getline(puzzle, temp);
  converter.str("");
  converter.clear();
  converter.str(temp);
  converter >> xmax;


  //Gets ymax
  getline(puzzle, temp);
  converter.str("");
  converter.clear();
  converter.str(temp);
  converter >> ymax;

  //Empties board if it has data
  if(board.empty() == false)
  {
    for(unsigned int i=0; i<board.size(); i++)
    {
      while(board[i].empty() == false)
      {
        board[i].erase(board[i].begin());
      }
    }

    while(board.empty() == false)
    {
      board.erase(board.begin());
    }
  }

  //Initializes all coordinates of the board
  vector<int> tempVector;
  for(int k=1; k<=ymax; k++)
  {
    tempVector.push_back(UNLIT);
  }
  for(int i=1; i<=xmax; i++)
  {
    board.push_back(tempVector);
  }

  //Marks black squares in board
  while(puzzle.good())
  {

    getline(puzzle, temp);
    converter.str("");
    converter.clear();
    converter << temp;
    converter >> tempx >> tempy >> blackNum;

    //Places black square in board
    board[tempx-1][tempy-1] = blackNum;

  }

  puzzle.close();

  return;
}


//CHANGE METHOD OF GENERATION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Generates a random solvable board
void generate_board(int xsize, int ysize, vector< vector<int> >& board)
{
  int tempx, tempy, numBlack, tempNum;

  //Empties board if it has data
  if(board.empty() == false)
  {
    for(unsigned int i=0; i<board.size(); i++)
    {
      while(board[i].empty() == false)
      {
        board[i].erase(board[i].begin());
      }
    }

    while(board.empty() == false)
    {
      board.erase(board.begin());
    }
  }

  //Initializes all coordinates of the board
  vector<int> tempVector;
  for(int k=1; k<=ysize; k++)
  {
    tempVector.push_back(UNLIT);
  }
  for(int i=1; i<=xsize; i++)
  {
    board.push_back(tempVector);
  }

  //Generates a random maximum number of black squares that will fill the board
  numBlack = rand() % (xsize*ysize);

  //Places Black Squares
  for(int i=0; i<numBlack; i++)
  {
    //Generates locations and numbers for black squares
    //tempx and tempy are index+1
    tempx = (rand() % xsize) + 1;
    tempy = (rand() % ysize) + 1;
    tempNum = rand() % UNLIT;

    //Places Black Square if valid location
    if(blackValid(tempx, tempy, tempNum, xsize, ysize, board))
    {
      //Adds tempNum to board
      board[tempx-1][tempy-1] = tempNum;
    }

  }

  return;
}

//Checks if black square is in a valid location
//x and y are index+1
bool blackValid(int x, int y, int num, int xmax, int ymax, vector< vector<int> >& board)
{
  //Maximum number of adjacent black squares
  int maxAdjacent;

  //Sets maximum number of adjacent black squares
  if(num < BLANK)
  {
    maxAdjacent = NUMBERED-num;
  }
  else
  {
    maxAdjacent = NUMBERED;
  }

  //Returns false if square is a 4 and on the edge
  if((num == NUMBERED) && (x==1 || y==1 || x==xmax || y==ymax))
  {
    return false;
  }

  //Returns false if square is a 3 and in a corner
  if((num == 3) && ((x==1 && y==1) || (x==1 && y==ymax) || (x==xmax && y==1) || (x==xmax && y==ymax)))
  {
    return false;
  }

  if(board.empty() == false)
  {
    //Checks that a square doesn't already occupy the space and there are not too many adjacent squares
    if(board[x-1][y-1] >= BZERO && board[x-1][y-1] <= BLANK)
    {
      return false;
    }

    //Adjacent black squares
    //Makes sure location is not on the right edge
    if(x<xmax)
    {
      if((board[x][y-1] <= BLANK && board[x][y-1] >= BZERO)) /*Right*/
      {
        maxAdjacent--;
        
        //Returns false if adjacent black squares have maximum adjacent black squares
        if(AdjacentMaxed(x, y-1, xmax, ymax, board))
        {
          return false;
        }
      }
    }

    //Makes sure location is not on the left edge
    if(x!=1)
    {
      if((board[x-2][y-1] <= BLANK && board[x-2][y-1] >= BZERO)) /*Left*/
      {
        maxAdjacent--;

        //Returns false if adjacent black squares have maximum adjacent black squares
        if(AdjacentMaxed(x-2, y-1, xmax, ymax, board))
        {
          return false;
        }
      }
    }

    //Makes sure location is not on the upper edge
    if(y<ymax)
    {
      if((board[x-1][y] <= BLANK && board[x-1][y] >= BZERO)) /*Up*/
      {
        maxAdjacent--;

        //Returns false if adjacent black squares have maximum adjacent black squares
        if(AdjacentMaxed(x-1, y, xmax, ymax, board))
        {
          return false;
        }
      }
    }

    //Makes sure location is not on the bottom edge
    if(y!=1)
    {
      if((board[x-1][y-2] <= BLANK && board[x-1][y-2] >= BZERO)) /*Down*/
      {
          maxAdjacent--;

        //Returns false if adjacent black squares have maximum adjacent black squares
        if(AdjacentMaxed(x-1, y-2, xmax, ymax, board))
        {
          return false;
        }
      }
    }

    //Returns false if too many adjacent black squares
    if(maxAdjacent < 0)
    {
      return false;
    }

  }

  else
  {
    return true;
  }

  return true;
}

//Returns true if black square has maximum number of adjacent black squares
//x and y are true indecies
bool AdjacentMaxed(int x, int y, int xmax, int ymax, vector< vector<int> >& board)
{
  int maxAdjacent;

  //Finds maximum number of adjacent black squares
  if((board[x][y])<BLANK)
  {
    maxAdjacent = NUMBERED-(board[x][y]);
  }
  else
  {
    maxAdjacent = NUMBERED;
  }

  //If maximum number of squares already present, returns false
  if(maxAdjacent==0)
  {
    return false;
  }

  //Adjacent black squares
    //Makes sure location is not on the right edge
    if(x!=(xmax-1))
    {
      if((board[x+1][y] <= BLANK && board[x+1][y] >= BZERO)) /*Right*/
      {
        maxAdjacent--;
      }
    }

    //Makes sure location is not on the left edge
    if(x!=0)
    {
      if((board[x-1][y] <= BLANK && board[x-1][y] >= BZERO)) /*Left*/
      {
        maxAdjacent--;
      }
    }

    //Makes sure location is not on the upper edge
    if(y!=(ymax-1))
    {
      if((board[x][y+1] <= BLANK && board[x][y+1] >= BZERO)) /*Up*/
      {
        maxAdjacent--;
      }
    }

    //Makes sure location is not on the bottom edge
    if(y!=0)
    {
      if((board[x][y-1] <= BLANK && board[x][y-1] >= BZERO)) /*Down*/
      {
          maxAdjacent--;
      }
    }

    //Returns false if too many adjacent black squares
    if(maxAdjacent <= 0)
    {
      return false;
    }

  //If no problems, returns true
  return true;
}