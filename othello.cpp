/******************************************************************************
 *                    Othello Game with Alpha-Beta Pruning                    *
 *                                                                            *
 * Developed by: Mitchell Openlander                                          *
 * For: CS 4200 - Artificial Intelligence Methods                             *
 *      Doctor J.K. Jake Lee                                                  *
 *                                                                            *
 * This is an othello game programed in C++ tha uses Alpha-Beta pruning for   *
 *  it's evaluation function. The evaluation function relies on an array      *
 *  of values which represent how good a certain position is. The higer the   *
 *  number, the better the position.                                          *
 *                                                                            *
 * Black pieces are represented by a  1.                                      *
 * White pieces are represented by a -1.                                      *
 * Human is Black.                                                            *
 * Computer is White.                                                         *
 * Black goes first.                                                          *
 *                                                                            *
 * ---------------------------- Map of the Board ---------------------------- *
 *                                                                            *
 * Rows go down while columns go across.                                      *
 *  Or the row position is the x position and                                 *
 *   the column position is the y position.                                   *
 *                                                                            *
 * The following map represents the translation between the 1D array index    *
 *  and the coordinates (Row, Column) index.                                  *
 *                                                                            *
 *    //   0  1  2  3  4  5  6  7  Row Index                                  *
 *                                                                            *
 *    0    0  1  2  3  4  5  6  7                                             *
 *    1    8  9 10 11 12 13 14 15                                             *
 *    2   16 17 18 19 20 21 22 23                                             *
 *    3   24 25 26 27 28 29 30 31                                             *
 *    4   32 33 34 35 36 37 38 39                                             *
 *    5   40 41 42 43 44 45 46 47                                             *
 *    6   48 49 50 51 52 53 54 55                                             *
 *    7   56 57 58 59 60 61 62 63                                             *
 *                                                                            *
 * Calculations:                                                              *
 *  Row Index = pos % 8                                                       *
 *  Column Index = pos / 8                                                    *
 *  pos = row + (8 * column)                                                  *
 *                                                                            *
 *                                                                            *
 * ------------------------------- To Compile ------------------------------- *
 *                                                                            *
 * $ g++ othello.cpp -o othello -lglut -lGL -lGLU                             *
 ******************************************************************************/

#include <iostream>
#include <string.h>
#include <climits> //max and min int

#include <GL/glut.h>
#include <math.h> //for drawing circle calculation

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

// GLUT Functions
void reshape_handler(int, int);
void init_setup(int, int, const char*);
void onPaint(void);
void drawFinalScore();
void drawBoard();
void drawPiece(float, float, float, int, int);
void doAnimation(int);
void output(int, int, int, const char*);
void onKeyPress(unsigned char, int, int);
void onMouseMove(int, int);
void onMouseButton(int, int, int, int);
void finalScoreMouseButton(int, int, int, int);
void boardMouseButton(int, int, int, int);

//Global Variables
const int ROWS    =  8;
const int COLUMNS =  8;
const int BLACK   =  1; //human player
const int WHITE   = -1; //computer player
const string computerName = "Othello 9001";
string playerName;

const int heuristicValues[ROWS * COLUMNS] = {
    1000, 50, 100, 100, 100, 100,  50, 1000,
    50,  -20, -10, -10, -10, -10, -20,   50,
    100, -10,   1,   1,   1,   1, -10,  100,
    100, -10,   1,   1,   1,   1, -10,  100,
    100, -10,   1,   1,   1,   1, -10,  100,
    100, -10,   1,   1,   1,   1, -10,  100,
    50,  -20, -10, -10, -10, -10, -20,   50,
    1000, 50, 100, 100, 100, 100,  50, 1000
};

int initialValues[ROWS * COLUMNS] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, -1,  1,  0,  0,  0,
    0,  0,  0,  1, -1,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0
};

// GLUT variables
const int WINDOW_X = 712;
const int WINDOW_Y = 512;
const int SCORE_AREA_X = 200; //This is from the right side of the display
const char* WINDOW_NAME = "Othello";
const int ANI_MSEC = 10; //gap between frames
int* currentBoard;
int mousePos;


