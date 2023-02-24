#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <core/game.h>
#include <levels/levels.h>

struct game *
game_init ()
{
	struct game *game = malloc (sizeof (struct game));

	/*
	 * initialization params
	 */
	game->cur_level = LEVEL_OFFICE_0x0;
	game->loaded_level = calloc (N_LEVEL, sizeof (uint32_t));

	/*
	 * init levels
	 */
	level_logo_init (game);
	level_office_0x0_init (game);

	return game;
}

void
game_load_resources (const int level)
{
	switch (level) {
		case LEVEL_LOGO:
			level_logo_load_resources ();
			break;
		case LEVEL_OFFICE_0x0:
			level_office_0x0_load_resources ();
			break;
	}
}

void
game_click (const int level, const int x, const int y)
{
	switch (level) {
		case LEVEL_LOGO:
			level_logo_click (x, y);
			break;
		case LEVEL_OFFICE_0x0:
			level_office_0x0_click (x, y);
			break;
	}
}

void
game_step (const int level)
{
	switch (level) {
		case LEVEL_LOGO:
			level_logo_step ();
			break;
		case LEVEL_OFFICE_0x0:
			level_office_0x0_step ();
			break;
	}
}

void
game_clear_screen (const int level)
{
	switch (level) {
		case LEVEL_LOGO:
			level_logo_clear ();
			break;
		case LEVEL_OFFICE_0x0:
			level_office_0x0_clear ();
			break;
	}
}

void
game_render (const int level)
{
	switch (level) {
		case LEVEL_LOGO:
			level_logo_render ();
			break;
		case LEVEL_OFFICE_0x0:
			level_office_0x0_render ();
			break;
	}
}
