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
void playGame(int*);
int opponent(int);
void hasMovesLeft(int*, int);
void displayStats(int*);
bool isValidMove(int*, int, int);
bool placeTile(int*, int, int);
int* findBestMove(int*);
int maxChoice(int, int*, int, int, int);
int minChoice(int, int*, int, int, int);
int heuristicEvaluation(int*, int);
int differenceEvaluation(int*, int);
int* copyBoard(int*);

//Global Variables
const int heuristicValues[][] = {
    {1000, 50, 100, 100, 100, 100, 50, 1000},
    {50, -20, -10, -10, -10, -10, -20, 50},
    {100, -10, 1, 1, 1, 1, -10, 100},
    {100, -10, 1, 1, 1, 1, -10, 100},
    {100, -10, 1, 1, 1, 1, -10, 100},
    {100, -10, 1, 1, 1, 1, -10, 100},
    {50, -20, -10, -10, -10, -10, -20, 50},
    {1000, 50, 100, 100, 100, 100, 50, 1000}
};
const int BLACK = 1; //human player
const int WHITE = -1; //computer player

const int initialValues[][] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, -1, 1, 0, 0, 0},
    {0, 0, 0, 1, -1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

//board array
//current player

int main() {
    cout << "Hello and Welcome to the Othello game." << endl << endl;

    //initialize things?

    playGame(copyBoard(initialValues));

    //ask if they wish to play again
    //while player wants to play again
    // reinitialize things
    // playGame
    // ask if they wish to play again
    //display a thank you and good bye message

    return 0;
}

void playGame(int* board) {
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

int opponent(int player) {
    //return the other player
}

bool hasMovesLeft(int* board, int player) {
    //returns if the player has any more moves left
    //iterate through each cell
    // if the cell is empty
    //  check that an adjacent cell has the opponent's colour
    //   check that at the end of teh opponent's streak there is the player's colour
    //    break when valid move is found
}

void displayStats(int* board) {
    //cout current score?
    //cout board
}

bool isValidMove(int* board, int row, int column) {
    //returns if the move is valid for the current player
    //first make sure the cell isn't already taken
    //then check that an adjacent square has the opponent's colour
    //then check that at the end of the opponent's streak there is the player's colour
}

bool placeTile(int* board, int row, int column) {
    //returns if the move was played or not
    //if valid move
    // place the current player's colour in the cell
    // flip the adjacent opponent's colour train
}

int* findBestMove(int* board) {
    //Iterate through moves to find the vaild moves
    //set max score and best move
    //On each valid move, make the move -- copy to new board
    // Figure out the next to move (after the computer's move)
    // If player, new score = min choice score
    // If computer, new score = max choice
    // if new score > max score
    //  max score = new score
    //  save move as the best move
    // release the temp board
    //return best move
}

int maxChoice(int player, int* board, int depth, int alpha, int beta) {
    //Iterate through moves to find the valid moves
    //if depth == 0
    // return the eval function
    //max = -inf
    //new score = 0
    //For each valid move
    // do the move -- make a new copy of the board
    // figure out the next to move (after the valid move is made)
    // if no valid moves are left
    //  new score = the difference in score between player and opponent
    //  new score = win or loss
    // if next to play is the current player
    //  new score = max choice
    // if next to play is the opponent
    //  new score = min choice
    // if new score > max
    //  max = new score
    // if max >= beta
    //  break and return max
    // alpha = Max(alpha, max)
    // release the temp board
    //return max
}

int minChoice(int player, int* board, int depth, int alpha, int beta) {
    //Iterate through moves to find the valid moves
    //if depth == 0
    // return the eval function
    //min = infinity
    //new score = 0
    //For each valid move the opponent can make
    // do the move -- make a new copy of the board
    // figure out the next to move (after the valid move is made)
    // if no valid moves are left
    //  new score = the difference in score between player and opponent
    //  new score = win or loss
    // if next to play is the current player
    //  new score = max choice
    // if next to play is the opponent
    //  new score = min choice
    // if new score < min
    //  min = new score
    // if min <= alpha
    //  break and return min
    // beta = Min(beta, min)
    // release the temp board
    //return min
}

int heuristicEvaluation(int* board, int player) {
    int playerCount = 0;
    int opponentCount = 0;
    int opponent = opponent(player);

    for (int i = 1; i < 8; i++) { //rows
        for (int j = 1; j < 8; j++) { //columns
            if (board[i][j] == player) {
                playerCount += heuristicValues[i][j];
            } else if (board[i][j] == opponent) {
                opponentCount += heuristicValues[i][j];
            }
        }
    }

    return (playerCount - opponentCount);
}

int differenceEvaluation(int* board, int player) {
    int playerCount = 0;
    int opponentCount = 0;
    int opponent = opponent(player);

    for (int i = 1; i < 8; i++) { //rows
        for (int j = 1; j < 8; j++) { //culumns
            if (board[i][j] == player) {
                playerCount++;
            } else if (board[i][j] == opponent) {
                opponentCount++;
            }
        }
    }

    return (playerCount - opponentCount);
}

int* copyBoard(int* board) {
    int temp[8][8];

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            temp[i][j] = board[i][j];
        }
    }

    return temp;
}
