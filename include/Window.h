#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

enum eWindowError {
	WINDOW_ERROR_NULL,
	WINDOW_ERROR_INIT,
	WINDOW_ERROR_WINDOW,
	WINDOW_ERROR_GL,
	WINDOW_ERROR_GLEW
};

class Window
{
public:
	Window();


	int init();
	void quit();
	void display();
	void clear();
	void clear(float r, float g, float b);
	void clear(float r, float g, float b, float a);
	void drawRect(float x1, float x2, float y1, float y2, float r, float g, float b);
	void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float r, float g, float b);
	void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b);
	void translateCamera();
	void translateCamera(double x, double y, float z);
	void resetCamera();
	void zoom(int dir);
	void scrollCamera();
	void setMouse();
	void setMouse(bool clicked);
	void getWorldPos(double x, double y, double* newx, double* newy);
	void getScreenPos(double x, double y, double* newx, double* newy);


	double camx = 0.0f;
	double camy = 0.0f;
	float camzoom = 1.0f;
	float camzoomfactor = 1.1f;

	double mouseclickx = 0;
	double mouseclicky = 0;
	bool mouseclicked = false;
private:

	SDL_Window* window;
	SDL_GLContext glc;

	bool initSDL();
	bool createWindow();
	bool createGLContext();
	bool initGL();
	bool initGLEW();
	void displaySDLError();
	void displayGLError(GLenum e);
};