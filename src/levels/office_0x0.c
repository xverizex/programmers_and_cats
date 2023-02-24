#include <core/game.h>
#include <core/sprite.h>
#include <textures/list.h>
#include <GLES3/gl3.h>
#include <shaders/list.h>
#include <core/animation.h>
#include <game/human.h>
#include <game/floor.h>
#include <game/wall.h>
#include <game/board.h>
#include <game/fired.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <common/coord.h>
#include <core/cmath.h>
#include <core/rect.h>
#include <game/productivity.h>
#include <game/game_over.h>

static struct game *game;

#define SIZE_BOARDS 8
#define SIZE_HUMANS 8
static struct human *humans[SIZE_HUMANS];
static struct computer_board *boards[SIZE_BOARDS];

extern uint32_t screen_width;
extern uint32_t screen_height;

#define SIZE_FLOOR_X    8
#define SIZE_FLOOR_Y    9
static struct floor ***floor;
#ifdef WEBGL
static uint32_t cur_scale = 3;  //4
#else
static uint32_t cur_scale = 3;
#endif
static uint32_t shiftx = 2;
static uint32_t shifty = 3;

static struct wall *wall_h[SIZE_FLOOR_X];
static struct wall *wall_v[SIZE_FLOOR_Y];
static uint32_t shiftx_wall = 64;
static uint32_t shifty_wall = 29;
static uint32_t shifty_human = 20;

struct times *tm;

static uint32_t speed_time = 100;

static struct fired *fired;

static struct productivity *prod;

static struct game_over *game_over;

static uint32_t is_end = 0;

static struct times *seconds_timer;
static uint32_t sec;

void 
level_office_0x0_init (struct game *_game)
{
	game = _game;
	tm = timer_init (1);
}

void
level_office_0x0_clear ()
{
	glClearColor (0x30 / 255.f, 0x90 / 255.f, 0xa5 / 255.f, 1.f);
	glClear (GL_COLOR_BUFFER_BIT);
}

void
level_office_0x0_step ()
{
	if (is_end) return;

	int percent = 0;
	int count = 0;
	for (int i = 0; i < SIZE_HUMANS; i++) {
		struct human *h = humans[i];
		human_step (h, SIZE_FLOOR_Y, SIZE_FLOOR_X, floor);
		if ((h->tilex == h->dstx) && (h->tiley == h->dsty)) {
			if (h->com_board) {
				h->com_board->is_working = 1;
				//h->is_visible = 0;
				board_working (h->com_board, h);
				if (h->is_working == 0) 
					h->is_working = 1;
				if (timer_run (h->cats, 0, h->random_cats)) {
					h->need_to_watch_cats = ~h->need_to_watch_cats;
					h->random_cats = (rand() % 20 + 1) * 1000;
				}
				h->com_board->sprite->cur_tex = anim_run (h->com_board->anim, h->need_to_watch_cats ? 1: 0, 0);

				if (h->need_to_watch_cats)
					percent--;
				
				count++;
			}
		}
	}

	if (count >= 2 && percent == 0) {
		percent = 2;
	}

	if (timer_run (prod->tm, 0, 500)) {
		if (percent >= 0) {
			productivity_set (prod, prod->percent + percent);
		} else {
			productivity_set (prod, prod->percent - 1);
		}
	}

	if (prod->percent <= 0) {
		is_end = 1;
		game_build_text (game_over, sec);
	}

	if (timer_run (seconds_timer, 0, 1000)) {
		sec++;
	}
	
	
#if 0
	if (timer_run (tm, 0, 10000)) {
			for (int x = 0; x < SIZE_FLOOR_X; x++) {
						for (int y = 0; y < SIZE_FLOOR_Y; y++) {

				printf ("%d", floor[y][x]->com_board? 1: floor[y][x]->human? 1: 0);
			}
			printf ("\n");
		}

		printf ("--------------------------\n");
	}
#endif

}

static void
wall_init_level (int startx, int starty)
{
	int x = startx + floor[0][0]->sprite->tex[0].w * cur_scale;
	int y = starty + shifty_wall * cur_scale;

	float w;
	float h;

	for (int xx = 0; xx < SIZE_FLOOR_X; xx++) {
		wall_h[xx] = wall_init ();
		struct wall *wa = wall_h[xx];
		wall_scale (wa, cur_scale);

		wa->sprite->cur_tex = 0;

		w = wa->sprite->tex->w * cur_scale / 2.f;
		h = wa->sprite->tex->h * cur_scale / 2.f;

		sprite_set_pos (wa->sprite, x, y, 0.f);

		x += w;
		y -= 16 * cur_scale;
		
	}

	wall_h[1]->sprite->cur_tex = 2;

	x = startx;
	y = starty + shifty_wall * cur_scale;

	for (int yy = 0; yy < SIZE_FLOOR_Y; yy++) {
		wall_v[yy] = wall_init ();
		struct wall *wa = wall_v[yy];
		wall_scale (wa, cur_scale);

		wa->sprite->cur_tex = 1;

		w = wa->sprite->tex->w * cur_scale / 2.f;
		h = wa->sprite->tex->h * cur_scale / 2.f;

		sprite_set_pos (wa->sprite, x, y, 0.f);
		x -= w;
		y -= 16 * cur_scale;
	}
	
}

