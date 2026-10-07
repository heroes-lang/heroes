/* Defect 248's header (panel 189's ffi-pragmatist, 2026-10-04; its surface
 * case, lane b14-harness-b, 2026-10-07): a warning clang gives once for each
 * unit that includes this header. A `#warning` and not the defect's Latin-1
 * byte, the repair's own second shape (2 copies before, 1 after): the case
 * is copied as text, and a byte that is not UTF-8 is no text. */
#warning "defect 248: a bound header's warning, told once"
#define ANSWER 42
