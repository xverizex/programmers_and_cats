#ifndef OFFICE_MATH_H
#define OFFICE_MATH_H

float *get_cam();
void math_scale(float *e, float sc);
void math_perspective(float *e, float angle, float aspect, float near, float far);
void math_translate(float *e, float x, float y, float z);
void math_mat4_mul(float *e, float *a, float *b);
void math_lookat(float *e, float ex, float ey, float ez, float rx, float ry, float rz, float ux, float uy, float uz);
void math_transpose_mat4(float *e, float *a);
void math_vec3_cross(float *e, float *a, float *b);
void math_vec3_normalize(float *e, float *a);
void math_vec3_sub(float *e, float *a, float *b);
void math_mat4_mul_vec4(float *e, float *m, float *v);
void math_ortho(float *e, float left, float right, float bottom, float top, float near, float far);
void math_mat4_inverse(float *e, float *m);
void math_rotate(float *e, float angle, int xaxis, int yaxis, int zaxis);
int math_intersect_ray_triangle(float *origin, float *direction, float *vec0, float *vec1, float *vec2);
int math_simple_intersect_ray_triangle(float *origin, float *direction, float *vec0, float *vec1, float *vec2);

#endif