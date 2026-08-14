# 048 — What the spec says about C: two clauses, and the one nobody proposed

**Status** `ratified — 2026-08-14, author decision`
**Convened** 2026-08-14, by the author · **Lane** full panel, five judges

## Why it was convened

Panel 047 moved the generated C to `-std=gnu11` and **deliberately did not ask
whether the spec should say so**. It ran three judges and wrote down what that
gave up: no llm-ergonomist, because there was no spec diff to show a blind
reader; no spec-warden, because the change cost zero tokens. This sitting is that
second half, with both of them in it.

## The proposal, verbatim

Two candidate clauses for spec § FFI, measured with `heroes measure` before any
judge saw them — 2974 today, ceiling 4096:

> **X.** The headers are read as a POSIX C compiler presents them, so `M_PI`,
> `strdup` and `fileno` are there, not only ISO C's names. **(+38)**

> **Y.** `link` names the library the symbols live in; leave it out only when
> they are in the C runtime itself. `math.h` is separate on some systems and not
> others, so a program that uses it writes `link "m"` everywhere. **(+54)**

Both: **+92**.

## The verdict table

| judge | X | Y | measured | prediction |
|---|---|---|---|---|
| **llm-ergonomist** | **object** | **approve** | X changed **zero characters** of three programs; Y changed **3 of its 6 `extern` lines** | with Y, non-`"m"` link clauses on maths tasks drop from ≥30% to ≈0; with X, delta ≤3pp on symbol-named tasks |
| **spec-warden** | object at +38, accept at **+9** | object at +54, accept at **+19** | verified 2974/3012/3028/3066; removal search over six candidates: **nothing comes out of the document** | with today's spec, 3 of 3 blind readers bind a POSIX symbol unprompted — if so X buys nothing |
| **compiler-engineer** | **approve** | **object** | X costs **0 lines**, already discharged. Y as written is **unenforceable**; the enforceable reading costs ~12 lines and a 13-site migration | if Y lands as written, at M-selfhost-probe `grep` still finds no linkage diagnostic and ≥10 groups still name no library |
| **ffi-pragmatist** | **object** | **object** | 30 probes × 4 libcs: **four POSIX names glibc hides** that Darwin, musl and FreeBSD show. `-lrt` **errors on Darwin**; `-liconv` is required there and **does not exist on glibc** | a `strptime` binding exits 0 on Darwin and raises `ffi_unknown_name` on Linux; ladder rung 5 (raylib) needs link tokens `link` cannot spell |
| **historian** | approve the **content**, object to the wording | **object** on placement | 1 of 10 specs states a C dialect; **0 of 10** state the `libm` rule — but **POSIX.1-2024 does** | a golden with `link "m"` is green on both under gnu11 and red on Linux only under c11 |

## What the five found, and where they disagree

**The blind reader is the sitting's centre, and its two results point opposite
ways.** Given only the spec and three tasks, it wrote `link "m"` **taking `"m"`
from outside the document** — and said so unprompted: the spec's own pattern
(`sqlite3.h` → `link "sqlite3"`) yields `link "math"`, which is wrong on every
machine. It also wrote `link "c"` twice, believing a group without `link` to be
ungrammatical, because the prose says a group *"names its header and its
library"*. **Y would have changed three of its six `extern` lines.** X changed
nothing at all — and the reader explained why: the section's own example list
already says *"sockets"*, and sockets are POSIX, not ISO C. X states what the
document already implies.

**The disagreement about Y is real and is not smoothed here.** The ergonomist
approves it because it wrote the wrong thing without it. The engineer objects
because *"in the C runtime itself"* is a property of the platform, so §12 makes
the compiler owe a rule it cannot check — CLAUDE.md §11's named failure, one
sitting after design.md §4.19 had the same shape removed from it. Both are right
about different halves: **the reader's problem is real and the clause is the
wrong instrument.**

**Three judges independently found the same thing: the spec is already false.**
Line 192 says a group *"names its header and its library"*, and 13 of 20 corpus
groups name no library — including `crates/heroes/src/library/source.hero:102`,
`extern "hero_os.h"`, which is **the closure list's only `extern` group**. Read
literally, the spec makes the port's own library source illegal and the compiler
buggy for accepting it. Repairing that is §1.0 compiler-need and costs
approximately nothing.

**And Y as worded is factually wrong about that very group.** `hero_os.h`'s
symbols are in neither the C runtime nor a `link` library: `toolchain.rs`
compiles `runtime.c` to an object and links it. A reader applying Y concludes
`extern "hero_os.h"` needs a `link`.

