/* Defect 591, 2026-10-11: a bound function renamed by the header to a
   linker name no library defines, on every platform. */
#pragma redefine_extname fixedbugs_591_renamed fixedbugs_591_nowhere_renamed
/* Defect 640, 2026-10-11: the header declares the function itself, `long
   long` on every platform, where `labs`'s `long` is 32 bits on Windows. */
long long fixedbugs_591_renamed(long long x);
