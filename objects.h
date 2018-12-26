#ifndef OBJECTS_H
#define OBJECTS_H

#include "config.h"

void drawObstacle(ObjectPosition op);

void drawRocket();

void drawCylinder(GLfloat radius, GLfloat height);
void drawHorizontalWing();
void drawVerticalWing();
void drawBackground(GLfloat radius, GLfloat height);
void drawCity(ObjectPosition il, int buildingHeight);
void drawCloud(ObjectPosition cl);
int collision(ObjectPosition obj, ObjectPosition player, int size);
int collision2(ObjectPosition *obj, ObjectPosition player, int size, int radius);
void drawMovingObstacle(ObjectPosition op);
int collisionC(ObjectPosition *obj, ObjectPosition player, int size, int radius);

#endif