/******************************************************************************
 * Othello Game with Alpha-Beta Pruning
 *
 * Developed by: Mitchell Openlander
 * For: CS 4200 - Artificial Intelligence Methods
 *      Doctor J.K. Jake Lee
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
pos = row + (8 * column)

Rows are vertical
Columns are horizontal

*/


//Black = 1
//White = -1
//Player is Black
//Computer is White
//Black goes first

#include <iostream>
#include <string>
#include <climits> //max and min int

using namespace std;

//Function Prototypes
void playGame(int*);
int opponent(int);
bool hasMovesLeft(int*, int);
void displayStats(int*);
bool isValidMove(int*, int, int, int);
bool isValidMove(int*, int, int);
bool placeTile(int*, int, int, int);
bool placeTile(int*, int, int);
int findBestMove(int*, int);
int maxChoice(int, int*, int, int, int);
int minChoice(int, int*, int, int, int);
int heuristicEvaluation(int*, int);
int differenceEvaluation(int*, int);
int score(int*, int);
int* copyBoard(int*);
bool hasTopTrain(int*, int, int);
bool hasLeftTrain(int*, int, int);
bool hasRightTrain(int*, int, int);
bool hasBottomTrain(int*, int, int);
bool hasTopLeftTrain(int*, int, int);
bool hasTopRightTrain(int*, int, int);
bool hasBottomLeftTrain(int*, int, int);
bool hasBottomRightTrain(int*, int, int);

//Global Variables
const int ROWS = 8;
const int COLUMNS = 8;
const int BLACK = 1; //human player
const int WHITE = -1; //computer player

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

int main() {
    //Welcome the player and get their name for a more personalized experience
    cout << "Hello and Welcome to the Othello game." << endl << endl;
    cout << "Please enter your name: ";
    getline(cin, playerName);

    cout << endl;

    //Greet the player and lay down some base rules
    cout << "Greetings " << playerName << ". My name is " << computerName << "." << endl;
    cout << "Today we will be playing Othello. I hope you know the rules." << endl;
    cout << "You will play as Black and I will play as White." << endl;
    cout << "Let's get started." << endl << endl;

    playGame(copyBoard(initialValues));

    char again;
    cout << endl << "Would you like to play again (y for yes)? ";
    cin >> again;

    while (again == 'y') {
        playGame(copyBoard(initialValues));

        cout << endl << "Would you like to play again (y for yes)? ";
        cin >> again;
    }

    cout << endl;
    cout << "Thank you " << playerName << " for playing and have a wonderful day :)" << endl;

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
                cout << "----------" << playerName << "'s turn" << "----------" << endl << endl;
                displayStats(board);

                cout << endl << endl;

                int rowIndex = 0;
                int columnIndex = 0;

                cout << "Row Index: ";
                cin >> rowIndex;
                cout << "Column Index: ";
                cin >> columnIndex;

                cout << endl;

                while (!isValidMove(board, currentPlayer, rowIndex, columnIndex)) {
                    cout << "Sorry, that move is not valid." << endl;
                    cout << "Please try again." << endl << endl;

                    cout << "Row Index: ";
                    cin >> rowIndex;
                    cout << "Column Index: ";
                    cin >> columnIndex;
                }

                placeTile(board, currentPlayer, rowIndex, columnIndex);

            } else { //Human has no moves
                cout << playerName << " has no available moves." << endl;
            }
        }

        if (currentPlayer == WHITE) { //Computer is current player
            if (hasMovesLeft(board, currentPlayer)) { //Computer has moves left
                cout << "----------" << computerName << "'s turn" << "----------" << endl << endl;
                displayStats(board);
                cout << endl << endl;

                int bestMove = findBestMove(board, currentPlayer);
                placeTile(board, currentPlayer, bestMove);

                cout << computerName << " placed a tile at " << (bestMove % 8) << ", " << (bestMove / 8) << "." << endl << endl;
            } else { //Computer has no moves left
                cout << computerName << " has no available moves." << endl;
            }
        }

        currentPlayer = opponent(currentPlayer);
    }

    int humanScore = score(board, BLACK);
    int computerScore = score(board, WHITE);

    cout << "No more moves left." << endl << endl;
    cout << "---Final Score---" << endl;
    cout << playerName << ":  " << humanScore << endl;
    cout << computerName << ":  " << computerScore << endl << endl;

    if (humanScore > computerScore) { //Human wins
        cout << playerName << " wins!" << endl;
        cout << "Good job!" << endl;
    } else if (computerScore > humanScore) { //Computer wins
        cout << computerName << " wins." << endl;
        cout << "Better luck next time." << endl;
    } else { //Tie
        cout << "Tie game." << endl;
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

    for (int i = 0; i < ROWS * COLUMNS && !hasMoveLeft; i++) {
        hasMoveLeft = isValidMove(board, player, i);
    }

    return hasMoveLeft;
}

