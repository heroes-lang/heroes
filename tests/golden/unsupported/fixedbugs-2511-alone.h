/* Defect 2511, 2026-11-13: a macro that assigns through its argument, with no
 * function of its name behind it. */
#define alone2511(s) (*(s) = 0, 0)
