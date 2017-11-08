/*
  Oliver Thompson
  Homework 1
  randplacement.cpp
*/

#include "randplacement.h"

//Randomly places bulbs in valid locations
void place_bulbs(vector< vector<int> >& board)
{
  int tempx, tempy, index;
  unsigned int uIndex;

  //Generates random locations to puts lights and places them if valid
  for(unsigned int i=0; i<board.size(); i++)
  {
    for(unsigned int k=0; k<board[i].size(); k++)
    {
      tempx = rand() % board.size();
      tempy = rand() % board[i].size();

      //Places light if valid location
      if(board[tempx][tempy] == UNLIT && lit_up(tempx, tempy, board))
      {
        board[tempx][tempy] = LIGHT;

        //Change unlit squares to lit squares
        //Look to the right
        index = tempx;
        uIndex = index;
        while(uIndex < board.size() && board[index][tempy] > BLANK)
        {

          if(board[index][tempy] == UNLIT)
          {
            board[index][tempy] = LIT;
          }
          index++;
          uIndex++;
        }

        //Look to the left
        index = tempx;
        uIndex = index;
        while(index >= 0 && board[index][tempy] > BLANK)
        {
          if(board[index][tempy] == UNLIT)
          {
            board[index][tempy] = LIT;
          }
          index--;
        }

        //Look upwards
        index = tempy;
        uIndex = index;
        while(uIndex < board[0].size() && board[tempx][index] > BLANK)
        {
          if(board[tempx][index] == UNLIT)
          {
            board[tempx][index] = LIT;
          }
          index++;
          uIndex++;
        }

        //Look downwards
        index = tempy;
        uIndex = index;
        while(index >= 0 && board[tempx][index] > BLANK)
        {
          if(board[tempx][index] == UNLIT)
          {
            board[tempx][index] = LIT;
          }
          index--;
        }
      }
    }
  }

  return;
}

