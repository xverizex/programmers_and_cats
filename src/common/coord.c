#include <common/coord.h>
#include <core/cmath.h>
#include <stdio.h>

extern uint32_t screen_width;
extern uint32_t screen_height;

void 
coord_get_real (int x, int y, int *xx, int *yy)
{
	float *cam = get_cam ();

	float ex = cam[12];
	float ey = cam[13];
	float ez = cam[14];

	y = screen_height - y;

	int ax = ex;
	int by = ey;

	x -= ax;
	y -= by;

	*xx = x;
	*yy = y;
}