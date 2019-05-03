/* Dima AbdelJaber
 * November 20, 2018
 * Buzzer.h class defines methods used in buzzer.c and pacman.c
 */
#ifndef buzzer_included
#define buzzer_included

void buzzer_init(); //Initial for buzzer to work
void buzzer_set_period(short cycles); //determine the pitch the buzzer will play at
void buzzer_stop(); //Buzzer stops
void buzzer_start(); //Buzzer starts

#endif // included
