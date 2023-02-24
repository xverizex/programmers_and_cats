#include <stdio.h>
#include <stdlib.h>
#include <game/path.h>

static struct path *path;

void
path_init (const int size)
{
	path = malloc (sizeof (struct path) * size);
	for (int i = 0; i < size; i++) {
		path[i].coord = NULL;
		path[i].size_coord = 0;
	}
}

struct path *
path_get (int index)
{
	return &path[index];
}

void
path_add (int index, uint32_t x, uint32_t y)
{
	int indx = path[index].size_coord++;
	path[index].coord = realloc (path[index].coord, sizeof (struct coord) * (indx + 1));
	path[index].coord[indx].x = x;
	path[index].coord[indx].y = y;
}

void
path_clear (int index)
{
	if (path[index].coord)
		free (path[index].coord);
	path[index].coord = NULL;
	path[index].size_coord = 0;
}