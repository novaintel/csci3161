/*----------------------------------------------------------------------------------------
*	Includes
*/


#include <stdlib.h>
#include <stdio.h>

#include <GL/glut.h>




struct Mouse
{
	int x;		
	int y;		
	int lmb;	
	int mmb;	
	int rmb;	

	int xpress; 
	int ypress; 
};


typedef struct Mouse Mouse;

/*
*	Create a global mouse structure to hold the mouse information.
*/
Mouse TheMouse = { 0, 0, 0, 0, 0 };

/*
*	Variables to hold the current size of the window.
*/
int winw = 640;
int winh = 480;

// a point data type
typedef GLfloat point2[2];

GLfloat interp = 0.0;





typedef void(*ButtonCallback)();

/*
*	This is a simple structure that holds a button.
*/
struct Button
{
	int   x;							
	int   y;							
	int   w;							
	int   h;							
	int	  state;						
	int	  highlighted;					
	char* label;						
	ButtonCallback callbackFunction;	
};
typedef struct Button Button;




void TheButtonCallback()
{
	printf("I have been called\n");
}


Button MyButton = { 5, 5, 100, 25, 0, 0, "Button", TheButtonCallback };



void Font(void *font, char *text, int x, int y)
{
	glRasterPos2i(x, y);

	while (*text != '\0')
	{
		glutBitmapCharacter(font, *text);
		++text;
	}
}



int ButtonClickTest(Button* b, int x, int y)
{
	if (b)
	{
		/*
		*	If clicked within button area, then return true
		*/
		if (x > b->x      &&
			x < b->x + b->w &&
			y > b->y      &&
			y < b->y + b->h) {
			return 1;
		}
	}

	/*
	*	otherwise false.
	*/
	return 0;
}


void ButtonRelease(Button *b, int x, int y)
{
	if (b)
	{

		if (ButtonClickTest(b, TheMouse.xpress, TheMouse.ypress) &&
			ButtonClickTest(b, x, y))
		{

			if (b->callbackFunction) {
				b->callbackFunction();
			}
		}

		b->state = 0;
	}
}


void ButtonPress(Button *b, int x, int y)
{
	if (b)
	{

		if (ButtonClickTest(b, x, y))
		{
			b->state = 1;
		}
	}
}



void ButtonPassive(Button *b, int x, int y)
{
	if (b)
	{
		/*
		*	if the mouse moved over the control
		*/
		if (ButtonClickTest(b, x, y))
		{

			if (b->highlighted == 0) {
				b->highlighted = 1;
				glutPostRedisplay();
			}
		}
		else

		if (b->highlighted == 1)
		{
			b->highlighted = 0;
			glutPostRedisplay();
		}
	}
}


void ButtonDraw(Button *b)
{
	int fontx;
	int fonty;

	if (b)
	{
		/*
		*	We will indicate that the mouse cursor is over the button by changing its
		*	colour.
		*/
		if (b->highlighted)
			glColor3f(0.7f, 0.7f, 0.8f);
		else
			glColor3f(0.6f, 0.6f, 0.6f);

		/*
		*	draw background for the button.
		*/
		glBegin(GL_QUADS);
		glVertex2i(b->x, b->y);
		glVertex2i(b->x, b->y + b->h);
		glVertex2i(b->x + b->w, b->y + b->h);
		glVertex2i(b->x + b->w, b->y);
		glEnd();

		/*
		*	Draw an outline around the button with width 3
		*/
		glLineWidth(3);

		/*
		*	The colours for the outline are reversed when the button.
		*/
		if (b->state)
			glColor3f(0.4f, 0.4f, 0.4f);
		else
			glColor3f(0.8f, 0.8f, 0.8f);

		glBegin(GL_LINE_STRIP);
		glVertex2i(b->x + b->w, b->y);
		glVertex2i(b->x, b->y);
		glVertex2i(b->x, b->y + b->h);
		glEnd();

		if (b->state)
			glColor3f(0.8f, 0.8f, 0.8f);
		else
			glColor3f(0.4f, 0.4f, 0.4f);

		glBegin(GL_LINE_STRIP);
		glVertex2i(b->x, b->y + b->h);
		glVertex2i(b->x + b->w, b->y + b->h);
		glVertex2i(b->x + b->w, b->y);
		glEnd();

		glLineWidth(1);


		/*
		*	Calculate the x and y coords for the text string in order to center it.
		*/
		fontx = b->x + (b->w - glutBitmapLength(GLUT_BITMAP_HELVETICA_10, b->label)) / 2;
		fonty = b->y + (b->h + 10) / 2;

		/*
		*	if the button is pressed, make it look as though the string has been pushed
		*	down. It's just a visual thing to help with the overall look....
		*/
		if (b->state) {
			fontx += 2;
			fonty += 2;
		}

		/*
		*	If the cursor is currently over the button we offset the text string and draw a shadow
		*/
		if (b->highlighted)
		{
			glColor3f(0, 0, 0);
			Font(GLUT_BITMAP_HELVETICA_10, b->label, fontx, fonty);
			fontx--;
			fonty--;
		}

		glColor3f(1, 1, 1);
		Font(GLUT_BITMAP_HELVETICA_10, b->label, fontx, fonty);
	}
}

