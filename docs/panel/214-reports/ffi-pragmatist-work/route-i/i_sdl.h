/* Route I's stand-in for `extern "SDL3/SDL.h"` in
 * tests/golden/run/ffi-a-construction-polls-an-sdl3-event.hero: the three
 * records as the Heroes declarations name them, each a struct, since a
 * Heroes `record` does not say which of its fields share bytes. */
typedef struct SDL_CommonEvent { uint32_t type; uint32_t reserved; uint64_t timestamp; } SDL_CommonEvent;
typedef struct SDL_UserEvent { uint32_t type; uint32_t reserved; uint64_t timestamp; uint32_t windowID; int32_t code; void *data1; void *data2; } SDL_UserEvent;
typedef struct SDL_Event { uint32_t type; SDL_CommonEvent common; SDL_UserEvent user; } SDL_Event;
bool SDL_Init(uint32_t a0);
bool SDL_PushEvent(SDL_Event *a0);
bool SDL_PollEvent(SDL_Event *a0);
void SDL_Quit(void);
