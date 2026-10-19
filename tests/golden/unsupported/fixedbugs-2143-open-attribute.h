/* Defect 2143's header (panel 225's critic, `first`): a value C defines,
   then a `#pragma clang attribute push` left open, so a unit reading it is
   refused inside it. */
#define K_ONE 1
#pragma clang attribute push (__attribute__((no_sanitize("signed-integer-overflow"))), apply_to = function)
