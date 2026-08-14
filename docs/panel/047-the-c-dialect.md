# 047 — The C dialect: what `-std=` buys, and what it costs on the second platform

**Status** `provisional — author ratification pending`
**Convened** 2026-08-13, by CI run 4 · **Lane** three judges (see § What this lane gave up)

## Why it was convened

M-program-corpus gave this project a CI on two platforms. It had been developed
on one machine for its whole life. The first run that ever compared them failed:

    error[ffi_unknown_name]: `math.h` declares no `M_PI`
      — clang read the header and could not find it

from `tests/golden/run/ffi-constant.hero`, which declares `extern "math.h"` with
`constant M_PI: f64`.

**Neither compiler is wrong.** `M_PI` is not in ISO C — it is POSIX/XSI — and
glibc guards it behind `__USE_MISC`, which `-std=c11` switches off by defining
`__STRICT_ANSI__`. Darwin's `math.h` does not guard it at all. So the same
`.hero` file compiles on one platform and fails on the other, which is the exact
failure §4.19 exists to prevent, arriving through the **flags** rather than
through the types.

## The proposal, verbatim

> `toolchain::FLAGS` changes `-std=c11` to `-std=gnu11`, for the runtime object
> and for every generated translation unit. What the emitter *writes* is
> unchanged and still C11; the flag changes only which declarations the
> platform's headers expose.

Alternatives put to the judges: **(b)** keep `-std=c11` and have the emitter
define `_DEFAULT_SOURCE` and/or `_POSIX_C_SOURCE 200809L` before the first
`#include`; **(c)** change nothing and accept that the FFI reaches only ISO C.

## The verdict table

| judge | verdict | section | measured cost | prediction | condition |
|---|---|---|---|---|---|
| **ffi-pragmatist** | **approve**, no veto | §1.11, §4.19 | 63/63 `run/` goldens cross-compile for `x86_64-linux-gnu` under gnu11; **62/63 under c11**, the one failure being `ffi-constant` | Linux goes green on `ffi-constant` and **no other golden's output changes on either platform**; separately, SQLite and libcurl need no flag change at all (measured 5/5 on glibc, musl, FreeBSD) | design.md §4.19:1947 repaired in the same commit |
| **compiler-engineer** | **accept-with-condition** | §3.1, §1.11, CLAUDE.md §12 | **(a) 2 lines**, 0 golden bytes, 543 tests green. **(b) 127 golden lines**, hand-edited, `UPDATE_GOLDEN` forbidden in `emit/` | at the landing commit, both CI jobs green **and** `git diff --stat tests/golden/emit/` shows 0 changed lines | the spike harness's duplicate flag list moves too; `ffi-constant.hero` gets the `fixedbugs` treatment; design.md:876 rewritten |
| **historian** (advisory) | **object to the option set** | precedent | — | `_DEFAULT_SOURCE` in the generated header passes on both; `_POSIX_C_SOURCE` fails on both | — |

## What was measured, and by whom

