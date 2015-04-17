/*******************************************************************
*
* DEFINITIONS.C
*
* Useful definitions and functions
*
*
* VERSION: 17.04.2015
*
* by Hendrik Hangmann
* University of Paderborn
*
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

float get_random_float(float a)
{
	return ((float)rand()/(float)(RAND_MAX)) * a;
}

float min(float a, float b)
{
	if (a>b) return b; else return a;
}
