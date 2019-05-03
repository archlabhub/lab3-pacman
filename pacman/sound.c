/* Dima AbdelJaber
 * November 20, 2018
 * music.c class handles the the music played
 */
#include <msp430.h>
#include "sound.h"

static char isplaymusic = 0; //what music should play

//update music
void music_update(int winStreak){
  switch(isplaymusic){
  case 1: break; 
  case 2: break; 
  case 3: music_Dot(); 
  break;
  case 4: music_Dot(); 
  break;
  case 5: music_Dot(); 
  break;
  case 6: music_Dot(); 
  break;
  case 7: music_Dot(); 
  break;
  }
}

/**Starts music of given song*/
void music_start(int typeOfmusic){  
  isplaymusic = typeOfmusic;
  if(isplaymusic){ 
    buzzer_start();
  }
}

//music when dot is collided with pacman
void music_Dot(){
  if(isplaymusic == 3 || isplaymusic == 4){
    buzzer_set_period(400);
    isplaymusic++;
  }
  else if(isplaymusic == 5 || isplaymusic == 6){
    buzzer_set_period(500);
    isplaymusic++;
  }
  else{
    music_stop();
  }
}

//Stops music
void music_stop(){
  isplaymusic = 0;
  buzzer_stop();
}
