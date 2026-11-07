/* Defect 2316's records. `fixedbugs_2316_held` is 1.25 MiB, past the 1 MiB
 * chunk the room takes for a value, so each value of it stands in a chunk of
 * its own, which is what `hero_frame_give` frees; and under every leg's
 * stack with room to spare at `-O0` and under the sanitizer, so nothing but
 * the defect can stop a program over it. One word is named and the rest left
 * to the struct (`partial`). `fixedbugs_2316_box` holds one by value beside a
 * tag, a group record kept off the frame because what it holds is. */
#include <stdint.h>

typedef struct fixedbugs_2316_held {
    int64_t first;
    int64_t rest[(1 << 17) + (1 << 15) - 1];
} fixedbugs_2316_held;

typedef struct fixedbugs_2316_box {
    int64_t tag;
    fixedbugs_2316_held inner;
} fixedbugs_2316_box;
