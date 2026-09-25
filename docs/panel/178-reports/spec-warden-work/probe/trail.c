struct t { long a; int b; };
_Static_assert(sizeof(struct t) == 16, "trailing");
