/* Defect 2286's part (a): defect 2278's chain rebuilt over a header's records,
 * each level two halves of the one below in an array, 1 MiB at the top. */
#include <stdint.h>

typedef struct fixedbugs_2286_g0 { int64_t v[2]; } fixedbugs_2286_g0;
typedef struct fixedbugs_2286_g1 { fixedbugs_2286_g0 h[2]; } fixedbugs_2286_g1;
typedef struct fixedbugs_2286_g2 { fixedbugs_2286_g1 h[2]; } fixedbugs_2286_g2;
typedef struct fixedbugs_2286_g3 { fixedbugs_2286_g2 h[2]; } fixedbugs_2286_g3;
typedef struct fixedbugs_2286_g4 { fixedbugs_2286_g3 h[2]; } fixedbugs_2286_g4;
typedef struct fixedbugs_2286_g5 { fixedbugs_2286_g4 h[2]; } fixedbugs_2286_g5;
typedef struct fixedbugs_2286_g6 { fixedbugs_2286_g5 h[2]; } fixedbugs_2286_g6;
typedef struct fixedbugs_2286_g7 { fixedbugs_2286_g6 h[2]; } fixedbugs_2286_g7;
typedef struct fixedbugs_2286_g8 { fixedbugs_2286_g7 h[2]; } fixedbugs_2286_g8;
typedef struct fixedbugs_2286_g9 { fixedbugs_2286_g8 h[2]; } fixedbugs_2286_g9;
typedef struct fixedbugs_2286_g10 { fixedbugs_2286_g9 h[2]; } fixedbugs_2286_g10;
typedef struct fixedbugs_2286_g11 { fixedbugs_2286_g10 h[2]; } fixedbugs_2286_g11;
typedef struct fixedbugs_2286_g12 { fixedbugs_2286_g11 h[2]; } fixedbugs_2286_g12;
typedef struct fixedbugs_2286_g13 { fixedbugs_2286_g12 h[2]; } fixedbugs_2286_g13;
typedef struct fixedbugs_2286_g14 { fixedbugs_2286_g13 h[2]; } fixedbugs_2286_g14;
typedef struct fixedbugs_2286_g15 { fixedbugs_2286_g14 h[2]; } fixedbugs_2286_g15;
typedef struct fixedbugs_2286_g16 { fixedbugs_2286_g15 h[2]; } fixedbugs_2286_g16;
