#include "objectReader.h"

GLfloat* vectorPoints, *normalPoints;
size_t size = 100;
int count = 0;

struct polygon {
	GLfloat* vectorPoints;
	GLfloat* normalPoints;
	GLfloat* colorPoints;
	int numIndices;
};

std::vector<polygon> subObjects;
std::vector<std::vector<int>> polygonPoints;
std::vector<int> numPolygonObject;


void readFile(char* fileName){
	std::ifstream myReadFile;
	myReadFile.open(fileName);
	char output[100];
	char* cVert = new char[4];
	int currentVertex = 0, count = 0;
	std::vector<int> currentPolygon;
	GLfloat x, y, z;

	if (myReadFile.is_open()) {
		myReadFile >> output;
		while (!myReadFile.eof()) {
			
			if (output[0] == 'v'){
				myReadFile >> x;
				myReadFile >> y;
				myReadFile >> z;
				pushVectorPoint(x, y, z);
			}
			else if (output[0] == 'n'){
				myReadFile >> x;
				myReadFile >> y;
				myReadFile >> z;
				pushNormalPoint(x, y, z);
			}
			else if (output[0] == 'f'){

				myReadFile >> output;
			
				while (!(output[0] == 'f' || output[0] == 'g' || myReadFile.eof())){
					for (int i = 0; output[i] != '\0'; i++)
						cVert[i] = output[i];
					currentVertex = atoi(cVert);
					currentPolygon.push_back(currentVertex);
					myReadFile >> output;
				}
				if (output[0] == 'f' || output[0] == 'g'){
					polygonPoints.push_back(currentPolygon);
					currentPolygon.clear();
					count++;
					continue;
				}		
			}
			else if (output[0] == 'g'){
				if (count > 0)
					numPolygonObject.push_back(count);
				myReadFile >> output;
			}
			else
				break;
			myReadFile >> output;
		}
	}
	myReadFile.close();
	numPolygonObject.push_back(count);
	makePolygons();
}


void drawPlane(){
	polygon currentPolygon;

	/*glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, 0);
	glRotatef(90, 0, 1, 0);*/

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	std::vector<polygon>::iterator row;
	for (row = subObjects.begin(); row != subObjects.end(); row++) {
		currentPolygon = *row;
		glVertexPointer(3, GL_FLOAT, 0, currentPolygon.vectorPoints);
		glNormalPointer(GL_FLOAT, 0, currentPolygon.normalPoints);
		glColorPointer(3, GL_FLOAT, 0, currentPolygon.colorPoints);
		glDrawArrays(GL_LINE_LOOP, 0, currentPolygon.numIndices);
	}


//	glVertexPointer(3, GL_FLOAT, 0, vectorPoints);
//	glNormalPointer(GL_FLOAT, 0, normalPoints);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	//glPopMatrix();



	glutSwapBuffers();
}

void makePolygons(){
	std::vector< std::vector<int> >::iterator row;
	std::vector<int>::iterator col;
	polygon currentPolygon;
	int count;
	int colourCount = 0;
	int objectCount = 0;

	for (row = polygonPoints.begin(); row != polygonPoints.end(); row++) {
		count = 0;
		currentPolygon = {
			(GLfloat*)malloc(3 * 100 * sizeof(GLfloat)),
			(GLfloat*)malloc(3 * 100 * sizeof(GLfloat)),
			(GLfloat*)malloc(3 * 100 * sizeof(GLfloat)),
			0,
		};
		for (col = row->begin(); col != row->end(); col++) {
			count++;
			currentPolygon.vectorPoints[3 * count - 3] = vectorPoints[3 * *col - 3];
			currentPolygon.vectorPoints[3 * count - 2] = vectorPoints[3 * *col - 2];
			currentPolygon.vectorPoints[3 * count - 1] = vectorPoints[3 * *col - 1];
			currentPolygon.normalPoints[3 * count - 3] = normalPoints[3 * *col - 3];
			currentPolygon.normalPoints[3 * count - 2] = normalPoints[3 * *col - 2];
			currentPolygon.normalPoints[3 * count - 1] = normalPoints[3 * *col - 1];	
			currentPolygon.numIndices++;
			if ((objectCount <= 3) || (objectCount >= 8 && objectCount <= 10) || (objectCount >= 12 && objectCount <= 13) || (objectCount >= 26 && objectCount <= 32)){
				currentPolygon.colorPoints[3 * count - 1] = 0.0;
				currentPolygon.colorPoints[3 * count - 2] = 1.0;
				currentPolygon.colorPoints[3 * count - 3] = 1.0;
			}
			else if ((objectCount >= 4 && objectCount <= 5) || (objectCount == 11) || (objectCount >= 14 && objectCount <= 25)){
				currentPolygon.colorPoints[3 * count - 1] = 0.0;
				currentPolygon.colorPoints[3 * count - 2] = 0.0;
				currentPolygon.colorPoints[3 * count - 3] = 0.0;
			}
			else if ((objectCount == 6)){
				currentPolygon.colorPoints[3 * count - 1] = 1.0;
				currentPolygon.colorPoints[3 * count - 2] = 0.0;
				currentPolygon.colorPoints[3 * count - 3] = 1.0;
			}
			else if ((objectCount == 7)){
				currentPolygon.colorPoints[3 * count - 1] = 0.0;
				currentPolygon.colorPoints[3 * count - 2] = 0.0;
				currentPolygon.colorPoints[3 * count - 3] = 1.0;
			}
			colourCount++;
			if (objectCount < numPolygonObject.size()){
				if (numPolygonObject[objectCount] == colourCount){
					objectCount++;
					colourCount == 0;
				}
			}
				
		}
		
		subObjects.push_back(currentPolygon);
	}
}

void pushVectorPoint(GLfloat x, GLfloat y, GLfloat z)
{
	count++;
	if (count > size)
	{
		size += 100;
		vectorPoints = (GLfloat*)realloc(vectorPoints, 3 * size*sizeof(GLfloat));
		normalPoints = (GLfloat*)realloc(normalPoints, 3 * size*sizeof(GLfloat));
	}
	vectorPoints[3 * count - 3] = x;
	vectorPoints[3 * count - 2] = y;
	vectorPoints[3 * count - 1] = z;
}

void pushNormalPoint(GLfloat x, GLfloat y, GLfloat z)
{
	normalPoints[3 * count - 3] = z;
	normalPoints[3 * count - 2] = x;
	normalPoints[3 * count - 1] = y;
}

void objectReaderInit(){
	vectorPoints = (GLfloat*)malloc(3 * size*sizeof(GLfloat));
	normalPoints = (GLfloat*)malloc(3 * size*sizeof(GLfloat));
}