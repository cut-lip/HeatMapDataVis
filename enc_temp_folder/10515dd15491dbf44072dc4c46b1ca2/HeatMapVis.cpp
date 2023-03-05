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

// Draw the outer frame for the heatmap table
void drawGrid()
{
	glLineWidth(2.0);	// Set line width to 2
	glPushMatrix();		// Push new glmatrix
	for (unsigned int rowNum = 0; rowNum < NUM_ROWS; ++rowNum)
	{	// iterate Rows
		for (unsigned int colNum = 0; colNum < NUM_COLUMNS; ++colNum)
		{	// iterate Columns
			glRectf(		// Draw rectangles for heatmap grid
				0.0 + GRID_MARGIN,
				SCREEN_HEIGHT - (SCREEN_HEIGHT / NUM_ROWS) * rowNum,
				((SCREEN_WIDTH / NUM_COLUMNS) * (colNum + 1)) + GRID_MARGIN - 20,
				SCREEN_HEIGHT - (SCREEN_HEIGHT / NUM_ROWS) * (rowNum + 1)
			);
		}
	}

	// This is where you encode the color of the rectangle
	glColor3f(0.0, 1.0, 0.0);
	// Draw color
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(2.0, SCREEN_HEIGHT - 2.0, SCREEN_WIDTH / 20 * 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 10);

	float green = 0.05;

	for (unsigned int rowNum = 0; rowNum < NUM_ROWS; ++rowNum)
	{
		for (unsigned int colNum = 0; colNum < NUM_COLUMNS; ++colNum)
		{
			glColor3f(0.0, green, 0.0);
			glRectf(	// Draw rectangles for heatmap grid
				SCREEN_WIDTH * rowNum + GRID_MARGIN,
				SCREEN_HEIGHT - (SCREEN_HEIGHT / NUM_ROWS) * rowNum,
				((SCREEN_WIDTH / NUM_COLUMNS) * (colNum + 1)) + GRID_MARGIN - 20,
				SCREEN_HEIGHT - (SCREEN_HEIGHT / NUM_ROWS) * (rowNum + 1)
			);
			green += 0.05;
		}
	}

	glPopMatrix();	// Pop glmatrix from stack
}

void heatMap()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Draw outlines
	glColor3f(0.0, 0.0, 0.0);

	drawGrid();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	/*
	// This is where you encode the color of the rectangle
	glColor3f(0.0, 1.0, 0.0);
	// Draw color
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(2.0, SCREEN_HEIGHT - 2.0, SCREEN_WIDTH / 20 * 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 10);

	// Draw outline
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(2.0, SCREEN_HEIGHT - 2.0, SCREEN_WIDTH / 20 * 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 10);
	*/
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
	// Initialize fstream
	std::ifstream myFile("breast-cancer-wisconsin.DATA");

	// Temp vector for current data point
	std::vector<std::vector<GLfloat>> allData(DATA_SIZE);
	// Extract data class into seperate vector
	std::vector<bool> classify{};

	for (unsigned int i = 0; i < DATA_SIZE; i++)
	{	// For each data point in the file
		getline(myFile, line);
		int vecCount = 10;	// number of significant attributes

		// Split string into a vector
		std::vector<int> dataInt;
		// Initialize sstream
		std::stringstream ss(line);

		// While stream has more data to read
		while (ss.good()) {
			std::string substr = "";
			// Split string based on comma delimiter,
			// extracting attributes
			getline(ss, substr, ',');
			// Add attribute to temp vector
			dataInt.push_back(stoi(substr));
			continue;
		}

		// Convert ints to floats
		std::vector<GLfloat> data(dataInt.begin(), dataInt.end());

		// Set color determined by class (2 = B, 4 = M)
		if (*--(data.end()) == 4) classify.push_back(false);
		else					  classify.push_back(true);

		// Remove labels from data
		data.erase(data.begin());
		data.erase(--data.end());

		allData[i] = data;	// add data point to current index of data set
	}
	// close fstream
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