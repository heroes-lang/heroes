/* Defect 591, 2026-10-11: a header another header includes, renaming
   `labs` to a linker name no library defines. */
#pragma redefine_extname labs fixedbugs_591_inner_labs
