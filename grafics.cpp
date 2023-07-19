#include <GL/freeglut.h>
#include <time.h>
#include <windows.h>
#include "vec.h"

int IsPause;
double fulltime;
double delta_time;
double view_x, view_h, view_z;
static const int N = 50, M = 50;
vec P[2][2], V[2][2], AbsV[2][2], G[N][M];


/*
void DrawBezier(vec &P0, vec &P1, vec &P2, vec &P3)
{
	glBegin(GL_LINE_STRIP);

	for (double t = 0; t <= 1 + 0.001; t += 0.01)
	{

		vec Q = P0 * pow(1 - t, 3) + P1 * 3 * pow(1 - t, 2) * t + P2 * 3 * t * t * (1 - t) + P3 * t*t*t;
		glVertex3d(Q.x, Q.y, Q.z);
	}
	glEnd();
}

int minl(int a, int b)
{
	return a < b ? a : b;
}

int maxl(int a, int b)
{
	return a > b ? a : b;
}

void Render(void)
{
	for (int i = 0; i < NumOfr; i++)
	{
		R[i] += V[i] * delta_time;
		if (R[i].x > 2)
			V[i].x = -AbsV[i].x;
		else if (R[i].x < -2)
			V[i].x = AbsV[i].x;
		if (R[i].y > 2)
			V[i].y = -AbsV[i].y;
		else if (R[i].y < -2)
			V[i].y = AbsV[i].y;
		if (R[i].z > 2)
			V[i].z = -AbsV[i].z;
		else if (R[i].z < -2)
			V[i].z = AbsV[i].z;
	}

	glPushMatrix();
	for (int i = 0; i < NumOfr - 1; i++)
	{
		vec P0, P1, P2, P3;
		P0 = R[i];
		P3 = R[i + 1];
		P1 = (R[i + 1] - R[maxl(0, i - 1)]) * (1.0 / 6) + R[i];
		P2 = (R[minl(i + 2, NumOfr)] - R[i]) * (-1.0 / 6) + R[i + 1];
		DrawBezier(P0, P1, P2, P3);
	}

	glPopMatrix();
}*/



void Reshape(int W, int H)
{
	int Width, Height;
	double x = 1.7, y = 1.7;
	glViewport(0, 0, Width = W, Height = H);
	if (W > H)
		x *= (double)W / H;
	else
		y *= (double)H / W;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-x, x, -y, y, -100, 100);

	gluLookAt(12, 12, 24, 0, 0, 0, 0, 10, 0);
	glMatrixMode(GL_MODELVIEW);
}

void Keyboard(unsigned char key, int X, int Y)
{
	if (key == 27)
		exit(0);
	if (key == 'w')
		view_h += 0.1;
	if (key == 's')
		view_h -= 0.1;
}

void Idle(void)
{
	long cur_time = clock();
	static long start_time = -1, last_time = -1, pause_time;
	if (start_time == -1)
		start_time = last_time = cur_time;
	if (IsPause == 1)
		pause_time += cur_time - last_time;
	delta_time = (double)(cur_time - pause_time - start_time) / CLOCKS_PER_SEC - fulltime;
	fulltime = (double)(cur_time - pause_time - start_time) / CLOCKS_PER_SEC;
	last_time = cur_time;
}

void build_geom(void)
{
	int i, j;
	double stepv = 1.0 / M, stepu = 1.0 / N, u, v;
	for (i = 0, v = 0; i < N; i++, v += stepv)
		for (j = 0, u = 0; j < M; j++, u += stepu)
			G[i][j] = P[0][0] * (u - 1) * (v - 1) + P[0][1] * v * (u - 1) + P[1][0] * u * (v - 1) + P[1][1] * u * v;
}

void draw_geom(void)
{
	glColor3d(0, 1, 0);
/*
	glBegin(GL_LINE_STRIP);
	glVertex3d(P[0][0].x, -1, P[0][0].z);
	glVertex3d(P[0][0].x, 1, P[0][0].z);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3d(P[1][1].x, -1, P[1][1].z);
	glVertex3d(P[1][1].x, 1, P[1][1].z);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3d(P[0][1].x, -1, P[0][1].z);
	glVertex3d(P[0][1].x, 1, P[0][1].z);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3d(P[1][0].x, -1, P[1][0].z);
	glVertex3d(P[1][0].x, 1, P[1][0].z);
	glEnd();

	*/

	for (int i = 0; i < N; i++)
	{
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < M; j++)
			glVertex3dv(&G[i][j].x);
		glEnd();
	}

	for (int j = 0; j < M; j++)
	{
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < N; i++)
			glVertex3dv(&G[i][j].x);
		glEnd();
	}
}

void Render(void)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			P[i][j] += V[i][j] * delta_time;
			if (P[i][j].y > 1)
				V[i][j].y = -AbsV[i][j].y;
			else if (P[i][j].y < -1)
				V[i][j].y = AbsV[i][j].y;
		}
	}
	build_geom();
	draw_geom();
}

void Display(void)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	Idle();
	glPushMatrix();
	glScaled(0.8, 0.8, 0.8);
	glRotated(15 * view_x, 0, 1, 0);
	glTranslated(2 * view_z, 0, 0);
	glTranslated(0, 2 * view_h, 0);
	Render();
	glPopMatrix();
	glFinish();

	glutSwapBuffers();
	glutPostRedisplay();
}

void SpecialKeyboard(int key, int MouseX, int MouseY)
{
	if (key == GLUT_KEY_RIGHT)
		view_x += 0.1;
	if (key == GLUT_KEY_LEFT)
		view_x -= 0.1;
	if (key == GLUT_KEY_UP)
		view_z += 0.1;
	if (key == GLUT_KEY_DOWN)
		view_z -= 0.1;
}

void main(int argc, char *argv[])
{
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			AbsV[i][j] = V[i][j] = vec::Rnd0();

	P[0][0] = vec(0, 0, 0);
	P[0][1] = vec(0, 0, 1);
	P[1][0] = vec(1, 0, 0);
	P[1][1] = vec(1, 0, 1);

	/*
	for (int i = 0; i < NumOfr; i++)
	{
		R[i] = vec::Rnd();
		AbsV[i] = vec::Rnd();
		V[i] = AbsV[i];
	}*/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("template");

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(Display);
	glutSpecialFunc(SpecialKeyboard);
	glutMainLoop();
}
