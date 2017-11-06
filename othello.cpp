/******************************************************************************
 * Othello Game with Alpha-Beta Pruning
 *
 * Developed by: Mitchell Openlander
 * For: CS 4200 - Artificial Intelligence Methods
 *      Doctor Jake J.K. Lee
 *
 * //TODO general method used and whatnot
 ******************************************************************************/
/*

//   0  1  2  3  4  5  6  7  Row Index

0    0  1  2  3  4  5  6  7
1    8  9 10 11 12 13 14 15
2   16 17 18 19 20 21 22 23
3   24 25 26 27 28 29 30 31
4   32 33 34 35 36 37 38 39
5   40 41 42 43 44 45 46 47
6   48 49 50 51 52 53 54 55
7   56 57 58 59 60 61 62 63

Row Index = pos % 8
Column Index = pos / 8

Rows are vertical
Columns are horizontal

pos = ((row + 1) * (column + 1)) - 1

*/


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
bool hasMovesLeft(int*, int);
void displayStats(int*);
bool isValidMove(int*, int, int, int);
bool isValidMove(int*, int, int);
bool placeTile(int*, int, int);
int* findBestMove(int*);
int maxChoice(int, int*, int, int, int);
int minChoice(int, int*, int, int, int);
int heuristicEvaluation(int*, int);
int differenceEvaluation(int*, int);
int* copyBoard(int*);

//Global Variables
const int ROWS = 8;
const int COLUMNS = 8;

const int heuristicValues[ROWS * COLUMNS] = {
    1000, 50, 100, 100, 100, 100, 50, 1000,
    50, -20, -10, -10, -10, -10, -20, 50,
    100, -10, 1, 1, 1, 1, -10, 100,
    100, -10, 1, 1, 1, 1, -10, 100,
    100, -10, 1, 1, 1, 1, -10, 100,
    100, -10, 1, 1, 1, 1, -10, 100,
    50, -20, -10, -10, -10, -10, -20, 50,
    1000, 50, 100, 100, 100, 100, 50, 1000
};
const int BLACK = 1; //human player
const int WHITE = -1; //computer player

int initialValues[ROWS * COLUMNS] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 1, 0, 0, 0,
    0, 0, 0, 1, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

string playerName;
const string computerName = "Othello 9001";

//board array
//current player

int main() {
    cout << "Hello and Welcome to the Othello game." << endl << endl;
    cout << "Please enter your name: ";
    getline(cin, playerName);

    cout << endl;
    cout << "Greetings " << playerName << ". My name is " << computerName << "." << endl;
    cout << "Today we will be playing Othello. I hope you know the rules." << endl;
    cout << "You will play as Black and I will play as White." << endl;
    cout << "Let's get started." << endl << endl;

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

    int currentPlayer = BLACK;

    while (hasMovesLeft(board, currentPlayer) || hasMovesLeft(board, opponent(currentPlayer))) {
        if (currentPlayer == BLACK) { //Human is current player
            if (hasMovesLeft(board, currentPlayer)) { //Human has moves left
                cout << playerName << "'s turn." << endl;

            } else { //Human has no moves
                cout << playerName << " has no available moves." << endl;
                currentPlayer = opponent(currentPlayer); //Computer's turn
            }
        }

        if (currentPlayer == WHITE) { //Computer is current player
            if (hasMovesLeft(board, currentPlayer)) { //Computer has moves left
                cout << computerName << "'s turn." << endl;
            } else { //Computer has no moves left
                cout << computerName << " has no available moves." << endl;
                currentPlayer = opponent(currentPlayer);
            }
        }
    }
}

int opponent(int player) {
    int opnt = WHITE;

    if (player == WHITE) {
        opnt = BLACK;
    }

    return opnt;
}

bool hasMovesLeft(int* board, int player) {
    bool hasMoveLeft = false;

    for (int i = 0; i < (ROWS * COLUMNS) - 1 && !hasMoveLeft; i++) {
        hasMoveLeft = isValidMove(board, player, i);
    }

    return hasMoveLeft;
}

void displayStats(int* board) {
    //cout current score?
    //cout board
}

bool isValidMove(int* board, int player, int row, int column) {
    return (board, player, ((row + 1) * (column + 1)) - 1);
}

bool isValidMove(int* board, int player, int pos) {
    //returns if the move is valid for the current player
    //first make sure the cell isn't already taken
    //then check that an adjacent square has the opponent's colour
    //then check that at the end of the opponent's streak there is the player's colour

    bool valid = false;

    if (board[pos] == 0) {

        //Left Cell
        int leftPos = pos - 1;
        if (pos % ROWS != 0 && board[leftPos] == opponent(player)) {
            while (leftPos % ROWS != 0 && board[leftPos - 1] == opponent(player)) {
                leftPos--;
            }

            valid = leftPos % ROWS != 0 && board[leftPos - 1] == player;
        }

        //Right Cell
        int rightPos = pos + 1;
        if (!valid && pos % ROWS != ROWS && board[rightPos] == opponent(player)) {
            while (rightPos % ROWS != ROWS && board[rightPos + 1] == opponent(player)) {
                rightPos++;
            }

            valid = rightPos % ROWS != ROWS && board[rightPos + 1] == player;
        }

        //Up Cell
        int upPos = pos - ROWS;
        if (!valid && pos / COLUMNS != 0 && board[upPos] == opponent(player)) {
            while (upPos / COLUMNS != 0 && board[leftPos - ROWS] == opponent(player)) {
                upPos -= ROWS;
            }

            valid = upPos / COLUMNS != 0 && board[upPos - ROWS] == player;
        }

        //Down Cell
        int downPos = pos + ROWS;
        if (!valid && pos / COLUMNS != COLUMNS && board[downPos] == opponent(player)) {
            while (downPos / COLUMNS != COLUMNS && board[leftPos + ROWS] == opponent(player)) {
                downPos += ROWS;
            }

            valid = downPos / COLUMNS != COLUMNS && board[downPos + ROWS] == player;
        }

        //TODO diagonals
    }

    return valid;
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
    int opnt = opponent(player);

    for (int i = 1; i < ROWS * COLUMNS; i++) {
            if (board[i] == player) {
                playerCount += heuristicValues[i];
            } else if (board[i] == opnt) {
                opponentCount += heuristicValues[i];
            }
    }

    return (playerCount - opponentCount);
}

int differenceEvaluation(int* board, int player) {
    int playerCount = 0;
    int opponentCount = 0;
    int opnt = opponent(player);

    for (int i = 1; i < ROWS * COLUMNS; i++) {
        if (board[i] == player) {
            playerCount++;
        } else if (board[i] == opnt) {
            opponentCount++;
        }
    }

    return (playerCount - opponentCount);
}

int* copyBoard(int* board) {
    int temp[ROWS * COLUMNS];

    for (int i = 0; i < ROWS * COLUMNS; i++) {
        temp[i] = board[i];
    }

    return temp;
}