**The warden's most useful output is not a verdict but a price.** It re-measured
the same two clauses written minimally: X at **+9**, Y at **+19**, together
**+28** against +92. *"The wording choice is worth 1.1 amendments of headroom —
more than either decision."*

**The removal search returned nothing, and corrected the record while failing.**
Six candidates priced by deletion, all refused — four are compiler-need, one was
refused by `docs/measurements/007`'s own blind reader last night. And `2588`,
which `DECIDE.md` records as the one lapsed row that is *not* compiler-need, **is
compiler-need**: the closure list's only `extern` group uses `constant`. So of
312 unfunded tokens, **zero are recoverable**, where the record believes 28 are.

**The warden also declined to use its veto on a dead number.** Its brief grants a
veto above 3000; design.md §1.6 raised the ceiling to 4096 in August. It said so
and stood down — *"that is the same failure mode §1.6 documents twice"*.

## The option nobody proposed, and it dissolves Y

**The warden and the engineer arrived at it independently, and the historian
found the precedent.** Do not put the rule in the spec. Fix the compiler.

Today, a missing `link` produces this — reproduced on Darwin by the engineer, and
the path is `crates/heroes-cli/src/commands/compile.rs:255`:

    internal error: compiling the generated C failed:
    Undefined symbols for architecture arm64:
      "_sqlite3_libversion_number", referenced from: _h_nolink_main
    the generated C is at build/0962cc8ee452a884/nolink.c
    EXIT=2

**Exit 2 means *the tool could not run*** (CLAUDE.md §10). The compiler is
blaming itself for a mistake in the author's `.hero` file. `emit/ffi.rs:8-11`
already contains the written verdict on exactly this shape, about a different
case: *"Left as an internal error it would print C the author never wrote, name a
file under `build/<hash>/`, and blame the compiler for a mistake in a `.hero`
file — three of the four things §4.17 exists to prevent."* The argument is on
record and was not applied here.

Priced by the engineer, drafted and counted: **~85 lines including its test**,
one file, one pass, **zero spec tokens**, exit 2 → exit 1. It does not make X
unnecessary — X is about name *visibility*, already diagnosed as
`ffi_unknown_name`. It makes **Y** unnecessary, and for the reason that decides
it: *"Y helps only the reader who read the spec and remembered; the diagnostic
helps the reader who did not, which under §1.2 is where the whole cost lives."*

**Zig is the precedent, twice** (historian, sourced): `isLibCLibName()` makes
`"m"` resolve from libc on gnu, musl, Darwin and FreeBSD — *the platform question
removed from the user with zero specification text* — and issue #15223 proposes
the missing-symbol diagnostic while recording the constraint that decides
Heroes' version: **"This enforcement can only occur when Zig serves as the
linker."** Heroes does not serve as the linker; clang does. So the strong form is
unavailable and the available form is a reader of clang's stderr, which is what
`emit/ffi.rs` already is.

## Two premises corrected, one of them mine

**The `libm` fold did not happen.** The convening prompt suggested glibc 2.34
folded libm into libc, and asked whether Y therefore instructs an author to write
something inert. Red Hat's own announcement, sourced: *"We have also been unable
to complete the transition of the `libm`, `libmvec`, and `libresolv` components
in time for the glibc 2.34 release."* Debian trixie ships glibc 2.41 with
`libm.so.6` today. **`-lm` is still load-bearing on glibc and on FreeBSD**, a
POSIX-mandated no-op on musl (which ships an *empty* `libm.a` for exactly that
reason) and on macOS. Y is true; the objection to it is placement, not truth.

**And clause Y is already in a specification — POSIX's.** `c17`, verbatim:
*"`-l m` … shall make available all interfaces referenced in `<math.h>` … An
implementation may search this library in the absence of this option."* That is
Y, written by The Open Group, and it also proves `link "m"` is safe everywhere.

**A third premise, measured here rather than argued.** Everyone in this sitting
including the synthesis had been explaining the Darwin/Linux split as *"libm is
inside libSystem on Darwin"*. Measured: on Darwin arm64, `sqrt` produces **zero
undefined symbols at `-O0` and at `-O2`** — clang lowers it to a hardware
instruction, so the call never reaches the linker at all. The missing `link "m"`
was invisible here for the project's whole life for a **second, independent**
reason nobody had named. Whether the same lowering happens on x86-64 Linux could
not be measured from this machine, and the historian's caveat — that the failure
may be optimisation-level dependent — stays open for exactly that reason.

## The judge that reported last, and confirmed the resolution

The **ffi-pragmatist** returned after this synthesis was first written. Its
verdict is **object on both clauses, no veto** — *"neither clause is true as
worded"* — and it recommends, unprompted and independently, exactly the pair
below: *"neither clause; one diagnostic."*