//Makes a random solution with black square numbers being satisified
void valid_forced(vector< vector<int> >& board)
{
  unsigned int temp;
  int stemp;

  //Searches for cases where black squares have one option for bulb placement
  for(unsigned int i=0; i<board.size(); i++)
  {
    for(unsigned int k=0; k<board[i].size(); k++)
    {
      //Places 4 bulbs around a square with a 4
      if(board[i][k] == NUMBERED)
      {
        //Right bulb
        temp = i+1;
        board[temp][k] = LIGHT;
        temp++;
        while(temp < board.size() && board[temp][k] > BLANK)
        {
          board[temp][k] = LIT;
        }

        //Left bulb
        stemp = i-1;
        board[stemp][k] = LIGHT;
        stemp--;
        while(stemp >= 0 && board[stemp][k] > BLANK)
        {
          board[stemp][k] = LIT;
        }

        //Up bulb
        temp = k+1;
        board[i][temp] = LIGHT;
        temp++;
        while(temp < board[i].size() && board[i][temp] > BLANK)
        {
          board[i][temp] = LIT;
        }

        //Down bulb
        stemp = k-1;
        board[i][stemp] = LIGHT;
        stemp--;
        while(stemp >= 0 && board[i][stemp] > BLANK)
        {
          board[i][stemp] = LIT;
        }
      }

      //Places bulbs around 3's on edges or with an adjacent black square
      if(board[i][k] == BTHREE && ((i == 0 || k == 0 || i == board.size()-1 || k == board[i].size()-1) ||
        ((i != board.size()-1 && board[i+1][k] <= BLANK) || (i != 0 && board[i-1][k] <= BLANK) ||
        (k != board[i].size()-1 && board[i][k+1] <= BLANK) || (k != 0 && board[i][k-1] <= BLANK))))
      {
        //Right Bulb
        if(i != board.size()-1)
        {
          temp = i+1;
          if(board[temp][k] == UNLIT)
          {
            board[temp][k] = LIGHT;
            temp++;
            while(temp < board.size() && board[temp][k] > BLANK)
            {
              board[temp][k] = LIT;
            }
          }
        }

        //Left Bulb
        if(i != 0)
        {
          stemp = i-1;
          if(board[stemp][k] == UNLIT)
          {
            board[stemp][k] = LIGHT;
            stemp--;
            while(stemp >= 0 && board[stemp][k] > BLANK)
            {
              board[stemp][k] = LIT;
            }
          }
        }

        //Up Bulb
        if(k != board[i].size()-1)
        {
          temp = k+1;
          if(board[i][temp] == UNLIT)
          {
            board[i][temp] = LIGHT;
            temp++;
            while(temp < board[i].size() && board[i][temp] > BLANK)
            {
              board[i][temp] = LIT;
            }
          }
        }

        //Down Bulb
        if(k != 0)
        {
          stemp = k-1;
          if(board[i][stemp] == UNLIT)
          {
            board[i][stemp] = LIGHT;
            stemp--;
            while(stemp >= 0 && board[i][stemp] > BLANK)
            {
              board[i][stemp] = LIT;
            }
          }
        }
      }

      //Places bulbs next to 2's in corners or with adjacent black boxes!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //Places bulbs next to 1's with adjacent black boxes!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    }
  }

  return;
}

//Returns the fitness value
int fitness_eval(vector< vector<int> >& board)
{
  int fitness=0;
  int totalfitness=0;

  //Evaluates fitness if board is a valid solution
  if(valid_board(board))
  {
    //Increments for each lit up square
    for(unsigned int i=0; i<board.size(); i++)
    {
      for(unsigned int k=0; k<board[i].size(); k++)
      {
        if(board[i][k] == LIGHT || board[i][k] == LIT)
        {
          fitness++;
          totalfitness++;
        }

        //Increments for unlit squares to establish a proper ratio
        if(board[i][k] == UNLIT)
        {
          totalfitness++;
        }
      }
    }
  }

  //Gives fitness of 0 to invalid solutions
  else
  {
    fitness = 0;
    totalfitness = 1;
  }

  return (fitness);
}

//Returns true if the space is not adjacent to any black squares that have enough lights next to them
//x and y are true indices
bool lit_up(int x, int y, vector< vector<int> >& board)
{
  unsigned int uX;
  unsigned int uY;

  uX = x;
  uY = y;

  //Makes sure location isn't on the right edge
  if(uX!=(board.size()-1))
  {
    if(board[x+1][y] <= NUMBERED)
    {
      if(board[x+1][y] == BZERO)
      {
        return false;
      }

      if(adjacent_lights(x+1, y, board))
      {
        return false;
      }
    }
  }

  //Makes sure location isn't on the left edge
  if(uX!=0)
  {
    if(board[x-1][y] <= NUMBERED)
    {
      if(board[x-1][y] == BZERO)
      {
        return false;
      }

      if(adjacent_lights(x-1, y, board))
      {
        return false;
      }
    }
  }

  //Makes sure location isn't on the upper edge
  if(uY!=board[0].size()-1)
  {
    if(board[x][y+1] <= NUMBERED)
    {
      if(board[x][y+1] == BZERO)
      {
        return false;
      }

      if(adjacent_lights(x, y+1, board))
      {
        return false;
      }
    }
  }

  //Makes sure location isn't on the bottom edge
  if(uY!=0)
  {
    if(board[x][y-1] <= NUMBERED)
    {
      if(board[x][y-1] == BZERO)
      {
        return false;
      }

      if(adjacent_lights(x, y-1, board))
      {
        return false;
      }
    }
  }

  //Returns true otherwise
  return true;
}

//Returns true if the black square has enough adjacent lights
//x and y are true indicies
bool adjacent_lights(int x, int y, vector< vector<int> >& board)
{
  int lights = 0;
  unsigned int uX, uY;

  uX = x;
  uY = y;
  
  //Makes sure location isn't on the right edge
  if(uX!=board.size()-1)
  {
    if(board[x+1][y] == LIGHT)
    {
      lights++;
    }
  }

  //Makes sure location isn't on the left edge
  if(uX!=0)
  {
    if(board[x-1][y] == LIGHT)
    {
      lights++;
    }
  }

  //Makes sure location isn't on the upper edge
  if(uY!=board[0].size()-1)
  {
    if(board[x][y+1] == LIGHT)
    {
      lights++;
    }
  }

  //Makes sure location isn't on the bottom edge
  if(uY!=0)
  {
    if(board[x][y-1] == LIGHT)
    {
      lights++;
    }
  }

  //Returns true if number of lights equals number on square
  if(lights >= board[x][y])
  {
    return true;
  }

  //Otherwise returns false
  return false;
}

//Returns true if board is a valid solution
bool valid_board(vector< vector<int> >& board)
{
  for(unsigned int i=0; i<board.size(); i++)
  {
    for(unsigned int k=0; k<board[i].size(); k++)
    {
      //If space is a black numbered square then checks for enough adjacent lights
      if(board[i][k] <= NUMBERED)
      {
        //Returns false if not enough adjacent lights
        if(adjacent_lights(i, k, board) == false)
        {
          return false;
        }
      }

      //If space is a light, checks for conflicting lights
      if(board[i][k] == LIGHT)
      {
        unsigned int index = i+1;
        int sindex;
        //Looks right
        while(index < board.size() && board[index][k] == LIT)
        {
          index++;
        }

        if(index < board.size() && board[index][k] == LIGHT)
        {
          return false;
        }

        sindex = i-1;
        //Looks left
        while(sindex >= 0 && board[sindex][k] == LIT)
        {
          sindex--;
        }

        if(sindex >= 0 && board[sindex][k] == LIGHT)
        {
          return false;
        }

        index = k+1;
        //Looks up
        while(index < board[i].size() && board[i][index] == LIT)
        {
          index++;
        }

        if(index < board[i].size() && board[i][index] == LIGHT)
        {
          return false;
        }

        sindex = k-1;
        //Looks down
        while(sindex >= 0 && board[i][sindex] == LIT)
        {
          sindex--;
        }

        if(sindex >= 0 && board[i][sindex] == LIGHT)
        {
          return false;
        }
      }
    }
  }
  //Otherwise returns true
  return true;
}

//Returns the fitness value
int numberless_fitness_eval(vector< vector<int> >& board)
{
  float fitness=0;
  float totalfitness=0;

  //Evaluates fitness
  //Increments for each lit up square
  for(unsigned int i=0; i<board.size(); i++)
  {
    for(unsigned int k=0; k<board[i].size(); k++)
    {
      if(board[i][k] == LIGHT || board[i][k] == LIT)
      {
        fitness++;
        totalfitness++;
      }

      //Increments for unlit squares to establish a proper ratio
      if(board[i][k] == UNLIT)
      {
        totalfitness++;
      }
    }
  }

  for(unsigned int i=0; i<board.size(); i++)
  {
    for(unsigned int k=0; k<board.size(); k++)
    {
      //If space is a light, checks for conflicting lights
      //Sets fitness to 0 if a conflicting light is found
      if(board[i][k] == LIGHT)
      {
        unsigned int index = i+1;
        int sindex;
        //Looks right
        while(index < board.size() && board[index][k] == LIT)
        {
          index++;
        }

        if(index < board.size() && board[index][k] == LIGHT)
        {
          fitness = 0;
          totalfitness = 1;
        }

        sindex = i-1;
        //Looks left
        while(sindex >= 0 && board[sindex][k] == LIT)
        {
          sindex--;
        }

        if(sindex >= 0 && board[sindex][k] == LIGHT)
        {
          fitness = 0;
          totalfitness = 1;
        }

        index = k+1;
        //Looks up
        while(index < board[i].size() && board[i][index] == LIT)
        {
          index++;
        }

        if(index < board[i].size() && board[i][index] == LIGHT)
        {
          fitness = 0;
          totalfitness = 1;
        }

        sindex = k-1;
        //Looks down
        while(sindex >= 0 && board[i][sindex] == LIT)
        {
          sindex--;
        }

        if(sindex >= 0 && board[i][sindex] == LIGHT)
        {
          fitness = 0;
          totalfitness = 1;
        }
      }
    }
  }

  return (fitness);
}

//Recombines two parents into a child
vector< vector<int> > recombination(vector< vector<int> >& par, vector< vector<int> >& mar)
{
  vector< vector<int> > child;
  vector<int> tempVector;

  //Generates an empty board
  for(unsigned int k=0; k<par[0].size(); k++)
  {
    tempVector.push_back(UNLIT);
  }

  for(unsigned int i=0; i<par.size(); i++)
  {
    child.push_back(tempVector);
  }

  //Copies half of the board from par
  for(unsigned int i=0; i<(par.size()/2); i++)
  {
    for(unsigned int k=0; k<par[i].size(); k++)
    {
      if(par[i][k] < LIT)
      {
        child[i][k] = par[i][k];
      }
    }
  }

  //Copies half of the board from mar
  for(unsigned int i=((mar.size()/2)+1); i<mar.size(); i++)
  {
    for(unsigned int k=0; k<mar[i].size(); k++)
    {
      if(mar[i][k] < LIT)
      {
        child[i][k] = mar[i][k];
      }
    }
  }

  //Lights board according to bulb placement
  for(unsigned int i=0; i<child.size(); i++)
  {
    for(unsigned int k=0; k<child[i].size(); k++)
    {
      if(child[i][k] == LIGHT)
      {
        unsigned int index = i+1;
        int sindex;
        //Look to the right
        while(index < child.size() && child[index][k] == UNLIT)
        {
          child[index][k] = LIT;
          index++;
        }

        sindex = i-1;
        //Look to the left
        while(sindex >= 0 && child[sindex][k] == UNLIT)
        {
          child[sindex][k] = LIT;
          sindex--;
        }

        index = k+1;
        //Look up
        while(index < child[i].size() && child[i][index] == UNLIT)
        {
          child[i][index] = LIT;
          index++;
        }

        sindex = k-1;
        //Look down
        while(sindex >= 0 && child[i][sindex] == UNLIT)
        {
          child[i][sindex] = LIT;
          sindex--;
        }
      }
    }
  }

  return child;
}

//Mutates child
void mutate(vector< vector<int> >& child)
{
  int chance = rand() % 3;
  vector<int> xlocs;
  vector<int> ylocs;
  int index;
  unsigned int temp;
  int stemp;

  //Removes a bulb
  if(chance == 0)
  {
    //Finds lights on the board
    for(unsigned int i=0; i<child.size(); i++)
    {
      for(unsigned int k=0; k<child[i].size(); k++)
      {
        if(child[i][k] == LIGHT)
        {
          xlocs.push_back(i);
          ylocs.push_back(k);
        }
      }
    }

    if(xlocs.empty() == false)
    {
      //Selects a random light
      index = rand() % xlocs.size();

      //Removes that light
      child[xlocs[index]][ylocs[index]] = UNLIT;

      //Unlights squares lit by the removed light
      //Looks right
      temp = xlocs[index] + 1;
    
      while(temp < child.size() && child[temp][ylocs[index]] == LIT)
      {
        child[temp][ylocs[index]] = UNLIT;
        temp++;
      }

      //Looks left
      stemp = xlocs[index] - 1;
    
      if(stemp >= 0 && child[stemp][ylocs[index]] == LIT)
      {
        child[stemp][ylocs[index]] = UNLIT;
        stemp--;
      }

      //Looks up
      temp = ylocs[index] + 1;
    
      while(temp < child[xlocs[index]].size() && child[xlocs[index]][temp] == LIT)
      {
        child[xlocs[index]][temp] = UNLIT;
        temp++;
      }

      //Looks down
      stemp = ylocs[index] - 1;
    
      if(stemp >= 0 && child[xlocs[index]][stemp] == LIT)
      {
        child[xlocs[index]][stemp] = UNLIT;
        stemp--;
      }

      //Relights squares that are lit by other lights that were unlit by the previous block of code
      for(unsigned int i=0; i<child.size(); i++)
      {
        for(unsigned int k=0; k<child[i].size(); k++)
        {
          //Finds remaining lights on board
          if(child[xlocs[index]][ylocs[index]] == LIGHT)
          {
            //Looks right
            temp = xlocs[index]+1;
            while(temp < child.size() && child[temp][ylocs[index]] > BLANK && child[temp][ylocs[index]] != LIGHT)
            {
              child[temp][ylocs[index]] = LIT;
              temp++;
            }

            //Looks left
            stemp = xlocs[index]-1;
            while(stemp >= 0 && child[stemp][ylocs[index]] > BLANK && child[stemp][ylocs[index]] != LIGHT)
            {
              child[stemp][ylocs[index]] = LIT;
              stemp--;
            }

            //Looks up
            temp = ylocs[index]+1;
            while(temp < child[xlocs[index]].size() && child[xlocs[index]][temp] > BLANK && child[xlocs[index]][temp] != LIGHT)
            {
              child[xlocs[index]][temp] = LIT;
              temp++;
            }

            //Looks down
            stemp = ylocs[index]-1;
            while(stemp >=0 && child[xlocs[index]][stemp] > BLANK && child[xlocs[index]][stemp] != LIGHT)
            {
              child[xlocs[index]][stemp] = LIT;
              stemp--;
            }
          }
        }
      }
    }
  }

  //Places a blub
  else if(chance == 1)
  {
    //Finds unlit squares on the board
    for(unsigned int i=0; i<child.size(); i++)
    {
      for(unsigned int k=0; k<child[i].size(); k++)
      {
        if(child[i][k] == UNLIT)
        {
          xlocs.push_back(i);
          ylocs.push_back(k);
        }
      }
    }

    if(xlocs.empty() == false)
    {
      //Randomly selects an unlit square
      index = rand() % xlocs.size();

      //Places a light in the square
      child[xlocs[index]][ylocs[index]] = LIGHT;

      //Lights squares in line with the new light
      //Looks right
      temp = xlocs[index] + 1;
    
      while(temp < child.size() && child[temp][ylocs[index]] > BLANK && child[temp][ylocs[index]] != LIGHT)
      {
        child[temp][ylocs[index]] = LIT;
        temp++;
      }

      //Looks left
      stemp = xlocs[index] - 1;
    
      if(stemp >= 0 && child[stemp][ylocs[index]] > BLANK && child[stemp][ylocs[index]] != LIGHT)
      {
        child[stemp][ylocs[index]] = LIT;
        stemp--;
      }

      //Looks up
      temp = ylocs[index] + 1;
    
      while(temp < child[xlocs[index]].size() && child[xlocs[index]][temp] > BLANK && child[xlocs[index]][temp] != LIGHT)
      {
        child[xlocs[index]][temp] = LIT;
        temp++;
      }

      //Looks down
      stemp = ylocs[index] - 1;
    
      if(stemp >= 0 && child[xlocs[index]][stemp] > BLANK && child[xlocs[index]][stemp] != LIGHT)
      {
        child[xlocs[index]][stemp] = LIT;
        stemp--;
      }
    }
  }

  //Removes and Places a bulb
  else if(chance == 2)
  {

//cout << "\t Removes and Replaces" << endl;

    //Finds lights on the board
    for(unsigned int i=0; i<child.size(); i++)
    {
      for(unsigned int k=0; k<child[i].size(); k++)
      {
        if(child[i][k] == LIGHT)
        {
          xlocs.push_back(i);
          ylocs.push_back(k);
        }
      }
    }

    if(xlocs.empty() == false)
    {
      //Selects a random light
      index = rand() % xlocs.size();

      //Removes that light
      child[xlocs[index]][ylocs[index]] = UNLIT;

      //Unlights squares lit by the removed light
      //Looks right
      temp = xlocs[index] + 1;
    
      while(temp < child.size() && child[temp][ylocs[index]] == LIT)
      {
        child[temp][ylocs[index]] = UNLIT;
        temp++;
      }

      //Looks left
      stemp = xlocs[index] - 1;
    
      if(stemp >= 0 && child[stemp][ylocs[index]] == LIT)
      {
        child[stemp][ylocs[index]] = UNLIT;
        stemp--;
      }

      //Looks up
      temp = ylocs[index] + 1;
    
      while(temp < child[xlocs[index]].size() && child[xlocs[index]][temp] == LIT)
      {
        child[xlocs[index]][temp] = UNLIT;
        temp++;
      }

      //Looks down
      stemp = ylocs[index] - 1;
    
      if(stemp >= 0 && child[xlocs[index]][stemp] == LIT)
      {
        child[xlocs[index]][stemp] = UNLIT;
        stemp--;
      }

      //Relights squares that are lit by other lights that were unlit by the previous block of code
      for(unsigned int i=0; i<child.size(); i++)
      {
        for(unsigned int k=0; k<child[i].size(); k++)
        {
          //Finds remaining lights on board
          if(child[xlocs[index]][ylocs[index]] == LIGHT)
          {
            //Looks right
            temp = xlocs[index]+1;
            while(temp < child.size() && child[temp][ylocs[index]] > BLANK && child[temp][ylocs[index]] != LIGHT)
            {
              child[temp][ylocs[index]] = LIT;
              temp++;
            }

            //Looks left
            stemp = xlocs[index]-1;
            while(stemp >= 0 && child[stemp][ylocs[index]] > BLANK && child[stemp][ylocs[index]] != LIGHT)
            {
              child[stemp][ylocs[index]] = LIT;
              stemp--;
            }

            //Looks up
            temp = ylocs[index]+1;
            while(temp < child[xlocs[index]].size() && child[xlocs[index]][temp] > BLANK && child[xlocs[index]][temp] != LIGHT)
            {
              child[xlocs[index]][temp] = LIT;
              temp++;
            }

            //Looks down
            stemp = ylocs[index]-1;
            while(stemp >=0 && child[xlocs[index]][stemp] > BLANK && child[xlocs[index]][stemp] != LIGHT)
            {
              child[xlocs[index]][stemp] = LIT;
              stemp--;
            }
          }
        }
      }
    }

    //Clears xlocs and ylocs
    while(xlocs.empty() == false)
    {
      xlocs.erase(xlocs.begin());
      ylocs.erase(ylocs.begin());
    }

    //Finds unlit squares on the board
    for(unsigned int i=0; i<child.size(); i++)
    {
      for(unsigned int k=0; k<child[i].size(); k++)
      {
        if(child[i][k] == UNLIT)
        {
          xlocs.push_back(i);
          ylocs.push_back(k);
        }
      }
    }

    if(xlocs.empty() == false)
    {
      //Randomly selects an unlit square
      index = rand() % xlocs.size();

      //Places a light in the square
      child[xlocs[index]][ylocs[index]] = LIGHT;

      //Lights squares in line with the new light
      //Looks right
      temp = xlocs[index] + 1;
    
      while(temp < child.size() && child[temp][ylocs[index]] > BLANK && child[temp][ylocs[index]] != LIGHT)
      {
        child[temp][ylocs[index]] = LIT;
        temp++;
      }

      //Looks left
      stemp = xlocs[index] - 1;
    
      while(stemp >= 0 && child[stemp][ylocs[index]] > BLANK && child[stemp][ylocs[index]] != LIGHT)
      {
        child[stemp][ylocs[index]] = LIT;
        stemp--;
      }

      //Looks up
      temp = ylocs[index] + 1;
    
      while(temp < child[xlocs[index]].size() && child[xlocs[index]][temp] > BLANK && child[xlocs[index]][temp] != LIGHT)
      {
        child[xlocs[index]][temp] = LIT;
        temp++;
      }

      //Looks down
      stemp = ylocs[index] - 1;
    
      while(stemp >= 0 && child[xlocs[index]][stemp] > BLANK && child[xlocs[index]][stemp] != LIGHT)
      {
        child[xlocs[index]][stemp] = LIT;
        stemp--;
      }
    }
  }

  return;
}

//Gives fitness with a penalty function
int constraint_fitness(vector< vector<int> >& board)
{
  int fitness=0;
  int totalfitness=0;
  int lights = 0;

  //Evaluates fitness
  //Increments for each lit up square
  for(unsigned int i=0; i<board.size(); i++)
  {
    for(unsigned int k=0; k<board[i].size(); k++)
    {
      if(board[i][k] == LIGHT || board[i][k] == LIT)
      {
        fitness++;
        totalfitness++;
      }

      //Increments for unlit squares to establish a proper ratio
      if(board[i][k] == UNLIT)
      {
        totalfitness++;
      }
    }
  }

//cout << "Unpentalized Fitness: " << fitness << endl;

  for(unsigned int i=0; i<board.size(); i++)
  {
    for(unsigned int k=0; k<board.size(); k++)
    {
      //If space is a light, checks for conflicting lights
      //Implements a penalty if a conflicting light is found
      if(board[i][k] == LIGHT)
      {
        unsigned int index = i+1;
        int sindex;
        //Looks right
        while(index < board.size() && board[index][k] == LIT)
        {
          index++;
        }

        if(index < board.size() && board[index][k] == LIGHT)
        {
          fitness = fitness - (index-i);
        }

        sindex = i-1;
        //Looks left
        while(sindex >= 0 && board[sindex][k] == LIT)
        {
          sindex--;
        }

        if(sindex >= 0 && board[sindex][k] == LIGHT)
        {
          fitness = fitness - (i-index);
        }

        index = k+1;
        //Looks up
        while(index < board[i].size() && board[i][index] == LIT)
        {
          index++;
        }

        if(index < board[i].size() && board[i][index] == LIGHT)
        {
          fitness = fitness - (index-k);
        }

        sindex = k-1;
        //Looks down
        while(sindex >= 0 && board[i][sindex] == LIT)
        {
          sindex--;
        }

        if(sindex >= 0 && board[i][sindex] == LIGHT)
        {
          fitness = fitness - (k-index);
        }
      }
    }
  }

//cout << "Minus Conflicting Lights: " << fitness << endl;


  //Implements a penalty if a black square has too many or too few lights
  for(unsigned int x=0; x<board.size(); x++)
  {
    for(unsigned int y=0; y<board.size(); y++)
    {
      lights = 0;

      if(board[x][y] <= NUMBERED)
      {
        //Makes sure location isn't on the right edge
        if(x!=board.size()-1)
        {
          if(board[x+1][y] == LIGHT)
          {
            lights++;
          }
        }

        //Makes sure location isn't on the left edge
        if(x!=0)
        {
          if(board[x-1][y] == LIGHT)
          {
            lights++;
          }
        }

        //Makes sure location isn't on the upper edge
        if(y!=board[0].size()-1)
        {
          if(board[x][y+1] == LIGHT)
          {
            lights++;
          }
        }

        //Makes sure location isn't on the bottom edge
        if(y!=0)
        {
          if(board[x][y-1] == LIGHT)
          {
            lights++;
          }
        }

        //Pentalizes based on number of improper lights
        if(lights > board[x][y])
        {
          fitness = fitness - (lights-board[x][y]);
        }

        else if(lights < board[x][y])
        {
          fitness = fitness - (board[x][y]-lights);
        }

      }
    }
  }

//cout << "Fully pentalized: " << fitness << endl;

  return (fitness);
}

//Returns the number of lit sells in the board
int lit_cells(vector< vector<int> >& board)
{
  int fitness=0;
  int totalfitness=0;

  //Evaluates fitness
  //Increments for each lit up square
  for(unsigned int i=0; i<board.size(); i++)
  {
    for(unsigned int k=0; k<board[i].size(); k++)
    {
      if(board[i][k] == LIGHT || board[i][k] == LIT)
      {
        fitness++;
        totalfitness++;
      }

      //Increments for unlit squares to establish a proper ratio
      if(board[i][k] == UNLIT)
      {
        totalfitness++;
      }
    }
  }

  return fitness;
}

//Returns a number decremented each time there is a conflicting light
int conflicting_lights(vector< vector<int> >& board)
{
  int fitness=0;

  for(unsigned int i=0; i<board.size(); i++)
  {
    for(unsigned int k=0; k<board.size(); k++)
    {
      //If space is a light, checks for conflicting lights
      //Implements a penalty if a conflicting light is found
      if(board[i][k] == LIGHT)
      {
        unsigned int index = i+1;
        int sindex;
        //Looks right
        while(index < board.size() && board[index][k] == LIT)
        {
          index++;
        }

        if(index < board.size() && board[index][k] == LIGHT)
        {
          fitness = fitness - (index-i);
        }

        sindex = i-1;
        //Looks left
        while(sindex >= 0 && board[sindex][k] == LIT)
        {
          sindex--;
        }

        if(sindex >= 0 && board[sindex][k] == LIGHT)
        {
          fitness = fitness - (i-index);
        }

        index = k+1;
        //Looks up
        while(index < board[i].size() && board[i][index] == LIT)
        {
          index++;
        }

        if(index < board[i].size() && board[i][index] == LIGHT)
        {
          fitness = fitness - (index-k);
        }

        sindex = k-1;
        //Looks down
        while(sindex >= 0 && board[i][sindex] == LIT)
        {
          sindex--;
        }

        if(sindex >= 0 && board[i][sindex] == LIGHT)
        {
          fitness = fitness - (k-index);
        }
      }
    }
  }

  return fitness;
}

//Returns a number decremented each time there is a black box violation
int cell_violations(vector< vector<int> >& board)
{
  int fitness=0;
  int lights = 0;

  //Implements a penalty if a black square has too many or too few lights
  for(unsigned int x=0; x<board.size(); x++)
  {
    for(unsigned int y=0; y<board.size(); y++)
    {
      lights = 0;

      if(board[x][y] <= NUMBERED)
      {
        //Makes sure location isn't on the right edge
        if(x!=board.size()-1)
        {
          if(board[x+1][y] == LIGHT)
          {
            lights++;
          }
        }

        //Makes sure location isn't on the left edge
        if(x!=0)
        {
          if(board[x-1][y] == LIGHT)
          {
            lights++;
          }
        }

        //Makes sure location isn't on the upper edge
        if(y!=board[0].size()-1)
        {
          if(board[x][y+1] == LIGHT)
          {
            lights++;
          }
        }

        //Makes sure location isn't on the bottom edge
        if(y!=0)
        {
          if(board[x][y-1] == LIGHT)
          {
            lights++;
          }
        }

        //Pentalizes based on number of improper lights
        if(lights > board[x][y])
        {
          fitness = fitness - (lights-board[x][y]);
        }

        else if(lights < board[x][y])
        {
          fitness = fitness - (board[x][y]-lights);
        }

      }
    }
  }

  return fitness;
}

//Returns a vector with the rankings of solutions based on dominance
vector<int> domination(vector<int>& lit, vector<int>& lights, vector<int>& black)
{
  vector<int> dominance;
  int index;
  unsigned int ranked = 0;
  int ranking = 1;
  bool coDominant = false;

  //Establishes vector size to place rankings
  for(unsigned int i=0; i<lit.size(); i++)
  {
    dominance.push_back(0);
  }

  //Ranks all members of the population
  while(ranked < lit.size())
  {
    index = 0;
    for(unsigned int i=1; i<lit.size(); i++)
    {
      //Only evaluates unranked solutions
      if(dominance[i] == 0)
      {
        //If i dominates in all fitnesses then it is taken as top ranking
        if(lit[i] > lit[index] && lights[i] > lights[index] && black[i] > black[index])
        {
          coDominant = false;
          index = i;
        }

        //If solution at [index] is coDominant
        else if(lit[i] > lit[index] || lights[i] > lights[index] || black[i] > black[index])
        {
          coDominant = true;
        }
      }

    }

    //Assigns ranking
    dominance[index] = ranking;

    //If solution was not coDominant then increments rankings so that further solutions are ranked below it
    if(coDominant == false)
    {
      ranking++;
    }

    ranked++;
  }

  return dominance;
}