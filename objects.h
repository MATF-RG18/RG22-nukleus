#ifndef OBJECTS_H
#define OBJECTS_H

#include "config.h"

void drawObstacle(ObjectPosition op);

void drawRocket();

void  drawCylinder(GLfloat radius,GLfloat height);
void drawHorizontalWing();
void drawVerticalWing();
void drawBackground(GLfloat radius,GLfloat height);
void drawCity(ObjectPosition il,int buildingHeight);
void drawCloud(ObjectPosition cl);
#endif