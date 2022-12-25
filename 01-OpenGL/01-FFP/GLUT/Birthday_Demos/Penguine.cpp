//headr files 
#include<GL/freeglut.h>
#include<math.h>
#define DEG2RAD 3.14159/180.0
#define PI 3.14159

//global variables
bool bFullScreen = false;

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
	void DrawEllipse(float, float);
	void penguine(void);
	
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
	glColor3f(1.0, 1.0, 1.0);
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

void drawFilledCircle2(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int triangleAmount = 200;

	GLfloat twicePi = 2.0f * PI;
	glColor3f(0.0, 0.0, 0.0);
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

void drawFilledCircle3(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int triangleAmount = 200;

	GLfloat twicePi = 2.0f * PI;
	glColor3f(1.0, 0.5, 0.0);
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

void  DrawEllipse(float xradius, float yradius)
{
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < 360; i++)
	{
		// Convert degree into radians
		float degInRad = i * DEG2RAD;
		glVertex2f(cos(degInRad) * xradius, sin(degInRad) * yradius);
	}
	glEnd();
}

void penguine()
{
	DrawEllipse(0.3,0.7);
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex2f(-0.1,-0.5);
	glVertex2f(0.1,-0.5);
	glVertex2f(0.2,0.2);
	glVertex2f(-0.2,0.2);
	glEnd();

	drawFilledCircle(-0.1, 0.4, 0.08);
	drawFilledCircle(0.1, 0.4, 0.08);

	drawFilledCircle2(-0.1, 0.4, 0.04);
	drawFilledCircle2(0.1, 0.4, 0.04);

	glBegin(GL_TRIANGLES);
	glColor3f(1, 0.5, 0);
	glVertex2f(-0.06, 0.32);
	glVertex2f(0.06, 0.32);
	glVertex2f(0.0, 0.0);
	glEnd();

	drawFilledCircle3(-0.15, -0.65, 0.1);
	drawFilledCircle3(0.15, -0.65, 0.1);

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5, 0.0);
	glVertex2f(-0.2, 0.0);
	glVertex2f(-0.2, 0.2);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.5, 0.0);
	glVertex2f(0.2, 0.0);
	glVertex2f(0.2, 0.2);
	glEnd();
}

void display()
{
	//code 
	glClear(GL_COLOR_BUFFER_BIT);

	penguine();

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