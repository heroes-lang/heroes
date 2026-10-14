/* ffi-pragmatist, panel 215, route E as the compiler's own C type: the gap made a member, so the
   record is no longer the header's struct; what an extern taking the header's pointer then says. */
#include <unistd.h>
struct cd { char c; double d; };                                   /* the header's */
struct hero_cd { char c; unsigned char hero_pad[7]; double d; };   /* route E's */
static long w_cd(const struct cd *p) { return (long)write(1, p, sizeof *p); }
int main(void) {
    struct hero_cd a = {1, {0}, 2.0};
    return (int)w_cd(&a);                                          /* line 9 */
}
