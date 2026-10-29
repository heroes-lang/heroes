/* A library's names, which Heroes cannot spell... */
typedef struct { int x; } match;
typedef struct { int x; } var;
struct test { int x; };
struct let { int x; };
struct pair { int in; int def; };
static inline int func(int x) { return x + 1; }
#define nil 7

/* ...and the program's own second names for them, the routes the messages
   name (defect 2238): a typedef for a type and for a tag, a macro for a field
   and for a constant, a caller for a function. */
typedef match kw_match;
typedef var c_var;
typedef struct test T;
typedef struct let L;
#define kw_in in
#define c_def def
static inline int c_func(int x) { return func(x); }
#define c_nil nil
