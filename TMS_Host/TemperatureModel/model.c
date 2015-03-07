/*******************************************************************
*
* MODEL.C
*
* This file models a R(esistance)-C(apacitor)-Network
*
*
* VERSION: 24.02.2015
*
* by Hendrik Hangmann
* University of Paderborn
*
********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "model.h"

/**
 *  creates new RC network model
 *
 *  x = network width
 *  y = network height
 *  l = network layers
 */
rc_network * create_rcn(int x, int y, int l)
{
    rc_network * rcn;
    
    rcn = malloc(sizeof * rcn);
	assert(rcn);
	rcn->num_nodes_per_layer = x * y;
	rcn->num_layers = l;
	rcn->size_x = x;
	rcn->size_y = y;
	rcn->num_nodes = rcn->num_nodes_per_layer * rcn->num_layers;

	rcn->nodes = malloc(rcn->num_nodes * sizeof *rcn->nodes + 6 * sizeof *rcn->nodes->neighbors + 6 *sizeof *rcn->nodes->inv_resistance + 6*sizeof *rcn->nodes->neighbor_type);
	assert(rcn->nodes);

	rcn->inv_resistance_sink = malloc(rcn->num_nodes_per_layer * sizeof *rcn->inv_resistance_sink);
	assert(rcn->inv_resistance_sink);
	rcn->heatflow_source = malloc(rcn->num_nodes_per_layer * sizeof *rcn->heatflow_source );
	assert(rcn->heatflow_source );


    //num_resistances = ((x - 1) * x + y * (y - 1)) * l + rcn->num_nodes_per_layer;  // total number of connections between nodes (except sink and source)

    return rcn;
}


/**
 * initialize RCN
 * calculate nodes' number neighbors and
 * set each neighbors
 */
void init (rc_network * rcn)
{
    int i, j ,k, l;
    int node_index;
    int num_of_neighbors;
    node neighboring_nodes_raw[6]; // at maximum 6 possible neighbors

    for(i = 0; i < rcn->num_layers; i++){  
        for(j = 0; j < rcn->size_x; j++){  
            for(k = 0; k < rcn->size_y; k++){  
                node_index = i * rcn->num_nodes_per_layer + j * rcn->size_x + k;

                num_of_neighbors = 0;

                if(i > 0) 
                    {
                        // there is a layer below ( i - 1 )
                        num_of_neighbors++;
                        neighboring_nodes_raw[num_of_neighbors - 1] = rcn->nodes[node_index - rcn->num_nodes_per_layer];
                    }
                if(i < rcn->num_layers - 1) 
                {
                    // there is a layer above ( i + 1 )
                    num_of_neighbors++;
                    neighboring_nodes_raw[num_of_neighbors - 1] = rcn->nodes[node_index + rcn->num_nodes_per_layer];
                }
                if(j > 0)
                {
                    // there is a node on the left ( j - 1 )
                    num_of_neighbors++;
                    neighboring_nodes_raw[num_of_neighbors - 1] = rcn->nodes[node_index - rcn->size_x];
                }
                if(k > 0)
                {
                    // there is a lower node ( k - 1 )
                    num_of_neighbors++;
                    neighboring_nodes_raw[num_of_neighbors - 1] = rcn->nodes[node_index - 1];
                }
                if(j < rcn->size_x - 1)
                {
                    // there is a node on the right ( j + 1 )
                    num_of_neighbors++;
                    neighboring_nodes_raw[num_of_neighbors - 1] = rcn->nodes[node_index + rcn->size_x];
                }
                if(k < rcn->size_y - 1)
                {
                    // there is a higher node ( k - 1 )
                    num_of_neighbors++;
                    neighboring_nodes_raw[num_of_neighbors - 1] = rcn->nodes[node_index + 1];
                }
                

                rcn->nodes[node_index].num_neighbors = num_of_neighbors; //set number of neighbors

                rcn->nodes[node_index].neighbor_type = malloc(rcn->nodes[node_index].num_neighbors * sizeof *rcn->nodes[node_index].neighbor_type);
                assert(rcn->nodes[node_index].neighbor_type);

                rcn->nodes[node_index].neighbors = malloc(rcn->nodes[node_index].num_neighbors * sizeof *rcn->nodes[node_index].neighbors);
                assert(rcn->nodes[node_index].neighbors);

                rcn->nodes[node_index].inv_resistance = malloc(rcn->nodes[node_index].num_neighbors * sizeof *rcn->nodes[node_index].inv_resistance);
                assert(rcn->nodes[node_index].inv_resistance);


                rcn->nodes[node_index].coordx = j;
                rcn->nodes[node_index].coordy = k;
                rcn->nodes[node_index].layer = i;

                for(i = 0; i < num_of_neighbors; i++){
                    rcn->nodes[node_index].inv_resistance[i] = 1.0;
                }
                                
                node neighboring_nodes[num_of_neighbors];
                rcn->nodes[node_index].neighbors = neighboring_nodes;    //set actual neighbors nodes
                
                for(l = 0; l < num_of_neighbors; l++){
                    neighboring_nodes[0] = neighboring_nodes_raw[0];
                }
               // printf("Node %d has %d neighbors...\n", node_index, num_of_neighbors);
                
            }
        }
    }
    
    
}


/**
 * calculate heatflow and temperature
 * of each node
 */
void calculate_nodes (rc_network * rcn, float dt)
{
    int i, j, k, l;
    int layer_index;
    float heat_flow_sink[rcn->num_nodes_per_layer];

    float summed_heatflow;
    
    int begin_highest_layer;
    
    begin_highest_layer = rcn->num_nodes - rcn->num_nodes_per_layer;
    
    for(i = begin_highest_layer; i < rcn->num_nodes; i++){   // calculate I_sink for top layer
         layer_index = i - begin_highest_layer;
         heat_flow_sink[layer_index] = (rcn->temperature_sink - rcn->nodes[i].temperature) * rcn->inv_resistance_sink[layer_index];
    }
       
    for(j = 0; j < rcn->num_nodes; j++){  
                
        for(k = 0; k < rcn->nodes[j].num_neighbors; k++){
        
            rcn->nodes[j].heatflow += (rcn->nodes[j].neighbors[k].temperature - rcn->nodes[j].temperature) * rcn->nodes[j].inv_capacity; // calculate I_n
        }
    }
    
    for(l = 0; l < rcn->num_nodes; l++){  
        
        summed_heatflow = rcn->nodes[l].heatflow;
    
        if (l >= begin_highest_layer)
        {
            summed_heatflow += heat_flow_sink[l - begin_highest_layer];    // add I_sink 
        }
        
        if (l <= rcn->num_nodes_per_layer)
        {
            summed_heatflow += *rcn->heatflow_source;    // add I_source
        }
        
        rcn->nodes[j].temperature = summed_heatflow * rcn->nodes[j].inv_capacity * dt;
    }
}

/**
 * run simulation on RCN
 */
void simulate (rc_network * rcn, float duration, float dt)
{
    while(duration > dt){
		calculate_nodes(rcn,dt);
		duration -= dt;
	}
	
	if(duration > 0) calculate_nodes(rcn, duration);
}

void rcn_free(rc_network * rcn)
{
    assert(rcn);
	free(rcn->nodes);
	free(rcn->inv_resistance_sink);
	free(rcn->heatflow_source);
    free(rcn);
}
