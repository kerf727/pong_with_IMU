#ifndef GAME_H_
#define GAME_H_

/*
 * Difficulty Level: Set to a number between 1 (easiest) and 3 (hardest)
 */
#define DIFFICULTY_LEVEL 1

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

#define WINNING_SCORE		5

typedef enum
{
	START_SCREEN_STATE,
	RUNNING_STATE,
	GAME_OVER_STATE,
	RESET_STATE,
	QUIT_STATE,
	NUM_STATES
} game_state;

#define GAME_START_TIME_MS 3000
#define RESET_WAIT_TIME_MS 5000

typedef struct
{
	int state;
	int player_score;
	int opponent_score;
	int difficulty_level;
	int countdown_s;

	int player_x_pos;
	int player_x_vel;

	int opponent_speed;
	int opponent_x_pos;
	int opponent_x_vel;

	int ball_speed;
	int ball_pos[2];
	int ball_vel[2];
} game_t;

#endif // !GAME_H_