/*******************************************************************
*
* PARAMETER.C
*
* Methods for parameters
*
*
* VERSION: 17.04.2015
*
* by Hendrik Hangmann
* University of Paderborn
*
********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "parameter.h"
#include "model.h"
#include "math.h"
#include "definitions.h"

void param_to_vector(parameter_set * param, float * vector, int num_layers, int nodes_per_layer)
{
	int i,j,k,l,n;
	for (i = 0; i < num_layers; i++) {
		vector[i] = param->capacitance_per_layer[i];
	}

	for (n = 0; n < NUM_HEATER_LEVELS; n++) {
		vector[i + n] = param->heatflow[n];
	}


	for (j = 0; j < num_layers - 1; j++) {
		vector[j + n + i] = param->resistance_inter_layer[j];
	}

/*	for (m = 0; m < nodes_per_layer; m++) {
		vector[j + 3 + i + m] = param->resistance_sink[m];
	}
*/
	vector[j + n + i] = param->resistance_sink;
	for (k = 0; k < num_layers; k++) {
		vector[j + n + i + k +1] = param->resistances_x_per_layer[k];
	}
	for (l = 0; l < num_layers; l++) {
		vector[j + n + i + k + l+1] = param->resistances_y_per_layer[l];
	}

	vector[j + n + i + k + l +1] = param->temperature_heat_sink;
}

void vector_to_param(float * vector, parameter_set * param, int num_layers,int num_nodes_per_layer)
{
	int i,j,k,l,n ;
	for (i = 0; i < num_layers; i++) {
		param->capacitance_per_layer[i]= vector[i];
	}

	for (n = 0; n < NUM_HEATER_LEVELS; n++) {
		param->heatflow[n] = vector[i + n];
	}

	for (j = 0; j < num_layers - 1; j++) {
		param->resistance_inter_layer[j] = vector[j + n + i];
	}

	/*for (m = 0; m < num_nodes_per_layer; m++) {
		param->resistance_sink[m] = vector[j + 3 + i + m];
	}*/
	param->resistance_sink = vector[j + n + i];
	for (k = 0; k < num_layers; k++) {
		param->resistances_x_per_layer[k] = vector[j + n + i + k + 1];
	}
	for (l = 0; l < num_layers; l++) {
		param->resistances_y_per_layer[l] = vector[j + n + i + k + l + 1];
	}

	param->temperature_heat_sink = vector[j + n + i + k + l  + 1];
}


void init_paramaters2 (parameter_set * params, int l, int nodes_per_layer)
{
	params->heatflow[16] = 729.0f ;//+ get_random_float(100.0f);
	params->heatflow[0] = 0.000000001f ;//+ get_random_float(0.1f);

	/*float arg=get_random_float(0.5f);
	for (i = 0; i < nodes_per_layer; i++) {
		params->resistance_sink[i] = 50.775294f ;//+ get_random_float(0.5f);;
	}*/
	params->resistance_sink = 50.775294f ;
	params->temperature_heat_sink = 19.0f;//+ get_random_float(5.0f);
	params->capacitance_per_layer[0] = 0.15f ;//+ get_random_float(0.1f);
	params->capacitance_per_layer[1] = 0.0000329150f ;//+ get_random_float(0.1f);
	params->capacitance_per_layer[2] = 0.0000000150f ;//+ get_random_float(0.1f);
	params->capacitance_per_layer[3] = 0.0000000010f ;//+ get_random_float(0.1f);
	params->resistances_x_per_layer[0] = 287.943773 ;//+ get_random_float(2.0f);
	params->resistances_x_per_layer[1] = 150.943773f ;//+ get_random_float(2.0f);
	params->resistances_x_per_layer[2] = 80.943773f ;//+ get_random_float(2.0f);
	params->resistances_y_per_layer[0] = 276.84f;// + get_random_float(1.0f);
	params->resistances_y_per_layer[1] = 150.84f;// + get_random_float(1.0f);
	params->resistances_y_per_layer[2] = 80.84f;// + get_random_float(1.0f);
	params->resistance_inter_layer[0] = 5.544872f;// + get_random_float(8.0f);
	params->resistance_inter_layer[1] = 5.544872f;// + get_random_float(8.0f);
}

