#include <GL\glut.h>
#include <iostream>
#include <windows.h>
#include<math.h>
#define GL_PI 3.14159
using namespace std;

// Function to initialize the drivers
void myInit(void)
{
    // Clear all the screen color
    glClearColor(0.0, 0.0, 0.0, 1.0);

    

    // Specify the display area
    gluOrtho2D(0.0, 400.0, 0.0, 400.0);
}

void myDisplay(void)
{
    // Clear the screen buffer
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat x, y, angle;
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 200+170.0f * sin(angle);
        y = 200+170.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 50 + 5.0f * sin(angle);
        y = 200 + 5.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 200 + 5.0f * sin(angle);
        y = 50 + 5.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 200 + 5.0f * sin(angle);
        y = 350 + 5.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 350 + 5.0f * sin(angle);
        y = 200 + 5.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 120 + 5.0f * sin(angle);
        y = 70 + 5.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 70 + 5.0f * sin(angle);
        y = 120 + 5.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 70 + 5.0f * sin(angle);
        y = 280 + 5.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 120 + 5.0f * sin(angle);
        y = 330 + 5.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 280+ 5.0f * sin(angle);
        y = 330 + 5.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 330 + 5.0f * sin(angle);
        y = 280 + 5.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 320 + 5.0f * sin(angle);
        y = 120 + 5.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glBegin(GL_POINTS);
    for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += 0.0001f)
    {
        x = 270 + 5.0f * sin(angle);
        y = 70 + 5.0f * cos(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(200.0f, 200.0f, 0.0f);
    glVertex3f(130.0f, 130.0f, 0.0f);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(200.0f, 200.0f, 0.0f);
    glVertex3f(310.0f, 140.0f, 0.0f);
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
    glutInitWindowSize(1200, 740);

    // Specify the window size
    glutInitWindowPosition(0, 0);

    // Sets the starting position
    // for the window
    glutCreateWindow("Sanket's Clock");

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