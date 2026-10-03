/* Defect 185's second shape: a struct whose only member is an array of
   float. */
typedef struct {
    float weights[2];
} Pair;

static inline float sum_of(Pair p) { return p.weights[0] + p.weights[1]; }
