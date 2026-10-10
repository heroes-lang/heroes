/* Defect 591, 2026-10-11: the header's own declaration gives the bound
   function a linker name, by an `__asm__` label, that nothing defines. */
long long fixedbugs_591_twice(long long x) __asm__("fixedbugs_591_nowhere_twice");
