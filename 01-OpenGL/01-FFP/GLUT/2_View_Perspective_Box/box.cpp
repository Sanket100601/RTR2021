#include <GL\glut.h>
#include <iostream>
#include <windows.h>

// Function to initialize the drivers
void myInit(void)
{
	// Clear all the screen color
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Sets background color to black
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	// Specify the display area
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
}

// Function to display 
void myDisplay(void)
{
	// Clear the screen buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Clear the screen buffer
	glClear(GL_COLOR_BUFFER_BIT);


	glColor3f(0.0f, 0.0f, 0.8f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(20, 30);
	glVertex2i(50, 20);

	glVertex2i(20, 60);
	glVertex2i(50, 50);

	glVertex2i(20, 60);
	glVertex2i(20, 30);

	glVertex2i(50, 50);
	glVertex2i(50, 20);

	glEnd();

	glColor3f(0.0f, 0.0f, 0.5f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(50, 20);
	glVertex2i(80, 30);

	glVertex2i(50, 50);
	glVertex2i(80, 60);

	glVertex2i(50, 50);
	glVertex2i(50, 20);

	glVertex2i(80, 60);
	glVertex2i(80, 30);

	glEnd();

	glColor3f(0.0f, 0.0f, 0.35f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(50, 50);
	glVertex2i(20, 60);

	glVertex2i(50, 70);
	glVertex2i(80, 60);

	glVertex2i(50, 70);
	glVertex2i(50, 50);

	glVertex2i(80, 60);
	glVertex2i(20, 60);

	glEnd();

	// Sends all output to display
	glFlush();
}

// Driver Code
int main(int argc, char** argv)
{
	// Initialize the init function
	glutInit(&argc, argv);

	// Initialize the toolkit;
	glutInitDisplayMode(
		GLUT_SINGLE | GLUT_RGB);

	// Sets the display mode and
	// specify the colour scheme
	glutInitWindowSize(800, 600);

	// Specify the window size
	glutInitWindowPosition(0, 0);

	// Sets the starting position
	// for the window
	glutCreateWindow("2 View Perspective Of Box");

	// Creates the window and
	// sets the title
	glutDisplayFunc(myDisplay);
	myInit();

	// Additional initializations
	glutMainLoop();

	// Go into a loop until event
	// occurs
	return 0;
}