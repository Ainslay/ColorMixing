// Opracowane przez: Jakub Spałek, Aleksandra Pyrkosz

#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>

using namespace std;

enum State
{
	MIRROR,
	SHADOW,
	NONE
};

State state = NONE;

GLfloat eyeX = 0.0;
GLfloat eyeY = 40.0;
GLfloat eyeZ = 50.0;
GLfloat pointX = 0.5 * cos(-3.14 / 2);
GLfloat pointY = 0.0;
GLfloat pointZ = 0.5 * sin(-3.14 / 2);

GLfloat lightPosition[] = { 100.0f, 100.0f, 50.0f, 1.0f };
GLfloat dimLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat strongLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat ambientMaterials[] = { 0.329412, 0.223529, 0.027451, 1.0 };
GLfloat diffuseMaterials[] = { 0.780392, 0.568627, 0.113725, 1.0 };
GLfloat specularMaterials[] = { 0.992157, 0.941176, 0.807843, 1.0 };
GLfloat shininessMaterials = 27.8974;

GLfloat fLightPosition[] = { 100.0f, 100.0f, 50.0f, 1.0f };
GLfloat fLightPositionUnder[] = { 100.0f, -100.0f, 50.0f, 1.0f };

GLfloat transparency = 0.5f;
GLfloat actorsRotate = 0.0f;

void DrawActors(State state)
{
	if (state == MIRROR)
	{
		actorsRotate += 0.5f;

		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();

			glColor3f(0.7, 0.6, 0.0);
			glTranslatef(-3.0f, 10.0f, -5.0f);
			glRotatef(actorsRotate, 0, actorsRotate / 6, 1);
			glutSolidTeapot(3.0f);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor4f(0.0, 0.0, 1.0, transparency);
			glTranslatef(10.0, 0.0, 9.0f);
			glutSolidTorus(1.0f, 3.0f, 32, 32);

			glDisable(GL_BLEND);

		glPopMatrix();
	}
	else if (state == SHADOW)
	{
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_LIGHT0);

		glPushMatrix();

			glDisable(GL_LIGHTING);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor4f(0.0, 0.0, 0.0, 1.0);
			glTranslatef(-10.0f, 0.2f, -12.0f);
			glRotatef(actorsRotate, 0, actorsRotate / 6, 1);

			glScalef(1.2f, 0.02f, 1.2f);
			glutSolidTeapot(3.0f);

			glDisable(GL_BLEND);
			glEnable(GL_LIGHTING);

		glPopMatrix();
	}
	else if (state == NONE)
	{
		actorsRotate += 0.5f;
		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();

			glColor3f(0.7, 0.6, 0.0);
			glTranslatef(-3.0f, 10.0f, -5.0f);
			glRotatef(actorsRotate, 0, actorsRotate / 6, 1);
			glutSolidTeapot(3.0f);

		glPopMatrix();
	}
}

void DrawFloor()
{
	glPushMatrix();
		glMatrixMode(GL_MODELVIEW);

		glBegin(GL_TRIANGLES);

			glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
			glVertex3f(0.0f, 0.0f, 150.0f);
			glVertex3f(0.0f, 0.0f, -150.0f);
			glVertex3f(130.0f, 0.0f, 0.0f);

			glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
			glVertex3f(0.0f, 0.0f, 150.0f);
			glVertex3f(0.0f, 0.0f, -150.0f);
			glVertex3f(-130.0f, 0.0f, 0.0f);

		glEnd();
	glPopMatrix();
}

void Display()
{
	glClearColor(0.0, 0.0, 0.0, 0.4);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	// materials
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterials);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterials);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterials);
	glMateriali(GL_FRONT, GL_SHININESS, shininessMaterials);

	glPushMatrix();
		glLoadIdentity();

		// swiatło ogólne
		glLightfv(GL_LIGHT0, GL_AMBIENT, dimLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, strongLight); 
		glLightfv(GL_LIGHT0, GL_SPECULAR, strongLight); 

	glPopMatrix();

	glNormal3f(0.0f, 1.0f, 0.0f);

	if (state == MIRROR)
	{
		glPushMatrix();
		gluLookAt(eyeX, eyeY, eyeZ, pointX, pointY, pointZ, 0.0, 1.0, 0.0);

		glLightfv(GL_LIGHT0, GL_POSITION, fLightPositionUnder);	

		glPushMatrix();
		glFrontFace(GL_CW);											
		glScalef(1.0f, -1.0f, 1.0f);								
		DrawActors(MIRROR);												
		glFrontFace(GL_CCW);										
		glPopMatrix();

		glDisable(GL_LIGHTING);										

		glEnable(GL_BLEND);											
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			
		DrawFloor();												
		glDisable(GL_BLEND);										

		glEnable(GL_LIGHTING);									
		glLightfv(GL_LIGHT0, GL_POSITION, fLightPosition);			

		DrawActors(MIRROR);												

		glPopMatrix();
	}
	else if (state == SHADOW)
	{
		glPushMatrix();

		gluLookAt(eyeX, eyeY, eyeZ, pointX, pointY, pointZ, 0.0, 1.0, 0.0);

		DrawFloor();
		DrawActors(SHADOW);
		DrawActors(NONE);

		glPopMatrix();
	}
	else
	{
		state = MIRROR;
		glEnable(GL_LIGHT0);
		glutPostRedisplay();
	}

	glFlush();
	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (width < height && width > 0)
		glFrustum(-1.0, 1.0, -1.0 * height / width, 1.0 * height / width, 2.0, 100.0);
	else
		if (width >= height && height > 0)
			glFrustum(-1.0 * width / height, 1.0 * width / height, -1.0, 1.0, 2.0, 100.0);

	glMatrixMode(GL_MODELVIEW);

	Display();
}

void Key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case('+'):
		eyeZ = eyeZ - 1.0;
		break;
	case('-'):
		eyeZ = eyeZ + 1.0;
		break;
	case('1'):
		glEnable(GL_LIGHT0);
		break;
	case('2'):
		glDisable(GL_LIGHT0);
		break;
	case('s'):
		if (transparency <= 0.9)
			transparency += 0.1;
		break;
	case('a'):
		if (transparency >= 0.1)
			transparency -= 0.1;
		break;
	}
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		eyeX = eyeX - 1.0f;
		break;
	case GLUT_KEY_RIGHT:
		eyeX = eyeX + 1.0f;
		break;
	case GLUT_KEY_UP:
		eyeY = eyeY + 0.5f;
		break;
	case GLUT_KEY_DOWN:
		if (eyeY >= 1)
			eyeY = eyeY - 0.5f;
		break;
	}
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void Menu(int value)
{
	switch (value)
	{
	case MIRROR:
		state = MIRROR;
		break;
	case SHADOW:
		state = SHADOW;
		break;
	}
}

void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(3, TimerFunction, 1);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(800, 800);

	glutCreateWindow("Mieszanie kolorow - Jakub Spalek, Aleksandra Pyrkosz");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKeys);

	glutTimerFunc(33, TimerFunction, 1);

	glutCreateMenu(Menu);
	glutAddMenuEntry("Lustro", MIRROR);
	glutAddMenuEntry("Cienie", SHADOW);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	cout << "Ruch kamery: klawisze strzalek\n";
	cout << "Zoom: [+]/[-]\n";
	cout << "Przelacz swiatlo ogolne: [1]/[2]\n";
	cout << "Zmiana przezroczystosci: [a]/[s]\n";
	cout << "Wybor trybu: [PPM]";

	glutMainLoop();

	return 0;
}

