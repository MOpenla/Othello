/******************************************************************************
 * Othello Game with Alpha-Beta Pruning
 *
 * Developed by: Mitchell Openlander
 * For: CS 4200 - Artificial Intelligence Methods
 *      Doctor Jake J.K. Lee
 *
 * //TODO general method used and whatnot
 ******************************************************************************/

//Black = 1
//White = -1
//Player is Black
//Computer is White

#include <iostream>
#include <string>

using namespace std;

//Function Prototypes
void playGame();
void displayStats();
bool isValidMove(int, int);
bool placeTile(int, int);
//minMax
//findMin
//findMax
//placeTile
//printCurrentBoard
//hasMovesLeft

//Global Variables
//board array
//heuristic values array
//current player

int main() {
    cout << "Hello and Welcome to the Othello game." << endl << endl;

    //initialize things

    playGame();

    //ask if they wish to play again
    //while player wants to play again
    // playGame
    // ask if they wish to play again
    //display a thank you and good bye message

    return 0;
}

void playGame() {
    //while loop - checks if any moves are left
    // if human's turn and has a move left
    //  cout color's turn
    //  display stats
    //  get move
    //  check if move is legal
    //   apply move if it is. get new move if it isn't
    // if computers turn
    //  cout color's turn
    //  calculate minMax
    //  find best move
    //  apply that move
    // switch players
    //display final score and winner
}

void displayStats() {
    //cout current score?
    //cout board
}

bool isValidMove(int row, int column) {
    //returns if the move is valid for the current player
    //first make sure the cell isn't already taken
    //then check that an adjacent square has the opponent's colour
    //then check that at the end of the opponent's streak there is the player's colour
}

bool placeTile(int row, int column) {
    //returns if the move was played or not
    //if valid move
    // place the current player's colour in the cell
    // flip the adjacent opponent's colour train
}
