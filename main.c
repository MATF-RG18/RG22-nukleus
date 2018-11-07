#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>



#define TIMERMSECS 100


int m=1;

// Promenljive za kretanje po x i y osi
static GLfloat movX = 3.0f;
static GLfloat movY = 3.0f;

// Quadric objekat
GLUquadricObj *IDquadric;


// Callback funkcije
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);


void triangularPrism();
int main(int argc, char **argv)
{
     //Quadric Objekat
    IDquadric=gluNewQuadric();


    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);


    glClearColor(0, 0, 0.5f, 0);
    glEnable(GL_DEPTH_TEST);


    // Postavlja se tajmer
    glutTimerFunc(TIMERMSECS, on_timer, 0);

    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        // Zavrsava se program
        exit(0);
        break;

    case 'd':
            movX+=2.5;
            break;
    case 'a':
            movX-=2.5;
            break;
        break;
    case 'w':
            movY+=2.5;
            break;
    case 's':
            movY-=2.5;
            break;
    }
}

static void on_timer(int value)
{ 
    // Uslov za kretanje animacije
    if(m!=0){
        
        // Postavlja se tajmer
        glutTimerFunc(TIMERMSECS, on_timer, 0);
    
        
        // Forsiranje ponovnog iscrtavanja
        glutPostRedisplay();
    }
}

static void on_reshape(int width, int height)
{
    // Postavlja se viewport
    glViewport(0, 0, width, height);

    // Postavlja se projekcija
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 1500);
}

static void on_display(void)
{


    // Postavlja se boja
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Postavlja se vidna tacka
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 500, 0, 0, 0, 0, 1, 0);

 
    glTranslatef(0, movY, 0);
    glTranslatef(movX,0,0);

   glPushMatrix();
        glColor3f(1, 0, 0);
        glRotatef(-90,1,0,0);
        glutSolidCone(25,25,10,10);
        glPushMatrix();
        glTranslatef(0,25,0);
        glRotatef(270,1,0,0);
        glColor3f(1,1,1);
        glRotatef(-90,1,0,0);
        gluCylinder(IDquadric,25.0f,25.0f,100.0f,32,32);
        glPopMatrix();

   glPopMatrix();


    // Postavlja se nova slika u prozor 
    glutSwapBuffers();
}
