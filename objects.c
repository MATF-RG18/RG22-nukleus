#include "objects.h"
#define PI 3.1415927


// Da ne bi uvek racunali sve elemente za koliziju. One koje smo prosli preskacemo
int skip = 0;

//Parametri za osvetljenje
GLfloat ambObstacle[] = {0.6, 0, 0.015, 1};
GLfloat specular_coeffs[] = {1, 1, 1, 0.7};
GLfloat ambRocketHead[] = {1, 1, 1, 1};
GLfloat ambRocketBody[] = {1, 1, 1, 1};
GLfloat ambRocketBottom[] = {0.5, 0.5, 0.5, 1};
GLfloat ambRocketTail[] = {1, 0, 0, 1};
GLfloat ambIsland[] = {0, 1, 0, 1};
GLfloat ambCity[] = {0.5, 0.5, 0.5, 1};
GLfloat ambCloud[] = {0.949, 0.984, 0.992, 0.8};
GLfloat ambObstacle2[] = {0,0, 0, 1};
//Crtamo raketu
void drawRocket()
{

    //Postavljamo raketu na pocetak nivoa
    glTranslatef(0, 0, gameLength - 50);

    //Rotiramo raketu
    //Da bi se videlo kako cela raketa izgleda staviti glRotatef pod komentar
    glRotatef(-70, 1, 0, 0);

    glPushMatrix();

    //Ovde crtamo vrh rakete
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambRocketBottom);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambRocketBottom);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(3, 3, 25, 10);

    //Ovde crtamo telo rakete
    glPushMatrix();

    glRotatef(270, 1, 0, 0);
    glRotatef(-90, 1, 0, 0);
    //Crtamo telo rakete
    drawCylinder(3, 18);
    glPopMatrix();

    // Ovde crtamo horizontalna krila
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(-5, -17.5, -0.5);
    drawHorizontalWing();
    glPopMatrix();

    // Ovde crtamo vertikalna krila
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(-1, -17.5, -2.5);
    drawVerticalWing();
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
}

// Funkcija za crtanje tela rakete
void drawCylinder(GLfloat radius, GLfloat height)
{
    GLfloat x = 0.0;
    GLfloat y = 0.0;
    GLfloat angle = 0.0;
    GLfloat angle_stepsize = 0.1;

    glPushMatrix();

    glMaterialfv(GL_BACK_LEFT, GL_AMBIENT, ambRocketBody);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ambRocketBody);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_BACK_LEFT, GL_SHININESS, 15);

    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    while (angle < 2 * PI)
    {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
        glVertex3f(x, y, 0.0);
        glNormal3f(y, 0, x);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0.0, height);
    glVertex3f(radius, 0.0, 0.0);
    glEnd();
    glPopMatrix();

    // Ovde "zaklapamo" raketu
    glPushMatrix();

    glMaterialfv(GL_BACK_LEFT, GL_AMBIENT, ambRocketBody);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ambRocketBody);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_BACK_LEFT, GL_SHININESS, 15);
    glBegin(GL_POLYGON);
    angle = 0.0;

    while (angle < 2 * PI)
    {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
        glNormal3f(y, 0, x);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0.0, height);

    glEnd();
    glPopMatrix();
}

