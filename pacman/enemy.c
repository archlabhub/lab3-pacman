/* Dima AbdelJaber
 * November 20, 2018
 * enemy.c class handles the the enemy going after pacman
 */
#include <shape.h>
#include "enemy.h"

void enemyDraw(){
    //initialize the layer
   layerInit(&enemyLayer0);
   //draw the layer
   layerDraw(&enemyLayer0);
}

//when velocity is equal to 0 then enemies are not moving and the directions of the enemy will change
void enemyAI(int i){

  MovLayer* enemyLayer;

  //Finds enemy
  switch(i){
  case 1: enemyLayer = &movingEnemy0; break; 
  case 2: enemyLayer = &movingEnemy1; break;
  default: return;
  }
  //enemy state
  static char enemyState = 0;
  Vec2 enemySpeed =  enemyLayer->velocity;
  int x, y;
    
  //when the enemy is not moving or velocity = 0 it changes it's velocity
  if( enemySpeed.axes[0] == 0 && enemySpeed.axes[1] == 0){
    switch(enemyState){
    case 0: x = 5; y = -5; //Bottom-right
    break;
    case 1: x = 0; y = 6; //up
    break;
    case 2: x = -5; y = 0; //left
    break;
    case 3: x = 5; y = 0; //right
    break;
    case 4: x = -5; y = 4; //top-left
    break;
    case 5: x = 0; y = -4; //Down
    break; 
    case 6: x = 5; y = 0; //right
    break; 
    case 7: x = 0; y = 6; //up
    break;
    case 8: x = -5; y = 0; //left
    break; 
    case 9: x = -5; y = -4; //bottom-left
    break; 
    case 10: x = 5; y = 0; //right
    break;
    case 11: x = 5; y = 7; //top-right
    break;
    case 12: x = -5; y = 0; //left
    break;
    case 13: x = 0; y = -4; //Down
    break; 
    case 14: x = 0; y = 6; enemyState = -1;
    break;
    }
    enemyLayer->velocity.axes[0] = x;
    enemyLayer->velocity.axes[1] = y;
    
    enemyState++;
  }

}
