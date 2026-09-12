# 033 — M-c-callbacks: the address goes out, and nothing comes back

Closed 2026-09-05, the day it opened. Tag `m-c-callbacks`, chain row 34.

## Goal

A Heroes function reaches a C callback parameter. Before this milestone a
function value was refused at the FFI by two different doors: declared `ptr`, the
`extern` declaration was accepted and the **call site** failed `type_mismatch`;
declared at its real type, the **declaration** failed `ffi_type`. So `atexit`,
`qsort`, `signal`, `sqlite3_exec`, `pthread_create` and every raylib callback
were unbindable, and design.md §1.11's founding constraint — everything comes
from C — met a class of C function it could not reach at all.

The milestone is not part of M-isolated-threads, which is where it was born.
`docs/panel/111` split it out because three seats measured that the permission is
bought entirely by §1.11 and CLAUDE.md §12's FFI-completeness instruction and
needs no thread at all; bundled, it would have let the weaker half ride the
stronger's Principle 0 ticket. The author placed it at row 34, ahead of the
milestone it came from.

## What surprised

**The backend had been able to do this the whole time, and the deferral rested on
a worry nobody had run.** `function worker(arg: ptr) -> ptr` emits
`void * h_main_worker(void * h0_arg)`, which **is** what `void *(*)(void *)`
points at, so the address goes to C with no cast and no trampoline, under
`-Wall -Werror`. Panel 030 R6 had deferred the whole question on the possibility
that a C11 backend could not express it. The permission itself was a **+32/−1**
diff in one file, because the parameter-versus-result distinction already existed
in the checker.

**Where a callback may stand is a question about position, and it can never be a
question about vocabulary.** C spells a function pointer everywhere it spells a
type, so *can a header declare this* cannot be the rule. Once position is the
rule, it refuses **three** shapes rather than the two panel 111 R4 named — an
`extern`'s result, an `@` out-parameter, and an `extern constant`, the last named
by no seat and reaching the same danger through the one door that does not go
through a signature. One argument covers all three and it is the permission's own
mirror: an address passed **out** is one this compiler emitted, so C calls a body
this compiler type-checked; an address handed **back** is a body nothing here has
seen.

**One unspellable parameter withheld the header check for every other parameter
of the same `extern`, in silence.** `emit/extern_probe.hero`'s `c_type_of` had no
`.function_ty` arm, and a type it cannot spell drops the probe for the whole
group. Binding a callback would therefore have switched clang's verification off
for the parameters beside it — `DrawText`'s defect with a new cause. Two more
gaps behind it had been latent from the day the probe existed, invisible while
every boundary type spelled itself without a table.

**A sitting was convened on 2.6% of its own subject.** The brief handed
`docs/panel/112` the claim that `const` is why callbacks do not bind. The
ffi-pragmatist seat refused the sample and measured the real `sqlite3.h`: **106
callback signatures, 153 unspellable parameter occurrences**, and `const` is the
**fourth** cause by frequency at 2.6%, while a pointer to a struct the header
declares is **66.7%**. 17 of 106 bind today; a `const` spelling would make it 18,
a pointer-to-header-record spelling would make it 82. The convener's own count
was wrong in his favour as well — `const` buys 2 of the 8 named callbacks, not 3,
because `nftw` is blocked by a `struct FTW *` that is not `const` at all.

**Whether a null callback is tolerated is a property of the C library on that
machine.** Not of the type, not even of the C function. The same program,
`atexit` handed `nullptr`, does three different things: segfault at exit 139 on
macOS, a glibc assertion on Linux, and **exit 0** on Windows, where the CRT
checks. No compiler on either side of the boundary can carry that, which is why a
compile-time refusal was never available for defect 013 and why a run-time
message was.

**The witness for a null function pointer is the faulting PC, not the faulting
address.** A program that calls through a null function pointer does not touch a
bad address: it goes to **execute** at address zero. Measured over four shapes, a
wild store to 0, a wild read from 0 and a call through a garbage pointer all give
a nonzero PC; exactly one shape gives `pc == 0`. `si_addr` is the witness panel
104 had already rejected once, in the same handler, for the same reason.

**On Windows the first colon in a clang location belongs to the drive letter.**
`ffi_site.hero`'s `location()` read `<file>:<line>:<col>:` by taking the first
three colons, so an absolute path made the file name come out as `C` and the
parse refused. Since that function is the gate every `ffi_*` classifier passes
through, width, sign, class, arity, format string, writable parameter and
callback type all fell through to exit 2 together — on the one platform where an
author is most likely to type an absolute path. CI never saw it because CI
compiles relative paths.

