# Panel 205, spec-warden

Copied by the coordinator at 03:13 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 205, spec-warden

I started at 02:52 and finished writing at 03:13 (`date`). I worked in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/205-spec-warden/`. My copy is `tree/`, rsynced from `lane-panel-205` without `.claude/worktrees`. I removed its `.git` at once and ran no git in it. I built its compiler from the seed (`heroes 0.2.0`). My running notes are in `notes.txt`, the drafts in `drafts/` and the probes in `probes/`.

- **`verdict`**: **object, provisional**. Every price below comes from the vendored tables, a lower bound, with no `--refresh`. Per item:
  - **Approve, at 0 spec tokens:** the re-raise half of the engineer's route H. It makes § 13 true again.
  - **Object to every Q2 sentence.** W1 would be a **veto** if anyone proposes it: it is a sentence the compiler contradicts today.
  - **Object to these Q1 drafts:**
    - H1 and H2b are false.
    - H3 is a new form, and it cannot carry `200112L`.
    - H2, H2c and H2e state the same truth as H2f at a higher price.
  - **H2f (or H2d)**: approve only together with a built route that reads the module's first group before any libc header, and paid by P2.
  - **Object to route H's blanket silence over a header the program writes itself.**
  - No veto on budget.

- **`section`**: design.md §1.6 (the payment rule), §1.2, and Principle 0 (CLAUDE.md § 2). For the objection to route H's silence: §1.12, and §4.19's `ffi_macro_name` paragraph, whose note drafts a `static inline` function in a header of the program's own. §1.6 does not cover that objection; I say so here. For the false message: §4.17.

- **`spec_token_delta`**: measured on the vendored tables (claude-legacy / cl100k), lower bounds.
  - **Starting point:**
    - Frozen spec: 7348 / 7479. Real 9847, recorded 2026-10-09.
    - Headroom is 393; after the FFI floor (60), 333 are spendable.
    - Panel 203 adds +38/+40 (the critic's `spec_V3T6.md`). Panel 204's F2+G1m adds +33/+34.
    - Both together: **7419 / 7553 (+71/+74)**. That is the base for every draft below.
  - **Q1 drafts:**

    | draft | delta |
    |---|---|
    | H0 (no sentence) | 0 |
    | **H2f** | **+9/+9** (7428/7562) |
    | H2g | +10/+10 |
    | H2d | +11/+11 |
    | H2h | +15/+15 |
    | H1 | +2/+2 |
    | H2b | +17/+17 |
    | H2c | +17/+17 |
    | H2e | +20/+21 |
    | H2 | +33/+34 |
    | H3 | +30/+30 |

  - **Q2 drafts:**

    | draft | delta |
    |---|---|
    | W0 (no sentence) | 0 |
    | W1 | +21/+20 |
    | W2 | +25/+25 |

  - **Real counts are unmeasured.** By the ratio 9847/7479 = 1.317 (an inference):
    - H2f would cost about +12 real.
    - About 236 real tokens remain after 203 and 204 land.
    - Even with H2f, the spec plus the floor stays under 10240.

- **`removal`**: none is owed for H0, W0 or the re-raise, which cost 0 tokens. For H2f: nothing, and that is a problem. It is payable only by the registered predictions P1 and P2.

- **`needed_for_self_hosting`**: no.
  - `selfhost/` binds only `stdlib.h` and the compiler's own three headers (`grep '^extern "' selfhost`).
  - The runtime's unit sets `_GNU_SOURCE` for itself (`runtime/runtime.c:92-94`).

- **`argument`**: § 13's *one that disagrees is refused* is false today, and the repair costs no token. Through a header's own pragma, these all build: a wrong sign, a handle of another tag, a callback, an integer passed for a pointer, and an undeclared function. That is 12 shapes, and route H's re-raise refuses all 12. No Q2 sentence is true: W1 fails on this Mac by an environment variable alone, and W2 fails under H. Q1's cheapest true sentence, H2f (+9/+9 vendored), holds only when a module's first group comes before every libc header. I simulated that on the emitted unit on both platforms; it is false on today's compiler and under H, and nobody has built it. And route H as built silences a program's own header: a missing return builds and prints garbage.

- **`prediction`**:
  - **P1.** If H2f lands, the landing's `--refresh` reads it at +10 to +14 real. Instrument: `heroes measure`; scored at the landing.
  - **P2.** The blind seat runs 4 readers per arm on a Linux task that needs `sched_getcpu`, against a compiler with a first-group route.
    - With H2f, at least 3 of 4 write a header of their own defining `_GNU_SOURCE`, named by the module's first group, on the first try.
    - Without H2f, at most 1 of 4 do.
    - If both arms reach 3, H2f stays out.
  - **P3.** Route H's re-raise refuses all 12 wrong bindings that build today, and leaves the 4 correct ones printing what they print today. **Scored a hit at 03:11 on `heroes-h`.** My narrower route C missed `ifd`.
  - **P4.** The split route (inside the groups' region, every check flag raised again except `sign-conversion`) refuses `own.hero` and compiles GMP and libavutil.
    - **A hit at C level** on this Mac.
    - Owed again on a built compiler, against the ffi-pragmatist's Mac census.

- **`condition`**:
  - **H2f becomes approve** (with P2) once a compiler that reads a module's first group before every libc header is built, and on it:
    - `own` and `first` build and print `true` on Linux arm64;
    - `sw.hero` is refused on this Mac.
  - **H0 instead** if P2's arm without H2f reaches 3 of 4.
  - **My objection to H lifts** if the groups' region keeps `return-type`, `uninitialized`, `conditional-uninitialized` and `shorten-64-to-32` as errors. Measured at C level: `own.hero` is then refused and GMP and libavutil still compile. It also lifts if the sitting records the loss and amends §4.19's `ffi_macro_name` paragraph to say a header's C is not judged.

## What I measured (all run in my folder)

**1. Defect 570 is wider than filed.** All runs below are on today's compiler, `probes/s570` and `probes/eng`.

| shape | today |
|---|---|
| `abs(x: u32)` against `int abs(int)` through a header with `ignored` | builds, prints 5 |
| … the same with `GCC diagnostic ignored` | builds, prints 5 |
| … with `diagnostic warning` (downgrade) | builds, prints 5 |
| … with `ignored "-Wconversion"` | builds, prints 5 |
| … with `ignored "-Weverything"` | builds, prints 5 |
| … with `push` and no `pop` | builds, prints 5 |
| … with balanced push/pop, or `#pragma clang system_header` | refused |
| `take_a(p: B)` against `struct a *`, with `ignored "-Wincompatible-pointer-types"` | builds, prints 1 |
| a callback, with `ignored "-Wincompatible-function-pointer-types"` | builds |
| `read_p(p: i64)` against `int *`, with `ignored "-Wint-conversion"` | builds; **aborts 134 at run time** |
| engineer's `mac` (`_Pragma` inside a constant's macro) | builds |
| engineer's `ifd` (`getpid` bound through a header that does not declare it) | builds |
| engineer's `icv` | builds |
| `@n: i64` against `size_t *`, and a K&R `int kr()` | refused anyway (not part of the hole) |

