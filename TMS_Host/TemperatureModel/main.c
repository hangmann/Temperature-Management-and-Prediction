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
#include <math.h>
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

int heater_position_to_sensor_position(int i)
{
	int k = i/5;
	int ret = 5*i - 24 * k;
	return ret;
}

void parse_measurements(FILE ** file, int line_len, rc_network * rcn, measurement_grid * m_grid, int heater_size_x, int heater_size_y)
{
	char * timestep_measurement;
	int max_line_len = line_len;//275;//51;
	char line[max_line_len];
	int i, j, number_of_temperatures, number_of_heaters;
	int linenumber;
	int line_info = -1;
	int second;
	linenumber=0;
	float vcc, temperature_diode;
	float temp;
	int temp_heater, pos;
	int num_heaters = heater_size_x*heater_size_y;

	/**
	 * structure of a line:
	 * 1. Infonumber (3 is Output)
	 * 2. timestamp
	 * 3. temperatures (3 - (3 + #sensors))
	 * 4. VCC (at 3 + #sensors + 1)
	 * 5. Heater levels (starting at 3 + #sensors + 2, ending at 3 + #sensors + 2 + #heaters)
	 */
	while(fgets(line, max_line_len, *file)) {
		//printf("%s\n\n",line);
		timestep_measurement = strtok(line," ");
		i = 0;
		number_of_temperatures = 0;
		number_of_heaters = 0;
		temp = -2.0f;
		while (timestep_measurement != NULL)
		{
			if (i==0)
				line_info = atoi(timestep_measurement);
			if (line_info == 3)
			{
				if (i == 1) {
					second = atoi(timestep_measurement);
					m_grid[linenumber].timestep = second;
					//printf("Second %d\n",second);
				} else if ( 1 < i && i < 2 + rcn->num_nodes_per_layer) {
					temp =atof(timestep_measurement);
					m_grid[linenumber].temperature[number_of_temperatures] = temp;
					number_of_temperatures++;
				} else if (2+ rcn->num_nodes_per_layer == i) {
					temperature_diode = atof(timestep_measurement);
				//	printf("Tempdiode %f\n",temperature_diode);
				} else if (3 + rcn->num_nodes_per_layer == i) {
					vcc = atof(timestep_measurement);
				//	printf("vcc %f\n",vcc);
				} else if (3 + rcn->num_nodes_per_layer < i && i < 4 + rcn->num_nodes_per_layer + num_heaters) {
					temp_heater = atoi(timestep_measurement);
					pos = i - (4 + rcn->num_nodes_per_layer);//heater_position_to_sensor_position(i - (4 + rcn->num_nodes_per_layer));
					m_grid[linenumber].heater_level[pos] = temp_heater;
					//printf("%d\n",temp_heater);
					number_of_heaters++;
				//	printf("heater %d\n",temp_heater);
				} else {
				//	printf("ERROR? %d, %f\n", atoi(timestep_measurement), atof(timestep_measurement));
				//	timestep_measurement = NULL;
					// nothing. error
				}
			}

			timestep_measurement = strtok (NULL, " ");
			i++;
		}
		//printf("Number of heaters%d\n", number_of_heaters);



/*
		printf("\tRead %d temperatures...\n", number_of_temperatures);
		printf("\tRead %d heat codes...\n", number_of_heaters);
		printf("\tRead vcc %f...\n", vcc);
		printf("\tRead diode %f...\n", temperature_diode);
*/

	//	printf("Line %d\n", linenumber);
		m_grid[linenumber].size_x=rcn->size_x;
		m_grid[linenumber].size_y=rcn->size_y;
		m_grid[linenumber].num_nodes=rcn->num_nodes_per_layer;

		linenumber++;
	}
	fclose( *file );
}

float get_random_float(float a)
{
	return ((float)rand()/(float)(RAND_MAX)) * a;
}



void init_paramaters (parameter_set * params, int l, int nodes_per_layer)
{
	params->heatflow_active = 741.0194702148f ;//+ get_random_float(100.0f);
	params->heatflow_inactive = 0.000000001f ;//+ get_random_float(0.1f);
	int i;
	float arg=get_random_float(0.5f);
	for (i = 0; i < nodes_per_layer; i++) {
		params->resistance_sink[i] = 50.775294f ;//+ get_random_float(0.5f);;
	}
	params->temperature_heat_sink = 27.7548294067f;//+ get_random_float(5.0f);
	params->capacitance_per_layer[0] = 0.0519234203f ;//+ get_random_float(0.1f);
	params->capacitance_per_layer[1] = 0.0000359467f ;//+ get_random_float(0.1f);
	params->resistances_x_per_layer[0] = 718.3076171875 ;//+ get_random_float(2.0f);
	params->resistances_x_per_layer[1] = 1000.2604980469f ;//+ get_random_float(2.0f);
	params->resistances_y_per_layer[0] = 716.2671508789f;// + get_random_float(1.0f);
	params->resistances_y_per_layer[1] = 159.9222717285f;// + get_random_float(1.0f);
	params->resistance_inter_layer[0] = 5.5448718071;// + get_random_float(8.0f);


}

