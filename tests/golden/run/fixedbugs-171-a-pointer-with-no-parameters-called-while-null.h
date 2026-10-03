/* Beside defect 171's run cases, lane ffimsg, 2026-10-03: a function
   pointer taking nothing and giving nothing, null. */
static void (*tick)(void) = 0;
