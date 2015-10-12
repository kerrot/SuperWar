#include<winsock2.h>  //for Sleep
#include"obj.h"
#include"def.h"
#include"func.h"
#include<time.h>
#include<gl/glut.h>
#include<iostream>
#define TIME 1000
using namespace std;

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

Game game;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game.display();

    glutSwapBuffers ();
}

void timer(int value){
	game.showfps = true;
	glutTimerFunc(TIME, timer, 1);
}

void spinDisplay (void)
{
	
	if(game.get(STATE) == LOADING){
		display();
		//Sleep(500);
	}
	game.goon();
    glutPostRedisplay();
}

void init (void)
{
	srand( time(NULL) );
	alutInit(0, NULL);

	game.start();
	
	GLfloat ambient[4] = { 0.2, 0.2,0.2, 1.0 };
	GLfloat diffuse[4] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat specular[4] = { 0, 0, 0, 1.0 };
	GLfloat pos[4] = { 0, 0, 10, 0.0 };

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glClearColor (0.0, 0.0, 0.0, 1.0);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_BLEND);									//Enable ≥z©ÅE
	glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluOrtho2D(0, (GLfloat)w, (GLfloat)h, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
}

void keyborad1(int key, int x, int y){
	if (game.get(STATE) == ENEMYTURN)
	{
		if (key == GLUT_KEY_F12)
		{
			game.init(OURTURN);
		}
		else
		{
			return;
		}
	}

	switch(key){
		case GLUT_KEY_UP:game.input(UP);break;
		case GLUT_KEY_DOWN:game.input(DOWN);break;
		case GLUT_KEY_RIGHT:game.input(RIGHT);break;
		case GLUT_KEY_LEFT:game.input(LEFT);break;
	}
}

void keyboard2(unsigned char key, int x, int y){
	if (game.get(STATE) == ENEMYTURN)
	{
		return;
	}
	switch(key){
		case 'z':game.input(Z);break;
		case 'x':game.input(X);break;
		case 'c':game.input(C);break;
		case 'a':game.input(A);break;
		case 'd':game.input(D);break;
		case 'w':game.input(W);break;
		case 's':game.input(S);break;
		case 'q':game.input(Q);break;
		case 'e':game.input(E);break;
	}
}

int main()
{
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOWX, WINDOWY);
	glutCreateWindow("SuperWar");
	glutDisplayFunc(display);
    glutReshapeFunc (myReshape);
    glutIdleFunc (spinDisplay);
	glutTimerFunc(TIME, timer, 1);
	glutSpecialFunc(keyborad1);
	glutKeyboardFunc(keyboard2);
	glutSetCursor(GLUT_CURSOR_NONE);
	init();
	glutMainLoop();

}

