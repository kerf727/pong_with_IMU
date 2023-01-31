#ifndef GAME_H_
#define GAME_H_

#define WIDTH				640
#define HEIGHT				480
#define SPEED				300
#define PADDLE_WIDTH		100

#define WINNING_SCORE		5

#define RUNNING_STATE		0
#define PLAYER_WON_STATE	1
#define PLAYER_LOST_STATE	2
#define QUIT_STATE			3

typedef struct
{
	int state;

	int player_x_pos;
	int opponent_x_pos;

	int player_score;
	int opponent_score;

	int ball_x_pos;
	int ball_y_pos;
} game_t;

#endif // !GAME_H_