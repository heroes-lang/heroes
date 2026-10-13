/* Defect 2045, 2026-10-13: a header that declares `hero_os_pid`, which the
 * runtime's `hero_os.h` declares in every unit, with another signature. */
int hero_os_pid(int x);
static inline int five(void) { return 5; }
