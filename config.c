#include "config.h"

//Duzina nivoa
int gameLength = 10000;

int compare(const void *s1, const void *s2)
{
    ObjectPosition *op1 = (ObjectPosition *)s1;
    ObjectPosition *op2 = (ObjectPosition *)s2;
    return op2->z - op1->z;
}