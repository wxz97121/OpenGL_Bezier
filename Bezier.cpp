#include<GL\freeglut.h>
#include<iostream>
#include<cmath>
#include<vector>
using namespace std;
#define eps 100
class Point
{
public:
	double x, y;
	Point() { x = 0; y = 0; }
	Point(double xx, double yy) { x = xx; y = yy; }
	Point operator + (const Point b)
	{
		return Point(x + b.x, y + b.y);
	}
	Point operator - (const Point b)
	{
		return Point(x - b.x, y - b.y);
	}
	friend Point operator * (double a, Point my)
	{
		return Point(a*my.x, a*my.y);
	}
	friend Point operator * (Point my, double a)
	{
		return Point(a*my.x, a*my.y);
	}
	Point operator /(const double a)
	{
		return Point(x / a, y / a);
	}
};
bool Fin = false;
int moving = -1;
int num = 0;
int Height = 480;
Point myPoints[100];
double factorial[100];
void myInit()
{
	factorial[0] = 1;
	for (int i = 1; i <= 50; i++) factorial[i] = factorial[i - 1] * i;
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 0, 0);
	glPointSize(10);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 640, 0, Height);
}
void drawDot(int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}
void drawLine(Point p1, Point p2)
{
	glBegin(GL_LINES);
	glVertex2d(p1.x, p1.y);
	glVertex2d(p2.x, p2.y);
	glEnd();
	glFlush();
}

Point drawBezier(double t)
{
	Point P;
	for (int i = 0; i < num; i++)
		P = P + myPoints[i] * factorial[num - 1] / factorial[i] / factorial[num - i - 1] * pow(1 - t, num - i - 1)*pow(t, i);
	return P;
}
void reDraw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	for (int i = 0; i < num; i++) drawDot(myPoints[i].x, myPoints[i].y);
	if (Fin)
	{
		glColor3f(1, 0, 1);
		for (int i = 0; i < num - 1; i++)
			drawLine(myPoints[i], myPoints[i + 1]);
		Point POld = myPoints[0];
		for (double t = 0; t < 1.0; t += 0.01)
		{
			Point P = drawBezier(t);
			drawLine(POld, P);
			POld = P;
		}
		glColor3f(1, 0, 0);
	}
	glFlush();
}
void myMouse(int Button, int state, int x, int y)
{
	if (Button == GLUT_LEFT_BUTTON && state == GLUT_UP && moving != -1) moving = -1;
	if (Button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//cout << "Debug " << x << " " << y << endl;
		for (int i = 0; i < num; i++)
		{
			if (pow(x - myPoints[i].x, 2) + pow(Height - y - myPoints[i].y, 2) < eps)
			{
				//cout << "Debug " << i << endl;
				moving = i;
				return;
			}
		}
		if (Fin)
		{
			Fin = false;
			num = 0;
			glClear(GL_COLOR_BUFFER_BIT);
			glFlush();
		}
		myPoints[num++] = Point(x, Height - y);
		drawDot(x, Height - y);
	}
	if (Button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (Fin || num==0)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glFlush();
			Fin = false;
			return;
		}
		glColor3f(1, 0, 1);
		for (int i = 0; i < num - 1; i++)
			drawLine(myPoints[i], myPoints[i + 1]);
		Point POld = myPoints[0];
		for (double t = 0; t < 1.0; t += 0.01)
		{
			Point P = drawBezier(t);
			drawLine(POld, P);
			POld = P;
		}
		glColor3f(1, 0, 0);
		Fin = true;
	}
}

void myDisplay()
{
	//cout << "Hey" << endl;
	//glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}
void myMotion(int x, int y)
{
	//cout << moving << endl;
	if (moving != -1)
	{
		//cout << "Debug " << x << " " << y << endl;
		myPoints[moving].x = x;
		myPoints[moving].y = Height - y;
		reDraw();
	}
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, Height);
	glutInitWindowPosition(200, 50);
	glutCreateWindow("Bezier Curve");
	glutMouseFunc(myMouse);
	glutDisplayFunc(myDisplay);
	glutMotionFunc(myMotion);
	myInit();
	glutMainLoop();
	return 0;

}