int main(int argc, char **argv) {
    glutInit(&argc, argv);

    //Welcome the player and get their name for a more personalized experience
    cout << "Hello and Welcome to the Othello game." << endl << endl;
    cout << "Please enter your name: ";
    getline(cin, playerName);

    cout << endl;

    //Greet the player and lay down some base rules
    cout << "Greetings " << playerName << ". My name is " << computerName << "." << endl;
    cout << "Today we will be playing Othello. I hope you know the rules." << endl;
    cout << "You will play as Black (X) and I will play as White (O)." << endl;
    cout << "Let's get started." << endl << endl;

    currentBoard = copyBoard(initialValues);

    //Comment out these two lines to play a terminal game
    init_setup(WINDOW_X, WINDOW_Y, WINDOW_NAME);
    glutMainLoop();

    playGame(currentBoard);

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

/**
    Handles the main gameplay, cycling through the players until no more moves are left.
    Also outputs the final score.

    @param board the starting board in which the game is played on
*/
void playGame(int* board) {
    int currentPlayer = BLACK; //Black goes first

    while (hasMovesLeft(board, currentPlayer) || hasMovesLeft(board, opponent(currentPlayer))) { //Loop until there are no moves left
        if (currentPlayer == BLACK) { //Human is current player
            cout << "---------- " << playerName << "'s turn" << " ----------" << endl << endl;

            if (hasMovesLeft(board, currentPlayer)) { //Human has moves left
                displayStats(board);

                cout << endl << endl;

                int rowIndex = 0;
                int columnIndex = 0;

                cout << "X Index: ";
                cin  >> rowIndex;
                cout << "Y Index: ";
                cin  >> columnIndex;

                while (!isValidMove(board, currentPlayer, rowIndex, columnIndex)) { //Make sure a valid move is retrieved from the player
                    cout << "Sorry, that move is not valid." << endl;
                    cout << "Please try again." << endl << endl;

                    cout << "X Index: ";
                    cin  >> rowIndex;
                    cout << "Y Index: ";
                    cin  >> columnIndex;
                }

                placeTile(board, currentPlayer, rowIndex, columnIndex);

            } else { //Human has no moves
                cout << playerName << " has no available moves." << endl;
            }

            cout << endl;
        }

        if (currentPlayer == WHITE) { //Computer is current player
            cout << "---------- " << computerName << "'s turn" << " ----------" << endl << endl;

            if (hasMovesLeft(board, currentPlayer)) { //Computer has moves left
                displayStats(board);
                cout << endl << endl;

                int bestMove = findBestMove(board, currentPlayer);
                placeTile(board, currentPlayer, bestMove);

                cout << computerName << " placed a tile at " << (bestMove % 8) << ", " << (bestMove / 8) << "." << endl;
            } else { //Computer has no moves left
                cout << computerName << " has no available moves." << endl;
            }

            cout << endl;
        }

        currentPlayer = opponent(currentPlayer);
    }

    //At this point neither player will have any more moves left.
    //Figure out the final score and declare a winner.
    int humanScore = score(board, BLACK);
    int computerScore = score(board, WHITE);

    cout << "No more moves left." << endl << endl;
    cout << "---Final Score---"   << endl;
    cout << playerName   << ":  " << humanScore    << endl;
    cout << computerName << ":  " << computerScore << endl << endl;

    if (humanScore > computerScore) { //Human wins
        cout << playerName  << " wins!" << endl;
        cout << "Good job!" << endl;
    } else if (computerScore > humanScore) { //Computer wins
        cout << computerName << " wins." << endl;
        cout << "Better luck next time." << endl; //Offer words of encouragement so the human dosen't feel crushed
    } else { //Tie
        cout << "Tie game." << endl;
    }
}

/**
    Returns the value player's opponent.

    @param player the player value to find the opponent of
    @returns the integer value of the opponent.
*/
int opponent(int player) {
    int opnt = WHITE;

    if (player == WHITE) {
        opnt = BLACK;
    }

    return opnt;
}

/**
    Checks if the player can make a move on the board.

    @param board the board to check if the player has moves left on
    @param player the player who's being checked
    @return true if the player has a move left, false otherwise
*/
bool hasMovesLeft(int* board, int player) {
    bool hasMoveLeft = false;

    //Iterate through each position until a valid move is found
    // This only has to loop until a vaild move is found because
    //  once a move is found then the player has a move left.
    for (int i = 0; i < ROWS * COLUMNS && !hasMoveLeft; i++) {
        hasMoveLeft = isValidMove(board, player, i);
    }

    return hasMoveLeft;
}

/**
    Display the stats of the board including:
     The current score of both players and
     The current board configuration.

    @param board the board to display the stats of
*/
void displayStats(int* board) {
    cout << "Score" << endl;
    cout << " - "   << playerName   << ":  " << score(board, BLACK) << endl; //List the player first to make them feel special despite they're about to be crushed
    cout << " - "   << computerName << ":  " << score(board, WHITE) << endl;

    cout << endl << endl;

    cout << "    0  1  2  3  4  5  6  7" << endl; //Print row indexes
    int columnNum = 0;

    for (int i = 0; i < ROWS * COLUMNS; i++) {
        if (i % ROWS == 0) { //If dropping down to a new column
            cout << endl;    //End the current column
            cout << columnNum << "   ";  //Print the column index

            columnNum++;
        }

        char output = '-';

        if (board[i] == -1) { //Board is occupied by the computer
            output = 'X';
        } else if (board[i] == 1) { //Board is occupied by the human
            output = 'O';
        }

        cout << output << "  ";
    }
}

/**
    Overloaded function for isValidMove(int*, int, int)
     that accepts row and column indexes rather than the array index.

    @param board configuration of the board to check for the vaild move
    @param player which player the move is being searched for
    @param row row index of the cell which is being checked if it is valid
    @param column column index of the cell which is being checked if it is valid
    @returns true if the player can place a piece in (row, column) of the board
*/
bool isValidMove(int* board, int player, int row, int column) {
    return isValidMove(board, player, row + (ROWS * column));
}

/**
    Checks if the player can place a piece at position pos on the board.

    @param board configuration of the board to check for the vaild move
    @param player which player the move is being searched for
    @param pos array index of the cell which is being checked if it is valid
    @returns true if the player can place a piece in (row, column) of the board
*/
bool isValidMove(int* board, int player, int pos) {
    bool valid = false;

    if (board[pos] == 0) { //Cell is empty
        //Check the adjacent cells for the opponent's piece
        // which may be followed by more of the opponent's pieces
        // but ends with the player's piece.
        //Once it is determined that the move is valid then no
        // further checking is needed.
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

/**
    Overloaded function for placeTile(int*, int, int)
     that accepts row and column indexes rather than the array index.

    @param board in which to place the piece
    @param player player placing the piece
    @param row row index of the cell in which the piece is being placed
    @param column column index of the cell in which the piece is being placed
    @returns true if the piece was placed successfully
*/
bool placeTile(int* board, int player, int row, int column) {
    return placeTile(board, player, row + (ROWS * column));
}

/**
    Places the players piece in position pos on the board.

    @param board in which to place the piece
    @param player player placing the piece
    @param pos array index of the cell in which the piece is being placed
    @returns true if the piece was placed successfully
*/
bool placeTile(int* board, int player, int pos) {
    bool placed = false;

    if (isValidMove(board, player, pos)) { //The move is valid
        board[pos] = player; //Place the player's piece

        /**
            These blocks check the adjacent cells for an oppoent's train
             caboosed by player. If a train is found then all of the
             opponent's pieces in that train become the player's pieces.
        */

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
            while (rightPos % ROWS != ROWS - 1 && board[rightPos] == opponent(player)) {
                board[rightPos] = player;
                rightPos++;
            }

            placed = true;
        }

        //Down Cell
        if (hasBottomTrain(board, player, pos)) {
            int downPos = pos + ROWS;
            while (downPos / COLUMNS != COLUMNS - 1 && board[downPos] == opponent(player)) {
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
            while (topRightPos / COLUMNS != 0 && topRightPos % ROWS != ROWS - 1 && board[topRightPos] == opponent(player)) {
                board[topRightPos] = player;
                topRightPos -= ROWS - 1;
            }

            placed = true;
        }

        //Bottom Left Cell
        if (hasBottomLeftTrain(board, player, pos)) {
            int bottomLeftPos = pos + (ROWS - 1);
            while (bottomLeftPos / COLUMNS != COLUMNS - 1 && bottomLeftPos % ROWS != 0 && board[bottomLeftPos] == opponent(player)) {
                board[bottomLeftPos] = player;
                bottomLeftPos += ROWS - 1;
            }

            placed = true;
        }

        //Bottom Right Cell
        if (hasBottomRightTrain(board, player, pos)) {
            int bottomRightPos = pos + (ROWS + 1);
            while (bottomRightPos / COLUMNS != COLUMNS - 1 && bottomRightPos % ROWS != ROWS - 1 && board[bottomRightPos] == opponent(player)) {
                board[bottomRightPos] = player;
                bottomRightPos += ROWS + 1;
            }

            placed = true;

        }

    }

    return placed;
}

/**
    Finds the best move which currentPlayer can make on the board.
    This is the initial call of the MinMax algorithm.

    @param board configuration of the board to search for the best move
    @param currentPlayer player making the move
    @return array index of the position which will give the currentPlayer the best outcome
*/
int findBestMove(int* board, int currentPlayer) {
    int maxScore = INT_MIN; //negative infinity
    int bestMove = 0;       //This is the position to place the tile for the best move
    const int recursionLimit = 6; //6 might be overkill but performance usually doesn't suffer espcially with pruning turned on
    int alpha = INT_MIN; //negative infinity
    int beta  = INT_MAX; //positive infinity

    for (int i = 0; i < ROWS * COLUMNS; i++) { //Iterate through all the moves on the board
        if (isValidMove(board, currentPlayer, i)) { //Check if the currentPlayer can place a tile there
            int* newBoard = copyBoard(board); //Copy the board since we're using pointers and stuff. Basically don't mess up the real board.
            placeTile(newBoard, currentPlayer, i); //Go ahead and make the move
            int newScore = 0;

            if (hasMovesLeft(newBoard, opponent(currentPlayer))) { //Opponent can move next
                newScore = minChoice(currentPlayer, newBoard, recursionLimit, alpha, beta);
            } else if (hasMovesLeft(newBoard, currentPlayer)) { //Computer can move next
                newScore = maxChoice(currentPlayer, newBoard, recursionLimit, alpha, beta);
            } else { //No one can move next, therefore no further analysis can be made
                //Look at the difference in the final score rather than the heuristic
                // since it will be the final score that matters and not the best
                // place to place a tile.
                newScore = differenceEvaluation(board, currentPlayer);
            }

            //Check if placing a tile at this new position yeilds a greater
            // score than the max score. If it does then the new score
            // becomes the max score and its move is stored.
            if (newScore > maxScore) {
                maxScore = newScore;
                bestMove = i;
            }
        }
    }

    return bestMove;
}

/**
    Finds the maximum score the player can achive on the board.

    @param player player to find the maximum score for
    @param board configuration of the board to find the max score the player can achieve on
    @param depth current number of recursion calls left before relying on the evaluation function
    @param alpha current alpha value for the alpha-beta pruning
    @param beta current beta value for the alpha-beta pruning
    @returns the max score that player can achive on the board
*/
int maxChoice(int player, int* board, int depth, int alpha, int beta) {
    int max = INT_MIN;   //negative infinity
    bool pruned = false; //flag for stopping the loop in the case of pruning. Used to avoid multiple returns

    if (depth <= 0) { //Maximum number of recursions were met
        //Return the evaluation function
        max = heuristicEvaluation(board, player);
    } else {
        int newScore = 0;

        for (int i = 0; i < ROWS * COLUMNS && !pruned; i++) { //Iterate through all the moves on the board
            if (isValidMove(board, player, i)) {  //Check if the move is valid for player
                int* newBoard = copyBoard(board); //Copy the board since we're using pointers and stuff. Basically don't mess up the real board.
                placeTile(newBoard, player, i);  //Go ahead and make the move

                if (hasMovesLeft(newBoard, opponent(player))) { //Opponent can move next
                    newScore = minChoice(player, newBoard, depth - 1, alpha, beta);
                } else if (hasMovesLeft(newBoard, player)) { //Player can move next
                    newScore = maxChoice(player, newBoard, depth - 1, alpha, beta);
                } else { //No one can move next, therefore no further analysis can be made
                    //Look at the difference in the final score rather than the heuristic
                    // since it will be the final score that matters and not the best
                    // place to place a tile.
                    newScore = differenceEvaluation(board, player);
                }

                //Check if placing a tile at this new position yeilds a greater
                // score than the max score. If it does then the new score
                // becomes the max score.
                if (newScore > max) {
                    max = newScore;
                }

                //This is the pruning stage.
                //If the new score is greater than the already determined minimum
                // then nothing matters and this branch of the search can stop.
                //Else if the new score is greater than the maximum,
                // declare it the maximum to be used on further minMax alpha-beta pruning steps
                if (newScore >= beta) {
                    pruned = true;
                } else if (newScore > alpha) {
                    alpha = newScore;
                }

            }
        }
    }

    return max;
}

/**
    Finds the minimum score the player can achive on the board.
    This observes the play the opponent would make.

    @param player player to find the minimum score for
    @param board configuration of the board to find the min score the player can achieve on
    @param depth current number of recursion calls left before relying on the evaluation function
    @param alpha current alpha value for the alpha-beta pruning
    @param beta current beta value for the alpha-beta pruning
    @returns the min score that player can achive on the board
*/
int minChoice(int player, int* board, int depth, int alpha, int beta) {
    int min = INT_MAX;   //positive infinity
    bool pruned = false; //flag for stopping the loop in the case of pruning. Used to avoid multiple returns

    if (depth <= 0) { //Maximum number of recursions were met
        //Return the evaluation function
        min = heuristicEvaluation(board, player);
    } else {
        int newScore = 0;

        for (int i = 0; i < ROWS * COLUMNS && !pruned; i++) { //Iterate through all the moves on the board
            if (isValidMove(board, opponent(player), i)) { //Check if the move is valid for the player's opponent
                int* newBoard = copyBoard(board);          //Copy the board since we're using pointers and stuff. Basically don't mess up the real board.
                placeTile(newBoard, opponent(player), i);  //Go ahead and make the opponent's move

                if (hasMovesLeft(newBoard, player)) { //Since the opponent just moved, check if player can move next
                    newScore = maxChoice(player, newBoard, depth - 1, alpha, beta);
                } else if (hasMovesLeft(newBoard, opponent(player))) { //Opponent can move next
                    newScore = minChoice(player, newBoard, depth - 1, alpha, beta);
                } else { //No one can move next, therefore no further analysis can be made
                    //Look at the difference in the final score rather than the heuristic
                    // since it will be the final score that matters and not the best
                    // place to place a tile.
                    newScore = differenceEvaluation(board, player);
                }

                //Check if placing a tile at this new position yeilds a lesser
                // score than the min score. If it does then the new score
                // becomes the min score.
                if (newScore < min) {
                    min = newScore;
                }

                //This is the pruning stage.
                //If the new score is less than the already determined maximum
                // then nothing matters and this branch of the search can stop.
                //Else if the new score is less than the minimum,
                // declare it the minimum to be used on further minMax alpha-beta pruning steps
                if (newScore <= alpha) {
                    pruned = true;
                } else if (newScore < beta) {
                    beta = newScore;
                }

            }
        }
    }

    return min;
}

/**
    Calculates the score of the player using the heuristicValues array.

    @param board board to calculate the score on
    @param player player's score to be calculated
    @return the heuristic score of the player on the board
*/
int heuristicEvaluation(int* board, int player) {
    int playerCount = 0;
    int opponentCount = 0;
    int opnt = opponent(player);

    for (int i = 1; i < ROWS * COLUMNS; i++) { //Iterate through each cell in the board
        if (board[i] == player) { //Player owns the cell
            playerCount += heuristicValues[i]; //Add that cells heuristic value to the total player score
        } else if (board[i] == opnt) { //Opponent owns the cell
            opponentCount += heuristicValues[i]; //Add that cells heuristic value to the total opponent score
        }
    }

    //Return the difference in scores
    // because the player wants to win by as much as possible
    return (playerCount - opponentCount);
}

/**
    Calculates the score difference between the player and their opponent.

    @param board board to calculate the score on
    @param player player's score to be calculated
    @return the difference in score on the board between player and opponent
*/
int differenceEvaluation(int* board, int player) {
    return (score(board, player) - score(board, opponent(player)));
}

/**
    Calculates the score of a player.
    Score is how many cells the player owns.

    @param board board to calculate the score on
    @param player player's score to be calculated
    @return the score of the player on the board.
*/
int score(int* board, int player) {
    int score = 0;

    for (int i = 0; i < ROWS * COLUMNS; i++) {
        if (board[i] == player) {
            score++;
        }
    }

    return score;
}

/**
    Copies a board.

    @param board board to be copied
    @return a copy of the board
*/
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
    if (pos % ROWS != ROWS - 1 && board[rightPos] == opponent(player)) {
        while (rightPos % ROWS != ROWS - 1 && board[rightPos + 1] == opponent(player)) {
            rightPos++;
        }

        valid = rightPos % ROWS != ROWS - 1 && board[rightPos + 1] == player;
    }

    return valid;
}

bool hasBottomTrain(int* board, int player, int pos) {
    bool valid = false;

    int downPos = pos + ROWS;
    if (pos / COLUMNS != COLUMNS - 1 && board[downPos] == opponent(player)) {
        while (downPos / COLUMNS != COLUMNS - 1 && board[downPos + ROWS] == opponent(player)) {
            downPos += ROWS;
        }

        valid = downPos / COLUMNS != COLUMNS - 1 && board[downPos + ROWS] == player;
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
    if (pos / COLUMNS != 0 && pos % ROWS != ROWS - 1 && board[topRightPos] == opponent(player)) {
        while (topRightPos / COLUMNS != 0 && topRightPos % ROWS != ROWS - 1 && board[topRightPos - (ROWS - 1)] == opponent(player)) {
            topRightPos -= ROWS - 1;
        }

        valid = topRightPos / COLUMNS != 0 && topRightPos % ROWS != ROWS - 1 && board[topRightPos - (ROWS - 1)] == player;
    }

    return valid;
}

bool hasBottomLeftTrain(int* board, int player, int pos) {
    bool valid = false;

    int bottomLeftPos = pos + (ROWS - 1);
    if (pos / COLUMNS != COLUMNS - 1 && pos % ROWS != 0 && board[bottomLeftPos] == opponent(player)) {
        while (bottomLeftPos / COLUMNS != COLUMNS - 1 && bottomLeftPos % ROWS != 0 && board[bottomLeftPos + (ROWS - 1)] == opponent(player)) {
            bottomLeftPos += ROWS - 1;
        }

        valid = bottomLeftPos / COLUMNS != COLUMNS - 1 && bottomLeftPos % ROWS != 0 && board[bottomLeftPos + (ROWS - 1)] == player;
    }

    return valid;
}

bool hasBottomRightTrain(int* board, int player, int pos) {
    bool valid = false;

    int bottomRightPos = pos + (ROWS + 1);
    if (pos / COLUMNS != COLUMNS - 1 && pos % ROWS != ROWS - 1 && board[bottomRightPos] == opponent(player)) {
        while (bottomRightPos / COLUMNS != COLUMNS - 1 && bottomRightPos % ROWS != ROWS - 1 && board[bottomRightPos + (ROWS + 1)] == opponent(player)) {
            bottomRightPos += ROWS + 1;
        }

        valid = bottomRightPos / COLUMNS != COLUMNS - 1 && bottomRightPos % ROWS != ROWS - 1 && board[bottomRightPos + (ROWS + 1)] == player;
    }

    return valid;
}









/*
 * GLUT Functions
 */

bool endOfGame;

void init_setup(int width, int height, const char* windowName) {
    //Glut Init
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //single buffer, rgb colour
    glutInitWindowSize(width, height); //init window size
    glutInitWindowPosition(5, 5); //init window position
    glutCreateWindow((char*)windowName); //set the window's name
    glutReshapeFunc(reshape_handler); //set the reshape call back

    glutDisplayFunc(onPaint);
    glutKeyboardFunc(onKeyPress);
    glutPassiveMotionFunc(onMouseMove);
    glutMouseFunc(onMouseButton);
    glutTimerFunc(ANI_MSEC, doAnimation, 0);

    endOfGame = false;
}

void reshape_handler(int width, int height) {
    glViewport(0, 0, width, height); //set viewpoint
    glMatrixMode(GL_PROJECTION); //projection matrix
    glLoadIdentity(); //loads identity matrix
    gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height); //2D orthographic projection
}

void onPaint(void) {
    if (endOfGame) {
        drawFinalScore();
    } else {
        drawBoard();
    }


    glFlush();
    glutSwapBuffers(); //double buffering
}

void drawFinalScore() {
    int humanScore = score(currentBoard, BLACK);
    int computerScore = score(currentBoard, WHITE);
    string winnerString;
    string messageString;

    if (humanScore > computerScore) { //Human Wins
        glClearColor(0.65, 0.84, 0.65, 1.00); //green to signify win
        glClear(GL_COLOR_BUFFER_BIT); //clear the colour from the buffer

        winnerString = playerName + " wins!";
        messageString = "Congradulations! I didn't think you had it in you!";
    } else if (computerScore > humanScore) { //Computer Wins
        glClearColor(1.00, 0.32, 0.32, 1.00); //red to signify loss
        glClear(GL_COLOR_BUFFER_BIT); //clear the colour from the buffer

        winnerString = computerName + " wins.";
        messageString = "We can't all be winners.    Better luck next time.";
    } else { //Tie Game
        glClearColor(0.96, 0.96, 0.96, 1.0); //gray for neutrality
        glClear(GL_COLOR_BUFFER_BIT); //clear the colour from the buffer

        winnerString = "                  Tie Game                      ";
        messageString = "              Better luck next time.              ";
    }

    //Display output messages
    glColor3f(0.0, 0.0, 0.0); //black
    output(225, WINDOW_Y - 50, 1, winnerString.c_str());
    output(125, WINDOW_Y - 80, 2, messageString.c_str());

    //Display final scores
    glColor3f(0.0, 0.0, 0.0); //black
    output(125, WINDOW_Y - 150, 1, "Final Score:");
    output(135, WINDOW_Y - 175, 2, playerName.c_str());
    output(300, WINDOW_Y - 175, 2, to_string(score(currentBoard, BLACK)).c_str());
    output(135, WINDOW_Y - 200, 2, computerName.c_str());
    output(300, WINDOW_Y - 200, 2, to_string(score(currentBoard, WHITE)).c_str());

    //Display buttons
    int gameSection_X = WINDOW_X - SCORE_AREA_X;
    int divisionX = gameSection_X / ROWS;
    int divisionY = WINDOW_Y / COLUMNS;
    int buttonsY  = WINDOW_Y - (7 * divisionY); //Put the buttons in the 7th column
    int exitButtonX = 2 * divisionX;
    int rematchButtonX = 6 * divisionX;

    // Exit Button
    glColor3f(0.0, 0.0, 0.0); //black
    glBegin(GL_LINES);
        glVertex2i(exitButtonX, buttonsY);
        glVertex2i(exitButtonX + divisionX * 2, buttonsY);

        glVertex2i(exitButtonX + divisionX * 2, buttonsY);
        glVertex2i(exitButtonX + divisionX * 2, buttonsY + divisionY);

        glVertex2i(exitButtonX + divisionX * 2, buttonsY + divisionY);
        glVertex2i(exitButtonX, buttonsY + divisionY);

        glVertex2i(exitButtonX, buttonsY + divisionY);
        glVertex2i(exitButtonX, buttonsY);
    glEnd();

    // Rematch Button
    glColor3f(0.0, 0.0, 0.0); //black
    glBegin(GL_LINES);
        glVertex2i(rematchButtonX, buttonsY);
        glVertex2i(rematchButtonX + divisionX * 2, buttonsY);

        glVertex2i(rematchButtonX + divisionX * 2, buttonsY);
        glVertex2i(rematchButtonX + divisionX * 2, buttonsY + divisionY);

        glVertex2i(rematchButtonX + divisionX * 2, buttonsY + divisionY);
        glVertex2i(rematchButtonX, buttonsY + divisionY);

        glVertex2i(rematchButtonX, buttonsY + divisionY);
        glVertex2i(rematchButtonX, buttonsY);
    glEnd();

    // Check if the mouse is over one of the buttons
    if (mousePos == 50 || mousePos == 51 || mousePos == 54 || mousePos == 55) {
        int buttonX = exitButtonX;

        if (mousePos == 54 || mousePos == 55) {
            buttonX = rematchButtonX;
        }

        glColor3f(0.0, 0.0, 0.0); //black
        glBegin(GL_POLYGON);
            glVertex2i(buttonX, buttonsY);
            glVertex2i(buttonX + divisionX * 2, buttonsY);
            glVertex2i(buttonX + divisionX * 2, buttonsY + divisionY);
            glVertex2i(buttonX, buttonsY + divisionY);
        glEnd();

        //Draw the button labels
        if (mousePos == 54 || mousePos == 55) {
            buttonX = rematchButtonX;
            glColor3f(0.0, 0.0, 0.0); //black
            output(175, 90, 2, "Exit");
            glColor3f(1.0, 1.0, 1.0); //white
            output(405, 90, 2, "Play Again");
        } else {
            glColor3f(1.0, 1.0, 1.0); //white
            output(175, 90, 2, "Exit");
            glColor3f(0.0, 0.0, 0.0); //black
            output(405, 90, 2, "Play Again");
        }
    } else {
        glColor3f(0.0, 0.0, 0.0); //black
        output(175, 90, 2, "Exit");
        output(405, 90, 2, "Play Again");
    }
}

void drawBoard() {
    glClearColor(0.96, 0.96, 0.96, 1.0); //background colour light gray
    glClear(GL_COLOR_BUFFER_BIT); //clear the colour from the buffer

    //Draw the score area
    glColor3f(0.18, 0.49, 0.20); //Dark green
    glBegin(GL_POLYGON);
        glVertex2i(WINDOW_X - SCORE_AREA_X, 0);
        glVertex2i(WINDOW_X - SCORE_AREA_X, WINDOW_Y);
        glVertex2i(WINDOW_X, WINDOW_Y);
        glVertex2i(WINDOW_X, 0);
    glEnd();

    //Draw the Score
    glColor3f(1.0, 1.0, 1.0); //white
    output(WINDOW_X - SCORE_AREA_X + 60, WINDOW_Y - 35, 1, "Othello");
    output(WINDOW_X - SCORE_AREA_X + 10, WINDOW_Y - 75, 2, "Score");
    output(WINDOW_X - SCORE_AREA_X + 20, WINDOW_Y - 100, 2, (playerName + ":").c_str());
    output(WINDOW_X - 50, WINDOW_Y - 100, 2, to_string(score(currentBoard, BLACK)).c_str());
    output(WINDOW_X - SCORE_AREA_X + 20, WINDOW_Y - 125, 2, (computerName + ":").c_str());
    output(WINDOW_X - 50, WINDOW_Y - 125, 2, to_string(score(currentBoard, WHITE)).c_str());
    //TODO print current player at the bottom of the score pane

    //Draw the Gridlines
    int gameSection_X = WINDOW_X - SCORE_AREA_X;
    int divisionX = gameSection_X / ROWS;
    int divisionY = WINDOW_Y / COLUMNS;

    glColor3f(0.0, 0.0, 0.0); //set pen colour to black
    glBegin(GL_LINES);
        //Vertical lines
        for (int i = divisionX; i < gameSection_X; i += divisionX) {
            glVertex2i(i, 0);
            glVertex2i(i, WINDOW_Y);
        }

        //Horizontal lines
        for (int i = divisionY; i < WINDOW_Y; i += divisionY) {
            glVertex2i(0, i);
            glVertex2i(gameSection_X, i);
        }
    glEnd();

    //Draw the Pieces
    for (int i = 0; i < ROWS * COLUMNS; i++) {
        if (currentBoard[i] != 0) { //Cell is not empty
            int row = i % COLUMNS;
            int column = i / ROWS;

            float posX = (row * divisionX) + (divisionX / 2);
            float posY = (column * divisionY) + (divisionY / 2);

            drawPiece(posX, WINDOW_Y - posY, (divisionX / 2) - 10, 20, currentBoard[i]);
        }
    }

    //Draw outline around current cell
    if (mousePos >= 0) {
        int row = mousePos % COLUMNS;
        int column = mousePos / ROWS;

        int posX = (row * divisionX);
        int posY = WINDOW_Y - (column * divisionY);

        if (isValidMove(currentBoard, BLACK, mousePos)) {
            glColor3f(0.0, 1.0, 0.0); //green
        } else {
            glColor3f(1.0, 0.0, 0.0); //red
        }

        glLineWidth(4.0);
        glBegin(GL_LINES);
            glVertex2i(posX, posY);
            glVertex2i(posX + divisionX, posY);

            glVertex2i(posX + divisionX, posY);
            glVertex2i(posX + divisionX, posY - divisionY);

            glVertex2i(posX + divisionX, posY - divisionY);
            glVertex2i(posX, posY - divisionY);

            glVertex2i(posX, posY - divisionY);
            glVertex2i(posX, posY);
        glEnd();
        glLineWidth(1.0);
    }
}

void drawPiece(float cx, float cy, float r, int num_segments, int player) {
    if (player == WHITE) {
        glColor3f(1.0, 1.0, 1.0);
    } else if (player == BLACK) {
        glColor3f(0.0, 0.0, 0.0);
    }

    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        glVertex2f(x + cx, y + cy);//output vertex

    }
    glEnd();

    //Draw circle outline
    glColor3f(0.0, 0.0, 0.0); //set pen colour to black
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        glVertex2f(x + cx, y + cy);//output vertex

    }
    glEnd();
}

