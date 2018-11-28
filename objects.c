#include "objects.h"
#define PI 3.1415927


    //Parametri za osvetljenje
    GLfloat ambObstacle[] = {1, 0, 0, 1};
    GLfloat specular_coeffs[] = { 1, 1, 1, 0.7 };
    GLfloat ambRocketHead[] = { 1, 1, 1, 1 };
    GLfloat ambRocketBody[] = { 1, 1, 1, 1 };
    GLfloat ambRocketBottom[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat ambRocketTail[] = {1, 0, 0, 1};


//Crtamo raketu
void drawRocket()
{

    //Postavljamo raketu na pocetak nivoa
    glTranslatef(0,0,gameLength-50);
    

    //Rotiramo raketu 
    //Da bi se videlo kako cela raketa izgleda staviti glRotatef pod komentar
    glRotatef(-110,1,0,0);
   
    glPushMatrix();

        //Ovde crtamo vrh rakete
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambRocketBottom);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambRocketBottom);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, specular_coeffs);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
        glRotatef(-90,1,0,0);
        glutSolidCone(5,5,25,10);
        

        //Ovde crtamo telo rakete
        glPushMatrix();
       
            glRotatef(270,1,0,0);
            glRotatef(-90,1,0,0);

            drawCylinder(5,20);
        glPopMatrix();
        

       // Ovde crtamo horizontalna krila
        glPushMatrix();
            glRotatef(90,1,0,0);
            glTranslatef(-8,-19,0);
            drawHorizontalWing();
        glPopMatrix();
        

       // Ovde crtamo vertikalna krila
       glPushMatrix();
            glRotatef(90,1,0,0);
            glTranslatef(-1,-19,-4);
            drawVerticalWing();
       glPopMatrix();

   glPopMatrix();
glPopMatrix();
}



// Funkcija za crtanje tela rakete
void  drawCylinder(GLfloat radius,GLfloat height)
{
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    glPushMatrix();

        glMaterialfv(GL_BACK_LEFT, GL_AMBIENT, ambRocketBody);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, ambRocketBody);
        glMaterialfv(GL_FRONT,GL_SPECULAR, specular_coeffs);
        glMaterialf(GL_BACK_LEFT, GL_SHININESS, 15);

    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);    
            glNormal3f(y,0,x);
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
        glMaterialfv(GL_FRONT,GL_SPECULAR, specular_coeffs);
        glMaterialf(GL_BACK_LEFT, GL_SHININESS, 15);
    glBegin(GL_POLYGON);
    angle = 0.0;

        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glNormal3f(y,0,x);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);

    glEnd();
   glPopMatrix();
}


