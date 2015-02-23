#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>

/* network topology node */
struct node
{
	float capacity; /* capacity */
	float temperature; /* temperature */
	float heatflow; /* heat flow to node */
	int coordx;
    int coordy;
    int layer;
    int num_neighbors;
    struct node * neighbors;
    float * resistance;
};

/* thermal model */
struct rc_network
{
	int num_layers;  			/* number of layers */
    int num_nodes;          /* number of nodes in models */
    int num_nodes_per_layer;          /* number of nodes in models */
    struct node * nodes;    /* nodes in rc network*/
   
    float * resistance_sink;               /* resistances between maximum layer nodes and head sink */
	float * heatflow_source;              /* heat source temperatures */
    
    float temperature_sink;
    
	int size_x;				/* network width */
	int size_y;				/* network height */
};

struct rc_network * create(int x, int y, int l);
void calculate_nodes (struct rc_network * rcn);
void free(struct rc_network * rcn);
void init(struct rc_network * rcn);
void simulate(struct model_t * model, float sim_time, float dt);
//void print(struct model_t * model, FILE * fout);
//void temperatures_print(struct model_t * model, FILE * fout, int line_length);

#endif
