/* Dima AbdelJaber
 * November 20, 2018
 * movingLayer.h class defines everything in movingLayer.c
 */
#ifndef movingLayer_included
#define movingLayer_included
#include <shape.h>


//Moving Layer. The velocity represents one iteration direction & magnitude
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

void movLayerDraw(MovLayer* movLayers, Layer* layers);

//link to the movelayer and position becomes next position
void movLayerAdvance(MovLayer *movLayer);

#endif // included
