- [ ] **panel 131** | A Heroes program has a `record Room`. Its compiled binary either does or does not contain a piece of data describing that type. Which is it, and what decides?

    **Where to look:** the emitted C for a small program, `HERO_TU_QUIET static
    const HeroDesc` in it, and `runtime/heroes_runtime.h`'s `struct HeroDesc`.

    **Why it matters:** this one fact is what refused run-time reflection on
    2026-09-11, and the answer surprised the sitting. A descriptor is written per
    **translation unit** and **only when the type is used as a container
    element** — so a two-record program with no `[Room]` and no `{str: Room}`
    emits **zero** user descriptors, and a `Room` sitting in a local variable has
    no way at all to say what it is. The record had believed the opposite since
    2026-09-06.
