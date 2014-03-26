#include "gfx/opengl_render.hpp"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "gfx/camera/camera.hpp"
#include "gfx/camera/first_person.hpp"

static unsigned int last_state = -1; // make sure to print first state.

// Helper macro.
#define CASE(x) \
	case ((x)): \
		if ((x) != last_state) { \
			printf("OpenGL STATE: %s\n", #x); \
			last_state = (x); \
		} \
		break;

// macro that is given value from glGetError()
// prints user-friendly message once.
//
// Guaranteened not to flood.
//
// @returns with the same value as glGetError. 0 means OK.
#define GL_PRINT_ERROR \
	switch (glGetError()) { \
	CASE(GL_NO_ERROR); \
	CASE(GL_INVALID_ENUM); \
	CASE(GL_INVALID_VALUE); \
	CASE(GL_INVALID_OPERATION); \
	CASE(GL_STACK_OVERFLOW); \
	CASE(GL_STACK_UNDERFLOW); \
	CASE(GL_OUT_OF_MEMORY); \
	CASE(GL_TABLE_TOO_LARGE); \
	}

int screen_width = 0;
int screen_height = 0;

SDL_Window *window = NULL;
SDL_GLContext context;

Mat4x4f projectionMatrix;

Camera *camera;
bool wasd[4] = { false, false, false, false };

void setPerspectiveProjection(float fovy, float near, float far)
{
	float radians = 0.5 * fovy * DEGREES_2_RADIANS;
	float d       = cosf(radians) / sinf(radians);
	float aspect  = (float) screen_width / (float) screen_height;
	float deltaZ  = far - near;

	projectionMatrix.set(
		d / aspect, .0f, .0f               , .0f,
		.0f       , d  , .0f               , .0f,
		.0f       , .0f, -(far+near)/deltaZ, (-2*far*near)/deltaZ,
		.0f       , .0f, -1.0f             , .0f);

	glMultMatrixf(projectionMatrix.m);
}

void on_resize(int width, int height)
{
	screen_width = width;
	screen_height = height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	setPerspectiveProjection(90, 1, 200.0);
	//gluPerspective(60.0f, (float) width / height, 1.0f, 5.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

Vec2f generate_movement_vector(float dt)
{
	const float speed = 25.0f * dt;
	Vec2f movement;

	if (wasd[0] ^ wasd[2])
		movement.y = (wasd[0]) ? speed : -speed;

	if (wasd[1] ^ wasd[3])
		movement.x = (wasd[1]) ? -speed : speed;

	return movement;
}

int on_event(SDL_Event *event)
{
	switch (event->type) {
	case SDL_QUIT:
		return 0;
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym) {
		case SDLK_ESCAPE:
			return 0;
		case SDLK_w: wasd[0] = true; break;
		case SDLK_a: wasd[1] = true; break;
		case SDLK_s: wasd[2] = true; break;
		case SDLK_d: wasd[3] = true; break;
		default:
			break;
		}
		break;

	case SDL_KEYUP:
		switch (event->key.keysym.sym) {
		case SDLK_w: wasd[0] = false; break;
		case SDLK_a: wasd[1] = false; break;
		case SDLK_s: wasd[2] = false; break;
		case SDLK_d: wasd[3] = false; break;
		case SDLK_j:
			std::cout << camera->getModelViewMatrix() << std::endl;
			break;
		default:
			break;
		}
		break;

	case SDL_WINDOWEVENT:
		switch (event->window.event) {
		case SDL_WINDOWEVENT_RESIZED:
			on_resize(event->window.data1, event->window.data2);
			break;
		default:
			break;
		}
		break;
	case SDL_MOUSEMOTION:
	{
		Vec2f motion(-event->motion.xrel, -event->motion.yrel);
		motion *= DEGREES_2_RADIANS * .1f;
		camera->onCameraMotion(motion);
		break;
	}
	case SDL_MOUSEWHEEL:
		camera->onCameraZoom(1.0 - 0.1*event->wheel.y);
		break;
	default:
		break;
	}

	return 1;
}

int init_OpenGL()
{
	glClearColor(0.5, 0.5, 0.5, 1);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	return 0;
}

int init_SDL2(int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		printf("Unable to initialize SDL2: %s\n", SDL_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	screen_width = width;
	screen_height = height;

	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_GRABBED;
	window = SDL_CreateWindow("ROAMing",
	                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                          width, height, flags);

	if (window == NULL) {
		printf("Unable to create SDL_Window: %s\n", SDL_GetError());
		return -1;
	}

	context = SDL_GL_CreateContext(window);
	SDL_Event resizeEvent;
	resizeEvent.type = SDL_WINDOWEVENT;
	resizeEvent.window.event = SDL_WINDOWEVENT_RESIZED;
	resizeEvent.window.windowID = SDL_GetWindowID(window);
	resizeEvent.window.data1 = width;
	resizeEvent.window.data2 = height;
	SDL_PushEvent(&resizeEvent);

	return init_OpenGL();
}

void render(TerrainPatch *patch)
{
	if (init_SDL2(1024, 768) != 0) {
		return;
	}

	camera = new FirstPerson(Vec3f(0, 1, 0), Vec3f(0, 0, 0));
	//camera->forceMatrix(Mat4x4f(-0.999875,   0.0111575, -0.0112211,  22.1669,
	//                            -0.0111999, -0.99993,    0.00372371, 25.8863,
	//                            -0.0111788, 0.00384731,  0.99993,   -36.5337,
	//                            0, 0, 0, 1));
	camera->forceMatrix(Mat4x4f(0.729182, -0.68285, 0.0448257, -1.528,
	                            0.511006, 0.586905, 0.628025, -23.0045,
	                            -0.455157, -0.435041, 0.776898, -7.35539,
	                            0, 0, 0, 1));
	int running = 1;
	uint32_t last = 0;

	float *triPool = new float[100000*9];

	while (running) {
		uint32_t current = SDL_GetTicks();
		float delta = (current - last) / 1000.0f;

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			running &= on_event(&event);
		}

		camera->onCameraMovement(generate_movement_vector(delta));

		patch->reset();
		patch->tessellate(camera->getPosition()/200);
		patch->getTessellation(triPool);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		camera->setupScene();

		glColor3f(0.0, 0, 0);
		glScalef(200.0, 200.0, 20.0);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
			size_t i;
			for (i=0; i<patch->amountOfLeaves(); ++i) {
				int idx = i*9;
				glVertex3f(triPool[idx+0], triPool[idx+1], triPool[idx+2]);
				glVertex3f(triPool[idx+3], triPool[idx+4], triPool[idx+5]);
				glVertex3f(triPool[idx+3], triPool[idx+4], triPool[idx+5]);
				glVertex3f(triPool[idx+6], triPool[idx+7], triPool[idx+8]);
				glVertex3f(triPool[idx+6], triPool[idx+7], triPool[idx+8]);
				glVertex3f(triPool[idx+0], triPool[idx+1], triPool[idx+2]);
			}
		glEnd();

		GL_PRINT_ERROR;
		last = current;
		SDL_GL_SwapWindow(window);
	}

}
