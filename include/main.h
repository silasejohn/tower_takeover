#ifndef _PROS_MAIN_H_ //has this file/line been defined earlier or in an included file? (If not defined)
#define _PROS_MAIN_H_ //include gaurd?

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 *
 * For instance, E_CONTROLLER_MASTER has a shorter name: CONTROLLER_MASTER.
 * E_CONTROLLER_MASTER is pedantically correct within the PROS styleguide, but
 * not convienent for most student programmers.
 */
#define PROS_USE_SIMPLE_NAMES

/**
 * If defined, C++ literals will be available for use. All literals are in the
 * pros::literals namespace.
 *
 * For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
*/
#define PROS_USE_LITERALS

// #define DEBUG

// Nice includes below (dependecies)
#include "api.h"
#include "pros/apix.h"
#include "pros/rtos.h"
#include "okapi/api.hpp"

// Global Constants
#define IDLE 0

using namespace okapi;

#ifdef __cplusplus //im guessing this means, if this is a c++ file, and we want to include the following c code
extern "C" {       //do the following
#endif

void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);

#ifdef __cplusplus
}
#endif //cleaning up formatting

#ifdef __cplusplus //if (not completely sure about this part yet so come back to it)

#include <ostream> //output streams
#include <iostream> //input and output streams
#include <chrono> //time
#include <ctime> //time
#include <cstdlib> //randomness, conversions, with other things as well

#include <vector> //ArrayList basically

#include <stdio.h> //input output langauge but used in C? (not c++)
                   //<cstdio> in c++, but uses printf, removing all docs and stuff
#include <stdarg.h> //also for c, but basically unlimited arguments to a function (unknown typr and number)
// this is basically variadic functions (write your params, then an ellipses for unknowns)
#endif

#endif  // _PROS_MAIN_H_