- `heroes-h` (copied read-only from the compiler-engineer's tree) refuses all 12 shapes that build today. The four correct programs (`ign_ok`, `every_ok`, `hi_ok`, `g2`) print the same output on both compilers.
- My own route C prototype (`tree/heroes-c`, six flags added to `PROBE_ERRORS`) leaves `ifd` building. So route H's list is the complete one on the shapes I ran.
- On this Mac, none of the 8 Homebrew headers with an unbalanced pragma touches a check flag. Today the hole is reached through a header the program writes itself.

**2. What route H costs.** `probes/own/own.hero` binds a header of the program's own that has a missing return, a 64-to-32 truncation and an uninitialised read.
- Today, and under route C, it is refused `ffi_header_refused`.
- Under `heroes-h` it builds and prints `1 705032704 4786664`.
- At C level (`probes/split`), raising every check flag again inside the groups' region except `sign-conversion` refuses it with 3 errors, while `gmp.h` and `libavutil` compile.

**3. Q2's status quo cannot be stated truthfully.** `probes/q2`:
- `g.hero` through `package` is refused.
- `link` with `CPATH` is refused.
- `link` with `C_INCLUDE_PATH` builds and prints `6`.
- Under `heroes-h`, both `package` and `CPATH` print `6`.

**4. Where a switch has to stand.**

On this Mac, `probes/q1`, plain C with the compiler's flags:

| order | `strlcpy` under `_POSIX_C_SOURCE` |
|---|---|
| `cfg.h` after clang's `stdbool`/`stddef` only | hidden, rc 1 (what the switch means) |
| `stdio.h` first, then `cfg.h` | visible, rc 0 (the switch does nothing) |
| `stdint.h` first (today's prefix) | visible |
| the real emitted `sw` unit, group block moved above the prefix | refused for `strlcpy`, its only error |

On Linux arm64 (Docker, clang 22.1.8):

| order | `sched_getcpu` |
|---|---|
| plain C, switch header first | declared |
| plain C, `stdio.h` group first | undeclared |
| plain C, today's prefix first | undeclared |
| the real `first` unit as emitted | undeclared, rc 1 |
| the same unit with `<gnu.h>` read first | rc 0 |

- So a switch works only from the **first** group. That is what makes H1 and H2b false.
- Today's compiler, built inside the container: `plain`, `own` and `first` are all refused `ffi_unknown_name`.
  - **The message is false**: *`gnu.h` declares no `sched_getcpu`*, when under the switch it does. That breaks §4.17's promise and is a row for defect 568.
- A config-only group followed by `sched.h` is refused `expected_extern_block`. I found no ruling on member-less groups in the frozen tree's `docs/panel/*.md`.

**5. The suites on H2f** (put in place of the spec in my copy, then restored, `cmp`-identical to the frozen spec):
- `spec`: 19 passed, 4 failed. The four are `budget`, `spendable`, `real` and `ledger`, the counts a landing moves.
- `grammar`: 9 passed, 0 failed.

## The drafts, whole

Each draft amends 204's G1m (base: `drafts/base1.md`). The words after the semicolon replace *C reads a module's headers in the order its groups are written, so one that needs another's names comes after it.*

- **H2f** (+9/+9; true only with a first-group route, simulated): *C reads a module's headers in the order its groups are written: one that needs another's names comes after it, and one defining `_GNU_SOURCE` first.*
- **H2d** (+11/+11; keeps G1m word for word, the conservative alternative): *…so one that needs another's names comes after it, and one defining `_GNU_SOURCE` comes first.*
- **H2h** (+15/+15): *…comes after it, and one setting a switch such as `_GNU_SOURCE` first.*
- **H2g** (+10/+10): *…comes after it and one setting `_GNU_SOURCE` before all.*
- **H2c** (+17/+17): *…and one that sets a switch such as `_GNU_SOURCE` comes first.*
- **H2e** (+20/+21): *…and a header of your own setting a switch such as `_GNU_SOURCE` comes first.*
- **H2** (+33/+34): *…and a macro every header must see, such as `_GNU_SOURCE`, is defined in a header of the program's own that the first group names.*
- **H1** (+2/+2; false, see the `stdio.h`-first rows): *…so one that needs another's names or macros comes after it.*
- **H2b** (+17/+17; false for the same reason): *…so one that needs another's names, or a macro such as `_GNU_SOURCE`, comes after the group whose header defines it.*
- **H3** (+30/+30; a new form, unbuilt, and it cannot write `_POSIX_C_SOURCE 200112L`):
  - the production becomes `Extern = "extern" string [ ( "link" | "package" ) string ] [ "define" string ] NEWLINE`;
  - after the package sentence: *`define "_GNU_SOURCE"` after a group's head defines that macro before every header of its module.*
- **W1** (+21/+20; false): *…when the symbols need one; a warning clang makes an error refuses the program inside a header as in its own C, and C reads…*
- **W2** (+25/+25; false under H and under the split): *…when the symbols need one; a warning in a library's own header never refuses a program, one in a header of its own does, and C reads…*
- **design.md §4.19** (0 spec tokens), `drafts/design-4.19-switch.md`, to replace 204 R1's clause *a header of the program's own as the one place a switch … goes*. That clause is false today for a switch libc reads once at its first header (`sw` on this Mac, `first` on Linux), so it must not land before a route that makes it true:

  > A switch reaches a module's headers only from its first group (panel 205). A macro libc reads once, at its first header (`_GNU_SOURCE` at glibc's `features.h`, `_POSIX_C_SOURCE` at Darwin's `sys/cdefs.h`), is defined in a header of the program's own that the module's first group names, and no header of the compiler's own that reads one comes before it. Until panel 205 the prefix's `<stdint.h>` and `<math.h>` came first and latched every such switch: `sched_getcpu` was refused `ffi_unknown_name` on Linux arm64 through a header defining `_GNU_SOURCE`, the message saying it declares no such name, and `strlcpy` stayed declared on this Mac under `_POSIX_C_SOURCE 200112L`. A macro a header tests itself (`_XOPEN_SOURCE` for `<ucontext.h>`) worked from any earlier group, and does.

## What a reader writes for `p205/gnu` and `p205/sw`

| draft | `p205/gnu` (Linux) | `p205/sw` (Mac) |
|---|---|---|
| today's text plus G1m, today's compiler | `plain.hero`, refused with a false message. Retrying `own`/`first` is refused too. Only `redecl` builds, unchecked. | `sw.hero` builds and prints `1`; the switch does nothing (I ran it, on `heroes-h` too) |
| H2f (or H2d) with a first-group route | `own`/`first` on the first try: rc 0, simulated on the real unit | refused for `strlcpy`, as C means, simulated |
| H0 with that route and a repaired message | `plain` first, then one round trip | the same as H2f |
| H3 | `extern "sched.h" define "_GNU_SOURCE"` (unbuilt) | cannot be written: the form has no value |

## What I did not run

- **No `--refresh`.** Every real number is an inference.
- **No first-group route was built**, by me or (as of the engineer's 03:07 note) by the engineer. H2f's truth rests on C-level simulations only.
- **Route H**: not run on Linux or Windows. Its census belongs to the engineer.
- **The split route**: C level only, three units, not built. The other four headers that fail on `sign-conversion` (avcodec, avformat, swscale, libfdt) are unrun.
- **P2**: not run. It is a paid run, the coordinator's.
- **Suites**: `unseen` read 0 documents in my copy because it has no `.git`, so it is unrun in effect. `records`, `special` and `fixes` were not run on the drafts.
- **Platforms**: nothing on Windows or Linux x86-64.
- **Carried, not re-run by me**: panel 076's `strerror_r` measurement, and the ffi-pragmatist's finding that a package's `-D` reaches another module's unit. That second one is a question for the sitting: it is program-wide, against G1m's *a module's*.
- **Process**: one container command wrote its stderr to `/tmp/e` inside the `--rm` container (the container's own filesystem, not the host). I avoided that afterwards.
- I read `heroes-h` and the engineer's cases from its folder and copied them into mine. Nothing in its folder was written to.
- `drafts/b203.md` (my own +35/+38 reconstruction) is superseded by `drafts/b203c.md`, the critic's file, which reads +38/+40.
