#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

#include "./game.h"
#include "./logic.h"
#include "./rendering.h"

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window *window = SDL_CreateWindow("Hello SDL World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

	if (window == NULL)
	{
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	game_t game = {
		.state = RUNNING_STATE,
		.player_x_pos = WIDTH / 2,
		.player_x_vel = 0,
		.opponent_x_pos = WIDTH / 2,
		.ball_pos = {WIDTH / 2, HEIGHT / 2},
		.ball_vel = {BALL_SPEED / 2, BALL_SPEED},
		.player_score = 0,
		.opponent_score = 0
	};

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	SDL_Event event;
	while (game.state != QUIT_STATE)
	{
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				game.state = QUIT_STATE;
				break;
			}
		}

		// Update the keyboard state
		SDL_PumpEvents();

		const Uint8 left_pressed = keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A];
		const Uint8 right_pressed = keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D];
		
		// Update the player velocity
		game.player_x_vel = 0;
		if (left_pressed && !right_pressed)
		{
			game.player_x_vel = -PLAYER_SPEED;
		}
		else if (!left_pressed && right_pressed)
		{
			game.player_x_vel = PLAYER_SPEED;
		}

		// Update the game state
		update_game_state(&game);

		// Clear the window
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw the image to the window
		render_game(renderer, &game);
		SDL_RenderPresent(renderer);

		// Wait 1/60th of a second
		SDL_Delay(1000 / FPS);
	}

	// SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}