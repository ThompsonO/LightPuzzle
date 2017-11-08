/*
  Oliver Thompson
  Homework 1d
  main.cpp
*/

/*
  Config File:
  Results Log Filename
  Solution Log Filename
  Number of Runs
  Number of Fitness Evaluations
  Random or Puzzle from file Flag, 0 = Random, 1 = From File
  Puzzle from file Filename
  Random generated number of rows (horizontal axis)
  Random generated number of columns (vertical axis)
  Seed RNG Flag, 0 = Seed generated based on time, 1 = Seed given
  Random Number Generator Seed
  Fitness Evaluation Flag, 0 = Black Numbers Ignored, 1 = Normal Fitness Evaluation, 2 = Constraint Satisfaction
  Validity Forced Flag, 0 = Uniform Random, 1 = Validity Forced plus Uniform Random
  Parent Selection Flag, 0 = Fitness Proportional Selection, 1 = Tournament Selection w/ Replacemnet, 2 = Uniform Random
  Surviavl Selection Flag, 0 = Truncation, 1 = Tournament Selection w/o Replacement, 2 = Fitness Proportional Selection, 3 = Uniform Random
  Termination Flag, 0 = Based on Number of Evals, 1 = Based on Convergence Criterion
  Population Size
  Child Population Size
  Parent Selection Tournament Size
  Survival Selection Tournament Size
  Convergence Criterion Value
  Survival Strategy Flag, 0 = Plus, 1 = Comma
*/

/*
  Board Legend:
  0-4 = Number on a black square
  5 = Blank black square
  6 = Unlit square
  7 = Light bulb
  8 = Lit square
*/

// Compile with g++ *.cpp -W -Wall -s -pedantic-errors

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "board.h"
#include "randplacement.h"

using namespace std;

const int BZERO = 0;
const int BONE = 1;
const int BTWO = 2;
const int BTHREE = 3;
const int NUMBERED = 4;
const int BLANK = 5;
const int UNLIT = 6;
const int LIGHT = 7;
const int LIT = 8;

