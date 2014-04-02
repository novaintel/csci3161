#include "objectReader.h"

GLfloat* vectorPoints, *normalPoints;
size_t size = 100;
int count = 0;

struct polygon {
	GLfloat* vectorPoints;
	GLfloat* normalPoints;
	GLfloat red;
	GLfloat blue;
	GLfloat green;
};

std::vector<polygon> subObjects;


void readFile(char* fileName){
	std::ifstream myReadFile;
	myReadFile.open(fileName);
	char output[100];
	char* cVert = new char[4];
	polygon currentPolygon;
	std::string currentline;
	int currentVertex = 0, count = 0;
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
			
				while (!(output[0] == 'f' || output[0] == 'g')){
//					if (output[0] == 'f' || output[0] == 'g')
//						break;
					for (int i = 0; output[i] != '\0'; i++)
						cVert[i] = output[i];
					currentVertex = atoi(cVert);
					if (currentVertex == 234)
						std::cout << "Oh No!";

					x = vectorPoints[3 * currentVertex - 3];
					y = vectorPoints[3 * currentVertex - 2];
					z = vectorPoints[3 * currentVertex - 1];

					count++;
					currentPolygon.vectorPoints[3 * count - 3] = x;
					currentPolygon.vectorPoints[3 * count - 2] = y;
					currentPolygon.vectorPoints[3 * count - 1] = z;

					myReadFile >> output;
				}
				if (output[0] == 'f' || output[0] == 'g')
					continue;

			}
			else if (output[0] == 'g'){
				subObjects.push_back(currentPolygon);
				count = 0;
				currentPolygon = {
					(GLfloat*)malloc(3 * 100 * sizeof(GLfloat)),
					(GLfloat*)malloc(3 * 100 * sizeof(GLfloat)),
					0.0,
					0.0,
					0.0,
				};
				myReadFile >> output;
			}
			else
				break;
			myReadFile >> output;
		}
	}
	myReadFile.close();

}


void drawPlane(){

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	for (std::vector<polygon>::iterator it = subObjects.begin(); it != subObjects.end(); ++it){
		glVertexPointer(3, GL_FLOAT, 0, it->vectorPoints);
	}

	//	glVertexPointer(3, GL_FLOAT, 0, vectorPoints);
	//	glNormalPointer(GL_FLOAT, 0, normalPoints);
	glDrawArrays(GL_LINE_LOOP, 0, count);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glutSwapBuffers();
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