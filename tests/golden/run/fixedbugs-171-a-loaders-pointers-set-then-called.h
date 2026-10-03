/* Beside defect 171's run cases, lane ffimsg, 2026-10-03: a loader's
   shape. volk names each pointer by the API's own name; GLAD names it
   `glad_glX` and makes `glX` a macro over it. Both are null until the
   loader sets them, and C calls them as functions. */
#include <stdint.h>

typedef int32_t (*PFN_add)(int32_t, int32_t);
static PFN_add vkAdd = 0;
static PFN_add glad_glAdd = 0;
#define glAdd glad_glAdd

static inline int32_t real_add(int32_t a, int32_t b) { return a + b; }
static inline void load_all(void) { vkAdd = real_add; glad_glAdd = real_add; }
