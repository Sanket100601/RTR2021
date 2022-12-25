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
	gluOrtho2D(0.0, 110.0, 0.0, 110.0);
}

// Function to display 
void myDisplay(void)
{
	// Clear the screen buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Clear the screen buffer
	glClear(GL_COLOR_BUFFER_BIT);

	/////// P ///////

	glColor3f(0.8f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(10, 40);
	glVertex2i(15, 40);

	glVertex2i(10, 60);
	glVertex2i(15, 60);

	glVertex2i(10, 60);
	glVertex2i(10, 40);

	glVertex2i(15, 60);
	glVertex2i(15, 40);

	glEnd();

	glColor3f(0.8f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(15, 50);
	glVertex2i(20, 50);

	glVertex2i(15, 60);
	glVertex2i(20, 60);

	glVertex2i(15, 60);
	glVertex2i(15, 50);

	glVertex2i(20, 60);
	glVertex2i(20, 50);

	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(15, 52);
	glVertex2i(18, 52);

	glVertex2i(15, 58);
	glVertex2i(18, 58);

	glVertex2i(15, 58);
	glVertex2i(15, 52);

	glVertex2i(18, 58);
	glVertex2i(18, 52);

	glEnd();

	glColor3f(0.4f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(10, 60);
	glVertex2i(20, 60);

	glVertex2i(12, 63);
	glVertex2i(22, 63);

	glVertex2i(12, 63);
	glVertex2i(10, 60);

	glVertex2i(22, 63);
	glVertex2i(20, 60);

	glEnd();

	glColor3f(0.4f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(20, 50);
	glVertex2i(22, 53);

	glVertex2i(20, 60);
	glVertex2i(22, 63);

	glVertex2i(20, 60);
	glVertex2i(20, 50);

	glVertex2i(22, 63);
	glVertex2i(22, 53);

	glEnd();

	glColor3f(0.4f, 0.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(15, 40);
	glVertex2i(17, 43);

	glVertex2i(15, 50);
	glVertex2i(17, 50);

	glVertex2i(15, 50);
	glVertex2i(15, 40);

	glVertex2i(17, 50);
	glVertex2i(17, 43);

	glEnd();

	/////// I ///////

	glColor3f(0.0f, 0.8f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(25, 30);
	glVertex2i(30, 30);

	glVertex2i(25, 70);
	glVertex2i(30, 70);

	glVertex2i(25, 70);
	glVertex2i(25, 30);

	glVertex2i(30, 70);
	glVertex2i(30, 30);

	glEnd();

	glColor3f(0.0f, 0.8f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(15, 65);
	glVertex2i(40, 65);

	glVertex2i(15, 70);
	glVertex2i(40, 70);

	glVertex2i(15, 70);
	glVertex2i(15, 65);

	glVertex2i(40, 70);
	glVertex2i(40, 65);

	glEnd();

	glColor3f(0.0f, 0.8f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(15, 30);
	glVertex2i(40, 30);

	glVertex2i(15, 35);
	glVertex2i(40, 35);

	glVertex2i(15, 35);
	glVertex2i(15, 30);

	glVertex2i(40, 35);
	glVertex2i(40, 30);

	glEnd();


	glColor3f(0.0f, 0.4f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(15, 70);
	glVertex2i(40, 70);

	glVertex2i(17, 73);
	glVertex2i(42, 73);

	glVertex2i(17, 73);
	glVertex2i(15, 70);

	glVertex2i(42, 73);
	glVertex2i(40, 70);

	glEnd();

	glColor3f(0.0f, 0.4f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(40, 65);
	glVertex2i(42, 68);

	glVertex2i(40, 70);
	glVertex2i(42, 73);

	glVertex2i(40, 70);
	glVertex2i(40, 65);

	glVertex2i(42, 73);
	glVertex2i(42, 68);

	glEnd();

	glColor3f(0.0f, 0.4f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(30, 35);
	glVertex2i(32, 38);

	glVertex2i(30, 65);
	glVertex2i(32, 65);

	glVertex2i(30, 65);
	glVertex2i(30, 35);

	glVertex2i(32, 65);
	glVertex2i(32, 38);

	glEnd();

	glColor3f(0.0f, 0.4f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(30, 35);
	glVertex2i(40, 35);

	glVertex2i(32, 38);
	glVertex2i(42, 38);

	glVertex2i(32, 38);
	glVertex2i(30, 35);

	glVertex2i(42, 38);
	glVertex2i(40, 35);

	glEnd();

	glColor3f(0.0f, 0.4f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(40, 30);
	glVertex2i(42, 33);

	glVertex2i(40, 35);
	glVertex2i(42, 38);

	glVertex2i(40, 35);
	glVertex2i(40, 30);

	glVertex2i(42, 38);
	glVertex2i(42, 33);

	glEnd();

	/////// X ////////

	glColor3f(0.0f, 0.0f, 0.8f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(40, 40);
	glVertex2i(45, 40);

	glVertex2i(50, 60);
	glVertex2i(55, 60);

	glVertex2i(50, 60);
	glVertex2i(40, 40);

	glVertex2i(55, 60);
	glVertex2i(45, 40);

	glEnd();


	glColor3f(0.0f, 0.0f, 0.8f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(50, 40);
	glVertex2i(55, 40);

	glVertex2i(40, 60);
	glVertex2i(45, 60);

	glVertex2i(40, 60);
	glVertex2i(50, 40);

	glVertex2i(45, 60);
	glVertex2i(55, 40);

	glEnd();

	glColor3f(0.0f, 0.0f, 0.4f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(40, 60);
	glVertex2i(45, 60);

	glVertex2i(42, 62);
	glVertex2i(47, 62);

	glVertex2i(42, 62);
	glVertex2i(40, 60);

	glVertex2i(47, 62);
	glVertex2i(45, 60);

	glEnd();

	glColor3f(0.0f, 0.0f, 0.4f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2f(47.2, 55);
	glVertex2i(49, 58);

	glVertex2i(45, 60);
	glVertex2i(47, 62);

	glVertex2i(45, 60);
	glVertex2f(47.2, 55);

	glVertex2i(47, 62);
	glVertex2i(49, 58);

	glEnd();

	glColor3f(0.0f, 0.0f, 0.4f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(55, 40);
	glVertex2f(57, 43);

	glVertex2i(50, 50);
	glVertex2i(52, 53);

	glVertex2i(50, 50);
	glVertex2i(55, 40);

	glVertex2i(52, 53);
	glVertex2i(57, 43);

	glEnd();

	glColor3f(0.0f, 0.0f, 0.4f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(50, 60);
	glVertex2f(55, 60);

	glVertex2i(52, 62);
	glVertex2i(57, 62);

	glVertex2i(52, 62);
	glVertex2i(50, 60);

	glVertex2i(57, 62);
	glVertex2i(55, 60);

	glEnd();

	//////////// E ///////////////

	glColor3f(255.0f, 255.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(60, 30);
	glVertex2f(65, 30);

	glVertex2i(60, 70);
	glVertex2i(65, 70);

	glVertex2i(60, 70);
	glVertex2i(60, 30);

	glVertex2i(65, 70);
	glVertex2i(65, 30);

	glEnd();

	glColor3f(255.0f, 255.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(65, 65);
	glVertex2f(80, 65);

	glVertex2i(65, 70);
	glVertex2i(80, 70);

	glVertex2i(65, 70);
	glVertex2i(65, 65);

	glVertex2i(80, 70);
	glVertex2i(80, 65);

	glEnd();

	glColor3f(255.0f, 255.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(65, 48);
	glVertex2f(75, 48);

	glVertex2i(65, 53);
	glVertex2i(75, 53);

	glVertex2i(65, 53);
	glVertex2i(65, 48);

	glVertex2i(75, 53);
	glVertex2i(75, 48);

	glEnd();

	glColor3f(255.0f, 255.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(65, 30);
	glVertex2f(80, 30);

	glVertex2i(65, 35);
	glVertex2i(80, 35);

	glVertex2i(65, 35);
	glVertex2i(65, 30);

	glVertex2i(80, 35);
	glVertex2i(80, 30);

	glEnd();

	glColor3f(0.6f,0.6f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(60, 70);
	glVertex2f(80, 70);

	glVertex2i(63, 72);
	glVertex2i(83, 72);

	glVertex2i(63, 72);
	glVertex2i(60, 70);

	glVertex2i(83, 72);
	glVertex2i(80, 70);

	glEnd();

	glColor3f(0.6f, 0.6f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(80, 65);
	glVertex2f(83, 67);

	glVertex2i(80, 70);
	glVertex2i(83, 72);

	glVertex2i(80, 70);
	glVertex2i(80, 65);

	glVertex2i(83, 72);
	glVertex2i(83, 67);

	glEnd();

	glColor3f(0.6f, 0.6f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(65, 53);
	glVertex2f(67, 55);

	glVertex2i(65, 65);
	glVertex2i(67, 65);

	glVertex2i(65, 65);
	glVertex2i(65, 53);

	glVertex2i(67, 65);
	glVertex2i(67, 55);

	glEnd();

	glColor3f(0.6f, 0.6f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(65, 53);
	glVertex2f(75, 53);

	glVertex2i(67, 55);
	glVertex2i(77, 55);

	glVertex2i(67, 55);
	glVertex2i(65, 53);

	glVertex2i(77, 55);
	glVertex2i(75, 53);

	glEnd();

	glColor3f(0.6f, 0.6f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(75, 48);
	glVertex2f(77, 50);

	glVertex2i(75, 53);
	glVertex2i(77, 55);

	glVertex2i(75, 53);
	glVertex2i(75, 48);

	glVertex2i(77, 55);
	glVertex2i(77, 50);

	glEnd();

	glColor3f(0.6f, 0.6f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(65, 35);
	glVertex2f(67, 37);

	glVertex2i(65, 48);
	glVertex2i(67, 48);

	glVertex2i(65, 48);
	glVertex2i(65, 35);

	glVertex2i(67, 48);
	glVertex2i(67, 37);

	glEnd();

	glColor3f(0.6f, 0.6f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(65, 35);
	glVertex2f(80, 35);

	glVertex2i(67, 37);
	glVertex2i(82, 37);

	glVertex2i(67, 37);
	glVertex2i(65, 35);

	glVertex2i(82, 37);
	glVertex2i(80, 35);

	glEnd();


	glColor3f(0.6f, 0.6f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(80, 30);
	glVertex2f(82, 32);

	glVertex2i(80, 37);
	glVertex2i(82, 37);

	glVertex2i(80, 37);
	glVertex2i(80, 30);

	glVertex2i(82, 37);
	glVertex2i(82, 32);

///////////////////////////////////////// L ///////////////////////////////

	glColor3f(0.0f, 1.0f, 0.0f);

	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(85, 40);
	glVertex2i(90, 40);

	glVertex2i(85, 60);
	glVertex2i(90, 60);

	glVertex2i(85, 60);
	glVertex2i(85, 40);

	glVertex2i(90, 60);
	glVertex2i(90, 40);

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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	// Sets the display mode and
	// specify the colour scheme
	glutInitWindowSize(800, 600);

	// Specify the window size
	glutInitWindowPosition(0, 0);

	// Sets the starting position
	// for the window
	glutCreateWindow("1 View Perspective Of Group Name");

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
