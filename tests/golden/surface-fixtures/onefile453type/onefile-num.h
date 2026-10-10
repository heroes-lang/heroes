#ifdef ONEFILE_WIDE
typedef long long onefile_num;
#else
typedef int onefile_num;
#endif
onefile_num onefile_get(void);
static inline int onefile_size(void) { return (int)sizeof(onefile_num); }
