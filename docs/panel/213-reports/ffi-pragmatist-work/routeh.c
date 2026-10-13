/* Route H at its cheapest: a typedef per element type, the same struct, so the
   runtime's prototypes take it with no cast and HERO_RUNTIME_ABI does not move. */
#include "heroes_runtime.h"
typedef HeroArrayHeader h_arr_int64;      /* what `[i64]` would be declared as */
#define HERO_LLDB_SUMMARY __attribute__((used, section("__TEXT,__lldbsummaries")))
HERO_LLDB_SUMMARY static const unsigned char s1[] =
    "\x01" "\x23" "\x0c" "h_arr_int64" "\0" "\x15" "[i64] len=${var.len}";
int main(void) {
    h_arr_int64 *xs = hero_array_new(&hero_desc_int, 2);
    int64_t n = hero_array_len(xs);
    hero_array_decref(xs);
    return (int)n;
}
