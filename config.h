#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <sys/time.h>
#include <sys/types.h>
//Struktura gde cuvamo koordinate prepreka
typedef struct
{
    int x;
    int y;
    int z;
} ObjectPosition;

//Duzina nivoa
extern int gameLength;
int compare(const void *s1, const void *s2);
#endif