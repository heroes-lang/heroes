/* Defect 2072's other side: values C defines, which build and read as C says. */
#define WRAPPED (18446744073709551615ULL + 1)
#define HIGH_BIT (1u << 31)
#define FROM_A_FLOAT ((int)(1000.0 / 60))
#define NARROWED ((short)70000)