void random_init_paramaters (parameter_set * params, int l, int nodes_per_layer)
{
	int i;

	float a = 0.001f;

	for (i = 0; i < l; i++) {
		params->capacitance_per_layer[i] = get_random_float(a); //l*get_random_float(a) / (i+1);
	}

	a = 0.1f;
	params->heatflow_inactive = get_random_float(a);
	a = 750.0f;
	params->heatflow_active =  get_random_float(a);

	a = 10.0f;
	for (i = 0; i < l-1; i++) {
		params->resistance_inter_layer[i] = get_random_float(a);//0.5f+get_random_float(a);
	}

	a = 10.0f;
	params->resistance_sink[0]=get_random_float(a);
	for (i = 1; i < nodes_per_layer; i++) {
		params->resistance_sink[i]=params->resistance_sink[0];
	}

	a = 10.0f;
	params->resistances_x_per_layer[0] = get_random_float(a);
	for (i = 1; i < l; i++) {
		params->resistances_x_per_layer[i] = params->resistances_x_per_layer[0];
	}

//	a = 30.0f;
	params->resistances_y_per_layer[0] = get_random_float(a);
	for (i = 1; i < l; i++) {
		params->resistances_y_per_layer[i] = params->resistances_y_per_layer[0];
	}

	a = 50.0f;
	params->temperature_heat_sink=25.0f;//get_random_float(a);


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

	params->resistance_sink = malloc(x * y * sizeof *params->resistance_sink); // one capacitance per layer
	assert(params->resistance_sink);

	int var;

	for (var = 0; var < l; var++) {
		params->resistances_x_per_layer[var] = 1.0f;
		params->resistances_y_per_layer[var] = 1.0f;
	}
	for (var = 0; var < l; var++) {
		params->resistance_inter_layer[var] = 1.0f;
	}
	for (var = 0; var < x*y; var++) {
		params->resistance_sink[var] = 1.0f;
	}

	return params;
}

void print_parameters(parameter_set * params, int num_layers, int nodes_per_layer) {
	int i;
	printf("\t\tHeatflow Active: \t\t%.15f\n",params->heatflow_active);
	printf("\t\tHeatflow Inactive: \t\t%.15f\n",params->heatflow_inactive);
	printf("\t\tTemperature Heat Sink: \t\t%.15f\n",params->temperature_heat_sink);

	for (i = 0; i < num_layers; i++) {
		printf("\t\tCapacitance on Layer %d: \t%.15f\n", i, params->capacitance_per_layer[i]);
	}

	for (i = 0; i < num_layers -1; i++) {
		printf("\t\tResistances above Layer %d: \t%.15f\n", i, params->resistance_inter_layer[i]);
	}

	for (i = 0; i < num_layers; i++) {
		printf("\t\tX-Resistances on Layer %d: \t%.15f\n", i, params->resistances_x_per_layer[i]);
		printf("\t\tY-Resistances on Layer %d: \t%.15f\n", i, params->resistances_y_per_layer[i]);
	}

	for (i = 0; i < nodes_per_layer; i++) {
		printf("\t\tResistance Sink %d: \t\t%.15f\n",i, params->resistance_sink[i]);
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
	int i, j;
	for (i = 0; i < rcn->num_nodes; i++) {
		rcn->nodes[i].inv_capacity = params->capacitance_per_layer[rcn->nodes[i].layer];

		for (j = 0; j < rcn->nodes[i].num_neighbors; j++) {
			if (rcn->nodes[i].neighbor_type[j] == Z_UP && rcn->nodes[i].layer < (rcn->num_layers - 1)) {
				rcn->nodes[i].inv_resistance[j] = params->resistance_inter_layer[rcn->nodes[i].layer];
			}
			else if (rcn->nodes[i].neighbor_type[j] == Z_DOWN && rcn->nodes[i].layer != 0) {
				rcn->nodes[i].inv_resistance[j] = params->resistance_inter_layer[rcn->nodes[i].layer - 1];
			}
			else if (rcn->nodes[i].neighbor_type[j] == Y_UP || rcn->nodes[i].neighbor_type[j] == Y_DOWN) {
				rcn->nodes[i].inv_resistance[j] = params->resistances_y_per_layer[rcn->nodes[i].layer];
			}
			else if (rcn->nodes[i].neighbor_type[j] == X_UP || rcn->nodes[i].neighbor_type[j] == X_DOWN) {
				rcn->nodes[i].inv_resistance[j] = params->resistances_x_per_layer[rcn->nodes[i].layer];
			}
			else
			{
				printf("Node %d has %d neighbors, which are not defined?!...\n", i, rcn->nodes[i].num_neighbors);
	            int var;
	            for (var = 0; var < rcn->nodes[i].num_neighbors; var++) {
	            	//printf("\tNeighbor on layer %d and coords %d,%d with type: %d\n", *rcn->nodes[i].neighbors[var].layer, rcn->nodes[i].neighbors[var].coordx, rcn->nodes[i].neighbors[var].coordy, rcn->nodes[i].neighbor_type[var]);
				}
			}

		}
		//rcn->nodes[i].heatflow = params->heatflow_inactive;
		rcn->temperature_sink = params->temperature_heat_sink;
		memcpy(rcn->inv_resistance_sink, params->resistance_sink, sizeof * rcn->inv_resistance_sink);

	}

}

/**
 * find index of neighbor in specified direction
 */
int get_neighbor_index_of(node temp_node, node_type type)
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
	            int x;
	            for (x = 0; x < rcn->nodes[node_index].num_neighbors; x++) {
	            	printf("Resistance on layer %d (node %d) is:\t\t%f\n", rcn->nodes[node_index].layer, node_index, rcn->nodes[node_index].inv_resistance[x]);
				}
            	printf("Temperature on node %d (layer %d) is:\t\t%f\n", node_index, rcn->nodes[node_index].layer, rcn->nodes[node_index].temperature);

	            printf("Capacity on layer/node %d/%d are:    \t\t%f\n", rcn->nodes[node_index].layer, node_index, rcn->nodes[node_index].inv_capacity);

			}
		}
	}
}