**The ffi-pragmatist did the thing this panel was for.** It found a real
glibc/musl/FreeBSD header tree on the machine (zig's bundled libc) and a working
cross-compiler, put a shim on `PATH`, and ran **the real `heroes` pipeline
against real glibc headers**. The Linux claims in this file are compiled, not
cited. It reproduced the CI failure and measured the fix on the actual artifact.

Its visibility table, compiled on four libcs, is the sitting's centre. Under
`-std=c11` on glibc, **hidden**: `M_PI`, `M_E`, `strdup`, `strndup`, `fileno`,
`popen`, `setenv`, `unsetenv`, `newlocale`, `uselocale`, `clock_gettime`,
`CLOCK_MONOTONIC`. Under `gnu11`, all twelve visible. That is most of what §1.11
says a program binds.

**Three independent measurements of the same load-bearing fact**, and they agree:
`-std=c11` and `-std=gnu11` differ by **exactly one predefined macro,
`__STRICT_ANSI__`**. The compiler-engineer measured it, the ffi-pragmatist
measured it, and the synthesis measured it a third time (480 macros against 479,
one line of `diff`).

**The ABI question, answered by compiling rather than by reasoning**: LLVM IR
byte-identical for the real runtime and the real generated unit at `-O0` and
`-O2`; 16 glibc struct layouts unchanged, with three types going *hidden →
visible* and none re-laid-out; all ten constructs the emitter writes accepted
identically; §4.19's own `_Generic` classifier returning identical answers on ten
real header tokens. **No ABI objection survived measurement, so the judge with
the veto did not use it.**

## Disagreements, unsmoothed

**The historian objected to the option set, and it was right that a fourth option
exists.** Three of three sourceable C-emitting compilers — Nim, Vala, CHICKEN —
pass **no `-std=` at all**, and Nim solves the visibility problem the way this
project could: in its own generated header, `lib/nimbase.h` defines `_GNU_SOURCE`.
That is the dominant precedent and it was not on the menu.

**The synthesis does not take it, and the reason is this project's own
acceptance criterion.** Passing no `-std=` means taking clang's default dialect,
which is `gnu17` on current clang, was `gnu11` before Clang 11, and moves again
with the compiler. A project whose v1 acceptance is a **byte-identical fixpoint
on generated C** cannot let the dialect float — and it decided exactly this
question about its *own* build four hours before this sitting, pinning
`rust-toolchain.toml` after clippy 1.97 rejected forty lines that clippy 1.90
accepts. Naming the dialect is the same discipline applied to C. The precedent is
real and it comes from projects that do not have this acceptance criterion.

**Option (b) was refuted from three directions, one of them by its own
proposer's data.**

- As tabled, it said "`_DEFAULT_SOURCE` **and/or** `_POSIX_C_SOURCE 200809L`".
  The historian sourced why `_POSIX_C_SOURCE` cannot work on glibc — it sets
  `__USE_XOPEN2K8`, and `M_PI` needs `__USE_MISC`. The ffi-pragmatist then
  compiled the other half: on Darwin, `-D_POSIX_C_SOURCE=200809L` **hides**
  `arc4random`, `strlcpy`, `asprintf`, `getprogname` and `MAP_ANON` (5/5), and
  hides `M_PI` on FreeBSD. It is the one spelling in this whole space that
  measurably makes bindings *harder*.
- `_DEFAULT_SOURCE` alone does work, and is **exactly equivalent to gnu11** on
  glibc and musl across all 20 cases. It buys nothing gnu11 does not, and costs
  127 hand-edited golden lines in a directory where `UPDATE_GOLDEN` is
  forbidden.
- It needs **the same rule written in two files** — the emitter reaches the
  generated unit, and `runtime/runtime.c` is a separate translation unit — where
  `toolchain::FLAGS` is already the one place both are configured. CLAUDE.md
  §11's rule points at (a).

**One objection was raised and withdrawn by the judge that raised it.** The
ffi-pragmatist expected (b)'s ordering hazard — a `#define` emitted after a
system header — to fail silently. It tried to make it silent and could not:
moving the define one line down produced `error: use of undeclared identifier
'M_PI'`. It withdrew the objection. That is recorded because a withdrawn
objection is evidence too.

**The "gnu11 loosens the language" worry came to one keyword.** Of eight GNU-only
constructs, `-std=c11` with this project's flag set already accepts **seven**.
Only bare `typeof` differs — and the emitter writes `__typeof__`, legal in both.
`emit/mangle.rs` carries no keyword list and needs none: it defends structurally
with `h_`/`t<n>`/`bb<n>` prefixes, and only `extern` names pass through
unmangled, which are real C identifiers out of real headers.

## What `-std=c11` was actually buying

Nothing it was believed to buy. The emitter has **never** written portable ISO C:
`__builtin_*_overflow` is mandated by CLAUDE.md §7 itself, and every generated
unit now carries `__builtin_types_compatible_p(__typeof__(c), void)`. A real
program's C emits 31 such references. The flag never made the output portable —
it only narrowed the *headers*, on one of the two platforms, against the language
whose founding constraint is that everything comes from C.

**And both judges found, independently, that CLAUDE.md §7's own "Compile flags:"
list does not contain `-std=` at all.** The flag that decides header visibility
on half the machines in the world was missing from the contract that lists the
flags. That is repaired with this sitting whatever the author decides.

## Resolution — provisional, author ratification pending

**Adopted: (a) `-std=gnu11`**, with all four conditions, in one commit.

It is the most conservative resolution available: it is the only option measured
end-to-end on the platform that fails, it moves zero golden bytes, it keeps the
dialect *named* rather than inherited from whatever clang ships, and it writes
the rule in the one place both translation units already read.

What a veto would have compelled: nobody vetoed. The ffi-pragmatist holds the
veto on ABI breakage, went looking for breakage with a cross-compiler, and
reported *"I came to break this and could not."*

Conditions, all four in the landing commit:

1. **design.md §4.19:1947** — *"under `-std=c11` `stdbool.h` spells `true` as
   `#define true 1`"* — rests on a flag this change removes. The **fact**
   survives (measured: `true` classifies as `int`, sizeof 4, identically on
   Darwin and glibc); the sentence rests it on the world instead of on the value,
   which is CLAUDE.md §11's class exactly.
2. **design.md:876** — the 128-bit refusal's first leg is *"`__int128` is a clang
   extension and §7 says C11"*. That leg dies. CLAUDE.md §12 requires a refusal to
   name its falsifier, so the clause is rewritten; the row survives on its other
   two legs.
3. **The spike harness's duplicate flag list** (`tests/golden/golden.rs`) moves
   with it, or the configuration a test proves is not the one the product ships.
4. **`tests/golden/run/ffi-constant.hero` gets the `fixedbugs` treatment** (§9),
   and must say that its symptom is a **platform**, not a program — or the next
   reader deletes the `M_PI` binding as gratuitous.

Two things explicitly not done, both on the ffi-pragmatist's instruction:
`runtime/runtime.c`'s `_POSIX_C_SOURCE` block **stays** (its
`__has_include(<xlocale.h>)` half answers a different question and the block is
inert on Darwin), and `_POSIX_C_SOURCE` is **added nowhere**.

