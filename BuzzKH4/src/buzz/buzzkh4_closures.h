#ifndef BUZZKH4_CLOSURES_H
#define BUZZKH4_CLOSURES_H

#include <buzz/buzzvm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct string {
  char *ptr;
  size_t len;
};


/*

 * prextern int() function in Buzz
 */
extern int buzzkh4_print(buzzvm_t vm);

/*
 * set_wheels(ls,rs) function in Buzz
 * Sets the wheel speeds to ls (left) and rs (right)
 * speeds are expressed in cm/s
 */
extern int buzzkh4_set_wheels(buzzvm_t vm);

/*
 * set_leds(r,g,b) function in Buzz
 * Sets the color of the 3 leds to (r,g,b)
 * speeds are expressed in cm/s
 */
extern int buzzkh4_set_leds(buzzvm_t vm);

/*
 * Updates battery information in Buzz
 */
extern int buzzkh4_update_battery(buzzvm_t vm);

extern int buzzkh4_update_simulated_battery(buzzvm_t vm);

/*
 * Updates IR information in Buzz
 * Proximity and ground sensors
 */
extern int buzzkh4_update_ir(buzzvm_t vm);

extern int BuzzGoTo(buzzvm_t vm);

extern int BuzzStartProcessing(buzzvm_t vm);

extern int BuzzStopProcessing(buzzvm_t vm);

extern int BuzzStopCharging(buzzvm_t vm);

extern int BuzzStartCharging(buzzvm_t vm);

extern int BuzzCreateBalancedForest(buzzvm_t vm);

extern int BuzzCreateTree(buzzvm_t vm);

extern void init_string(struct string *s);

extern int soc;



#endif
