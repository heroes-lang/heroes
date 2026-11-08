/* Defect 2286's fixed array: a group record holding defect 2253's 128 MiB
 * record in an array of one, so its construction writes the array's elements
 * and no literal of the whole field stands on a frame. */
#include "fixedbugs-2253-held.h"

typedef struct fixedbugs_2286_held_array {
    heroes_held hs[1];
} fixedbugs_2286_held_array;
