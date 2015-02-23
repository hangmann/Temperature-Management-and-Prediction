#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>

/* grid coordinates */
struct coordinate_t
{
	int x;
	int y;
};

/* network topology connector */
struct connector
{
	float R; /* resistance */
	struct coordinate_t coord;
};

/* network topology node */
struct node_t
{
	float C; /* capacity */
	float T; /* temperature */
	float F; /* heat flow to node */
	struct coordinate_t coord;
};

/* layer of nodes */
struct node_layer_t
{
	struct node_t * nodes;
	
};

/* layer of nodes */
struct connector_layer_t
{
	struct connector_t * connector;
	
};

/* thermal model */
struct model_t
{
	int num_layers;  			/* number of layers */
	struct node_layer_t * n_layers;		/* layers of nodes */
	struct node_layer_t * sink_n_layer;	/* node layer of heat sink */
	struct node_layer_t * source_layer;	/* node layer of heat source */
	struct connector_layer_t * c_layers;	/* layers of connectors */
	struct connector_layer_t sink_c_layer;	/* connector layer to heat sink */
	int size_x;				/* network width */
	int size_y;				/* network height */
};

#endif
