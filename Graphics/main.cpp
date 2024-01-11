
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"

const int NUM_STARS = 100;
const double PI = 3.14;


typedef struct {
	double x;
	double y;
	int size;
} STAR;

STAR stars[NUM_STARS];


void init()
{
	int i;
	//                 R     G    B
	glClearColor(0.0,0.0,0.3,0);// color of window background
	glOrtho(-2, 1, -1, 1, -1, 1); // set the coordinates system

	srand(time(0)); // seed of random numbers

	for (i = 0;i < NUM_STARS;i++)
	{
		stars[i].x = (((rand() % 2000) - 1000) / 1000.0 + 1.0) * 1.5 - 2.0; // random number in range [-2,1)
		stars[i].y = 0.5 + (rand() % 500) / 1000.0; // random number in range [0.5, 1)
		stars[i].size = 1 + rand() % 2;
	}
}



void DrawBackground()
{
	// draw background sky
	glBegin(GL_POLYGON);
	glColor3d(0.0, 0.0, 0.3);
	glVertex2d(-2, 1); // left-top
	glVertex2d(1, 1); // right-top
	glColor3d(0.3, 0.5, 0.7);
	glVertex2d(1, 0); // right-bottom
	glVertex2d(-2, 0); // left-bottom
	glEnd();


	// starts points definition
	glColor3d(1, 1, 0); // yellow
	for (int i = 0;i < NUM_STARS;i++)
	{
		glPointSize(stars[i].size);
		glBegin(GL_POINTS);
		glVertex2d(stars[i].x, stars[i].y);
		glEnd(); // ends points definition
	}

	// draw background water
	glBegin(GL_POLYGON);
	glColor3d(0.0, 0.0, 95 / 255.0);
	glVertex2d(-2, 0); // left-top
	glVertex2d(1, 0); // right-top
	glColor3d(0.0, 0.0, 0.0);
	glVertex2d(1, -1); // right-bottom
	glVertex2d(-2, -1); // left-bottom
	glEnd();
}

void DrawBuilding(double x, double width,double y, double height, bool isNear,bool isReflection)
{
	double red, green, blue;
	//building
	glBegin(GL_POLYGON);
	
	//top
	if(!isNear)	red = 0.8, green = 0.6, blue = 0.6; 
	else red = 0.9, green = 0.7, blue = 0.9; 
	if (isReflection) red -= 0.3, green -= 0.3;
	glColor3d(red, green, blue); 
	glVertex2d(x, y);

	// bottom
	glColor3d(0.2, 0.2, 0.2); 
	glVertex2d(x, y-height);
	glVertex2d(x + width, y - height);

	//top
	if (!isNear) red = 0.8, green = 0.6, blue = 0.6; 
	else red = 0.9, green = 0.7, blue = 0.9; 
	if (isReflection) red -= 0.3, green -= 0.3;
	glColor3d(red, green, blue);
	glVertex2d(x + width, y);

	glEnd();
	//3D
	glBegin(GL_POLYGON);
	
	//top
	if (!isNear) red = 0.5, green = 0.3, blue = 0.3; 
	else red = 0.6, green = 0.4, blue = 0.5; 
	if (isReflection) red -= 0.3, green -= 0.3;
	glColor3d(red, green, blue);
	glVertex2d(x+width-0.03, y);

	//bottom
	glColor3d(0.2, 0.2, 0.2);
	glVertex2d(x + width - 0.03, y - height);
	glVertex2d(x + width, y - height);
	
	//top
	if (!isNear) red = 0.5, green = 0.3, blue = 0.3;
	else red = 0.6, green = 0.4, blue = 0.5;
	if (isReflection) red -= 0.3, green -= 0.3;
	glColor3d(red, green, blue);
	glVertex2d(x + width, y);
	glEnd();
	
	// Draw windows
	if (!isNear) red = 0.25, green = 0.88, blue = 0.82;
	else red = 1, green = 1, blue = 0.0; 
	if (isReflection) red -= 0.3, green -= 0.3;
	glColor3d(red, green, blue);
	double windowWidth = 0.02;
	double windowHeight = 0.02;
	double gap = 0.02;
	if (isReflection) {
		for (double i = x + gap; i < x + width - 0.05; i += windowWidth + gap)
		{
			for (double j = y + gap * 2; j < 0.2 * y; j += windowHeight + gap)
			{
				glBegin(GL_POLYGON);
				glVertex2d(i, j);
				glVertex2d(i, j - windowHeight);
				glVertex2d(i + windowWidth, j - windowHeight);
				glVertex2d(i + windowWidth, j);
				glEnd();
			}
		}
	}
	else {
		for (double i = x + gap; i < x + width - 0.05; i += windowWidth + gap)
		{
			for (double j = y - gap; j > 0.2 * y; j -= windowHeight + gap)
			{
				glBegin(GL_POLYGON);
				glVertex2d(i, j);
				glVertex2d(i, j - windowHeight);
				glVertex2d(i + windowWidth, j - windowHeight);
				glVertex2d(i + windowWidth, j);
				glEnd();
			}
		}
	}
}

void DrawGraph()
{
	double x, y;
	// far line
	for (x = -1; x <= 1;x += 0.16)
	{
		y =0.1+ 0.6 * cos(0.4 * PI * x) + 0.2 * sin(4 * PI * x);
		DrawBuilding(x,0.14,y,y,false, false);//building
		DrawBuilding(x, 0.14, -1 * y, -1 * y, false, true);//reflection
	}
	// near line
	for (x = -1; x <= 1;x += 0.21)
	{
		y = 0.1+ 0.3 * cos(0.4 * PI * x) + 0.1 * sin(3 * PI * x);
		DrawBuilding(x, 0.18, y, y, true, false);//building
		DrawBuilding(x, 0.18, -1 * y, -1 * y, true, true);//reflection
	}
}

void DrawParabolaAndLines(double xMin, double xMax, double xMid) {
	for (double i = xMin; i <= xMax; i += 0.15) {
		glBegin(GL_LINES);
		glColor3d(1.0, 0.0, 0.0);
		glVertex2d(i, 2 * pow((xMid * -1) + i, 2)+0.05);
		glVertex2d(i, 0);
		glEnd();
	}

	for (double i = xMin; i <= xMax; i += 0.001) {
		glBegin(GL_POINTS);
		glColor3d(1.0, 0.0, 0.0);
		glVertex2d(i, 2 * pow((xMid * -1) + i, 2) + 0.05);
		glEnd();
	}
}

void DrawBridge() {
	DrawParabolaAndLines(-1.8,-1.2, -1.5);//mid
	DrawParabolaAndLines(-1.2, -1, -0.9);//right
	DrawParabolaAndLines(-2.1, -1.8, -2.1);//right
	glBegin(GL_LINES);
	glColor3d(1.0, 0.0, 0.0);
	glVertex2d(-2,0);
	glVertex2d(-1, 0);
	glEnd();
}

// put all the drawings here
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawBackground();
	
	DrawGraph();

	DrawBridge();

	glutSwapBuffers(); // show all
}

void idle() 
{
	if (rand() % 5 == 0)
	{
		int i = rand() % NUM_STARS;
		stars[i].size = 1 + rand() % 2;
	}

	glutPostRedisplay();
}


void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("First Example");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	init();

	glutMainLoop();
}