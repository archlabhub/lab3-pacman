/* Dima AbdelJaber
 * November 20, 2018
 * pacman.h class includes and defines methods and variables in pacman.c
 */

#ifndef pacman_included
#define pacman_included
#include "movingLayer.h"

//enemy layers and moving layers linked to all of the layers
extern Layer enemyLayer1;
extern Layer enemyLayer0;
extern MovLayer movingEnemy1;
extern MovLayer movingEnemy0;

static const AbRectOutline fieldOutline = {	//outside border
  abRectOutlineGetBounds, abRectOutlineCheck, {screenWidth/2 - 5, screenHeight/2 - 10}
};

static const AbRectOutline obstacleOutline = {	//fences inside outside border
  abRectOutlineGetBounds, abRectOutlineCheck, {11, 8} //obstacle width =11, height=8
};

Layer DotsLayer5 = {		//layer for the dot on the top right
  (AbShape *)&circle2, {screenWidth-20, 27}, {0,0}, {0,0}, COLOR_RED, &enemyLayer0,
};

Layer DotsLayer4 = {		//layer for the dot on the top left
  (AbShape *)&circle2, {20, 27}, {0,0}, {0,0}, COLOR_RED, &DotsLayer5,
};

Layer DotsLayer3 = {	//layer for the dot on the middle right
  (AbShape *)&circle2, {screenWidth-20, (screenHeight/2)+4}, {0,0}, {0,0}, COLOR_RED, &DotsLayer4,
};

Layer DotsLayer2 = {   //layer for the dot on the middle left
  (AbShape *)&circle2, {20, (screenHeight/2)+4}, {0,0}, {0,0}, COLOR_DARK_ORANGE, &DotsLayer3,
};

Layer DotsLayer1 = {	  //layer for the dot on the bottom right
  (AbShape *)&circle2, {screenWidth-20, (screenHeight-14)}, {0,0}, {0,0}, COLOR_DARK_ORANGE, &DotsLayer2,
};
Layer DotsLayer0 = {		//layer with Dot, bottom-left
  (AbShape *)&circle2, {14, (screenHeight-14)}, {0,0}, {0,0}, COLOR_DARK_ORANGE,
  &DotsLayer1,
};

Layer fieldLayer = {	//boarder layer
  (AbShape *) &fieldOutline, {screenWidth/2-1, (screenHeight/2)+6}, {0,0}, {0,0}, COLOR_SKY_BLUE, &DotsLayer0
};

Layer pacmanLayer0 = {	//Layer of pacman
  (AbShape *)&circle5, {screenWidth/2, (screenHeight-14)}, {0,0}, {0,0}, COLOR_ORANGE, &fieldLayer, 
};

MovLayer movLayer6 = { &DotsLayer5, {0,0}, &movingEnemy0}; //moving layer for dot 1
MovLayer movLayer5 = { &DotsLayer4, {0,0}, &movLayer6}; //moving layer for dot 2
MovLayer movLayer4 = { &DotsLayer3, {0,0}, &movLayer5}; //moving layer for dot 3
MovLayer movLayer3 = { &DotsLayer2, {0,0}, &movLayer4}; //moving layer for dot 4
MovLayer movLayer2 = { &DotsLayer1, {0,0}, &movLayer3}; //moving layer for dot 5
MovLayer movLayer1 = { &DotsLayer0, {0,0}, &movLayer2}; //moving layer for dot 6
MovLayer movLayer0 = { &pacmanLayer0, {0,0}, &movLayer1}; //Moving layer for the Pacman

#endif
