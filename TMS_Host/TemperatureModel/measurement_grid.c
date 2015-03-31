/*******************************************************************
*
* MEASUREMENT_GRID.C
*
* Measurment grids, similar to layer 0 of RC Network
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
#include <assert.h>
#include "measurement_grid.h"

/**
 * save specified RCN layer to a 2-D grid of measurement points
 */
measurement_grid * layer_to_grid(rc_network * rcn, measurement_grid * m_grid, int num_heaters ,int layer)
{
	int i, j;
	measurement_grid * temp_grid = create_grid(rcn->size_x,rcn->size_y,1);

	j = 0;

	for (i = 0; i < rcn->num_nodes; i++) {
		if (rcn->nodes[i].layer == layer)
		{
			temp_grid->temperature[j] = rcn->nodes[i].temperature;
			j++;
		}
	}

	for (i = 0; i < num_heaters ; i++) {
		temp_grid->heater_level[i] = m_grid[i].heater_level[i];
	}

	return temp_grid;
}

measurement_grid * create_grid(int x, int y, int num_lines)
{
	measurement_grid * m_grid;

	m_grid = malloc(num_lines * sizeof (measurement_grid));//10154
	assert(m_grid);

	int var;
	for (var = 0; var < num_lines; ++var) {
		(m_grid+var)->size_x=x;
		(m_grid+var)->size_y=y;
		(m_grid+var)->num_nodes = x*y;
		(m_grid+var)->temperature	= malloc(m_grid->num_nodes		* sizeof (*(m_grid+var)->temperature ));
		(m_grid+var)->heater_level	= malloc(m_grid->num_nodes		* sizeof (*(m_grid+var)->heater_level));
	}

	return m_grid;
}

void print_measurement_grid(measurement_grid * m_grid, int number_of_grids, int number_of_sensors, int number_of_heaters)
{
	int i,j,k;
	for (i = 0; i < number_of_grids; i++) {
		printf("%d\t", i);
		for (j = 0; j < number_of_sensors; j++) {
			printf("%f ",m_grid[i].temperature[j]);
		}
		for (k = 0; k < number_of_heaters; k++) {
			printf("%d ",m_grid[i].heater_level[k]);
		}
		printf("\n");
	}
}

void init_grid (measurement_grid * grid, int num_lines)
{
	int var;
	for (var = 0; var < num_lines; ++var) {
		int var2;
		for (var2 = 0; var2 < grid->num_nodes; var2++) {
			grid[var].temperature[var2]		= -1;
			grid[var].heater_level[var2]	= -1;
			grid[var].timestep				= -1;
		}
	}
}

void mgrid_free(measurement_grid * m_grid)
{
    assert(m_grid);
    free(m_grid);
}
