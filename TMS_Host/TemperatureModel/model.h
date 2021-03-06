/*******************************************************************
*
* MODEL.H
*
* Definitions for R(esistance)-C(apacitor)-Network
*
*
* VERSION: 24.02.2015
*
* by Hendrik Hangmann
* University of Paderborn
*
********************************************************************/

#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include "parameter.h"

typedef struct node node;
typedef enum node_type node_type;

enum node_type
{
	X_UP,	// After on X  0
	X_DOWN,	// Before on X 1
	Y_UP,	// After on Y  2
	Y_DOWN,	// Before on Y 3
	Z_UP,	// above       4
	Z_DOWN	// below       5
};

/* network topology node */
struct node
{
	float inv_capacity; /* capacity */
	float temperature; /* temperature */
	float heatflow; /* heat flow to node */
	int coordx;
    int coordy;
    int layer;
    int num_neighbors;
    node ** neighbors;
    enum node_type * neighbor_type;
    float * inv_resistance;
};

/* thermal model */
typedef struct
{
	int num_layers;  			/* number of layers */
    int num_nodes;          /* number of nodes in models */
    int num_nodes_per_layer;          /* number of nodes in models */
    node * nodes;    /* nodes in rc network*/
   
    float inv_resistance_sink;          /* resistances between maximum layer nodes and head sink */
	float * heatflow_source;              /* heat source temperatures */
    
    float temperature_sink;
    
	int size_x;				/* network width */
	int size_y;				/* network height */
} rc_network;

rc_network * create_rcn(int x, int y, int l);
void calculate_nodes (rc_network * rcn, float dt);
void rcn_free(rc_network * rcn);
void init(rc_network * rcn);
void simulate(rc_network * rcn, float sim_time, float dt);
//void print(struct model_t * model, FILE * fout);
//void temperatures_print(struct model_t * model, FILE * fout, int line_length);

#endif
