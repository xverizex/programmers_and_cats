/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15
 */

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <core/cmath.h>

#define M_PIf          3.14159265358979323846f

static float cam[16];

float *
get_cam ()
{
	return cam;
}

void 
math_scale (float *e, float sc)
{
	e[0] = sc;
	e[5] = sc;
	e[10] = sc;
	e[15] = 1.f;
}

/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15
 */

void 
math_rotate (float *e, float angle, int xaxis, int yaxis, int zaxis)
{
	e[0] = e[5] = e[10] = e[15] = 1.f;

	float radians = angle * M_PIf / 180.f;
	float c = cosf (radians);
	float s = sinf (radians);
	if (xaxis) {
		e[5] = c;
		e[6] = -s;
		e[9] = s;
		e[10] = c;
	}
	if (yaxis) {
		e[0] = c;
		e[2] = s;
		e[8] = -s;
		e[10] = c;
	}
	if (zaxis) {
		e[0] = c;
		e[1] = -s;
		e[4] = s;
		e[5] = c;
	}
}

void 
math_perspective (float *e, float angle, float aspect, float near, float far)
{
	memset (e, 0, sizeof (float) * 16);

	float f = 1.f / tanf ((angle * M_PIf / 180.f) * 0.5f);

	e[0] = f / aspect;
	e[5] = f; 
	e[10] = (near + far) / (near - far);
	e[14] = 2.f * (far * near) / (near - far);
	e[11] = -1.f;
	e[15] = 1.f;
}

void 
math_ortho (float *e, float left, float right, 
		float bottom, float top,
		float near, float far)
{
	memset (e, 0, sizeof (float) * 16);

	float rl, tb, fn;

	rl = 1.f / (right - left);
	tb = 1.f / (top - bottom);
	fn = -1.f / (far - near);

	e[0] = 2.f * rl;
	e[5] = 2.f * tb;
	e[10] = 2.f * fn;
	e[12] = -(right + left) * rl;
	e[13] = -(top + bottom) * tb;
	e[14] = (far + near) * fn;
	e[15] = 1.f;
}

/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15
 */

void 
math_translate (float *e, float x, float y, float z)
{
	memset (e, 0, sizeof (float) * 16);

#if 1
	e[0] = 1.f;
	e[5] = 1.f;
	e[10] = 1.f;
	e[15] = 1.f;
	e[12] = x;
	e[13] = y;
	e[14] = z;
#else
	e[0] = 1.f;
	e[5] = 1.f;
	e[10] = 1.f;
	e[15] = 1.f;
	e[3] = x;
	e[7] = y;
	e[11] = z;
#endif
}

void 
math_vec3_sub (float *e, float *a, float *b)
{
	e[0] = a[0] - b[0];
	e[1] = a[1] - b[1];
	e[2] = a[2] - b[2];
}