void DrawLetterH(){

	glBegin(GL_POLYGON);
		glColor3f(0.9804f, 0.4863f, 0.0706f);
		glVertex2f(200, 100);	
		glVertex2f(250, 100);
		glColor3f(0.5019f, 0.6706f, 0.9608f);
		glVertex2f(250, 200);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0.9804f, 0.4863f, 0.0706f);
		glVertex2f(200, 100);
		glColor3f(0.5019f, 0.6706f, 0.9608f);
		glVertex2f(250, 200);
		glColor3f(0.1411f, 0.7764f, 0.7765f);
		glVertex2f(200, 250);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(0.5019f, 0.6706f, 0.9608f);
		glVertex2f(250, 200);
		glVertex2f(250, 250);
		glColor3f(0.1411f, 0.7764f, 0.7765f);
		glVertex2f(200, 250);
	glEnd();

	glBegin(GL_POLYGON);
		
		glVertex2f(200, 250);
		glColor3f(0.4000f, 0.9804f, 0.5451f);
		glVertex2f(200, 350);
		glVertex2f(250, 350);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0.5019f, 0.6706f, 0.9608f);
		glVertex2f(250, 250);
		glColor3f(0.1411f, 0.7764f, 0.7765f);
		glVertex2f(200, 250);
		glColor3f(0.4000f, 0.9804f, 0.5451f);	
		glVertex2f(250, 350);
	glEnd();

	
	glBegin(GL_POLYGON);
		glColor3f(0.5019f, 0.6706f, 0.9608f);
		glVertex2f(250, 200);
		glVertex2f(250, 250);
		glColor3f(0.9804f, 0.4863f, 0.0706f);
		glVertex2f(350, 250);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0.5019f, 0.6706f, 0.9608f);
		glVertex2f(250, 200);
		glColor3f(0.9804f, 0.4863f, 0.0706f);
		glVertex2f(350, 200);
		glVertex2f(350, 250);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0.4000f, 0.9804f, 0.5451f);
		glVertex2f(350, 100);
		glVertex2f(400, 100);
		glColor3f(0.9804f, 0.4863f, 0.0706f);
		glVertex2f(350, 200);	
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(0.4000f, 0.9804f, 0.5451f);
		glVertex2f(400, 100);
		glColor3f(0.9804f, 0.4863f, 0.0706f);
		glVertex2f(350, 200);
		glColor3f(0.5019f, 0.6706f, 0.9608f);
		glVertex2f(400, 250);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(0.5019f, 0.6706f, 0.9608f);
		glVertex2f(400, 250);
		glColor3f(0.9804f, 0.4863f, 0.0706f);
		glVertex2f(350, 250);
		glVertex2f(350, 200);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0.5019f, 0.6706f, 0.9608f);
		glVertex2f(400, 250);
		glVertex2f(400, 350);
		glVertex2f(350, 350);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f(400, 250);
		glVertex2f(350, 350);
		glColor3f(0.9804f, 0.4863f, 0.0706f);
		glVertex2f(350, 250);
	glEnd();


}


