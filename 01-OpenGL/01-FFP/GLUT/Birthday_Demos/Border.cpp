//headr files 
#include<GL/freeglut.h>
#include<math.h>
#define PI 3.142

//global variables
bool bFullScreen = true;
float x_position1;
float y_position1;
float x_position2;
float y_position2;

//entry point function
int main(int argc, char* argv[])
{
	//local function declaration
	void initialize(void);
	void resize(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);
	void DrawBorder(void);

	//code
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sanket S. Pawar");

	initialize();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();

	return 0;
}

void initialize(void)
{
	//code 
	glClearColor(1.0f, 1.0f, 0.5f, 0.0f);
}

void resize(int width, int height)
{
	//code 
	glViewport(0, 0, width, height);
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int triangleAmount = 200; 

	GLfloat twicePi = 2.0f * PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			x + (radius * cos(i * twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
		);
	}
	glEnd();
}

void DrawBorder()
{
	glColor3f(0, 0, 0);
	for (x_position1 = -0.95; x_position1 < 1.0; x_position1 += 0.10)
	{
		drawFilledCircle(x_position1, 0.95, 0.009);
	}

	for (x_position2 = -0.95; x_position2 < 1.0; x_position2 += 0.10)
	{
		drawFilledCircle(x_position2, -0.95, 0.009);
	}

	for (y_position1 = -0.85; y_position1 < 0.95; y_position1 += 0.10)
	{
		drawFilledCircle(-0.95, y_position1, 0.009);
	}
	
	for (y_position2 = -0.85; y_position2 < 0.95; y_position2 += 0.10)
	{
		drawFilledCircle(0.95, y_position2, 0.008);
	}
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-0.95, 0.90);
	glVertex2f(-0.90, 0.85);
	glVertex2f(-0.90, 0.85);
	glVertex2f(-0.95, 0.80);

	glVertex2f(-0.95, 0.80);
	glVertex2f(-0.90, 0.75);
	glVertex2f(-0.90, 0.75);
	glVertex2f(-0.95, 0.70);

	glVertex2f(-0.95, 0.70);
	glVertex2f(-0.90, 0.65);
	glVertex2f(-0.90, 0.65);
	glVertex2f(-0.95, 0.60);

	glVertex2f(-0.95, 0.60);
	glVertex2f(-0.90, 0.55);
	glVertex2f(-0.90, 0.55);
	glVertex2f(-0.95, 0.50);

	glVertex2f(-0.95, 0.50);
	glVertex2f(-0.90, 0.45);
	glVertex2f(-0.90, 0.45);
	glVertex2f(-0.95, 0.40);

	glVertex2f(-0.95, 0.40);
	glVertex2f(-0.90, 0.35);
	glVertex2f(-0.90, 0.35);
	glVertex2f(-0.95, 0.30);

	glVertex2f(-0.95, 0.30);
	glVertex2f(-0.90, 0.25);
	glVertex2f(-0.90, 0.25);
	glVertex2f(-0.95, 0.20);

	glVertex2f(-0.95, 0.20);
	glVertex2f(-0.90, 0.15);
	glVertex2f(-0.90, 0.15);
	glVertex2f(-0.95, 0.10);

	glVertex2f(-0.95, 0.10);
	glVertex2f(-0.90, 0.05);
	glVertex2f(-0.90, 0.05);
	glVertex2f(-0.95, 0.00);

	glVertex2f(-0.95, -0.00);
	glVertex2f(-0.90, -0.05);
	glVertex2f(-0.90, -0.05);
	glVertex2f(-0.95, -0.10);

	glVertex2f(-0.95, -0.10);
	glVertex2f(-0.90, -0.15);
	glVertex2f(-0.90, -0.15);
	glVertex2f(-0.95, -0.20);

	glVertex2f(-0.95, -0.20);
	glVertex2f(-0.90, -0.25);
	glVertex2f(-0.90, -0.25);
	glVertex2f(-0.95, -0.30);

	glVertex2f(-0.95, -0.30);
	glVertex2f(-0.90, -0.35);
	glVertex2f(-0.90, -0.35);
	glVertex2f(-0.95, -0.40);

	glVertex2f(-0.95, -0.40);
	glVertex2f(-0.90, -0.45);
	glVertex2f(-0.90, -0.45);
	glVertex2f(-0.95, -0.50);

	glVertex2f(-0.95, -0.50);
	glVertex2f(-0.90, -0.55);
	glVertex2f(-0.90, -0.55);
	glVertex2f(-0.95, -0.60);

	glVertex2f(-0.95, -0.60);
	glVertex2f(-0.90, -0.65);
	glVertex2f(-0.90, -0.65);
	glVertex2f(-0.95, -0.70);

	glVertex2f(-0.95, -0.70);
	glVertex2f(-0.90, -0.75);
	glVertex2f(-0.90, -0.75);
	glVertex2f(-0.95, -0.80);

	glVertex2f(-0.95, -0.80);
	glVertex2f(-0.90, -0.85);
	glVertex2f(-0.90, -0.85);
	glVertex2f(-0.95, -0.90);
	glEnd();

	/////////////////////////////////
	
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.95, 0.90);
	glVertex2f(0.90, 0.85);
	glVertex2f(0.90, 0.85);
	glVertex2f(0.95, 0.80);

	glVertex2f(0.95, 0.80);
	glVertex2f(0.90, 0.75);
	glVertex2f(0.90, 0.75);
	glVertex2f(0.95, 0.70);

	glVertex2f(0.95, 0.70);
	glVertex2f(0.90, 0.65);
	glVertex2f(0.90, 0.65);
	glVertex2f(0.95, 0.60);

	glVertex2f(0.95, 0.60);
	glVertex2f(0.90, 0.55);
	glVertex2f(0.90, 0.55);
	glVertex2f(0.95, 0.50);

	glVertex2f(0.95, 0.50);
	glVertex2f(0.90, 0.45);
	glVertex2f(0.90, 0.45);
	glVertex2f(0.95, 0.40);

	glVertex2f(0.95, 0.40);
	glVertex2f(0.90, 0.35);
	glVertex2f(0.90, 0.35);
	glVertex2f(0.95, 0.30);

	glVertex2f(0.95, 0.30);
	glVertex2f(0.90, 0.25);
	glVertex2f(0.90, 0.25);
	glVertex2f(0.95, 0.20);

	glVertex2f(0.95, 0.20);
	glVertex2f(0.90, 0.15);
	glVertex2f(0.90, 0.15);
	glVertex2f(0.95, 0.10);

	glVertex2f(0.95, 0.10);
	glVertex2f(0.90, 0.05);
	glVertex2f(0.90, 0.05);
	glVertex2f(0.95, 0.00);

	glVertex2f(0.95, -0.00);
	glVertex2f(0.90, -0.05);
	glVertex2f(0.90, -0.05);
	glVertex2f(0.95, -0.10);

	glVertex2f(0.95, -0.10);
	glVertex2f(0.90, -0.15);
	glVertex2f(0.90, -0.15);
	glVertex2f(0.95, -0.20);

	glVertex2f(0.95, -0.20);
	glVertex2f(0.90, -0.25);
	glVertex2f(0.90, -0.25);
	glVertex2f(0.95, -0.30);

	glVertex2f(0.95, -0.30);
	glVertex2f(0.90, -0.35);
	glVertex2f(0.90, -0.35);
	glVertex2f(0.95, -0.40);

	glVertex2f(0.95, -0.40);
	glVertex2f(0.90, -0.45);
	glVertex2f(0.90, -0.45);
	glVertex2f(0.95, -0.50);

	glVertex2f(0.95, -0.50);
	glVertex2f(0.90, -0.55);
	glVertex2f(0.90, -0.55);
	glVertex2f(0.95, -0.60);

	glVertex2f(0.95, -0.60);
	glVertex2f(0.90, -0.65);
	glVertex2f(0.90, -0.65);
	glVertex2f(0.95, -0.70);

	glVertex2f(0.95, -0.70);
	glVertex2f(0.90, -0.75);
	glVertex2f(0.90, -0.75);
	glVertex2f(0.95, -0.80);

	glVertex2f(0.95, -0.80);
	glVertex2f(0.90, -0.85);
	glVertex2f(0.90, -0.85);
	glVertex2f(0.95, -0.90);
	glEnd();
	/////////////////////////////////

	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-0.95, 0.90);
	glVertex2f(-0.90, 0.95);
	glVertex2f(-0.90, 0.95);
	glVertex2f(-0.85, 0.90);
	
	glVertex2f(-0.85, 0.90);
	glVertex2f(-0.80, 0.95);
	glVertex2f(-0.80, 0.95);
	glVertex2f(-0.75, 0.90);
	
	glVertex2f(-0.75, 0.90);
	glVertex2f(-0.70, 0.95);
	glVertex2f(-0.70, 0.95);
	glVertex2f(-0.65, 0.90);

	glVertex2f(-0.65, 0.90);
	glVertex2f(-0.60, 0.95);
	glVertex2f(-0.60, 0.95);
	glVertex2f(-0.55, 0.90);

	glVertex2f(-0.55, 0.90);
	glVertex2f(-0.50, 0.95);
	glVertex2f(-0.50, 0.95);
	glVertex2f(-0.45, 0.90);

	glVertex2f(-0.45, 0.90);
	glVertex2f(-0.40, 0.95);
	glVertex2f(-0.40, 0.95);
	glVertex2f(-0.35, 0.90);

	glVertex2f(-0.35, 0.90);
	glVertex2f(-0.30, 0.95);
	glVertex2f(-0.30, 0.95);
	glVertex2f(-0.25, 0.90);

	glVertex2f(-0.25, 0.90);
	glVertex2f(-0.20, 0.95);
	glVertex2f(-0.20, 0.95);
	glVertex2f(-0.15, 0.90);

	glVertex2f(-0.15, 0.90);
	glVertex2f(-0.10, 0.95);
	glVertex2f(-0.10, 0.95);
	glVertex2f(-0.05, 0.90);

	glVertex2f(-0.05, 0.90);
	glVertex2f(0.00, 0.95);
	glVertex2f(0.00, 0.95);
	glVertex2f(0.05, 0.90);
	
	glVertex2f(0.05, 0.90);
	glVertex2f(0.10, 0.95);
	glVertex2f(0.10, 0.95);
	glVertex2f(0.15, 0.90);

	glVertex2f(0.15, 0.90);
	glVertex2f(0.20, 0.95);
	glVertex2f(0.20, 0.95);
	glVertex2f(0.25, 0.90);

	glVertex2f(0.25, 0.90);
	glVertex2f(0.30, 0.95);
	glVertex2f(0.30, 0.95);
	glVertex2f(0.35, 0.90);

	glVertex2f(0.35, 0.90);
	glVertex2f(0.40, 0.95);
	glVertex2f(0.40, 0.95);
	glVertex2f(0.45, 0.90);

	glVertex2f(0.45, 0.90);
	glVertex2f(0.50, 0.95);
	glVertex2f(0.50, 0.95);
	glVertex2f(0.55, 0.90);

	glVertex2f(0.55, 0.90);
	glVertex2f(0.60, 0.95);
	glVertex2f(0.60, 0.95);
	glVertex2f(0.65, 0.90);

	glVertex2f(0.65, 0.90);
	glVertex2f(0.70, 0.95);
	glVertex2f(0.70, 0.95);
	glVertex2f(0.75, 0.90);

	glVertex2f(0.75, 0.90);
	glVertex2f(0.80, 0.95);
	glVertex2f(0.80, 0.95);
	glVertex2f(0.85, 0.90);

	glVertex2f(0.85, 0.90);
	glVertex2f(0.90, 0.95);
	glVertex2f(0.90, 0.95);
	glVertex2f(0.95, 0.90);
	glEnd();

	/////////////////////////////////////////////

	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-0.95, -0.90);
	glVertex2f(-0.90, -0.95);
	glVertex2f(-0.90, -0.95);
	glVertex2f(-0.85, -0.90);

	glVertex2f(-0.85, -0.90);
	glVertex2f(-0.80, -0.95);
	glVertex2f(-0.80, -0.95);
	glVertex2f(-0.75, -0.90);

	glVertex2f(-0.75, -0.90);
	glVertex2f(-0.70, -0.95);
	glVertex2f(-0.70, -0.95);
	glVertex2f(-0.65, -0.90);

	glVertex2f(-0.65, -0.90);
	glVertex2f(-0.60, -0.95);
	glVertex2f(-0.60, -0.95);
	glVertex2f(-0.55, -0.90);

	glVertex2f(-0.55, -0.90);
	glVertex2f(-0.50, -0.95);
	glVertex2f(-0.50, -0.95);
	glVertex2f(-0.45, -0.90);

	glVertex2f(-0.45, -0.90);
	glVertex2f(-0.40, -0.95);
	glVertex2f(-0.40, -0.95);
	glVertex2f(-0.35, -0.90);

	glVertex2f(-0.35, -0.90);
	glVertex2f(-0.30, -0.95);
	glVertex2f(-0.30, -0.95);
	glVertex2f(-0.25, -0.90);

	glVertex2f(-0.25, -0.90);
	glVertex2f(-0.20, -0.95);
	glVertex2f(-0.20, -0.95);
	glVertex2f(-0.15, -0.90);

	glVertex2f(-0.15, -0.90);
	glVertex2f(-0.10, -0.95);
	glVertex2f(-0.10, -0.95);
	glVertex2f(-0.05, -0.90);

	glVertex2f(-0.05, -0.90);
	glVertex2f(0.00, -0.95);
	glVertex2f(0.00, -0.95);
	glVertex2f(0.05, -0.90);

	glVertex2f(0.05, -0.90);
	glVertex2f(0.10, -0.95);
	glVertex2f(0.10, -0.95);
	glVertex2f(0.15, -0.90);

	glVertex2f(0.15, -0.90);
	glVertex2f(0.20, -0.95);
	glVertex2f(0.20, -0.95);
	glVertex2f(0.25, -0.90);

	glVertex2f(0.25, -0.90);
	glVertex2f(0.30, -0.95);
	glVertex2f(0.30, -0.95);
	glVertex2f(0.35, -0.90);

	glVertex2f(0.35, -0.90);
	glVertex2f(0.40, -0.95);
	glVertex2f(0.40, -0.95);
	glVertex2f(0.45, -0.90);

	glVertex2f(0.45, -0.90);
	glVertex2f(0.50, -0.95);
	glVertex2f(0.50, -0.95);
	glVertex2f(0.55, -0.90);

	glVertex2f(0.55, -0.90);
	glVertex2f(0.60, -0.95);
	glVertex2f(0.60, -0.95);
	glVertex2f(0.65, -0.90);

	glVertex2f(0.65, -0.90);
	glVertex2f(0.70, -0.95);
	glVertex2f(0.70, -0.95);
	glVertex2f(0.75, -0.90);

	glVertex2f(0.75, -0.90);
	glVertex2f(0.80, -0.95);
	glVertex2f(0.80, -0.95);
	glVertex2f(0.85, -0.90);

	glVertex2f(0.85, -0.90);
	glVertex2f(0.90, -0.95);
	glVertex2f(0.90, -0.95);
	glVertex2f(0.95, -0.90);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	DrawBorder();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	//code
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':
		if (bFullScreen == false)
		{
			glutFullScreen();
			bFullScreen = true;
			break;
		}
		else
		{
			glutLeaveFullScreen();
			bFullScreen = false;
			break;
		}
	default:
		break;
	}
}
void mouse(int button, int state, int x, int y)
{
	//code 
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
}

void uninitialize(void)
{
	//code
}
