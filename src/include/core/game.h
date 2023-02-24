#ifndef OFFICE_GAME_H
#define OFFICE_GAME_H

#include <stdint.h>

struct game {
	uint32_t cur_level;
	uint32_t *loaded_level;
};

struct game *game_init(void);
void game_load_resources(const int level);
void game_click(const int level, const int x, const int y);
void game_step(const int level);
void game_clear_screen(const int level);
void game_render(const int level);

#endif
