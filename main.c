#include "config.h"
#include "objects.h"
#include <sys/time.h>
#include <sys/types.h>
#define PI 3.1415927

#define TIMERMSECS 30
#define TRARATE (20)
#define A_WIDTH 600
#define A_HEIGHT 600
//Uslov za kretanje animacije
static int m = 0;
//Faza igrice
// phase == 0 -> pocetna animacija
// phase == 1 -> pocetak igrice
static int phase = 1;

// Promenljiva za kretanje po z osi
static GLfloat tra = 0.0f;
static int r1 = 0, r2 = 0, r3 = 0, rf1 = 0, rf2 = 0, rf3 = 0, orgCl = 0;
static int i = 0;

GLfloat light_ambient1[] = {0.1f, 0.1f, 0.8f, 1};
GLfloat light_diffuse1[] = {0.8f, 0.4f, 0.2f, 1};
GLfloat light_specular1[] = {1, 1, 1, 0.4};
GLfloat light_position1[] = {-1, -1, 0.5, 1};
//Raketa
ObjectPosition player;

//Prepreke
ObjectPosition op[1000];

//Grad
ObjectPosition il[1000];

//Oblaci
ObjectPosition cl[20];

// Prepreke koje se krecu
ObjectPosition op2[25];

//Visina zgrada u gradu
int buildingHeight[1000];

// Callback funkcije
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);

// Funcija za poredjenje pozicija po z osi
int compare(const void *s1, const void *s2)
{
    ObjectPosition *op1 = (ObjectPosition *)s1;
    ObjectPosition *op2 = (ObjectPosition *)s2;
    return op2->z - op1->z;
}

int main(int argc, char **argv)
{

    //Osvetljenje
    GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1};
    GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1};
    GLfloat light_specular[] = {1, 1, 1, 0.4};
    //GLfloat model_ambient[] = { 0.1, 0, 0, 1 };
    GLfloat light_position[] = {-1, -1, 0.5, 1};

    //Punimo pocetne koordinate igraca
    player.x = 0;
    player.y = 0;
    player.z = gameLength - 50;

    //Punimo strukturu koja generise prepreke
    int jj = 0, kk = 0, oo = 0;
    for (jj = 0; jj < 1000; jj++)
    {

        op[jj].x = -250 + rand() % 500;
        op[jj].y = -250 + rand() % 500;
        op[jj].z = 500 + rand() % 9000;
    }
    // Sortiramo sve prepreke
    qsort(op, 1000, sizeof(ObjectPosition), compare);

    // Punimo strukturu koja generise prepreke koje se krecu
    for (jj = 0; jj < 50; jj++)
    {
        op2[jj].x = -250 + rand() % 500;
        op2[jj].y = -250 + rand() % 500;
        op2[jj].z = 500 + rand() % 9000;
    }
    //Punimo strukturu koja generise grad
    for (kk = 0; kk < 1000; kk++)
    {
        il[kk].x = -150 + rand() % 300;

        il[kk].y = -150 + rand() % 300;
        il[kk].z = 0;
        buildingHeight[kk] = rand() % 5;
    }

    //Punimo strukturu koja generise oblake
    for (oo = 0; oo < 20; oo++)
    {

        if (oo % 2)
        {
            cl[oo].x = abs(-600 + rand() % 1200) * (-1);
            cl[oo].y = -600 + rand() % 1200;
            cl[oo].z = -50 + rand() % 200;
        }
        else if (oo % 3)
        {
            cl[oo].x = abs(-600 + rand() % 1200) * (-1);
            cl[oo].y = abs(-600 + rand() % 1200) * (-1);
            cl[oo].z = abs(-50 + rand() % 200) * (-1);
        }
        else
        {
            cl[oo].x = -600 + rand() % 1200;
            cl[oo].y = -600 + rand() % 1200;
            cl[oo].z = -50 + rand() % 200;
        }
        if(oo<=10)
        {
            cl[oo].z += gameLength/3;
        }
        cl[oo].z += 2500;

    }
    //qsort(cl, 50, sizeof(ObjectPosition), compare);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(A_WIDTH, A_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    glClearColor(0.337, 0.501, 0.784, 0);
    glEnable(GL_DEPTH_TEST);
    //  glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
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
    switch (key)
    {
    case 27:
        // Zavrsava se program
        exit(0);
        break;

    case 'd':
        if (player.x >= 175)
        {
            player.x = 175;
            break;
        }
        player.x += 2.5;
        break;
    case 'a':
        if (player.x <= -175)
        {
            player.x = -175;
            break;
        }
        player.x -= 2.5;
        break;
        break;
    case 'w':
        if (player.y >= 175)
        {
            player.y = 175;
            break;
        }
        player.y += 2.5;
        break;
    case 's':
        if (player.y <= -175)
        {
            player.y = -175;
            break;
        }
        player.y -= 2.5;
        break;
    case 'k':
        // Pocinjemo nivo
        tra = 0;
        m = 1;
        // Resetujemo tajmer
        glutTimerFunc(TIMERMSECS, on_timer, 0);
        break;
    }
}

