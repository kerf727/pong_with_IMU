#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "./game.h"
#include "./logic.h"

void initialize_game_state(game_t *game, int difficulty_level)
{
	game->state = RUNNING_STATE;
	game->countdown_s = 0;

	game->player_score = 0;
	game->opponent_score = 0;

	game->player_x_pos = WIDTH / 2;
	game->player_x_vel = 0;

	game->opponent_x_pos = WIDTH / 2;
	game->opponent_x_vel = 0;

	game->ball_pos[0] = WIDTH  / 2;
	game->ball_pos[1] = HEIGHT / 2;

	if (difficulty_level == 1)
	{
		game->opponent_speed = 200;
		game->ball_speed = 300;
	}
	else if (difficulty_level == 2)
	{
		game->opponent_speed = 250;
		game->ball_speed = 350;
	}
	else if (difficulty_level == 3)
	{
		game->opponent_speed = 300;
		game->ball_speed = 400;
	}
	else
	{
		fprintf(stderr, "Difficulty level must be between 1 and 3.\n");
	}
	game->difficulty_level = difficulty_level;

	game->ball_vel[0] = game->ball_speed / 2;
	game->ball_vel[1] = game->ball_speed;
}

void update_game_state(game_t *game)
{
	// Update player position
	game->player_x_pos += game->player_x_vel / FPS;

	// Handle when player paddle hits edge of screen
	if (game->player_x_pos - PADDLE_WIDTH / 2 <= 0)
		game->player_x_pos = PADDLE_WIDTH / 2;
	if (game->player_x_pos >= WIDTH - PADDLE_WIDTH / 2)
		game->player_x_pos = WIDTH - PADDLE_WIDTH / 2;

	// Update opponent velocity to track ball
	if (abs(game->ball_pos[0] - game->opponent_x_pos) > PADDLE_WIDTH / 4)
	{
		if (game->ball_pos[0] > game->opponent_x_pos)
			game->opponent_x_vel = game->opponent_speed;
		else
			game->opponent_x_vel = -game->opponent_speed;
	}
	else
	{
		game->opponent_x_vel = game->ball_vel[0] / 2;
	}

	// Update opponent position
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

	srand(time(NULL));
	int max_dither = game->ball_speed / 5;
	int ball_speed_dither = (rand() % max_dither) - max_dither / 2;

	// Check for goals, update scores, reset ball position and velocity
	// Player scored, send ball to opponent
	if (game->ball_pos[1] <= PADDLE_OFFSET + PADDLE_THICKNESS)
	{
		game->player_score += 1;
		game->ball_pos[0] = WIDTH / 2;
		game->ball_pos[1] = HEIGHT / 2;
		game->ball_vel[0] = game->ball_speed / 2 + ball_speed_dither;
		game->ball_vel[0] *= (game->opponent_x_pos > 0) ? 1.0f : -1.0f;
		game->ball_vel[1] = -game->ball_speed;
	}

	// Opponent scored, send ball to player
	if (game->ball_pos[1] >= HEIGHT - PADDLE_OFFSET - PADDLE_THICKNESS)
	{
		game->opponent_score += 1;
		game->ball_pos[0] = WIDTH / 2;
		game->ball_pos[1] = HEIGHT / 2;
		game->ball_vel[0] = game->ball_speed / 2 + ball_speed_dither;
		game->ball_vel[0] *= (game->player_x_pos > 0) ? 1.0f : -1.0f;
		game->ball_vel[1] = game->ball_speed;
	}

	// Update game state
	if (game->player_score == WINNING_SCORE ||
		game->opponent_score == WINNING_SCORE)
		game->state = GAME_OVER_STATE;
}