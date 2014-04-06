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
float currentMouseX, currentMouseY;
float planeHeight = 0;
bool fullscreen = false;
GLUquadricObj *cylinder;
int rotate = 0;
void loadImage();

// the image data
GLubyte *imageData;

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

	loadImage();
	objectReaderInit();
	readFile("C:\\Users\\James\\Documents\\code\\School\\csci3161\\FlightSim\\cessna.txt");

	cylinder = gluNewQuadric();

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
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(-90, 0.0f, 1.0f, 0.0f);

	glRotatef(rotate, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, planeHeight, 0.0f);
	drawPlane();
	glPopMatrix();

	// Rotate the camera as necessary
	glRotatef(camera.vecRot.y, 0.0f, 1.0f, 0.0f);
	glTranslatef(-camera.vecPos.x, 0.0f, -camera.vecPos.z);

	// Show camera's statistics
	glColor3f(0.0f, 0.0f, 1.0f);

	drawBase();


	glPushMatrix();
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	gluQuadricDrawStyle(cylinder, GLU_FILL); /* smooth shaded */
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluCylinder(cylinder, 50.0, 50.0, 6.0, 20, 100);
	glPopMatrix();

	// Display graphics to the user
	glutSwapBuffers();
}


/************************************************************************

Function:		loadImage

Description:	Loads in the PPM image

*************************************************************************/
void loadImage()
{


	// the ID of the image file
	FILE *fileID;

	// maxValue
	int  maxValue;

	// total number of pixels in the image
	int  totalPixels;

	// temporary character
	char tempChar;

	// counter variable for the current pixel in the image
	int i;

	// array for reading in header information
	char headerLine[100];

	float RGBScaling;

	// temporary variables for reading in the red, green and blue data of each pixel
	int red, green, blue;

	// open the image file for reading
	fileID = fopen("C:\\Users\\James\\Documents\\code\\School\\csci3161\\FlightSim\\sky08.ppm", "r");

	// read in the first header line
	fscanf(fileID, "%[^\n] ", headerLine);

	// make sure that the image begins with 'P3', which signifies a PPM file
	if ((headerLine[0] != 'P') || (headerLine[1] != '3'))
	{
		printf("This is not a PPM file!\n");
		exit(0);
	}

	// we have a PPM file
	printf("This is a PPM file\n");

	// read in the first character of the next line
	fscanf(fileID, "%c", &tempChar);

	// while we still have comment lines (which begin with #)
	while (tempChar == '#')
	{
		// read in the comment
		fscanf(fileID, "%[^\n] ", headerLine);

		// print the comment
		printf("%s\n", headerLine);

		// read in the first character of the next line
		fscanf(fileID, "%c", &tempChar);
	}

	// the last one was not a comment character '#', so we nee dto put it back into the file stream (undo)
	ungetc(tempChar, fileID);

	// read in the image hieght, width and the maximum value
	fscanf(fileID, "%d %d %d", &imageWidth, &imageHeight, &maxValue);

	// print out the information about the image file
	printf("%d rows  %d columns  max value= %d\n", imageHeight, imageWidth, maxValue);

	// compute the total number of pixels in the image
	totalPixels = imageWidth * imageHeight;

	// allocate enough memory for the image  (3*) because of the RGB data
	imageData = (GLubyte*)malloc(3 * sizeof(GLuint)* totalPixels);


	// determine the scaling for RGB values
	RGBScaling = 255.0 / maxValue;


	// if the maxValue is 255 then we do not need to scale the 
	//    image data values to be in the range or 0 to 255
	if (maxValue == 255)
	{
		for (i = 0; i < totalPixels; i++)
		{
			// read in the current pixel from the file
			fscanf(fileID, "%d %d %d", &red, &green, &blue);

			// store the red, green and blue data of the current pixel in the data array
			imageData[3 * totalPixels - 3 * i - 3] = red;
			imageData[3 * totalPixels - 3 * i - 2] = green;
			imageData[3 * totalPixels - 3 * i - 1] = blue;
		}
	}
	else  // need to scale up the data values
	{
		for (i = 0; i < totalPixels; i++)
		{
			// read in the current pixel from the file
			fscanf(fileID, "%d %d %d", &red, &green, &blue);

			// store the red, green and blue data of the current pixel in the data array
			imageData[3 * totalPixels - 3 * i - 3] = red   * RGBScaling;
			imageData[3 * totalPixels - 3 * i - 2] = green * RGBScaling;
			imageData[3 * totalPixels - 3 * i - 1] = blue  * RGBScaling;
		}
	}


	// close the image file
	fclose(fileID);
}


//-----------------------------------------------------------------------------
// Name: IdleFunction
// Desc: The function that is executed whenever the program is idle
//-----------------------------------------------------------------------------
void IdleFunction(void)
{
	float xrotrad, yrotrad;
	yrotrad = (camera.vecRot.y / 180.0f * 3.141592654f);
	xrotrad = (camera.vecRot.x / 180.0f * 3.141592654f);
	camera.vecPos.x += (float)(sin(yrotrad));
	camera.vecPos.z -= (float)(cos(yrotrad));
	camera.vecPos.y -= (float)(sin(xrotrad));
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
	if (k == 'w')
	{
		setWireFrame(!getWireFrame());
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
	}
}

void releaseKey(int key, int x, int y) {

	/*switch (key) {
	case GLUT_KEY_LEFT: if (deltaAngle < 0.0f)
	deltaAngle = 0.0f;
	break;
	case GLUT_KEY_RIGHT: if (deltaAngle > 0.0f)
	deltaAngle = 0.0f;
	break;
	case GLUT_KEY_UP:  if (deltaMove > 0)
	deltaMove = 0;
	break;
	case GLUT_KEY_DOWN: if (deltaMove < 0)
	deltaMove = 0;
	break;
	}*/
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