**X is false on glibc, and not by accident.** 30 probes across four libcs at the
ratified `gnu11`: `strptime`, `wcswidth`, `wcwidth` and `swab` are declared on
Darwin, musl and FreeBSD and **undeclared on glibc** — because glibc's gnu11
default is `_DEFAULT_SOURCE`, which does not set `__USE_XOPEN`, while musl's
`features.h` sets `_XOPEN_SOURCE 700` under the same flag. A permanent divergence
between two libcs, not a version accident. And `arc4random` is gated on
`__GLIBC_MINOR__ >= 36`, so the visible set moves with the libc **version** too.
X's own three examples are true 4/4; its *generalisation* is what a reader binds
against, and that is false.

**Y's rule breaks in three places it compiled.** `-lrt` is an **error** on Darwin
(`ld: library 'rt' not found`) and correct on glibc. `-liconv` is **required** on
Darwin and the library **does not exist** on glibc, where `iconv_open` has always
been in libc — so no single `link` line binds iconv on both. And `pthread_create`
moved from libpthread into libc at glibc **2.34**, so "in the C runtime itself" is
a property of a libc *minor version*.

**It confirmed the `libm` correction from the other side**, reading glibc's own
`abilists` at six versions: `sqrt`, `pow`, `log`, `ceil` are in libm at 2.28
through 2.41, while `pthread_create`, `dlopen`, `timer_create` and `forkpty` moved
to libc at 2.34. And it measured the Darwin no-op precisely: binaries built with
and without `-lm`, at `-Wl,-no_uuid`, **differ in exactly one byte, and that byte
is the output filename**.

**It vetoed the inference option** this sitting was asked about — a header→library
table is wrong on three rows, differently per platform *and per glibc minor
version*, *"CLAUDE.md §11's expiring premise as an algorithm"*. And it objected to
a check-time `link` requirement on measured grounds: of 17 `extern` groups in the
repository, **10 carry no `link` and all 10 are correct**.

**One finding is outside this sitting's question and is the most consequential.**
`toolchain.rs` builds `format!("-l{library}")` by design, so no `.hero` can hand
clang an arbitrary flag — and macOS frameworks cannot be spelled that way at all.
**§4.19's ladder rung 5, raylib (design.md:2056), is currently unbindable on
Darwin**: it needs `-framework OpenGL -framework Cocoa -framework IOKit` there
against `-lGL -lX11 -lm -lpthread -ldl -lrt` on Linux. `link` is not a finished
feature, which is its own argument against freezing prose about it.

**And it reported an instrument failure rather than deleting it**, the second in
two sittings: its first link matrix said `-lm` was unnecessary on glibc,
contradicting the CI. Cause — `zig cc` auto-links `libm.so.6` and five others, so
*every* cross-link succeeded regardless of `-l`. It rebuilt the instrument to read
glibc's stub symbol tables directly. *"Panel 047's lesson was that compiling is
not linking; this one is that a cross-compiler that links everything cannot answer
what needs linking."*

## Resolution — provisional, author ratification pending

**Neither X nor Y lands.** What lands is the pair nobody proposed:

1. **The spec's existing false sentence is repaired.** Line 192's *"names its
   header and its library"* becomes true of the 13 groups that name no library —
   §1.0 compiler-need, because one of those groups is the closure list's own, and
   a §12 reading makes the port illegal today. Measured cost: it must not exceed
   **+10**.
2. **`ffi_missing_link`**: the linker's `undefined reference` becomes a Heroes
   diagnostic on the author's line, at **exit 1**, naming the `extern` group that
   declared the symbol and offering `link "…"` as a `guess` fix. **Zero spec
   tokens.** The `declaration()` gate already in `emit/ffi.rs` keeps a genuine
   compiler bug — an undefined symbol the emitter itself failed to define — at
   exit 2, which is a fact about the value and not a premise about the world.

**Queued rather than landed** — and with the last judge in, X's case is weaker
than when this was written, not stronger:

- **X at the warden's +9 floor wording**, and worded as a **floor** rather than an
  identity (historian: `gnu11` gives glibc *more* than POSIX and Darwin ignores
  the dialect entirely), and naming only `M_PI`, which is the single symbol the
  two-platform CI pins. The engineer's condition binds whatever lands: **X must
  state the observable, never the flag** — that is the sentence panel 047 had to
  repair at design.md §4.19:1947, and in the spec it would have no panel file to
  record why.