int main(int argc, char **argv)
{
  string log, sol, resultsFile, solutionFile, puzzle, temp;
  int runs, evals, randomFlag, rows, cols, seedFlag, fitnessFlag, validityFlag, strategyFlag;
  int parentFlag, survivalFlag, terminationFlag, population, childSize, worstIndex;
  int pTournamentSize, sTournamentSize, convergence, evalLimit, counter, avgFit;
  int bestIndex, secondIndex, evaluationNumber, mutant, mutantChance, solutionFitness;
  unsigned int rngSeed;
  vector< vector< vector<int> > > solutionPopulation;
  vector< vector<int> > board, solution, holder;
  vector<int> tempVector, populationFitness, tournament;
  stringstream converter, outputConverter;
  int fittest = 0;
  int trial, avgDom, bestDom;
  int totalFitness, currentFitness, goalFitness;
  ofstream resultsLog, solutionLog;
  vector<int> litCells, conflictingLights, cellViolations, dominance;
  int oldTopRanked = 1;
  int topRanked = 0;

  //Takes the config filename as a parameter at runtime
  if(argc!=2)
  {
    cout<<"Config File Not Specified" << endl;
    return 1;
  }

  //Chance out of 100 for a child to be mutated
  mutantChance = 5;

  //Initializes Fitness of overall solution
  solutionFitness = 0;
  
  //Opens the Config File
  ifstream input;
  input.open(argv[1], ifstream::in);

  //Gets results and solution filenames
  getline(input, log);
  getline(input, sol);

  //Opens the resultsLog and solutionLog
  outputConverter.str("");
  outputConverter.clear();
  outputConverter << log;
  outputConverter >> resultsFile;

  outputConverter.str("");
  outputConverter.clear();
  outputConverter << sol;
  outputConverter >> solutionFile;

  resultsLog.open(resultsFile.c_str(), ofstream::out);
  solutionLog.open(solutionFile.c_str(), ofstream::out);

  resultsLog << "Results Log" << endl << endl;
  solutionLog << "Solution" << endl << endl;

  //Gets the number of runs
  getline(input, temp);
  converter.str(temp);
  converter >> runs;

  //Gets the number of fitness evaluations
  getline(input, temp);
  converter.str(temp);
  converter >> evals;

  //Gets the flag for random or puzzle from file
  getline(input, temp);
  converter.str(temp);
  converter >> randomFlag;

  //Gets the puzzle filename for non-random puzzles
  getline(input, puzzle);

  //Gets the number of rows
  getline(input, temp);
  converter.str(temp);
  converter >> rows;

  //Gets the number of columns
  getline(input, temp);
  converter.str(temp);
  converter >> cols;

  //Gets the Seed flag for random number generator
  getline(input, temp);
  converter.str(temp);
  converter >> seedFlag;

  //Gets the seed for the rng
  getline(input, temp);
  converter.str(temp);
  converter >> rngSeed;

  //Gets the fitness evaluation flag
  getline(input, temp);
  converter.str(temp);
  converter >> fitnessFlag;

  //Gets the Forced Validity flag
  getline(input, temp);
  converter.str(temp);
  converter >> validityFlag;

  //Gets the Parent Selection flag
  getline(input, temp);
  converter.str(temp);
  converter >> parentFlag;

  //Gets the Survival Selection flag
  getline(input, temp);
  converter.str(temp);
  converter >> survivalFlag;

  //Gets the Termination flag
  getline(input, temp);
  converter.str(temp);
  converter >> terminationFlag;

  //Gets the Population Size (Mu)
  getline(input, temp);
  converter.str(temp);
  converter >> population;

  //Gets the Children Size (Lambda)
  getline(input, temp);
  converter.str(temp);
  converter >> childSize;

  //Gets the Parent Tournament Size
  getline(input, temp);
  converter.str(temp);
  converter >> pTournamentSize;

  //Gets the Suvival Tournament Size
  getline(input, temp);
  converter.str(temp);
  converter >> sTournamentSize;

  //Gets the Number of Runs Until Convergence (n)
  getline(input, temp);
  converter.str(temp);
  converter >> convergence;

  //Gets the Survival Strategy Flag
  getline(input, temp);
  converter.str(temp);
  converter >> strategyFlag;

  //Seeds Random Number Generator based on flag
  if(seedFlag == 1)
  {
    srand(rngSeed);
  }
  else
  {
    rngSeed = time(NULL);
    srand(rngSeed);
  }

  //Prints algorithm parameters to Results Log File
  resultsLog << log << endl << endl;
  resultsLog << "Random Generator Seed: " << rngSeed << endl;
  resultsLog << "Number of Runs: " << runs << endl;
  resultsLog << "Number of Evaluations: " << evals << endl;
  resultsLog << "Random Flag: " << randomFlag << endl;

  if( randomFlag == 0)
  {
    resultsLog << "Number of Rows: " << rows << endl;
    resultsLog << "Number of Columns: " << cols << endl;
  }
  else
  {
    resultsLog << "Puzzle File Name: " << puzzle << endl;
  }

  resultsLog << endl;

  //Runs for the specified number of runs
  for(int i=0; i<runs; i++)
  {
    if(randomFlag == 0)
    {
      generate_board(rows, cols, board);
    }
    else
    {
      read_board(puzzle, board);
    }

    //Resets best fitness for each run
    fittest = 0;
    avgFit = 0;

    //Copies puzzle board into a placeholder to revert to upon evaluation
    for(unsigned int m=0; m<board.size(); m++)
    {
      tempVector.clear();
      for(unsigned int k=0; k<board[m].size(); k++)
      {
        tempVector.push_back(board[m][k]);
      }
      holder.push_back(tempVector);
    }

    //Initializes an empty population
    for(int r=0; r<population; r++)
    {
      solutionPopulation.push_back(board);
    }

    //Prints the run number to the results log
    resultsLog << "Run " << (i+1) << endl;


    //Sets Termination criteria based on convergence or number of evaluations
    if(terminationFlag == 1)
    {
      evalLimit = convergence;
    }

    else
    {
      evalLimit = evals;
    }

    counter = 0;
    evaluationNumber = 0;

    //Generates the initial population
    for(int r=0; r<population; r++)
    {
      //Places bulbs with forced validity plus random solution
      if(validityFlag == 1)
      {
        valid_forced(board);
      }

      //Places bulbs for a random solution
      else
      {
        place_bulbs(board);
      }
      //Copies solution into population
      for(unsigned int k=0; k<board.size(); k++)
      {
        for(unsigned int m=0; m<board[k].size(); m++)
        {
          solutionPopulation[r][k][m] = board[k][m];
        }       
      }

      //Evaluates the fitness of the solution
      if(fitnessFlag ==0)
      {
        trial = numberless_fitness_eval(board);
      }
      
      else if(fitnessFlag == 1)
      {
        trial = fitness_eval(board);
      }

      else if(fitnessFlag == 2)
      {
        trial = constraint_fitness(board);
      }

      //Stores fitness components for determining domination
      litCells.push_back(lit_cells(board));
      conflictingLights.push_back(conflicting_lights(board));
      cellViolations.push_back(cell_violations(board));

      //Increments number of evaluations performed
      counter++;
      evaluationNumber++;
      avgFit += trial;

      //Keeps tracks of fitness values of population
      populationFitness.push_back(trial);

      //Replaces previous best solution if new solution is better
      if(trial > fittest)
      {
        fittest = trial;

        //If terminationg based on convergence, then evaluation counter is reset
        if(terminationFlag == 1)
        {
          counter = 0;
        }

        //Copies new board into solution if it is the best solution so far
        if(trial > solutionFitness)
        {
          solutionFitness = trial;

          if(solution.empty() == false)
          {
            //Clears the solution board
            for(unsigned int m=0; m<solution.size(); m++)
            {
              while(solution[m].empty() == false)
              {
                solution[m].erase(solution[m].begin());
              }
            }

            while(solution.empty() == false)
            {
              solution.erase(solution.begin());
            }
          }

          //Copies the new solution into the solution
          for(unsigned int m=0; m<board.size(); m++)
          {
            tempVector.clear();
            for(unsigned int k=0; k<board[m].size(); k++)
            {
              tempVector.push_back(board[m][k]);
            }
            solution.push_back(tempVector);
          }
        }
      }

      //Reverts board back to the origional board
      for(unsigned int m=0; m<holder.size(); m++)
      {
        for(unsigned int k=0; k<holder[m].size(); k++)
        {
          board[m][k] = holder[m][k];
        }
      }
    }

    //Prints evaluation number and new best fitness to the results log
    avgDom = 0;
    bestDom = litCells[0];

    //Finds best and average cells lit up and prints them
    for(int c=0; c<population; c++)
    {
      avgDom += litCells[c];
      
      if(litCells[c] > bestDom)
      {
        bestDom = litCells[c];
      }

    }

    resultsLog << evaluationNumber << "\t" << (avgDom/population) << "\t" << bestDom;

    avgDom = 0;
    bestDom = conflictingLights[0];

    //Finds best and average conflicting lights and prints them
    for(int c=0; c<population; c++)
    {
      avgDom += conflictingLights[c];
      
      if(conflictingLights[c] > bestDom)
      {
        bestDom = conflictingLights[c];
      }

    }

    resultsLog << "\t" << (avgDom/population) << "\t" << bestDom;

    avgDom = 0;
    bestDom = cellViolations[0];

    //Finds best and average black box violations and prints them
    for(int c=0; c<population; c++)
    {
      avgDom += cellViolations[c];
      
      if(cellViolations[c] > bestDom)
      {
        bestDom = cellViolations[c];
      }

    }

    resultsLog << "\t" << (avgDom/population) << "\t" << bestDom << endl;

    //Performs Recombination and Mutation
    //Completes the board for the specified number of evals
    while(counter < evalLimit && oldTopRanked != topRanked)
    {
      for(int z=0; z<childSize; z++)
      {
        bestIndex = 0;
        secondIndex = 0;

        //Ranks solutions based on domination
        dominance = domination(litCells, conflictingLights, cellViolations);

        //Tournament Parent Selection with Replacement
        if(parentFlag == 1)
        {
          //Clears tournament vector
          if(tournament.empty() == false)
          {
            for(unsigned int t=0; t<tournament.size(); t++)
            {
              tournament.erase(tournament.begin());
            }
          }

          //Generates indecies of solutions to place in the tournament
          for(int t=0; t<pTournamentSize; t++)
          {
            tournament.push_back(rand()%population);
          }

          //Initializes bestIndex
          bestIndex = tournament[0];

          //Finds the fittest parents in the tournament
          for(int t=0; t<pTournamentSize; t++)
          {
            //Replaces best index if found fitness is better
            if(dominance[bestIndex] > dominance[tournament[t]])
            {
              bestIndex = tournament[t];
            }
          }

          //Clears tournament vector
          if(tournament.empty() == false)
          {
            for(unsigned int t=0; t<tournament.size(); t++)
            {
              tournament.erase(tournament.begin());
            }
          }

          //Generates indecies of solutions to place in the tournament
          for(int t=0; t<pTournamentSize; t++)
          {
            tournament.push_back(rand()%population);
          }

          //Initialzes secondIndex
          secondIndex = tournament[0];

          //Finds the fittest parents in the tournament
          for(int t=0; t<pTournamentSize; t++)
          {
            //Replaces best index if found fitness is better
            if(dominance[secondIndex] > dominance[tournament[t]])
            {
              secondIndex = tournament[t];
            }
          }
        }

        //Fitness Proportional Selection
        else if(parentFlag == 0)
        {
          totalFitness = 0;
          bestIndex = 0;
          secondIndex = 0;
          for(unsigned int v=0; v<dominance.size(); v++)
          {
            //Totals fitness
            totalFitness += dominance[v];
          }

          //Randomly selects a value to select the parent with
          goalFitness = rand() % totalFitness;
          currentFitness = 0;

          //Cumulatively iterates through population to find the individual that the value corresponds to
          for(unsigned int u=0; u<dominance.size(); u++)
          {
            currentFitness += dominance[u];
            //Selects first parent
            if(goalFitness > currentFitness)
            {
              bestIndex = u;
            }
          }

          //Randomly selects a value to select the parent with
          goalFitness = rand() % totalFitness;
          currentFitness = 0;

          //Cumulatively iterates through population to find the individual that the value corresponds to
          for(unsigned int u=0; u<dominance.size(); u++)
          {
            currentFitness += dominance[u];
            //Selects first parent
            if(goalFitness > currentFitness)
            {
              secondIndex = u;
            }
          }
        }

        //Uniform Random Selection
        else
        {
          bestIndex = tournament[(rand()%tournament.size())];
          secondIndex = tournament[(rand()%tournament.size())];
        }

        //Recombines Parents into a child
        solutionPopulation.push_back(recombination(solutionPopulation[bestIndex], solutionPopulation[secondIndex]));

        //Possibly mutates the child
        mutant = (rand() % 100) + 1;
        if(mutant <= mutantChance)
        {
          mutate(solutionPopulation[population+z]);
        }

        //Evaluates the fitness of the child
        if(fitnessFlag == 0)
        {
          trial = numberless_fitness_eval(solutionPopulation[population+z]);
        }
      
        else if(fitnessFlag == 1)
        {
          trial = fitness_eval(solutionPopulation[population+z]);
        }

        else if(fitnessFlag == 2)
        {
          trial = constraint_fitness(solutionPopulation[population+z]);
        }

        //Stores fitness components for determining domination
        litCells.push_back(lit_cells(solutionPopulation[population+z]));
        conflictingLights.push_back(conflicting_lights(solutionPopulation[population+z]));
        cellViolations.push_back(cell_violations(solutionPopulation[population+z]));

        //Increments number of evaluations performed
        counter++;
        evaluationNumber++;
//        avgFit += trial;
        
        //Keeps tracks of fitness values of population
        populationFitness.push_back(trial);

        //Replaces previous best solution if new solution is better
        if(trial > fittest)
        {
          fittest = trial;

          //If terminationg based on convergence, then evaluation counter is reset
          if(terminationFlag == 1)
          {
            counter = 0;
          }

          //Prints evaluation number and new best fitness to the results log
//          resultsLog << evaluationNumber << "\t" << (avgFit/evaluationNumber) << "\t" << trial << endl;

          //Copies new board into solution if it is the best solution so far
          if(trial > solutionFitness)
          {
            solutionFitness = trial;

            if(solution.empty() == false)
            {
              //Clears the solution board
              for(unsigned int m=0; m<solution.size(); m++)
              {
                while(solution[m].empty() == false)
                {
                  solution[m].erase(solution[m].begin());
                }
              }
            
              while(solution.empty() == false)
              {
                solution.erase(solution.begin());
              }
            }

            //Copies the new solution into the solution
            for(unsigned int m=0; m<solutionPopulation[population+z].size(); m++)
            {
              tempVector.clear();
              for(unsigned int k=0; k<solutionPopulation[population+z][m].size(); k++)
              {
                tempVector.push_back(solutionPopulation[population+z][m][k]);
              }
              solution.push_back(tempVector);
            }
          }
        }
      }

      //Ranks solutions based on domination
      dominance = domination(litCells, conflictingLights, cellViolations);

      //Keeps Fittest based on Survival Flag
      //Tournament Style
      if(survivalFlag == 1)
      {
        //Plus Surival Strategy
        if(strategyFlag == 0)
        {
          for(int w=0; w<childSize; w++)
          {
            //Clears tournament vector
            if(tournament.empty() == false)
            {
              for(unsigned int t=0; t<tournament.size(); t++)
              {
                tournament.erase(tournament.begin());
              }
            }

            //Fills tournament with random indecies
            for(int t=0; t<sTournamentSize; t++)
            {
               tournament.push_back(rand()%(population+childSize-(w+1)));
            }

            //worstIndex initialized
            worstIndex = tournament[0];

            //Finds the most unfit in the tournament
            for(int t=0; t<sTournamentSize; t++)
            {
              if(dominance[worstIndex] < dominance[tournament[t]])
              {
                worstIndex = tournament[t];
              }
            }

            //Removes Solution and fitness from population
            solutionPopulation.erase(solutionPopulation.begin()+worstIndex);
            populationFitness.erase(populationFitness.begin()+worstIndex);
            dominance.erase(dominance.begin()+worstIndex);
            litCells.erase(litCells.begin()+worstIndex);
            conflictingLights.erase(conflictingLights.begin()+worstIndex);
            cellViolations.erase(cellViolations.begin()+worstIndex);
          }
        }

        //Comma Surival Strategy
        else if(strategyFlag == 1)
        {
          for(int w=0; w<childSize; w++)
          {
            //Clears tournament vector
            if(tournament.empty() == false)
            {
              for(unsigned int t=0; t<tournament.size(); t++)
              {
                tournament.erase(tournament.begin());
              }
            }

            //If The number of parents is greater than the size of the tournament
            if(!(sTournamentSize > (int(solutionPopulation.size()))-childSize))
            {
              //Fills tournament with random indecies from parents
              for(int t=0; t<sTournamentSize; t++)
              {
                 tournament.push_back(rand()%(population-(w+1)));
              }
            }

            else
            {
              //Gets Remaining parents and puts them in tournament
              for(unsigned int t=0; t<solutionPopulation.size()-childSize; t++)
              {
                tournament.push_back(t);
              }

              //Fills remainder of tournament with random children
              for(unsigned int t=0; t<sTournamentSize-(solutionPopulation.size()-childSize); t++)
              {
                tournament.push_back(rand()%((childSize-(w+1)) + (solutionPopulation.size()-childSize)));
              }

            }


            //worstIndex initialized
            worstIndex = tournament[0];

            //Finds the most unfit in the tournament
            for(int t=0; t<sTournamentSize; t++)
            {
              if(dominance[worstIndex] < dominance[tournament[t]])
              {
                worstIndex = tournament[t];
              }
            }

            //Removes Solution and fitness from population
            solutionPopulation.erase(solutionPopulation.begin()+worstIndex);
            populationFitness.erase(populationFitness.begin()+worstIndex);
            dominance.erase(dominance.begin()+worstIndex);
            litCells.erase(litCells.begin()+worstIndex);
            conflictingLights.erase(conflictingLights.begin()+worstIndex);
            cellViolations.erase(cellViolations.begin()+worstIndex);
          }
        }

      }

      //Truncation
      else if(survivalFlag == 0)
      {
        //Finds the worst in the set and deletes it
        for(int c=0; c<childSize; c++)
        {
          //Finds the index of the worst in the population
          worstIndex = 0;
          for(unsigned int u=0; u<dominance.size(); u++)
          {
            if(dominance[u] > dominance[worstIndex])
            {
              worstIndex = u;
            }
          }

          //Deletes the most unfit
          solutionPopulation.erase(solutionPopulation.begin()+worstIndex);
          populationFitness.erase(populationFitness.begin()+worstIndex);
          dominance.erase(dominance.begin()+worstIndex);
          litCells.erase(litCells.begin()+worstIndex);
          conflictingLights.erase(conflictingLights.begin()+worstIndex);
          cellViolations.erase(cellViolations.begin()+worstIndex);
        }
      }

      //Fitness Proportional Selection
      else if(survivalFlag == 2)
      {
        for(int y=0; y<childSize; y++)
        {
          totalFitness = 0;
          bestIndex = 0;
          secondIndex = 0;
          for(unsigned int v=0; v<dominance.size(); v++)
          {
            //Totals fitness
            totalFitness += dominance[v];
          }

          //Randomly selects a value to select the individual with
          goalFitness = rand() % totalFitness;
          currentFitness = 0;

          //Cumulatively iterates through population to find the individual that the value corresponds to
          for(unsigned int u=0; u<dominance.size(); u++)
          {
            currentFitness += dominance[u];
            //Selects individual
            if(goalFitness > currentFitness)
            {
              bestIndex = u;
            }
          }

          solutionPopulation.erase(solutionPopulation.begin()+bestIndex);
          populationFitness.erase(populationFitness.begin()+bestIndex);
          dominance.erase(dominance.begin()+bestIndex);
          litCells.erase(litCells.begin()+bestIndex);
          conflictingLights.erase(conflictingLights.begin()+bestIndex);
          cellViolations.erase(cellViolations.begin()+bestIndex);

        }
      }

      //Uniform Random
      else
      {
        for(int y=0; y<childSize; y++)
        {
          worstIndex = rand()%solutionPopulation.size();
          solutionPopulation.erase(solutionPopulation.begin()+worstIndex);
          populationFitness.erase(populationFitness.begin()+worstIndex);
          dominance.erase(dominance.begin()+worstIndex);
          litCells.erase(litCells.begin()+worstIndex);
          conflictingLights.erase(conflictingLights.begin()+worstIndex);
          cellViolations.erase(cellViolations.begin()+worstIndex);
        }
      }

      avgFit = 0;
      for(unsigned int t=0; t<populationFitness.size(); t++)
      {
        avgFit += populationFitness[t];
      }

//cout << evaluationNumber << "\t" << (avgFit/population) << "\t" << fittest << endl;


    //Prints evaluation number and new best fitness to the results log
    avgDom = 0;
    bestDom = litCells[0];

    //Finds best and average cells lit up and prints them
    for(int c=0; c<population; c++)
    {
      avgDom += litCells[c];
      
      if(litCells[c] > bestDom)
      {
        bestDom = litCells[c];
      }

    }

    resultsLog << evaluationNumber << "\t" << (avgDom/population) << "\t" << bestDom;

    avgDom = 0;
    bestDom = conflictingLights[0];

    //Finds best and average conflicting lights and prints them
    for(int c=0; c<population; c++)
    {
      avgDom += conflictingLights[c];
      
      if(conflictingLights[c] > bestDom)
      {
        bestDom = conflictingLights[c];
      }

    }

    resultsLog << "\t" << (avgDom/population) << "\t" << bestDom;

    avgDom = 0;
    bestDom = cellViolations[0];

    //Finds best and average black box violations and prints them
    for(int c=0; c<population; c++)
    {
      avgDom += cellViolations[c];
      
      if(cellViolations[c] > bestDom)
      {
        bestDom = cellViolations[c];
      }

    }

    resultsLog << "\t" << (avgDom/population) << "\t" << bestDom << endl;

      //If termination based on convergence then tabluates to see if there has been change in non-dominated solutions
      if(terminationFlag == 1)
      {
        oldTopRanked = topRanked;
        topRanked = 0;
        //Totals number of top ranked solutions to determine convergence
        for(unsigned int c=0; c<dominance.size(); c++)
        {
          if(dominance[c] == 1)
          {
            topRanked++;
          }
        }
      }

    }

    //Creates text block for the next run
    resultsLog << endl;

    //Clears the holder board
    for(unsigned int m=0; m<holder.size(); m++)
    {
      while(holder[m].empty() == false)
      {
        holder[m].erase(holder[m].begin());
      }
    }

    while(holder.empty() == false)
    {
      holder.erase(holder.begin());
    }

    //Erases the Solution Population
    while(solutionPopulation.empty() == false)
    {
      solutionPopulation.erase(solutionPopulation.begin());
      populationFitness.erase(populationFitness.begin());
      dominance.erase(dominance.begin());
      litCells.erase(litCells.begin());
      conflictingLights.erase(conflictingLights.begin());
      cellViolations.erase(cellViolations.begin());
    }
 
  }

  //Prints out puzzle that the best solution solved (i.e. Black box locations)
  for(unsigned int i=0; i<solution.size(); i++)
  {
    for(unsigned int k=0; k<solution[i].size(); k++)
    {
      if(solution[i][k] <= BLANK)
      {
        solutionLog << i+1 << " " << k+1 << " " << solution[i][k] << endl;
      }
    }
  }

  //Prints out Fitness of solution
  solutionLog << solutionFitness << endl;

  //Prints out Light Bulb locations
  for(unsigned int i=0; i<solution.size(); i++)
  {
    for(unsigned int k=0; k<solution[i].size(); k++)
    {
      if(solution[i][k] == LIGHT)
      {
        solutionLog << i+1 << " " << k+1 << endl;
      }
    }
  }

  input.close();
  resultsLog.close();
  solutionLog.close();

  return 0;
}