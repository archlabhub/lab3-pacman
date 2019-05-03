/* Dima AbdelJaber
 * November 20, 2018
 * pacman.c class handles the game and the colors and the pacman and its actions (eating) it also handles collision and wining and losing
 */
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include "pacman.h"
#include "buzzer.h"
#include "sound.h"
#define GREEN_LED BIT6
#define RED_LED BIT0
#define SWITCHES 0x0f  //Switches

//pacman Dots pacman has. Starts at 0
static char dotsCollected = 0;
//temporary variable
Vec2 centerposition = { {0,0} };

//Velocity changes to zero when the moving layer is touching the region
void checkFences(MovLayer *movLayer, Region *fence){
  Vec2 newposition;
  u_char axis;
  Region shapeboundary;
  for (; movLayer; movLayer = movLayer->next) {
    vec2Add(&newposition, &movLayer->layer->posNext, &movLayer->velocity);
    
    abShapeGetBounds(movLayer->layer->abShape, &newposition, &shapeboundary);
    for (axis = 0; axis < 2; axis ++) {
        //when touching fence
      if ((shapeboundary.topLeft.axes[axis] <= fence->topLeft.axes[axis]) || (shapeboundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	movLayer->velocity.axes[axis] = 0;
      }	//outside of the fence
    } //axis
  } //movLayer
}

//when center of a region1 is inside region2 it will return true
int isPlaceInRegion(Vec2* reg1, Region* reg2){  
  //X-axis in region2
  if (centerposition.axes[0] >= reg2->topLeft.axes[0] && centerposition.axes[0] <= reg2->botRight.axes[0]){
    //Y-axis in region2
    if(centerposition.axes[1] <= reg2->botRight.axes[1] && centerposition.axes[1] >= reg2->topLeft.axes[1] ){
      return 1;
    }
  }
  return 0;
}

//when center of region1 is inside of region2 it returns 1 if center of region1 is in region2, if not it returns 0
int regionsIntersect(Region* reg1, Region* reg2){
  vec2Add((&centerposition), (&(reg1->topLeft)), (&(reg1->botRight)) );
  centerposition.axes[0] /= 2;  
  centerposition.axes[1] /= 2;//center of region 1
  return isPlaceInRegion(&centerposition, reg2);
}

// when moving layer touches the region then the velocity changes to 0
void checkFencesOutside(MovLayer *movLayer, Region *fence){
  Vec2 newposition;
  u_char axis;
  Region shapeboundary;
  for (; movLayer; movLayer = movLayer->next) {
    vec2Add(&newposition, &movLayer->layer->posNext, &movLayer->velocity);
    abShapeGetBounds(movLayer->layer->abShape, &newposition, &shapeboundary);
    for (axis = 0; axis < 2; axis ++) {
      if(regionsIntersect(&shapeboundary, fence)){
	movLayer->velocity.axes[axis] = 0;
      }
    } //axis
  } //movLayer
}

u_int bgColor = COLOR_BLACK;     // the color of the background is set to black
int redrawScreen = 1;           // whether the screen needs to be redrawn or not
static Region fieldFence;		//fence around the play field
static Region obstacleFence0, obstacleFence1, obstacleFence2, obstacleFence3, obstacleFence4; //Obstacle fences

//score of the game
void updateDotText(const int Dots){
  short width = screenWidth/3+20;
  short height = 3;
  int color = COLOR_PURPLE;
  int BGcolor = COLOR_BLACK;
  char text[] = "0";
  
  switch(Dots){
  case 0: text[0] = '0'; 
  break;
  case 1: text[0] = '1'; 
  break;
  case 2: text[0] = '2'; 
  break;
  case 3: text[0] = '3'; 
  break;
  case 4: text[0] = '4'; 
  break;
  case 5: text[0] = '5'; 
  break;
  case 6: text[0] = '6'; 
  break;
  default: text[0] = '7'; 
  break;
  }
  
  drawString5x7(width, height, text, color, BGcolor);
}

//when the game has ends. when state is 1 game is lost and 2 game is won
void gameEnds(const int state){
  if(state != 1 && state != 2){
    return;
  }
  clearScreen(COLOR_VIOLET); //color of the background when display the outcome
  music_stop();
  if(state == 1){
    drawString5x7((screenWidth/2) -15, screenHeight/2-5, "LOSER", COLOR_NAVY, COLOR_VIOLET);
  }
  else{
    drawString5x7((screenWidth/2) -15, screenHeight/2-5, "WINNER", COLOR_NAVY, COLOR_VIOLET);
  }
  
  or_sr(0x10); //turns the cpu off
  and_sr(~8); //turns the Interrupts off
}

//the center position of pacman is Vec2. Stores result into centerposition structure
void _pacmanCenterposition(){
  Region pacman;
  abShapeGetBounds((&pacmanLayer0)->abShape, &((&pacmanLayer0)->pos), &pacman);
  vec2Add((&centerposition), (&(pacman.topLeft)), (&(pacman.botRight)) );
  centerposition.axes[0] /= 2;
  centerposition.axes[1] /= 2; //Finds center of pacman
}

//when player touches a dot plays a music and gets put on top
void objectCollisions(){
  _pacmanCenterposition();
  Region DotRegion;
  Layer* DotLayer;
  int Dot, newX, newY;
  //when pacman collide with dor
  for(Dot = 0; Dot < 6; Dot++){
    switch(Dot){
    case 0: DotLayer = &DotsLayer0; newX = 75; newY = 3; 
    break;
    case 1: DotLayer = &DotsLayer1; newX = 85; newY = 3; 
    break;
    case 2: DotLayer = &DotsLayer2; newX = 95; newY = 3; 
    break;
    case 3: DotLayer = &DotsLayer3; newX = 105; newY = 3; 
    break;
    case 4: DotLayer = &DotsLayer4; newX = 115; newY = 3; 
    break;
    case 5: DotLayer = &DotsLayer5; newX = 75; newY = 10; 
    break;
    default: return;
    }
    
    abShapeGetBounds(DotLayer->abShape, &(DotLayer->pos), &DotRegion);
    if(isPlaceInRegion(&centerposition, &DotRegion)){
      DotLayer->posNext.axes[0] = newX;
      DotLayer->posNext.axes[1] = newY;
      DotLayer->pos.axes[0] = newX;
      DotLayer->pos.axes[1] = newY;
      dotsCollected++; //adds dots collected
      updateDotText(dotsCollected); //updates the text when the pacman and dot ollide
      music_start(3); //music when oacman and dot collide
    }
  }
  if(dotsCollected == 6){ //the player wins when all 6 dots are collected
    gameEnds(2); //the game ends and displays the winning messages
  }
}

//if enemies and pacman collide the game ends and displays a losing message
void enemyCollision(){
  _pacmanCenterposition();
  Region enemyRegion;
  Layer* enemyLayer;
  int enemy;
  for(enemy = 0; enemy < 2; enemy++){
    switch(enemy){
    case 0: enemyLayer = &enemyLayer0; 
    break;
    case 1: enemyLayer = &enemyLayer1; 
    break;
    }
    abShapeGetBounds(enemyLayer->abShape, &(enemyLayer->pos), &enemyRegion);
    if( isPlaceInRegion(&centerposition, &enemyRegion) ){
      gameEnds(1);
    }
  }
}

//Increases the region by amount of pixels in all directions
void increaseRegion(Region* region, const int pixels){
  region->topLeft.axes[0] = region->topLeft.axes[0] - pixels;
  region->topLeft.axes[1] = region->topLeft.axes[1] - pixels;
  region->botRight.axes[0] = region->botRight.axes[0] + pixels;
  region->botRight.axes[1] = region->botRight.axes[1] + pixels;
}

//draw obsticles
void drawAllLayers(){
  Layer obstacleLayer4 = {	//top right layer
      // last and next position ({0,0}, {0,0})
    (AbShape *) &obstacleOutline, {((screenWidth/4)*3)-7, ((screenHeight/4))+6}, {0,0}, {0,0}, COLOR_SKY_BLUE, &pacmanLayer0 
  };
  
  Layer obstacleLayer3 = {  //bottom-left
      // last and next position ({0,0}, {0,0})
    (AbShape *) &obstacleOutline, {((screenWidth/4)+3), ((screenHeight/4)*3)+6}, {0,0}, {0,0}, COLOR_SKY_BLUE, &obstacleLayer4
  };
  
  Layer obstacleLayer2 = {  //center
      // last and next position ({0,0}, {0,0})
    (AbShape *) &obstacleOutline, {(screenWidth/2)-1, (screenHeight/2)+6}, {0,0}, {0,0}, COLOR_SKY_BLUE, &obstacleLayer3
  };
  
  Layer obstacleLayer1 = {		// bottom-right
      // last and next position ({0,0}, {0,0})
    (AbShape *) &obstacleOutline, {((screenWidth/4)*3)-3, ((screenHeight/4)*3)+6}, {0,0}, {0,0}, COLOR_SKY_BLUE, &obstacleLayer2
  };
  
  Layer obstacleLayer0 = {		 //top-left
      // last and next position ({0,0}, {0,0})
    (AbShape *) &obstacleOutline, {screenWidth/4, (screenHeight/4)+6}, {0,0}, {0,0}, COLOR_SKY_BLUE, &obstacleLayer1 
  };
  
  layerInit(&obstacleLayer0); //initial
  layerDraw(&obstacleLayer0); //draw the layer
  layerGetBounds(&obstacleLayer0, &obstacleFence0); //obstacle 0
  layerGetBounds(&obstacleLayer1, &obstacleFence1); //obstacle 1
  layerGetBounds(&obstacleLayer2, &obstacleFence2); //obstacle 2
  layerGetBounds(&obstacleLayer3, &obstacleFence3); //obstacle 3
  layerGetBounds(&obstacleLayer4, &obstacleFence4); //obstacle 4
  increaseRegion(&obstacleFence0, 6);
  increaseRegion(&obstacleFence1, 6);
  increaseRegion(&obstacleFence2, 6);
  increaseRegion(&obstacleFence3, 6);
  increaseRegion(&obstacleFence4, 6);
}

//main. Initializing, enables interrupts and green LED
void main(){
  P1DIR |= GREEN_LED;		//led on when CPU is on		
  P1OUT |= GREEN_LED;
  configureClocks();
  lcd_init();
  buzzer_init();
  shapeInit();
  drawAllLayers();
  layerGetBounds(&fieldLayer, &fieldFence);
  p2sw_init( SWITCHES );
  drawString5x7( 7, 4, "PACMAN", COLOR_MAGENTA, COLOR_BLACK); //pacman on top of the screen in color Magenta
  updateDotText(dotsCollected); //text to update the number of dots collected
  enableWDTInterrupts();      //enables interrupt
  or_sr(0x8);	              //enable interrupts
  for(;;) { 
    while (!redrawScreen) { //Pause the screen when it doesn't need updating
      P1OUT &= ~GREEN_LED;    //led off 
      or_sr(0x10);	      //CPU is OFF
    }
    P1OUT |= GREEN_LED;       //led on as long as the CPU on
    redrawScreen = 0;
    movLayerDraw(&movLayer0, &pacmanLayer0);
  }
}

//timer interrupt handler
void wdt_c_handler(){
  static char count = 0;
  P1OUT |= GREEN_LED;	//LED on whenever the cpu is on
  count ++; //counter

  if (count >= 16) { //if the count is bigger than or equal to 16 then...
    checkFences(&movLayer0, &fieldFence);
    //if objects touch obstacle next position set that objects velocity to 0
    checkFencesOutside(&movLayer0, &obstacleFence0); 
    checkFencesOutside(&movLayer0, &obstacleFence1);
    checkFencesOutside(&movLayer0, &obstacleFence2);
    checkFencesOutside(&movLayer0, &obstacleFence3);
    checkFencesOutside(&movLayer0, &obstacleFence4);
    movLayerAdvance(&movLayer0); //all moving layers advance
    objectCollisions(); //collision with dots or enemies
    enemyCollision();//enemy collision
    music_update(0); //music is updated when touching the dot
    enemyAI(1); //AI's velocity is updated when at 0
    enemyAI(2);
    
    if (p2sw_read())
      redrawScreen = 1;
    count = 0;
  }
  P1OUT &= ~GREEN_LED;		    //LED is off when cpu is off
}

//velocity x and velocity y is changed to match the direction
void changeVelocity(int* velocityX, int* velocityY, int direction){
  switch( direction ){
  case 1: (*velocityX) = 0; (*velocityY) = -3; 
  break; //pacman goes up
  case 2: (*velocityX) = 0; (*velocityY) = 3; 
  break; //down
  case 3: (*velocityX) = -3; (*velocityY) = 0; 
  break; //right
  case 4: (*velocityX) = 3; (*velocityY) = 0;
  break; //left
  default: (*velocityX) = 5; (*velocityY) = 5;
  }
}

//Changes the direction the pacman is moving based on what button was pressed
void updatePacmanMovement(){
  MovLayer* pacmanmovLayer = &movLayer0; //pacman's MovLayer
  Layer* pacmanLayer = &pacmanLayer0; 
  Vec2 newposition;
  int velocityX, velocityY;
  vec2Add(&newposition, &(pacmanLayer->posNext), &(pacmanmovLayer->velocity));
  int direction = 0;

  //direction pacman will go to depending on whats pressed
  switch((P2IFG & (SWITCHES))){
  case BIT0: direction = 1; 
  break; //Button 1
  case BIT1: direction = 2; 
  break; //Button 2
  case BIT2: direction = 3; 
  break; //Button 3
  case BIT3: direction = 4; 
  break; //Button 4
  default: return;
  }

  changeVelocity( (&velocityX), (&velocityY), direction); //Update new velocity based on direction
  pacmanmovLayer->velocity.axes[0] = velocityX; //Update velocity of the Object
  pacmanmovLayer->velocity.axes[1] = velocityY; //Update velocity of the Object
  pacmanLayer->posNext = newposition;
  P2IFG = 0;
}
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  updatePacmanMovement();
}
