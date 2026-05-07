/**
 * Ernest Stover 
 * Implementation of the othello game in C++
 * Ai is configured using the mini max algorithm with 
 * the option of enabling or disabling alpha beta pruning 
 * started: 10.24.2025
 * finished: 11.9.2025
 */

#include <stdio.h>
#include <iostream>
#include <stack>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;
using Board = std::vector<std::vector<int>>;


#define BLACK_DISK 1
#define WHITE_DISK 2
#define ROW_AMOUNT 8
#define COL_AMOUNT 8
#define MAX_GAME_SCORE 64
#define NEG_INFINITY -1000
#define POS_INFINITY 1000
#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define RESET_TEXT "\033[0m"

class OthelloGame 
{
    private:
        const int SEARCH_DEPTH = 3;
        Board GAME_BOARD = Board(8, vector<int>(8, 0));     // Othello board is an 8x8
        bool two_player_;                                   // two player check 
        bool black_turn_ = true;                            // keeps track of piece turn 
        bool enable_beta_alpha_pruning_ = false;            // turns on alpha beta pruning 
        bool enable_debug_mode_ = false;                    // turns on debug move showing decision tree 
        bool ai_play_black_ = false;                        // check to see if Ai plays black
        int total_game_score_ = 0;
        int black_piece_score_ = 0;
        int white_piece_score_ = 0; 
    
    public:
        bool runGame = true;
        OthelloGame()

        {
            // setup for default state of the game  
            GAME_BOARD[3][3] = WHITE_DISK;
            GAME_BOARD[3][4] = BLACK_DISK;
            GAME_BOARD[4][3] = BLACK_DISK;
            GAME_BOARD[4][4] = WHITE_DISK;
        }

