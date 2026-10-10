/* Defect 590, 2026-10-11: an evaluation method set through a macro's
   `_Pragma`, in a header another header includes. */
#define FIXEDBUGS_590_EVAL _Pragma("clang fp eval_method(double)")
FIXEDBUGS_590_EVAL
