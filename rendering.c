#include <SDL2/SDL.h>

#include "./game.h"
#include "./rendering.h"

const SDL_Color PADDLE_COLOR = {.r = 255, .g = 255, .b = 255};
const SDL_Color BALL_COLOR = {.r = 255, .g = 255, .b = 255};

void render_player(SDL_Renderer *renderer, const game_t *game)
{
	SDL_SetRenderDrawColor(renderer, PADDLE_COLOR.r, PADDLE_COLOR.g, PADDLE_COLOR.b, 255);

	SDL_Rect paddle_rect;
	paddle_rect.x = game->player_x_pos - PADDLE_WIDTH / 2;
	paddle_rect.y = HEIGHT - 50;
	paddle_rect.h = 10;
	paddle_rect.w = PADDLE_WIDTH;
	SDL_RenderDrawRect(renderer, &paddle_rect);
	SDL_RenderFillRect(renderer, &paddle_rect);
}

void render_opponent(SDL_Renderer *renderer, const game_t *game)
{
}

void render_ball(SDL_Renderer *renderer, const game_t *game)
{
}

void render_score(SDL_Renderer *renderer, const game_t *game)
{
}

void render_running_state(SDL_Renderer *renderer, const game_t *game)
{
	render_player(renderer, game);
	render_opponent(renderer, game);
	render_ball(renderer, game);
	render_score(renderer, game);
}

void render_game(SDL_Renderer *renderer, const game_t *game)
{
	switch(game->state)
	{
		case RUNNING_STATE:
			render_running_state(renderer, game);
			break;
		case PLAYER_WON_STATE:
			break;
		case PLAYER_LOST_STATE:
			break;
		case QUIT_STATE:
			break;
	}
}