// Funcija koja crta horizontalna krila. Sastoji se od 2 piramide
void drawHorizontalWing()
{
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambRocketTail);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambRocketTail);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 15);

    glScalef(5, 15, 1);
    glTranslatef(0,0,0);
    glBegin(GL_TRIANGLES);
    
    glVertex3f(1, 0, 0);
    glVertex3f(1, 0, 1);
    glVertex3f(0, 0, 0.5);

    glNormal3f(1, 0, 0);
    glNormal3f(1, 0, 1);
    glNormal3f(0, 0, 0.5);

    glVertex3f(1, 0, 0);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 1, 0.5);

    glNormal3f(1, 0, 0);
    glNormal3f(1, 0, 1);
    glNormal3f(1, 1, 0.5);

    glVertex3f(1, 0, 0);
    glVertex3f(0, 0, 0.5);
    glVertex3f(1, 1, 0.5);

    glNormal3f(1, 0, 0);
    glNormal3f(0, 0, 0.5);
    glNormal3f(1, 1, 0.5);

    glVertex3f(0, 0, 0.5);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 1, 0.5);

    glNormal3f(0, 0, 0.5);
    glNormal3f(1, 0, 1);
    glNormal3f(1, 1, 0.5);

    
    //Drugo krilo

    glVertex3f(1, 0, 0);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 1, 0.5);

    glNormal3f(1, 0, 0);
    glNormal3f(1, 0, 1);
    glNormal3f(1, 1, 0.5);

    glVertex3f(1, 0, 0);
    glVertex3f(1, 0, 1);
    glVertex3f(2, 0, 0.5);

    glNormal3f(1, 0, 0);
    glNormal3f(1, 0, 1);
    glNormal3f(2, 0, 0.5);

    glVertex3f(1, 0, 0);
    glVertex3f(2, 0, 0.5);
    glVertex3f(1, 1, 0.5);

    glNormal3f(1, 0, 0);
    glNormal3f(2, 0, 0.5);
    glNormal3f(1, 1, 0.5);

    glVertex3f(1, 1, 0.5);
    glVertex3f(1, 0, 1);
    glVertex3f(2, 0, 0.5);

    glNormal3f(1, 1, 0.5);
    glNormal3f(1, 0, 1);
    glNormal3f(2, 0, 0.5);
    glEnd();
    glPopMatrix();
}
//Funcija koja crta vertikalna krila. Sastoji se od 2 piramide
void drawVerticalWing()
{
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambRocketTail);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambRocketTail);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 15);

    glScalef(1, 15, 5);
    //Trece krilo
    glBegin(GL_TRIANGLES);
    glVertex3f(0.5, 0, 0.5);
    glVertex3f(1.5, 0, 0.5);
    glVertex3f(1, 0, -0.5);

    glNormal3f(0.5, 0, 0.5);
    glNormal3f(1.5, 0, 0.5);
    glNormal3f(1, 0, -0.5);

    glVertex3f(0.5, 0, 0.5);
    glVertex3f(1.5, 0, 0.5);
    glVertex3f(1, 1, 0.5);

    glNormal3f(0.5, 0, 0.5);
    glNormal3f(1.5, 0, 0.5);
    glNormal3f(1, 1, 0.5);

    glVertex3f(0.5, 0, 0.5);
    glVertex3f(1, 0, -0.5);
    glVertex3f(1, 1, 0.5);

    glNormal3f(0.5, 0, 0.5);
    glNormal3f(1, 0, -0.5);
    glNormal3f(1, 1, 0.5);

    glVertex3f(1.5, 0, 0.5);
    glVertex3f(1, 0, -0.5);
    glVertex3f(1, 1, 0.5);

    glNormal3f(1.5, 0, 0.5);
    glNormal3f(1, 0, -0.5);
    glNormal3f(1, 1, 0.5);

    //Cetvrto krilo

    glVertex3f(0.5, 0, 0.5);
    glVertex3f(1.5, 0, 0.5);
    glVertex3f(1, 0, 1.5);

    glNormal3f(0.5, 0, 0.5);
    glNormal3f(1.5, 0, 0.5);
    glNormal3f(1, 0, 1.5);

    glVertex3f(0.5, 0, 0.5);
    glVertex3f(1, 0, 1.5);
    glVertex3f(1, 1, 0.5);

    glNormal3f(0.5, 0, 0.5);
    glNormal3f(1, 0, 1.5);
    glNormal3f(1, 1, 0.5);

    glVertex3f(1.5, 0, 0.5);
    glVertex3f(1, 0, 1.5);
    glVertex3f(1, 1, 0.5);

    glNormal3f(1.5, 0, 0.5);
    glNormal3f(1, 0, 1.5);
    glNormal3f(1, 1, 0.5);
    glEnd();
    glPopMatrix();
}

