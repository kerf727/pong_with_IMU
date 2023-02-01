#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "./game.h"
#include "./rendering.h"

const SDL_Color PADDLE_COLOR = {.r = 255, .g = 255, .b = 255};
const SDL_Color BALL_COLOR = {.r = 255, .g = 255, .b = 255};
const SDL_Color BACKGROUND_COLOR = {.r = 0, .g = 0, .b = 0};

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
	// gfxPrimitivesSetFont(gfxPrimativesFontdata, 10, 20);
	TTF_Font *Sans = TTF_OpenFont("Sans.ttf", 24);
	SDL_Color White = {255, 255, 255};
	
	SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Sans, "0", White);
	SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	SDL_Rect Message_rect; // create a rect
	Message_rect.x = 0;	   // controls the rect's x coordinate
	Message_rect.y = 0;	   // controls the rect's y coordinte
	Message_rect.w = 100;  // controls the width of the rect
	Message_rect.h = 100;  // controls the height of the rect

	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}

void render_running_state(SDL_Renderer *renderer, const game_t *game)
{
	render_player(renderer, game);
	render_opponent(renderer, game);
	render_ball(renderer, game);
	render_halfway_line(renderer, game);
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