void DrawStar(){
	glBegin(GL_POLYGON);
		glColor3f(1, 0, 0);
		glVertex2f(300, 200);
		glVertex2f(250, 125);
		glVertex2f(350, 125);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(0, 1, 0);
		glVertex2f(300, 200);
		glVertex2f(350, 125);
		glVertex2f(377.85, 210.72);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(0, 0, 1);
		glVertex2f(300, 200);
		glVertex2f(377.85, 210.72);
		glVertex2f(300, 275);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(1, 1, 0);
		glVertex2f(300, 200);
		glVertex2f(300, 275);
		glVertex2f(227.08, 222.01);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(1, 0, 1);
		glVertex2f(300, 200);
		glVertex2f(227.08, 222.01);
		glVertex2f(250, 125);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(1, 0, 0);
		glVertex2f(300, 50);
		glVertex2f(250, 125);
		glVertex2f(350, 125);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(0, 1, 0);
		glVertex2f(352.98, 272.92);
		glVertex2f(350, 125);
		glVertex2f(377.85, 210.72);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(0, 0, 1);
		glVertex2f(300, 200);
		glVertex2f(377.85, 210.72);
		glVertex2f(300, 275);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(1, 1, 0);
		glVertex2f(300, 200);
		glVertex2f(300, 275);
		glVertex2f(227.08, 222.01);
	glEnd();
	glBegin(GL_POLYGON);
		glColor3f(1, 0, 1);
		glVertex2f(300, 200);
		glVertex2f(227.08, 222.01);
		glVertex2f(250, 125);
	glEnd();
}



void Draw2D()
{
	DrawLetterH();
	ButtonDraw(&MyButton);
}


void Draw()
{
	/*
	*	Clear the background
	*/
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);

	/*
	*	Set perspective viewing transformation
	*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (winh == 0) ? (1) : ((float) winw / winh), 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	/*
	*	Set the orthographic viewing transformation
	*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, winw, winh, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	*	Draw the 2D overlay
	*/
	Draw2D();

	/*
	*	Bring the back buffer to the front and vice-versa.
	*/
	glutSwapBuffers();
}


void MouseButton(int button, int state, int x, int y)
{
	/*
	*	update the mouse position
	*/
	TheMouse.x = x;
	TheMouse.y = y;

	/*
	*	has the button been pressed or released?
	*/
	if (state == GLUT_DOWN)
	{
		/*
		*	This holds the location of the first mouse click
		*/
		if (!(TheMouse.lmb || TheMouse.mmb || TheMouse.rmb)) {
			TheMouse.xpress = x;
			TheMouse.ypress = y;
		}

		/*
		*	Which button was pressed?
		*/
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			TheMouse.lmb = 1;
			ButtonPress(&MyButton, x, y);
		}
	}
	else
	{
		/*
		*	Which button was released?
		*/
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			TheMouse.lmb = 0;
			ButtonRelease(&MyButton, x, y);
			break;
		}
	}

	glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
	/*
	*	Calculate how much the mouse actually moved
	*/
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;

	/*
	*	update the mouse position
	*/
	TheMouse.x = x;
	TheMouse.y = y;


	/*
	*	Check MyButton to see if we should highlight it cos the mouse is over it
	*/
	ButtonPassive(&MyButton, x, y);

	/*
	*	Force a redraw of the screen
	*/
	glutPostRedisplay();
}

void MousePassiveMotion(int x, int y)
{
	/*
	*	Calculate how much the mouse actually moved
	*/
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;

	/*
	*	update the mouse position
	*/
	TheMouse.x = x;
	TheMouse.y = y;

	/*
	*	Check MyButton to see if we should highlight it cos the mouse is over it
	*/
	ButtonPassive(&MyButton, x, y);

}

/*----------------------------------------------------------------------------------------
*
*/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(winw, winh);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Assignment 1");

	glutDisplayFunc(Draw);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);

	glutMainLoop();
}








