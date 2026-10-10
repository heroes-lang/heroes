/* Defect 556, lane b18-ffi, 2026-10-10: and `twice` defined here too, the
   same definition in another header. */
long long twice(long long x) { return 2 * x; }
