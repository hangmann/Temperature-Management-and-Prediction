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
#include "measurement_grid.h"

#ifndef MAIN_H_
#define MAIN_H_



typedef struct
{
	float * resistance_inter_layer; // #layers- 1 = #inter-layer resistances
	float * resistance_sink;  // R_s
	float * resistances_x_per_layer;
	float * resistances_y_per_layer;
	float * capacitance_per_layer;
	float temperature_heat_sink;   //   temperature of heat sink
	float heatflow_active;   // heat flows for layer 0, if heat core on tile active
	float heatflow_inactive;  // heat flows for layer 0, if heat core on tile inactive

}parameter_set;


int num_lines = 0;

void open_measurements(FILE ** file, char ** path);
void parse_measurements(FILE ** file, int line_len, rc_network * rcn, measurement_grid * m_grid, int heater_size_x, int heater_size_y);
char * strtok(char * s, const char * toks);
parameter_set * create_parameter_set(int x, int y, int l);
void random_init_paramaters (parameter_set * params, int l, int nodes_per_layer);
void params_free(parameter_set * params);
float get_random_float(float a);
void print_parameters(parameter_set * params, int num_layers,int nodes_per_layer);
int main (int argc, char **argv);
void assign_parameters(parameter_set * params, rc_network * rcn, measurement_grid * m_grid);
float simulate_model(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, int t);
int get_neighbor_index_of(node temp_node, node_type type);
void annealing1 (measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float temp_start,  float temp_step,int equlibrium,float sigma, float * rmse);
void annealing2 (measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float temp_start,  float temp_step,int equlibrium,float sigma, float * rmse);
void param_annealing(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float step, float * param, float temp_start, float temp_step, int equlibrium_reached,float * rmse);

#endif /* MAIN_H_ */
