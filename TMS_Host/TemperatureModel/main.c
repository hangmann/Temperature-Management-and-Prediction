/*******************************************************************
*
* MAIN.C
*
* Program's main file. Instantiates models, learns parameters, etc
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
#include <limits.h>
#include <assert.h>
#include <time.h>
#include "main.h"
#include "model.h"
#include "measurement_grid.h"

/**
 * Splits a char array in several char arrays
 * at given token divider 'toks'
 */
char * strtok(char * s, const char * toks)
 {
	static char * last = 0;
	char * ret = 0;
	if (toks) {
		s = s ? s : last;
		if (s) {
			unsigned long tokmap[1 << (CHAR_BIT - 5)] = {0};
			/* ^ a map of token dividers, containing 256 bits. */
			char * p;
			unsigned char tmp;
			while (tmp = (unsigned char) *toks++) {
				tokmap[ (tmp & ~31) >> 5 ] |= 1u << (tmp & 31);
			}
			p = s;
			while (tokmap[(*p & ~31) >> 5 ] & (1u << (*p & 31))) {
				++p;
			}

			/* no tokens left */
			if (! *p) {
				last = 0;
			} else {
				ret = p;
				tokmap[0] |= 1;
				do {
					++p;
				} while (!(tokmap[(*p & ~31) >> 5] &
					  (1u << (*p & 31))));
				*p = 0;
				last = p + 1;
			}
		}
	}
	return ret;
 }

/**
 * tries top open file from path
 */
void open_measurements(FILE ** file, char ** path)
{
	// argv[1] should be filename
	*file = fopen( *path, "r" );

	/* fopen returns 0, the NULL pointer, on failure */
	if ( *file == 0 )
	{
		printf( "ERROR: Could not open file\n" );
		exit(EXIT_FAILURE);
	}
}

void parse_measurements(FILE ** file, rc_network * rcn, measurement_grid * m_grid, int heater_size_x, int heater_size_y)
{
	char * timestep_measurement;
	int max_line_len = 267;
	char line[max_line_len];
	int i, j, number_of_temperatures, number_of_heaters;
	int linenumber;
	int second;
	linenumber=0;
	float vcc, temperature_diode;
	float temp;
	int temp_heater;

	/**
	 * structure of a line:
	 * 1. Infonumber (3 is Output)
	 * 2. timestamp
	 * 3. temperatures (3 - (3 + #sensors))
	 * 4. VCC (at 3 + #sensors + 1)
	 * 5. Heater levels (starting at 3 + #sensors + 2, ending at 3 + #sensors + 2 + #heaters)
	 */
	while(fgets(line, max_line_len, *file)) {
		timestep_measurement = strtok(line," ");
		i = 0;
		number_of_temperatures = 0;
		number_of_heaters = 0;

		while (timestep_measurement != NULL)
		{
			if (i==0){
				// line info number
			} else if (i == 1) {
				second = atoi(timestep_measurement);
			} else if ( 1 < i && i < 2 + rcn->num_nodes_per_layer) {
				temp =atof(timestep_measurement);
				m_grid[linenumber].temperature[number_of_temperatures] = temp;
				number_of_temperatures++;
			} else if (2 + rcn->num_nodes_per_layer == i) {
				temperature_diode = atof(timestep_measurement);
			} else if (3 + rcn->num_nodes_per_layer == i) {
				vcc = atof(timestep_measurement);
			} else if (3 + rcn->num_nodes_per_layer < i && i < 4 + rcn->num_nodes_per_layer + heater_size_x*heater_size_y -3 ) {
				temp_heater = atoi(timestep_measurement);
				m_grid[linenumber].heater_level[number_of_heaters] = temp_heater;
				number_of_heaters++;
			} else {
				// nothing. error
			}

			timestep_measurement = strtok (NULL, " ");
			i++;
		}
/*
		printf("\tRead %d temperatures...\n", number_of_temperatures);
		printf("\tRead %d heat codes...\n", number_of_heaters);
		printf("\tRead vcc %f...\n", vcc);
		printf("\tRead diode %f...\n", temperature_diode);
*/

		m_grid[linenumber].size_x=rcn->size_x;
		m_grid[linenumber].size_y=rcn->size_y;
		m_grid[linenumber].num_nodes=rcn->num_nodes;

		linenumber++;

	}

	fclose( *file );
}

float get_random_float(float a)
{
	return ((float)rand()/(float)(RAND_MAX)) * a;
}

