/*******************************************************************
*
* MEASUREMENT_GRID.H
*
* Definitions for measurement grids
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
#include "model.h"

#ifndef MEASUREMENT_GRID_H_
#define MEASUREMENT_GRID_H_

typedef struct
{
	float temperature;
	float heatflow;
}grid_node;

typedef struct
{
	int num_nodes;
	int size_x;
	int size_y;
	grid_node * nodes;
}measurement_grid;

measurement_grid layer_to_grid(rc_network * rcn, int layer);
void init_grid(measurement_grid * grid);
measurement_grid * create_grid(int x, int y);

#endif /* MEASUREMENT_GRID_H_ */
