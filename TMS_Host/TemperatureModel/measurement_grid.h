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
	int timestep;
	float * temperature;
	float * heatflow;
	int * heater_level;
}measurement_grid;

measurement_grid * layer_to_grid(rc_network * rcn, measurement_grid * m_grid, int num_heaters ,int layer);
measurement_grid * create_grid(int x, int y, int num_lines);
void mgrid_free(measurement_grid * m_grid);
void init_grid (measurement_grid * grid, int num_lines);

#endif /* MEASUREMENT_GRID_H_ */
