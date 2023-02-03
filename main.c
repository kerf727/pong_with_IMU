#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

#include "./game.h"
#include "./logic.h"
#include "./rendering.h"

// #define USE_IMU_AS_CONTROLLER

#ifdef USE_IMU_AS_CONTROLLER

#include <time.h>
#include "./serial_port.h"
#define UART_READ_INTERVAL_MS	100
#define BUFFER_LENGTH			64
#define NUM_DATA_VALUES			3

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
	const char *device = "\\\\.\\COM7";
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

		// TODO: implement serial port read
#ifdef USE_IMU_AS_CONTROLLER
		float data[3];
		if (clock() - last_time > UART_READ_INTERVAL_MS)
			{
			SSIZE_T received = read_port(port, rx_buf, BUFFER_LENGTH);
			printf("****\n");
			printf("%s\n\n", rx_buf);

			/* Remove first line of data to ensure an entire line is read in */
			token = strtok_r(rx_buf, "\n", &saveptr);

			/* Separate by commas to get pitch and roll data */
			// token = strtok_r(NULL, ",", &saveptr);
			for (int i = 0; i < NUM_DATA_VALUES; i++)
			{
				token = strtok_r(NULL, ",", &saveptr);
				data[i] = atof(token);
				// printf("token: %s\n", token);
			}

			printf("data: [%f, %f, %f]\n", data[0], data[1], data[2]);

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
				if (game.state == RUNNING_STATE)
		{
			update_game_state(&game);
		}

		// Clear the window
		render_clear_screen(renderer);

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

#ifdef USE_IMU_AS_CONTROLLER
	CloseHandle(port);
#endif // USE_IMU_AS_CONTROLLER

	return EXIT_SUCCESS;
}