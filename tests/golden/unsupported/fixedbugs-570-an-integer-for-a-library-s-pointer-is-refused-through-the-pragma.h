/* Defect 570, 2026-10-10: an integer for a string pointer behind an ignored pragma. */
#pragma clang diagnostic ignored "-Wint-conversion"
#include <string.h>