float simulate_model(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, int t)
{
	// temporary variables
	int j,k,l,x;
	int sim_time;
	float delta = 0.005f;
	float mse, e;

	rcn = create_rcn(rcn->size_x,rcn->size_y,rcn->num_layers);
	init(rcn);
	assign_parameters(params,rcn,m_grid);

	for (x = 0; x < rcn->num_nodes; x++) {

		rcn->nodes[x].temperature = m_grid[0].temperature[x % rcn->num_nodes_per_layer]; // m_grid[0] is first timestep of measurements
		/*
		 * Setting initial temperatures for heat source layer (= 0)
		 */
		if(x < rcn->num_nodes_per_layer){
						/**
			 * setting initial heatflow
			 */
			if (m_grid[0].heater_level[x] > 0)
				rcn->heatflow_source[x] = params->heatflow_active;
			else
				rcn->heatflow_source[x] = params->heatflow_inactive;
		}
	}

	mse = 0.0f;

	/**
	 * for each measurement step set heatflow sources
	 */
	for (j = 1; j < num_lines - 1; j++) {//10154
		if (j>0)
			sim_time = m_grid[j].timestep - m_grid[j-1].timestep;
		else
			sim_time=1;
		/**
		 * find differences in heater levels. TODO: impelement 31 levels. not only 2
		 */
		for (k = 0; k < rcn->num_nodes_per_layer; k++) {
		//	printf("Heater Level in line %d of Heater %d: %d\n", j, k, m_grid[j].heater_level[k]);
			//if (m_grid[j].heater_level[k] != m_grid[j-1].heater_level[k] || j == 1) {
				//heaters changes or first non-initial measurement
				if (m_grid[j].heater_level[k] > 0){
					rcn->heatflow_source[k] = params->heatflow_active;
					//printf("->ACTIVED %d in line %d\n", k, j);
				}else if (m_grid[0].heater_level[x] == -1){
					printf("ERROR: Heatcodes not set properly!\n");
				}else{
					rcn->heatflow_source[k] = params->heatflow_inactive;
					//printf("->DEACTIVED %d in line %d\n", k, j);
				}

			//}
		}

		/**
		 * calculate timestep on RC Network
		 */
		simulate(rcn, sim_time, delta);
		//printf("\nSimulated timestep.\n");

		/**
		 * Calculate RSME
		 */

		if(t==1) printf("Diff: %d ",j);
		for (l = 0; l < rcn->num_nodes_per_layer; l++) {
			e = rcn->nodes[l].temperature - m_grid[j].temperature[l];
			mse += e*e;
			if(t==1) printf("%f ", e);
		}
		if(t==1) printf("\n");

		if(t==1) printf("Temp: %d ",j);
		for (l = 0; l < rcn->num_nodes_per_layer; l++) {
			if(t==1) printf("%f ", rcn->nodes[l].temperature);
		}
		if(t==1) printf("\n");

		if(t==1) printf("Heat: %d ",j);
		for (l = 0; l < rcn->num_nodes_per_layer; l++) {
			if(t==1) printf("%d ", m_grid[j].heater_level[l]);
		}
		if(t==1) printf("\n");
	}
	mse /= (num_lines);//10154
	mse /= (rcn->num_nodes_per_layer);
	//printf("%d - %d - %f - %f\n", num_lines, rcn->num_nodes_per_layer, mse, sqrt(mse));
	return sqrt(mse);
}

void get_clocks(unsigned long * t_start, unsigned long * t_stop, unsigned long * t_diff, unsigned long * t_diff_msec)
{
	if ( t_start <= t_stop ) {
		*t_diff_msec = ( *t_stop - *t_start ) / ( CLOCKS_PER_SEC / 1000 );
		*t_diff = *t_stop - *t_start;

	} else {
		*t_diff_msec = ( ULONG_MAX - *t_start + *t_stop ) / ( CLOCKS_PER_SEC / 1000 );       // Milliseconds
		*t_diff = ULONG_MAX - (*t_stop - *t_start);
	}
}

float min(float a, float b)
{
	if (a>b) return b; else return a;
}

void param_annealing(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float step, float * param, float temp_start,  float temp_step, int equilibrium_reached ,float * rmse)
{
	float temp;
	temp = temp_start;
	float cost = *rmse;
	float cost_2;
	float delta_cost;
	int equilibrium = equilibrium_reached;
	float k;
	float * old_param = malloc(sizeof (parameter_set));
	parameter_set * old_params = create_parameter_set(rcn->size_x, rcn->size_y, rcn->num_layers);

	k=1.0f;
	//printf("COST: %f\n", cost);
	memcpy(old_param, param, sizeof (float));
	memcpy(old_params, params, sizeof (parameter_set));
	while(temp > 0.00001f) // while not frozen
	{
		while(equilibrium != 0)
		{

			equilibrium--;
			float offset = (*param * ((2.0f*step)*get_random_float(1.0f) - step)/100);
			*param += offset; // random values in between -step and step percent

//			printf("Offset is %f\n",offset);

			cost_2 = simulate_model(m_grid, rcn, params,0);

			delta_cost = cost_2 - cost;
			//printf("old: %f\tnew:\t%f\tcost:\t%f\tcost2:\t%f\tdeltacost:\t%f\n", *old_param, *param, cost, cost_2, delta_cost);
			float r = get_random_float(1.0f);
			//printf("min(%f, %f) = %f == %f?\n", 1.0f, exp(- (delta_cost)/(k*temp) ), min(1, exp(- (delta_cost)/(k*temp) )), r);
			if (min(1.0f, exp(- (delta_cost)/(k*temp) )) > r)
			{
				memcpy(old_param, param, sizeof (parameter_set));
				memcpy(old_params, params, sizeof (parameter_set));
				cost = cost_2;
			}else
			{	// restore old parameters
				memcpy(param, old_param, sizeof *old_param);
				memcpy(params, old_params, sizeof (parameter_set));
			}
		}
		equilibrium = equilibrium_reached;
		temp *= temp_step;
		print_parameters(params,rcn->num_layers,rcn->num_nodes_per_layer);
	//	print_rcn(rcn);
		printf("Cost:\t%f\nTemperature:\t%f\n", cost, temp);
	}
	*rmse = cost;
	free(old_param);
}

