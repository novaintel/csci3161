// Include files 
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "BaseScene.h"
#include "objectReader.h"
#include "imageloader.h"
#include "Mountain.h"
#include <GL\freeglut.h>

// Default Image dimensions
int imageWidth = 640;
int imageHeight = 480;
float currentMouseX, currentMouseY;
float planeHeight = 0;
bool fullscreen = false;
int lastRotate = 0;
int rotate = 0;
bool isSeaSky = false;

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

bool fog = false;
GLfloat	fogColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };		// Fog Color


GLuint loadTexture(Image* image) {

	GLuint textureId;

	glGenTextures(1, &textureId); //Make room for our texture

	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit

	//Map the image to the texture

	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D

		0,                            //0 for now

		GL_RGB,                       //Format OpenGL uses for image

		image->width, image->height,  //Width and height

		0,                            //The border of the image

		GL_RGB, //GL_RGB, because pixels are stored in RGB format

		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers

		image->pixels);               //The actual pixel data

	return textureId; //Returns the id of the texture

}


GLuint _textureSkyId; 
GLuint _textureSeaId; 

GLUquadric *Sky;
GLUquadric *Sea;

void initRendering() {

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);

	glEnable(GL_COLOR_MATERIAL);

	Sky = gluNewQuadric();
	Sea = gluNewQuadric();


	Image* imageSky = loadBMP("sky08.bmp");
	Image* imageSea = loadBMP("sea02.bmp");

	_textureSkyId = loadTexture(imageSky);
	_textureSeaId = loadTexture(imageSea);

	delete imageSky;
	delete imageSea;

}


//-----------------------------------------------------------------------------
// Name: InitScene
// Desc: Initializes extensions, textures, render states, etc. before rendering
//-----------------------------------------------------------------------------
int InitScene(void)
{
	objectReaderInit();
	readFile("cessna.txt");

	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, imageWidth, imageHeight);
	GLfloat aspect = (GLfloat)imageWidth / imageHeight;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.1f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat amb_light[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glFogi(GL_FOG_MODE, GL_EXP2);			// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);					// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.015f);						// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);					// Fog Hint Value
	glFogf(GL_FOG_START, 1.0f);							// Fog Start Depth
	glFogf(GL_FOG_END, 5.0f);							// Fog End Depth

	initRendering();

	memset(&camera, 0, sizeof(struct ThirdPersonCamera_t));
	camera.fRadius = 7.0f;


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
	//glRotatef(camera.vecRot.x, 1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(-90, 0.0f, 1.0f, 0.0f);

	glRotatef(rotate, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, planeHeight, 0.0f);
	drawPlane();
	glPopMatrix();

	// Rotate the camera as necessary
	glRotatef(camera.vecRot.y, 0.0f, 1.0f, 0.0f);
	glTranslatef(-camera.vecPos.x, -planeHeight, -camera.vecPos.z);

	if (!isSeaSky)
		drawBase();

	else{
		if (getWireFrame()){
			glPushMatrix();
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, -1.5f);
			glutWireCylinder(50,50,32,32);
			glPopMatrix();

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPushMatrix();
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, -1.0f);
			gluDisk(Sea, 0, 50, 32, 32);
			glPopMatrix();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else{
			glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureSkyId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluQuadricTexture(Sky, 1);
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, -1.5f);
			gluCylinder(Sky, 100, 100, 100, 32, 32);
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();

			glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureSeaId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluQuadricTexture(Sea, 1);
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, -1.0f);
			gluDisk(Sea, 0, 100, 32, 32);
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();

			glPushMatrix();
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			glTranslatef(50.0f, 30.0f, -4.5f);
			landgen();
			glPopMatrix();


			glPushMatrix();
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			glTranslatef(-30.0f, 80.0f, -4.5f);
			landgen();
			glPopMatrix();

			glPushMatrix();
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			glTranslatef(-70.0f, -45.0f, -4.5f);
			landgen();
			glPopMatrix();
		}
	}

	// Display graphics to the user
	glutSwapBuffers();
}


//-----------------------------------------------------------------------------
// Name: IdleFunction
// Desc: The function that is executed whenever the program is idle
//-----------------------------------------------------------------------------
float planeSpeed = 0.1;
void IdleFunction(void)
{
	float xrotrad, yrotrad;
	yrotrad = (camera.vecRot.y / 180.0f * 3.141592654f);
	xrotrad = (camera.vecRot.x / 180.0f * 3.141592654f);
	camera.vecPos.x += (float)(sin(yrotrad))*planeSpeed;
	camera.vecPos.z -= (float)(cos(yrotrad))*planeSpeed;
	camera.vecPos.y -= (float)(sin(xrotrad))*planeSpeed;
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

	if (k == 'f')
	{
		if (!fullscreen){
			glutFullScreen();
			fullscreen = true;
		}
		else {
			glutLeaveFullScreen();
			fullscreen = false;
		}
	}
	if (k == 'b')
	{
		if (!fog){
			glEnable(GL_FOG);
			fog = true;
		}
		else{
			glDisable(GL_FOG);
			fog = false;
		}
	}

	if (k == 'w')
	{
		setWireFrame(!getWireFrame());
	}

	if (k == 's')
	{
		isSeaSky = !isSeaSky;
	}

	if (k == 'q')
	{
		exit(0);
	}

	// If not, then continue rendering
	glutPostRedisplay();
}

void pressKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP: planeHeight += 0.1;  break;
	case GLUT_KEY_DOWN: planeHeight -= 0.1;  break;
	case GLUT_KEY_PAGE_UP: planeSpeed += 0.1; break;
	case GLUT_KEY_PAGE_DOWN: if (planeSpeed > 0.1)planeSpeed -= 0.1; else planeSpeed = 0.1; break;
		
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_PAGE_DOWN: if (planeSpeed <= 0.1)planeSpeed = 0.1;  break;
	}
}

//-----------------------------------------------------------------------------
// Name: MouseFunction
// Desc: Handles mouse input (movement)
//-----------------------------------------------------------------------------
void MouseFunction(int x, int y)
{

	int diffx = x - camera.fLastX;
	int diffy = y - camera.fLastY;

	
	if (diffx < 0){
		rotate = 45;
	}
	else if (diffx > 0){
		rotate = -45;
	}
	else{
		rotate = 0;
	}

	camera.fLastX = x;
	camera.fLastY = y;

	camera.vecRot.x += (float)diffy;
	camera.vecRot.y += (float)diffx;

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
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	glutIdleFunc(IdleFunction);
	glutDisplayFunc(DisplayFunction);
	glutKeyboardFunc(KeyboardFunction);
	glutReshapeFunc(ReshapeFunction);
	glutPassiveMotionFunc(MouseFunction);
	glutMainLoop();

	return 0;
}