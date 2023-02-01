#ifndef RENDERING_H_
#define RENDERING_H_

// declared as static to keep it internal to the rendering translation unit
static TTF_Font *font;

void TTF_Init_font(void);

void render_clear_screen(SDL_Renderer *renderer);

void render_game(SDL_Renderer *renderer, const game_t *game);

#endif // !RENDERING_H_