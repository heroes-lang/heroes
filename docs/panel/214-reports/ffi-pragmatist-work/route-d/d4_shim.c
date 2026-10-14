/* Route D, d4: SDL_Event as the program's unit could re-declare it from the
 * Heroes record of tests/golden/run/ffi-a-construction-polls-an-sdl3-event.hero,
 * which names three of the union's members (panel 186's R7 lets it), held
 * against the header in the shim's unit. */
#include <SDL3/SDL.h>
#include <stdint.h>
struct hero_rec_SDL_CommonEvent { uint32_t type; uint32_t reserved; uint64_t timestamp; };
struct hero_rec_SDL_UserEvent { uint32_t type; uint32_t reserved; uint64_t timestamp; uint32_t windowID; int32_t code; void *data1; void *data2; };
union hero_rec_SDL_Event { uint32_t type; struct hero_rec_SDL_CommonEvent common; struct hero_rec_SDL_UserEvent user; };
_Static_assert(sizeof(union hero_rec_SDL_Event) == sizeof(SDL_Event), "the program's SDL_Event is the header's size");