        /* -------------------------------------------------------------------------------------------- *
        *                                   GENERAL GAME FUNCTIONS                                      *
        * --------------------------------------------------------------------------------------------- */
        /**
         * pauses execution until a key is entered 
         */
        void waitforKeyStroke()
        {
            printf("\nEnter any key to continue: ");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);    // clear the input buffer 
            getchar();      // wait for char to be entered 
        }

        /**
         * prints the game options 
         */
        void printOptions()
        {
            short option_input;
            printf("\n             OTHELLO Options               ");
            printf("\n");
            while (1)
            {
                printf("\nOption 1: One player  Option 2: Two Player\n");
                printf("\nEnter an option: ");
                if (scanf("%hd", &option_input) != 1)
                {// clear the input buffer if a short is not entered 
                    while (getchar() != '\n');
                    cout << RED_TEXT << "INVALID: " << RESET_TEXT << "not an option\n";
                    continue;
                }

                if (option_input == 1) 
                {// enables one player 
                    two_player_ = false;
                    cout << GREEN_TEXT << "SUCCESS: " << RESET_TEXT << "One Player enabled\n";
                    break;
                }
                else if (option_input == 2) 
                {// enables two player 
                    two_player_ = true;
                    cout << GREEN_TEXT << "SUCCESS: " << RESET_TEXT << "Two Player enabled\n";
                    break;
                }
                else 
                {// invalid option entered
                    cout << RED_TEXT << "INVALID: " << RESET_TEXT << "not an option\n";
                }
            }
            waitforKeyStroke();
        }

        /**
         * prints the current game board 
         *  */ 
        void printBoard()
        {
            int row, col;

            printf("\n\n              OTHELLO              \n\n");
    
            // Print column numbers on top
            printf("  ");  // Spacing for row numbers
            for (col = 0; col < COL_AMOUNT; col++)
            {
                printf("  %d ", col);
            }
            printf("\n");

            // Print top border
            printf("  ");  // Spacing for row numbers
            for (col = 0; col < COL_AMOUNT; col++)
            {
                printf("----");
            }
            printf("-\n");
    
            // Iterate through gameboard 
            for (row = 0; row < ROW_AMOUNT; row++)
            {
                printf("%d |", row);  // Print row number on the left

                for (col = 0; col < COL_AMOUNT; col++) 
                { 
                    if (GAME_BOARD[row][col] == BLACK_DISK)
                    {
                        printf(" B ");    
                    }
                    else if(GAME_BOARD[row][col] == WHITE_DISK)
                    {
                        printf(" W ");
                    }
                    else // 0 or empty slot on gameboard 
                    {
                        printf("   ");
                    }
                    printf("|");
                } 
                printf("\n");

                // Print border between rows
                printf("  ");  // Spacing for row numbers
                for (col = 0; col < COL_AMOUNT; col++)
                {
                    printf("----");
                }
                printf("-\n");
            }
        }

        /**
         * responsible for inserting a new piece into the 
         * gameboard array 
         */
        void insertNewPiece()
        {       
            while(1) 
            {// checks for inserted piece 
                char piece_row_col[10];
                printf("\nEnter piece position (row col, e.g. 11):  ");
                printf(black_turn_ ? "Black: " : "White: ");
                scanf("%s", piece_row_col);

                // Extract individual digits
                int row = piece_row_col[0] - '0';     // First char
                int col = piece_row_col[1] - '0';     // Second char
            
                if (row < 0 || row >= ROW_AMOUNT || col < 0 || col >= COL_AMOUNT)
                {// check to make sure row or col is not greate than row amount or col amount 
                    cout << RED_TEXT << "INVALID POSITION->" << RESET_TEXT << " Try again.\n";
                    continue;
                }

                if (outFlank(GAME_BOARD, row, col, true)) 
                {// checks if black piece entered will cause a outflank and that the space isn't populated 
                    GAME_BOARD[row][col] = black_turn_ ? BLACK_DISK : WHITE_DISK;
                    black_turn_ = !black_turn_;
                    calculateScore();
                    return;       
                }
                else 
                {// outflank will not occur 
                    
                    if (GAME_BOARD[row][col] != 0)
                    {// error response for board space being occupied 
                        cout << RED_TEXT << "\n\nINVALID: " << RESET_TEXT << "GAME BOARD space occupied!!!\n";
                    }
                    else
                    {// error response for outflank not occuring on plyaer move 
                        cout << RED_TEXT << "\n\nINVALID: " << RESET_TEXT << "OUTFLANK will not occurr!!!\n";
                    }    
                    continue;  
                }
            } 
        }

        /**
         * generates the score for each player 
         * white & black 
         */
        void calculateScore()
        {
            black_piece_score_ = white_piece_score_ = 0;
            for (int row = 0; row < ROW_AMOUNT; row++)
            {// iterate through Game Board 
                for (int col = 0; col < COL_AMOUNT; col++)
                {
                    if (GAME_BOARD[row][col] == BLACK_DISK) {black_piece_score_++;}         // increase black piece score
                    else if (GAME_BOARD[row][col] == WHITE_DISK) {white_piece_score_++;}    // increase white score 
                }
            }
        }
        
        /**
         * function responsible for carrying out outflanks 
         */
        bool outFlank(Board& board, int row, int col, bool flipPieces)
        {
           bool outflankCheck = false;
           int away_piece_color = black_turn_ ? WHITE_DISK : BLACK_DISK;
           int home_piece_color = black_turn_ ? BLACK_DISK : WHITE_DISK;
           
           if (board[row][col] != 0)
           {// space on board already occupied
                return false;
           }

           int directions[8][2] = {// all outflank directions to check 
                {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1},
                {-1, 1}, {1, -1}, {1, 1}
            };

            for (auto &dir : directions)
            {// enhanced for loop to check each direction an outlfank can occur 
                int cur_row = row + dir[0];
                int cur_col = col + dir[1];
                vector<pair<int, int>> toFlip;      // list to hold the row,col pair of pieces that need to be flipped 

                while (cur_row >= 0 && cur_row < ROW_AMOUNT && cur_col >= 0 && cur_col < COL_AMOUNT)
                {// move along direction
                    if (board[cur_row][cur_col] == away_piece_color)
                    {// if piece on the board is equal to opp color then add it to toFlip list 
                        toFlip.push_back({cur_row, cur_col});
                    }
                    else if (board[cur_row][cur_col] == home_piece_color)
                    {// if piece on the board is equal to our piece color then check if outflank is possible or if toFlip list is empty
                        if (!toFlip.empty())
                        {// found a valid outflank
                            outflankCheck = true;
                            if (flipPieces)
                            {
                                for (auto &[fr, fc] : toFlip)   // get each piece row,col out of the toFlip list
                                {
                                    board[fr][fc] = home_piece_color;   // update the value to our piece color 
                                }
                            }
                        }
                        break;
                    }
                    else {break;}
                    cur_row += dir[0];      // iterate to next row
                    cur_col += dir[1];      // iterate to next col 
                }
            }

            if (outflankCheck && flipPieces)        
            {// flip pieces and outflank is true so enter the piece into the game board
                board[row][col] = home_piece_color;
            }
            return outflankCheck;
        }

        /**
         * check to make sure piece has a valid move before playing ÷
         */
        bool hasAnyValidMove() {
            for (int cur_row = 0; cur_row < ROW_AMOUNT; cur_row++)
            {
                for (int cur_col = 0; cur_col < COL_AMOUNT; cur_col++)
                {
                    if (outFlank(GAME_BOARD, cur_row, cur_col, false))
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        /**
         * main game loop 
         */
        void playGame()
        { 
            bool gameOver = false;
            int noMoveCounter = 0;
            printOptions(); 
            cout << "here";

            while(!gameOver)
            {
                printBoard();       // show the current game board
                calculateScore();   // update the piece counts

                printf("\n Score -> Black %d | White: %d\n", black_piece_score_, white_piece_score_);
                // prints the state of Alpha-Beta pruning and Debug mode 
                cout << "\nAlpha-Beta Pruning is Currenlty " << (enable_beta_alpha_pruning_ ? "\033[32mON\033[0m" : "\033[31mOFF\033[0m") << "\n";
                cout <<"Debug Mode is Currently " << (enable_debug_mode_ ? "\033[32mON\033[0m" : "\033[31mOFF\033[0m") << "\n";

                if (!hasAnyValidMove())
                {// check if black or white has a valid move 
                    cout << (black_turn_ ? "Black" : "White") << " has no valid moves. Skipping turn";
                    black_turn_ = !black_turn_;
                    noMoveCounter++; // keeps track of the amount of times no moves has occured
                }
                else 
                { 
                    if (!two_player_ && !black_turn_) // checks if it is black turn to play and if two player is enabled 
                    {// WHITE TURN to play 
                        cout << "\nAI is exploring move tree... \n";
                        this_thread::sleep_for(chrono::seconds(2));         // pauses the execution of the program for 2 seconds 
                        Board board(8, vector<int>(8));
                        for (int row = 0; row < ROW_AMOUNT; row++)          // making a copy of the game board 
                        {
                            for (int col = 0; col < COL_AMOUNT; col++)
                            {
                                board[row][col] = GAME_BOARD[row][col];
                            }
                        }

                        auto [aiRow, aiCol] = getBestMove(board, SEARCH_DEPTH, false, enable_beta_alpha_pruning_); 

                        if (aiRow != -1 && aiCol != -1)
                        {
                            cout << "AI plays at (" << aiRow << ", " << aiCol << ")\n";
                            outFlank(GAME_BOARD, aiRow, aiCol, true);                   // checks what outflanks the new piece will cause  
                            GAME_BOARD[aiRow][aiCol] = WHITE_DISK;          // inserts the piece on the game board 
                        }
                        else
                        {
                            cout << "AI could not find a valid move, skipping turn.\n";
                        }
                        calculateScore();       // updates the score 
                        black_turn_ = true;     // changes turn back to black 

                        char choice;
                        if (enable_beta_alpha_pruning_)
                        {// in this state alpha beta pruning is enabled, check to see whether to disable or not 
                            cout << "\nWould you like to disable alpha-beta pruning for the next AI turn? (y/n): ";
                            cin >> choice;
                            if (choice == 'y' || choice == 'Y')
                            {
                                enable_beta_alpha_pruning_ = false;     // turn alpha beta pruning OFF 
                            }
                            else if (choice == 'n' || choice == 'N')
                            {
                                enable_beta_alpha_pruning_ = true;      // keep alpha beta pruning ON 
                            }
                            else
                            {
                                cout <<  "Invalid input, keeping current setting.\n"; 
                            }
                        }
                        else
                        {// in this state alpha beta pruning is disabled, check to whether to enable or not  
                            cout << "\nWould you like to enable alpha-beta pruning for the next AI turn? (y/n); ";
                            cin >> choice;
                            if (choice == 'y' || choice == 'Y')
                            {
                                enable_beta_alpha_pruning_ = true;      // turn alpha beta pruning ON
                            }
                            else if (choice == 'n' || choice == 'N')
                            {
                                enable_beta_alpha_pruning_ = false;     // turn alpha beta pruning OFF
                            }
                            else 
                            {
                                cout << "Invalid input, keeping current setting.\n";
                            }
                        }

                        if (enable_debug_mode_)
                        {// in this state debug mode is enabled, check to see whether to disable or not 
                            cout << "\nDisable debug mode (y/n): ";
                            cin >> choice;
                            if (choice == 'y' || choice == 'Y')
                            {
                                enable_debug_mode_ = false;         // turn debug mode OFF 
                            }
                            else if (choice == 'n' || choice == 'N')
                            {
                                enable_debug_mode_ = true;          // turn debug mode ON
                            }
                            else 
                            {
                                cout << "Invalid input, keeping current setting.\n";
                            }
                        }
                        else
                        {// in this state debug mode is disabled, check to see whether to enable or not
                            cout << "\nEnable debug mode (y/n): ";
                            cin >> choice;
                            if (choice == 'y' || choice == 'Y')
                            {
                                enable_debug_mode_ = true;          // turn debug mode ON
                            }
                            else if (choice == 'n' || choice == 'N')
                            {
                                enable_debug_mode_ = false;         // turn debug mode OFF
                            }
                            else 
                            {
                                cout << "Invalid input, keeping current setting.\n";
                            }
                        }
                    }
                    else 
                    {// BLACK TURN 
                        char ask_ai_play_black;
                        cout << "\nDo you want AI to play for you (y/n): ";
                        cin >> ask_ai_play_black;
                        if (ask_ai_play_black == 'y' || ask_ai_play_black == 'Y')
                        {// check to see if Ai play on black turn 
                            ai_play_black_ = true;      // Ai plays for black 
                        }
                        else if (ask_ai_play_black == 'n' || ask_ai_play_black == 'N')
                        {
                            ai_play_black_ = false;     // Ai does not play for black
                        }
                        else 
                        {
                            cout << "Invalid input, keeping current setting.\n";
                        }

                        if (ai_play_black_)
                        {// in this state Ai plays for black 
                            cout << "\nAI (playing as black for a turn)...\n";
                            this_thread::sleep_for(chrono::seconds(2));         // pauses the execution of the program for 2 seconds
                            Board board(8, vector<int>(8));
                            for (int row = 0; row < ROW_AMOUNT; ++row)          // making a copy of the game board
                            {
                                for (int col = 0; col < COL_AMOUNT; ++col)
                                {
                                    board[row][col] = GAME_BOARD[row][col];
                                }
                            }

                            auto [aiRow, aiCol] = getBestMove(board, SEARCH_DEPTH, true, enable_beta_alpha_pruning_);

                            if (aiRow != -1 && aiCol != -1)
                            {
                                cout << "AI played as black at (" << aiRow << ", " << aiCol << ")\n";
                                outFlank(GAME_BOARD, aiRow, aiCol, true);   // checks what outflanks the new piece will cause
                                GAME_BOARD[aiRow][aiCol] = BLACK_DISK;      // inserts the piece on teh game board 
                                ai_play_black_ = false;                     // turn Ai play for back to false
                                black_turn_ = false;                        // pass turn to white (human or AI)
                            }
                            else
                            {
                                cout << "AI (black) found no valid moves.\n";
                            }
                        }
                        else
                        {// Ai does not play, so the user enters a piece 
                            insertNewPiece();
                        }
                        calculateScore();       // updates the score
                        black_turn_ = false;    // changes turn back to white 
                    }
                    noMoveCounter = 0;  // reset skip counter
                }

                if (noMoveCounter >= 2)
                {// move counter is greater than 2 so both black and white haven't played -> END GAME 
                    gameOver = true;
                }
            }

            /* ------------------------------ END OF GAME SUMMARY ------------------------------ */ 
            calculateScore();
            printBoard();

            cout << "\nGAME OVER!\n";
            cout << "Final Score:\n";
            cout << "Black: " << black_piece_score_ << " | White: " << white_piece_score_ << "\n";

            if (black_piece_score_ > white_piece_score_) {cout << "Black wins!\n";}
            else if (white_piece_score_ > black_piece_score_) {cout << "White wins!\n";}
            else {cout << "It's a draw!\n";}
        }


        /* -------------------------------------------------------------------------------------------- *
        *                               ARTIFICIAL INTELLIGENCE FUNCTIONS                               *
        * --------------------------------------------------------------------------------------------- */
        /**
        * Implementation of minimax with with
        * the option to turn on belta alpha pruning 
        * and the option to show the steps take to get the best option 
        */
        int minMax(Board board, int depth, bool isMaximizing, int alpha, int beta, bool usePruning, string moveSeq = " ", int level = 0)
        {
            // Terminal Conditions 
            if (depth == 0)
            {// reached max depth 
                int eval = evaluateBoard(board);
                if (enable_debug_mode_)
                {// returns information about the move at depth at 0
                    cout << string(level * 2, ' ') << "Sequence: " << moveSeq << " => Heuristic: " << eval << endl;
                }
                return eval;
            }
            
            auto moves = getValidMoves(board, isMaximizing); // gets all the valid moves from current game board 

            if (moves.empty()) 
            {// no valid moves
                return minMax(board, depth - 1, !isMaximizing, alpha, beta, usePruning, moveSeq + " (pass)", level + 1);
            }

            if (isMaximizing)
            {// maximizing recursive call 
                int maxEval = NEG_INFINITY;     // set max value to a very small value 
                for (auto [row, col] : moves)
                {// evaluate each move 
                    Board newBoard = simulateMove(board, row, col, isMaximizing);                           // simulate move and create the resulting board 
                    string newSeq = moveSeq + " -> (" + to_string(row) + ", " + to_string(col) + ")";       // input the move data into a string for printing 
                    int eval = minMax(newBoard, depth-1, false, alpha, beta, usePruning, newSeq, level+1); // go to the next level of the tree and evalaute 
                    maxEval = max(maxEval, eval);   // see if the result is greater than our current max evaluation 
                    if (usePruning)
                    {// alpha beta pruning is enabled 
                        alpha = max(alpha, eval);
                        if (beta <= alpha) {break;}

                    }
                }
                return maxEval;
            } 
            else 
            {
                int minEval = POS_INFINITY;     // set min value to a very large value 
                for (auto [row, col] : moves)
                {// minimizing recurive call 
                    Board newBoard = simulateMove(board, row, col, !isMaximizing);                          // simulate move and create the resulting board
                    string newSeq = moveSeq + " -> (" + to_string(row) + ", " + to_string(col) + ")";       // create the move data into a string for printing 
                    int eval = minMax(newBoard, depth-1, true, alpha, beta, usePruning, newSeq, level+1);  // go to the next layer and evaluate
                    minEval = min(minEval, eval);   // see if the result is smaller than our current min evaluation
                    if (usePruning)
                    {// alpha beta pruning is enabled 
                        beta = min(beta, eval);
                        if (beta <= alpha) {break;}
                    }
                }
                return minEval;
            }
        }

        /**
         * returns the best move 
         * for the Ai to choose 
         */
        pair<int,int> getBestMove(Board board, int searchDepth, bool blackTurn, bool usePruning)
        {
            int bestVal = NEG_INFINITY;
            pair<int,int> bestMove = {-1, -1};

            auto moves = getValidMoves(board, blackTurn);       // get the valid moves based on the current game board
            for (auto [row, col] : moves)
            {// iterates through each move valid move 
                Board newBoard = simulateMove(board, row, col, blackTurn);              // simulate move and create the resulting board
                string moveSeq = "(" + to_string(row) + "," + to_string(col) + ")";     // create the move data into a string for printing  
                int moveVal = minMax(newBoard, searchDepth-1, !blackTurn, -1000, 1000, usePruning, moveSeq, 1);    // go to the next layer and evaluate
                if (moveVal > bestVal)
                {// update best value
                    bestVal = moveVal;
                    bestMove = {row, col};
                }
            }
            cout << "\nAI best move: (" << bestMove.first << ", " << bestMove.second <<     // print Ai best move 
                ") with score " << bestVal << endl;

            return bestMove;
        }

        /**
         * assigns a value to the position 
         * on the board based on heurstics 
         */ 
        int evaluateBoard(const Board& board)
        {
            static const int POSITION_WEIGHTS[8][8] = { //assign weights to postions on the game board
                {100, -20, 10, 5, 5, 10, -20, 100},
                {-20, -50, -2, -2, -2, -2, -50, -20},
                {10,  -2,  -1, -1, -1, -1,  -2,  10},
                {5,   -2,  -1, -1, -1, -1,  -2,   5},
                {5,   -2,  -1, -1, -1, -1,  -2,   5},
                {10,  -2,  -1, -1, -1, -1,  -2,  10},
                {-20, -50, -2, -2, -2, -2, -50, -20},
                {100, -20, 10, 5, 5, 10, -20, 100}
            };
           int score = 0;
           for (int cur_row = 0; cur_row < ROW_AMOUNT; cur_row++)
           {
                for (int cur_col = 0; cur_col < COL_AMOUNT; cur_col++)
                {
                    if (board[cur_row][cur_col] == BLACK_DISK) {score += POSITION_WEIGHTS[cur_row][cur_col];}       // if piece at the position is black add score 
                    else if (board[cur_row][cur_col] == WHITE_DISK) {score -= POSITION_WEIGHTS[cur_row][cur_col];}  // if piece at the position is white subtract score
                }
           }
            return score;
        }

        /**
         * creates a copy of our board
         */
        Board copyBoard(const Board& src) 
        {
            return src;
        }

        /**
         * returns all valid moves possible
         */
        vector<pair<int,int>> getValidMoves(Board board, bool blackTurn)
        {
            vector<pair<int,int>> moves;
            for (int cur_row = 0; cur_row < ROW_AMOUNT; cur_row++)
            {
                for (int cur_col = 0; cur_col < COL_AMOUNT; cur_col++)
                {
                    if (outFlank(board, cur_row, cur_col, blackTurn)) 
                    {
                        moves.push_back({cur_row, cur_col});
                    }
                }
            }
            return moves;
        }

        /**
         * copy of my outflank function 
         * used in my minimax algorithm to test
         * different paths in copy of the board game 
         */
        bool outFlankBoard(Board& board, int row, int col, bool flipPieces)
        {
            bool outflankCheck = false;
            int opp_piece_color = black_turn_ ? WHITE_DISK : BLACK_DISK;
            int our_piece_color = black_turn_ ? BLACK_DISK : WHITE_DISK;

            if (board[row][col] != 0)
            {
                return false; // space already occupied 
            }

            int directions[8][2] = { // all outflank directions to check 
                {-1, 0}, {1, 0}, {0, -1}, {0, 1},
                {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
            };

            for (auto &dir : directions)
            {
                int cur_row = row + dir[0];
                int cur_col = col + dir[1];
                vector<pair<int,int>> toFlip;  

                while (cur_row >= 0 && cur_row < ROW_AMOUNT && cur_col >= 0 && cur_col < COL_AMOUNT)
                {
                    if (board[cur_row][cur_col] == opp_piece_color)
                    {
                        toFlip.push_back({cur_row, cur_col});
                    }
                    else if (board[cur_row][cur_col] == our_piece_color)
                    {
                        if (!toFlip.empty())
                        {
                            outflankCheck = true;
                            if (flipPieces)
                            {
                                for (auto &p : toFlip)
                                board[p.first][p.second] = our_piece_color;
                            }
                        }
                        break;
                    }

                    else{break;}
                    cur_row += dir[0];
                    cur_col += dir[1];
                }
            }
            if (outflankCheck && flipPieces)
            {
                board[row][col] = our_piece_color;
            }
            return outflankCheck;
        }

        /**
         * simulates move for my AI to be
         * able to be able to seee options 
         */
        Board simulateMove(Board board, int row, int col, bool blackTurn)
        {
            outFlank(board, row, col, blackTurn);
            return board;
        }
};


int main() // main game loop 
{
    OthelloGame game; // create object of the game
    game.playGame(); // starts game loop
    return 0;
}

