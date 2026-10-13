/* Defect 2049, 2026-10-13: the second reproducer, a header that declares
 * the C name this compiler gives the program's `main`, another way. */
int h_fixedbugs2049aheaderthatdeclaresmainscnameisrefused_main(int x);
static inline int eight(void) { return 8; }
