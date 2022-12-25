#include <GL\glut.h>
#include <iostream>
#include <windows.h>
#define GL_PI 3.14159

float x, y, angle;
float y_position1 = 0;
float y_position2 = 10;
float y_position3 = 20;
float y_position4 = 15;

// Function to initialize the drivers
void myInit(void)
{
	// Clear all the screen color
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Sets background color to black
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	// Specify the display area
	gluOrtho2D(0.0, 110.0, 0.0, 110.0);
}

void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
	
	if (y_position1 <= 80)
	{	
		y_position1 += 0.15;
	}
	if (y_position2 <= 90)
	{
		y_position2 += 0.15;
	}
	if (y_position3 <= 100)
	{
		y_position3 += 0.15;
	}
	if (y_position4 <= 95)
	{
		y_position4 += 0.15;
	}
}

// Function to display 
void myDisplay(void)
{
	// Clear the screen buffer
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 1.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2f(40, y_position1);
	glVertex2f(70, y_position1);
	glVertex2f(70, y_position1 + 10);
	glVertex2f(40, y_position1 + 10);

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2f(40, y_position2);
	glVertex2f(70, y_position2);
	glVertex2f(70, y_position2 + 10);
	glVertex2f(40, y_position2 + 10);

	glEnd();

	glColor3f(1.5f, 0.5f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2f(40, y_position3);
	glVertex2f(70, y_position3);
	glVertex2f(70, y_position3 + 10);
	glVertex2f(40, y_position3 + 10);

	glEnd();


	glColor3f(1.0f, 1.0f, 1.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2f(38, 0);
	glVertex2f(40, 0);
	glVertex2f(40, 110);
	glVertex2f(38, 110);

	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POINTS);
	for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
	{
		x = 55 + 5 * sin(angle);
		y = y_position4 + 5 * cos(angle);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	for (angle = 0.0; angle <= (2.0 * GL_PI); angle += 0.262f)
	{
		glVertex2f(55, y_position4);
		glVertex2f(5 * cos(angle) + 55, 5 * sin(angle) + y_position4);
	}
	glEnd();

	glutSwapBuffers();
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

	glutTimerFunc(1000, timer, 0);

	// Additional initializations
	glutMainLoop();

	// Go into a loop until event
	// occurs
	return 0;
}