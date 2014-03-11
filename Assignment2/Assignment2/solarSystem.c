#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

// Camera position
float x = 0.0, y = 30.0; // initially 5 units south of origin
float planetsizes[] = { 0.2439, 0.6, 0.637, 0.339, 4.0, 3.0, 1.54, 1.46, 0.153 };
float planetcolors[9][3];

static float HourOfDay = 0.0;
static float DayOfYear = 0.0;
static float AnimateIncrement = 24.0;


void changeSize(int w, int h)
{
	float ratio = ((float)w) / ((float)h); // window aspect ratio
	glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	gluPerspective(45.0, ratio, 0.1, 100.0); // perspective transformation
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
	glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}

void update(void)
{
	glutPostRedisplay(); // redisplay everything
}

GLfloat returnRandom(){
	return (GLfloat)rand() / (GLfloat)RAND_MAX;
}

void drawSphere(float radius)
{
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glutSolidSphere(radius, 20, 20);
	glPopMatrix();

}

void renderScene(void)
{
	int i, j;

	// Clear color and depth buffers
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update the animation state
		HourOfDay += AnimateIncrement;
		DayOfYear += AnimateIncrement / 24.0;

		HourOfDay = HourOfDay - ((int)(HourOfDay / 24)) * 24;
		DayOfYear = DayOfYear - ((int)(DayOfYear / 365)) * 365;

	// Reset transformations
	glLoadIdentity();

	// Set the camera centered at (x,y,1) and looking along directional
	// vector (lx, ly, 0), with the z-axis pointing up
	gluLookAt(
		x, y, -10.0,
		x, y - 30, 0.0,
		0.0, 0.0, -1.0);

	glPushMatrix();
	glTranslatef(0.0, 0, 0);
	glColor3f(1.0, 1.0, 0.0);
	drawSphere(6.96);
	glPopMatrix();

	for (j = 0; j < 9; j++) {
		glRotatef(360.0*DayOfYear / 365.0, 0.0, 1.0, 0.0);
		glTranslatef(4.0, j*7.5, 0.0);
		glPushMatrix();						// Save matrix state
		// Second, rotate the earth on its axis.
		//		Use HourOfDay to determine its rotation.
		glColor3f(returnRandom(), returnRandom(), returnRandom());
		glRotatef(360.0*HourOfDay / 24.0, 0.0, 1.0, 0.0);
		// Third, draw the earth as a wireframe sphere.
		glColor3f(0.2, 0.2, 1.0);
		drawSphere(planetsizes[j]);
		glPopMatrix();
	}

	glutSwapBuffers(); // Make it all visible
}

void pressSpecialKey(int key)
{
	switch (key) {
	case GLUT_KEY_UP: y -= 1.0; break;
	case GLUT_KEY_DOWN: y += 1.0; break;
	case GLUT_KEY_LEFT: x += 1.0; break;
	case GLUT_KEY_RIGHT: x -= 1.0; break;
	}
}

void init(){
	int i, j;
	for (i = 0; i < 9; i++){
		for (j = 0; j < 3; j++){
			planetcolors[i][j] = returnRandom();
		}
	}
}


int main(int argc, char **argv)
{
	// general initializations
	glutInit(&argc, argv);
	init();
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 400);
	glutCreateWindow("Solar System");

	// register callbacks
	glutReshapeFunc(changeSize); // window reshape callback
	glutDisplayFunc(renderScene); // (re)display callback
	glutIdleFunc(update); // incremental update 
	glutSpecialFunc(pressSpecialKey); // process special key pressed


	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0; // this is just to keep the compiler happy
}