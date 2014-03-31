#include "objectReader.h"

GLfloat* points;
size_t size = 100;
int count = 0;

void readFile(char* fileName){
	int i, x,y,z;
	char *inname = "test.txt";
	std::ifstream infile(inname);

	if (!infile) {
		exit(1);
	}
	while (infile >> i) {
		if (i == 'v'){
			infile >> x;
			infile >> y;
			infile >> z;
			pushPoint(x, y, z);
		}
	}
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