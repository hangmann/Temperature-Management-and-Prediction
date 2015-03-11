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
    node_type neighboring_type_raw[6]; // at maximum 6 possible neighbors

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
                        neighboring_type_raw[num_of_neighbors - 1] = Z_DOWN;
                    }
                if(i < rcn->num_layers - 1) 
                {
                    // there is a layer above ( i + 1 )
                    num_of_neighbors++;
                    neighboring_nodes_raw[num_of_neighbors - 1] = rcn->nodes[node_index + rcn->num_nodes_per_layer];
                    neighboring_type_raw[num_of_neighbors - 1] = Z_UP;
                }
                if(j > 0)
                {
                    // there is a node on the left ( j - 1 )
                    num_of_neighbors++;
                    neighboring_nodes_raw[num_of_neighbors - 1] = rcn->nodes[node_index - rcn->size_x];
                    neighboring_type_raw[num_of_neighbors - 1] = X_DOWN;
                }
                if(k > 0)
                {
                    // there is a lower node ( k - 1 )
                    num_of_neighbors++;
                    neighboring_nodes_raw[num_of_neighbors - 1] = rcn->nodes[node_index - 1];
                    neighboring_type_raw[num_of_neighbors - 1] = Y_DOWN;
                }
                if(j < rcn->size_x - 1)
                {
                    // there is a node on the right ( j + 1 )
                    num_of_neighbors++;
                    neighboring_nodes_raw[num_of_neighbors - 1] = rcn->nodes[node_index + rcn->size_x];
                    neighboring_type_raw[num_of_neighbors - 1] = X_UP;
                }
                if(k < rcn->size_y - 1)
                {
                    // there is a higher node ( k - 1 )
                    num_of_neighbors++;
                    neighboring_nodes_raw[num_of_neighbors - 1] = rcn->nodes[node_index + 1];
                    neighboring_type_raw[num_of_neighbors - 1] = Y_UP;
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
                rcn->nodes[node_index].temperature = 0.0f;
                rcn->nodes[node_index].heatflow = 0.0f;

        		int x;
                for(x = 0; x < num_of_neighbors; x++){
                    rcn->nodes[node_index].inv_resistance[x] = 1.0;
                }

                memcpy(rcn->nodes[node_index].neighbors, neighboring_nodes_raw, sizeof *(rcn->nodes[node_index].neighbors));
                memcpy(rcn->nodes[node_index].neighbor_type, neighboring_type_raw, sizeof *(rcn->nodes[node_index].neighbors));
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
    int j, k, l;
    int layer_index;
    float heat_flow_sink[rcn->num_nodes_per_layer];
    float summed_heatflow;
    
    int begin_highest_layer;
    
    begin_highest_layer = rcn->num_nodes - rcn->num_nodes_per_layer;

    for(l = 0; l < rcn->num_nodes; l++){  
        //printf("Heatflow of %d: %f\n", l, rcn->nodes[l].heatflow);
        float summed_heatflow;
        summed_heatflow = rcn->nodes[l].heatflow;	//I_n

        for(k = 0; k < rcn->nodes[l].num_neighbors; k++){

        	summed_heatflow += (rcn->nodes[l].neighbors[k].temperature - rcn->nodes[l].temperature) * rcn->nodes[l].inv_resistance[k]; // calculate I_n
		}

        //printf("1. Heatflow:\t%f\n", summed_heatflow);
    
        if (l >= begin_highest_layer)
        {
        	layer_index = l - begin_highest_layer;
			heat_flow_sink[layer_index] = (rcn->temperature_sink - rcn->nodes[l].temperature) * rcn->inv_resistance_sink;  //calculate I_sink
        	summed_heatflow += heat_flow_sink[l - begin_highest_layer];    // add I_sink (only on top layer)

   //         printf("Because Top Layer\n");
    //        printf("2. Heatflow:\t%f\t\t(T_sin - N(i) * R_inv = %f - %f * %f)\n", summed_heatflow, rcn->temperature_sink, rcn->nodes[l].temperature, rcn->inv_resistance_sink);
        }
        
        if (l < rcn->num_nodes_per_layer)
        {
            summed_heatflow += rcn->heatflow_source[l];    // add I_source (only on layer 0)
   //         printf("Because Source\n");
    //        printf("3. Heatflow:\t%f\n", summed_heatflow);
        }
        
        rcn->nodes[l].temperature = summed_heatflow * rcn->nodes[l].inv_capacity * dt;

     //   printf("Temperature:\t%f\t\t= I * C_inv * dt = %f * %f * %f\n", rcn->nodes[l].temperature, summed_heatflow, rcn->nodes[l].inv_capacity, dt);
       // printf("Temperature is %f\n", rcn->nodes[l].temperature);
    /*    printf("%d: T: %f\tC:\t%f\tI:\t%f\n", l, rcn->nodes[l].temperature, rcn->nodes[l].inv_capacity, rcn->nodes[l].heatflow);
        for(k = 0; k < rcn->nodes[l].num_neighbors; k++){
			printf("\t%f\n", rcn->nodes[l].inv_resistance[k]);
		}
		*/
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
}

void rcn_free(rc_network * rcn)
{
    assert(rcn);
	free(rcn->nodes);
	free(rcn->heatflow_source);
    free(rcn);
}
