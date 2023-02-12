#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <time.h>

#include "./game.h"
#include "./logic.h"
#include "./rendering.h"

// #define USE_IMU_AS_CONTROLLER

#ifdef USE_IMU_AS_CONTROLLER

#include <time.h>
#include "./serial_port.h"
#define UART_READ_INTERVAL_MS	100
#define BUFFER_LENGTH			16
#define NUM_DATA_VALUES 1
// int num_data_values = 1;

#endif // USE_IMU_AS_CONTROLLER

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (TTF_Init() != 0)
	{
		printf("TTF_Init Error: %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}
	TTF_Init_font();

#ifdef USE_IMU_AS_CONTROLLER
	// COM ports higher than COM9 need \\.\ prefix,
	// which is written as "\\\\.\\" in C to escape the backslashes
	const char *device = "\\\\.\\COM10";
	uint32_t baud_rate = 115200;
	HANDLE port = open_serial_port(device, baud_rate);
	if (port == INVALID_HANDLE_VALUE)
	{
		printf("Failed to open serial port.\n");
		return 1;
	}
	printf("Serial port opened.\n");

	int msec = 0;
	clock_t last_time = clock();
	clock_t start_time, end_time;

	char rx_buf[BUFFER_LENGTH];
	char *saveptr;
	char *token;
#endif // USE_IMU_AS_CONTROLLER

	SDL_Window *window = SDL_CreateWindow("Pong with IMU Controller", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

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

	game_t game;
	time_t restart_time = clock();

	game.state = START_SCREEN_STATE;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	SDL_Event event;
	while (game.state != QUIT_STATE)
	{
		// start_time = clock();

		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				game.state = QUIT_STATE;
				break;
			}
		}

#ifdef USE_IMU_AS_CONTROLLER
		float data[NUM_DATA_VALUES];
		char *delim;
		if (NUM_DATA_VALUES == 1)
		{
			delim = "\n";
		}
		else
		{
			delim = ",";
		}
		if (clock() - last_time > UART_READ_INTERVAL_MS)
			{
			SSIZE_T received = read_port(port, rx_buf, BUFFER_LENGTH);

			/* Remove first line of data to ensure an entire line is read in */
			token = strtok_r(rx_buf, "\n", &saveptr);

			/* Separate by delim to get pitch and roll data */
			for (int i = 0; i < NUM_DATA_VALUES; i++)
			{
				token = strtok_r(NULL, delim, &saveptr);
				data[i] = atof(token);
			}

			last_time = clock();
		}

		float roll = data[0];

		// Update player velocity
		game.player_x_vel = (int) PLAYER_SPEED * roll / MAX_ROLL_ANGLE;

		// Clamp velocity
		if (game.player_x_vel > PLAYER_SPEED)
			game.player_x_vel = PLAYER_SPEED;
		if (game.player_x_vel < -PLAYER_SPEED)
			game.player_x_vel = -PLAYER_SPEED;
#else
		// Update the keyboard state
		SDL_PumpEvents();
		const Uint8 left_pressed = keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A];
		const Uint8 right_pressed = keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D];

		// Update player velocity
		game.player_x_vel = 0;
		if (left_pressed && !right_pressed)
		{
			game.player_x_vel = -PLAYER_SPEED;
		}
		else if (!left_pressed && right_pressed)
		{
			game.player_x_vel = PLAYER_SPEED;
		}
#endif // USE_IMU_AS_CONTROLLER

		// Update the game state
		if (game.state == START_SCREEN_STATE)
		{
			if (clock() - restart_time < GAME_START_TIME_MS)
			{
				game.countdown_s = (int)(GAME_START_TIME_MS - (clock() - restart_time)) / 1000 + 1;
			}
			else
			{
				game.state = RUNNING_STATE;
				initialize_game_state(&game, DIFFICULTY_LEVEL);
			}
		}
		else if (game.state == RUNNING_STATE)
		{
			update_game_state(&game);
		}
		else if (game.state == GAME_OVER_STATE)
		{
			// Set restart time to compare against in RESET_STATE case
			restart_time = clock();
			game.state = RESET_STATE;
		}
		else if (game.state == RESET_STATE)
		{
			if (clock() - restart_time < RESET_WAIT_TIME_MS)
			{
				game.countdown_s = (int)(RESET_WAIT_TIME_MS - (clock() - restart_time)) / 1000 + 1;
			}
			else
			{
				game.state = RUNNING_STATE;
				initialize_game_state(&game, DIFFICULTY_LEVEL);
			}
		}

		// Clear the window
		render_clear_screen(renderer);

		// Draw the image to the window
		render_game(renderer, &game);
		SDL_RenderPresent(renderer);

		// end_time = clock();
		// printf("frame time: %lu\n", end_time - start_time);

		// Wait 1/60th of a second
		SDL_Delay(1000 / FPS);
	}

	// SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

#ifdef USE_IMU_AS_CONTROLLER
	CloseHandle(port);
#endif // USE_IMU_AS_CONTROLLER

	return EXIT_SUCCESS;
}