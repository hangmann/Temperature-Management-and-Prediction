/*******************************************************************
*
* OPTIMIZING.H
*
* Definitions for optimization algorithms
*
*
* VERSION: 17.04.2015
*
* by Hendrik Hangmann
* University of Paderborn
*
********************************************************************/

#ifndef OPTIMIZING_H_
#define OPTIMIZING_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "definitions.h"
#include "optimizing.h"
#include "parameter.h"
#include "measurement_grid.h"
#include "model.h"
#include "main.h"


void annealing1 (measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float temp_start,  float temp_step,int equlibrium,float sigma, float * rmse);
void annealing2 (measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float temp_start,  float temp_step,int equlibrium,float sigma, float * rmse);
void annealing3 (measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float temp_start,  float temp_step,int equlibrium,float sigma, float * rmse);
void param_annealing(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float step, float * param, float temp_start, float temp_step, int equlibrium_reached,float * rmse);
void PSO(measurement_grid * m_grid, rc_network * rcn, parameter_set * params, float * rmse);

#endif
