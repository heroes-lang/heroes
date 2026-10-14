#include <unistd.h>
struct cd { char c; double d; };
struct cd pass_cd(struct cd x) { return x; }
long take_cd(struct cd x) { return (long)write(1, &x, sizeof x); }   /* line 4 */
