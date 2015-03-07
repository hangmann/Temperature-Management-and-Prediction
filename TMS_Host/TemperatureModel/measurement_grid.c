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
			temp_grid.nodes[j].heatflow = rcn->nodes[i].heatflow;
			temp_grid.nodes[j].temperature = rcn->nodes[i].temperature;
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
		(m_grid+var)->nodes = malloc(m_grid->num_nodes * sizeof (grid_node));
		int var2;
		for (var2 = 0; var2 < m_grid->num_nodes; var2++) {
			(m_grid+var)->nodes[var2].heater_level=0;
			(m_grid+var)->nodes[var2].temperature=0;
			(m_grid+var)->nodes[var2].heatflow=0;
		}
		assert(m_grid->nodes);
	}

	return m_grid;
}

void mgrid_free(measurement_grid * m_grid)
{
    assert(m_grid);
    int var;
    for (var = 0; var < 10154; ++var) {
		free((m_grid+var)->nodes);
	}
    free(m_grid);
}