void displayStats(int* board) {
    cout << "Score" << endl;
    cout << playerName << ":  " << score(board, BLACK) << endl;
    cout << computerName << ":  " << score(board, WHITE) << endl;

    cout << endl << endl;

    cout << "    0  1  2  3  4  5  6  7" << endl;
    int columnNum = 0;

    for (int i = 0; i < ROWS * COLUMNS; i++) {
        if (i % ROWS == 0) {
            cout << endl;
            cout << columnNum << "   ";

            columnNum++;
        }

        char output = '-';

        if (board[i] == -1) {
            output = 'W';
        } else if (board[i] == 1) {
            output = 'B';
        }

        cout << output << "  ";
    }
}

bool isValidMove(int* board, int player, int row, int column) {
    return isValidMove(board, player, row + (ROWS * column));
}

bool isValidMove(int* board, int player, int pos) {
    bool valid = false;

    if (board[pos] == 0) {
        valid = valid || hasTopTrain(board, player, pos);
        valid = valid || hasLeftTrain(board, player, pos);
        valid = valid || hasRightTrain(board, player, pos);
        valid = valid || hasBottomTrain(board, player, pos);
        valid = valid || hasTopLeftTrain(board, player, pos);
        valid = valid || hasTopRightTrain(board, player, pos);
        valid = valid || hasBottomLeftTrain(board, player, pos);
        valid = valid || hasBottomRightTrain(board, player, pos);
    }

    return valid;
}

bool placeTile(int* board, int player, int row, int column) {
    return placeTile(board, player, row + (ROWS * column));
}

bool placeTile(int* board, int player, int pos) {
    bool placed = false;

    if (board[pos] == 0) {
        board[pos] = player;

        //Up Cell
        if (hasTopTrain(board, player, pos)) {
            int upPos = pos - ROWS;
            while (upPos / COLUMNS != 0 && board[upPos] == opponent(player)) {
                board[upPos] = player;
                upPos -= ROWS;
            }

            placed = true;
        }

        //Left Cell
        if (hasLeftTrain(board, player, pos)) {
            int leftPos = pos - 1;
            while (leftPos % ROWS != 0 && board[leftPos] == opponent(player)) {
                board[leftPos] = player;
                leftPos--;
            }

            placed = true;
        }

        //Right Cell
        if (hasRightTrain(board, player, pos)) {
            int rightPos = pos + 1;
            while (rightPos % ROWS != ROWS && board[rightPos] == opponent(player)) {
                board[rightPos] = player;
                rightPos++;
            }

            placed = true;
        }

        //Down Cell
        if (hasBottomTrain(board, player, pos)) {
            int downPos = pos + ROWS;
            while (downPos / COLUMNS != COLUMNS && board[downPos] == opponent(player)) {
                board[downPos] = player;
                downPos += ROWS;
            }

            placed = true;

        }

        //Top Left Cell
        if (hasTopLeftTrain(board, player, pos)) {
            int topLeftPos = pos - (ROWS + 1);
            while (topLeftPos / COLUMNS != 0 && topLeftPos % ROWS != 0 && board[topLeftPos] == opponent(player)) {
                board[topLeftPos] = player;
                topLeftPos -= ROWS + 1;
            }

            placed = true;
        }

        //Top Right Cell
        if (hasTopRightTrain(board, player, pos)) {
            int topRightPos = pos - (ROWS - 1);
            while (topRightPos / COLUMNS != 0 && topRightPos % ROWS != ROWS && board[topRightPos] == opponent(player)) {
                board[topRightPos] = player;
                topRightPos -= ROWS - 1;
            }

            placed = true;
        }

        //Bottom Left Cell
        if (hasBottomLeftTrain(board, player, pos)) {
            int bottomLeftPos = pos + (ROWS - 1);
            while (bottomLeftPos / COLUMNS != COLUMNS && bottomLeftPos % ROWS != 0 && board[bottomLeftPos] == opponent(player)) {
                board[bottomLeftPos] = player;
                bottomLeftPos += ROWS - 1;
            }

            placed = true;
        }

        //Bottom Right Cell
        if (hasBottomRightTrain(board, player, pos)) {
            int bottomRightPos = pos + (ROWS + 1);
            while (bottomRightPos / COLUMNS != COLUMNS && bottomRightPos % ROWS != ROWS && board[bottomRightPos] == opponent(player)) {
                board[bottomRightPos] = player;
                bottomRightPos += ROWS + 1;
            }

            placed = true;

        }

    }

    return placed;
}

