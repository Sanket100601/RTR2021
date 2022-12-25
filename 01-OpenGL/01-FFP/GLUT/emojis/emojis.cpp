#include <gl/freeglut.h>
#include <math.h>


void display()
{
	float x1, y1, x2, y2;
	float angle;
    double radius = 4;

	//1
	x1 = -25, y1 = 18;
	glColor3f(1.0, 1.0, 0.6);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	//2
	x1 = -16, y1 = 18;
	glColor3f(1.0, 1.0, 0.6);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();


	// 3
	x1 = -7, y1 = 18;
	glColor3f(1.0, 1.0, 0.6);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 4
	x1 = 2; y1 = 18;
	glColor3f(1.0, 1.0, 0.6);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 5
	x1 = 11; y1 = 18;
	glColor3f(1.0, 1.0, 0.6);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	//6
	x1 = 20; y1 = 18;
	glColor3f(1.0, 1.0, 0.6);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	//7
	x1 = -25; y1 = 0;
	glColor3f(1.0, 1.0, 0.6);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 8
	x1 = -16; y1 = 0;
	glColor3f(1.0, 1.0, 0.6);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 9
	x1 = -7; y1 = 0;
	glColor3f(1.0, 1.0, 0.6);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 10
	x1 = 2; y1 = 0;
	glColor3f(1.0, 1.0, 0.6);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 11
	x1 = 11; y1 = 0;
	glColor3f(1.0, 1.0, 0.6);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 12
	x1 = 20; y1 = 0;
	glColor3f(1.0, 1.0, 0.6);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 1-texture
	x1 = -27; y1 = 19;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();
	
	x1 = -23; y1 = 19;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 2-texture
	x1 = -18; y1 = 19;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	x1 = -14; y1 = 19;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 3-texture
	x1 = -9; y1 = 19;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	x1 = -5; y1 = 19;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 4-texture
	x1 = 0; y1 = 19;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	x1 = 4; y1 = 19;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 5 texture
	x1 = 13; y1 = 19;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	x1 = 9; y1 = 19;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 6-texture
	x1 = 18; y1 = 19;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	x1 = 22; y1 = 19;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 7-texture
	glBegin(GL_LINES);
	glVertex3f(-27, 1, 0);
	glVertex3f(-26, 1, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-24, 1, 0);
	glVertex3f(-23, 1, 0);
	glEnd();

	// 8-texture
	x1 = -18; y1 = 1;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	x1 = -14; y1 = 1;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 9-texture
	x1 = -9; y1 = 1;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	x1 = -5; y1 = 1;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 10-texture
	glBegin(GL_LINES);
	glVertex3f(0, 1, 0);
	glVertex3f(1, 0, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(4, 1, 0);
	glVertex3f(3, 0, 0);
	glEnd();


	// 11-texture
	x1 = 9; y1 = 1;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	x1 = 13; y1 = 1;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 12-texture
	x1 = 18; y1 = -1;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	x1 = 22; y1 = -1;
	radius = 0.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 1-reaction
	glBegin(GL_LINES);
	glVertex3f(-27, 17, 0);
	glVertex3f(-25, 15.5, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-23, 17, 0);
	glVertex3f(-25, 15.5, 0);
	glEnd();
	glFlush();

	// 2-reaction
	glBegin(GL_LINES);
	glVertex3f(-16, 17, 0);
	glVertex3f(-18, 15.5, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-16, 17, 0);
	glVertex3f(-14, 15.5, 0);
	glEnd();
	glFlush();
	
	// 3-reaction
	glBegin(GL_LINES);
	glVertex3f(-9, 16.5, 0);
	glVertex3f(-5, 16.5, 0);
	glEnd();

	// 4-REACTION
	x1 = 2, y1 = 16.5;
	radius = 1.5;
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	// 5-reaction
	glBegin(GL_LINES);
	glVertex3f(8, 20, 0);
	glVertex3f(10, 19, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(14, 20, 0);
	glVertex3f(12, 19, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(9, 17, 0);
	glVertex3f(11, 15.5, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(13, 17, 0);
	glVertex3f(11, 15.5, 0);
	glEnd();

	// 6-reaction
	glBegin(GL_LINES);
	glVertex3f(17, 20, 0);
	glVertex3f(19, 19, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(23, 20, 0);
	glVertex3f(21, 19, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(20, 17, 0);
	glVertex3f(18, 15.5, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(20, 17, 0);
	glVertex3f(22, 15.5, 0);
	glEnd();

	// 7-rection
	glBegin(GL_LINES);
	glVertex3f(-27, -1.5, 0);
	glVertex3f(-23, -1.5, 0);
	glEnd();

	// 8-reacction
	glBegin(GL_LINES);
	glVertex3f(-19, 1, 0);
	glVertex3f(-17, 2, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-13, 1, 0);
	glVertex3f(-15, 2, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-16, -1, 0);
	glVertex3f(-18, -2, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-16, -1, 0);
	glVertex3f(-14, -2, 0);
	glEnd();

	// 9-reaction
	glBegin(GL_LINES);
	glVertex3f(-10, 1, 0);
	glVertex3f(-8, 2, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-4, 1, 0);
	glVertex3f(-6, 2, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-9, -1, 0);
	glVertex3f(-7, -3, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-5, -1, 0);
	glVertex3f(-7, -3, 0);
	glEnd();

	// 10-reaction
	glBegin(GL_LINES);
	glVertex3f(0, -1, 0);
	glVertex3f(2, -3, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(4, -1, 0);
	glVertex3f(2, -3, 0);
	glEnd();

	// 12-reaction
	glBegin(GL_LINES);
	glVertex3f(20, 3, 0);
	glVertex3f(18, 1, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(20, 3, 0);
	glVertex3f(22, 1, 0);
	glEnd();

	glFlush();
}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Emojis");

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-32, 32, -24, 24);

	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}
