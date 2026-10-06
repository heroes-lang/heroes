struct pt { int x; int y; };
struct slot { char name[4]; int id; };
struct grid { signed char g[3][4]; };
struct outer { struct slot inner; struct pt pts[3]; };
