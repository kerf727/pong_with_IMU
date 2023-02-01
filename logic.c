#include <stdlib.h>

#include "./game.h"
#include "./logic.h"

void update_game_state(game_t *game)
{
	// Update player position
	game->player_x_pos += game->player_x_vel / FPS;

	// Handle when player paddle hits edge of screen
	if (game->player_x_pos - PADDLE_WIDTH / 2 <= 0)
		game->player_x_pos = PADDLE_WIDTH / 2;
	if (game->player_x_pos >= WIDTH - PADDLE_WIDTH / 2)
		game->player_x_pos = WIDTH - PADDLE_WIDTH / 2;

	// Update opponent position to track ball
	if (abs(game->ball_pos[0] - game->opponent_x_pos) > PADDLE_WIDTH / 4)
	{
		if (game->ball_pos[0] > game->opponent_x_pos)
			game->opponent_x_vel = OPPONENT_SPEED;
		else
			game->opponent_x_vel = -OPPONENT_SPEED;
	}
	else
	{
		game->opponent_x_vel = game->ball_vel[0] / 2;
	}

	game->opponent_x_pos += game->opponent_x_vel / FPS;

	// Handle when opponent paddle hits edge of screen
	if (game->opponent_x_pos - PADDLE_WIDTH / 2 <= 0)
		game->opponent_x_pos = PADDLE_WIDTH / 2;
	if (game->opponent_x_pos >= WIDTH - PADDLE_WIDTH / 2)
		game->opponent_x_pos = WIDTH - PADDLE_WIDTH / 2;

	// Handle when ball hits wall
	if (game->ball_pos[0] - BALL_RADIUS <= 0)
		game->ball_vel[0] *= -1;
	if (game->ball_pos[0] >= WIDTH - BALL_RADIUS)
		game->ball_vel[0] *= -1;

	// Handle when ball hits opponent's paddle
	if (game->ball_pos[1] <= PADDLE_OFFSET + PADDLE_THICKNESS + BALL_RADIUS &&
		game->ball_pos[0] >= game->opponent_x_pos - PADDLE_WIDTH / 2 &&
		game->ball_pos[0] <= game->opponent_x_pos + PADDLE_WIDTH / 2)
	{
		game->ball_vel[0] += game->opponent_x_vel / 2;
		game->ball_vel[1] *= -1;
	}

	// Update ball position
	game->ball_pos[0] += game->ball_vel[0] / FPS;
	game->ball_pos[1] += game->ball_vel[1] / FPS;

	// Handle when ball hits player's paddle
	if (game->ball_pos[1] >= HEIGHT - PADDLE_OFFSET - PADDLE_THICKNESS - BALL_RADIUS &&
		game->ball_pos[0] >= game->player_x_pos - PADDLE_WIDTH / 2 &&
		game->ball_pos[0] <= game->player_x_pos + PADDLE_WIDTH / 2)
	{
		game->ball_vel[0] += game->player_x_vel / 2;
		game->ball_vel[1] *= -1;
	}

	// Check for goals, update scores, reset ball position and velocity
	if (game->ball_pos[1] <= PADDLE_OFFSET + PADDLE_THICKNESS)
	{
		game->player_score += 1;
		game->ball_pos[0] = WIDTH / 2;
		game->ball_pos[1] = HEIGHT / 2;
		game->ball_vel[0] = -BALL_SPEED / 2;
		game->ball_vel[1] = -BALL_SPEED;
	}

	if (game->ball_pos[1] >= HEIGHT - PADDLE_OFFSET - PADDLE_THICKNESS)
	{
		game->opponent_score += 1;
		game->ball_pos[0] = WIDTH / 2;
		game->ball_pos[1] = HEIGHT / 2;
		game->ball_vel[0] = BALL_SPEED / 2;
		game->ball_vel[1] = BALL_SPEED;
	}

	// Update game state
	if (game->player_score == WINNING_SCORE)
		game->state = PLAYER_WON_STATE;
	else if (game->opponent_score == WINNING_SCORE)
		game->state = PLAYER_LOST_STATE;
}