## What broke and why

**Four defects in the probe, every one found by running rather than by reading.**
The missing `.function_ty` arm above; a per-module TU that never joined an
`extern`'s parameter types to its typedef reach, giving `unknown type name
'h_0fn_294870dd'` at exit 2 on a correct binding; the probe emitted in the
prelude **before** the typedef section, against `emit.hero`'s own written
invariant *types before anything that can mention one*; and, in the reader rather
than the writer, `emit/ffi.hero` rebuilding the probe line with a name table that
had no function typedefs, so a callback the header refutes stopped the compiler
with `assert failed: !found.is_err()` and no line at all.

**Defect 014 — every FFI diagnostic blind on Windows whenever the path was
absolute** (the colon above). Repaired by reading the location from the right,
cutting at the `": "` that ends the column and opens the severity, with four
tests over the exact strings and then re-measured on the box.

**Defect 013 — a call through a null function pointer died at 139 with an empty
stderr.** Filed blocked by panel 112 with two priced routes, both of which were
refusals: `-Wpedantic` fires on `examples/sqlite/main.hero`, which passes a
deliberate NULL callback and is **correct**, and letting `nullptr` inhabit a
function type is a spec change that leaves the defect standing. The author asked
for the repair anyway, and the route neither option named was in the same file
three lines above, written by panel 104 for a problem of the same family. The
question moved from *how is it prevented* to *how does the program say what
happened*. Cost: zero spec tokens, zero compiler lines, `HERO_RUNTIME_ABI`
unmoved.

**Three shipped defects, all the coordinator's, all one shape — a claim written
without being run.** A diagnostic citing a Part 8 wart that did not exist (Part 8
had 18 warts and none was this one, counted); a needle matching `const char *`,
which `cstr` spells exactly, so a callback whose only mistake was `i64` for `int`
was told *this one cannot be written at all today* while the one-character repair
builds and runs; and defect 013 above.

**And a rule this milestone broke, written down because a record that hides it is
worth less.** CLAUDE.md § Commands says the three platforms are measured
**before** the commit. Steps 0 and 1 were committed with one measured, because
the Linux container was filed as *scheduled, it has to be brought up* without
checking whether Docker was already running. It was. That is §1's named shape — a
failed search written as an impossibility — and it cost two commits that happened
to be right.

## Predictions, scored

Eight predictions named this close. Every number below was run in the session
that writes it.

**One row was nearly scored on a failed search, which is CLAUDE.md §1's named
shape and is recorded here because the record is worth more with it.** The
ffi-pragmatist's row was first written *unmeasured — `raylib.h` is not installed
on this Mac*, on the strength of a `#include <raylib.h>` that clang could not
find. raylib **is** installed: `/opt/homebrew/include/raylib.h`, version 6.0, and
`pkg-config --cflags raylib` names its directory. The include failed because
homebrew's path is not on clang's default search list, which is precisely what
`extern "raylib.h" package "raylib"` exists for — the spec's own answer, four
lines from the sentence this milestone added. Measured with it,
`SetTraceLogCallback` is `error[ffi_callback_type]` at exit 1, and the cause is
one the sitting's five-way breakdown does not name for this callback: `va_list`
is **`char *`** on Darwin arm64, a non-const `char *`, and Heroes has no spelling
for one — `cstr` is `const char *` and `ptr` is `void *`. All three spellings
were tried and all three are refused.