## What this lane gave up

Three judges ran: ffi-pragmatist, compiler-engineer, historian. **No spec-warden**
— the change costs zero spec tokens, so it had nothing to weigh. **No
llm-ergonomist** — there is no spec diff to put in front of a blind reader, and
the spec's FFI section is unchanged by every option on the table.

What that gives up is real and is written down rather than assumed: nobody asked
whether a reader of the spec can *tell* that `extern "math.h"` reaches POSIX and
not only ISO C. The spec says *"Anything beyond this document — sockets, maths,
JSON, databases — comes from C libraries"*, and after this change that sentence is
true on both platforms; before it, it was true on one. If the author wants the
spec to *say* which C, that is a different sitting with a different lane.

## Predictions to score

| # | judge | prediction | checkable at |
|---|---|---|---|
| 1 | ffi-pragmatist | The Linux job goes green on `ffi-constant.hero` and **no other golden's output changes on either platform** | CI run 5 — the next push |
| 2 | ffi-pragmatist | §4.19's ladder rungs 3 and 4 (SQLite, libcurl) need no flag change and no shim: their generated units compile clean under both dialects on glibc, musl and FreeBSD | CI run 5, and any future rung |
| 3 | compiler-engineer | `git diff --stat tests/golden/emit/` over the landing commit shows **0 changed lines** across all five `.expected` files | the landing commit itself |
| 4 | compiler-engineer | would change to `object` on any *new* red on Linux under gnu11 that c11 did not produce — a `-Wall` difference, a warning promoted by `-Werror=conditional-uninitialized`, or a `#line`-independent golden shift | CI run 5 |
| 5 | historian | `_DEFAULT_SOURCE` in the generated header passes on both platforms; `_POSIX_C_SOURCE` fails on both | **already scored, split**: the first half is confirmed by the ffi-pragmatist's glibc compile; the second half is confirmed on glibc and **falsified on Darwin**, where `_POSIX_C_SOURCE` leaves `M_PI` visible and instead hides five *other* identifiers. The synthesis measured the Darwin half twice, the first time wrongly — see below |

**A measurement error is on the record rather than quietly corrected.** The
synthesis first reported that `_DEFAULT_SOURCE` hides `M_PI` on Darwin,
contradicting the historian. It does not. The result was an artifact of shell
quoting — the whole flag string reached clang as one argument to `-std=`, so
every multi-flag case failed for a reason that had nothing to do with macros. It
was caught by re-running with a real output path, before it reached this file as
a finding. The rule it illustrates is the project's own: measurement beats
opinion, and a measurement is only worth what its instrument is.

## A defect found in passing, not an objection

The ffi-pragmatist reports `cargo test --test surface`'s
`build_says_where_it_put_the_binary` failing **2 of 9 runs** and passing alone
every time, with no FFI header involved. The synthesis saw the same failure once
during M-program-corpus. It looks like the `build/<hash>/` side of the cache race
that `toolchain.rs` already documents and fixes for `runtime-*.o`. It is queued
rather than fixed here: it is not this flag, and a panel is not where a race gets
diagnosed.

---

## Predictions scored — 2026-08-14, CI runs 5 and 6

**Both platforms green.** The first fully green run in this project's history.

| # | judge | outcome |
|---|---|---|
| 1 | ffi-pragmatist | **falsified in its first half, held in its second.** The flag alone did *not* turn the Linux job green on `ffi-constant.hero`: with `M_PI` resolved, the case failed one step later at the **linker** — `undefined reference to 'sqrt'`, because `libm` is inside `libSystem` on Darwin and separate on Linux, and three `extern "math.h"` groups declared no `link "m"`. The flag was necessary and not sufficient. The second half held exactly: **no golden's output changed on either platform**; what changed was one source line in three cases |
| 2 | ffi-pragmatist | **held.** `examples/sqlite/` and `examples/curl/` compiled and ran on Linux with no flag change and no shim — the corpus harness's seven properties are green on both jobs. The breakage was never the ladder; it was `math.h`'s XSI constants, and then `math.h`'s library |
| 3 | compiler-engineer | **held**, scored at the landing commit: `git diff --stat tests/golden/emit/` empty, zero golden bytes moved |
| 4 | compiler-engineer | **not triggered.** The condition was *any new red on Linux under gnu11 that c11 did not produce*. The link error is not one: under `c11` the same case failed **earlier**, at compile, so the missing `-lm` was always there and had never been reached. gnu11 did not create it — it uncovered it |
| 5 | historian | scored above, split |

**What the sitting missed, and it is worth naming.** Three judges measured the
dialect exhaustively — LLVM IR, struct layouts, macro diffs, 63 cross-compiled
goldens — and not one of them asked whether the programs would **link**. The
ffi-pragmatist compiled 63 of 63 for `x86_64-linux-gnu`; compiling is not
linking, and the one question nobody put is the one that cost the extra round
trip. A panel that measures the compiler's output can still miss the platform's
runtime, and the instrument that caught it was the CI rather than any judge.
