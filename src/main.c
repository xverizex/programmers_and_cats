#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <core/game.h>
#include <core/cmath.h>
#include <core/shaders.h>
#include <GLES3/gl3.h>
#include <locale.h>

static int cur_level;
static int x = 0;
static int y = 0;
static float *cam;
static struct game *game;
static SDL_Window *win;

static void loop ();

static uint32_t is_fullscreen;

uint32_t screen_width = 1280;
uint32_t screen_height = 720;
#ifdef WEBGL
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

// const EmscriptenUiEvent *uiEvent
static EM_BOOL
Emscripten_HandleResize(int eventType, const void *uiEvent, void *userData)
{
	
#if 1
    double w, h;
    emscripten_get_element_css_size("#canvas", &w, &h);
    
    emscripten_set_canvas_element_size("#canvas", w, h);
  
    emscripten_set_element_css_size("#canvas", w, h);
    
	printf ("resize\n");
#endif
    return 0;
}

static int
on_canvassize_changed (int, const void *d, void *a)
{
	return 0;
}

static EM_BOOL
Emscripten_HandleFullscreenChange (int eventType, const EmscriptenFullscreenChangeEvent *fullscreenChangeEvent, void *userData)
 {
	printf ("handle fullscreen\n");

	int w, h;
	#if 1
	if (fullscreenChangeEvent->isFullscreen) {
		emscripten_exit_soft_fullscreen();

    // Workaround https://github.com/kripken/emscripten/issues/5124#issuecomment-292849872
    	EM_ASM(JSEvents.inEventHandler = true);
    	EM_ASM(JSEvents.currentEventHandler = {allowsDeferredCalls:true});

    	EmscriptenFullscreenStrategy strategy = {
        	.scaleMode = EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH,
        	.canvasResolutionScaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF,
        	.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT,
        	.canvasResizedCallback = Emscripten_HandleResize,
        	.canvasResizedCallbackUserData = NULL
    	};
    	emscripten_request_fullscreen_strategy(NULL, EM_FALSE, &strategy);
	} else {

		emscripten_exit_fullscreen();    
	}
	#endif

	

	

	return 0;
}
EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent *e, void *userData)
{
	#if 0
  printf("%s, screen: (%ld,%ld), client: (%ld,%ld),%s%s%s%s button: %hu, buttons: %hu, movement: (%ld,%ld), target: (%ld, %ld)\n",
    emscripten_event_type_to_string(eventType), e->screenX, e->screenY, e->clientX, e->clientY,
    e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "", 
    e->button, e->buttons, e->movementX, e->movementY, e->targetX, e->targetY);
#endif
    if (eventType == EMSCRIPTEN_EVENT_MOUSEUP) {
		game_click (cur_level, e->targetX, e->targetY);
	}

  return 0;
}

EM_BOOL em_loop (double tim, void *data)
{
		loop ();

		return EM_TRUE;
}
#endif

static int resized;

int
thread_sdl_events (void *data)
{
	SDL_Event event;

	while (SDL_WaitEvent (&event)) {
		switch (event.type) {
			case SDL_WINDOWEVENT:
				{
					printf ("event.window.event: %d\n", event.window.event);
						if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
							screen_width = event.window.data1;
							screen_height = event.window.data2;
							SDL_SetWindowSize (win, screen_width, screen_height);
							resized = 1;
						}
				}
				break;
			case SDL_KEYDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				{
					SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent *) &event;
					game_click (cur_level, m->x, m->y);
				}
				break;
		}
	}

	return 0;
}

	
int
main (int argc, char **argv)
{
	setlocale (LC_ALL, "en_US.UTF-8");

	int ret = SDL_Init (SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	if (ret != 0) {
		fprintf (stderr, "SDL_Init: [%s]\n", SDL_GetError ());
		exit (-1);
	}

	
	cam = get_cam ();

#ifndef WEBGL
#if 1
	SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_ALPHA_SIZE, 8);
#endif
#endif

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

	

	win = SDL_CreateWindow ("Programmers and cats",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			screen_width,
			screen_height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

#if 0
	SDL_DisplayMode mode;
	SDL_GetWindowDisplayMode (0, &mode);
	screen_width = mode.w;
	screen_height = mode.h;

	SDL_SetWindowSize (win, screen_width, screen_height);
#endif

	SDL_GLContext ctx = SDL_GL_CreateContext (win);

	game = game_init ();

	SDL_Thread *thread_ptr = SDL_CreateThread (thread_sdl_events,
			"sdl_events",
			game);


	SDL_GL_SetSwapInterval (1);

	glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport (0, 0, screen_width, screen_height);

	shaders_init ();

	game_load_resources (game->cur_level);

	
	math_lookat (cam, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);



#ifdef WEBGL
	emscripten_set_mouseup_callback("#canvas", NULL, 1, mouse_callback);
	emscripten_set_fullscreenchange_callback("#document", NULL, 1, Emscripten_HandleFullscreenChange);
	//emscripten_set_resize_callback("#canvas", NULL, 1, Emscripten_HandleResize);
	emscripten_request_animation_frame_loop (em_loop, 0);
#else
	while (1) {
		loop ();		
	}
#endif
}

void loop ()
{
	if (resized) {
			resized = 0;
			glViewport (0, 0, screen_width, screen_height);
			printf ("screen size: %d %d\n", screen_width, screen_height);
		}

		const uint8_t *state = SDL_GetKeyboardState (NULL);
		if (state[SDL_SCANCODE_Q])
			exit (0);

#ifdef WEBGL
		if (state[SDL_SCANCODE_E])
			emscripten_request_fullscreen("#canvas", 0);
#endif

		if (state[SDL_SCANCODE_W]) {
			y += 8;
			math_lookat (cam, x, y, -1.f, x, y, 0.f, 0.f, 1.f, 0.f);
		}
		if (state[SDL_SCANCODE_S]) {
			y -= 8;
			math_lookat (cam, x, y, -1.f, x, y, 0.f, 0.f, 1.f, 0.f);
		}

		if (state[SDL_SCANCODE_A]) {
			x += 8;
			math_lookat (cam, x, y, -1.f, x, y, 0.f, 0.f, 1.f, 0.f);
		}

		if (state[SDL_SCANCODE_D]) {
			x -= 8;
			math_lookat (cam, x, y, -1.f, x, y, 0.f, 0.f, 1.f, 0.f);
		}


		cur_level = game->cur_level;
		game_clear_screen (cur_level);

		game_step (cur_level);

		game_render (cur_level);

		SDL_GL_SwapWindow (win);
		SDL_Delay (16);
}