/*******************************************************************
*
* PARAMETER.H
*
* Definitions for parameters, which will be learned and optimized
*
*
* VERSION: 17.04.2015
*
* by Hendrik Hangmann
* University of Paderborn
*
********************************************************************/



#ifndef PARAMETER_H_
#define PARAMETER_H_

#include "parameter.h"
#include "model.h"

typedef struct
{
	float * resistance_inter_layer; // #layers- 1 = #inter-layer resistances
	//float * resistance_sink;  // R_s
	float  resistance_sink;  // R_s
	float * resistances_x_per_layer;
	float * resistances_y_per_layer;
	float * capacitance_per_layer;
	float temperature_heat_sink;   //   temperature of heat sink
	float * heatflow;   // heat flows for layer 0, for each heater level

}parameter_set;


void param_to_vector(parameter_set * param, float * vector, int num_layers, int nodes_per_layer);
void vector_to_param(float * vector, parameter_set * param, int num_layers,int num_nodes_per_layer);
void init_paramaters2 (parameter_set * params, int l, int nodes_per_layer);
void init_paramaters (parameter_set * params, int l, int nodes_per_layer);
void random_init_paramaters (parameter_set * params, int l, int nodes_per_layer);
parameter_set * create_parameter_set(int x, int y, int l);
void params_free(parameter_set * params);


#endif