void change_by_stage (parameter_set * new_params, parameter_set * params, float start_percentage, rc_network * rcn, int stage)
{
	float a;
	int i;

	/*
	 * v[0] = p->C1;
	v[1] = p->G01;
	v[2] = p->T1;
	v[3] = p->Ioff;
	v[4] = p->Ion;
	v[5] = p->Ghs;
	v[0] = p->G1v;
	v[1] = p->G1h;
	v[2] = p->G0v;
	v[3] = p->G0h;
	v[4] = p->Ths;
	v[5] = p->C0;
	 */

	memcpy(new_params, params, sizeof * new_params);

	float percentage = start_percentage;

	a = params->capacitance_per_layer[rcn->num_layers-1-stage]+params->capacitance_per_layer[rcn->num_layers-1-stage] * (2.0f * percentage * get_random_float(1.0f) - percentage) / 100;
	if (a>0.000001f) new_params->capacitance_per_layer[rcn->num_layers-1-stage] = a;
	else new_params->capacitance_per_layer[rcn->num_layers-1-stage] = get_random_float(1.0f);

	if(stage > rcn->num_layers - 2){
		a = params->resistance_inter_layer[stage]+params->resistance_inter_layer[stage] * (2.0f * get_random_float(1.0f) - percentage) / 100;
		if (a>0.000001f) new_params->resistance_inter_layer[stage] = a;
		else new_params->resistance_inter_layer[stage] = get_random_float(1.0f);
	}

	if(stage == 0){
		new_params->heatflow_inactive = params->heatflow_inactive + params->heatflow_inactive * (2.0f * percentage * get_random_float(1.0f) - percentage) / 100;
		new_params->heatflow_active = params->heatflow_active + params->heatflow_active * (2.0f * percentage * get_random_float(1.0f) - percentage) / 100;
		for (i = 0; i < rcn->num_nodes_per_layer; i++) {
			a = params->resistance_sink[i] + params->resistance_sink[i] * (2.0f * get_random_float(1.0f) - percentage) / 100;
			if (a>0.000001f) new_params->resistance_sink[i] = a;
			else new_params->resistance_sink[i] = get_random_float(1.0f);
		}
	}

	if(stage == rcn->num_layers-1){

		for (i = 0; i < rcn->num_layers; i++) {
			a = params->resistances_x_per_layer[i]+params->resistances_x_per_layer[i] * (2.0f * get_random_float(1.0f) - percentage) / 100;
			if (a>0.000001f) new_params->resistances_x_per_layer[i] = a;
			else new_params->resistances_x_per_layer[i] = get_random_float(1.0f);
		}
		for (i = 0; i < rcn->num_layers; i++) {
			a = params->resistances_y_per_layer[i]+params->resistances_y_per_layer[i] * (2.0f * get_random_float(1.0f) - percentage) / 100;
			if (a>0.000001f) new_params->resistances_y_per_layer[i] = a;
			else new_params->resistances_y_per_layer[i] = get_random_float(1.0f);
		}

		a = params->temperature_heat_sink + params->temperature_heat_sink * (2.0f * percentage * get_random_float(1.0f) - percentage) / 100;
		if (a > 0.000001f) new_params->temperature_heat_sink = a;
		else new_params->temperature_heat_sink = get_random_float(30.0f);
	}
}

void change (parameter_set * new_params, parameter_set * params, float start_percentage, rc_network * rcn)
{
	int i;
	float a;

	float percentage = start_percentage;

	for (i = 0; i < rcn->num_nodes_per_layer; i++) {
		a = params->capacitance_per_layer[i]+params->capacitance_per_layer[i] * (2.0f * percentage * get_random_float(1.0f) - percentage) / 100;
		if (a>0.000001f) new_params->capacitance_per_layer[i] = a;
		else new_params->capacitance_per_layer[i] = get_random_float(1.0f);
	}
	for (i = 0; i < rcn->num_nodes_per_layer - 1; i++) {
		a = params->resistance_inter_layer[i]+params->resistance_inter_layer[i] * (2.0f * get_random_float(1.0f) - percentage) / 100;
		if (a>0.000001f) new_params->resistance_inter_layer[i] = a;
		else new_params->resistance_inter_layer[i] = get_random_float(1.0f);
	}
	for (i = 0; i < rcn->num_layers; i++) {
		a = params->resistances_x_per_layer[i]+params->resistances_x_per_layer[i] * (2.0f * get_random_float(1.0f) - percentage) / 100;
		if (a>0.000001f) new_params->resistances_x_per_layer[i] = a;
		else new_params->resistances_x_per_layer[i] = get_random_float(1.0f);
	}
	for (i = 0; i < rcn->num_layers; i++) {
		a = params->resistances_y_per_layer[i]+params->resistances_y_per_layer[i] * (2.0f * get_random_float(1.0f) - percentage) / 100;
		if (a>0.000001f) new_params->resistances_y_per_layer[i] = a;
		else new_params->resistances_y_per_layer[i] = get_random_float(1.0f);
	}

	new_params->heatflow_active = params->heatflow_active + params->heatflow_active * (2.0f * percentage * get_random_float(1.0f) - percentage) / 100;
	new_params->heatflow_inactive = params->heatflow_inactive + params->heatflow_inactive * (2.0f * percentage * get_random_float(1.0f) - percentage) / 100;

	a = params->temperature_heat_sink + params->temperature_heat_sink * (2.0f * percentage * get_random_float(1.0f) - percentage) / 100;
	if (a > 0.000001f) new_params->temperature_heat_sink = a;
	else new_params->temperature_heat_sink = get_random_float(30.0f);

	for (i = 0; i < rcn->num_nodes_per_layer; i++) {
		a = params->resistance_sink[i] + params->resistance_sink[i] * (2.0f * get_random_float(1.0f) - percentage) / 100;
		if (a>0.000001f) new_params->resistance_sink[i] = a;
		else new_params->resistance_sink[i] = get_random_float(1.0f);
	}

}

