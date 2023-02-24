#include <stdint.h>
#include <GLES3/gl3.h>
#include <core/shaders.h>
#include <core/sprite.h>
#include <core/cmath.h>

static uint32_t program;
static uint32_t ortho_loc;
static uint32_t pos_loc;
static uint32_t cam_loc;
static uint32_t tex_loc;
static uint32_t scale_loc;
static float *cam;

void
simple_2d_shader_init ()
{
	program = shaders_get (PROGRAM_2D_SIMPLE);

	glUseProgram (program);

	ortho_loc = glGetUniformLocation (program, "ortho");
	pos_loc = glGetUniformLocation (program, "pos");
	cam_loc = glGetUniformLocation (program, "cam");
	tex_loc = glGetUniformLocation (program, "s_texture");
	scale_loc = glGetUniformLocation (program, "scale");

	cam = get_cam ();
}

void
simple_2d_shader_render (struct sprite *sp)
{
	glUseProgram (program);

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, sp->tex->frame[sp->cur_tex]);

	glUniform1i (tex_loc, 0);
	glUniformMatrix4fv (cam_loc, 1, GL_FALSE, cam);
	glUniformMatrix4fv (ortho_loc, 1, GL_FALSE, sp->ortho);
	glUniformMatrix4fv (pos_loc, 1, GL_FALSE, sp->pos);
	glUniformMatrix4fv (scale_loc, 1, GL_FALSE, sp->sc);

	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, sp->tex->v_data);

	glEnableVertexAttribArray (1);
	glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, sp->tex->t_data);

	glDrawArrays (GL_TRIANGLES, 0, 6);
}