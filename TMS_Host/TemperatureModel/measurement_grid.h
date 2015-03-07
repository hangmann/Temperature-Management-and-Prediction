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
	int num_nodes;
	int size_x;
	int size_y;
	float * temperature;
	float * heatflow;
	int * heater_level;
}measurement_grid;

measurement_grid layer_to_grid(rc_network * rcn, int layer);
measurement_grid * create_grid(int x, int y);
void mgrid_free(measurement_grid * m_grid);

#endif /* MEASUREMENT_GRID_H_ */