void annealing1 (measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float temp_start, float temp_step, int equlibrium, float sigma, float * rmse)
{
	float temp;
	temp = temp_start;
	float cost = *rmse;
	float cost_2;
	float delta_cost;
	int eq = equlibrium;
	float s = sigma;
	float k;
	parameter_set * new_params = create_parameter_set(rcn->size_x, rcn->size_y, rcn->num_layers);

	k=1;
//	memcpy(new_params, params, sizeof (parameter_set));

	while(temp > 0.00001f) // while not frozen
	{
		while(eq != 0)
		{
			eq--;
			change(new_params, params, s, rcn);
			assign_parameters(params,rcn,m_grid);
			cost_2 = simulate_model(m_grid, rcn, new_params,0);
			delta_cost = cost_2 - cost;
			float r = get_random_float(1.0f);
			if (min(1, exp(- (delta_cost)/(k*temp) )) > r)
			{
				cost = cost_2;
				memcpy(params, new_params, sizeof (parameter_set));
			}
		}
		eq = equlibrium;
		print_parameters(new_params,rcn->num_layers,rcn->num_nodes_per_layer);
		printf("Cost:\t%f\t%f\t%f\n", cost, s, temp);
	//	print_rcn(rcn);
		temp *= temp_step;
	//	s *= 0.99;
	}
	*rmse = cost;
	free(new_params);

}

void annealing3(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float temp_start, float temp_step, int equlibrium, float sigma, float * rmse)
{


	float temp;
	temp = temp_start;
	float cost = *rmse;
	float cost_2;
	float delta_cost;
	int eq = equlibrium;
	float s = sigma;
	float k;
	parameter_set * new_params = create_parameter_set(rcn->size_x, rcn->size_y, rcn->num_layers);

	k=1;
//	memcpy(new_params, params, sizeof (parameter_set));



	int stage;
	for (stage = 0; stage < rcn->num_layers; stage++) {
		while(temp > 0.00001f) // while not frozen
		{
			while(eq != 0)
			{
				eq--;
				change_by_stage(new_params, params, s, rcn, stage);
				assign_parameters(params,rcn,m_grid);
				cost_2 = simulate_model(m_grid, rcn, new_params,0);
				delta_cost = cost_2 - cost;
				float r = get_random_float(1.0f);
				if (min(1, exp(- (delta_cost)/(k*temp) )) > r)
				{
					cost = cost_2;
					memcpy(params, new_params, sizeof (parameter_set));
				}
			}
			eq = equlibrium;
			print_parameters(new_params,rcn->num_layers,rcn->num_nodes_per_layer);
			printf("Stage %d\tCost:\t%f\t%f\t%f\n", stage, cost, s, temp);
			temp *= temp_step;
		}
		*rmse = cost;
		temp = temp_start;
	}
	free(new_params);


}

void annealing2 (measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float temp_start, float temp_step, int equlibrium, float neighboring_values, float * rmse)
{
	int i,j;

	//float neighboring_values = 25.0f;

	for (j = 0; j < 10; j++) {
		for (i = 0; i < rcn->num_layers; i++) {
			param_annealing(m_grid,rcn,params, neighboring_values,&(params->capacitance_per_layer[rcn->num_layers - 1 - i]), temp_start, temp_step,equlibrium, rmse);
			param_annealing(m_grid,rcn,params, neighboring_values,&(params->capacitance_per_layer[rcn->num_layers - 1 - i]), temp_start, temp_step,equlibrium, rmse);
		}

		for (i = 0; i < rcn->num_layers - 1; i++) {
			param_annealing(m_grid,rcn,params, neighboring_values, &(params->resistance_inter_layer[rcn->num_layers - 2 - i]), temp_start, temp_step,equlibrium, rmse);
		}

		param_annealing(m_grid,rcn,params, neighboring_values,&(params->heatflow_inactive), temp_start, temp_step, equlibrium, rmse);
		param_annealing(m_grid,rcn,params, neighboring_values,&(params->heatflow_active), temp_start, temp_step,equlibrium, rmse);

		float * res_sink = malloc(sizeof * params->resistance_sink);
		*res_sink = params->resistance_sink[0];
		for (i = 1; i < rcn->num_nodes_per_layer; i++) {
			params->resistance_sink[i] = *res_sink;
		}
		param_annealing(m_grid,rcn,params, neighboring_values,res_sink, temp_start, temp_step,equlibrium, rmse);

		for (i = 0; i < rcn->num_layers; i++) {
			param_annealing(m_grid, rcn,params, neighboring_values, &(params->resistances_x_per_layer[i]), temp_start, temp_step,equlibrium, rmse);
		}
		for (i = 0; i < rcn->num_layers; i++) {
			param_annealing(m_grid,rcn,params, neighboring_values,&(params->resistances_y_per_layer[i]), temp_start, temp_step,equlibrium,rmse);
		}

		param_annealing(m_grid,rcn,params, neighboring_values,&(params->temperature_heat_sink), temp_start, temp_step,equlibrium, rmse);

	}
}

