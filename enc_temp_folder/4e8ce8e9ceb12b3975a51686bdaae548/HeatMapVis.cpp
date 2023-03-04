#include "GL/glut.h"
#include "Canvas.h"
#include "myGLlib.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

const unsigned int SCREEN_WIDTH = 480;
const unsigned int SCREEN_HEIGHT = 440;

#define GRID_MARGIN 4.0
#define NUM_ROWS 10
#define NUM_COLUMNS 3
#define DATA_SIZE 683

Point2 CP = Point2(0, 0);
float CD = 0.0;

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
void turnTo(float angle)
{
	// Convert radians to degrees
	CD = angle * (180.0 / 3.141592653589793238463);
}

// turn the turtle given number of degrees
void turn(float angle)
{
	// Convert radians to degrees
	CD += angle * (180.0 / 3.141592653589793238463);
}

// move turtle forward in a straight line from CP
void forward(float dist, bool isVisible)
{
	const float radPerDeg = 0.017453393;
	float x = CP.getX() + dist * cos(radPerDeg * CD);
	float y = CP.getY() + dist * sin(radPerDeg * CD);

	// Move CP
	if (isVisible)
		lineTo(x, y);
	else
		moveTo(x, y);
}

void heatMap()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Draw outlines
	glColor3f(0.0, 0.0, 0.0);

	// Draw the tile frame to contain the glyphs
	glLineWidth(2.0);		// Set line width to 2
	//glPushMatrix();			// Push new matrix
	for (unsigned int rowNum = 0; rowNum < NUM_ROWS; ++rowNum)			// Rows
	{
		for (unsigned int colNum = 0; colNum < NUM_COLUMNS; ++colNum)	// Columns
		{
			glRectf(		// Draw rectangles for grid of glyphs
				0.0 + GRID_MARGIN,
				SCREEN_HEIGHT - (SCREEN_HEIGHT / NUM_ROWS) * rowNum,
				((SCREEN_WIDTH / NUM_COLUMNS) * (colNum + 1)) + GRID_MARGIN - 20,
				SCREEN_HEIGHT - (SCREEN_HEIGHT / NUM_ROWS) * (rowNum + 1)
			);
		}
	}
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// This is where you encode the color of the rectangle
	glColor3f(0.0, 1.0, 0.0);
	// Draw color
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(2.0, SCREEN_HEIGHT - 2.0, SCREEN_WIDTH / 20 * 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 10);

	// Draw outline
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(2.0, SCREEN_HEIGHT - 2.0, SCREEN_WIDTH / 20 * 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 10);

	unsigned char s[] = "1";
	int w;
	w = glutBitmapLength(GLUT_BITMAP_8_BY_13, s);
	glRasterPos2i(10.0, (SCREEN_HEIGHT - SCREEN_HEIGHT / 10) + 10);
	int len = 1;
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[0]);


}

// Extract data points into vector from text file
void extractData(std::vector<std::vector<GLfloat>> dataVec)
{

	// Read data file
	std::string line = "";
	std::ifstream myFile("breast-cancer-wisconsin.DATA");

	// Process data into a 2-D vector for ease of use
	std::vector<std::vector<GLfloat>> allData(DATA_SIZE);
	std::vector<bool> classify{};
	// Generalize this by using push_back and getline in test
	// Also populate vector of classification here

	for (unsigned int i = 0; i < DATA_SIZE; i++)
	{
		getline(myFile, line);
		int vecCount = 10;

		// Split string into a vector
		std::vector<int> dataInt;
		std::stringstream ss(line);

		while (ss.good()) {		// Replace "?" in data
			std::string substr = "";
			getline(ss, substr, ',');

			if (substr == "?") substr = std::to_string(*dataInt.begin());
			dataInt.push_back(stoi(substr));
			continue;
		}

		std::vector<GLfloat> data(dataInt.begin(), dataInt.end());

		// Set color determined by class
		if (*--(data.end()) == 4) classify.push_back(false);
		else					  classify.push_back(true);

		// Remove labels from data
		data.erase(data.begin());
		data.erase(--data.end());

		// Duplicate data attributes x 4 to move from 9-D to 36-D
		data.insert(data.end(), data.begin(), data.end());
		data.insert(data.end(), data.begin(), data.end());

		allData[i] = data;
	}
	myFile.close();
}

void myDisplay()
{

	std::vector<std::vector<GLfloat>>* dataVec = new std::vector<std::vector<GLfloat>>(DATA_SIZE);

	extractData(*dataVec);

	glClear(GL_COLOR_BUFFER_BIT);

	/*
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);
	*/

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);


	/*
	// Draw horizontal axis
	glLineWidth(4.0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2d(0.0, 20.0);
	glVertex2d(SCREEN_WIDTH, 20.0);
	glEnd();
	*/

	heatMap();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	delete(dataVec);
	glFlush();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GLC-L");
	glutDisplayFunc(myDisplay);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glutMainLoop();
}