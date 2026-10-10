/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * heroes_unit.h: the first header of every unit, and of every probe of its
 * headers, the compiler writes.
 *
 * NO HEADER OF THE C LIBRARY IS READ BEFORE A UNIT'S GROUPS (panel 205's R3,
 * defect 568). A switch the library reads once, at its first header
 * (`_GNU_SOURCE` at glibc's `features.h`, `_POSIX_C_SOURCE` at Darwin's
 * `sys/cdefs.h`), works only from a header read before every other, and a
 * unit read `<stdint.h>` through `heroes_runtime.h` and `<math.h>` as a seed
 * first. So a unit this compiler writes says, before it includes the
 * runtime, that it reads the library's standard headers after its groups
 * (`heroes_standard.h`); `heroes_runtime.h` then reads none, and spells its
 * integer types from clang's builtins. A unit written before then, the seed
 * among them, says nothing and reads `<stdint.h>` there as it always did, so
 * it compiles against this runtime unchanged until it is written again.
 *
 * Every probe of a unit's headers opens with this header too
 * (`selfhost/emit/externs.hero`'s `SEEDS`), so it reads the groups' headers
 * in the state the unit reads them. */
#define HERO_STANDARD_AFTER_GROUPS 1
#include "heroes_runtime.h"
