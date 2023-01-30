#include <stdio.h>
#include <SDL2/SDL.h>
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
		.x_pos = WIDTH / 2
	};

	int x_vel = 0;

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

		SDL_PumpEvents();

		const Uint8 left_pressed = keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A];
		const Uint8 right_pressed = keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D];
		
		x_vel = 0;
		if (left_pressed && !right_pressed)
		{
			x_vel = -SPEED;
		}
		else if (!left_pressed && right_pressed)
		{
			x_vel = SPEED;
		}

		game.x_pos += x_vel / 60;

		if (game.x_pos - PADDLE_WIDTH / 2 <= 0)
			game.x_pos = PADDLE_WIDTH / 2;
		if (game.x_pos >= WIDTH - PADDLE_WIDTH / 2)
			game.x_pos = WIDTH - PADDLE_WIDTH / 2;

		// Clear the window to black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw the image to the window
		// SDL_RenderCopy(renderer, ?, NULL, NULL);
		render_game(renderer, &game);
		SDL_RenderPresent(renderer);

		// Wait 1/60th of a second
		SDL_Delay(1000 / 60);
	}

	// SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}