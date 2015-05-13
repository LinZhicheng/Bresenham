// Bresenham.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdlib.h"
#include "GL/glut.h"
#include "GL/glaux.h"
#include "windows.h"

int w = 640, h = 480, pressX0 = 20, pressY0 = 10, pressXEnd = 480, pressYEnd = 360;

void setPixel(double x, double y){
	glBegin(GL_POLYGON);
	glVertex3d(x, y, 0);
	glVertex3d(x + 10, y, 0);
	glVertex3d(x + 10, y + 10, 0);
	glVertex3d(x, y + 10, 0);
	glEnd();
}

void lineBres(int x0, int y0, int xEnd, int yEnd)
{

	int x, y;

	if (x0 > xEnd){
		x = xEnd;
		y = yEnd;
		xEnd = x0;
		yEnd = y0;
	}
	else{
		x = x0;
		y = y0;
	}

	int dx = abs(xEnd - x), dy = yEnd - y, ady = abs(dy);
	double px = 2 * ady - dx;
	double py = 2 * dx - ady;
	double twoDy = 2 * ady, twoDyMinesDx = 2 * (ady - dx);
	double twoDx = 2 * dx, twoDxMinesDy = 2 * (dx - ady);

	setPixel(x, y);

	if (ady < dx){
		while (x < xEnd){
			x += 10;
			if (px < 0)
				px += twoDy;
			else{
				if (dy > 0)
					y += 10;
				else
					y -= 10;
				px += twoDyMinesDx;
			}
			setPixel(x, y);
		}
	}
	else{
		if (dy > 0){
			while (y < yEnd){
				y += 10;
				if (py < 0){
					py += twoDx;
				}
				else{
					x += 10;
					py += twoDxMinesDy;
				}
				setPixel(x, y);
			}
		}
		else{
			while (y > yEnd){
				y -= 10;
				if (py < 0){
					py += twoDx;
				}
				else{
					x += 10;
					py += twoDxMinesDy;
				}
				setPixel(x, y);
			}
		}
	}
}

void init(GLvoid)     // Create Some Everyday Functions
{

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void display(void)   // Create The Display Function
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	//glTranslatef(0.0f, 0.0f, -1.0f);					// Move Left 1.5 Units And Into The Screen 6.0
	glBegin(GL_LINES);
	for (double i = 0; i < 640; i += 10){
		glVertex3d(i, 0, 0);
		glVertex3d(i, 480, 0);
	}
	for (double i = 0; i < 480; i += 10){
		glVertex3d(0, i, 0);
		glVertex3d(640, i, 0);
	}
	glEnd();
	glBegin(GL_POINT);
	setPixel(0, 0);
	lineBres(pressX0, pressY0, pressXEnd, pressYEnd);

	glutSwapBuffers();
	// Swap The Buffers To Not Be Left With A Clear Screen
}

void reshape(int w, int h)   // Create The Reshape Function (the viewport)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
	glLoadIdentity();                // Reset The Projection Matrix
	gluOrtho2D(0.0, w, 0.0, h);
	glMatrixMode(GL_MODELVIEW);  // Select The Model View Matrix
	glLoadIdentity();    // Reset The Model View Matrix
}

void keyboard(unsigned char key, int x, int y)  // Create Keyboard Function
{
	switch (key) {
	case 27:        // When Escape Is Pressed...
		exit(0);   // Exit The Program
		break;        // Ready For Next Case
	default:        // Now Wrap It Up
		break;
	}
}

void arrow_keys(int a_keys, int x, int y)  // Create Special Function (required for arrow keys)
{
	switch (a_keys) {
	case GLUT_KEY_UP:     // When Up Arrow Is Pressed...
		glutFullScreen(); // Go Into Full Screen Mode
		break;
	case GLUT_KEY_DOWN:               // When Down Arrow Is Pressed...
		glutReshapeWindow(640, 480); // Go Into A 500 By 500 Window
		break;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y){
	if (state == GLUT_DOWN){
		switch (button){
		case GLUT_LEFT_BUTTON:
			printf("%d,%d\n", x, y);
			pressX0 = x - x % 10;
			pressY0 = h - (y - y % 10 + 10);
			printf("%d,%d\n", (int)pressX0, (int)pressY0);
			break;
		case GLUT_RIGHT_BUTTON:
			printf("%d,%d\n", x, y);
			pressXEnd = x - x % 10;
			pressYEnd = h - (y - y % 10 + 10);
			printf("%d,%d\n", (int)pressX0, (int)pressY0);
			break;
		}
		glutPostRedisplay();
	}
}

void main(int argc, char** argv)   // Create Main Function For Bringing It All Together
{
	glutInit(&argc, argv); // Erm Just Write It =)
	init();
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Display Mode
	glutInitWindowSize(w, h); // If glutFullScreen wasn't called this is the window size
	glutCreateWindow("Bresenham"); // Window Title (argv[0] for current directory as title)
	glutDisplayFunc(display);  // Matching Earlier Functions To Their Counterparts
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow_keys);
	glutMouseFunc(mouse);
	glutMainLoop();          // Initialize The Main Loop
}