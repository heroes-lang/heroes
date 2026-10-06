/* Beside defect 400's cases, lane run400, 2026-10-06: three tags with no
 * typedef, one of each kind C keeps in its tag namespace. The struct is
 * declared and never defined; the union and the enum are what a handle
 * cannot be spelled as. */
#include <stdint.h>
struct opaque;
union utag { int32_t i; float f; };
enum colour { RED, GREEN };
