/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

/**
 *  @file core_graph.h
 *  @brief functions to add graphs to the display buffer
 */
#ifndef CORE_GRAPH_H
#define CORE_GRAPH_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_GRAPH_COUNT 4
#define MAX_DATA_SIZE 10

typedef enum GraphType {
  LINE_GRAPH,
  BAR_GRAPH
} GraphType;

typedef struct {
  GraphType type;
  uint8_t loc_x;
  uint8_t loc_y;
  uint8_t width;
  uint8_t height;
  uint8_t data[MAX_DATA_SIZE];
  uint8_t data_min;
  uint8_t data_max;
} Graph;
/**
 * Add a graph to the graph array
 *
 * @return graph_id -1 if graphs array is full.
 */
int8_t CoreAddGraph(GraphType type, uint8_t x, uint8_t y, uint8_t w, uint8_t h,
                uint8_t data_min, uint8_t data_max);

void CoreAddGraphData(uint8_t data, uint8_t graph_id);

/**
 * Draw graph with specific ID
 *
 * @return none
 */
void CoreDrawGraph(uint8_t graph_id, uint8_t requestor);


#endif
