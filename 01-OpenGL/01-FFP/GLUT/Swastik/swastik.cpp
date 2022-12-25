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

	//central vertical polygon for swastik
	
	glColor3f(0.8f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(45, 20);
	glVertex2i(55, 20);

	glVertex2i(45, 80);
	glVertex2i(55, 80);

	glVertex2i(45, 80);
	glVertex2i(45, 20);

	glVertex2i(55, 80);
	glVertex2i(55, 20);

	glEnd();

	// lower horizontal polygon for swastik

	glColor3f(0.8f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(20, 20);
	glVertex2i(55, 20);

	glVertex2i(30, 30);
	glVertex2i(55, 30);

	glVertex2i(30, 30);
	glVertex2i(20, 20);

	glVertex2i(55, 30);
	glVertex2i(55, 20);

	glEnd();

	// central horizontal polygon for swastik

	glColor3f(0.8f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(20, 45);
	glVertex2i(80, 45);

	glVertex2i(20, 55);
	glVertex2i(80, 55);

	glVertex2i(20, 55);
	glVertex2i(20, 45);

	glVertex2i(80, 55);
	glVertex2i(80, 45);

	glEnd();

	// left vertical polygon for swastik

	glColor3f(0.8f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(20, 45);
	glVertex2i(30, 45);

	glVertex2i(20, 80);
	glVertex2i(30, 70);

	glVertex2i(20, 80);
	glVertex2i(20, 45);

	glVertex2i(30, 70);
	glVertex2i(30, 45);

	glEnd();

	// right vertical polygon for swastik

	glColor3f(0.8f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(70, 30);
	glVertex2i(80, 20);

	glVertex2i(70, 55);
	glVertex2i(80, 55);

	glVertex2i(70, 55);
	glVertex2i(70, 30);

	glVertex2i(80, 55);
	glVertex2i(80, 20);

	glEnd();

	// upper horizontal polygon for swastik

	glColor3f(0.8f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(45, 70);
	glVertex2i(70, 70);

	glVertex2i(45, 80);
	glVertex2i(80, 80);

	glVertex2i(45, 80);
	glVertex2i(45, 70);

	glVertex2i(80, 80);
	glVertex2i(70, 70);

	glEnd();
	
	// 3D right polygons of swastik

	glColor3f(0.4f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(30, 70);
	glVertex2i(35, 73);

	glVertex2i(20, 80);
	glVertex2i(25, 84);

	glVertex2i(20, 80);
	glVertex2i(30, 70);

	glVertex2i(25, 84);
	glVertex2i(35, 73);

	glEnd();

	glColor3f(0.4f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(30, 55);
	glVertex2i(35, 60);

	glVertex2i(30, 70);
	glVertex2i(35, 73);

	glVertex2i(30, 55);
	glVertex2i(30, 70);

	glVertex2i(35, 73);
	glVertex2i(35, 60);

	glEnd();

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(30, 55);
	glVertex2i(45, 55);

	glVertex2i(35, 60);
	glVertex2i(45, 60);

	glVertex2i(35, 60);
	glVertex2i(30, 55);

	glVertex2i(45, 60);
	glVertex2i(45, 55);

	glEnd();

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(30, 30);
	glVertex2i(45, 30);

	glVertex2i(35, 35);
	glVertex2i(45, 35);

	glVertex2i(35, 35);
	glVertex2i(30, 30);

	glVertex2i(45, 35);
	glVertex2i(45, 30);

	glEnd();

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(55, 55);
	glVertex2i(60, 60);

	glVertex2i(60, 70);
	glVertex2i(55, 70);

	glVertex2i(60, 70);
	glVertex2i(55, 55);

	glVertex2i(55, 70);
	glVertex2i(60, 60);

	glEnd();

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(55, 55);
	glVertex2i(80, 55);

	glVertex2i(60, 60);
	glVertex2i(83, 60);

	glVertex2i(60, 60);
	glVertex2i(55, 55);

	glVertex2i(83, 60);
	glVertex2i(80, 55);

	glEnd();

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(55, 20);
	glVertex2i(60, 25);

	glVertex2i(55, 45);
	glVertex2i(60, 45);

	glVertex2i(55, 45);
	glVertex2i(55, 20);

	glVertex2i(60, 45);
	glVertex2i(60, 25);

	glEnd();

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(80, 20);
	glVertex2i(83, 25);

	glVertex2i(80, 55);
	glVertex2i(83, 60);

	glVertex2i(80, 55);
	glVertex2i(80, 20);

	glVertex2i(83, 60);
	glVertex2i(83, 25);

	glEnd();

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(45, 80);
	glVertex2i(80, 80);

	glVertex2i(50, 85);
	glVertex2i(85, 85);

	glVertex2i(50, 85);
	glVertex2i(45, 80);

	glVertex2i(85, 85);
	glVertex2i(80, 80);

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
	glutCreateWindow("Flag");

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