| origin | prediction | scored |
|---|---|---|
| panel 111, compiler-engineer | if the permission lands as +32/−1 with **no new reader** under `selfhost/emit/`, the probe returns 0 for that extern and a `qsort` golden exits **2**; the sound landing is **≥150 code lines across ≥4 files**; falsified if the reader lands under 95 code lines and `qsort` still exits 1 | **RIGHT about the size, and its antecedent never held.** The landing is **171 code lines across 12 files** under `selfhost/`, of which **121 across 8 files** are under `selfhost/emit/` — including a new reader, `emit/callback_guard.hero` at 73 code lines. So ≥150/≥4 holds and the falsifier's first conjunct is false. The consequent is measured false anyway: `qsort` is **exit 1**, `error[ffi_callback_type]`, on the author's own line |
| panel 111, ffi-pragmatist | all five of `sqlite3_exec`, `sqlite3_busy_handler`, `signal`, `qsort`, raylib `SetTraceLogCallback` are `error: incompatible function pointer types`, and exactly three compile; second, a `qsort` binding exits **2**, not 1 | **RIGHT on the count, wrong on the membership, wrong on the mechanism** — measured over **all eight**, one probe each. *Exactly three compile*: **RIGHT**, and they are `atexit`, `pthread_create` and `sqlite3_busy_handler`. But `sqlite3_busy_handler` is on this seat's list of failures and **binds at exit 0**, and the fifth failure is `nftw`, which the seat did not name — one swap. *All five are `error: incompatible function pointer types`*: **FALSE**. Every one is this compiler's own message at **exit 1** on the author's line — `ffi_callback_type` for `qsort`, `sqlite3_exec`, `nftw` and `SetTraceLogCallback`, `ffi_type` for `signal`'s result position — and not one probe reaches clang's own text at exit 2, which is also the second half falsified |
| panel 112, compiler-engineer | if the wart lands, `heroes measure spec/heroes-spec.md` reads **3830** and `variant Ty` still has **17 cases** — either moving means a language change went in under a wart's name | **RIGHT on both, counted today**: 3830 with headroom 266, and `variant Ty` in `selfhost/check/table.hero` has exactly **17** cases |
| panel 112, compiler-engineer | its own veto's falsifier: if a `const`/`c_int` kind ever lands, the commit touches **≥40 files** and `heroes measure` reads **≥3870**; under 20 files and ≤3840 the veto was overpriced | **NOT TRIGGERED**, and it cannot be scored at a milestone at all. R1 refused (a) and (b), so no such commit exists; its trigger is a commit rather than a date. It stands in `docs/panel/112` for whoever lands one |
| panel 112, ffi-pragmatist | with (a) landed alone and flag 13 unchanged, a `qsort` comparator passing either parameter to another `extern` taking `ptr` cannot reach exit 0 | **NOT TRIGGERED**: (a) was refused. Flag 13 is `-Werror=incompatible-pointer-types-discards-qualifiers` and is unchanged — `selfhost/cli/flags.hero::flags()` holds **14** flags, counted |
| panel 112, ffi-pragmatist | `sqlite3_exec` needs no shim under (b) and still needs one under (a): §4.19 ladder step 3 is untouched by `const` | **The half that can be scored is RIGHT.** Neither (a) nor (b) landed, so the two conditionals are not triggered; the claim under them was run today. `sqlite3_exec`'s diagnostic reads the header back as `int (*)(void *, int, char **, char **)`, in which **no `const` appears at all**, so the blocker is the pointer-to-pointer and a `const` spelling would move nothing |
| panel 112, spec-warden | with the SEGV repair landed: `heroes measure` reads exactly **3830**; every `extern` declaring a function-pointer parameter as `ptr` is **exit 1** on the author's line; **zero** programs in `examples/` or `tests/golden/` need editing; `atexit(f: ptr)` and `qsort(… compar: ptr)` are exit 1, not 139 | **Two clauses right, two false — and the repair that landed is not the one the seat priced.** 3830 exactly: **RIGHT**. Zero programs edited: **RIGHT**, measured — **0** existing `.hero` files under `examples/` or `tests/golden/` were modified across the whole milestone, and the 7 modified files there are `.expected` snapshots. *Every `extern` declaring it `ptr` is exit 1*: **FALSE**, and deliberately so — `atexit(f: ptr)` builds at exit 0, because `examples/sqlite/main.hero` declares `callback: ptr` and passing `nullptr` there is a correct program. *Exit 1, not 139*: **FALSE as written and met in substance** — it is **exit 134** with `panic: a null function pointer was called`, because defect 013 was repaired by naming the fault at run time rather than by refusing the declaration. Its third clause was already scored false at the sitting |
| panel 112, historian | the `ptr`-against-`char ***` acceptance is **not** on a compiler clock: the same reproducer is exit 0 under clang 22.1 **and** gcc 14, while `ptr` against `const void *` and `const char *` fails on both | **RIGHT on the claim it was making**, measured today over four cases and **three** compilers — Apple clang 21.0.0, Debian clang 22.1.8 in the project's own Linux image, and gcc 14.2.0 in a Debian trixie container. A `void *` argument to a `char ***` parameter is **exit 0 and clean on all three**; a `void *` comparator where the header wants `const void *` is **exit 1 on all three**, and `int (*)(void *)` for `int (*)(const char *)` likewise. **The second half needs one qualifier, and the sitting's own subject supplies it**: a `void *` to a **direct** `const void *` parameter is exit 0 on all three too, so the const half fails **inside a function pointer type and nowhere else** — which is exactly the seam panel 112 was convened on |

