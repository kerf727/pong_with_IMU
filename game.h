#ifndef GAME_H_
#define GAME_H_

#define FPS					60
#define WIDTH				480
#define HEIGHT				480
#define PADDLE_WIDTH		100
#define PADDLE_THICKNESS	10
#define PADDLE_OFFSET		10
#define PLAYER_SPEED		300
#define OPPONENT_SPEED		200
#define BALL_RADIUS			5
#define BALL_SPEED			300
#define MAX_ROLL_ANGLE		45.0f

#define WINNING_SCORE		1 // 5

#define RUNNING_STATE		0
#define PLAYER_WON_STATE	1
#define PLAYER_LOST_STATE	2
#define QUIT_STATE			3

typedef struct
{
	int state;

	int player_x_pos;
	int player_x_vel;
	int opponent_x_pos;
	int opponent_x_vel;

	int ball_pos[2];
	int ball_vel[2];

	int player_score;
	int opponent_score;
} game_t;

#endif // !GAME_H_