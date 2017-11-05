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
//Black goes first

#include <iostream>
#include <string>

using namespace std;

//Function Prototypes
void playGame();
void hasMovesLeft(int); //maybe pass board for most of these...
void displayStats();
bool isValidMove(int, int);
bool placeTile(int, int);
int* findBestMove();
//minMax
//findMin
//findMax

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

bool hasMovesLeft(int player) {
    //returns if the player has any more moves left
    //iterate through each cell
    // if the cell is empty
    //  check that an adjacent cell has the opponent's colour
    //   check that at the end of teh opponent's streak there is the player's colour
    //    break when valid move is found
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

int* findBestMove() {
    //Iterate through moves to find the vaild moves
    //set max score and best move
    //On each valid move, make the move -- copy to new board
    // Figure out the next to move (after the computer's move)
    // If player, new score = min choice score
    // If computer, new score = max choice
    // if new score > max score
    //  max score = new score
    //  save move as the best move
    //return best move
}

int maxChoice(int player, int* board) {

}

int minChoice(int player, int* board) {

}
