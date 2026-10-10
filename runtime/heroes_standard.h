/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * heroes_standard.h: the standard headers a unit reads after its groups, and
 * the ten names its own C takes from them.
 *
 * A SWITCH STANDS BEFORE EVERY HEADER OF THE C LIBRARY (panel 205's R3,
 * defect 568). glibc reads `_GNU_SOURCE` once, at the first header that
 * includes `features.h`, and Darwin `_POSIX_C_SOURCE` at the first that
 * includes `sys/cdefs.h`; a unit read `<stdint.h>` through `heroes_runtime.h`
 * and `<math.h>` as a seed before any group, so a header of the program's own
 * defining the switch did nothing wherever it stood. So `heroes_runtime.h`
 * spells its integer types from clang's builtins and reads no header of the
 * library where the unit says it reads them after its groups
 * (`heroes_unit.h`), and the unit includes this header after its groups'
 * guard (`selfhost/emit/decls.hero`), or after the compiler's own headers
 * where it binds no group.
 *
 * WHAT THE GUARD GAVE BACK IS DEFINED HERE AGAIN. The guard saves before the
 * groups, and gives back after them, the ten macros of these headers the
 * unit's own C writes, `INT64_C`, `UINT64_C`, `INT64_MIN` and `HUGE_VAL`,
 * and the six defect 567's division guard writes at an operand's own width,
 * `INT8_MIN`, `INT16_MIN`, `INT32_MIN` and `INT8_C`, `INT16_C`, `INT32_C`
 * (`selfhost/emit/guarded_names.hero`, defect 361): a group's
 * `#define INT64_C(c) 0` must not reach a literal of the program. Undefined
 * before the groups, each is undefined after them, whatever a group or the
 * library defined, and the library's include guard keeps the two lines below
 * from defining it again. So each is defined from what clang itself
 * predefines, the same value and type the library gives on every platform
 * this compiler builds for. Every other name of the two headers is left as
 * the headers made it, a group's macro that expands to one among them. The
 * compiler's test holds its emitter to these ten (`emit/macro_guard.hero`).
 *
 * `runtime.c` includes this header, which compiles it with the runtime and
 * puts it under the key every cached object answers to. */
#include <stdint.h>
#include <math.h>

#undef HERO_STANDARD_PASTED
#undef HERO_STANDARD_PASTE
#define HERO_STANDARD_PASTED(c, suffix) c##suffix
#define HERO_STANDARD_PASTE(c, suffix) HERO_STANDARD_PASTED(c, suffix)

#ifndef INT64_C
#  define INT64_C(c) HERO_STANDARD_PASTE(c, __INT64_C_SUFFIX__)
#endif
#ifndef UINT64_C
#  define UINT64_C(c) HERO_STANDARD_PASTE(c, __UINT64_C_SUFFIX__)
#endif
#ifndef INT64_MIN
#  define INT64_MIN (-__INT64_MAX__ - 1)
#endif
#ifndef INT8_C
#  define INT8_C(c) HERO_STANDARD_PASTE(c, __INT8_C_SUFFIX__)
#endif
#ifndef INT16_C
#  define INT16_C(c) HERO_STANDARD_PASTE(c, __INT16_C_SUFFIX__)
#endif
#ifndef INT32_C
#  define INT32_C(c) HERO_STANDARD_PASTE(c, __INT32_C_SUFFIX__)
#endif
#ifndef INT8_MIN
#  define INT8_MIN (-__INT8_MAX__ - 1)
#endif
#ifndef INT16_MIN
#  define INT16_MIN (-__INT16_MAX__ - 1)
#endif
#ifndef INT32_MIN
#  define INT32_MIN (-__INT32_MAX__ - 1)
#endif
#ifndef HUGE_VAL
#  define HUGE_VAL (__builtin_huge_val())
#endif
