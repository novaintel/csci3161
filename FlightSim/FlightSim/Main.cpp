// Include files 
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "BaseScene.h"
#include "objectReader.h"
#include <GL\freeglut.h>

// Default Image dimensions
int imageWidth = 640;
int imageHeight = 480;

//-----------------------------------------------------------------------------
// 3D vector
//-----------------------------------------------------------------------------
struct Vector3
{
	float x, y, z;
};

//-----------------------------------------------------------------------------
// Third Person Camera structure
//-----------------------------------------------------------------------------
struct ThirdPersonCamera_t
{
	struct Vector3 vecPos;
	struct Vector3 vecRot;
	float fRadius;			// Distance between the camera and the object.
	float fLastX;
	float fLastY;
};


struct ThirdPersonCamera_t camera;

//-----------------------------------------------------------------------------
// Name: InitScene
// Desc: Initializes extensions, textures, render states, etc. before rendering
//-----------------------------------------------------------------------------
int InitScene(void)
{
	//
	// Set default render states
	//


	objectReaderInit();
	readFile("C:\\Users\\James\\Documents\\code\\School\\csci3161\\FlightSim\\cessna.txt");

	// Enable depth testing 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Enable colour material
	glEnable(GL_COLOR_MATERIAL);

	// Disable texture mapping (for now)
	glDisable(GL_TEXTURE_2D);

	// Disable blending
	glDisable(GL_BLEND);

	// Disable dithering
	glDisable(GL_DITHER);

	// Enable culling (counter clock wise)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_CCW);

	// Enable fog
	//	glEnable( GL_FOG );

	// Enable perspective correction and polygon smoothing
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	//
	// Initialize scene objects and geometry
	//

	memset(&camera, 0, sizeof(struct ThirdPersonCamera_t));
	camera.fRadius = 10.0f;


	return GL_TRUE;
}

void DisplayFunction(void)
{
	int iViewport[4];

	// Clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Reset matrices to default position, scale, and rotation
	glLoadIdentity();

	glTranslatef(0.0f, -2.0f, -camera.fRadius);
	glRotatef(camera.vecRot.x, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(-90, 0.0f, 1.0f, 0.0f);

	drawPlane();
	glPopMatrix();

	// Rotate the camera as necessary
	glRotatef(camera.vecRot.y, 0.0f, 1.0f, 0.0f);
	glTranslatef(-camera.vecPos.x, 0.0f, -camera.vecPos.z);

	// Show camera's statistics
	glColor3f(0.0f, 0.0f, 1.0f);

	drawBase();

	// Display graphics to the user
	glutSwapBuffers();
}


//-----------------------------------------------------------------------------
// Name: IdleFunction
// Desc: The function that is executed whenever the program is idle
//-----------------------------------------------------------------------------
void IdleFunction(void)
{
	// Continue to display graphics even when messages aren't
	// being processed.
	glutPostRedisplay();
}

//-----------------------------------------------------------------------------
// Name: KeyboardFunction
// Desc: Handles keyboard input
//-----------------------------------------------------------------------------
void KeyboardFunction(GLubyte k, int x, int y)
{
	static float fRotSpeed = 1.0f;

	if (k == 'q')
	{
		camera.vecRot.x += fRotSpeed;
		if (camera.vecRot.x > 360) camera.vecRot.x -= 360;
	}
	if (k == 'z')
	{
		camera.vecRot.x -= 1;
		if (camera.vecRot.x < -360) camera.vecRot.x += 360;
	}
	if (k == 'w')
	{
		float xrotrad, yrotrad;
		yrotrad = (camera.vecRot.y / 180.0f * 3.141592654f);
		xrotrad = (camera.vecRot.x / 180.0f * 3.141592654f);
		camera.vecPos.x += (float)(sin(yrotrad));
		camera.vecPos.z -= (float)(cos(yrotrad));
		camera.vecPos.y -= (float)(sin(xrotrad));
	}
	if (k == 's')
	{
		float xrotrad, yrotrad;
		yrotrad = (camera.vecRot.y / 180.0f * 3.141592654f);
		xrotrad = (camera.vecRot.x / 180.0f * 3.141592654f);
		camera.vecPos.x -= (float)(sin(yrotrad));
		camera.vecPos.z += (float)(cos(yrotrad));
		camera.vecPos.y += (float)(sin(xrotrad));
	}
	if (k == 'd')
	{
		float yrotrad;
		yrotrad = (camera.vecRot.y / 180.0f * 3.141592654f);
		camera.vecPos.x += (float)(cos(yrotrad)) * 0.5f;
		camera.vecPos.z += (float)(sin(yrotrad)) * 0.5f;
	}
	if (k == 'a')
	{
		float yrotrad;
		yrotrad = (camera.vecRot.y / 180.0f * 3.141592654f);
		camera.vecPos.x -= (float)(cos(yrotrad)) * 0.5f;
		camera.vecPos.z -= (float)(sin(yrotrad)) * 0.5f;
	}

	if (k == 27)
	{
		exit(0);
	}

	// If not, then continue rendering
	glutPostRedisplay();
}

//-----------------------------------------------------------------------------
// Name: MouseFunction
// Desc: Handles mouse input (movement)
//-----------------------------------------------------------------------------
void MouseFunction(int x, int y)
{
	int diffx = x - camera.fLastX;
	int diffy = y - camera.fLastY;

	camera.fLastX = x;
	camera.fLastY = y;

	camera.vecRot.x += (float)diffy;
	camera.vecRot.y += (float)diffx;

	//glutWarpPointer(640 / 2, 480 / 2);
}


//-----------------------------------------------------------------------------
// Name: ReshapeFunction
// Desc: Changes the viewport when the window is resized
//-----------------------------------------------------------------------------
void ReshapeFunction(GLsizei width, GLsizei height)
{
	// Prevent divide by zero exception
	if (!height) height = 1;

	// Reset viewport
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Reset perspective
	gluPerspective(45.0f, (GLdouble)width / height, 0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//-----------------------------------------------------------------------------
// Name: main
// Desc: Program entry point.
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	// Initialize OpenGL via GLUT library
	glutInit(&argc, argv);
	glutInitWindowSize(imageWidth, imageHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutCreateWindow("Flight Sim");
	//	glutFullScreen();

	// Initialize scene objects and properties
	InitScene();

	// Set our input/output functions and begin the main loop
	glutIdleFunc(IdleFunction);
	glutDisplayFunc(DisplayFunction);
	glutKeyboardFunc(KeyboardFunction);
	glutReshapeFunc(ReshapeFunction);
	glutPassiveMotionFunc(MouseFunction);
	glutMainLoop();

	return 0;
}