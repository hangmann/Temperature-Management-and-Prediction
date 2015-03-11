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
measurement_grid layer_to_grid(rc_network * rcn, int layer)
{
	int i, j;
	measurement_grid temp_grid;

	j = 0;

	for (i = 0; i < rcn->num_nodes; i++) {
		if (rcn->nodes[i].layer == layer)
		{
			temp_grid.heatflow[j] = rcn->nodes[i].heatflow;
			temp_grid.temperature[j] = rcn->nodes[i].temperature;
			j++;
		}
	}

	return temp_grid;
}

measurement_grid * create_grid(int x, int y)
{
	measurement_grid * m_grid;

	m_grid = malloc(10154 * sizeof (measurement_grid));
	assert(m_grid);

	int var;
	for (var = 0; var < 10154; ++var) {
		(m_grid+var)->size_x=x;
		(m_grid+var)->size_y=y;
		(m_grid+var)->num_nodes = x*y;
		(m_grid+var)->temperature	= malloc(m_grid->num_nodes		* sizeof (*(m_grid+var)->temperature ));
		(m_grid+var)->heater_level	= malloc(m_grid->num_nodes		* sizeof (*(m_grid+var)->heater_level));
		(m_grid+var)->heatflow 		= malloc(m_grid->num_nodes		* sizeof (*(m_grid+var)->heatflow    ));
	}

	return m_grid;
}

void init_grid (measurement_grid * grid)
{
	int var;
	for (var = 0; var < 10154; ++var) {
		int var2;
		for (var2 = 0; var2 < grid->num_nodes; var2++) {
			grid[var].temperature[var2]		= -1;
			grid[var].heater_level[var2]	= -1;
			grid[var].heatflow[var2] 		= -1;
			grid[var].timestep				= -1;
		}
	}
}

void mgrid_free(measurement_grid * m_grid)
{
    assert(m_grid);
    free(m_grid);
}
