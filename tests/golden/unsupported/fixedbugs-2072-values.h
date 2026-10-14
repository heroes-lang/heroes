/* Defect 2072: values C leaves undefined, bound as constants. */
#define SHIFT_PAST (1 << 40)
#define SHIFT_NEGATIVE (-1 << 2)
#define SHIFT_BY_NEGATIVE (1 >> -1)
#define SHIFT_INTO_SIGN (1 << 31)
#define DIVIDED_BY_ZERO (1 / 0)
#define REMAINDER_BY_ZERO (1 % 0)
