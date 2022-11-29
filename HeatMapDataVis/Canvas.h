// Class Adapted from "Computer Graphics Using Open GL" Chapter 3

#pragma once

#include "GL\glut.h"
#include "myGLlib.h"


class Canvas {
private:

	Point2 CP;	// Current position
	IntRect viewport;
	RealRect window;

	float CD; // Current direction for turtle graphics

	// More members to be added
public:

	void setWindow(float l, float r, float b, float t)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D((GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t);
		window.set(l, r, b, t);
	}

	void setViewport(int l, int r, int b, int t)
	{
		// FROM pg. 86 of computer graphics using OpenGL
		glViewport(l, b, r - l, t - b);
	}

	// Constructor
	Canvas(int width, int height, char* windowTitle) {
		// We're doing this dummy argument thing here instead of in main
		// beacause cnv object will be a global variable, constructed before main is called

		// I zero-initialized to avoid error..
		char* argv[1]{ 0 };			// 'Dummy' argument list for glutInit()
		char dummyString[8]{ 0 };
		argv[0] = dummyString;	// Hook up the pointer to the dummy string
		int argc = 1;

		glutInit(&argc, argv);								 // Initialize GLUT
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);		 // Set display mode
		glutInitWindowSize(width, height);		        	 // Set window size
		glutInitWindowPosition(20, 20);					 	 // Set window position (on screen)
		glutCreateWindow(windowTitle);						 // Open screen window

		setWindow(0, (float)width, 0, (float)height);		 // Default world window
		setViewport(0, width, 0, height);					 // Default viewport
		CP.set(0.0f, 0.0f);									 // Initialize the CP to (0, 0)
	}

	IntRect getViewport(void)	// Divulge the viewport data
	{
		return this->viewport;
	}

	RealRect getWindow(void)	// Divulge the window data
	{
		return this->window;
	}


	float getWindowAspectRatio(void)
	{
		return window.getAspectRatio();
	}

	void clearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void setBackgroundColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 0.0);
	}

	void setColor(float r, float g, float b)
	{
		glColor3f(r, g, b);
	}

	// Move the CP through the given displacement
	void moveRel(float dx, float dy)
	{
		CP.set(CP.getX() + dx, CP.getY() + dy);
	}

	// Draw a line from CP to the given displacement, and move CP
	void lineRel(float dx, float dy)
	{
		float x = CP.getX() + dx;
		float y = CP.getY() + dy;

		lineTo(x, y);
		CP.set(x, y);
	}

	// Draw a line from CP to the new vertex
	void lineTo(float x, float y)
	{
		glBegin(GL_LINES);
		glVertex2f((GLfloat)CP.getX(), (GLfloat)CP.getY());
		glVertex2f((GLfloat)x, (GLfloat)y);
		glEnd();

		CP.set(x, y);	// Update the CP
		glFlush();		// Flush the buffer
	}

	void lineTo(Point2 p)
	{
		glBegin(GL_LINES);
		glVertex2f((GLfloat)CP.getX(), (GLfloat)CP.getY());
		glVertex2f((GLfloat)p.getX(), (GLfloat)p.getY());
		glEnd();

		CP.set(p.getX(), p.getY());	// Update the CP
		glFlush();		// Flush the buffer
	}

	// Update the CP
	void moveTo(float x, float y)
	{
		CP.set(x, y);
	}

	void moveTo(Point2 p)
	{
		CP.set(p.getX(), p.getY());
	}

	// TURTLE GRAPHICS
	// turn the turtle to given angle
	void turnTo(float angle) { CD = angle; }
	// turn the turtle given number of degrees
	void turn(float angle) { CD += angle; }

	// move turtle forward in a straight line from CP
	void forward(float dist, int isVisible)
	{
		const float radPerDeg = 0.017453393;
		float x = CP.getX() + dist * cos(radPerDeg * CD);
		float y = CP.getY() + dist * sin(radPerDeg * CD);

		// Move CP
		if (isVisible) lineTo(x, y);
		else moveTo(x, y);
	}

	// Initialize Current Transformation (CT)
	void initCT(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void scale2D(double sx, double sy)
	{
		glMatrixMode(GL_MODELVIEW);
		glScaled(sx, sy, 1.0);	// CT = CT * (2D scaling)
	}

	void translate2D(double dx, double dy)
	{
		glMatrixMode(GL_MODELVIEW);
		glTranslated(dx, dy, 0.0);	// CT = CT * (2D translation)
	}

	void rotate2D(double angle)
	{
		glMatrixMode(GL_MODELVIEW);
		glRotated(angle, 0.0, 0.0, 1.0);  // CT = CT * (2D rotation)
	}

	void pushCT(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
	}

	void popCT(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	// More functions to be added
};