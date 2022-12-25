//headr files 
#include<GL/freeglut.h>

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
	void bird(void);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize(int width, int height)
{
	//code 
	glViewport(0, 0, width, height);
}

void bird()
{
	glColor3f(0, 0, 1);
	glBegin(GL_TRIANGLES);	//Wing of bird 
	glVertex2f(-0.7, -0.15);
	glVertex2f(0.2, 0.0);
	glVertex2f(0.2, 0.2);
	glEnd();
	
	glColor3f(0, 1, 0);
	glBegin(GL_TRIANGLES);	//Wing
	glVertex2f(-0.2, -0.07);
	glVertex2f(0.2, 0.0);
	glVertex2f(-0.2, 0.4);
	glEnd();
	
	glColor3f(0, 0.6, 0);
	glBegin(GL_POLYGON);	//wings
	glVertex2f(-0.3, 0.0);
	glVertex2f(-0.2, 0.04);
	glVertex2f(-0.2, 0.2);
	glVertex2f(-0.3, 0.3);
	glEnd();
	
	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES);	//face
	glVertex2f(0.2, 0.1);
	glVertex2f(0.3, 0.2);
	glVertex2f(0.2, 0.2);
	glEnd();
	
}

void display()
{
	//code 
	glClear(GL_COLOR_BUFFER_BIT);
	
	bird();

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