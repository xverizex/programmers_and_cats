#include <game/human.h>
#include <shaders/list.h>
#include <textures/list.h>
#include <game/floor.h>
#include <stdlib.h>
#include <stdio.h>

enum ANIMATIONS {
	WALK_RIGHT_DOWN,
	STAY_RIGHT_DOWN_AND_BLINK,
	WALK_LEFT_DOWN,
	STAY_LEFT_DOWN_AND_BLINK,
	WALK_LEFT_UP,
	WALK_RIGHT_UP,
	WALK_DOWN,
	WALK_UP,
	WALK_LEFT,
	WALK_RIGHT,
	N_ANIMATIONS
};

#define SPEED_ANIMATION         300

struct human *
human_init (int *speed_time)
{
	struct human *h = malloc (sizeof (struct human));
	h->sprite = sprite_init (TEXTURE_MAN_1, COORD_SYSTEM_INT);
	h->sprite->shader = simple_2d_shader_render;

	sprite_move_init (h->sprite);
	
	//sprite_set_pos (h->sprite, -300, 700, 0.f);
	//sprite_set_size (h->sprite, 0.03f);

	h->animation = anim_init (N_ANIMATIONS);
	anim_set (h->animation, WALK_RIGHT_DOWN, 0, 3, *speed_time);
	anim_set (h->animation, STAY_RIGHT_DOWN_AND_BLINK, 4, 5, *speed_time);
	anim_set (h->animation, WALK_LEFT_DOWN, 6, 9, *speed_time);
	anim_set (h->animation, STAY_LEFT_DOWN_AND_BLINK, 10, 11, *speed_time);
	anim_set (h->animation, WALK_LEFT_UP, 12, 15, *speed_time);
	anim_set (h->animation, WALK_RIGHT_UP, 16, 19, *speed_time);
	anim_set (h->animation, WALK_DOWN, 20, 23, *speed_time);
	anim_set (h->animation, WALK_UP, 24, 27, *speed_time);
	anim_set (h->animation, WALK_LEFT, 28, 31, *speed_time);
	anim_set (h->animation, WALK_RIGHT, 32, 35, *speed_time);

	h->movex = 6;
	h->movey = -3;
	h->scale = 1;
	h->path = NULL;
	h->index_path = 0;
	h->need_to_clear_a_star = 0;
	h->is_assigned = 0;
	h->com_board = NULL;
	h->speed_time = speed_time;
	h->need_to_watch_cats = 0;
	h->is_working = 0;
	h->is_assigned = 0;

	h->not_found_path = timer_init (1);
	h->cats = timer_init (1);

	h->random_cats = rand() % 20 * 1000;

	return h;
}

void
human_set_scale (struct human *h, float sc)
{
	h->scale = sc;
	sprite_scale (h->sprite, sc);
}

void 
human_set_path (struct human *h, struct path *p)
{
	h->index_path = 0;
	h->path = p;
}

void 
human_destination (struct human *h, int oldx, int oldy, int dx, int dy)
{
	h->tilex = oldx;
	h->tiley = oldy;
	h->dstx = dx;
	h->dsty = dy;
}

static void
human_walk_left_down (struct human *h, struct floor *f, int indx, struct floor *oldfloor)
{
	sprite_move (h->sprite, -(h->movex * h->scale), h->movey * h->scale, 0.0f, *(h->speed_time));
	h->sprite->cur_tex = anim_run (h->animation, WALK_LEFT_DOWN, 0);
			
						
	if (h->sprite->x >= f->sprite->x && h->sprite->y <= f->sprite->y) {
		sprite_set_pos (h->sprite, f->sprite->x, f->sprite->y, 0.f);
		h->index_path++;
		int indx = h->index_path;
		int yy = h->path->coord[indx].y;
		int xx = h->path->coord[indx].x;
		if (oldfloor->human == h) {
			oldfloor->human = NULL;
		}
		h->tilex = h->path->coord[indx - 1].x;
		h->tiley = h->path->coord[indx - 1].y;
		h->oldtilex = h->tilex;
		h->oldtiley = h->tiley;

		h->path = NULL;
		h->index_path = 0;

		f->human = h;

	}

}

void
human_walk_right_down (struct human *h, struct floor *f, int indx, struct floor *oldfloor)
{
	sprite_move (h->sprite, h->movex * h->scale, h->movey * h->scale, 0.0f, *(h->speed_time));
	h->sprite->cur_tex = anim_run (h->animation, WALK_RIGHT_DOWN, 0);

			
	if (h->sprite->x <= f->sprite->x && h->sprite->y <= f->sprite->y) {
		sprite_set_pos (h->sprite, f->sprite->x, f->sprite->y, 0.f);
		h->index_path++;
		int indx = h->index_path;
		int yy = h->path->coord[indx].y;
		int xx = h->path->coord[indx].x;
		if (oldfloor->human == h) {
			oldfloor->human = NULL;
		}
		h->tilex = h->path->coord[indx - 1].x;
		h->tiley = h->path->coord[indx - 1].y;
		h->oldtilex = h->tilex;
		h->oldtiley = h->tiley;

		h->path = NULL;
		h->index_path = 0;

		f->human = h;

	}

}

