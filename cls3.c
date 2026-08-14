#include <stdint.h>
struct Box { int64_t a, b; };
typedef struct { const char *p; int64_t n; } HeroStr;
struct Box makebox(int64_t n);
void *aptr(int64_t n);
const char *acstr(int64_t n);
int64_t anint(int64_t n);
double areal(int64_t n);
HeroStr astr(int64_t n);
/* GCC's typeclass.h: pointer_type_class == 5, record_type_class == 12. */
#define HERO_RET_PTR(c) (__builtin_classify_type(c) == 5)
_Static_assert(HERO_RET_PTR(aptr((int64_t)0)), "void * is a pointer");
_Static_assert(HERO_RET_PTR(acstr((int64_t)0)), "const char * is a pointer");
_Static_assert(!HERO_RET_PTR(makebox((int64_t)0)), "a struct is not");
_Static_assert(!HERO_RET_PTR(anint((int64_t)0)), "an integer is not");
_Static_assert(!HERO_RET_PTR(areal((int64_t)0)), "a double is not");
_Static_assert(!HERO_RET_PTR(astr((int64_t)0)), "a HeroStr is not");
