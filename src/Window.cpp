#include <stdio.h>
#include <string>

#include <glew/glew.h>
#include <GL/GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Window.h"

Window::Window()
{
	window = NULL;
	glc = NULL;

	camx = 0;
	camy = 0;
	camzoom = 1;
}

int Window::init()
{
	if (!initSDL())
	{
		return WINDOW_ERROR_INIT;
	}
	if (!createWindow())
	{
		return WINDOW_ERROR_WINDOW;
	}
	if (!createGLContext())
	{
		return WINDOW_ERROR_GL;
	}
	if (!initGL())
	{
		return WINDOW_ERROR_GL;
	}
	if (!initGLEW())
	{
		return WINDOW_ERROR_GLEW;
	}

	return 0;
}


void Window::quit()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Window::clear()
{
	clear(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::clear(float r, float g, float b)
{
	clear(r, g, b, 1.0f);
}

void Window::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display()
{
	SDL_GL_SwapWindow(window);
}

void Window::drawRect(float x1, float y1, float x2, float y2, float r, float g, float b)
{
	glBegin(GL_QUADS);
	glColor3f(r, g, b);

	glVertex2f(x1, y1);
	glVertex2f(x1, y2);
	glVertex2f(x2, y2);
	glVertex2f(x2, y1);


	glEnd();
}

void Window::drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float r, float g, float b)
{
	glBegin(GL_QUADS);
	glColor3f(r, g, b);

	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);

	glEnd();
}

void Window::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b)
{
	glBegin(GL_TRIANGLES);
	glColor3f(r, g, b);

	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);

	glEnd();
}

void Window::translateCamera()
{
	translateCamera(camx, camy, camzoom);
}

void Window::translateCamera(double x, double y, float z)
{

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-(float)x, -(float)y, 0.f);
	glScalef(z, z, z);
}

void Window::scrollCamera()
{
	if (mouseclicked)
	{
		int x, y;
		double dx, dy;
		SDL_GetMouseState(&x, &y);

		dx = x - mouseclickx;
		dy = y - mouseclicky;
		mouseclickx = (double)x;
		mouseclicky = (double)y;

		camx -= dx;
		camy -= dy;
	}
}

void Window::zoom(int dir)
{
	int x, y; // original screen x,y
	double wx, wy; // original world x,y
	double sx, sy; // new screen x,y

	if (dir > 0)
	{
		camzoom *= camzoomfactor;
	}
	else
	{
		camzoom /= camzoomfactor;
	}
	SDL_GetMouseState(&x, &y);
	getWorldPos(x, y, &wx, &wy);

	glPushMatrix(); // original matrix
	translateCamera(camx, camy, camzoom);
	getScreenPos(wx, wy, &sx, &sy);
	glPopMatrix(); // discard translations

	camx += (sx - x); // move camera by difference of adjusted screen coords
	camy += (sy - y);


}
void Window::setMouse()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	mouseclickx = x;
	mouseclicky = y;
}
void Window::setMouse(bool clicked)
{
	setMouse();
	mouseclicked = clicked;
}
void Window::getWorldPos(double x, double y, double* newx, double* newy)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	posX = 0.f;
	posY = 0.f;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = x;
	winY = viewport[3] - y;
	glReadPixels((int)x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, newx, newy, &posZ);
}

void Window::getScreenPos(double x, double y, double* newx, double* newy)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble posX, posY, posZ;
	float z;
	posX = 0.f;
	posY = 0.f;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glReadPixels((int)x, (int)y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
	gluProject(x, y, z, modelview, projection, viewport, &posX, &posY, &posZ);
	*newx = posX;
	*newy = viewport[3] - posY;
}

void Window::resetCamera()
{
	
}

bool Window::initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		displaySDLError();
		return false;
	}

	return true;
}

bool Window::initGL()
{

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix(); // default modelview matrix

	glClearColor(0.f, 0.f, 0.f, 1.f);

	GLenum e = glGetError();
	if (e != GL_NO_ERROR)
	{
		displayGLError(e);
		return false;
	}

	return true;
}

bool Window::initGLEW()
{
	GLenum e = glewInit();
	if (e != GLEW_OK)
	{
		displayGLError(e);
		return false;
	}
	return true;
}

bool Window::createWindow()
{
	window = SDL_CreateWindow("iivlx", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		displaySDLError();
		return false;
	}
	return true;
}

bool Window::createGLContext()
{
	glc = SDL_GL_CreateContext(window);
	if (glc == NULL) {
		displaySDLError();
		return false;
	}
	return true;
}


void Window::displaySDLError()
{
	printf("SDL_Error: %s\n", SDL_GetError());
	SDL_ShowSimpleMessageBox(16, "Error", SDL_GetError(), 0);
}

void Window::displayGLError(GLenum e)
{
	printf("GL Error: %d\n", e);
	std::string e_str = std::to_string(e);
	SDL_ShowSimpleMessageBox(16, "GL Error", e_str.c_str() , 0);
}