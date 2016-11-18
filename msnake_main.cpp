#include <iostream>
using namespace std;
#include "termfuncs.h"
#include "snake.h"
#include "msnake.h"

// 	msnake_main.cpp
// 	 purpose: recreating the snake game by using the skills we have 
// 		  learned so far, particularly by using a 2-D array and a
// 		  class. in short, the m-snake game is a multi player 
//		  version of the snake game that has two snakes that can be
// 		  moved around by the user by pressing the 3 keys that the 
// 		  user chooses to move right, left, and forward. 
//		  there are 2 snacks on the board that the snake wants to eat
//		  to gain more energy. every time the snake moves, the user 
//		  gains 1 point in score and loses 1 unit in energy. if the
//		  snake runs out of energy or runs into its own body, the 
//		  game ends. a snake wins only if the snake is still alive 
//		  and has a higher score.
//	    note: this file only has the main function and the helpers.
//		  other functions are from the snake.h, snake.cpp
//		  and declared from msnake.h
//     	      by: Annie Chen
// last modified: Nov. 10, 2015

// -------------------------------------------------------------
// ----- Main Game that uses a Snake ---------------------------
// -------------------------------------------------------------
/*
 * multi-snake game
 */

Snake	setup_snake(int, Snake, int row, int col, char dir, int EU);
string	play_game(char[ROWS][COLS]);
void	display(char [ROWS][COLS], Snake, Snake);
bool place_snack(char bug, char board[ROWS][COLS]);
string	compute_outcome(Snake, Snake);
void setup_board(char board[ROWS][COLS]);
void setup_topbottom(char board[ROWS][COLS]);
void setup_sides(char board[ROWS][COLS]);
void setup_spaces(char board[ROWS][COLS]);
void draw_board(char board[ROWS][COLS]);
bool check_food(char food, char board[ROWS][COLS]); 
void print_food(char food, char board[ROWS][COLS]);
void report(string result); 

//    ---- note: DO NOT MODIFY main() AT ALL ----
int main()
{
	char	board[ROWS][COLS];
	string	result;
	setup_board(board);
	result = play_game(board);
	report(result);
}


// setup_board
// purpose: to call other functions that all together draws the board
// arguments: a blank 2-D array board that doesn't have anything in it yet
// returns: nothing, because it is a void function
void setup_board(char board[ROWS][COLS])
{ 
	setup_topbottom(board);
	setup_sides(board);
	setup_spaces(board);
}


// setup_topbottom
// purpose: dash and corner the top and bottom of the board (rows 0 & 23)
// arguments: a blank 2-D array board
// returns: nothing, because it is a void function
void setup_topbottom(char board[ROWS][COLS])
{
	board[0][0] = '+';
	board[0][COLS-1] = '+';
	board[ROWS-1][0] = '+';
	board[ROWS-1][COLS-1] = '+';

	for (int c = 1; c < COLS -1; c++) {
		board[0][c] = '-';
		board[ROWS-1][c] = '-';
	}
}

// setup_sides
// purpose: put lines in the sides of the board (columns 0 & 62)
// arguments: a 2-D array board
// returns: nothing, because it is a void function
void setup_sides(char board[ROWS][COLS]) 
{
	for (int r = 1; r < ROWS-1; r++) {
		board[r][0] = '|';
		board[r][COLS-1] = '|';
	}
}	

// setup_spaces
// purpose: put spaces in the rest of the board, which are the places in 
// 	    the array that has no board border
// arguments: a 2-D array board
// returns: nothing, because it is a void function
void setup_spaces(char board[ROWS][COLS])
{
	for (int r = 1; r < ROWS-1; r++) {
		for (int c = 1; c < COLS-1; c++) {
			board[r][c] = ' ';
		}
	}	
}