void 
math_vec3_normalize (float *e, float *a)
{
	float m = sqrtf (a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	if (m == 0.0f) return;

	e[0] = a[0] / m;
	e[1] = a[1] / m;
	e[2] = a[2] / m;
}

void 
math_vec3_cross (float *e, float *a, float *b)
{
	e[0] = a[1] * b[2] - a[2] * b[1];
	e[1] = a[2] * b[0] - a[0] * b[2];
	e[2] = a[0] * b[1] - a[1] * b[0];
}

/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15
 */

void 
math_transpose_mat4 (float *e, float *a)
{
	e[0] = a[0];
	e[1] = a[4];
	e[2] = a[8];
	e[3] = a[12];
	e[4] = a[1];
	e[5] = a[5];
	e[6] = a[9];
	e[7] = a[13];
	e[8] = a[2];
	e[9] = a[6];
	e[10] = a[10];
	e[11] = a[14];
	e[12] = a[3];
	e[13] = a[7];
	e[14] = a[11];
	e[15] = a[15];
}
/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15
 */

float 
math_vec3_dot (float *v, float *u)
{
	return (v[0] * u[0] + v[1] * u[1] + v[2] * u[2]);
}

void 
math_lookat (float *e, float ex, float ey, float ez, float rx, float ry, float rz, float ux, float uy, float uz)
{
	memset (e, 0, sizeof (float) * 16);

	float cam_forward[3];
	float cam_forward_t[3];
	float cam_right[3];
	float cam_right_t[3];
	float cam_up[3];

	math_vec3_sub (cam_forward_t, (float[3]) {rx, ry, rz}, (float[3]) {ex, ey, ez});
	math_vec3_normalize (cam_forward, cam_forward_t);

	math_vec3_cross (cam_right_t, cam_forward, (float[3]) {ux, uy, uz});
	math_vec3_normalize (cam_right, cam_right_t);

	math_vec3_cross (cam_up, cam_right, cam_forward);

	e[0] = cam_right[0];
	e[1] = cam_up[0];
	e[2] = -cam_forward[0];
	e[3] = 0.f;
	e[4] = cam_right[1];
	e[5] = cam_up[1];
	e[6] = -cam_forward[1];
	e[7] = 0.f;
	e[8] = cam_right[2];
	e[9] = cam_up[2];
	e[10] = -cam_forward[2];
	e[11] = 0.f;
	e[12] = -math_vec3_dot (cam_right, (float [3]) {ex, ey, ez});
	e[13] = -math_vec3_dot (cam_up, (float [3]) {ex, ey, ez});
	e[14] = math_vec3_dot (cam_forward, (float [3]) {ex, ey, ez});
	e[15] = 1.f;


}

/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15
 */

void 
math_mat4_mul (float *e, float *m, float *b)
{
	e[0] = m[0] * b[0] + m[1] * b[4] + m[2] * b[8] + m[3] * b[12];
	e[1] = m[0] * b[1] + m[1] * b[5] + m[2] * b[9] + m[3] * b[13];
	e[2] = m[0] * b[2] + m[1] * b[6] + m[2] * b[10] + m[3] * b[14];
	e[3] = m[0] * b[3] + m[1] * b[7] + m[2] * b[11] + m[3] * b[15];
	e[4] = m[4] * b[0] + m[5] * b[4] + m[6] * b[8] + m[7] * b[12];
	e[5] = m[4] * b[1] + m[5] * b[5] + m[6] * b[9] + m[7] * b[13];
	e[6] = m[4] * b[2] + m[5] * b[6] + m[6] * b[10] + m[7] * b[14];
	e[7] = m[4] * b[3] + m[5] * b[7] + m[6] * b[11] + m[7] * b[15];
	e[8] = m[8] * b[0] + m[9] * b[4] + m[10] * b[8] + m[11] * b[12];
	e[9] = m[8] * b[1] + m[9] * b[5] + m[10] * b[9] + m[11] * b[13];
	e[10] = m[8] * b[2] + m[9] * b[6] + m[10] * b[10] + m[11] * b[14];
	e[11] = m[8] * b[3] + m[9] * b[7] + m[10] * b[11] + m[11] * b[15];
	e[12] = m[12] * b[0] + m[13] * b[4] + m[14] * b[8] + m[15] * b[12];
	e[13] = m[12] * b[1] + m[13] * b[5] + m[14] * b[9] + m[15] * b[13];
	e[14] = m[12] * b[2] + m[13] * b[6] + m[14] * b[10] + m[15] * b[14];
	e[15] = m[12] * b[3] + m[13] * b[7] + m[14] * b[11] + m[15] * b[15];
}

void 
math_mat4_mul_vec4 (float *e, float *m, float *v)
{
	e[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3] * v[3];
	e[1] = m[4] * v[0] + m[5] * v[1] + m[6] * v[2] + m[7] * v[3];
	e[2] = m[8] * v[0] + m[9] * v[1] + m[10] * v[2] + m[11] * v[3];
	e[3] = m[12] * v[0] + m[13] * v[1] + m[14] * v[2] + m[15] * v[3];
}

int 
math_intersect_ray_triangle (float *origin, float *direction, float *v0, float *v1, float *v2)
{
	float edge1[3], edge2[3], p[3], t[3], q[3];

	float det, inv_det, u, v, dist;

	const float epsilon = 0.000001f;

	math_vec3_sub (edge1, v1, v0);
	math_vec3_sub (edge2, v2, v0);
	math_vec3_cross (p, direction, edge2);

	det = math_vec3_dot (edge1, p);

//	printf ("0: %f\n", det);
	if (det > -epsilon && det < epsilon) return 0;

	inv_det = 1.f / det;

	math_vec3_sub (t, origin, v0);

	u = inv_det * math_vec3_dot (t, p);
//	printf ("1: %f\n", u);
	if (u < 0.0f || u > 1.0f) return 0; 

	math_vec3_cross (q, t, edge1);

	v = inv_det * math_vec3_dot (direction, q);
//	printf ("2: %f\n", v);
	if (v < 0.0f || u + v > 1.0f) return 0;

	dist = inv_det * math_vec3_dot (edge2, q);
	printf ("distance: %f\n", dist);

	return dist > epsilon;
}

static float 
math_triangle_area (float *a, float *b, float *c)
{
	float ab[2] = {b[0] - a[0], b[1] - a[1]};
	float ac[2] = {c[0] - a[0], c[1] - a[1]};

	float cross_product = ab[0] * ac[1] - ab[1] * ac[0];
	return cross_product / 2.f;
}

int 
math_simple_intersect_ray_triangle (float *origin, float *d, float *v0, float *v1, float *v2)
{

	float vv0[3];
	float vv1[3];
	float vv2[3];

	float xp = 0.f;
	float yp = 0.f;

	if (v0[0] < 0.f) {
		xp = fabsf (v0[0]);
	} else if (v1[0] < 0.f) {
		xp = fabsf (v1[0]);
	} else if (v2[0] < 0.f) {
		xp = fabsf (v2[0]);
	}

	if (v0[1] < 0.f) {
		yp = fabsf (v0[1]);
	} else if (v1[1] < 0.f) {
		yp = fabsf (v1[1]);
	} else if (v2[1] < 0.f) {
		yp = fabsf (v2[1]);
	}

	printf ("xp yp: %f %f\n", xp, yp);

	vv0[0] = v0[0] + xp;
	vv0[1] = v0[1] + yp;
	vv0[2] = 0.f;

	vv1[0] = v1[0] + xp;
	vv1[1] = v1[1] + yp;
	vv1[2] = 0.f;

	vv2[0] = v2[0] + xp;
	vv2[1] = v2[1] + yp;
	vv2[2] = 0.f;

	d[0] += xp;
	d[1] += yp;
	d[2] = 0.f;

	float triangle_area = math_triangle_area (vv0, vv1, vv2);

	float area_sum = 0.0f;

	area_sum += math_triangle_area (vv0, vv1, d);
	area_sum += math_triangle_area (vv0, vv2, d);
	area_sum += math_triangle_area (vv1, vv2, d);

	printf ("<------------------>\n");
	printf (
			"triangle area: %f\n"
			"area sum: %f\n",
			triangle_area,
			area_sum
	       );


	if (triangle_area == area_sum) return 1;

	return 0;
}

/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15
 */
#include <stdlib.h>

float 
math_det_mat3_from_mat4 (float *e, int minory, int minorx)
{
	float m[3][3];
	int iy = 0;
	int ix = 0;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (y == minory) {
				iy--;
				break;
			}
			if (x == minorx) {
				continue;
			}
			m[iy][ix++] = e[y * 4 + x];
		}
		ix = 0;
		iy++;
	}

