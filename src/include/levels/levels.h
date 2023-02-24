#ifndef OFFICE_LEVELS_H
#define OFFICE_LEVELS_H

#include <core/game.h>

enum {
	LEVEL_LOGO,
	LEVEL_OFFICE_0x0,
	N_LEVEL
};

/*
 * level logo - show logo xverizex
 */
void level_logo_init(struct game *_game);
void level_logo_load_resources(void);
void level_logo_clear(void);
void level_logo_step(void);
void level_logo_click(int x, int y);
void level_logo_render();

void level_office_0x0_init(struct game *_game);
void level_office_0x0_load_resources(void);
void level_office_0x0_clear(void);
void level_office_0x0_step(void);
void level_office_0x0_click(int x, int y);
void level_office_0x0_render();

#endif
