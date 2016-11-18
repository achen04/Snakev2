#include <iostream>
using namespace std;
#include "termfuncs.h"

// 	msnake.cpp - 2snake version 1.0
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
//	    note: the following constants control the size of the board and 
//		  the energy unit that the snakes start at. it also controls
//		  the food symbols and the food score points 	  
//     	      by: Annie Chen
// last modified: Nov. 10, 2015


const int 	ROWS    = 24;		// change if you like
const int 	COLS    = 63;		// change if you like
const int	EU_INIT = 60;

const char	BUG_SYM = 'X';
const int	BUG_VAL = 25;
const char	EGG_SYM = 'O';
const int	EGG_VAL = 15;

// -------------------------------------------------------------
// ----- SNAKE DEFINITION --------------------------------------
//              CODE appears at end of this file
// -------------------------------------------------------------

class Snake
{
    public:
	void	init();
	void	set_name(string);
	void	set_pos(int r, int c, char dir);
	void	set_controls(char l, char r, char f);
	void	set_body(char);
	void	set_EU(int eu);
	void	set_color(string fg, string bg);	// not required

	string	get_name();
	int	get_score();

	bool	process_key(char input, char b[ROWS][COLS]);
	bool	is_alive();
	void	print_status();
	void	draw_head_on_board(char b[ROWS][COLS]);
	void 	setup_snake(int num, int r, int c, char d, int eu);

    private:
	int	energy, score;
	int	row, column;
	char	dir;
	int	direction;	 // the index number of the headdir array  
	char	headdir[4];
	string	name;	
	char	body;
	char	right, left, forward;
	bool	alive;
	void	move_forward(char b[ROWS][COLS]);
	void 	wrap_snake();

};

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
