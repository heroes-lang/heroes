/* A header of the program's own, named like no header of the package `pt`,
 * that leaves `#pragma pack(4)` set past its end (defect 2364). */
static inline int four(void) { return 4; }
#pragma pack(4)
