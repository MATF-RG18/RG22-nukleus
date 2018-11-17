#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <sys/types.h>
#include <sys/wait.h>


#define TIMERMSECS 20
#define TRARATE 0.2f

//Uslov za kretanje animacije
int m=1;
//Duzina nivoa
int gameLength= 1000;

// Promenljiva za kretanje po z osi
static GLfloat tra = 0.0f;
// Promenljive za kretanje po x i y osi
static GLfloat movX = 3.0f;
static GLfloat movY = 3.0f;

static int i=0;

// Quadric objekat
GLUquadricObj *IDquadric;
float rmatrix[100][100][100];

// Callback funkcije
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);

void triangularPrism();
int main(int argc, char **argv)
{

    //Osvetljenje
    GLfloat light_ambient[] = { 0.8, 0.5f, 0.0f, 1 };
    GLfloat light_diffuse[] = { 1, 1, 1, 1 };
    GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat model_ambient[] = { 0.1, 0, 0, 1 };
    GLfloat light_position[] = { -0.2f, -0.2f, -0.2f, -0.5f };

     //Quadric Objekat
    IDquadric=gluNewQuadric();

    //Punimo matricu 100x100x100 sa random vrednostima
    int jj=0,ii=0,kk=0;
    for(jj=0;jj<100;jj++)
        for(ii=0;ii<100;ii++)
            for(kk=0;kk<100;kk++)
                rmatrix[jj][ii][kk] = -500 + rand() % 1000;

    
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

    // Postavlja se osvetljenje
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

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

        tra += TRARATE;    
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

    //Parametri
    GLfloat ambients[] = { 0, 0, 0, 1 };
    GLfloat shininess = 5;
    GLfloat cill[] = { 1, 1, 1, 0.1 };
    GLfloat head[] = { 1, 0, 0, 0 };
    GLfloat objec[] = { 0.37, 0.8, 0.16, 1 };
    //GLfloat cloud[] = { 0.52, 0.8, 0.92, 1 };

    // Postavlja se boja
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Postavlja se vidna tacka
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(movX, movY, gameLength-tra, 0, 0, 0, 0, 1, 0);

    
    // Crtamo 100 objekata koji ce biti prepreka
    for (i = 0; i < 100; i++) {
            glPushMatrix();
            glMaterialfv(GL_FRONT_FACE, GL_AMBIENT, ambients);
            glMaterialfv(GL_FRONT_FACE, GL_DIFFUSE, objec);
            glTranslatef(rmatrix[i][0][0],rmatrix[0][i][0],rmatrix[0][0][i]);
            glutSolidCube(20);
            glPopMatrix();
                }



    //Ovde ce biti kod za oblake
    /*
    for(l=0;l<=2;l++){    
        glPushMatrix();
            glMaterialfv(GL_BACK, GL_AMBIENT, ambients);
            glMaterialfv(GL_BACK, GL_DIFFUSE, cloud);
            glMaterialf(GL_FRONT, GL_SHININESS, shininess);

          //  glTranslatef(p,0,l);
            glScalef(1,1,2);
            glutSolidSphere(30,30,20);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(25,0,0);
            glMaterialfv(GL_BACK, GL_AMBIENT, ambients);
            glMaterialfv(GL_BACK, GL_DIFFUSE, cloud);
            glMaterialf(GL_FRONT, GL_SHININESS, shininess);

          //  glTranslatef(p,0,l);
            glutSolidSphere(30,30,20);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0,25,0);
            glMaterialfv(GL_BACK, GL_AMBIENT, ambients);
            glMaterialfv(GL_BACK, GL_DIFFUSE, cloud);
            glMaterialf(GL_FRONT, GL_SHININESS, shininess);

          //  glTranslatef(p,0,l);
            glutSolidSphere(30,30,20);
        glPopMatrix();
    }
    */


    glTranslatef(0,0,-tra);

    glTranslatef(0, movY, 0);
    glTranslatef(movX,0,0);


    // Crtamo raketu
    glTranslatef(0,0,gameLength-50);
    glRotatef(-105,1,0,0);
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambients);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, head);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        glRotatef(-90,1,0,0);
        glutSolidCone(5,5,25,10);
        glPushMatrix();
       // glTranslatef(0,-40,0);
            glMaterialfv(GL_FRONT, GL_AMBIENT, ambients);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, cill);
            glMaterialf(GL_FRONT, GL_SHININESS, shininess);

            glRotatef(270,1,0,0);
            
            glRotatef(-90,1,0,0);
            gluCylinder(IDquadric,5.0f,5.0f,20.0f,32,32);
            glPopMatrix();


    glPopMatrix();


    // Postavlja se nova slika u prozor 
    glutSwapBuffers();
}
int randomC(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}