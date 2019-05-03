/* Dima AbdelJaber
 * November 20, 2018
 * enemy.h class defines eveything used in enemy.c
 */

#ifndef enemy_included
#define enemy_included
#include <shape.h>
#include "movingLayer.h"


//Shape of the enemy (square)
const AbRect rect10 = {abRectGetBounds, abRectCheck, {6,6}}; //6x6 square

//color of the enemy is pink
Layer enemyLayer1 = {	  //Layer of the second enemy
  (AbShape *)&rect10,
  {screenWidth/2-30, screenHeight/2 -55}, //bit below & right of center
  {0,0}, {0,0}, COLOR_PINK, 0, //last and the next pos ({0,0}, {0,0})
};
//color of the enemy is pink
Layer enemyLayer0 = {	//Layer of the first enemy
  (AbShape *)&rect10,
  {screenWidth-30, screenHeight/2 - 15}, //bit below & right of center
  {0,0}, {0,0}, COLOR_PINK, &enemyLayer1, //last and the next pos ({0,0}, {0,0})
};
MovLayer movingEnemy1 = { &enemyLayer1, {5,0}, 0};  //Moving layer with second enemy
MovLayer movingEnemy0 = { &enemyLayer0, {-5,0}, &movingEnemy1}; //Moving layer with first enemy
void enemyDraw();
void enemyAI(int i);

#endif