- **The counter-proposal the blind reader would fund first**, and which is not on
  this sitting's menu at all: the **inbound `cstr`**. The section documents one
  direction — *"`s.cstr()` passes a `str` to C"* — and says nothing about a `cstr`
  coming back, which made one of its three tasks unwritable. Verified after the
  fact: `to_str` on an inbound `cstr` **works**, and `ptr == nullptr` **works**;
  the spec's `==` list omits `ptr` and its built-in list gives `to_str` no
  signature. **The language is wider than its specification in two places at the
  FFI boundary**, and a reader following the spec exactly cannot write `getenv`.

What a veto would have compelled: nobody vetoed. The engineer declined explicitly
— *"I object on coherence, not on cost"* — and the warden declined on a stale
ceiling.

## Predictions to score

| # | judge | prediction | checkable at |
|---|---|---|---|
| 1 | llm-ergonomist | with Y, non-`"m"` link clauses on maths tasks drop from ≥30% to ≈0; **X's delta on symbol-named tasks ≤3pp**, and X measures zero by construction unless a task names a *job* whose natural symbol is POSIX | the next blind run, protocol of `docs/measurements/007` |
| 2 | spec-warden | given today's unamended spec and three POSIX-only-symbol tasks, **3 of 3 bind it through `extern` unprompted, with no hesitation** — if so X buys nothing | M-selfhost-probe |
| 3 | compiler-engineer | if the diagnostic lands: `emit/ffi.rs` ≤ 290 lines, **no other file under `crates/heroes/src/` gains a line**, and the new golden exits **1** on both platforms | the landing commit + CI |
| 4 | compiler-engineer | if Y had landed as written: at M-selfhost-probe, `grep` finds no linkage diagnostic and ≥10 groups still name no library — the clause would have bought a sentence nothing enforces | M-selfhost-probe |
| 5 | historian | a golden with `link "m"` is green on both platforms under `gnu11`, and **red on Linux only** under `c11`. If red on both, X's mechanism is uniform and X is stronger than credited | CI, by forcing the dialect once |
| 6 | synthesis | `sqrt` emits no undefined symbol on Darwin arm64 at either level — **measured, held**. Whether the same holds on x86-64 Linux is **open**, and decides whether a missing `link` is a reliable failure or an intermittent one | one CI run with a `nm` step |

## What this lane gave up

Nothing by design — this was the full panel, and it is the first sitting in this
repository to run all five briefs. All five reported — the ffi-pragmatist after the
synthesis was drafted, which is why its verdict arrives in a section of its own
rather than woven through. Nothing was lost; the only cost is that this file was
written twice, and the second writing did not change the resolution.

---

## A sentence of this file is falsified — 2026-08-14, panel 049

This file records, from its ffi-pragmatist's report: *"§4.19's ladder rung 5,
raylib (design.md:2056), is currently unbindable on Darwin."* **That is wrong**,
and it is the sentence that convened panel 049.

Measured there by three judges independently, and by the synthesis a fourth time:
raylib's homebrew **dylib** links on Darwin with `-I` and `-L` and **no framework
at all**, and one judge opened a window and drew from `.hero` source with the head
line unchanged. `heroes build` on a raylib group dies at `'raylib.h' file not
found` — the **header path**, one directory before the linker exists as a
question.

**design.md:2058 had recorded this a milestone earlier**, in ratified text: *"what
the group head lacks is search paths, not a framework keyword."* The document was
right and this file was wrong; the finding that stands is the narrower one, that
`link` cannot spell `-framework` **at all**, which matters for Apple's own APIs
(`SecRandomCopyBytes` has no `-l` spelling, measured) and not for any rung of the
ladder.

The record is appended rather than corrected in place (CLAUDE.md §14). What made
the error: a true statement about `-framework`'s absence was carried into a claim
about what blocks rung 5, without anyone compiling rung 5.

## Ratification — 2026-08-14, by author decision

**RATIFIED as it stands.** Both candidate clauses stay refused, the repaired
sentence and `ffi_missing_link` stay landed, and the queued items stay queued.

What the yes settles that the provisional default left implicit: **the sitting's
own result is that a panel convened to *add* to the spec may correctly land
nothing but a repair.** Three judges, judging two proposals, found the sentence
already there to be false — 13 of 20 groups name no library, one of them the
closure list's own — and under §12 that made the port illegal and the compiler
buggy for accepting it. A repair of a false sentence is §1.0 compiler-need and
needs no removal and no prediction; both refused clauses did, and neither could
pay. The record should read as a *precedent* for that order of business: ask
whether what is written is true before asking what to write next.

It also settles that `emit/ffi.rs`'s narrowing is **`declaration()`**, not whose
text a message is. The doc that claimed otherwise was already false when written.