int findBestMove(int* board, int currentPlayer) {
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

    int maxScore = INT_MIN;
    int bestMove = 0; //This is the position to place the tile for the best move
    const int recursionLimit = 6;
    int alpha = INT_MIN;
    int beta = INT_MAX;

    for (int i = 0; i < ROWS * COLUMNS; i++) {
        if (isValidMove(board, currentPlayer, i)) {
            int* newBoard = copyBoard(board);
            int newScore = 0;

            if (hasMovesLeft(newBoard, opponent(currentPlayer))) { //Opponent can move next
                newScore = minChoice(opponent(currentPlayer), newBoard, recursionLimit, alpha, beta);
            } else if (hasMovesLeft(newBoard, currentPlayer)) { //Computer can move next
                newScore = maxChoice(currentPlayer, newBoard, recursionLimit, alpha, beta);
            } else { //No one can move next, therefore no further analysis can be made
                newScore = differenceEvaluation(board, currentPlayer);
            }

            if (newScore > maxScore) {
                maxScore = newScore;
                bestMove = i;
            }
        }
    }

    return bestMove;
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

    int max = INT_MIN;

    if (depth <= 0) {
        max = heuristicEvaluation(board, player);
    } else {
        int newScore = 0;

        for (int i = 0; i < ROWS * COLUMNS; i++) {
            if (isValidMove(board, player, i)) {
                int* newBoard = copyBoard(board);

                if (hasMovesLeft(newBoard, opponent(player))) { //Opponent can move next
                    newScore = minChoice(opponent(player), newBoard, depth - 1, alpha, beta);
                } else if (hasMovesLeft(newBoard, player)) { //Computer can move next
                    newScore = maxChoice(player, newBoard, depth - 1, alpha, beta);
                } else { //No one can move next, therefore no further analysis can be made
                    newScore = differenceEvaluation(board, player);
                }

                if (newScore > max) {
                    max = newScore;
                }

            }
        }
    }

    return max;
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

    int min = INT_MAX;

    if (depth <= 0) {
        min = heuristicEvaluation(board, player);
    } else {
        int newScore = 0;

        for (int i = 0; i < ROWS * COLUMNS; i++) {
            if (isValidMove(board, player, i)) {
                int* newBoard = copyBoard(board);

                if (hasMovesLeft(newBoard, opponent(player))) { //Opponent can move next
                    newScore = minChoice(opponent(player), newBoard, depth - 1, alpha, beta);
                } else if (hasMovesLeft(newBoard, player)) { //Computer can move next
                    newScore = maxChoice(player, newBoard, depth - 1, alpha, beta);
                } else { //No one can move next, therefore no further analysis can be made
                    newScore = differenceEvaluation(board, player);
                }

                if (newScore < min) {
                    min = newScore;
                }

            }
        }
    }

    return min;
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
    return (score(board, player) - score(board, opponent(player)));
}

int score(int* board, int player) {
    int score = 0;

    for (int i = 0; i < ROWS * COLUMNS; i++) {
        if (board[i] == player) {
            score++;
        }
    }

    return score;
}

int* copyBoard(int* board) {
    int* temp = new int[ROWS * COLUMNS];

    for (int i = 0; i < ROWS * COLUMNS; i++) {
        temp[i] = board[i];
    }

    return temp;
}





/*
 * These functions check if the opponent occupies a cell adjacent to pos
 *
 *  If the opponent does occupy an adjacent cell then these functions check
 *   that the opponent occupies more cells in that direction.
 *   at the end of the train of opponent cells,
 *   these functions also check that there is a player cell.
 *  If the above conditions are true then the function returns true,
 *   else false is returned.
 *
 *  The general algorithm is
 *   1. Check that the current cell is not an edje cell
 *   2. Check that the adjacent cell contains the opponent's colour
 *   3. If 1 and 2 hold then loop while the adjacent cell is not an edge cell
 *       and the next adjacent cell contains the opponent's colour.
 *   4. Finally check that at the end of the train of opponent cells is a player cell.
 */