void random_init_paramaters (parameter_set * params, int l, int nodes_per_layer)
{
	int i;

	float a = 100.0f;
	srand((unsigned int)time(NULL));

	for (i = 0; i < l; i++) {
		params->capacitance_per_layer[i] = get_random_float(a);
	}

	a = 10.0f;
	params->heatflow_active = get_random_float(a);

	params->heatflow_inactive = get_random_float(a);

	a=150.0f;
	for (i = 0; i < l-1; i++) {
		params->resistance_inter_layer[i] = get_random_float(a);
	}

	a = 100.0f;
	params->resistance_sink=get_random_float(a);

	a = 150.0f;
	for (i = 0; i < l; i++) {
		params->resistances_x_per_layer[i] = get_random_float(a);
	}

	a = 150.0f;
	for (i = 0; i < l; i++) {
		params->resistances_y_per_layer[i] = get_random_float(a);
	}

	a = 100;
	params->temperature_heat_sink=get_random_float(a);;


}

parameter_set * create_parameter_set(int x, int y, int l)
{
	parameter_set * params;

	params = malloc(sizeof * params);
	assert(params);

	int l_interconnects = l - 1;

	params->resistance_inter_layer = malloc(l_interconnects * sizeof *params->resistance_inter_layer); // one resistance per connection between layers
	assert(params->resistance_inter_layer);

	params->resistances_x_per_layer = malloc(l * sizeof *params->resistances_x_per_layer);
	assert(params->resistances_x_per_layer);

	params->resistances_y_per_layer = malloc(l * sizeof *params->resistances_y_per_layer);
	assert(params->resistances_y_per_layer);

	params->capacitance_per_layer = malloc(l * sizeof *params->capacitance_per_layer); // one capacitance per layer
	assert(params->capacitance_per_layer);

	int var;

	for (var = 0; var < l; var++) {
		params->resistances_x_per_layer[var] = 1.0f;
		params->resistances_y_per_layer[var] = 1.0f;
	}
	for (var = 0; var < l-1; var++) {
		params->resistance_inter_layer[var] = 1.0f;
	}

	return params;
}

void print_parameters(parameter_set * params, int num_layers)
{
	int i;
	printf("\t\tHeatflow Active: \t\t%f\n",params->heatflow_active);
	printf("\t\tHeatflow Inctive: \t\t%f\n",params->heatflow_inactive);
	printf("\t\tResistance Sink: \t\t%f\n",params->resistance_sink);
	printf("\t\tTemperature Heat Sink: \t\t%f\n",params->temperature_heat_sink);

	for (i = 0; i < num_layers; i++) {
		printf("\t\tCapacitance on Layer %d: \t%f\n", i, params->capacitance_per_layer[i]);
	}

	for (i = 0; i < num_layers -1; i++) {
		printf("\t\tResistances above Layer %d: \t%f\n", i, params->resistance_inter_layer[i]);
	}

	for (i = 0; i < num_layers; i++) {
		printf("\t\tX-Resistances on Layer %d: \t%f\n", i, params->resistances_x_per_layer[i]);
		printf("\t\tY-Resistances on Layer %d: \t%f\n", i, params->resistances_y_per_layer[i]);
	}
}

void params_free(parameter_set * params)
{
	free(params->resistance_inter_layer);
	free(params->resistances_y_per_layer);
	free(params->resistances_x_per_layer);
	free(params->capacitance_per_layer);
	free(params);
}

/**
 * assign parameters to RCN model
 */
void assign_parameters(parameter_set * params, rc_network * rcn, measurement_grid * m_grid)
{
	//printf("Temp: %f\n", m_grid[10153].nodes[0].temperature);
	int i, j, k, l;
	l = 0;
	for (i = 0; i < rcn->num_nodes; i++) {
		rcn->nodes[i].inv_capacity = params->capacitance_per_layer[rcn->nodes[i].layer];
		for (j = 0; j < rcn->nodes[i].num_neighbors; j++) {
			if (rcn->nodes[i].neighbor_type[j] == Z_UP && rcn->nodes[i].layer == rcn->num_layers - 1) {
				rcn->nodes[i].inv_resistance[j] = params->resistance_sink;
			}
			if (rcn->nodes[i].neighbor_type[j] == Z_UP && rcn->nodes[i].layer != rcn->num_layers - 1) {
				rcn->nodes[i].inv_resistance[j] = params->resistance_inter_layer[rcn->nodes[i].layer];
			}
			if (rcn->nodes[i].neighbor_type[j] == Z_DOWN) {
				rcn->nodes[i].inv_resistance[j] = params->resistance_inter_layer[rcn->nodes[i].layer - 1];
			}
			if (rcn->nodes[i].neighbor_type[j] == Y_UP || rcn->nodes[i].neighbor_type[j] == Y_DOWN) {
				rcn->nodes[i].inv_resistance[j] = params->resistances_y_per_layer[rcn->nodes[i].layer];
			}
			if (rcn->nodes[i].neighbor_type[j] == X_UP || rcn->nodes[i].neighbor_type[j] == X_DOWN) {
				rcn->nodes[i].inv_resistance[j] = params->resistances_x_per_layer[rcn->nodes[i].layer];
			}
		}
		rcn->nodes[i].heatflow = params->heatflow_inactive;			// TODO Active? Inactive?
		rcn->temperature_sink = params->temperature_heat_sink;

		/*
		 * Setting initial temperatures for heat source layer (= 0)
		 */
		if (rcn->nodes[i].layer == 0)
		{
			rcn->nodes[i].temperature = m_grid[0].temperature[l]; // m_grid[0] is first timestep of measurements
			l++;
		}
	//	printf("%f\n", rcn->nodes[i].temperature);
	}

}