void param_to_vector(parameter_set * param, float * vector, int num_layers, int nodes_per_layer)
{
	int i,j,k,l,m;
	for (i = 0; i < num_layers; i++) {
		vector[i] = param->capacitance_per_layer[i];
	}

	vector[i +1] = param->heatflow_active;
	vector[i +2] = param->heatflow_inactive;

	for (j = 0; j < num_layers - 1; j++) {
		vector[j + 3 + i] = param->resistance_inter_layer[j];
	}

	for (m = 0; m < nodes_per_layer; m++) {
		vector[j + 3 + i + m] = param->resistance_sink[m];
	}

	for (k = 0; k < num_layers; k++) {
		vector[j + 4 + i + k +m] = param->resistances_x_per_layer[k];
	}
	for (l = 0; l < num_layers; l++) {
		vector[j + 5 + i + k + l+m] = param->resistances_y_per_layer[l];
	}

	vector[j + 6 + i + k + l +m] = param->temperature_heat_sink;
}

void vector_to_param(float * vector, parameter_set * param, int num_layers,int num_nodes_per_layer)
{
	int i,j,k,l,m;
	for (i = 0; i < num_layers; i++) {
		param->capacitance_per_layer[i]= vector[i];
	}

	param->heatflow_active=vector[i +1];
	param->heatflow_inactive=vector[i +2];

	for (j = 0; j < num_layers - 1; j++) {
		param->resistance_inter_layer[j] = vector[j + 3 + i];
	}

	for (m = 0; m < num_nodes_per_layer; m++) {
		param->resistance_sink[m] = vector[j + 3 + i + m];
	}

	for (k = 0; k < num_layers; k++) {
		param->resistances_x_per_layer[k] = vector[j + 4 + i + k + m];
	}
	for (l = 0; l < num_layers; l++) {
		param->resistances_y_per_layer[l] = vector[j + 5 + i + k + l + m];
	}

	param->temperature_heat_sink = vector[j + 6 + i + k + l  + m];
}

void PSO(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float * rmse)
{
	int number_of_particles = 50;
	int i, d;
	int d_max = 2 + rcn->num_layers * 4 + m_grid->size_x*m_grid->size_y;
	float r_p, r_g;

	parameter_set ** param_per_particle=malloc(number_of_particles *sizeof *param_per_particle);
	float present_vec[number_of_particles][d_max];
	float velocity_vec[number_of_particles][d_max];
	float pbest_vec[number_of_particles][d_max];
	float gbest_vec[d_max];
	float costs_vec[number_of_particles];
	float pbest_costs_vec[number_of_particles];
	float gbest_costs;
	int stop_criterion = 10;
	int improvement = stop_criterion * number_of_particles;
	int num_nan;
	float init_costs = 0.0f;
	gbest_costs = 1.0f/0.0f;

	//print_rcn(rcn);

	for (i = 0; i < number_of_particles; i++) {
			init_costs = sqrt(-1);
			num_nan = 0;
			param_per_particle[i] = create_parameter_set(rcn->size_x, rcn->size_y, rcn->num_layers);
			while(isinf(init_costs) || isnan(init_costs))// == sqrt (-1))
			{
				init_paramaters(param_per_particle[i],rcn->num_layers, rcn->num_nodes_per_layer);
				//print_parameters(param_per_particle[i],rcn->num_layers, rcn->num_nodes_per_layer);
				//print_rcn(rcn);
				init_costs = simulate_model(m_grid,rcn,param_per_particle[i],0);
				//print_rcn(rcn);
				num_nan++;
			}

			printf("%d: Cost %f\tParameters:\n", i, init_costs);
			print_parameters(param_per_particle[i],rcn->num_layers,rcn->num_nodes_per_layer);
			printf("\n");
	}

	for (i = 0; i < number_of_particles; i++) {
		//print_parameters(param_per_particle[i],rcn->num_layers);
		param_to_vector(param_per_particle[i],present_vec[i],rcn->num_layers,rcn->num_nodes_per_layer);
		costs_vec[i] = simulate_model(m_grid, rcn, param_per_particle[i],0);
		pbest_costs_vec[i] = 1.0f/0.0f;
		if (costs_vec[i] < pbest_costs_vec[i])
		{
			pbest_costs_vec[i] = costs_vec[i];
			for (d = 0; d < d_max; d++) {
				pbest_vec[i][d] = present_vec[i][d];
			}
		}
		if (costs_vec[i] < gbest_costs)
		{
			gbest_costs = costs_vec[i];
			for (d = 0; d < d_max; d++) {
				gbest_vec[d] = present_vec[i][d];
			}
		}
		//printf("Initial cost of %d:\t%f\n", i, costs_vec[i]);fflush(stdout);
	}
	printf("Global best:\t\t%f\n\n", gbest_costs);
	while(gbest_costs > 1.0f && improvement > 0)
	{
		//printf("Local: ");
		for (i = 0; i < number_of_particles; i++) {

			r_p =  get_random_float(1.0f);
			r_g =  get_random_float(1.0f);
			for (d = 0; d < d_max; d++) {
				velocity_vec[i][d] += r_p * (pbest_vec[i][d] - present_vec[i][d]) + r_g * (gbest_vec[d] - present_vec[i][d]);
			//	if (present_vec[i][d] + velocity_vec[i][d] > 0.000001f)
					present_vec[i][d] += velocity_vec[i][d];
			//	else
			//		present_vec[i][d] += velocity_vec[i][d]/16;
			}
			vector_to_param(present_vec[i], param_per_particle[i], rcn->num_layers,rcn->num_nodes_per_layer);
			costs_vec[i] = simulate_model(m_grid, rcn, param_per_particle[i],0);

			if (costs_vec[i] < pbest_costs_vec[i])
			{
				pbest_costs_vec[i] = costs_vec[i];
				for (d = 0; d < d_max; d++) {
					pbest_vec[i][d] = present_vec[i][d];
				}
			}
			if (costs_vec[i] < gbest_costs)
			{
				gbest_costs = costs_vec[i];
				for (d = 0; d < d_max; d++) {
					gbest_vec[d] = present_vec[i][d];
				}
				improvement = stop_criterion * number_of_particles;
			} else
				improvement--;

		//	printf("%f ", pbest_costs_vec[i]);
		}
		vector_to_param(gbest_vec,params,rcn->num_layers,rcn->num_nodes_per_layer);
		printf("Global best:\t%f\n", gbest_costs); fflush(stdout);
		print_parameters(params,rcn->num_layers,rcn->num_nodes_per_layer);
	}
	*rmse = gbest_costs;
	for (i = 0; i < number_of_particles; i++) {
		free(param_per_particle[i]);
	}
	free(param_per_particle);
}

