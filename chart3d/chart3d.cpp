#include <iostream>
#include <GL/freeglut.h>
#include <vector>

using namespace std;

class screendata
{
private:
	double ww, wh;
public:
	screendata() { ww = 800; wh = 600; };
	double getwh()
	{
		return wh;
	}
	double getww()
	{
		return ww;
	}
	void updatewwwh(double uww, double uwh)
	{
		ww = uww;
		wh = uwh;
	}
} sdt;

struct pnt { float x; float y; float z; };

GLfloat lightpos[] = {30,30,100,0};
GLfloat lightcol[] = { 1,1,1,1 };
GLfloat alightcol[] = { 0,0,0,0 };

bool usequads=true;
bool calcdone=false;

float k_cangle = 0; float cr = 30;
bool fillmode = false;

float mx1 = -50; float mx2 = 50;
float my1 = -25; float my2 = 25;

int p = 50;
float r = 1.0; float g = 1.0; float b = 1.0;
float scl = 1; bool tfixed = false;
float min; float max;
vector<pnt> dtp;

bool needrefresh = false;

void refreshangles();
void displayCB(void);

float getfuncres(float x, float y)
{
	//return pow(x, 2) - pow(y, 3);
	return (10 * sin(0.1*x) + 5 * cos(0.5*y));
}

void tmrf(int value)
{
	k_cangle += ((2 * 3.14) / 360);
    if (k_cangle > 2 * 3.14)  k_cangle = 0;
	refreshangles();
	glutTimerFunc(40, tmrf, 0);
}

void builddataarray(vector<pnt> &dtp)
{
	dtp.clear();
	float wx = mx2 - mx1;
	float wy = my2 - my1;
	float dx = wx / p; float dy = wy / p;
	for (int j = 0; j < p; j++)
		for (int i = 0; i < p; i++)
		{
			pnt cp;
			cp.x = mx1 + i * dx;
			cp.y = my1 + j * dy;
			cp.z = getfuncres(cp.x,cp.y);
			dtp.push_back(cp);
		}
}

void refreshangles()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, (GLdouble)(sdt.getww() / sdt.getwh()), 0.1, 1000);
	gluLookAt((cr*sin(k_cangle)), (cr*cos(k_cangle)), 30, 0, 0, 0, 0, 0, 1);
	glViewport(0, 0, sdt.getww(), sdt.getwh());
	glMatrixMode(GL_MODELVIEW);
	displayCB();
}

void reshapeCB(int width, int height)
{
	double ww = width;
	double wh = height;
	sdt.updatewwwh(ww, wh);
	refreshangles();
}

void specCD(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		usequads = true;
		cout << "Use quads \n";
	}
	if (key == GLUT_KEY_DOWN)
	{
		usequads = false;
		cout << "Use tris \n";
	}
}

void mouseCB(int but, int state, int x, int y)
{
	if ((but == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) 
		{ k_cangle += ((2 * 3.14) / 36); printf("left\n"); };
	if ((but == GLUT_RIGHT_BUTTON) && (state == GLUT_UP)) 
		{ k_cangle -= ((2 * 3.14) / 36); printf("right\n");};
	if ((but == GLUT_MIDDLE_BUTTON) && (state == GLUT_UP))
	{
		if (fillmode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			fillmode = false;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			fillmode = true;
		}
		k_cangle -= ((2 * 3.14) / 36); printf("middle\n");
	}
	
	if (k_cangle < 0.0f)  k_cangle = (float)((2 * 3.14)+k_cangle);
	if (k_cangle > 2 * 3.14)  k_cangle = 0;
	
	
	printf("%f\n", k_cangle);
	refreshangles();
}

void displayCB()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glColor3f(1, 1, 1);

	int ops = 0;

	int k = 0;
	for (int j = 0; j < p-1; j++)
		for (int i = 0; i < p-1; i++)
		{
			if (k < dtp.size() - p - 1)
			{
				if (usequads)
				{
					glBegin(GL_QUADS);
					glColor3f(1, 1, 1);
					glNormal3f(0, 0, 1);
					glVertex3f(dtp[k].x, dtp[k].y, dtp[k].z); ops++;
					glVertex3f(dtp[k + 1].x, dtp[k + 1].y, dtp[k + 1].z); ops++;
					glVertex3f(dtp[k + 1 + p].x, dtp[k + 1 + p].y, dtp[k + 1 + p].z); ops++;
					glVertex3f(dtp[k + p].x, dtp[k + p].y, dtp[k + p].z); ops++;
					glEnd();
				}
				else
				{
					glBegin(GL_TRIANGLES);
					glColor3f(1, 1, 1); ops++;
					glNormal3f(0, 0, 1); ops++;
					glVertex3f(dtp[k].x, dtp[k].y, dtp[k].z); ops++;
					glVertex3f(dtp[k + 1].x, dtp[k + 1].y, dtp[k + 1].z); ops++;
					glVertex3f(dtp[k + p].x, dtp[k + p].y, dtp[k + p].z); ops++;
					glVertex3f(dtp[k + p].x, dtp[k + p].y, dtp[k + p].z); ops++;
					glVertex3f(dtp[k + p + 1].x, dtp[k + p +1].y, dtp[k + p +1].z); ops++;
					glVertex3f(dtp[k + 1].x, dtp[k + 1].y, dtp[k + 1].z); ops++;
					glEnd();
				}
			}
			if (i==p-2)
			k+=2;
			else k++;
		}

	glutSwapBuffers();				/* Complete any pending operations */

	if (!calcdone)
	{
		printf("%d\n",ops);
		calcdone = true;
	}
}

void idleCB(void)
{
	if (needrefresh)
	{
		displayCB();
		needrefresh = false;
	}
}

int main(int argc, char *argv[])
{
	int win;
	sdt = screendata();

	glutInit(&argc, argv);		/* initialize GLUT system */

	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(800, 600);		/* width=400pixels height=500pixels */
	win = glutCreateWindow("Poly");	/* create window */

	/* from this point on the current window is win */

	builddataarray(dtp);

	glClearColor(0.0, 0.0, 0.0, 0.0);	/* set background to black */
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
	glLightfv(GL_LIGHT0, GL_AMBIENT, alightcol);
	glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, lightpos);
	glEnable(GL_LIGHT0);
	glutSpecialFunc(specCD);
	glutReshapeFunc(reshapeCB);
	glutIdleFunc(idleCB);			/* idle redraw */
	glutDisplayFunc(displayCB);		/* set window's display callback */
	glutMouseFunc(mouseCB);
	glutTimerFunc(40,tmrf,0);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutMainLoop();			/* start processing events... */

	/* execution never reaches this point */

	return 0;
}
