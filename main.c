#include "config.h"
#include "image.h"
#include "objects.h"

#define PI 3.1415927

#define TIMERMSECS 30
#define TRARATE (5)
#define A_WIDTH 600
#define A_HEIGHT 600


// Slika je preuzeta sa https://opengameart.org
// fajlovi image.c i image.h su preuzeti sa asistentovog sajta : http://poincare.matf.bg.ac.rs/~bozidar_antic/rg/cas7/Cas7.zip
#define WATERTEXTURE "tex_Water.bmp"
#define SKYTEXTURE "sky.bmp"
//Identifikatori tekstura
static GLuint textures[5];

//Uslov za kretanje animacije
static int m = 0;
// Faza igrice
// phase == 0 -> pocetna animacija
// phase == 1 -> pocetak igrice
static int phase = 0;

// Promenljiva za kretanje po z osi
static GLfloat tra = 0.0f;
static GLfloat traSpeed = 0.01f;
// Pomocne promenljive za generisanje oblaka
static int r1 = 0, r2 = 0, rf1 = 0, rf2 = 0;
static int i = 0;
// Ako zelimo da preskocimo animaciju
static int flagSkip = 0;
const int gln = 10000;
// Osvetljenje koje koristimo kada dodje do kolizije sa oblakom
GLfloat light_ambient1[] = {0.1f, 0.1f, 0.1f, 0.8};
GLfloat light_diffuse1[] = {0.2f, 0.1f, 0.1f, 1};
GLfloat light_specular1[] = {0.9, 0.9, 0.9, 0.4};
//GLfloat light_position1[] = {-1, -1, 0.5, 1};
GLfloat light_position1[] = {7000, 7000,7000, 0.9};

GLfloat light_ambient3[] = {0.337, 0.501, 0.784, 0};

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


// Inicijalizujemo teksturu
void textureInit(Image * image,char * filename,int pointer);
void drawSkybox(int size,int n);

int main(int argc, char **argv)
{

    //Osvetljenje
    GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1};
    GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1};
    GLfloat light_specular[] = {1, 1, 1, 0.4};
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
    qsort(cl, 20, sizeof(ObjectPosition), compare);


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(A_WIDTH, A_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);


    //Omogucujemo dubinu,culling i blend
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_GEOMETRY_SHADER);
 
    // Omugucavamo 2D teksture
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);


    // Postavlja se tajmer
    glutTimerFunc(TIMERMSECS, on_timer, 0);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Postavlja se osvetljenje
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Postavljamo svetlo plavu kao boju pozadine
    //glClearColor(0.337, 0.501, 0.784, 0);
    glClearColor(0.466, 0.764, 0.831, 0);

    // Kreiramo teksturu
    Image * image;
    image = image_init(0, 0);

    //Generisemo identifikatore tekstura
    glGenTextures(5, textures);
    // Kreiramo teksturu koja predstavlja vodu
    textureInit(image,WATERTEXTURE,0);
    textureInit(image,SKYTEXTURE,1);
    textureInit(image,SKYTEXTURE,2);
    textureInit(image,SKYTEXTURE,3);
    textureInit(image,SKYTEXTURE,4);
    //Unistavamo objekat za citanje tekstura iz fajla
    image_done(image);

    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        // Mozemo da kliknemo ESC da preskocimo pocetnu animaciju. Nakon toga se na ESC zavrsava program
        if(!flagSkip)
        {
            flagSkip = 1;
            phase = 1;
            //m=0;
            tra=0;
        }
        else 
        {
            //Zavrsava se program
            exit(0);
        }
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
    case 32:
        // Pocinjemo nivo.  32 -> spacebar
        // Na spacebar mozemo i da resetujemo nivo kada dodjemo do kolizije
        if(m==0)
        {
            tra = 0;
            traSpeed = 0.01;
            m = 1;
            // Resetujemo tajmer
            glutTimerFunc(TIMERMSECS, on_timer, 0);
            
        }
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
        //tra koristimo za kretanje po Z osi
        tra += TRARATE;
        // traSpeed uveca brzinu kretanja dok prolazi vreme
        tra += traSpeed;
        traSpeed += 0.03;
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
    gluPerspective(60, (float)width / height, 1, 15000);
}