bool hasTopTrain(int* board, int player, int pos) {
    bool valid = false;

    int upPos = pos - ROWS;
    if (pos / COLUMNS != 0 && board[upPos] == opponent(player)) {
        while (upPos / COLUMNS != 0 && board[upPos - ROWS] == opponent(player)) {
            upPos -= ROWS;
        }

        valid = upPos / COLUMNS != 0 && board[upPos - ROWS] == player;
    }

    return valid;
}

bool hasLeftTrain(int* board, int player, int pos) {
    bool valid = false;

    int leftPos = pos - 1;
    if (pos % ROWS != 0 && board[leftPos] == opponent(player)) {
        while (leftPos % ROWS != 0 && board[leftPos - 1] == opponent(player)) {
            leftPos--;
        }

        valid = leftPos % ROWS != 0 && board[leftPos - 1] == player;
    }

    return valid;
}

bool hasRightTrain(int* board, int player, int pos) {
    bool valid = false;

    int rightPos = pos + 1;
    if (pos % ROWS != ROWS && board[rightPos] == opponent(player)) {
        while (rightPos % ROWS != ROWS && board[rightPos + 1] == opponent(player)) {
            rightPos++;
        }

        valid = rightPos % ROWS != ROWS && board[rightPos + 1] == player;
    }

    return valid;
}

bool hasBottomTrain(int* board, int player, int pos) {
    bool valid = false;

    int downPos = pos + ROWS;
    if (pos / COLUMNS != COLUMNS && board[downPos] == opponent(player)) {
        while (downPos / COLUMNS != COLUMNS && board[downPos + ROWS] == opponent(player)) {
            downPos += ROWS;
        }

        valid = downPos / COLUMNS != COLUMNS && board[downPos + ROWS] == player;
    }

    return valid;
}

bool hasTopLeftTrain(int* board, int player, int pos) {
    bool valid = false;

    int topLeftPos = pos - (ROWS + 1);
    if (pos / COLUMNS != 0 && pos % ROWS != 0 && board[topLeftPos] == opponent(player)) {
        while (topLeftPos / COLUMNS != 0 && topLeftPos % ROWS != 0 && board[topLeftPos - (ROWS + 1)] == opponent(player)) {
            topLeftPos -= ROWS + 1;
        }

        valid = topLeftPos / COLUMNS != 0 && topLeftPos % ROWS != 0 && board[topLeftPos - (ROWS + 1)] == player;
    }

    return valid;
}

bool hasTopRightTrain(int* board, int player, int pos) {
    bool valid = false;

    int topRightPos = pos - (ROWS - 1);
    if (pos / COLUMNS != 0 && pos % ROWS != ROWS && board[topRightPos] == opponent(player)) {
        while (topRightPos / COLUMNS != 0 && topRightPos % ROWS != ROWS && board[topRightPos - (ROWS - 1)] == opponent(player)) {
            topRightPos -= ROWS - 1;
        }

        valid = topRightPos / COLUMNS != 0 && topRightPos % ROWS != ROWS && board[topRightPos - (ROWS - 1)] == player;
    }

    return valid;
}

bool hasBottomLeftTrain(int* board, int player, int pos) {
    bool valid = false;

    int bottomLeftPos = pos + (ROWS - 1);
    if (pos / COLUMNS != COLUMNS && pos % ROWS != 0 && board[bottomLeftPos] == opponent(player)) {
        while (bottomLeftPos / COLUMNS != COLUMNS && bottomLeftPos % ROWS != 0 && board[bottomLeftPos + (ROWS - 1)] == opponent(player)) {
            bottomLeftPos += ROWS - 1;
        }

        valid = bottomLeftPos / COLUMNS != COLUMNS && bottomLeftPos % ROWS != 0 && board[bottomLeftPos + (ROWS - 1)] == player;
    }

    return valid;
}

bool hasBottomRightTrain(int* board, int player, int pos) {
    bool valid = false;

    int bottomRightPos = pos + (ROWS + 1);
    if (pos / COLUMNS != COLUMNS && pos % ROWS != ROWS && board[bottomRightPos] == opponent(player)) {
        while (bottomRightPos / COLUMNS != COLUMNS && bottomRightPos % ROWS != ROWS && board[bottomRightPos + (ROWS + 1)] == opponent(player)) {
            bottomRightPos += ROWS + 1;
        }

        valid = bottomRightPos / COLUMNS != COLUMNS && bottomRightPos % ROWS != ROWS && board[bottomRightPos + (ROWS + 1)] == player;
    }

    return valid;
}