void
human_walk_down (struct human *h, struct floor *f, int indx, struct floor *oldfloor)
{
	sprite_move (h->sprite, 0.f, h->movey * h->scale, 0.0f, *(h->speed_time));
	h->sprite->cur_tex = anim_run (h->animation, WALK_DOWN, 0);
			
	if (h->sprite->y <= f->sprite->y) {
		sprite_set_pos (h->sprite, f->sprite->x, f->sprite->y, 0.f);
		h->index_path++;
		int indx = h->index_path;
		int yy = h->path->coord[indx].y;
		int xx = h->path->coord[indx].x;
		if (oldfloor->human == h) {
			oldfloor->human = NULL;
			
		}
		h->tilex = h->path->coord[indx - 1].x;
		h->tiley = h->path->coord[indx - 1].y;
		h->oldtilex = h->tilex;
		h->oldtiley = h->tiley;


		h->path = NULL;
		h->index_path = 0;

		f->human = h;

	}
}

void
human_walk_up (struct human *h, struct floor *f, int indx, struct floor *oldfloor)
{
	sprite_move (h->sprite, 0.f, -(h->movey * h->scale), 0.0f, *(h->speed_time));
	h->sprite->cur_tex = anim_run (h->animation, WALK_UP, 0);

	if (h->sprite->y >= f->sprite->y) {
		sprite_set_pos (h->sprite, f->sprite->x, f->sprite->y, 0.f);
		h->index_path++;
		int indx = h->index_path;
		int yy = h->path->coord[indx].y;
		int xx = h->path->coord[indx].x;
		if (oldfloor->human == h) {
			oldfloor->human = NULL;
		}
		h->tilex = h->path->coord[indx - 1].x;
		h->tiley = h->path->coord[indx - 1].y;
		h->oldtilex = h->tilex;
		h->oldtiley = h->tiley;

		h->path = NULL;
		h->index_path = 0;

		f->human = h;

	}
}

void
human_walk_left_up (struct human *h, struct floor *f, int indx, struct floor *oldfloor)
{
	sprite_move (h->sprite, -(h->movex * h->scale), -(h->movey * h->scale), 0.0f, *(h->speed_time));
	h->sprite->cur_tex = anim_run (h->animation, WALK_LEFT_UP, 0);

	if (h->sprite->y >= f->sprite->y) {
		sprite_set_pos (h->sprite, f->sprite->x, f->sprite->y, 0.f);
		h->index_path++;
		int indx = h->index_path;
		int yy = h->path->coord[indx].y;
		int xx = h->path->coord[indx].x;
		if (oldfloor->human == h) {
			oldfloor->human = NULL;
		}
		h->tilex = h->path->coord[indx - 1].x;
		h->tiley = h->path->coord[indx - 1].y;
		h->oldtilex = h->tilex;
		h->oldtiley = h->tiley;

		h->path = NULL;
		h->index_path = 0;

		f->human = h;

	}

}

void
human_walk_right_up (struct human *h, struct floor *f, int indx, struct floor *oldfloor)
{
	sprite_move (h->sprite, (h->movex * h->scale), -(h->movey * h->scale), 0.0f, *(h->speed_time));
	h->sprite->cur_tex = anim_run (h->animation, WALK_RIGHT_UP, 0);

	if (h->sprite->y >= f->sprite->y) {
		sprite_set_pos (h->sprite, f->sprite->x, f->sprite->y, 0.f);
		h->index_path++;
		int indx = h->index_path;
		int yy = h->path->coord[indx].y;
		int xx = h->path->coord[indx].x;
		if (oldfloor->human == h) {
			oldfloor->human = NULL;
		}
		h->tilex = h->path->coord[indx - 1].x;
		h->tiley = h->path->coord[indx - 1].y;
		h->oldtilex = h->tilex;
		h->oldtiley = h->tiley;

		h->path = NULL;
		h->index_path = 0;

		f->human = h;

	}
}

