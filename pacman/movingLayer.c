/* Dima AbdelJaber
 * November 20, 2018
 * movingLayer.c class handles the moving layers
 */
#include "movingLayer.h"

//Updates next position to be new position for the moving layers and then it will draw the layer
void movLayerDraw(MovLayer *movLayers, Layer *layers){
  int row, col;
  MovLayer *movLayer;
  and_sr(~8);			//disable interrupts

  //moving layer will updates their position to the next position
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { //moving layer
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			//disable interrupts

  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { //moving layer
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds); //bounds of layer
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelposition = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelposition)) {
	    color = probeLayer->color;
	    break; 
	  } // if probe check
	} // checks all layers at column, row
	lcd_writeColor(color); 
      } // column
    } // row
  } // updating moving layer
}	  

//Advances moving layer by assigning next position to position
void movLayerAdvance(MovLayer *movLayer){
  Vec2 newposition;
  u_char axis;
  Region shapeboundary;
  for (; movLayer; movLayer = movLayer->next) {
    vec2Add(&newposition, &movLayer->layer->posNext, &movLayer->velocity);    
    movLayer->layer->posNext = newposition;
  } //movLayer 
}