void onKeyPress(unsigned char c, int x, int y) {
    if (c == 'q') { //Quit the program
        exit(0);
    }
}

void onMouseMove(int x, int y) {
    if (x < WINDOW_X - SCORE_AREA_X) { //Mouse is within game area
        int gameSection_X = WINDOW_X - SCORE_AREA_X;
        int divisionX = gameSection_X / ROWS;
        int divisionY = WINDOW_Y / COLUMNS;

        int row = x / divisionX;
        int column = y / divisionY;
        int pos = row + (ROWS * column);

        mousePos = pos;
    } else {
        mousePos = -1;
    }

    glutPostRedisplay();
}

void onMouseButton(int button, int state, int x, int y) {
    if (endOfGame) {
        finalScoreMouseButton(button, state, x, y);
    } else {
        boardMouseButton(button, state, x, y);
    }
}

void finalScoreMouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        int gameSection_X = WINDOW_X - SCORE_AREA_X;
        int divisionX = gameSection_X / ROWS;
        int divisionY = WINDOW_Y / COLUMNS;

        int row = x / divisionX;
        int column = y / divisionY;
        int pos = row + (ROWS * column);

        if (pos == 50 || pos == 51) { //Clicked exit button
            exit(0);
        } else if (pos == 54 || pos == 55) { //Clicked rematch button
            currentBoard = copyBoard(initialValues);
            endOfGame = false;
        }

        glutPostRedisplay();
    }
}

void boardMouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        int gameSection_X = WINDOW_X - SCORE_AREA_X;
        int divisionX = gameSection_X / ROWS;
        int divisionY = WINDOW_Y / COLUMNS;

        int row = x / divisionX;
        int column = y / divisionY;
        int pos = row + (ROWS * column);

        if (isValidMove(currentBoard, BLACK, pos)) {
            placeTile(currentBoard, BLACK, pos);
            glutPostRedisplay();

            if (hasMovesLeft(currentBoard, WHITE)) {
                int bestMove = findBestMove(currentBoard, WHITE);
                placeTile(currentBoard, WHITE, bestMove);
                glutPostRedisplay();

                while (!hasMovesLeft(currentBoard, BLACK) && hasMovesLeft(currentBoard, WHITE)) {
                    bestMove = findBestMove(currentBoard, WHITE);
                    placeTile(currentBoard, WHITE, bestMove);
                    glutPostRedisplay();
                }
            }

            if (!hasMovesLeft(currentBoard, BLACK) && !hasMovesLeft(currentBoard, WHITE)) { //Neither player has moves left. game over
                endOfGame = true;
            }
        }
    }
}

void doAnimation(int val) {

}

void output(int x, int y, int mode, const char* string) {
    int length;
    glRasterPos2i(x, y);
    length = (int)strlen(string);
    void* font = GLUT_BITMAP_TIMES_ROMAN_24;

    if (mode == 2) {
        font = GLUT_BITMAP_HELVETICA_18;
    }

    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(font, string[i]);
    }
}
