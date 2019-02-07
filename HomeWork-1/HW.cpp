/*********
CTIS164 - Template Source Program
----------
STUDENT : Mian Muhammad Umair Amer
SECTION : 03
HOMEWORK: 01
----------
PROBLEMS: Everything Is Running Perfectly Fine - Just in Libraries change GLUT library according to your route..!!! 
*********/

#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD     10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
	bool back;
	int cx, cy;
}player_t;

/*Global Variables and Constants for This Program*/
int CX = -290, CY = 130; //fixed point of car(left bottom corner)
bool activetimer = false;
bool RestartMsg = false;
int timecounter = 0, smokecount = 0, smokecounter = 0;
int mincount = 0, value = 0;
bool smokeapp = true;

player_t P[5];

void intialiseStruct()
{
	for (int i = 0; i < 5; i++)
	{
		P[i].cx = CX;
		P[i].cy = CY - (i * 100);
		P[i].back = false;
	}
}

int max(player_t P[])
{
	int i;
	for (i = 1; i < 5; i++)
		if (P[i].cx + 95 > P[value].cx + 95)
			value = i;
	return value;
}

int min(player_t P[])
{
	int i;
	for (i = 0; i < 5; i++)
		if (P[i].back == true)
			if (P[i].cx < P[value].cx)
				value = i;
	return value;
}

//void onTimer(int v);
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void displayBackground()
{
	//To create the Score board Background
	glColor3ub(0, 0, 0);
	glRectf(-300, 200, 300, 300);

	//Creating Racing Tracks
	glColor3ub(255, 255, 0);
	for (int i = 2; i >= -3; i--)
	{
		glBegin(GL_LINES);
		glVertex2f(-300, 100 * i);
		glVertex2f(300, 100 * i);
		glEnd();
	}
	//Road Marks
	glColor3ub(255, 255, 255);
	for (int j = 0; j < 5; j++)
		for (int i = -5; i <= 5; i += 2)
		{
			glBegin(GL_LINES);
			glVertex2f(50 * i, -250 + (j * 100));
			glVertex2f(50 * (i + 1), -250 + (j * 100));
			glEnd();
		}

	//Label at top
	glColor3ub(255, 255, 255);
	glRectf(-170, 250, 170, 285);

	glBegin(GL_LINE_LOOP);
	glVertex2f(-175, 245);
	glVertex2f(-175, 290);
	glVertex2f(175, 290);
	glVertex2f(175, 245);
	glEnd();
	vprint(-265, 265, GLUT_BITMAP_9_BY_15, "TIMER");
	vprint(215, 265, GLUT_BITMAP_9_BY_15, "WINNER");
	glColor3ub(255, 0, 0);
	vprint(-170, 260, GLUT_BITMAP_TIMES_ROMAN_24, "Fast & Furious Racing (by Umair)");

	glColor3ub(0, 255, 0);
	vprint2(-278, 230, 0.15, "00:%02d:%02d", mincount, timecounter % 100);

	glColor3ub(255, 255, 0);
	if (RestartMsg == false)
		vprint(-170, 215, GLUT_BITMAP_8_BY_13, "Press <spacebar> to Stop/Restart animation");
	else
	{
		vprint(-100, 215, GLUT_BITMAP_8_BY_13, "Press <f1> To Restart Race");
		vprint(160, 215, GLUT_BITMAP_HELVETICA_10, "Press <ESC> To EXIT Race");
	}

}

void ForwardVehicleDisplay(int cx, int cy)
{
	//Lights
	glColor3ub(255, 0, 0);
	circle(cx + 90, cy + 18, 5);

	//Scilencer
	glColor3ub(0, 0, 0);
	glRectf(cx, cy, cx - 7, cy + 5);

	//Smoke
	glColor3ub(190, 190, 190);

	if (activetimer)
	{
		smokecounter++;
		if (smokecounter % 100 == 0)
			smokecount++;

		if (smokeapp == true && smokecount == 1)
			circle(cx - 15, cy + 8, 5);
		if (smokeapp == true && smokecount == 2)
			circle(cx - 30, cy + 15, 8);
		if (smokeapp == true && smokecount == 3)
			circle(cx - 50, cy + 28, 11);
		if (smokeapp == true && smokecount == 4)
		{
			smokeapp == false;
			smokecount = 0;
		}
		else
			smokeapp == true;
	}
}

void ReverseVehicleDisplay(int cx, int cy)
{
	//Lights
	glColor3ub(255, 0, 0);
	circle(cx, cy + 18, 5);

	//Scilencer
	glColor3ub(0, 0, 0);
	glRectf(cx + 90, cy, cx + 97, cy + 5);

	//Smoke
	glColor3ub(190, 190, 190);
	if (activetimer)
	{
		smokecounter++;
		if (smokecounter % 100 == 0)
			smokecount++;

		if (smokeapp == true && smokecount == 1)
			circle(cx + 105, cy + 8, 5);
		else if (smokeapp == true && smokecount == 2)
			circle(cx + 120, cy + 15, 8);
		else if (smokeapp == true && smokecount == 3)
			circle(cx + 140, cy + 28, 11);
		else if (smokeapp == true && smokecount == 4)
		{
			smokeapp == false;
			smokecount = 0;
		}
		else
			smokeapp == true;
	}
}

