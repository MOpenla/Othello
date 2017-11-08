/******************************************************************************
 * Othello Game with Alpha-Beta Pruning
 *
 * Developed by: Mitchell Openlander
 * For: CS 4200 - Artificial Intelligence Methods
 *      Doctor J.K. Jake Lee
 *
 * //TODO general method used and whatnot
 ******************************************************************************/

//TO COMPILE:
// $ g++ othello.cpp aStarTree.cpp -lglut -lGL -lGLU

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

void reshape_handler(int, int);
void init_setup(int, int, const char*);
void onPaint(void);
void drawPiece(float, float, float, int, int);
void doAnimation(int);
void output(int, int, int, const char*);
void onKeyPress(unsigned char, int, int);
void onMouseMove(int, int);
void onMouseButton(int, int, int, int);

//Global Variables
const int ROWS = 8;
const int COLUMNS = 8;
const int BLACK = 1; //human player
const int WHITE = -1; //computer player

const int WINDOW_X = 712;
const int WINDOW_Y = 512;
const int SCORE_AREA_X = 200; //This is from the right side of the display
const char* WINDOW_NAME = "Othello";
const int ANI_MSEC = 10; //gap between frames
int* currentBoard;
int mousePos;

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

    while (hasMovesLeft(board, currentPlayer) || hasMovesLeft(board, opponent(currentPlayer))) { //Loop while there are still moves left
        if (currentPlayer == BLACK) { //Human is current player
            cout << "----------" << playerName << "'s turn" << "----------" << endl << endl;

            if (hasMovesLeft(board, currentPlayer)) { //Human has moves left
                displayStats(board);

                cout << endl << endl;

                int rowIndex = 0;
                int columnIndex = 0;

                cout << "X Index: ";
                cin >> rowIndex;
                cout << "Y Index: ";
                cin >> columnIndex;

                while (!isValidMove(board, currentPlayer, rowIndex, columnIndex)) { //Make sure a valid move is retrieved from the player
                    cout << "Sorry, that move is not valid." << endl;
                    cout << "Please try again." << endl << endl;

                    cout << "X Index: ";
                    cin >> rowIndex;
                    cout << "Y Index: ";
                    cin >> columnIndex;
                }

                placeTile(board, currentPlayer, rowIndex, columnIndex);

            } else { //Human has no moves
                cout << playerName << " has no available moves." << endl;
            }

            cout << endl;
        }

        if (currentPlayer == WHITE) { //Computer is current player
            cout << "----------" << computerName << "'s turn" << "----------" << endl << endl;

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
    cout << " - " << playerName << ":  " << score(board, BLACK) << endl;
    cout << " - " << computerName << ":  " << score(board, WHITE) << endl;

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
            output = 'X';
        } else if (board[i] == 1) {
            output = 'O';
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
            placeTile(newBoard, currentPlayer, i);
            int newScore = 0;

            if (hasMovesLeft(newBoard, opponent(currentPlayer))) { //Opponent can move next
                newScore = minChoice(currentPlayer, newBoard, recursionLimit, alpha, beta);
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
    bool pruned = false;

    if (depth <= 0) {
        max = heuristicEvaluation(board, player);
    } else {
        int newScore = 0;

        for (int i = 0; i < ROWS * COLUMNS && !pruned; i++) {
            if (isValidMove(board, player, i)) {
                int* newBoard = copyBoard(board);
                placeTile(newBoard, player, i);

                if (hasMovesLeft(newBoard, opponent(player))) { //Opponent can move next
                    newScore = minChoice(player, newBoard, depth - 1, alpha, beta);
                } else if (hasMovesLeft(newBoard, player)) { //Player can move next
                    newScore = maxChoice(player, newBoard, depth - 1, alpha, beta);
                } else { //No one can move next, therefore no further analysis can be made
                    newScore = differenceEvaluation(board, player);
                }

                if (newScore > max) {
                    max = newScore;
                }

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
    bool pruned = false;

    if (depth <= 0) {
        min = heuristicEvaluation(board, player);
    } else {
        int newScore = 0;

        for (int i = 0; i < ROWS * COLUMNS && !pruned; i++) {
            if (isValidMove(board, opponent(player), i)) {
                int* newBoard = copyBoard(board);
                placeTile(newBoard, opponent(player), i);

                if (hasMovesLeft(newBoard, player)) { //Player can move next
                    newScore = maxChoice(player, newBoard, depth - 1, alpha, beta);
                } else if (hasMovesLeft(newBoard, opponent(player))) { //Opponent can move next
                    newScore = minChoice(player, newBoard, depth - 1, alpha, beta);
                } else { //No one can move next, therefore no further analysis can be made
                    newScore = differenceEvaluation(board, player);
                }

                if (newScore < min) {
                    min = newScore;
                }

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
    //glutSpecialFunc(onSpecialKeyPress);
    glutTimerFunc(ANI_MSEC, doAnimation, 0);
}

void reshape_handler(int width, int height) {
    glViewport(0, 0, width, height); //set viewpoint
    glMatrixMode(GL_PROJECTION); //projection matrix
    glLoadIdentity(); //loads identity matrix
    gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height); //2D orthographic projection
}

void onPaint(void) {
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
    glColor3f(1.0, 1.0, 1.0); //Dark-ish red
    output(WINDOW_X - SCORE_AREA_X + 60, WINDOW_Y - 25, 1, "Othello");
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

    glFlush();
    glutSwapBuffers(); //double buffering
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
                exit(0);
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
