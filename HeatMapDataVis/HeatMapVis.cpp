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
			glRectf(	// Draw rectangles for heatmap grid
				(SCREEN_WIDTH / NUM_COLUMNS) * colNum,
				(SCREEN_HEIGHT / NUM_ROWS) * rowNum,
				((SCREEN_WIDTH / NUM_COLUMNS) * (colNum + 1)),
				(SCREEN_HEIGHT / NUM_ROWS) * (rowNum + 1)
			);
		}
	}
	
	glPopMatrix();	// Pop glmatrix from stack
}

void drawColorRectangles()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	float green = 0.05;

	for (unsigned int rowNum = 0; rowNum < NUM_ROWS - 1; ++rowNum)
	{
		for (unsigned int colNum = 1; colNum < NUM_COLUMNS; ++colNum)
		{
			glColor3f(0.0, green, 0.0);
			glRectf(	// Draw rectangles for heatmap grid
				(SCREEN_WIDTH / NUM_COLUMNS) * colNum,
				(SCREEN_HEIGHT / NUM_ROWS) * rowNum,
				((SCREEN_WIDTH / NUM_COLUMNS) * (colNum + 1)) + GRID_MARGIN,
				(SCREEN_HEIGHT / NUM_ROWS) * (rowNum + 1)
			);
			green += 0.05;
		}
	}
}

// Print column labels
void printLabels()
{
	glColor3f(0.0, 0.0, 0.0);

	// Label string
	std::string s = "Data ID";

	// Initialize font
	void* font = GLUT_BITMAP_9_BY_15;

	glRasterPos2i(0.0 + 20, SCREEN_HEIGHT - (SCREEN_HEIGHT / NUM_ROWS) + 20);

	// Print neighborhhood class ratio
	for (std::string::iterator labelIt = s.begin(); labelIt != s.end(); ++labelIt)
	{	// Loop through string, displaying each character
		char c = *labelIt;
		glutBitmapCharacter(font, c);
	}

	std::string co1 = "Coefficient 1";
	glRasterPos2i((SCREEN_WIDTH / NUM_COLUMNS) + 20,
		SCREEN_HEIGHT - (SCREEN_HEIGHT / NUM_ROWS) + 20);

	// Print neighborhhood class ratio
	for (std::string::iterator labelIt = co1.begin(); labelIt != co1.end(); ++labelIt)
	{	// Loop through string, displaying each character
		char c = *labelIt;
		glutBitmapCharacter(font, c);
	}

	std::string co2 = "Coefficient 2";
	glRasterPos2i((2 * (SCREEN_WIDTH / NUM_COLUMNS)) + 20,
		SCREEN_HEIGHT - (SCREEN_HEIGHT / NUM_ROWS) + 20);

	// Print neighborhhood class ratio
	for (std::string::iterator labelIt = co2.begin(); labelIt != co2.end(); ++labelIt)
	{	// Loop through string, displaying each character
		char c = *labelIt;
		glutBitmapCharacter(font, c);
	}
}

// heat map display function
void heatMap(std::vector<std::vector<GLfloat>> dataVec)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Draw outlines
	glColor3f(0.0, 0.0, 0.0);	// set color to black

	drawGrid();	// draw grid
	drawColorRectangles();	// color cells

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	printLabels();	// print column labels

}

// Extract data points into vector from text file
std::vector<std::vector<GLfloat>> extractData()
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

		allData[i] = data;	// add data point to current index of data set
	}
	// close fstream
	myFile.close();

	return allData;
}

// Main OpenGL display function
void myDisplay()
{
	// Extract data set into dataVec
	std::vector<std::vector<GLfloat>> dataVec = extractData();

	glClear(GL_COLOR_BUFFER_BIT);	// Set clear buffer bit
	glMatrixMode(GL_PROJECTION);	// Load GL_PROJECTION matrix
	glPushMatrix();					// push new GL matrix to atack
	glLoadIdentity();				// load identity matrix
	// set orthoganol projection
	gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);

	// heat map display function
	heatMap(dataVec);

	// set GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	// pop GL matrix from stack

	glFlush();	// flush display buffer
}

// program main function - initialize glut and register display
int main(int argc, char** argv)
{
	glutInit(&argc, argv);	// Initialize GLUT
	// Set initial display mode
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	// Set initial window size
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	// Set initial window position
	glutInitWindowPosition(100, 100);
	// Set window name, create window
	glutCreateWindow("HeatMap");
	// Register OpenGL display func
	glutDisplayFunc(myDisplay);
	// Set clear color to white
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	// Initialize viewport to window dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// GLUT main loop initialization
	glutMainLoop();
}