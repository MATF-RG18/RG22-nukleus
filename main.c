#include "config.h"
#include "objects.h"


#define TIMERMSECS 20
#define TRARATE 2

//Uslov za kretanje animacije
static int m=1;
// Promenljiva za kretanje po z osi
static GLfloat tra = 0.0f;
// Promenljive za kretanje po x i y osi
static GLfloat movX = 3.0f;
static GLfloat movY = 3.0f;

static int i=0;


//Prepreke
ObjectPosition op[1000];


// Callback funkcije
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);



int main(int argc, char **argv)
{

    //Osvetljenje
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1 };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1 };
    GLfloat light_specular[] = { 1, 1, 1, 1 };
    //GLfloat model_ambient[] = { 0.1, 0, 0, 1 };
    GLfloat light_position[] = { 0.5, 1, 0.5, 1 };


    //Punimo strukturu koja generise prepreke
    int jj= 0;
    for (jj=0;jj<1000;jj++)
        {
            op[jj].x =  -250 + rand() % 500;
            op[jj].y =  -250 + rand() % 500;
            op[jj].z = -500 + rand() % 10000;
        }

    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
 

    glClearColor(0, 0, 0.94f, 0);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    // Postavlja se tajmer
    glutTimerFunc(TIMERMSECS, on_timer, 0);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Postavlja se osvetljenje
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        // Zavrsava se program
        
        printf("MovX = %f ,  MovY = %f  \n ", movX,movY);
        exit(0);
        break;

    case 'd':
            if(movX>=175)
            {
                movX=175;
                break;
            }
            movX+=2.5;
            break;
    case 'a':
            if(movX<=-175)
            {
                movX=-175;
                break;
            }
            movX-=2.5;
            break;
        break;
    case 'w':
            if(movY>=175)
            {
                movY=175;
                break;
            }
            movY+=2.5;
            break;
    case 's':
            if(movY<=-175)
            {
                movY=-175;
                break;
            }
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
    // Postavlja se boja
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Postavlja se vidna tacka
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(movX, movY, gameLength-tra, 0, 0, 0, 0, 1, 0);



    // Crtamo 100 objekata koji ce biti prepreka
    for (i = 0; i < 1000; i++) {
                drawObstacle(op[i]);
                }


    //Pomeramo raketu
    glTranslatef(0,0,-tra);
    glTranslatef(0, movY, 0);
    glTranslatef(movX,0,0);

    // Crtamo raketu
    drawRocket();

    // Postavlja se nova slika u prozor 
    glutSwapBuffers();
}