#if 0
	printf (
			"--------------------\n"
			"%f %f %f\n"
			"%f %f %f\n"
			"%f %f %f\n",
			m[0][0], m[0][1], m[0][2],
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2]
	       );
#endif
/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15
 */


	float det = 
		m[0][0]	* (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
		m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) +
		m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2])
		;

	return det;
}

float 
math_det_mat3 (float *e)
{
/*
 *  0  1  2
 *  3  4  5
 *  6  7  8 
 */


	float det = 
		e[0] * (e[4] * e[8] - e[7] * e[5]) -
		e[3] * (e[1] * e[8] - e[7] * e[2]) +
		e[6] * (e[1] * e[5] - e[4] * e[2])
		;

	return det;
}

void 
math_main_det_from_mat4 (float *s, float *e, int minory, int minorx)
{
	float m[3][3];
	int iy = 0;
	int ix = 0;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (y == minory) {
				iy--;
				break;
			}
			if (x == minorx) {
				continue;
			}
			m[iy][ix++] = e[y * 4 + x];
		}
		ix = 0;
		iy++;
	}

	int index = 0;
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			s[index++] = m[y][x];
		}
	}
}

static void 
debug_matrix (float *e)
{
	printf ("------------------------\n");
	printf (
			"%f %f %f %f\n"
			"%f %f %f %f\n"
			"%f %f %f %f\n"
			"%f %f %f %f\n",
			e[0], e[1], e[2], e[3],
			e[4], e[5], e[6], e[7],
			e[8], e[9], e[10], e[11],
			e[12], e[13], e[14], e[15]
	       );
}

/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9 10 11
 * 12 13 14 15
 */

void 
math_mat4_inverse (float *r, float *m)
{

	float det0;
	float det1;
	float det2;
	float det3;
	float main_det;
	float m0[9];
	float m1[9];
	float m2[9];
	float m3[9];

	math_main_det_from_mat4 (m0, m, 0, 0);
	det0 = math_det_mat3 (m0);

	math_main_det_from_mat4 (m1, m, 1, 0);
	det1 = math_det_mat3 (m1);

	math_main_det_from_mat4 (m2, m, 2, 0);
	det2 = math_det_mat3 (m2);

	math_main_det_from_mat4 (m3, m, 3, 0);
	det3 = math_det_mat3 (m3);

	float e[16];
	e[0] = math_det_mat3_from_mat4 (m, 0, 0);
	e[1] = math_det_mat3_from_mat4 (m, 0, 1);
	e[2] = math_det_mat3_from_mat4 (m, 0, 2);
	e[3] = math_det_mat3_from_mat4 (m, 0, 3);
	e[4] = math_det_mat3_from_mat4 (m, 1, 0);
	e[5] = math_det_mat3_from_mat4 (m, 1, 1);
	e[6] = math_det_mat3_from_mat4 (m, 1, 2);
	e[7] = math_det_mat3_from_mat4 (m, 1, 3);
	e[8] = math_det_mat3_from_mat4 (m, 2, 0);
	e[9] = math_det_mat3_from_mat4 (m, 2, 1);
	e[10] = math_det_mat3_from_mat4 (m, 2, 2);
	e[11] = math_det_mat3_from_mat4 (m, 2, 3);
	e[12] = math_det_mat3_from_mat4 (m, 3, 0);
	e[13] = math_det_mat3_from_mat4 (m, 3, 1);
	e[14] = math_det_mat3_from_mat4 (m, 3, 2);
	e[15] = math_det_mat3_from_mat4 (m, 3, 3);

	math_transpose_mat4 (r, e);

	main_det = m[0] * det0 - m[4] * det1 + m[8] * det2 - m[12] * det3;

	float invdet = 1.f / main_det;

	for (int i = 0; i < 16; i++) {
		r[i] *= invdet;
	}

}
