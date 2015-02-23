#include <stdlib.h>
#include "model.h"

/**
 *  x = network width
 *  y = network height
 *  l = network layers
 */
struct rc_network * create(int x, int y, int l)
{
    struct rc_network * model;
    int num_resistances;  //number of overall resistances between nodes (except sink and source)
    
    model->num_nodes_per_layer = x * y;
    model->num_layers = l;
    model->num_nodes = model->num_nodes_per_layer * model->num_layers;
    model->size_x = x;
    model->size_y = y;
    
    model = malloc(sizeof * model);
	assert(model);
	model->nodes = malloc(model->num_nodes * sizeof *model->nodes);
	assert(model->nodes);
    
    num_resistances = ((size_x - 1) * size_y + size_x * (size_y - 1)) * l + model->num_nodes_per_layer;  // total number of connections between nodes (except sink and source)
    
    model->resistances_nodes = malloc(model->num_resistances * sizeof *model->resistances_nodes);
	assert(model->resistances_nodes);
	
    model->resistance_sink = malloc(model->num_nodes_per_layer * sizeof *model->resistance_sink);
	assert(model->resistance_sink);
    model->temperature_source = malloc(model->num_nodes_per_layer * sizeof *model->temperature_source);
	assert(model->temperature_source);
    
    return model;
}

void init (struct rc_network * rcn)
{
    int i, j ,k, l;
    int node_index;
    int num_neighbors;
    struct nodes neighboring_nodes_raw[6]; // at maximum 6 possible neighbors
    
        
    for(i = 0; i < rcn->num_layers; i++){  
        for(j = 0; j < rcn->size_x; j++){  
            for(k = 0; k < rcn->size_y; k++){  
                node_index = i * rcn->num_nodes_per_layer + j * rcn->size_x + k;
                rcn->nodes[node_index]->coordx = j;
                rcn->nodes[node_index]->coordy = k;
                rcn->nodes[node_index]->layer = i;
                
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
                
                node->neighbors = malloc(rcn->nodes[node_index]->num_neighbors * sizeof *node->neighbors);
                assert(node->neighbors);
                
                node->resistance = malloc(rcn->nodes[node_index]->num_neighbors * sizeof *node->resistance);
                assert(node->resistance);
                
                for(i = 0; i < num_neighbors; i++){
                    node->resistance[i] = 1.0;
                }
                                
                struct nodes neighboring_nodes[num_neighbors];
                
                for(l = 0; l < num_of_neighbors; l++){
                    neighboring_nodes[0] = neighboring_nodes_raw[0];
                }
                rcn->nodes[node_index]->num_neighbors = num_of_neighbors; //set number of neighbors
                rcn->nodes[node_index]->neighbors = neighboring_nodes;    //set actual neighbors nodes
                
            }
        }
    }
    
    
}

void calculate_nodes (struct rc_network * rcn, float dt)
{
    int i, j, k, l;
    int layer_index;
    float heat_flow_sink[rcn->num_nodes_perlayer];
    int num_neighbors;
    float summed_heatflow;
    
    int begin_highest_layer;
    
    begin_highest_layer = rcn->num_nodes - rcn->num_nodes_perlayer;
    
    for(i = begin_highest_layer; i < rcn->num_nodes; i++){   // calculate I_sink for top layer
         layer_index = i - begin_highest_layer;
         heat_flow_sink[layer_index] = (temperature_sink - rcn->nodes[i]->temperature) / rcn->resistance_sink[layer_index];
    }
       
    for(j = 0; j < rcn->num_nodes; j++){  
                
        for(k = 0; k < rcn->nodes[j]->num_neighbors; k++){  
        
            rcn->nodes[j]->heatflow += (rcn->nodes[j]->neighbor[k]->temperature - rcn->nodes[j]->temperature) / rcn->nodes[j]->resistance[k]; // calculate I_n
        }
    }
    
    for(l = 0; l < rcn->num_nodes; l++){  
        
        summed_heatflow = rcn->nodes[l]->heatflow;
    
        if (l >= begin_highest_layer)
        {
            summed_heatflow += heat_flow_sink[l - begin_highest_layer];    // add I_sink 
        }
        
        if (l <= rcn->num_nodes_perlayer)
        {
            summed_heatflow += rcn->heatflow_source;    // add I_source 
        }
        
        rcn->nodes[j]->temperature = summed_heatflow / rcn->nodes[j]->capacity * dt;
    }
}

void simulate (struct rc_network * rcn, float duration, float dt)
{
    while(duration > dt){
		calculate_nodes(rcn,dt);
		duration -= dt;
	}
	
	if(duration > 0) calculate_nodes(rcn, duration);
}

void free(struct rc_network * rcn)
{
    assert(rcn);
	free(rcn->nodes);
    free(rcn->nodes);
	free(rcn->resistance_sink);
	free(rcn->heatflow_source);
    free(rcn);
}

int main (int argc, char **argv)
{
	// TODO
	return 0;
}

