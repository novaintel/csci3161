#include "BaseScene.h"

bool wireframe = true;
#define M_PI 3.14159265358979323846
const int gridRadius = 25;

void drawBase(){
	// Draw a white grid "floor" for the tetrahedron to sit on.

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	glBegin(GL_POLYGON);
	glColor3f(0.8, 0.8, 0.8);
	for (GLfloat i = -gridRadius; i <= gridRadius; i += 0.50) {
		glVertex3f(i, 0, gridRadius); glVertex3f(i, 0, -gridRadius);
		glVertex3f(gridRadius, 0, i); glVertex3f(-gridRadius, 0, i);
	}
	glEnd();





	glBegin(GL_LINES);
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(10, 0, 0);
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 10, 0);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 10);
	glColor3f(1.0, 1.0, 1.0);
	glEnd();



	if (wireframe){
		glutWireSphere(0.25, 20, 20);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else{
		glutSolidSphere(0.25, 20, 20);
	}

}

bool getWireFrame(){
	return wireframe;
}

void setWireFrame(bool newWireframe){
	wireframe = newWireframe;
}