// play_game -- run a game of multi-snake
//    args: a board ready to use
//    rets: "X wins" or "X died" where X is name of snake
//    note: USE THIS MAIN UNCHANGED FOR PHASE 0
//    note: FOR PHASE 1, move setup_snake into class 
//
string play_game(char b[ROWS][COLS])
{
	Snake	s0, s1;		// the players
	char	input;
	string	outcome = "";

	// set up the snakes and put food on board
	screen_clear();
	s0.setup_snake(0, ROWS/2-1, COLS/2, '^', EU_INIT);
	s1.setup_snake(1, ROWS/2  , COLS/2, 'v', EU_INIT);
	s0.draw_head_on_board(b);
	s1.draw_head_on_board(b);
	place_snack(BUG_SYM, b);
	place_snack(EGG_SYM, b);

	// play the game
	while( s0.is_alive() && s1.is_alive() )
	{
		display(b, s0, s1);
		input = getachar();
		s0.process_key(input, b);
		s1.process_key(input, b);
	}
	display(b, s0,s1);

	outcome = compute_outcome(s0, s1);
	return outcome;
}

// compute_outcome -- determine if someone won
//  args: two snakes
//  rets: "X wins" OR  "X died"
//  note: to win, a snake must be alive AND have a higher score
string compute_outcome(Snake a, Snake b)
{
	if (a.is_alive() && !b.is_alive() &&
	    a.get_score() > b.get_score()) {
		return a.get_name() + " wins";
	}
	
       	else if (b.is_alive() && !a.is_alive() &&
		 b.get_score() > a.get_score()) {
		return b.get_name() + " wins";
	}

	else if (!a.is_alive()) {
		return a.get_name() + " died";
	}

	else {
		return b.get_name() + " died";
	} 
}

// display - show current state of game including board and snake status
//  args: the board, the snakes
//  rets: nothing
//  does: sends info to cout
//
void display(char brd[ROWS][COLS], Snake a, Snake b)
{
	draw_board(brd);
	a.print_status();
	b.print_status();
}

// place_snack
// purpose: to place the snack onto the board. this function does this by
// 	    calling another function that will first check if there is 
//	    already on the board
// arguments: the snack that is to be placed
// returns: nothing
bool place_snack(char bug, char board[ROWS][COLS])
{
	if ( check_food(bug, board)) {
		return true;
	}
	
	else {
		return false;
	}
}

// check_food
// purpose: to check if there is the character offood on the board already.
// 	    if there isn't, meaning that the food is eaten, a food will be 
// 	    placed by print food function
// arguments: the character of the food that is to be checked/placed and the
//	     2-D array board
// returns: nothing
bool check_food(char food, char board[ROWS][COLS])
{
	bool snack = false;
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			if (board[r][c] == food) {
				snack = true;
				return false;  // no snacks were printed
			}
		}
	}
	
	if (!snack) {
		print_food(food, board);
		return true;		// a snack was printed
	}
	return false;
}

// print_food
// purpose: to put the food into the 2-D array board at a random location. 
// 	    this function also checks to see if the random location already
// 	    has a character. if it does, meaning that a snake/food is 
//	    occupying that location, another random location will be 
//	    selected until the function finds a free location
// arguments: the character of the food that is to be printed and 
// 	      the 2-D array board 
// returns: nothing
void print_food(char food, char board[ROWS][COLS]) 
{
	int rdmrow, rdmcol;
	rdmrow = random_int(1, ROWS-2);
	rdmcol = random_int(1, COLS-2);

	while (board[rdmrow][rdmcol] != ' ') { 	// checks to see if spot 
		rdmrow = random_int(1, ROWS-2);	     // on board is occupied
		rdmcol = random_int(1, COLS-2);      // if yes, finds another
	}					     // random location

	board[rdmrow][rdmcol] = food; 
 
}	

// draw_board
// purpose: prints the 2-D array board
// arguments: the 2-D array board
// returns: nothing
void draw_board(char board[ROWS][COLS])
{
	screen_clear();
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			cout << board[r][c];
		}
		cout << endl;
	}
}

// report
// purpose: to print out which player wins or dies
// arguments: a string that contains the name of player
//	       who won or the name of player that died
// returns: nothing
void report(string result)
{
	cout << "Game over. " << result << endl;
}