void
human_walk_left (struct human *h, struct floor *f, int indx, struct floor *oldfloor)
{
	sprite_move (h->sprite, -(h->movex * h->scale), 0.f, 0.0f, *(h->speed_time));
	h->sprite->cur_tex = anim_run (h->animation, WALK_LEFT, 0);

	if (h->sprite->x >= f->sprite->x) {
		sprite_set_pos (h->sprite, f->sprite->x, f->sprite->y, 0.f);
		h->index_path++;
		int indx = h->index_path;
		int yy = h->path->coord[indx].y;
		int xx = h->path->coord[indx].x;
		if (oldfloor->human == h) {
			oldfloor->human = NULL;
		}
		h->tilex = h->path->coord[indx - 1].x;
		h->tiley = h->path->coord[indx - 1].y;
		h->oldtilex = h->tilex;
		h->oldtiley = h->tiley;

		h->path = NULL;
		h->index_path = 0;

		f->human = h;

	}
}

void
human_walk_right (struct human *h, struct floor *f, int indx, struct floor *oldfloor)
{
	sprite_move (h->sprite, (h->movex * h->scale), 0.f, 0.0f, *(h->speed_time));
	h->sprite->cur_tex = anim_run (h->animation, WALK_RIGHT, 0);

	if (h->sprite->x <= f->sprite->x) {
		sprite_set_pos (h->sprite, f->sprite->x, f->sprite->y, 0.f);
		h->index_path++;
		int indx = h->index_path;
		int yy = h->path->coord[indx].y;
		int xx = h->path->coord[indx].x;
		if (oldfloor->human == h) {
			oldfloor->human = NULL;
		}
		h->tilex = h->path->coord[indx - 1].x;
		h->tiley = h->path->coord[indx - 1].y;
		h->oldtilex = h->tilex;
		h->oldtiley = h->tiley;
		
		h->path = NULL;
		h->index_path = 0;

		f->human = h;

	}
}


void
human_step (struct human *h, int sizeh, int sizew, struct floor ***floor)
{
	if (h->is_assigned == 0 && floor[0][1]->human == NULL) {
		floor_assign (floor[0][1], h);
		h->is_visible = 1;
	}


	if (h->path == NULL) {
		if (h->tilex == h->dstx && h->tiley == h->dsty) return;
		a_star_clear (h->a_star, sizew, sizeh, floor, h->tilex, h->tiley);
		path_clear (h->num_path);
		h->path = a_star_get_path (h->a_star, h->num_path, h->tilex, h->tiley, h->dstx, h->dsty);

	}

	if (h->path && h->is_visible) {
		int indx = h->index_path;
		int yy = h->path->coord[indx].y;
		int xx = h->path->coord[indx].x;
		struct floor *f = floor[yy][xx];
		
		if (f->human && f->human != h) {
			a_star_clear (h->a_star, sizew, sizeh, floor, h->tilex, h->tiley);
			path_clear (h->num_path);
			h->path = a_star_get_path (h->a_star, h->num_path, h->tilex, h->tiley, h->dstx, h->dsty);
			return;
		}

		
		
		floor_assign (f, h);

		//human_destination (h, h->oldtilex, h->oldtiley, xx, yy);
		/* unassign tile human */


		if (h->sprite->x < f->sprite->x && h->sprite->y > f->sprite->y) {
			human_walk_left_down (h, f, indx, floor[h->oldtiley][h->oldtilex]);
			return;
		}
		
		if (h->sprite->x > f->sprite->x && h->sprite->y > f->sprite->y) {
			human_walk_right_down (h, f, indx, floor[h->oldtiley][h->oldtilex]);
			return;
		}

		if (h->sprite->x == f->sprite->x && h->sprite->y > f->sprite->y) {
			human_walk_down (h, f, indx, floor[h->oldtiley][h->oldtilex]);
			return;
		}
		
		if (h->sprite->x == f->sprite->x && h->sprite->y < f->sprite->y) {
			human_walk_up (h, f, indx, floor[h->oldtiley][h->oldtilex]);
			return;
		}

		if (h->sprite->x < f->sprite->x && h->sprite->y < f->sprite->y) {
			human_walk_left_up (h, f, indx, floor[h->oldtiley][h->oldtilex]);
			return;
		}

		if (h->sprite->x > f->sprite->x && h->sprite->y < f->sprite->y) {
			human_walk_right_up (h, f, indx, floor[h->oldtiley][h->oldtilex]);
			return;
		}

		if (h->sprite->x < f->sprite->x && h->sprite->y == f->sprite->y) {
			human_walk_left (h, f, indx, floor[h->oldtiley][h->oldtilex]);
			return;
		}

		if (h->sprite->x > f->sprite->x && h->sprite->y == f->sprite->y) {
			human_walk_right (h, f, indx, floor[h->oldtiley][h->oldtilex]);
			return;
		}
	}
	
}


void
human_render (struct human *h)
{
	sprite_render (h->sprite);
}