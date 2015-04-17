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



#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "measurement_grid.h"
#include "optimizing.h"
#include "parameter.h"

void open_measurements(FILE ** file, char ** path);
void parse_measurements(FILE ** file, int line_len, rc_network * rcn, measurement_grid * m_grid, int heater_size_x, int heater_size_y);
char * strtok(char * s, const char * toks);
void change_by_stage (parameter_set * new_params, parameter_set * params, float start_percentage, rc_network * rcn, int stage);
void change (parameter_set * new_params, parameter_set * params, float start_percentage, rc_network * rcn);
float get_random_float(float a);
void print_parameters(parameter_set * params, int num_layers,int nodes_per_layer);
int main (int argc, char **argv);
float simulate_model(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, int t);
int get_neighbor_index_of(node temp_node, node_type type);

void assign_parameters_to_RCN(parameter_set * params, rc_network * rcn, measurement_grid * m_grid);

#endif /* MAIN_H_ */
