#ifndef GLUT_HEADER_H
#define GLUT_HEADER_H
#include <GL\freeglut.h>
#endif /* INC_HEADER_H */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

void pushPoint(GLfloat x, GLfloat y, GLfloat z);
void objectReaderInit();
void readFile(char* fileName);

