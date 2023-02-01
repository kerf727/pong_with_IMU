#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include "./game.h"
#include "./rendering.h"

#define SCORE_WIDTH			20
#define SCORE_HEIGHT		50
#define SCORE_Y_OFFSET		20
#define WIN_SCORE_WIDTH		150
#define WIN_SCORE_HEIGHT	150
#define FONT_FILE			("./OpenSans-Regular.ttf")
#define FONT_PTSIZE			24

const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255};
const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0};
const SDL_Color PADDLE_COLOR = WHITE;
const SDL_Color BALL_COLOR = WHITE;
const SDL_Color BACKGROUND_COLOR = BLACK;

void TTF_Init_font(void)
{
	font = TTF_OpenFont(FONT_FILE, FONT_PTSIZE);
}

void render_player(SDL_Renderer *renderer, const game_t *game)
{
	SDL_SetRenderDrawColor(renderer, PADDLE_COLOR.r, PADDLE_COLOR.g, PADDLE_COLOR.b, 255);

	SDL_Rect paddle_rect;
	paddle_rect.x = game->player_x_pos - PADDLE_WIDTH / 2;
	paddle_rect.y = HEIGHT - PADDLE_OFFSET - PADDLE_THICKNESS;
	paddle_rect.h = PADDLE_THICKNESS;
	paddle_rect.w = PADDLE_WIDTH;
	SDL_RenderDrawRect(renderer, &paddle_rect);
	SDL_RenderFillRect(renderer, &paddle_rect);
}

void render_opponent(SDL_Renderer *renderer, const game_t *game)
{
	SDL_SetRenderDrawColor(renderer, PADDLE_COLOR.r, PADDLE_COLOR.g, PADDLE_COLOR.b, 255);

	SDL_Rect paddle_rect;
	paddle_rect.x = game->opponent_x_pos - PADDLE_WIDTH / 2;
	paddle_rect.y = PADDLE_OFFSET;
	paddle_rect.h = PADDLE_THICKNESS;
	paddle_rect.w = PADDLE_WIDTH;
	SDL_RenderDrawRect(renderer, &paddle_rect);
	SDL_RenderFillRect(renderer, &paddle_rect);
}

void render_ball(SDL_Renderer *renderer, const game_t *game)
{
	filledCircleRGBA(renderer, game->ball_pos[0], game->ball_pos[1], BALL_RADIUS, BALL_COLOR.r, BALL_COLOR.g, BALL_COLOR.b, 255);
}

void render_halfway_line(SDL_Renderer *renderer, const game_t *game)
{
	SDL_SetRenderDrawColor(renderer, BALL_COLOR.r, BALL_COLOR.g, BALL_COLOR.b, 255);
	SDL_RenderDrawLine(renderer, 0, HEIGHT / 2, WIDTH, HEIGHT / 2);
}

void render_score(SDL_Renderer *renderer, const game_t *game)
{
	char player_score_str[2], opponent_score_str[2];
	sprintf(player_score_str, "%d", game->player_score);
	sprintf(opponent_score_str, "%d", game->opponent_score);
	SDL_Surface *player_score_surface = TTF_RenderText_Solid(font, player_score_str, BALL_COLOR);
	SDL_Texture *player_score_texture = SDL_CreateTextureFromSurface(renderer, player_score_surface);
	SDL_Surface *opponent_score_surface = TTF_RenderText_Solid(font, opponent_score_str, BALL_COLOR);
	SDL_Texture *opponent_score_texture = SDL_CreateTextureFromSurface(renderer, opponent_score_surface);

	SDL_Rect player_score_rect;
	player_score_rect.x = 20;
	player_score_rect.y = HEIGHT / 2 + SCORE_Y_OFFSET;
	player_score_rect.w = SCORE_WIDTH;
	player_score_rect.h = SCORE_HEIGHT;

	SDL_Rect opponent_score_rect;
	opponent_score_rect.x = 20;
	opponent_score_rect.y = HEIGHT / 2 - SCORE_Y_OFFSET - SCORE_HEIGHT;
	opponent_score_rect.w = SCORE_WIDTH;
	opponent_score_rect.h = SCORE_HEIGHT;

	SDL_RenderCopy(renderer, player_score_texture, NULL, &player_score_rect);
	SDL_RenderCopy(renderer, opponent_score_texture, NULL, &opponent_score_rect);
	SDL_FreeSurface(player_score_surface);
	SDL_FreeSurface(opponent_score_surface);
	SDL_DestroyTexture(player_score_texture);
	SDL_DestroyTexture(opponent_score_texture);
}

void render_running_state(SDL_Renderer *renderer, const game_t *game)
{
	render_player(renderer, game);
	render_opponent(renderer, game);
	render_ball(renderer, game);
	render_halfway_line(renderer, game);
	render_score(renderer, game);
}

void render_clear_screen(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 255);
	SDL_RenderClear(renderer);
}

void render_win_screen(SDL_Renderer *renderer, const game_t *game)
{
	render_clear_screen(renderer);

	char win_msg[] = "You Win!";
	char lose_msg[] = "You Lose!";
	SDL_Surface *message_surface;
	if (game->state == PLAYER_WON_STATE)
	{
		message_surface = TTF_RenderText_Solid(font, win_msg, BALL_COLOR);
	}
	else
	{
		message_surface = TTF_RenderText_Solid(font, lose_msg, BALL_COLOR);
	}
	SDL_Texture *message_texture = SDL_CreateTextureFromSurface(renderer, message_surface);

	char score_str[8];
	sprintf(score_str, "%d - %d", game->player_score, game->opponent_score);
	SDL_Surface *score_surface = TTF_RenderText_Solid(font, score_str, BALL_COLOR);
	SDL_Texture *score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);

	SDL_Rect message_rect;
	message_rect.x = WIDTH / 2 - WIN_SCORE_WIDTH / 2;
	message_rect.y = HEIGHT / 2 - WIN_SCORE_HEIGHT;
	message_rect.w = WIN_SCORE_WIDTH;
	message_rect.h = WIN_SCORE_HEIGHT;

	SDL_Rect score_rect;
	score_rect.x = WIDTH / 2 - WIN_SCORE_WIDTH / 2;
	score_rect.y = HEIGHT / 2;
	score_rect.w = WIN_SCORE_WIDTH;
	score_rect.h = WIN_SCORE_HEIGHT;

	SDL_RenderCopy(renderer, message_texture, NULL, &message_rect);
	SDL_RenderCopy(renderer, score_texture, NULL, &score_rect);
	SDL_FreeSurface(message_surface);
	SDL_FreeSurface(score_surface);
	SDL_DestroyTexture(message_texture);
	SDL_DestroyTexture(score_texture);
}

void render_game(SDL_Renderer *renderer, const game_t *game)
{
	switch(game->state)
	{
		case RUNNING_STATE:
			render_running_state(renderer, game);
			break;
		case PLAYER_WON_STATE:
		case PLAYER_LOST_STATE:
			render_win_screen(renderer, game);
			break;
		case QUIT_STATE:
			// game->state = QUIT_STATE;
			break;
	}
}