#include <math.h>
#include "BaseScene.h"


// window dimensions
GLint windowWidth = 500;
GLint windowHeight = 400;
bool fullscreen = false;
int id;

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	drawBase();

	glFlush();
}


void myResize(int newWidth, int newHeight)
{

	// update the new width
	windowWidth = newWidth;
	// update the new height
	windowHeight = newHeight;

	// update the viewport to still be all of the window
	glViewport(0, 0, windowWidth, windowHeight);

	// change into projection mode so that we can change the camera properties
	glMatrixMode(GL_PROJECTION);

	// load the identity matrix into the projection matrix
	glLoadIdentity();

	// gluPerspective(fovy, aspect, near, far)
	gluPerspective(45, (float)windowWidth / (float)windowHeight, 1, 20);

}


void init() {

	// Set the current clear color to sky blue and the current drawing color to
	// white.
	glClearColor(0.1, 0.39, 0.88, 1.0);
	glColor3f(1.0, 1.0, 1.0);

	// Tell the rendering engine not to draw backfaces.  Without this code,
	// all four faces of the tetrahedron would be drawn and it is possible
	// that faces farther away could be drawn after nearer to the viewer.
	// Since there is only one closed polyhedron in the whole scene,
	// eliminating the drawing of backfaces gives us the realism we need.
	// THIS DOES NOT WORK IN GENERAL.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Set the camera lens so that we have a perspective viewing volume whose
	// horizontal bounds at the near clipping plane are -2..2 and vertical
	// bounds are -1.5..1.5.  The near clipping plane is 1 unit from the camera
	// and the far clipping plane is 40 units away.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-2, 2, -1.5, 1.5, 1, 40);

	// Set up transforms so that the tetrahedron which is defined right at
	// the origin will be rotated and moved into the view volume.  First we
	// rotate 70 degrees around y so we can see a lot of the left side.
	// Then we rotate 50 degrees around x to "drop" the top of the pyramid
	// down a bit.  Then we move the object back 3 units "into the screen".
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -15);
	glRotatef(50, 1, 0, 0);
	glRotatef(70, 0, 1, 0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'f': //toggle screenmode
		if (!fullscreen){
			glutFullScreen();
			fullscreen = true;
		}
		else {
			glutLeaveFullScreen();
			fullscreen = false;
		}
		break;
	case 'w':
		setWireFrame(!getWireFrame());
		break;
	case 'q':
		glutDestroyWindow(id);
		exit(0);
		break;
	}
	glutPostRedisplay(); /* this redraws the scene without
						 waiting for the display callback so that any changes appear
						 instantly */
}
// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	id = glutCreateWindow("A Simple Tetrahedron");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	// register the resize function
	glutReshapeFunc(myResize);

	init();
	glutMainLoop();
}