void start_annealing1(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float * rmse)
{
	float temp_start = 2.0f;
	float temp_step = 0.8f;
	int equlibrium = 100;
	float sigma = 15.0f;
	unsigned long t_start, t_stop, t_diff, t_diff_msec;
	printf("Done!\nCreating Random Parameter Set... ");fflush(stdout);
	random_init_paramaters(params, rcn->num_layers, rcn->num_nodes_per_layer);
//	init_paramaters(params, rcn->num_layers, rcn->num_nodes_per_layer);
	printf("Done!\n\tParameters are:\n");fflush(stdout);
	print_parameters(params, rcn->num_layers,rcn->num_nodes_per_layer);
	printf("Assigning Parameters to RC Model... ");fflush(stdout);
	assign_parameters(params, rcn, m_grid);
	printf("Done!\n");fflush(stdout);
	//print_rcn(rcn);
	printf("First Simulation with random Parameters... ");fflush(stdout);
	*rmse = simulate_model(m_grid,rcn,params,0);

	printf("Done!\n Initial RMSE is %f.\nLearning using SA... ", *rmse);fflush(stdout);
	t_start = clock( );
	annealing1(m_grid, rcn, params,temp_start,temp_step,equlibrium, sigma, rmse);
	t_stop = clock( );
	get_clocks(&t_start, &t_stop, &t_diff, &t_diff_msec);

	printf("\nDone in %d msec (%d clocks). New RMSE:\t%f\n", (int)t_diff_msec, (int)t_diff, *rmse);fflush(stdout);
	print_parameters(params, rcn->num_layers,rcn->num_nodes_per_layer);
	params_free(params);
}

void start_annealing2(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float * rmse)
{
	float temp_start = 2.0f;
	float temp_step = 0.5f;
	int equlibrium = 5;
	float sigma = 15.0f;
	unsigned long t_start, t_stop, t_diff, t_diff_msec;
	printf("Done!\nCreating Random Parameter Set... ");fflush(stdout);
	//random_init_paramaters(params, rcn->num_layers, rcn->num_nodes_per_layer);
	init_paramaters(params, rcn->num_layers, rcn->num_nodes_per_layer);
	printf("Done!\n\tParameters are:\n");fflush(stdout);
	print_parameters(params, rcn->num_layers,rcn->num_nodes_per_layer);
	printf("Assigning Parameters to RC Model... ");fflush(stdout);
	assign_parameters(params, rcn, m_grid);
	printf("Done!\n");fflush(stdout);
	//print_rcn(rcn);
	printf("First Simulation with random Parameters... ");fflush(stdout);
	*rmse = simulate_model(m_grid,rcn,params,0);

	printf("Done!\n Initial RMSE is %f.\nLearning using SA... ", *rmse);fflush(stdout);
	t_start = clock( );
	annealing2(m_grid, rcn, params,temp_start,temp_step,equlibrium, sigma, rmse);
	t_stop = clock( );
	get_clocks(&t_start, &t_stop, &t_diff, &t_diff_msec);

	printf("\nDone in %d msec (%d clocks). New RMSE:\t%f\n", (int)t_diff_msec, (int)t_diff, *rmse);fflush(stdout);
	print_parameters(params, rcn->num_layers,rcn->num_nodes_per_layer);
	params_free(params);
}

