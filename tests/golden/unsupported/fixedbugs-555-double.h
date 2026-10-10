/* Defect 555, lane b18-ffi, 2026-10-10: `twice` declared to take and give a
   `double`, where the header that defines it says `long long`. */
double twice(double x);
