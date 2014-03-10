#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLint axis = 2;

void sphere()
{

	double x, y, z, thet, phi;
	int nlat, nlong;
	double c;
	x = y = 0;
	z = 1;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(x, y, z);
	c = 3.14159 / 180.0;
	z = sin(c*80.0);
	for (thet = -180.0; thet <= 180.0; thet += 20.0)
	{
		x = sin(c*thet)*cos(c*80.0);
		y = cos(c*thet)*cos(c*80.0);
		glVertex3d(x, y, z);
	}
	glEnd();
	x = y = 0;
	z = -1;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(x, y, z);
	z = -sin(c*80.0);
	for (thet = -180.0; thet <= 180.0; thet += 20.0)
	{
		x = sin(c*thet)*cos(c*80.0);
		y = cos(c*thet)*cos(c*80.0);
		glVertex3d(x, y, z);
	}
	glEnd();
	for (phi = -80.0; phi <= 80.0; phi += 20.0)
	{
		glBegin(GL_QUAD_STRIP);
		for (thet = -180.0; thet <= 180.0; thet += 20.0)
		{
			x = sin(c*thet)*cos(c*phi);
			y = cos(c*thet)*cos(c*phi);
			z = sin(c*phi);
			glVertex3d(x, y, z);

			x = sin(c*thet)*cos(c*(phi + 20.0));
			y = cos(c*thet)*cos(c*(phi + 20.0));
			z = sin(c*(phi + 20.0));
			glVertex3d(x, y, z);
		}
		glEnd();
	}
}

void spinSphere()
{

	/* Idle callback, spin cube 2 degrees about selected axis */

	theta[axis] += 2.0;
	if (theta[axis] > 360.0) theta[axis] -= 360.0;
	/* display(); */
	glutPostRedisplay();
}