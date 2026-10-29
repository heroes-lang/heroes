/* fixedbugs 2238: a library whose C names Heroes cannot spell. */
typedef struct { int x; } match;
typedef struct { int x; } var;
struct test { int x; };
struct let { int x; };
struct pair { int in; int def; int function; };
static inline int func(int x) { return x + 1; }
#define nil 7