void displayVehicle(int cx, int cy, int num, int g)
{
	if (P[num].back == true)
		ReverseVehicleDisplay(cx, cy);
	else
		ForwardVehicleDisplay(cx, cy);
	//Body
	glColor3ub(255, g, 20);
	glBegin(GL_POLYGON);
	glVertex2f(cx, cy);
	glVertex2f(cx, cy + 25);
	glVertex2f(cx + 10, cy + 30);
	glVertex2f(cx + 80, cy + 30);
	glVertex2f(cx + 90, cy + 25);
	glVertex2f(cx + 90, cy);
	glEnd();

	//Windows
	glColor3ub(255, 255, 255);
	glBegin(GL_POLYGON);
	glVertex2f(cx + 20, cy + 30);
	glVertex2f(cx + 30, cy + 40);
	glVertex2f(cx + 60, cy + 40);
	glVertex2f(cx + 70, cy + 30);
	glEnd();

	//Wheels
	glColor3ub(0, 0, 0);
	circle(cx + 20, cy, 10);
	circle(cx + 70, cy, 10);
	glColor3ub(255, 255, 255);
	circle(cx + 20, cy, 5);
	circle(cx + 70, cy, 5);

	//Partitions
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	//Window Partion
	glVertex2f(cx + 45, cy + 40);
	glVertex2f(cx + 45, cy + 30);
	//DoorHandle1
	glVertex2f(cx + 50, cy + 25);
	glVertex2f(cx + 55, cy + 25);
	//DoorHandle2
	glVertex2f(cx + 25, cy + 25);
	glVertex2f(cx + 30, cy + 25);
	//Door Partition
	glVertex2f(cx + 45, cy + 28);
	glVertex2f(cx + 45, cy + 2);
	//Bunker and Bonet Partion
	glVertex2f(cx + 20, cy + 28);
	glVertex2f(cx + 20, cy + 11);
	glVertex2f(cx + 70, cy + 28);
	glVertex2f(cx + 70, cy + 11);
	glEnd();

	//Car Number
	glColor3ub(0, 0, 0);
	vprint(cx + 5, cy + 12, GLUT_BITMAP_9_BY_15, "#");
	vprint(cx + 71, cy + 12, GLUT_BITMAP_9_BY_15, "0%d", num + 1);

}

void displayWinner(player_t P[])
{
	int maxval;
	glColor3ub(0, 255, 0);

	if (P[0].back == false && P[1].back == false && P[2].back == false && P[3].back == false && P[4].back == false)
	{
		maxval = max(P);
		if (timecounter != 0)
			vprint(230, 235, GLUT_BITMAP_HELVETICA_18, "%02d", maxval + 1);
	}
	else
	{
		maxval = min(P);
		vprint(230, 235, GLUT_BITMAP_HELVETICA_18, "%02d", maxval + 1);
	}
}


//
// To display onto window using OpenGL commands
//
void display() {

	//
	// clear window to black
	//
	glClearColor(0.15, 0.15, 0.15, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	displayBackground();

	int g = 255; // to change color of each car
	for (int i = 0; i < 5; i++)
	{
		displayVehicle(P[i].cx, P[i].cy, i, g);
		g -= 50;
	}

	displayWinner(P);


	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == 32)
		activetimer = !activetimer;
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	if (key == GLUT_KEY_F1)
	{
		timecounter = 0;
		mincount = 0;
		intialiseStruct();
		RestartMsg = false;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	//glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	if (activetimer)
	{
		timecounter++;
		if (timecounter % 100 == 0)
			mincount++;

		//For Forward Movement
		for (int i = 0; i < 5; i++)
			if (P[i].cx + 95 < winWidth / 2 && P[i].back == false)
				P[i].cx += rand() % 4 + 1;
			else
				P[i].back = true;

		//For Reverse Movement
		if (P[0].cx - 8 >= -winWidth / 2 && P[1].cx - 8 >= -winWidth / 2 && P[2].cx - 8 >= -winWidth / 2 && P[3].cx - 8 >= -winWidth / 2 && P[4].cx - 8 >= -winWidth / 2)
		{
			for (int i = 0; i < 5; i++)
				if (P[i].cx -8 >= -winWidth / 2 && P[i].back == true)
					P[i].cx -= rand() % 4 + 1;
		}
		else
		{
			activetimer = !activetimer;
			RestartMsg = true;
		}
	}

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {

	intialiseStruct();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("FAST & FURIOUS - Mian Muhammad Umair Amer");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();
	srand(time(0));
	glutMainLoop();
}
