/* Defect 585, 2026-10-10: a header that declares a name only where no POSIX level narrows it, as Darwin's string.h declares strlcpy. */
#if !defined(_POSIX_C_SOURCE)
static inline long long wide_only(void) { return 2; }
#endif