/**
 * find index of neighbor in specified direction
 */
int get_neighbor_index_of(node temp_node, enum node_type type)
{
	int i;
	//printf("Getting index\n");
	for (i = 0; i < temp_node.num_neighbors; i++) {
		if (temp_node.neighbor_type[i] == type) {
			return i;
		}
	}
	return -1;
}

/**
 * check and print RCN values
 */
void print_rcn(rc_network * rcn)
{
	int i, j, k, node_index, r_index;

	for (i = 0; i < rcn->num_layers; i++) {
		r_index = get_neighbor_index_of(rcn->nodes[rcn->num_nodes_per_layer * i], Z_UP);

		printf("Inter Layer (%d-%d) resistance:\t\t\t%f\n", i, i + 1, rcn->nodes[rcn->num_nodes_per_layer * i].inv_resistance[r_index]);
		for (j = 0; j < rcn->size_x; j++) {
			for (k = 0; k < rcn->size_y; k++) {
	            node_index = i * rcn->num_nodes_per_layer + j * rcn->size_x + k;
	    		r_index = get_neighbor_index_of(rcn->nodes[node_index], X_UP);
	            printf("X-Resistances on layer/node %d/%d are:\t\t%f\n", rcn->nodes[node_index].layer, node_index, rcn->nodes[node_index].inv_resistance[r_index]);
	    		r_index = get_neighbor_index_of(rcn->nodes[node_index], Y_UP);
	            printf("Y-Resistances on layer/node %d/%d are:\t\t%f\n", rcn->nodes[node_index].layer, node_index, rcn->nodes[node_index].inv_resistance[r_index]);

	            printf("Capacity on layer/node %d/%d are:    \t\t%f\n", rcn->nodes[node_index].layer, node_index, rcn->nodes[node_index].inv_capacity);
			}
		}
	}
}

int main (int argc, char **argv)
{
	FILE * file;
	rc_network * rcn;
	parameter_set * params;
	int sensor_size_x, sensor_size_y =0;
	int heater_size_x, heater_size_y=0;
	int num_layers =0;
	measurement_grid * m_grid;

	if ( argc != 7 )
	{
		printf( "Start this program as follows:\n %s PATH_TO_MEASUREMENTS  WIDTH_OF_HEATER_GRID  HEIGHT_OF_HEATER_GRID  WIDTH_OF_SENSOR_GRID  HEIGHT_OF_SENSOR_GRID  NUMBER_OF_LAYERS\n", argv[0] );
	}
	else
	{
		heater_size_x = atoi(argv[2]);
		heater_size_y = atoi(argv[3]);
		sensor_size_x = atoi(argv[4]);
		sensor_size_y = atoi(argv[5]);
		num_layers = atoi(argv[6]);

		open_measurements(&file, &argv[1]);

		printf("Creating RC Network with %d layers and a %dx%d grid... ", num_layers, sensor_size_x, sensor_size_y);
		rcn = create_rcn(sensor_size_x, sensor_size_y, num_layers);
		printf("Done!\nInitializing RC Network... ");fflush(stdout);
		init(rcn);

		params = create_parameter_set(sensor_size_x, sensor_size_y, num_layers);
		printf("Done!\n");
		printf("Parsing Measurements... ");fflush(stdout);

		m_grid = create_grid(sensor_size_x, sensor_size_y);
		init_grid(m_grid);
		parse_measurements(&file, rcn, m_grid, heater_size_x, heater_size_y);

		printf("Done!\nCreating Random Parameter Set... ");fflush(stdout);
		random_init_paramaters(params, num_layers, heater_size_x * heater_size_y);
		printf("Done!\n\tParameters are:\n");fflush(stdout);
		print_parameters(params, num_layers);
		printf("Assigning Parameters to RC Model... ");fflush(stdout);
		assign_parameters(params, rcn, m_grid);
		printf("Done!\n");fflush(stdout);
		//print_rcn(rcn);
		printf("Learning Parameters... ");fflush(stdout);
		// learn
		printf("Needs to be implemented.\n");fflush(stdout);
	}

	printf("Freeing parameters... ");fflush(stdout);
	params_free(params);
	printf("Done!\nFreeing RCN...");fflush(stdout);
	rcn_free(rcn);
	printf("Done!\nFreeing MGRID...");fflush(stdout);
	mgrid_free(m_grid);
	printf("Done!\nExiting...");fflush(stdout);
    exit(EXIT_SUCCESS);
}
