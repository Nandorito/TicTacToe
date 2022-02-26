/*
Fernando Colon
Game: Tic-Tac-Toe
02/07/2022
*/


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//global constants
const char X = 'X';
const char O = 'O';
const char EMPTY = ' ';
const char TIE = 'T';
const char NO_ONE = 'N';

//function prototypes
void instructions();
char askYesNo(string question);
int askNumber(string question, int high, int low = 0);
char humanPiece();
char opponent(char Piece);
void displayBoard(const vector<char>& board);
char winner(const vector<char>& board);
bool isLegal(const vector<char>& board, int move);
int humanMove(const vector<char>& board, char human);

//pay close attention that this is not passing by reference like the previous functions
int computerMove(const vector<char> board, char computer);
void announceWinner(char winner, char computer, char human);

//main function
int main()
{
    //inital setup
    int move;
    const int NUM_SQUARES = 9;
    vector<char> board(NUM_SQUARES, EMPTY);

    instructions();
    char human = humanPiece();//asks the user if they want to go first or not
    char computer = opponent(human);//gives the computer the option the user did not get
    char turn = X;//X goes first
    displayBoard(board);


    //start the game loop. it continues as long as no one has won yet
    while(winner(board) == NO_ONE)
    {
        if(turn == human)
        {
            move = humanMove(board, human);//gets a valid input then updates the board
            board[move] = human;
        }
        else
        {
            move = computerMove(board, computer);//gets a valid input for computer
            board[move] = computer;
        }
        displayBoard(board);
        turn = opponent(turn);
    }
    announceWinner(winner(board), computer, human);
    return 0;
}

void instructions()
{
    cout << "Welcome to the ultimate man-machine showdown: Tic-Tac-Toe.\n";
    cout << "--where human brain is pit against silicon processor\n\n";

    cout << "Make your move known by entering a number, 0-8. The number\n";
    cout << "corresponds to the desired board poistion, as illustrated:\n\n";

    cout << " 0 | 1 | 2\n";
    cout << " ---------\n";
    cout << " 3 | 4 | 5\n";
    cout << " ---------\n";
    cout << " 6 | 7 | 8\n\n";

    cout << "Prepare youself, human. The battle is about to begin.\n\n";
}

char askYesNo(string question)
{
    char response;
    do
    {
        cout << question << " (y/n): ";
        cin >> response;
    } while (response != 'y' && response != 'n');
    return response;
}    

int askNumber(string question, int high, int low)
{
    int number;
    do
    {
        cout << question << " (" << low << " - " << high << "): ";
        cin >> number;
    } while (number > high || number < low);
    return number;
}

char humanPiece()
{
    char goFirst = askYesNo("Do you require the first move?");
    if(goFirst == 'y')
    {
        cout << "\nThen take the first move. You will need it.\n";
        return X;
    }
    else
    {
        cout << "\nYour bravery will be your undoing...I will go first.\n";
        return O;
    }
}

char opponent(char piece)
{
    if(piece == X)
    {
        return O;
    }
    else
    {
        return X;
    }
}

void displayBoard(const vector<char>& board)
{
    cout << "\n\t" << board[0] << " | " << board[1] << " | " << board[2];
    cout << "\n\t---------\n";
    cout << "\n\t" << board[3] << " | " << board[4] << " | " << board[5];
    cout << "\n\t---------\n";
    cout << "\n\t" << board[6] << " | " << board[7] << " | " << board[8];
    cout << "\n\n";
}

char winner(const vector<char>& board)
{
    //all possible winning rows
    const int WINNING_ROWS[8][3] = {
        {0,1,2},
        {3,4,5},
        {6,7,8},
        {0,3,6},
        {1,4,7},
        {2,5,8},
        {0,4,8},
        {2,4,6},
    };
    const int TOTAL_ROWS = 8;

    //if any winning row has three values that are the same (and not EMPTY).
    //then we have a winner
    for(int row = 0; row < TOTAL_ROWS; ++row)
    {
        /*this if statement checks to make sure that a row isn't empty
        and that each char in the row is equal to one another, signifying
        the winner*/
        if( (board[WINNING_ROWS[row][0]] != EMPTY) && 
            (board[WINNING_ROWS[row][0]] == board[WINNING_ROWS[row][1]]) && 
            (board[WINNING_ROWS[row][1]] == board[WINNING_ROWS[row][2]]))
        {
            return board[WINNING_ROWS[row][0]];
        }
    }

    //since nobody won, check for a tie (no empty squares left)
    if(count(board.begin(), board.end(), EMPTY) == 0)
    {
        return TIE;
    }

    //since nobody has won and it isn't a tie, the game aint over
    return NO_ONE;
}

inline bool isLegal(int move, const vector<char>& board)
{
    return (board[move] == EMPTY);    
}

int humanMove(const vector<char>& board, char human)
{
    int move = (askNumber("Where will you move?", board.size() - 1));
    while (!isLegal(move, board))
    {
        cout << "\nThat square is already occupied, foolish human.\n";
        move = (askNumber("Where will you move?", board.size() - 1));
    }

    cout << "Fine...\n";
    return move;    
}


/*i chose to not pass by reference and instead make a copy of the vector 
so the computer can make changes to the board a choose the best option available.
this way, the actual board is not altered.*/
int computerMove(vector<char> board, char computer)
{
    unsigned int move = 0;
    bool found = false;

    //if computer can win on next move, that's the move to make
    while (!found && move < board.size())
    {
        if(isLegal(move, board))
        {
            board[move] = computer;
            found = winner(board) == computer;
            board[move] = EMPTY;
        }
        
        if(!found)
        {
            ++move;
        }
    }
    //if the user will win on the next turn, prevent it from happening
    if(!found)
    {
        move = 0;
        char human = opponent(computer);
        while(!found && move < board.size())
        {
            if(isLegal(move, board))
            {
                board[move] = human;
                found = winner(board) == human;
                board[move] = EMPTY;
            }

            if(!found)
            {
                ++move;
            }
        }
    }
    //if none of the above, choose the best moves
    if(!found)
    {
        move = 0;
        unsigned int i=0;
        const int BEST_MOVES[] = {4,0,2,6,8,1,3,5,7};//i have it choose center, then corners, then edges

        //pick best square
        while(!found && i < board.size())
        {
            move = BEST_MOVES[i];
            if(isLegal(move, board))
            {
                found = true;
            }
            ++i;
        }
    }

    cout << "I shall take square number " << (move) << endl;
    return move;    
}

void announceWinner(char winner, char computer, char human)
{
    if(winner == computer)
    {
        cout << winner << "'s won!\n";
        cout << "As I predicted, human, I am triumphant once more -- proof\n";
        cout << "that computers are superior to humans in all regards.\n";
    }
    else if(winner == human)
    {
        cout << human << "'s won!\n";
        cout << "No, no! It cannot be! Somehow you tricked me, human.\n";
        cout << "But never again! I, the computer, so swear it!\n";
    }
    else
    {
        cout << "Its's a tie.";
        cout << "You were most lucky, human, and somehow managed to tie me.\n";
        cout << "Celebrate...for this is the best you will ever achieve.\n";
    }
}