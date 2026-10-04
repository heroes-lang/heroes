---
kind: defect
area: emit
milestone: none
filed: 2026-10-02
commit: 3d704b03777c2359dc9cf746a33b9e9d4561ed25
github: none
---

# Defect 151 closed: a C struct holding an anonymous union is bound as separate fields, so a value built from Heroes reads back wrong

- [x] **151 — a C struct holding an anonymous union is bound as separate fields, so a value built from Heroes reads back wrong** | `extern "u.h"` over `typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;` with `record SA` naming `kind`, `i`, `f`, `x`: `s = SA(kind: 1, i: 7, f: 0.5, x: 3)` builds at exit 0 and `print(s.i)` prints 1056964608, the bits of 0.5, where 7 was written; `==` on it is accepted and prints `true` (u18); a field left out of such a struct is not reported, the program printing 12 (u07); and a record naming `a` and `c` of a struct `{a, b, c}` is told *does not name `c`* (u19) | `selfhost/emit/` (the record's layout check, `ffi_union_field`), panels 060 to 077's union rule · **class: blocking** · **closed 2026-10-03**

    **Origin:** lane literals' first pass for defect 150, 2026-10-02
    (`scratchpad/lane-literals/pass1/U150/`, `u.h`, `u17_anon_constructed.hero`,
    `u18_anon_compared.hero`, `u07_anon_omits_x.hero`,
    `u19_struct_omits_middle.hero`); reproduced by the coordinator at 02:42 on
    2026-10-02 on the trunk at `03e70520`.

    **Why it is a defect.** A program that checks and builds computes a value
    nobody wrote, at the C boundary (design.md §1.12, robustness, and §4.19).
    The lane's route, the header's layout read from clang, changes the union
    rule of panels 060 to 077, so the repair is a sitting's (panel 186).

    **Widened 2026-10-02** by panel 186's completeness critic: ONE declared
    field inside an anonymous union is enough for a wrong answer.
    `one_arm.h`'s `typedef struct { int32_t kind; union { int8_t b; int64_t
    q; }; } SB;` with `record SB` naming `kind` and `b`, `print(make_a() ==
    make_b())` over two values whose `q` differs: `build` exit 0, prints
    `true` (reproduced by the coordinator at 11:02 on `ae08ed93`), where the
    same union bound alone by one member is `ffi_union_field`
    (`docs/panel/186-briefs/probes/critic/one_arm.hero`).

    **2026-10-02, lane land186, the layout read from clang: a field left out
    named by it, two fields of one union not built, a misspelling told
    first** (panel 186 R1, R2, R4): repaired at `f2a08f13`, gated by its
    cases and the compiler's own tests; the net is owed at the batch's
    close, and the platform legs before the push.

    **2026-10-02, lane land186, one comparison rule: a field in a union is
    compared only as an integer, a pointer or an array of them as wide as
    the union** (panel 186 R3, the widening's `SB` among its cases):
    repaired at `75ff04b9`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close, and the platform legs before the
    push.

    **2026-10-02, lane land186, spec § 13 says it, and *as wide as the
    union* is the width clang gives the union's own block** (panel 186 R6):
    at `b7c510c5`, gated by its own cases; the rest is owed at the round's
    gate, and the platform legs before the push.

    **2026-10-02, lane land186, a record over a C union is built naming
    exactly one member of each union, and `build` judges what a
    construction leaves out** (panel 186 R7, home (a), read blind by the
    sitting's third reading): at `8977e7c6`, gated by its own cases; the
    rest is owed at the round's gate, and the platform legs before the push.

    **2026-10-02, lane land186, spec § 13 says it, O_cover in place of
    R_build_cover, and design.md §4.19 says where it is judged** (panel 186
    R7): at `fc0e7284`, gated by its own cases; the rest is owed at the
    round's gate, and the platform legs before the push.

    **2026-10-02, lane land186, R7's cases, the SDL3 event among them**:
    at `ec530b54`, gated by its own cases; the rest is owed at the round's
    gate, and the platform legs before the push.

    **2026-10-02, lane land186, a case that held a GNU fact as everyone's**:
    the Windows box's pre-push leg at `b48d02b8` refused
    `run/fixedbugs-151-members-with-no-bytes-left-out`, an empty struct
    having bytes for MSVC's target, which the compiler was right to refuse
    (R4); the case now holds only the zero-length array, no bytes on every
    target measured, and the empty struct is witnessed missing for
    `x86_64-pc-windows-msvc` and complete for `x86_64-linux-gnu` by
    `selfhost/cli/layout.hero`'s test: at `bac43e50`, gated by its own cases;
    the rest is owed at the round's gate, and the platform legs before the
    push.

    **2026-10-03, its cases read one by one, held for the SDL3 event**: of its 43 cases, `run` 9 and `unsupported` 13 under `fixedbugs-151-` and R7's `ffi-a-construction-` cases this item names (`run` 5, `unsupported` 11, `check` 5), all 43 passed on this Mac (00:30 to 00:42 by `date`) and 42 in the Linux arm64 container (one run, 00:36 to 00:54), where `run/ffi-a-construction-polls-an-sdl3-event` was skipped: alone, *1 of 1 cases were skipped for a missing library*, and built alone, `error[ffi_package]: the package sdl3 is not installed on this machine`. The suites' totals put it skipped on every other leg too, an inference from their arithmetic: the Windows leg's `run` 234 is the Mac's 242 less exactly the eight cases that can skip there, the event among them, and the CI's Linux jobs read 237, less raylib's four and the event, and its Darwin job 241, raylib being installed there and SDL3 not. The event, *the SDL3 event among them*, has run on this Mac alone; the item waits for it to run on another platform, which needs SDL3 installed there (the arm64 image's Debian has no `sdl3` for `pkg-config` today), or for a ruling that a case on a library a leg lacks is judged where the library is.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): a wrong value at the C
    boundary, built from Heroes.

    **Closed 2026-10-03** on the author's answer *4a*: its 42 cases other than the SDL3 event read green on this Mac and in the Linux arm64 container (the held line above), and the SDL3 event green on this Mac, the one platform carrying SDL3; that event's run on another platform is filed as defect 213, an improvement.
