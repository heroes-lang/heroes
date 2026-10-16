/* Defect 2074: a header constant whose value is a comma expression, which C
   defines as its right operand. */
#define K_COMMA (1, 2)
#define K_COMMAS (1, 2, 3)
