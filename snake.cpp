#include <iostream>
#include "snake.h"

using namespace std;

// snake.cpp
// purpose: To have the member functions of the snake class. these
//    	    written functions have to do with the snake and whenever
//	    a player moves/changes  the snake, a function here is called
//	    to  make the appropriate changes
//    name: Annie Chen
//    date: Nov 10, 2015


// -------------------------------------------------------------------------
//  SNAKE IMPLEMENTATION
//    The following section contains the member functions for a Snake
//---------------------------------------------------------------------------

//
// init -- set initial default values for a Snake 
//   args: none
//   rets: nothing
//   mods: modifies the snake by setting its member variables
//
void Snake::init()
{
	score    = 0;
	alive    = true;
	direction = 0;
}

// set_EU
// purpose: an accessor function to set the energy unit
// arguments: the energy unit of the snake to set
// returns: nothing
void Snake::set_EU(int eu)
{
	energy = eu;
}

// set_pos
// purpose: an accessor function that sets the row, column, and 
//	    direction of the snake head
// arguments: the row, column, and direction of the head to set
// returns: nothing
void Snake::set_pos(int r, int c, char d)
{
        row = r;
	column = c;
	dir = d;
}

// set_name
// Purpose: an accessor function to set the snake's name
// Arguments: a string with the name to set
// Return value: none
void Snake::set_name(string n)
{
	name = n;
}

// set_body
// purpose: an accessor function to set the body character
// arguments: the character of the snake body
// returns: nothing
void Snake::set_body(char bod)
{
	body = bod;	
}

// set_controls
// purpose: to set the controls that the player decided so that
//	    it corresponds to left, right, and forward keys
// arguments: the left key, right key, and forward key characters
// returns: nothing
void Snake::set_controls(char lkey, char rkey, char fkey) 
{
	left = lkey;
	right = rkey;
	forward = fkey;
}

// draw_head_on_board
// purpose: to put the head of the snake onto the board
// arguments: the 2-D array board
// returns: nothing
void Snake::draw_head_on_board(char b[ROWS][COLS])
{	
	if (dir == '^') {
		headdir[0] = '^'; 
		headdir[1] = '<';
		headdir[2] = 'v';
		headdir[3] = '>';
	}

	else if (dir == 'v') {
		headdir[0] = 'v'; 
		headdir[1] = '>';
		headdir[2] = '^';
		headdir[3] = '<';
	}
		       
	b[row][column] = headdir[direction];
}

// is_alive()
// purpose: to check is the snake is alive or not
// arguments: nothing
// returns: the boolean alive, which will return as true
// 	    or false
bool Snake::is_alive()
{
	return alive;
}

// get_name
// Purpose: to return the name of the snake
// Arguments: none
// Returns: a string with the snake's name
string Snake::get_name()
{
	return name;
}

// get_score
// purpose: to return the score of the player
// arguments: none
// returns: an integer with the score
int Snake::get_score()
{
	return score;
}

// print_status()
// purpose: to print the status, which is how the player is doing in terms of
//  	    energy and score, and also displays the keys that the user chose
// arguments: nothing
// returns: nothing
void Snake::print_status()
{
	cout << get_name() << " EU: " << energy << " Score: " <<
	        get_score() << " Keys " << right << left << forward << endl;

}

// process_key
// Purpose: to check if input is one of the keys and move the snake
//	     according to the input
// Arguments: the char that the user inputs and the board array
// Return value: true if input is one of the keys, and false if input is 
//		 not one of the keys
bool Snake::process_key(char input, char board[ROWS][COLS])
{		
	if (left == input) { 
		direction++;
	       	if (direction == 4)// if position is at the end of the array
		       	direction = 0; // the position goes back to the first
		draw_head_on_board(board); // element
		return true;		
	}
       	if (right == input) {
		direction--;
		if (direction == -1) // if position is at the front
			direction = 3; 	// of the array, position
		draw_head_on_board(board);	// goes to the end
		return true;			
	}
	if (forward == input) {
		board[row][column] = body;     
		score++;
		energy--;
		move_forward(board);
		draw_head_on_board(board);
		if (place_snack(BUG_SYM, board)) 
			energy = energy + BUG_VAL;
		if (place_snack(EGG_SYM, board))
			energy = energy + EGG_VAL;
		return true;
	}	
	else
		return false;
}

// move_forward
// purpose: moves the snake forward by going up the row/column or 
//          down row/column by editing the location of the snake which is 
//	    done by editing the row and column within the snake struct
// note: this is a private function
// arguments: nothing
// returns: nothing
void Snake::move_forward(char board[ROWS][COLS])
{
	if (energy < 1) {
		alive = false;	
	}

	if (headdir[direction] == '^') {
		row--;		// decreases row by one because going down 
	}

	if (headdir[direction] == '<') {
		column--;	// decrease column by one to go left
	}
	
	if (headdir[direction] == 'v') {
		row++;		// increase row by one to go up
	}

	if (headdir[direction] == '>') {
		column++;	// increase column by one to go right
	}
	
	wrap_snake();

	if (board[row][column] == body) {
		alive = false;
	}
}

// wrap_snake
// purpose: sends the snake to the other side of the board.
// it does this by sending the snake to from one vertical border to another,
// and one horizonal border to another
// note: this is a private function
// argumnents: nothing
// returns: nothing
void Snake::wrap_snake()
{
	if (row == 0) {
		row = ROWS-2;
	}

	if (row == ROWS-1) {
		row = 1;
	}

	if (column == 0) {
		column = COLS-2;
	}
	
	if (column == COLS-1) {
		column = 1;
	}
}

//
// setup_snake -- initialize a snake for the game
//  args: the snake number, the snake, its row, col, dir, and initial EU
//  returns: the score
void Snake::setup_snake(int num, int r, int c, char d, int eu)
{
	string	n;
	char	lkey,rkey,fkey, body;

	init();			// set any initial default values
	set_EU(eu);			// set initial energy
	set_pos(r, c, d);		// set pos and direction

	// now customize for the player
	cout << "Welcome, player " << num << endl;
	cout << "Name? ";
	cin  >> n;
	set_name(n);

	cout << "Body character? ";
	cin  >> body;
	set_body(body);

	cout << "Keys for left, right, fwd? ";
	cin  >> lkey >> rkey >> fkey ;
	set_controls(lkey, rkey, fkey);
}
