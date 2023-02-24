#include <game/a_star.h>
#include <stdio.h>
#include <stdlib.h>

void
a_star_clear (struct a_star *a, uint32_t width, uint32_t height, struct floor ***f, int clearx, int cleary)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (f[y][x]->com_board) {
				a->map[width * y + x].ceil = 9999;
			} else if (f[y][x]->human) {
				a->map[width * y + x].ceil = 9999;
			} else {
				a->map[width * y + x].ceil = 0;
			}
			if (clearx == x && cleary == y)
				a->map[width * y + x].ceil = 0;

			a->map[width * y + x].walk = 0;
		}
	}
	
}

struct a_star *
a_star_init (uint32_t width, uint32_t height, struct floor ***f)
{
	struct a_star *a = malloc (sizeof (struct a_star));
	a->map = malloc (sizeof (struct map_a_star) * width * height);
	
	a->size_w = width;
	a->size_h = height;

	a_star_clear (a, width, height, f, 0, 0);

	return a;
}

#define SIZE_WEIGHT     9

struct point {
	int x, y;
	int weight;
};

struct field {
	struct point point;
};

struct path *
a_star_get_path (struct a_star *a, uint32_t index_path, uint32_t fromx, uint32_t fromy, uint32_t tilex, uint32_t tiley)
{
	
	int startx = fromx;
	int starty = fromy;
	int oldstartx = startx;
	int oldstarty = starty;
	{
		uint32_t pos = a->size_w * fromx + fromx;
		a->map[pos].link = pos;
	}
	
	uint32_t count = 0;

	while (1) {
		struct field field[] = {
			{startx - 1, starty + 1, 24}, {startx, starty + 1, 10}, {startx + 1, starty + 1, 24},
			{startx - 1, starty + 0, 10}, {startx, starty + 0,  0}, {startx + 1, starty + 0, 10},
			{startx - 1, starty - 1, 24}, {startx, starty - 1, 10}, {startx + 1, starty - 1, 24}
		};

		uint32_t main_pos = a->size_w * field[4].point.y + field[4].point.x;

		for (int i = 0; i < 9; i++) {
			if (i == 4) continue;
			int weight = 0;

			if (field[i].point.y < 0 || field[i].point.y >= a->size_h) continue;
			if (field[i].point.x < 0 || field[i].point.x >= a->size_w) continue;

			int horisont = 0;
			int vertical = 0;
			if (field[i].point.x < tilex)
				horisont = tilex - field[i].point.x;
			if (field[i].point.x > tilex)
				horisont = field[i].point.x - tilex;
			if (field[i].point.y < tiley)
				vertical = tiley - field[i].point.y;
			if (field[i].point.y > tiley)
				vertical = field[i].point.y - tiley;

			weight = horisont > vertical? horisont: vertical;

			uint32_t pos = a->size_w * field[i].point.y + field[i].point.x;
			weight += a->map[pos].ceil;

			a->map[pos].weight = weight;
		}

		uint32_t min_weight = 9999;
		int cur_pos = -1;

		int found = 0;
		for (int i = 0; i < 9; i++) {
			if (field[i].point.y < 0 || field[i].point.y >= a->size_h) continue;
			if (field[i].point.x < 0 || field[i].point.x >= a->size_w) continue;

			uint32_t pos = a->size_w * field[i].point.y + field[i].point.x;
			if (i == 4) continue;
			if (a->map[pos].weight < min_weight && a->map[pos].walk < 6) {
				found = 1;
				min_weight = a->map[pos].weight;
				cur_pos = pos;
				int x = startx;
				int y = starty;
				startx = field[i].point.x;
				starty = field[i].point.y;
				uint32_t dpos = a->size_w * starty + startx;
				a->map[dpos].link = pos;
				a->map[main_pos].next = dpos;
				a->map[main_pos].x = startx;
				a->map[main_pos].y = starty;
				a->map[main_pos].walk++;
			}
		}

		if (found) {
			count++;
		}
		else {
			found = 0;
			for (int i = 0; i < 9; i++) {
				if (field[i].point.y < 0 || field[i].point.y >= a->size_h) continue;
				if (field[i].point.x < 0 || field[i].point.x >= a->size_w) continue;

				uint32_t pos = a->size_w * field[i].point.y + field[i].point.x;
				if (i == 4) continue;
				if (a->map[pos].weight < 9999 && a->map[pos].walk < 6) {
					found = 1;
					min_weight = a->map[pos].weight;
					cur_pos = pos;
					int x = startx;
					int y = starty;
					startx = field[i].point.x;
					starty = field[i].point.y;
					uint32_t dpos = a->size_w * starty + startx;
					a->map[dpos].link = pos;
					a->map[main_pos].next = dpos;
					a->map[main_pos].x = startx;
					a->map[main_pos].y = starty;
					a->map[main_pos].walk++;
					break;
				}
			}
			if (found == 0) return NULL;
		}

		if (startx == oldstartx && starty == oldstarty) return NULL;
		oldstartx = startx;
		oldstarty = starty;
		//if (a->map[main_pos].walk >= 12) return NULL;
		if (startx == tilex && starty == tiley) break;
		if (found == 0) return NULL;
	}

	uint32_t pos = a->size_w * fromy + fromx;
	for (int i = 0; i < count; i++) {	
		path_add (index_path, a->map[pos].x, a->map[pos].y);

		pos = a->map[pos].next;
	}

	return path_get (index_path);
}