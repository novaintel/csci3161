
/************************************************************************************

File: 			simple.c

Description:	A complete OpenGL program to draw a square on the screen.

Author:			Stephen Brooks

*************************************************************************************/


/* include the library header files */
#include <gl/freeglut.h>



/************************************************************************

Function:		myDisplay

Description:	Displays a black square on a black background.

*************************************************************************/
void myDisplay(void)
{
	// clear the screen 
	glClear(GL_COLOR_BUFFER_BIT);
	// Left
	glBegin(GL_POLYGON);
		glColor3f(0, 0, 1);
		glVertex2f(0, 1);
		glVertex2f(0, 0.4);
		glVertex2f(0.2, 0.6);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(0, 1, 0);
		glVertex2f(0, 1);
		glVertex2f(0.2, 1);
		glVertex2f(0.2, 0.6);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(1, 0, 0);
		glVertex2f(0.2, 0.6);
		glVertex2f(0, 0.4);
		glVertex2f(0.2, 0);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(1, 1, 0);
		glVertex2f(0, 0);
		glVertex2f(0, 0.4);
		glVertex2f(0.2, 0);
	glEnd();

	//Right
	glBegin(GL_POLYGON);
		glColor3f(0, 0, 1);
		glVertex2f(0.2, 0.4);
		glVertex2f(0.2, 0.6);
		glVertex2f(0.5, 0.4);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(0, 1, 0);
		glVertex2f(0.2, 0.6);
		glVertex2f(0.5, 0.4);
		glVertex2f(0.8, 0.6);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(1, 0, 0);
		glVertex2f(0.5, 0.4);
		glVertex2f(0.8, 0.6);
		glVertex2f(0.8, 0.4);
	glEnd();

	// send all output to display 
	glFlush();
}



/************************************************************************

Function:		main

Description:	Sets up the openGL rendering context and the windowing
system, then begins the display loop.

*************************************************************************/
void main(int argc, char** argv)
{

	// initialize the toolkit
	glutInit(&argc, argv);


	// open the screen window
	glutCreateWindow("simple");
	// register redraw function
	glutDisplayFunc(myDisplay);

	// go into a perpetual loop
	glutMainLoop();
}


