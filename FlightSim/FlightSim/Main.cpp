
#include <stdlib.h>
#include <GL\freeglut.h>
#include <stdarg.h>
#include <iostream>
#include <memory.h>
#include <time.h>
#include <fstream>
#include "BaseScene.h"
#include "objectReader.h"
using namespace std;

GLuint liliTexName;
// Default Image dimensions
int imageWidth = 640;
int imageHeight = 480;


//Callbacks
void Display();
void Idle();
void Reshape(int w, int h);
void Keyboard(unsigned char key, int, int);
void MouseButton(int button, int state, int x, int y);
void PassiveMouseMotion(int x, int y);

//font
GLvoid *font_style = GLUT_BITMAP_HELVETICA_12;

// a cleanup function
void quit(int i = 0);

void main(int argc, char **argv)
{
	// INITIALIZE THE GLUT WINDOW
	glutInit(&argc, argv);
	glutInitWindowSize(imageWidth, imageHeight);
	glutInitDisplayString("rgb double");
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Project #1");

	//SETUP GLUT CALLBACKS
	cout << "Setting up callbacks... ";
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	//glutMotionFunc(MouseMotion);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutPassiveMotionFunc(PassiveMouseMotion);
	cout << "[completed]\n";

	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_FLAT);
	glEnable(GL_LIGHT0);


	objectReaderInit();
	readFile("C:\\Users\\James\\Documents\\code\\School\\csci3161\\FlightSim\\cessna.txt");
	
	glutMainLoop();
}


// This function draws the scene
float angle = 0;
float zpos = 0;
float eyex = -10, eyey = 2, eyez, lookdirx = 10;
float lookdirz = -1;
float sidediry, sidedirz = 0;
float sidedirx = 1;
GLfloat viewMatrix[16];
void Display()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	if (angle>360)
		angle = 0;
	angle += .1;

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	//SIMPLE FIRST PERSON CAMERA**********************************************************************************
	// There are a lot of ways to make this camera better. This one is based on vector manipulation only and it works!
	//see PassiveMouseMotion() and Keyboard() also
	gluLookAt(eyex, eyey, eyez, eyex + lookdirx, eyey, eyez + lookdirz, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
	sidedirx = viewMatrix[0];
	sidediry = viewMatrix[4];
	sidedirz = viewMatrix[8];



/*	GLfloat light_position[] = { 0.0, 10.0, 10.0, 1.0 };
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_specular[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat light_diffuse[] = { 1, 1, 1, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);*/
	//glShadeModel(GL_SMOOTH);

	drawBase();
	
	glPushMatrix();
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glTranslatef(eyex + 3, eyey, eyez + lookdirz);
	drawPlane();
	glPopMatrix();

	// swap the buffers
	glutSwapBuffers();

}


// This function is continuously called when events are not being received
// by the window.  This is a good place to update the state of your objects 
// after every frame.
void Idle() {
	glutPostRedisplay();

}

// keyboard handler
float speed = .1;
void Keyboard(unsigned char key, int, int)
{
	switch (key)
	{
		// if q, exit
	case 'q':
		quit(1);
		break;
	case 'w': //move forward
		eyex += lookdirx*speed;
		eyez += lookdirz*speed;
		break;
	case 's'://move backward
		eyex -= lookdirx*speed;
		eyez -= lookdirz*speed;
		break;
	case 'a':
		break;
	case 'd':
		break;
	}
}

void MouseButton(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		zpos += 1; // moves some of the geometry
		break;
	case GLUT_MIDDLE_BUTTON:
		break;
	case GLUT_RIGHT_BUTTON:
		zpos -= 1;// moves some of the geometry
		break;
	}
}

float turnSpeed = .01; // how fast the mouse rotates the camera
/*This implements a 1st person mouse look using only vectors. Think about better ways to do this...
We make the default mouse position to be the center of the window. You could put it other places, but this is the safest place. Why? */
void PassiveMouseMotion(int x, int y)
{

	//did we move the mouse at all?
	int deltaX = x - imageWidth / 2;
	int deltaY = y - imageHeight / 2;


	if (deltaX == 0 && deltaY == 0) return; //if we didnt move the mouse, dont do anything

	// update the look direction based on the side direction and which way we moved the mouse
	lookdirx += sidedirx*deltaX*turnSpeed;
	lookdirz += sidedirz*deltaX*turnSpeed;
	glutWarpPointer(imageWidth / 2, imageHeight / 2); // put the cursor back to the center of the screen


}

// This functions handles what happens when the window is reshaped
void Reshape(int w, int h)
{
	imageWidth = w;
	imageHeight = h;
	glViewport(0, 0, w, h);
	double aspect = ((double)w) / ((double)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, aspect, .01, 1000);
	//glOrtho(-2,2,-2,2,1,100);

}

// a cleanup function.  call this when you want to exit.
void quit(int i)
{
	exit(i);
}