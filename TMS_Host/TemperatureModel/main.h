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
#include "model.h"

#ifndef MAIN_H_
#define MAIN_H_



typedef struct
{
	float * resistance_inter_layer; // #layers- 1 = #inter-layer resistances
	float resistance_sink;  // R_s
	float * resistances_x_per_layer;
	float * resistances_y_per_layer;
	float * capacitance_per_layer;
	float temperature_heat_sink;   //   temperature of heat sink
	float heatflow_active;   // heat flows for layer 0, if heat core on tile active
	float heatflow_inactive;  // heat flows for layer 0, if heat core on tile inactive

}parameter_set;

void open_measurements(FILE ** file, char ** path);
void parse_measurements(FILE ** file);
char * strtok(char * s, const char * toks);
parameter_set * create_parameter_set(int x, int y, int l);
void random_init_paramaters (parameter_set * params, int l, int nodes_per_layer);
void params_free(parameter_set * params);
float get_random_float(float a);
void print_parameters(parameter_set * params, int num_layers);
int main (int argc, char **argv);

#endif /* MAIN_H_ */
