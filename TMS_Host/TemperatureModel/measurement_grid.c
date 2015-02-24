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

measurement_grid layer_to_grid(rc_network * rcn, int layer)
{
	// TODO
}

measurement_grid * create_grid(int x, int y)
{
	measurement_grid * m_grid;

	m_grid = malloc(sizeof * m_grid);
	assert(m_grid);

	m_grid->size_x = x;
	m_grid->size_y = y;
	m_grid->num_nodes = x * y;

	m_grid->nodes = malloc(m_grid->num_nodes * sizeof *m_grid->nodes);
	assert(m_grid->nodes);

}

