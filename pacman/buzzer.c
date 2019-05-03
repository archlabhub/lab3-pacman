/* Dima AbdelJaber
 * November 20, 2018
 * Buzzer.c class handles the buzzer
 */

#include <msp430.h>
#include "buzzer.h"	

//Initalizes the buzzer to start it
//freudenthal's code
void buzzer_init(){
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR |= BIT6;		/* enable output to speaker (P2.6) */
}

//Starts the buzzer
void buzzer_stop(){
  P2DIR &= ~BIT6;
}

//Stops the buzzer
void buzzer_start(){
  P2DIR |= BIT6;
}

//Changes speed of the buzzer
void buzzer_set_period(short cycles){
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}


    
    
  

