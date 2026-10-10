#define FIXEDBUGS_563_EXTERN(type) extern type
FIXEDBUGS_563_EXTERN(int) fixedbugs_563_jpeg_level(void);
static inline long long jpeg_level(void) { return 80; }
