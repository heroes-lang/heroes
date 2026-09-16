- [x] **048 — `heroes build --emit-c` emits C that does not compile, for any `tag` binding** | and, found at the sitting, it also skips the pointee check, so a binding writing eight bytes into a four-byte slot emitted at exit 0 and compiled clean | **CLOSED 2026-09-16**, both halves, each needing the opposite shape | `docs/panel/157-the-artifact-of-a-build-that-never-happened.md` · `selfhost/cli/compile.hero` · `selfhost/cli/produce.hero` · `tests/harness/suite_emission.hero`

    **Origin:** panel 156's ffi-pragmatist, at its own boundary and unasked —
    that sitting was about a crash message. Panel 157 was convened on it and
    **widened it**: what was filed as an artifact bug turned out to carry a
    §1.12 corruption with five committed witnesses.

    **THE CAUSE WAS ONE LINE'S POSITION.** `cli/compile.hero` wrote the C and
    returned before `cli/produce.hero` ran at all, so the artifact was the FIRST
    emission — before the pointee check, which is a refusal, and before the tag
    round, which learns a spelling from clang's own words. Both halves of the
    defect are that one early return.

    **The two halves needed OPPOSITE shapes, and that is why the sitting was
    worth holding.**

    | half | shape | why it could not be the other |
    |---|---|---|
    | the pointee check (§1.12) | a **refusal** | its *no* is what stops the corruption |
    | the tag qualifier | an **advisory probe** — ask clang, **discard the verdict**, keep only the spelling | fourteen `fixedbugs/` cases exist precisely to have C where a build has none, and a refusal would delete them |

    **The §1.12 half, measured.** A binding `function fill(@n: i32)` against
    `static inline void fill(uint64_t *n)`:

    | | before | after |
    |---|---|---|
    | `build` | exit 1, `error[ffi_parameter_type]` | unchanged |
    | `--emit-c` | **exit 0** | **exit 1**, the same diagnostic on the author's line, with its fix |
    | clang on that artifact | **exit 0, 0 errors, 0 warnings** | there is no artifact |
    | running it | printed `2863311530` — `0xAAAAAAAA` over the `guard: i64 @ 123456` in the next stack slot | — |

    Four of the five committed silent cases now refuse at `--emit-c`:
    `ffi-pointee-opaque`, `-sign`, `-void`, `-width`. **`ffi-missing-link` still
    emits at 0, and that is correct rather than a remainder**: a library nobody
    named is a LINKER failure, and no check that stops before the link can see
    it. The instrument for it is panel 157's R4, which is not built.

    **The tag half, measured** on `tests/golden/surface-fixtures/structtag/`:

    | | `struct probe` in the artifact | clang errors on it |
    |---|---|---|
    | before | **0** | **20** |
    | after | 21 | **0** |

    **Why clang and not a rule**, settled at the sitting by compiling four
    header shapes: `struct X *` fails on a typedef of an ANONYMOUS struct, the
    bare word fails on a tag-only header, and a typedef shim at the top of the
    artifact fails on the anonymous shape too. **No fixed spelling is correct**,
    so the probe is unavoidable, and the only clang-free alternative left is a C
    declaration parser inside Heroes — which the compiler seat's veto is aimed
    at, held and not cast.

    **A regression the repair caused and the net caught.** Removing the early
    return dropped `--emit-c` into the `needs_main` check, and 23 emission cases
    went red at once: `no_entry_point`, on files whose whole purpose is to
    produce C for another program to compile — which that diagnostic's own note
    recommends. The rule is now stated positively where it belongs: a `main` is
    what a BINARY needs, and `--emit-c` produces no binary.

    **What `emission` learned, and it maintains itself.** Its stated premise —
    *"`--emit-c` never calls clang"* — is **false** now and is corrected under
    its date rather than deleted; its substance survives, because the tag half
    is advisory. A `fixedbugs/` case refused at exit 1 is a **pass**, on the
    condition that **nothing is blessed for it**: so retiring the four blessed
    files is what records the decision, and the decision cannot drift from the
    instrument. A list of names would have been a premise about the world and
    would have expired in silence.

    **Verified**: `emission` 478 passed, 0 failed (from 482 — the four cases now
    assert the refusal instead of comparing bytes, **which falsifies panel 157's
    prediction that the count would not fall**, and the prediction is scored
    that way rather than reworded); the compiler's own tests **654**, after one
    of them was corrected — it asserted `stopped` on the `--emit-c` path, which
    was the old contract; the net's own tests 154; and the full net. **The seed
    was regenerated and the fixpoint verified through the REPAIRED path**, which
    is the interesting direction: the second emission runs the probe and the
    pointee check and is byte-identical to the first.

    **What panel 157 priced and this did NOT build**: R4's instrument, which
    would build every case both ways and compare exit code and stdout, and
    **link** rather than merely compile. It is the only thing that would have
    caught this class, and it stays owed.
