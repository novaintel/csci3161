#include "Mountain.h"

struct Vector3
{
	float x, y, z;
};

Vector3 *landpt;
int ptptr;
int landGenFirst = 1;

void mountain(int level, GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
{
	GLfloat nx, ny, nz;
	nx = (x1 + x2) / 2.;
	ny = (y1 + y2) / 2.;
	nz = (z1 + z2) / 2.;

	nz += (rand() % 3 ? -1. : 1.) * nz / 5.;

	if (level)
	{
		mountain(level - 1, x1, y1, z1, nx, ny, nz);
		mountain(level - 1, nx, ny, nz, x1, y1, z1);
	}
	else
	{
		landpt[ptptr / 2].x = nx;
		landpt[ptptr / 2].y = ny;
		landpt[ptptr / 2].z = nz;

		if (landpt[ptptr / 2 - 1].x == x1 && landpt[ptptr / 2 - 1].y == y1)
			landpt[ptptr / 2 - 1].z = z1;

		ptptr++;
	}
}

void sortIt(int size)
{
	bool flipped = true;
	Vector3 temp;

	for (int k = 0; k<size; k += size / 8)
	{
		for (int i = k; i<k + size / 8; i++)
		{
			flipped = false;
			for (int j = k + size / 8 - 1; j >= k + 1; j--)
			{
				if (fabs(landpt[j].x) < fabs(landpt[j - 1].x))
				{
					temp.x = landpt[j].x;
					temp.y = landpt[j].y;
					temp.z = landpt[j].z;
					landpt[j].x = landpt[j - 1].x;
					landpt[j].y = landpt[j - 1].y;
					landpt[j].z = landpt[j - 1].z;
					landpt[j - 1].x = temp.x;
					landpt[j - 1].y = temp.y;
					landpt[j - 1].z = temp.z;
					flipped = true;
				}
				else if (landpt[j].x == 0)
				{
					if (fabs(landpt[j].y) < fabs(landpt[j - 1].y))
					{
						temp.x = landpt[j].x;
						temp.y = landpt[j].y;
						temp.z = landpt[j].z;
						landpt[j].x = landpt[j - 1].x;
						landpt[j].y = landpt[j - 1].y;
						landpt[j].z = landpt[j - 1].z;
						landpt[j - 1].x = temp.x;
						landpt[j - 1].y = temp.y;
						landpt[j - 1].z = temp.z;
						flipped = true;
					}
				}
			}
		}
		flipped = true;
	}
}

void landgen(void)
{
	int level = 5, i;
	static int arraySize = pow(2.0, MAXLEVEL + 1);
	if (landGenFirst)
	{
		ptptr = 0;
		landGenFirst = 0;
		landpt = new Vector3[arraySize];
		srand(time(NULL));
		ptptr = 0;

		mountain(MAXLEVEL - 1, 0.0, 0.0, INITHEIGHT, LANDW, LANDL, 0.0);
		mountain(MAXLEVEL - 1, 0.0, 0.0, INITHEIGHT, 0.0, LANDL, 0.0);
		mountain(MAXLEVEL - 1, 0.0, 0.0, INITHEIGHT, -1. * LANDW, LANDL, 0.0);
		mountain(MAXLEVEL - 1, 0.0, 0.0, INITHEIGHT, -1. * LANDW, 0.0, 0.0);
		mountain(MAXLEVEL - 1, 0.0, 0.0, INITHEIGHT, -1. * LANDW, -1. * LANDL, 0.0);
		mountain(MAXLEVEL - 1, 0.0, 0.0, INITHEIGHT, 0.0, -1. * LANDL, 0.0);
		mountain(MAXLEVEL - 1, 0.0, 0.0, INITHEIGHT, LANDW, -1. * LANDL, 0.0);
		mountain(MAXLEVEL - 1, 0.0, 0.0, INITHEIGHT, LANDW, 0.0, 0.0);

		sortIt(arraySize);
	}

	for (i = 0; i<arraySize / 8 * 7; i++)
	{
		if ((i + 1) % (arraySize / 8) == 0) continue;

		glBegin(GL_TRIANGLE_STRIP);

		glVertex3f(landpt[i].x, landpt[i].y, landpt[i].z);
		glVertex3f(landpt[i + arraySize / 8].x, landpt[i + arraySize / 8].y, landpt[i + arraySize / 8].z);
		glVertex3f(landpt[i + 1].x, landpt[i + 1].y, landpt[i + 1].z);
		glVertex3f(landpt[i + arraySize / 8 + 1].x, landpt[i + arraySize / 8 + 1].y, landpt[i + arraySize / 8 + 1].z);

		glEnd();
	}

	for (i = arraySize / 8 * 7; i<arraySize - 1; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);

		glVertex3f(landpt[i].x, landpt[i].y, landpt[i].z);
		glVertex3f(landpt[i - arraySize / 8 * 7].x, landpt[i - arraySize / 8 * 7].y, landpt[i - arraySize / 8 * 7].z);
		glVertex3f(landpt[i + 1].x, landpt[i + 1].y, landpt[i + 1].z);
		glVertex3f(landpt[i - arraySize / 8 * 7 + 1].x, landpt[i - arraySize / 8 * 7 + 1].y, landpt[i - arraySize / 8 * 7 + 1].z);

		glEnd();
	}

	glBegin(GL_TRIANGLE_FAN);

	glVertex3f(0.0, 0.0, INITHEIGHT);
	glVertex3f(landpt[0].x, landpt[0].y, landpt[0].z);
	glVertex3f(landpt[arraySize / 8].x, landpt[arraySize / 8].y, landpt[arraySize / 8].z);
	glVertex3f(landpt[arraySize / 4].x, landpt[arraySize / 4].y, landpt[arraySize / 4].z);
	glVertex3f(landpt[arraySize / 8 * 3].x, landpt[arraySize / 8 * 3].y, landpt[arraySize / 8 * 3].z);
	glVertex3f(landpt[arraySize / 2].x, landpt[arraySize / 2].y, landpt[arraySize / 2].z);
	glVertex3f(landpt[arraySize / 8 * 5].x, landpt[arraySize / 8 * 5].y, landpt[arraySize / 8 * 5].z);
	glVertex3f(landpt[arraySize / 4 * 3].x, landpt[arraySize / 4 * 3].y, landpt[arraySize / 4 * 3].z);
	glVertex3f(landpt[arraySize / 8 * 7].x, landpt[arraySize / 8 * 7].y, landpt[arraySize / 8 * 7].z);
	glVertex3f(landpt[0].x, landpt[0].y, landpt[0].z);

	glEnd();
}
