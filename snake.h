#ifndef SNAKE_H
#define SNAKE_H
#include "msnake.h"

using namespace std;

// snake.h
// purpose: to declare the snake class definitions
//    name: Annie Chen
//    date: Nov 11, 2015


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

#endif
