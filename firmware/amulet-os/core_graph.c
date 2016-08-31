/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */


#include "core_graph.h"
#include "core_ui.h"

Graph graphs[MAX_GRAPH_COUNT];
uint8_t graphs_top_index = 0;

int8_t CoreAddGraph(GraphType type, uint8_t x, uint8_t y, uint8_t w, uint8_t h,
                uint8_t data_min, uint8_t data_max) {
  if (graphs_top_index >= MAX_GRAPH_COUNT) return -1; //If graphs array is full, do nothing

  uint8_t graph_id = graphs_top_index;
  graphs[graph_id].type = type;
  graphs[graph_id].loc_x = x;
  graphs[graph_id].loc_y = y;
  graphs[graph_id].width = w;
  graphs[graph_id].height = h;
  graphs[graph_id].data_min = data_min;
  graphs[graph_id].data_max = data_max;

  for (uint8_t n = 0; n < MAX_DATA_SIZE; n++)
    graphs[graph_id].data[n] = 0;

  graphs_top_index++;

  return graph_id;
}

void CoreAddGraphData(uint8_t data, uint8_t graph_id) {
  if (graph_id >= MAX_GRAPH_COUNT) return; //If graph_id invalid, do nothing

  // Shift all the current data points down 1 and lose the oldest
  for (uint8_t n = 0; n < MAX_DATA_SIZE - 1; n++)
    graphs[graph_id].data[n] = graphs[graph_id].data[n+1];

  graphs[graph_id].data[MAX_DATA_SIZE-1] = data;
}

void CoreDrawGraph(uint8_t graph_id, uint8_t requestor) {
  if (graph_id >= MAX_GRAPH_COUNT) return; //If graph_id invalid, do nothing

  uint8_t x = graphs[graph_id].loc_x;
  uint8_t y = graphs[graph_id].loc_y;
  uint8_t w = graphs[graph_id].width;
  uint8_t h = graphs[graph_id].height;
  float d_max = (float)graphs[graph_id].data_max;
  float d_min = (float)graphs[graph_id].data_min;
  CoreClearRect(x,y,w,h,requestor);

  CoreDrawFastVLine(x,y,h,requestor);
  CoreDrawFastHLine(x,y+h,w,requestor);

  uint8_t interval = w/MAX_DATA_SIZE;
  uint8_t offset = interval/2;
  uint8_t d, dx, dy, prev_x, prev_y;
  float calc_y;
  for (uint8_t i = 0; i<MAX_DATA_SIZE; i++) {
    dx = (interval * i) + x;
    d = graphs[graph_id].data[i];
    if (d >= d_max) dy = y;
    else if (d <= d_min) dy = y+h;
    else {
      calc_y = (1.0-((d-d_min)/(d_max-d_min))) * h;
      dy = (uint8_t)calc_y + y;
    }
    if (graphs[graph_id].type == LINE_GRAPH) {
      dx += offset;
      if (i != 0) {
        CoreDrawLine(prev_x,prev_y,dx,dy,requestor);
        CoreDrawLine(prev_x+1,prev_y,dx+1,dy,requestor);
      }
    }
    else if (graphs[graph_id].type == BAR_GRAPH) {
      uint8_t bar_height = (y+h)-dy;
      CoreFillRect(dx+1,dy,interval-2,bar_height,requestor);
    }
    prev_x = dx;
    prev_y = dy;
  }
}