**A measured note the historian's row turned up and nobody had written down: gcc
cannot read this project's flag list at all.** Two of the fourteen flags in
`selfhost/cli/flags.hero::flags()` are clang spellings that do not exist in gcc —
`-Wconditional-uninitialized` and `-Wshorten-64-to-32` — and a third,
`-Wincompatible-pointer-types-discards-qualifiers`, is `-Wdiscarded-qualifiers`
there. Handed the list verbatim, gcc 14 stops at `cc1: error` before it reads a
line of C. The three were substituted and named to score the row above. The
project requires clang by design (CLAUDE.md §7, and `heroes doctor` says so), so
this is not a defect; it is the price of that decision, measured, for whichever
sitting next argues from what another C compiler would do.

## What landed, and what carried forward

**Six steps, all in one day.** Step 0 landed the permission and the thread guard
in one commit, because panel 111 R9's ratified words say the two cannot be
separated — the guard protects exactly the functions the permission admits, and
the permission is the door. Step 1 applied the two `/decide` answers in the
session that asked them. Step 2 is a commit with no code at all: the record of
what Windows and Linux were asked and answered for the guard. Step 3 put the
permission into the spec. Step 4 is panel 112 and defect 014. Step 5 repaired
defect 013.

**The permission, end to end.** A Heroes function value crosses the FFI as a
parameter and in no other position. `atexit(bye)` binds, compiles and C calls it
back at exit 0; `pthread_create` binds, spawns and joins with no cast and no
trampoline. Three of the eight named real callbacks bind today, measured rather
than counted from the brief.

**The guard, on all three platforms.** `hero_thread_guard("<module>.<function>")`
at the entry of every function whose address the program takes, in a program that
declares an `extern` with a function-typed parameter, and **zero** guards in a
program that hands C no function. A `_Thread_local bool` is zero on a thread a
foreign library made on Windows Server 2025 and on glibc as well as here — which
C11 7.5 promising is not the same as a machine doing it. On panel 111's own first
witness, a `str` touched from 16 threads C made, **ten ASan runs of ten** stop
with the function's name at exit 134, against a use-after-free or a double free
in 9 of 10 before. The corruption is not repaired; it is unreachable.

**The spec moved once, to exactly the predicted bound.** One paragraph replaced
`Neither may be an absolute path.`: *A callback is a **parameter**, never a
result; its parameters follow the same rule and `()` is `void`:
`atexit(f: (function() -> ()))`.* +39 gross across four measured wordings against
a **−8** removal verified by running it, +31 net, landing at **3830** with
headroom 266. Panel 111's spec-warden had registered ≤ 3830 before the sentence
was written, so the row is met with nothing to spare and any further callback
text falsifies it.

**What carried forward**, each with its home:

- **`qsort` still does not bind, and now the document says so on purpose.**
  design.md Part 8 wart 19 names const-qualified pointee parameters as the known
  hole, at zero spec tokens, with the ffi seat's ordering rather than the brief's
  and a fifth cause nobody had named — an out-parameter **inside** a callback is
  a parse error, 26 occurrences in one header. The `SCHEDULED.md` item that asked
  the question is closed by the sitting.
- **The refcount half of design.md's own v1 invariant is still open** and belongs
  to M-isolated-threads: `_Atomic` appears nowhere in `runtime/`, and the
  boundary being narrow and never inlined is the half that is done.
- **Sixteen shared mutable buffers** in `runtime/parts/` that the plan did not
  name, one of them on the closure list — M-isolated-threads.
- **`pthread_t` has two spellings and no portable one**: an opaque pointer here,
  an `unsigned long` on glibc. The compiler says which, at exit 1, with a `guess`
  fix on each. That is M-core-packages' platform-typedef question arriving in a
  second place, and M-isolated-threads meets it first.
- **The loopback HTTP server with one connection per thread**, Part 7 item 13's
  first witness, is writable for the first time and stopped by the guard until
  isolation lands — which is the correct state rather than a regression.
