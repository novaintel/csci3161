#ifndef GLUT_HEADER_H
#define GLUT_HEADER_H
#include <GL\freeglut.h>
#endif /* INC_HEADER_H */
#include <iostream>
#include <string>
#include <fstream>
#include<vector>

void pushVectorPoint(GLfloat x, GLfloat y, GLfloat z);
void pushNormalPoint(GLfloat x, GLfloat y, GLfloat z);
void objectReaderInit();
void readFile(char* fileName);
void drawPlane();
void makePolygons();

