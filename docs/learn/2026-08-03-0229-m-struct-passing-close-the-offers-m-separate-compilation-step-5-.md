- [ ] **M-struct-passing close — the offers** | M-separate-compilation step 5 (the library is a module too) | `hero_args_count()` from your program was accepted at exit 0 until today, and `args()` still is. What is the difference between the two, seen from the C that gets emitted — and why do the library's CONSTANTS (`HERO_OS_OK`) stay reachable while its extern functions do not?

    **Where to look:** selfhost/resolve/walk.hero (the is_library gate) · tests/golden/check/extern-across-modules-library.hero
    **Why it matters:** the wrapper is not bureaucracy: it is the place the checks live, and the constant's accessor is compiler-written on both sides
