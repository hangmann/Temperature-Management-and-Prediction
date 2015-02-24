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

void parse_measurements(FILE ** file)
{
	char * timestep_measurement;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, *file)) != -1) {
		//printf("Retrieved line of length %zu :\n", read);
		//printf("%s", line);
		timestep_measurement = strtok (line," ");
		int i = 0;
		int second;
		float data[3];
		while (timestep_measurement != NULL)
		{
			if (i == 0) second = atoi(timestep_measurement);
			else data[i-1] = atof(timestep_measurement);
			i++;
			timestep_measurement = strtok (NULL, " ");
		}
		printf("%d: %f, %f, %f\n",second, data[0], data[1], data[2]);
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
	printf("Heatflow Active: \t\t%f\n",params->heatflow_active);
	printf("Heatflow Inctive: \t\t%f\n",params->heatflow_inactive);
	printf("Resistance Sink: \t\t%f\n",params->resistance_sink);
	printf("Temperature Heat Sink: \t\t%f\n",params->temperature_heat_sink);

	for (i = 0; i < num_layers; i++) {
		printf("Capacitance on Layer %d: \t%f\n", i, params->capacitance_per_layer[i]);
	}

	for (i = 0; i < num_layers -1; i++) {
		printf("Resistances above Layer %d: \t%f\n", i, params->resistance_inter_layer[i]);
	}

	for (i = 0; i < num_layers; i++) {
		printf("X-Resistances on Layer %d: \t%f\n", i, params->resistances_x_per_layer[i]);
		printf("Y-Resistances on Layer %d: \t%f\n", i, params->resistances_y_per_layer[i]);
	}
}

void params_free(parameter_set * params)
{
	assert(params);
	free(params->resistance_inter_layer);
	free(params->resistances_y_per_layer);
	free(params->resistances_x_per_layer);
	free(params->capacitance_per_layer);
	free(params);
}

int main (int argc, char **argv)
{
	FILE * file;
	rc_network * rcn;
	parameter_set * params;
	int sensor_size_x, sensor_size_y =0;
	int heater_size_x, heater_size_y=0;
	int num_layers =0;

	if ( argc != 7 )
	{
		printf( "Start this program as follows:\n %s PATH_TO_MEASUREMENTS  WIDTH_OF_HEATER_GRID  HEIGHT_OF_HEATER_GRID  WIDTH_OF_SENSOR_GRID  HEIGHT_OF_SENSOR_GRID  NUMBER_OF_LAYERS\n", argv[0] );
	}
	else
	{
		open_measurements(&file, &argv[1]);
		parse_measurements(&file);

		heater_size_x = atoi(argv[2]);
		heater_size_y = atoi(argv[3]);
		sensor_size_x = atoi(argv[4]);
		sensor_size_y = atoi(argv[5]);
		num_layers = atoi(argv[6]);

		printf("Creating RC Network with %d layers and a %dx%d grid... ", num_layers, sensor_size_x, sensor_size_y);
		rcn = create_rcn(sensor_size_x, sensor_size_y, num_layers);
		printf("Done!\nInitializing RC Network... ");
		init(rcn);

		params = create_parameter_set(sensor_size_x, sensor_size_y, num_layers);
		printf("Done!\nCreating Random Parameter Set... ");
		random_init_paramaters(params, num_layers, heater_size_x * heater_size_y);
		printf("Done!\nParameters are:\n");
		print_parameters(params, num_layers);
		assign_parameters(params, num_layers, sensor_size_x, sensor_size_y);
	}

	params_free(params);
	rcn_free(rcn);
    exit(EXIT_SUCCESS);
}
