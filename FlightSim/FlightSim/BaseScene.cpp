#include "BaseScene.h"

bool wireframe = true;
#define M_PI 3.14159265358979323846
const int gridRadius = 100;

void drawBase(){
	// Draw a white grid "floor" for the tetrahedron to sit on.

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	glColor3f(.8, .8, .8);
	glBegin(GL_QUADS);
	glVertex3f(-gridRadius, -0.001, -gridRadius);
	glVertex3f(-gridRadius, -0.001, gridRadius);
	glVertex3f(gridRadius, -0.001, gridRadius);
	glVertex3f(gridRadius, -0.001, -gridRadius);
	glEnd();

	glBegin(GL_LINES);
	for (int i = -gridRadius; i <= gridRadius; i++) {
	    glColor3f(.8, .8, .8);
		glVertex3f(i, 0, -gridRadius);
		glVertex3f(i, 0, gridRadius);
		glVertex3f(-gridRadius, 0, i);
		glVertex3f(gridRadius, 0, i);
	};
	glEnd();

	
	glLineWidth(10);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(10, 0, 0);
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 10, 0);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 10);
	glColor3f(1.0, 1.0, 1.0);
	glEnd();
	glLineWidth(1);


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
