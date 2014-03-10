#include "sphere.h"

double user_theta = 0;
double user_height = 0;

void computeLocation() {
	double x = 2 * cos(user_theta);     // my x-, y-, and z-coordinates
	double y = 2 * sin(user_theta);
	double z = user_height;
	double d = sqrt(x * x + y * y + z * z); // distance to origin

	glMatrixMode(GL_PROJECTION);        // Set projection parameters.
	glLoadIdentity();
	glFrustum(-d * 0.5, d * 0.5, -d * 0.5, d * 0.5, d - 1.1, d + 1.1);
	gluLookAt(x, y, z, 0, 0, 0, 0, 0, 1);

}

// Initializes information for drawing within OpenGL.
void init() {
	GLfloat sun_direction[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat sun_intensity[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat lightColor[] = { 0.753, 0.749, 0.678, 1.0 };

	glClearColor(0, 0, 0, 0.0);   // Set window color to white.
	computeLocation();

	glEnable(GL_DEPTH_TEST);            // Draw only closest surfaces


	glEnable(GL_LIGHT0);                // Set up sunlight.
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);

	glEnable(GL_COLOR_MATERIAL);        // Configure glColor().
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

}

// Draws the current image.
void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear window.
	//glColor3f(0.714, 0.494, 0.3569);
	glColor3f(1.0, 1.0, 0);
	glShadeModel(GL_SMOOTH);
	drawSphere(1.0, 10, 10); // glutSolidSphere(1.0, 10, 10);
	glColor3f(1.0, 1.0, 1.0);
	drawSphere(10.0, 10, 10); // glutSolidSphere(1.0, 10, 10);
	glutSwapBuffers();

}

// Arranges that the window will be redrawn roughly every 40 ms.
void idle() {
	static int lastTime = 0;                // time of last redraw
	int time = glutGet(GLUT_ELAPSED_TIME);  // current time

	if (lastTime == 0 || time >= lastTime + 40) {
		lastTime = time;
		glutPostRedisplay();

	}

}

// When window becomes visible, we want the window to
// continuously repaint itself.
void visible(int vis) {
	glutIdleFunc(vis == GLUT_VISIBLE ? idle : NULL);

}

// Called when a "special" key is pressed
void special(int k, int x, int y) {
	switch (k) {
	case GLUT_KEY_UP:    user_height += 0.1; break;
	case GLUT_KEY_DOWN:  user_height -= 0.1; break;
	case GLUT_KEY_LEFT:  user_theta += 0.1; break;
	case GLUT_KEY_RIGHT: user_theta -= 0.1; break;

	}
	computeLocation();
	glutPostRedisplay();

}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);    // Set up display window.
	glutInitWindowSize(1024, 768);
	glutCreateWindow("Sphere");

	init();
	glutDisplayFunc(draw);
	glutVisibilityFunc(visible);
	//glutSpecialFunc(special);
	glutMainLoop();
	return 0;

}