static void on_display(void)
{
    // Postavlja se boja
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.541, 0.8, 0.858,0);
    // Omogucava da kada smo u fazi igranja da ESC ne resetuje nivo kada se prvi put klikne
    if(phase==1)
    {
        flagSkip = 1;
    }

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


    //Crtamo grad
    for (i = 0; i < 100; i++)
    {
        drawCity(il[i], buildingHeight[i]);
    }

 

    // Crtamo vodu
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBegin(GL_QUADS);
        glNormal3f(20, 20, 12000);

        glTexCoord2f(0, 0);
        glVertex3f(-4000, -4000, -1);

        glTexCoord2f(0, 10);
        glVertex3f(4000, -4000, -1);

        glTexCoord2f(10, 10);
        glVertex3f(4000, 4000, -1);

        glTexCoord2f(10, 0);
        glVertex3f(-4000, 4000, -1);
    glEnd();
    glPopMatrix();

    //Crtamo skybox
    drawSkybox(4000,1);

    /*
    
    glPushMatrix();    
    glBindTexture(GL_TEXTURE_2D, textures[5]);
    glBegin(GL_QUADS);
        glNormal3f(20, 20, 12000);

        glTexCoord2f(0, 0);
        glVertex3f(-3000, -3000, gameLength-tra-4000);

        glTexCoord2f(0, 1);
        glVertex3f(3000, -3000, gameLength-tra-4000);

        glTexCoord2f(1, 1);
        glVertex3f(3000, 3000, gameLength-tra-4000);

        glTexCoord2f(1, 0);
        glVertex3f(-3000, 3000, gameLength-tra-4000);
    glEnd();
    glPopMatrix();
    */

    // Iskljucujemo aktivnu teksturu 
    glBindTexture(GL_TEXTURE_2D, 0);

    // Crtamo ostrvo (krug oko grada)
    drawBackground(400, 50);
    
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
        //Transliramo raketu za -tra. Azuriramo koordinate igraca
        glTranslatef(0, 0, -tra);
        glTranslatef(0, player.y, 0);
        glTranslatef(player.x, 0, 0);

        // Azuriramo z koordinate
        player.z = gameLength - 50;
        player.z = player.z - tra;
        
        // Testiramo da li je doslo do kolizije sa preprekama
        if (collision2(op, player, 1000, 20) <= 0)
        {
            m = 0;
        }
       
        // Testiramo da li je doslo do kolizije sa preprekama koje se krecu
        if (collision2(op2, player, 25, 30) <= 0)
        {
            m = 0;
        }
        
        // Testiramo da li je doslo do kolizije sa oblacima
        if (collision2(cl, player, 20, 300) <= 0)
        {

            // Ako je doslo do kolizije iskljucujemo LIGHT0 i ukljucujemo LIGHT1
            
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

    glPushMatrix();
        drawRocket();
    glPopMatrix();
    // Postavlja se nova slika u prozor
    glutSwapBuffers();
}


// Ucitavamo teksturu
void textureInit(Image * image,char * filename,int pointer)
{

    // Ucitavamo image i podesavamo parametre
    image_read(image,filename);
    glBindTexture(GL_TEXTURE_2D, textures[pointer]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    //Iskljucujemo aktivnu teksturu 
    glBindTexture(GL_TEXTURE_2D, 0);
}


// Crtamo skybox. Size oznacava velicinu kocke. N oznacava kolko cemo slika imati u teksturi
void drawSkybox(int size,int n)
{
    // Crtamo box 1
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBegin(GL_QUADS);
        glNormal3f(20, 20, 12000);

        glTexCoord2f(0, 0);
        glVertex3f(-size, 0, gameLength);

        glTexCoord2f(0, n);
        glVertex3f(-size, 0, 0);

        glTexCoord2f(n, n);
        glVertex3f(0, size, 0);

        glTexCoord2f(n, 0);
        glVertex3f(0, size, gameLength);
    glEnd();
    glPopMatrix();

    // Crtamo box 2
    glPushMatrix();
    
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glBegin(GL_QUADS);
        glNormal3f(20, 20, 12000);

        glTexCoord2f(0, 0);
        glVertex3f(size, 0, gameLength);

        glTexCoord2f(0, n);
        glVertex3f(size, 0, 0);

        glTexCoord2f(n, n);
        glVertex3f(0, size, 0);

        glTexCoord2f(n, 0);
        glVertex3f(0, size, gameLength);
    glEnd();
    glPopMatrix();
    // Crtamo box 3
    glPushMatrix();
    
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glBegin(GL_QUADS);
        glNormal3f(20, 20, 12000);

        glTexCoord2f(0, 0);
        glVertex3f(size, 0, gameLength);

        glTexCoord2f(0, n);
        glVertex3f(size, 0, 0);

        glTexCoord2f(1, n);
        glVertex3f(0, -size, 0);

        glTexCoord2f(n, 0);
        glVertex3f(0, -size, gameLength);
    glEnd();
    glPopMatrix();

    // Crtamo box 4
    glPushMatrix();
    
    glBindTexture(GL_TEXTURE_2D, textures[4]);
    glBegin(GL_QUADS);
        glNormal3f(20, 20, 12000);

        glTexCoord2f(0, 0);
        glVertex3f(-size, 0, gameLength);

        glTexCoord2f(0, n);
        glVertex3f(-size, 0, 0);

        glTexCoord2f(n, n);
        glVertex3f(0, -size, 0);

        glTexCoord2f(n, 0);
        glVertex3f(0, -size, gameLength);
    glEnd();
    glPopMatrix();







}