static void
floor_init_level (int startx, int starty)
{
	int x = startx;
	int y = starty;

	float w;
	float h;

	floor = malloc (sizeof (floor) * SIZE_FLOOR_Y);

	for (int yy = 0; yy < SIZE_FLOOR_Y; yy++) {
			floor[yy] = malloc (sizeof (struct floor) * SIZE_FLOOR_X);
	}

	for (int yy = 0; yy < SIZE_FLOOR_Y; yy++) {
		for (int xx = 0; xx < SIZE_FLOOR_X; xx++) {
			floor[yy][xx] = floor_init (yy, xx);
			struct floor *f = floor[yy][xx];
			floor_scale (f, cur_scale);
		
			w = f->sprite->tex->w * cur_scale / 2.f;
			h = f->sprite->tex->h * cur_scale / 2.f;

			x += w;
			y -= h - shifty * cur_scale;

			sprite_set_pivot (f->sprite, 0, 0);//shifty);
			sprite_set_pos (f->sprite, x, y, 0.f);
		}

			startx -= w;
			starty -= h - shifty * cur_scale;
			x = startx;
			y = starty;
	}
}

static void
com_boards_init_level (int startx, int starty)
{
	struct bo {
		int x;
		int y;
		int type;
		int pivotx;
		int pivoty;
	};
	const struct bo bo[SIZE_BOARDS] = {
		{3, 0, FROM_LEFT_DOWN_TO_RIGHT_UP, 16, shifty + cur_scale},
		{6, 0, FROM_LEFT_DOWN_TO_RIGHT_UP, 16, shifty + cur_scale},
		{3, 6, FROM_LEFT_DOWN_TO_RIGHT_UP, 16, shifty + cur_scale},
		{6, 6, FROM_LEFT_DOWN_TO_RIGHT_UP, 16, shifty + cur_scale},
		{3, 3, FROM_LEFT_DOWN_TO_RIGHT_UP, 16, shifty + cur_scale},
		{6, 3, FROM_LEFT_DOWN_TO_RIGHT_UP, 16, shifty + cur_scale},
		{0, 2, FROM_LEFT_UP_TO_RIGHT_DOWN, -(16), shifty + cur_scale},
		{0, 5, FROM_LEFT_UP_TO_RIGHT_DOWN, -(16), shifty + cur_scale}
	};

	for (int i = 0; i < SIZE_BOARDS; i++) {
		boards[i] = computer_board_init (bo[i].type, bo[i].x, bo[i].y, SIZE_FLOOR_Y, SIZE_FLOOR_X, floor);
		board_set_scale (boards[i], cur_scale);
		sprite_set_pivot (boards[i]->sprite, bo[i].pivotx, bo[i].pivoty);
		sprite_set_pos (boards[i]->sprite, floor[bo[i].y][bo[i].x]->sprite->x, floor[bo[i].y][bo[i].x]->sprite->y, 0.f);
	}
	
}

static void
humans_init_level ()
{
	srand (time(0));

	path_init (SIZE_HUMANS);


	for (int i = 0; i < SIZE_HUMANS; i++) {
		humans[i] = human_init (&speed_time);
		int x = 1;
		int y = 0;
		int xx = floor[y][x]->sprite->x;
		int yy = floor[y][x]->sprite->y;
		struct human *h = humans[i];
		h->oldtilex = x;
		h->oldtiley = y;
		h->tilex = x;
		h->tiley = y;
		if (floor[0][1]->human == NULL) {
			floor_assign (floor[0][1], h);
			h->is_visible = 1;
		} else {
			h->is_visible = 0;
		}
			
		//human_set_path (h, path_get(0));
		human_set_scale (h, cur_scale);
		sprite_set_pivot (h->sprite, 0.f, shifty_human);
		sprite_set_pos (h->sprite, xx, yy, 0.f);
		int free_board = -1;
		for (int m = 0; m < SIZE_BOARDS; m++) {
			struct computer_board *b = boards[m];
			if (b->is_free) {
				free_board = m;
				b->is_free = 0;
				break;
			}
		}
		
		h->a_star = a_star_init (SIZE_FLOOR_X, SIZE_FLOOR_Y, floor);
		struct computer_board *b = boards[free_board];
		h->path = a_star_get_path (h->a_star, i, h->tilex, h->tiley, b->tilex, b->tiley);
		h->num_path = i;
		h->dstx = b->tilex;
		h->dsty = b->tiley;
		h->com_board = b;
		h->floor = (struct floor ***) floor;
	}
}