void start_annealing3(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float * rmse)
{
	float temp_start = 2.0f;
	float temp_step = 0.8f;
	int equlibrium = 10;
	float sigma = 15.0f;
	unsigned long t_start, t_stop, t_diff, t_diff_msec;
	printf("Done!\nCreating Random Parameter Set... ");fflush(stdout);
	random_init_paramaters(params, rcn->num_layers, rcn->num_nodes_per_layer);
//	init_paramaters(params, rcn->num_layers, rcn->num_nodes_per_layer);
	printf("Done!\n\tParameters are:\n");fflush(stdout);
	print_parameters(params, rcn->num_layers,rcn->num_nodes_per_layer);
	printf("Assigning Parameters to RC Model... ");fflush(stdout);
	assign_parameters(params, rcn, m_grid);
	printf("Done!\n");fflush(stdout);
	//print_rcn(rcn);
	printf("First Simulation with random Parameters... ");fflush(stdout);
	*rmse = simulate_model(m_grid,rcn,params,0);

	printf("Done!\n Initial RMSE is %f.\nLearning using SA... ", *rmse);fflush(stdout);
	t_start = clock( );
	annealing3(m_grid, rcn, params,temp_start,temp_step,equlibrium, sigma, rmse);
	t_stop = clock( );
	get_clocks(&t_start, &t_stop, &t_diff, &t_diff_msec);

	printf("\nDone in %d msec (%d clocks). New RMSE:\t%f\n", (int)t_diff_msec, (int)t_diff, *rmse);fflush(stdout);
	print_parameters(params, rcn->num_layers,rcn->num_nodes_per_layer);
	params_free(params);
}

void start_PSO(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float * rmse)
{
	printf( "Done.\nLearning using PSO...\n"); fflush(stdout);
	PSO(m_grid,rcn,params,rmse);
	printf("Done.\nNew RMSE:\t%f\nNew Parameters:\n",*rmse);fflush(stdout);
	print_parameters(params, rcn->num_layers,rcn->num_nodes_per_layer);
}

void get_line_number(FILE ** file, int line_len)
{
	int max_line_len = line_len;
	char line[max_line_len];

	while(fgets(line, max_line_len, *file)) {
		num_lines ++;
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
	int line_len = 0;
	measurement_grid * m_grid;
	float rmse;
	int algorithm;

	srand((unsigned int)time(NULL));
	if ( argc != 9 && argc != 2)
	{
		printf( "Start this program as follows:\n ");
		printf("%s PATH_TO_MEASUREMENTS  WIDTH_OF_HEATER_GRID  HEIGHT_OF_HEATER_GRID  WIDTH_OF_SENSOR_GRID  HEIGHT_OF_SENSOR_GRID  NUMBER_OF_LAYERS  ALGORITHM\n", argv[0] );
		printf("---\nALGORITHM:\n\t0 for Simulated Annealing\n\t1 for Particle Swarm Optimization\n");
	}
	else if(argc == 2)
	{
		algorithm = 1;
		line_len = 273;
		open_measurements(&file, &argv[1]);
		get_line_number(&file, line_len);
		rcn = create_rcn(5, 5, 2);
		init(rcn);
		m_grid = create_grid(5, 5, num_lines);
		init_grid(m_grid, num_lines);
		params = create_parameter_set(rcn->size_x,rcn->size_y,rcn->num_layers);
		init_paramaters(params,2,25);
		open_measurements(&file, &argv[1]);
		parse_measurements(&file, line_len, rcn, m_grid, 5, 5);

		float co = simulate_model(m_grid,rcn,params,0);
		printf("cost:\t%f\n", co);
		print_parameters(params,rcn->num_layers,rcn->num_nodes_per_layer);
	}
	else
	{
		heater_size_x = atoi(argv[2]);
		heater_size_y = atoi(argv[3]);
		sensor_size_x = atoi(argv[4]);
		sensor_size_y = atoi(argv[5]);
		num_layers = atoi(argv[6]);
		algorithm = atoi(argv[7]);
		line_len = atof(argv[8]);

		open_measurements(&file, &argv[1]);
		get_line_number(&file, line_len);

		printf("Creating RC Network with %d layers and a %dx%d grid... ", num_layers, sensor_size_x, sensor_size_y);
		rcn = create_rcn(sensor_size_x, sensor_size_y, num_layers);
		printf("Done!\nInitializing RC Network and MGRID based on textfile with %d lines... ", num_lines);fflush(stdout);
		init(rcn);
		m_grid = create_grid(sensor_size_x, sensor_size_y, num_lines);
		init_grid(m_grid, num_lines);
		params = create_parameter_set(rcn->size_x,rcn->size_y,rcn->num_layers);
		printf("Done!\n");
		printf("Parsing Measurements... ");fflush(stdout);
		open_measurements(&file, &argv[1]);
		parse_measurements(&file, line_len, rcn, m_grid, heater_size_x, heater_size_y);

	//	rmse = simulate_model(m_grid,rcn,params);
		switch (algorithm) {
			case 0: start_annealing1(m_grid, rcn, params, &rmse);break;
			case 1: start_PSO(m_grid,rcn,params,&rmse); break;			// best
			case 2: start_annealing2(m_grid, rcn, params, &rmse);break;
			case 3: start_annealing3(m_grid, rcn, params, &rmse);break;	// ~ like PSO
			default: printf("Algorithm not known"); exit(EXIT_FAILURE);break;
		}

		float co = simulate_model(m_grid,rcn,params,1);
		printf("cost:\t%f", co);
		print_parameters(params,rcn->num_layers,rcn->num_nodes_per_layer);
		rcn_free(rcn);
		mgrid_free(m_grid);
		printf("Done!\nExiting...");fflush(stdout);
	    exit(EXIT_SUCCESS);
	}


}
