/* Defect 2101: constants a header spells as statement expressions, which C
   reads inside a function and not at file scope. */
#define ST_PURE (__extension__ ({ 3; }))
#define ST_TWO (__extension__ ({ 3; 4; }))
