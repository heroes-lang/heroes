/* Panel 214, 2026-10-14: the header of `ffi-a-header-declaring-a-function-the-unit-writes-is-told.hero`.
 * Declared `static`, so clang's words are one on every platform: against
 * the unit's own `static` definition, *conflicting types*; a declaration
 * with external linkage is *static declaration follows non-static* on this
 * Mac and Linux and, Microsoft's extension, *conflicting types* on Windows. */
static void hero_extern_refs(int);
static inline int x6_f(void) { return 1; }
