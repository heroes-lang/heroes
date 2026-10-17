/* An operation C leaves undefined for its operands, inside the argument of
   __builtin_constant_p, which C never computes: the value is 7 either way. */
static int unevaluated_one = 1;
#define B_SEVEN (__builtin_constant_p(2147483647 + unevaluated_one) ? 7 : 7)
