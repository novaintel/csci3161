#include "sphere.h"

void drawSphere(double r, int lats, int longs) {
	int i, j;
	for (i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = M_PI * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);

		}
		glEnd();

	}

}

void drawSphere2(double r, int lats, int longs) {

	double x, y, z, thet, phi;
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