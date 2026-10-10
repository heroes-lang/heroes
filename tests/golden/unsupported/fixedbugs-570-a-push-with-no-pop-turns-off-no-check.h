/* Defect 570, 2026-10-10: a push and an ignored "-Wsign-conversion" with no pop */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#include <stdlib.h>
