/* Defect 570, 2026-10-10: three ignored pragmas (-Wsign-conversion, -Wshorten-64-to-32, -Wincompatible-pointer-types), defect 570 as filed, */
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wincompatible-pointer-types"
#include <stdlib.h>