// Funcija koja crta horizontalna krila. Sastoji se od 2 trougla
void drawHorizontalWing()
{
  glPushMatrix();
     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambRocketTail);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambRocketTail);
     glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, specular_coeffs);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 15);

    glScalef(8,20,1);


    glBegin(GL_TRIANGLES);


        
        glVertex3f(1,0,0);
        glVertex3f(1,0,1);
        glVertex3f(0,0,0.5);
        
        glNormal3f(1,0,0);
        glNormal3f(1,0,1);
        glNormal3f(0,0,0.5);

        glVertex3f(1,0,0);
        glVertex3f(1,0,1);
        glVertex3f(1,1,0.5);

        glNormal3f(1,0,0);
        glNormal3f(1,0,1);
        glNormal3f(1,1,0.5);

        glVertex3f(1,0,0);
        glVertex3f(0,0,0.5);
        glVertex3f(1,1,0.5);

        glNormal3f(1,0,0);
        glNormal3f(0,0,0.5);
        glNormal3f(1,1,0.5);

        glVertex3f(1,0,0);
        glVertex3f(1,0,1);
        glVertex3f(1,1,0.5);
        
        glNormal3f(1,0,0);
        glNormal3f(1,0,1);
        glNormal3f(1,1,0.5);
        //Drugo krilo
        
        glVertex3f(1,0,0);
        glVertex3f(1,0,1);
        glVertex3f(1,1,0.5);

        glNormal3f(1,0,0);
        glNormal3f(1,0,1);
        glNormal3f(1,1,0.5);

        glVertex3f(1,0,0);
        glVertex3f(1,0,1);
        glVertex3f(2,0,0.5);

        glNormal3f(1,0,0);
        glNormal3f(1,0,1);
        glNormal3f(2,0,0.5);

        glVertex3f(1,0,0);
        glVertex3f(2,0,0.5);
        glVertex3f(1,1,0.5);

        glNormal3f(1,0,0);
        glNormal3f(2,0,0.5);
        glNormal3f(1,1,0.5);


        glVertex3f(1,1,0.5);
        glVertex3f(1,0,1);
        glVertex3f(2,0,0.5);

        glNormal3f(1,1,0.5);
        glNormal3f(1,0,1);
        glNormal3f(2,0,0.5);
    glEnd();
    glPopMatrix();
}
//Funcija koja crta vertikalna krila. Sastoji se od 2 trougla
void drawVerticalWing()
{
  glPushMatrix();
     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambRocketTail);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambRocketTail);
     glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, specular_coeffs);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 15);

    glScalef(1,20,8);
    //Trece krilo
    glBegin(GL_TRIANGLES);
        glVertex3f(0.5,0,0.5);
        glVertex3f(1.5,0,0.5);
        glVertex3f(1,0,-0.5);

        glNormal3f(0.5,0,0.5);
        glNormal3f(1.5,0,0.5);
        glNormal3f(1,0,-0.5);

        glVertex3f(0.5,0,0.5);
        glVertex3f(1.5,0,0.5);
        glVertex3f(1,1,0.5);

        glNormal3f(0.5,0,0.5);
        glNormal3f(1.5,0,0.5);
        glNormal3f(1,1,0.5);

        glVertex3f(0.5,0,0.5);
        glVertex3f(1,0,-0.5);
        glVertex3f(1,1,0.5);

        glNormal3f(0.5,0,0.5);
        glNormal3f(1,0,-0.5);
        glNormal3f(1,1,0.5);
        
        glVertex3f(1.5,0,0.5);
        glVertex3f(1,0,-0.5);
        glVertex3f(1,1,0.5);

        glNormal3f(1.5,0,0.5);
        glNormal3f(1,0,-0.5);
        glNormal3f(1,1,0.5);

        //Cetvrto krilo

        glVertex3f(0.5,0,0.5);
        glVertex3f(1.5,0,0.5);
        glVertex3f(1,0,1.5);

        glNormal3f(0.5,0,0.5);
        glNormal3f(1.5,0,0.5);
        glNormal3f(1,0,1.5);

        glVertex3f(0.5,0,0.5);
        glVertex3f(1,0,1.5);
        glVertex3f(1,1,0.5);

        glNormal3f(0.5,0,0.5);
        glNormal3f(1,0,1.5);
        glNormal3f(1,1,0.5);

        glVertex3f(1.5,0,0.5);
        glVertex3f(1,0,1.5);
        glVertex3f(1,1,0.5);

        glNormal3f(1.5,0,0.5);
        glNormal3f(1,0,1.5);
        glNormal3f(1,1,0.5);
    glEnd();
    glPopMatrix();
}

//Funcija za crtanje prepreka
void drawObstacle(ObjectPosition op)
{
            glPushMatrix();
        
            glMaterialfv(GL_FRONT, GL_AMBIENT, ambObstacle);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, ambObstacle);
            glMaterialfv(GL_FRONT,GL_SPECULAR, specular_coeffs);
            glMaterialf(GL_FRONT, GL_SHININESS, 50);
            glTranslatef(op.x ,op.y,op.z);
            //glutSolidCube(20);
            glutSolidSphere(10,25,25);
            glPopMatrix();
}