static void on_timer(int value)
{
    // Uslov za kretanje animacije
    if (m != 0)
    {

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
    gluPerspective(60, (float)width / height, 1, 5000);
}

static void on_display(void)
{
    // Postavlja se boja
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Na osnovu ovoga menjamo faze
    if (tra > gameLength)
    {
        if (phase == 0)
        {
            m = 0;
            phase = 1;
            tra = 0;
        }
        else
            m = 0;
    }
    // Postavlja se vidna tacka
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Postavljamo kameru u odnosu na fazu igrice
    if (phase == 0)
    {
        gluLookAt(300, 300, tra + 300, -10000, -10000, -10000, -10000, -10000, 0);
    }
    else
        gluLookAt(player.x, player.y, gameLength - tra, 0, 0, 0, 0, 1, 0);

    //drawBackground(400,addOnX,addOnY);
    for (i = 0; i < 100; i++)
    {
        drawCity(il[i], buildingHeight[i]);
    }


    drawBackground(250, 50);
    //drawBackground(900,40);
    
    // Crtamo 1000 objekata koji ce biti prepreka
    if (phase == 1)
    {
        for (i = 0; i < 1000; i++)
        {
            drawObstacle(op[i]);
        }

        glPushMatrix();

        // Crtamo objekte koji se krecu
        for (i = 0; i < 25; i++)
        {
            if (i % 2)
            {
                if (r1 > 2000)
                    rf1 = 1;
                if (r1 < -2000)
                    rf1 = 0;
                if (r2 > 2000)
                    rf2 = 1;
                if (r2 < -2000)
                    rf2 = 0;

                if (rf1 == 0)
                {
                    op2[i].x += 5;
                    r1 += 5;
                }

                if (rf1 == 1)
                {
                    op2[i].x -= 5;
                    r1 -= 5;
                }

                if (rf2 == 0)
                {
                    op2[i].y += 5;
                    r2 += 5;
                }

                if (rf2 == 1)
                {
                    op2[i].y -= 5;
                    r2 -= 5;
                }
            }
            else
            {
                if (r1 > 2000)
                    rf1 = 1;
                if (r1 < -2000)
                    rf1 = 0;
                if (r2 > 2000)
                    rf2 = 1;
                if (r2 < -2000)
                    rf2 = 0;

                if (rf1 == 0)
                {
                    op2[i].x -= 5;
                    r1 += 5;
                }

                if (rf1 == 1)
                {
                    op2[i].x += 5;
                    r1 -= 5;
                }

                if (rf2 == 0)
                {
                    op2[i].y -= 5;
                    r2 += 5;
                }

                if (rf2 == 1)
                {
                    op2[i].y += 5;
                    r2 -= 5;
                }
            }
            drawMovingObstacle(op2[i]);
        }

        glPopMatrix();
    }
    
    // Crtamo oblake na njihovim lokacijama

    glPushMatrix();
    for (i = 0; i < 20; i++)
    {
        drawCloud(cl[i]);
    }
    glPopMatrix();

    //Pomeramo raketu
    if (phase == 1)
    {

        glTranslatef(0, 0, -tra);
        glTranslatef(0, player.y, 0);
        glTranslatef(player.x, 0, 0);

        // Azuriramo z koordinate
        player.z = gameLength - 50;
        player.z = player.z - tra;
        /*
        // Testiramo da li je doslo do kolizije sa preprekama
        if (collision2(op, player, 1000, 15) <= 0)
        {
            m = 0;
        }
       
        // Testiramo da li je doslo do kolizije sa preprekama koje se krecu
        if (collision2(op2, player, 25, 30) <= 0)
        {
            m = 0;
        }
        */
        // Testiramo da li je doslo do kolizije sa oblacima
        if (collision2(cl, player, 20, 500) <= 0)
        {
            m = 1;
            glDisable(GL_LIGHT0);

            glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
            glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
            glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
            glEnable(GL_LIGHT1);
        }
        else 
        {
            glDisable(GL_LIGHT1);
            glEnable(GL_LIGHT0);
        }
    }
    // Crtamo raketu

    drawRocket();

    // Postavlja se nova slika u prozor
    glutSwapBuffers();
}
