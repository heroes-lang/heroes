/* Defect 2101: constants a header spells as statement expressions that clang
   computes each time they are read. */
static int st_counter;
#define ST_SIDE (__extension__ ({ st_counter++; }))
#define ST_DECL (__extension__ ({ int st_x = 3; st_x; }))
#define ST_READ (__extension__ ({ st_counter; }))
