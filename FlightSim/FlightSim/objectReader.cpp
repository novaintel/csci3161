#include "objectReader.h"

GLfloat* points;
size_t size = 100;
int count = 0;

void readFile(char* fileName){
	std::ifstream myReadFile;
	myReadFile.open(fileName);
	char output[100];
	GLfloat x, y, z;
	
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
			
			myReadFile >> output;
			if (output[0] == 'v'){
				myReadFile >> x;
				myReadFile >> y;
				myReadFile >> z;
				pushPoint(x, y, z);
			}
			else{
				myReadFile.close();
				return;
			}
			
		}
	}
	myReadFile.close();
}

void drawPlane(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, points);
	glDrawArrays(GL_LINE_LOOP, 0, count);
	glDisableClientState(GL_VERTEX_ARRAY);
	glutSwapBuffers();
}

void pushPoint(GLfloat x, GLfloat y, GLfloat z)
{
	count++;
	if (count>size)
	{
		size += 100;
		points = (GLfloat*)realloc(points, 3 * size*sizeof(GLfloat));
	}
	points[3 * count - 3] = z;
	points[3 * count - 2] = x;
	points[3 * count - 1] = y;
}

void objectReaderInit(){
	points = (GLfloat*)malloc(3 * size*sizeof(GLfloat));
}