//Funcija za crtanje prepreka
void drawObstacle(ObjectPosition op)
{
    glPushMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambObstacle);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ambObstacle);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
    glTranslatef(op.x, op.y, op.z);
    //glutSolidCube(20);
    glutSolidSphere(10, 25, 25);
    glPopMatrix();
}

//Funcija za crtanje kruga oko grada
void drawBackground(GLfloat radius, GLfloat height)
{
    GLfloat x = 0.0;
    GLfloat y = 0.0;
    GLfloat angle = 0.0;
    glTranslatef(0,0,0);
    glPushMatrix();
    glMaterialfv(GL_BACK_LEFT, GL_AMBIENT, ambIsland);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ambIsland);
    glMaterialf(GL_FRONT, GL_SHININESS, 15);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glBegin(GL_POLYGON);
    //int i = 0;
    for(angle=0;angle<2*PI;angle+=PI/15)
    {
        glVertex3f(radius*cos(angle), radius*sin(angle), 1);
        glNormal3f(radius*cos(angle), radius*sin(angle), 1);
        glNormal3f(radius*cos(angle+PI/50), radius*sin(angle+PI/50), 1);
        //glNormal3f(x, y, 1);
    }

    glEnd();

    glNormal3f(x, y, 0.0);
    glPopMatrix();
}

// Funckija za crtanje cilja (mesto gde treba da padne raketa i gde se zavrsava nivo)
void drawCity(ObjectPosition il, int buildingHeight)
{
    int i = 0;
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambCity);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ambCity);
    // glMaterialfv(GL_FRONT,GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, 15);
    glTranslatef(il.x, il.y, il.z);
    glutSolidCube(25);

    glPushMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambCity);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ambCity);

    for (i = 0; i < buildingHeight; i++)
    {
        glTranslatef(0, 0, 25);
        glutSolidCube(25);
    }
    glPopMatrix();

    glPopMatrix();
}

// Funckija za crtanje oblaka
void drawCloud(ObjectPosition cl)
{

    glPushMatrix();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glTranslatef(0, 0, 2500);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambCloud);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ambCloud);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, 25);
    glTranslatef(cl.x, cl.y, cl.z);
    glutSolidSphere(500, 25, 25);

    glPopMatrix();
}


// Funckija za detekciju kolizije. (relativno optimizovana)
int collision2(ObjectPosition *obj, ObjectPosition player, int size, int radius)
{
    int j;
    float dx, dy, dz;
    float dist = 0;

    for (j = skip; j < size; j++)
    {

        dz = player.z - obj[j].z - 15;
        dx = player.x - obj[j].x;
        dy = player.y - obj[j].y;

        if (obj[j].z - 50 > player.z)
            skip++;

       

        dist = sqrt(pow(dz, 2) + pow(dx, 2) + pow(dy, 2));

        //printf("%f\n",dist);
        if (dist <= radius)
            return -1;
    }
    return 1;
}
// Funkcija za detekciju kolizije kod oblaka 
int collisionC(ObjectPosition *obj, ObjectPosition player, int size, int radius)
{
    int j;
    float dx, dy, dz;
    float dist = 0;

    for (j = 0; j < size; j++)
    {
        dz = player.z - obj[j].z - 15;
        dx = player.x - obj[j].x;
        dy = player.y - obj[j].y;
        dist = sqrt(pow(dz, 2) + pow(dx, 2) + pow(dy, 2));

        //printf("%f\n",dist);
        if (dist <= radius)
            return -1;
    }
    return 1;
}




// Crtamo prepreke koje se krecu
void drawMovingObstacle(ObjectPosition op)
{
    glPushMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambObstacle2);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ambObstacle2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, 5);
    glTranslatef(op.x, op.y, op.z);
    glutSolidSphere(25, 25, 25);
    glPopMatrix();

}
