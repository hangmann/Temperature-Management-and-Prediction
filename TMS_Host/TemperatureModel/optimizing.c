/*******************************************************************
*
* OPTIMIZING.C
*
* Optimizing Algorithms
*
*
* VERSION: 17.04.2015
*
* by Hendrik Hangmann
* University of Paderborn
*
********************************************************************/

#include "optimizing.h"

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
			assign_parameters_to_RCN(params,rcn,m_grid);
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
	int j;

	for (j = 0; j < 10; j++) {
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
					assign_parameters_to_RCN(params,rcn,m_grid);
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

}

void annealing2 (measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float temp_start, float temp_step, int equlibrium, float neighboring_values, float * rmse)
{
	int i,j;

	//float neighboring_values = 25.0f;

	for (j = 0; j < 10; j++) {
		printf("Capacitance\n");
		for (i = 0; i < rcn->num_layers; i++) {
			param_annealing(m_grid,rcn,params, neighboring_values,&(params->capacitance_per_layer[rcn->num_layers - 1 - i]), temp_start, temp_step,equlibrium, rmse);
		}
		printf("Interlayer Resistance\n");
		for (i = 0; i < rcn->num_layers - 1; i++) {
			param_annealing(m_grid,rcn,params, neighboring_values, &(params->resistance_inter_layer[rcn->num_layers - 2 - i]), temp_start, temp_step,equlibrium, rmse);
		}
		printf("Heatflow\n");
		for (i = 0; i < NUM_HEATER_LEVELS; i++) {
			param_annealing(m_grid,rcn,params, neighboring_values,&(params->heatflow[i]), temp_start, temp_step, equlibrium, rmse);
		}

	/*	float * res_sink = &(params->resistance_sink[0]);
		printf("Heatsink\n");
		for (i = 0; i < rcn->num_nodes_per_layer; i++) {
			param_annealing(m_grid,rcn,params, neighboring_values,&(params->resistance_sink[i]), temp_start/2, temp_step,equlibrium / 2 , rmse);
		}
		*/
		param_annealing(m_grid,rcn,params, neighboring_values,&(params->resistance_sink), temp_start/2, temp_step,equlibrium / 2 , rmse);
		printf("X Res\n");
		for (i = 0; i < rcn->num_layers; i++) {
			param_annealing(m_grid, rcn,params, neighboring_values, &(params->resistances_x_per_layer[i]), temp_start, temp_step,equlibrium, rmse);
		}
		printf("Y Res\n");
		for (i = 0; i < rcn->num_layers; i++) {
			param_annealing(m_grid,rcn,params, neighboring_values,&(params->resistances_y_per_layer[i]), temp_start, temp_step,equlibrium,rmse);
		}
		printf("T sink\n");
		param_annealing(m_grid,rcn,params, neighboring_values,&(params->temperature_heat_sink), temp_start, temp_step,equlibrium, rmse);

	}
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

void PSO(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float * rmse)
{
	int number_of_particles = 10;
	int i, d;
	int d_max = NUM_HEATER_LEVELS + rcn->num_layers * 4 + m_grid->size_x*m_grid->size_y;
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
				random_init_paramaters(param_per_particle[i],rcn->num_layers, rcn->num_nodes_per_layer);
				//print_parameters(param_per_particle[i],rcn->num_layers, rcn->num_nodes_per_layer);
				//print_rcn(rcn);
				init_costs = simulate_model(m_grid,rcn,param_per_particle[i],0);
				//print_rcn(rcn);
				num_nan++;
			}

			printf("Particle %d:\n\tParameters:\n", i);
			print_parameters(param_per_particle[i],rcn->num_layers,rcn->num_nodes_per_layer);
			printf("\n\tCost:\t%f\n",init_costs);
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