void init_paramaters (parameter_set * params, int l, int nodes_per_layer)
{
	params->heatflow[16] = 729.0f ;//+ get_random_float(100.0f);
	params->heatflow[0] = 0.000000001f ;//+ get_random_float(0.1f);

	/*float arg=get_random_float(0.5f);
	for (i = 0; i < nodes_per_layer; i++) {
		params->resistance_sink[i] = 50.775294f ;//+ get_random_float(0.5f);;
	}
	*/
	params->resistance_sink = 1.775294f ;
	params->temperature_heat_sink = 19.0f;//+ get_random_float(5.0f);
	params->capacitance_per_layer[0] = 0.15f ;//+ get_random_float(0.1f);
	params->capacitance_per_layer[1] = 0.0000329150f ;//+ get_random_float(0.1f);
	params->resistances_x_per_layer[0] = 287.943773f ;//+ get_random_float(2.0f);
	params->resistances_x_per_layer[1] = 150.943773f ;//+ get_random_float(2.0f);
	params->resistances_y_per_layer[0] = 276.84f;// + get_random_float(1.0f);
	params->resistances_y_per_layer[1] = 150.84f;// + get_random_float(1.0f);
	params->resistance_inter_layer[0] = 5.544872f;// + get_random_float(8.0f);
}

void random_init_paramaters (parameter_set * params, int l, int nodes_per_layer)
{
	int i;

	float a = 750.0f;
	float add;

	add = get_random_float(750.0f/5000.0f);
	params->capacitance_per_layer[0] = 750.0f/5000.0f + 2 * add *  get_random_float(1.0f) - add;

	for (i = 1; i < l; i++) {
		add =get_random_float(a/pow(1000,i+1));
		params->capacitance_per_layer[i] = params->capacitance_per_layer[-i] - 2*add*get_random_float(1.0f) + add;
		if (params->capacitance_per_layer[i] <= 0.0000000001f) params->capacitance_per_layer[i] = 0.0000000001f;
	}

	a = 0.000000001f;
	add=get_random_float(a*10);
	params->heatflow[0] = a + 2*add*get_random_float(1.0f) - add;
	a = 729.0f;
	add=get_random_float(a/2);
	params->heatflow[16] = a + 2*add*get_random_float(1.0f) - add;

	a = 5.544872f;add=get_random_float(a/2);
	for (i = 0; i < l-1; i++) {
		params->resistance_inter_layer[i] = a + 2*add*get_random_float(1.0f) - add;//0.5f+get_random_float(a);
	}

	a = 50.775294f;add=get_random_float(a/2);
	params->resistance_sink=a + 2*add*get_random_float(1.0f) - add;
	/*for (i = 1; i < nodes_per_layer; i++) {
		params->resistance_sink[i]=params->resistance_sink[0];
	}*/

	a = 287.943773f;add=get_random_float(a/2);
	params->resistances_x_per_layer[0] = a + 2*add*get_random_float(1.0f) - add;
	for (i = 1; i < l; i++) {
		params->resistances_x_per_layer[i] = params->resistances_x_per_layer[i-1]/2;
	}

	a = 276.84f;add=get_random_float(a/2);
	params->resistances_y_per_layer[0] = a + 2*add*get_random_float(1.0f) - add;
	for (i = 1; i < l; i++) {
		params->resistances_y_per_layer[i] = params->resistances_y_per_layer[i-1]/2;
	}

	a = 19.0f;
	add=get_random_float(a/2);
	params->temperature_heat_sink= a+2*add*get_random_float(1.0f) - add;
}

void print_parameters(parameter_set * params, int num_layers, int nodes_per_layer) {
	int i;

	for (i = 0; i < NUM_HEATER_LEVELS; i++) {
		printf("\t\tHeatflow Level %2d: \t\t%.10f\n", i, params->heatflow[i]);
	}

	//printf("\t\tHeatflow Active: \t\t%.10f\n",params->heatflow[16]);

	printf("\t\tTemperature Heat Sink: \t\t%.10f\n",params->temperature_heat_sink);

	for (i = 0; i < num_layers; i++) {
		printf("\t\tCapacitance on Layer %d: \t%.10f\n", i, params->capacitance_per_layer[i]);
	}

	for (i = 0; i < num_layers -1; i++) {
		printf("\t\tResistances above Layer %d: \t%.10f\n", i, params->resistance_inter_layer[i]);
	}

	for (i = 0; i < num_layers; i++) {
		printf("\t\tX-Resistances on Layer %d: \t%.10f\n", i, params->resistances_x_per_layer[i]);
		printf("\t\tY-Resistances on Layer %d: \t%.10f\n", i, params->resistances_y_per_layer[i]);
	}

	//for (i = 0; i < nodes_per_layer; i++) {
		printf("\t\tResistance Sink: \t\t%.10f\n", params->resistance_sink);
	//}
}

void params_free(parameter_set * params)
{
	free(params->resistance_inter_layer);
	free(params->resistances_y_per_layer);
	free(params->resistances_x_per_layer);
	free(params->capacitance_per_layer);
	free(params->heatflow);
	free(params);
}
