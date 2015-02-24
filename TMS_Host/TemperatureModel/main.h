/*******************************************************************
*
* MAIN.H
*
* Definitions for main program
*
*
* VERSION: 24.02.2015
*
* by Hendrik Hangmann
* University of Paderborn
*
********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#ifndef MAIN_H_
#define MAIN_H_

typedef struct
{
	//TODO
}parameter_set;

void open_measurements(FILE ** file, char ** path);
void parse_measurements(FILE ** file);
char * strtok(char * s, const char * toks);

#endif /* MAIN_H_ */