void
level_office_0x0_load_resources ()
{	
	init_text ();

	seconds_timer = timer_init (1);

	game_over = game_over_init ();
	game_over_set_pos (game_over, screen_width / 2 - game_over->background->width / 2, screen_height / 2 - game_over->background->height / 2);


	int startx = -(screen_width / 2);
	int starty = (screen_height / 2);

	fired = fired_init ();
	fired_scale (fired, cur_scale);
	fired->sprite->cur_tex = 13;

	prod = productivity_init ();
	productivity_set_pos (prod, screen_width / 2 - prod->background->width / 2, screen_height - prod->background->height - 20);
	productivity_set (prod, 60);

	floor_init_level (startx, starty);
	wall_init_level (startx, starty);
	com_boards_init_level (startx, starty);
	humans_init_level ();

}

void
level_office_0x0_click (int x, int y)
{
	int xx, yy;
	//coord_get_real (x, y, &xx, &yy);

	y = screen_height - y;

	for (int i = 0; i < SIZE_HUMANS; i++) {
		struct human *h = humans[i];
		float *cam = get_cam ();

		int hx = cam[12] - h->sprite->x;
		int hy = cam[13] + h->sprite->y;

		uint32_t width = h->sprite->tex->width_orig * h->sprite->scale;
		int32_t posx = hx - width / 2;
		uint32_t height = h->sprite->tex->height_orig * h->sprite->scale;
		int32_t posy = hy;
#if 0
		printf ("%d %d: posx: %d; width: %d; posy: %d; height: %d; human: %d\n",
			x, y,
			posx, width,
			posy, height,
			i
			);
#endif

#if 1
		if (h->is_working == 1) {
			if ((x >= posx) && (x <= (posx + width))) {
				if ((y >= posy) && (y <= (posy + height))) {
					if (h->need_to_watch_cats) {
						//productivity_set (prod, prod->percent + 7);
					}
					else {
						productivity_set (prod, prod->percent - 7);
					}

					sprite_set_pivot (fired->sprite, 0, 4 * h->scale);
					sprite_set_pos (fired->sprite, h->sprite->x, h->sprite->y, 0.f);
					fired->is_play = 1;
					board_unsit (h);
					h->is_working = 2;
					int dx = floor[0][1]->sprite->x;
					int dy = floor[0][1]->sprite->y;
					int free_board = -1;
					h->com_board->is_free = 1;
					board_set_still (h->com_board);
					h->com_board = NULL;
					for (int m = 0; m < SIZE_BOARDS; m++) {
						struct computer_board *b = boards[m];
						if (b->is_free) {
							free_board = m;
							b->is_free = 0;
							break;
						}
					}
					h->tilex = 1;
					h->tiley = 0;
					h->is_assigned = 0;
					sprite_set_pos (h->sprite, dx, dy, 0.f);
					struct computer_board *b = boards[free_board];
					h->path = a_star_get_path (h->a_star, i, h->tilex, h->tiley, b->tilex, b->tiley);
					h->num_path = i;
					h->dstx = b->tilex;
					h->dsty = b->tiley;
					h->com_board = b;
					h->is_working = 0;
					return;
				}
			}
		}
		
	}
#endif
}

void
level_office_0x0_render ()
{
	for (int x = 0; x < SIZE_FLOOR_X; x++) {
		wall_render (wall_h[x]);
	}
	for (int y = 0; y < SIZE_FLOOR_Y; y++) {
		wall_render (wall_v[y]);
	}

	for (int y = 0; y < SIZE_FLOOR_Y; y++) {
		for (int x = 0; x < SIZE_FLOOR_X; x++) {
			floor_render (floor[y][x], 0);
			if (floor[y][x]->com_board) {
				for (int i = 0; i < 2; i++) {
					int xx = floor[y][x]->com_board->ptile[i].x;
					int yy = floor[y][x]->com_board->ptile[i].y;
					floor_render (floor[yy][xx], 1);
				}
			}				
		}
	}

	fired_render (fired);

	productivity_render (prod);

	if (is_end)
		game_over_render (game_over);
}
