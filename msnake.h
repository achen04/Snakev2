#ifndef MSNAKE_H
#define MSNAKE_H

using namespace std;

// msnake.h
// purpose: declare functions that have already been defined in main
//    note: the following constants control the size of the board and 
//	    the energy unit that the snakes start at. it also controls
//	    the food symbols and the food score points
// 	by: Annie Chen
//    date: Nov 10. 2015

const int 	ROWS    = 24;		// change if you like
const int 	COLS    = 63;		// change if you like
const int	EU_INIT = 60;

const char	BUG_SYM = 'X';
const int	BUG_VAL = 25;
const char	EGG_SYM = 'O';
const int	EGG_VAL = 15;


// place_snack
// purpose: to place the snack onto the board. this function does this by
// 	    calling another function that will first check if there is 
//	    already on the board
// arguments: the snack that is to be placed
// returns: nothing
bool place_snack(char bug, char board[ROWS][COLS]);



// check_food
// purpose: to check if there is the character offood on the board already.
// 	    if there isn't, meaning that the food is eaten, a food will be 
// 	    placed by print food function
// arguments: the character of the food that is to be checked/placed and the
//	     2-D array board
// returns: nothing
bool check_food(char food, char board[ROWS][COLS]);



// print_food
// purpose: to put the food into the 2-D array board at a random location. 
// 	    this function also checks to see if the random location already
// 	    has a character. if it does, meaning that a snake/food is 
//	    occupying that location, another random location will be 
//	    selected until the function finds a free location
// arguments: the character of the food that is to be printed and 
// 	      the 2-D array board 
// returns: nothing
void print_food(char food, char board[ROWS][COLS]);


#endif
