# Panel 020 — the C emitter: the subset gate, the mangler, the artifact surface

**Convened** 2026-08-04, before M5a's first line. **Trigger**: CLAUDE.md §4 —
architecture (backend) *and* the tool surface, plus one new output class.
**Status**: `RATIFIED 2026-08-12` (was `provisional — author ratification pending`;
this line had gone on saying *pending* against this file's own § Ratification for
three days — repaired 2026-08-15, and it is the class CLAUDE.md §11 is about: a
header that outlived the fact it stated).

Five judges, differentiated by input. The compiler-engineer read the real `ir/`
tree and ran clang; the llm-ergonomist read **only** `spec/heroes-spec.md` and
four label-stripped variants; the spec-warden measured five candidate spec files
with `heroes measure`; the ffi-pragmatist wrote and compiled **19 C files** and a
decoy runtime; the historian verified every claim by URL and marked six
`UNVERIFIED` rather than guessing.

Two judges arrived with vetoes. Both are lifted below, each by a change rather
than by an argument — the pattern panel 019 set (CLAUDE.md §12).

---

## The proposal, verbatim

> M5a is "scalars run": `int`/`bool`/`if`/`while`/functions/`print` → C11 → a
> native binary, from the M4 IR. Nine points, each a decision the emitter cannot
> avoid.
>
> 1. **The emitter is a printer plus two mechanical passes.**
>    `crates/heroes/src/emit/` reads `ir::Program` + `Checked` + `Source` and
>    returns one C11 translation unit as a `String` (no file I/O in the library —
>    the CLI writes it). Two passes precede printing: **declaration ordering**
>    (prototypes for every function, in source order; no `typedef`s at M5a
>    because there are no aggregates yet) and the **mangler**. No analysis, no
>    optimisation.
> 2. **The mangler.** `h_<module>_<name>`; `<module>` is the source file's stem,
>    sanitised to `[A-Za-z0-9_]` (modules land at M8a). No `_<typehash>` until
>    generics are monomorphised (M6). `extern` names pass through unmangled.
>    Heroes `function main` becomes `h_<module>_main`, and the emitter adds a
>    shim `int main(void) { h_<module>_main(); return 0; }` — so the mangler has
>    **no exception**, and the shim is where runtime init goes later.
> 3. **The subset gate.** A walk over the IR that refuses, in source order, the
>    first form the backend cannot emit yet, naming the milestone that will emit
>    it. It is **not** a `Diagnostic`: a correct program the compiler cannot yet
>    compile is not a wrong program, so it must not flow through `check --json`,
>    `--permissive` or `--apply`. New type
>    `emit::Unsupported { form, milestone, span }`, rendered as one line
>    `<file>:<line>:<col>: unsupported: <form> is not emitted yet (<milestone>)`,
>    **exit 2** (the tool could not do its job). Gate table, over IR ops and
>    types, not surface forms — it dies row by row: str/`Cast`/`Len`-on-str and
>    `f64` → M5b; `Construct`/`Field`/`Index`/`MapGet`/`Tag`/`Payload`/`Switch`
>    and every `T?` form → M5c; `FuncRef`, a function with generics,
>    `Abort::Assert`, `FnKind::Test` → M6. `Hole` is refused before the gate
>    (§4.16: no binary while the file has holes). `CopyOut` and `Arg::InOut`
>    **are** emitted at M5a — on a scalar with a path-free place §4.8's copy-out
>    is one assignment, and it is the sentence panel 000 called the place this
>    project would stall.
> 4. **The artifact surface** (panel 016's stopping rule). `heroes build <file>`
>    now produces a **binary** (`build/<hash>/<stem>`) and says on stderr what it
>    wrote. Flags: `--emit-c` (stop at the C, print it on **stdout**),
>    `--no-line` (only with `--emit-c`, for debugging the emitter), `-o <path>`
>    (where the artifact goes — the argv table's first value-taking flag). New
>    verb `heroes run <file>`: build, then execute — a different artifact class
>    (a process), and both design.md §3.1 and the ROADMAP's fixpoint invocation
>    already type it.
> 5. **`#line` policy.** `#line <n> "<source path exactly as given in argv>"`
>    emitted only when the source line *changes*; around synthetic code
>    (prologue, copy-out, the `main` shim) `#line <n> "<stem>.c"` restores to the
>    generated file. **The emitted C never mentions the output path** — the
>    restore name is derived from the source stem — so `--emit-c` is
>    byte-identical whether it goes to stdout or to `-o a.c`.
> 6. **The C shape** (CLAUDE.md §7, spike 01 as the target): all slots and
>    temporaries hoisted to the prologue; one `label:`+block per basic block with
>    an explicit `goto bb0` entry; `__builtin_{add,sub,mul}_overflow` and a
>    negation check → `hero_panic_overflow()`; `/` and `%` guarded (zero →
>    `hero_panic`, `INT64_MIN / -1` → overflow); `hero_unreachable()` for
>    `Term::Unreachable`; `int64_t`/`bool`.
> 7. **Compile flags.** `-std=c11 -Wall -Werror=return-type
>    -Werror=uninitialized -fno-strict-aliasing`, `-O0` for `build`, `-O2` for
>    `run`. Two additions the ROADMAP carried in: **adopt `-Werror=format`** now
>    (M7's variadics are the risk it guards); **measure
>    `-Wconditional-uninitialized`** on the corpus at M5a and decide at M5b with
>    a number rather than an opinion.
> 8. **Where the runtime comes from.** `runtime/runtime.c` compiled once to
>    `build/runtime.o` and linked. Located by, in order: `$HEROES_RUNTIME`,
>    `runtime/` under the current directory, `runtime/` under an ancestor of the
>    executable. Failure to find it is exit 2 with a message naming all three.
>    (The alternative — `include_str!` the runtime into the binary — was
>    considered and rejected: it needs a mechanism Heroes does not have at M8b,
>    while file lookup needs only the file I/O M8a schedules anyway.)
> 9. **`run/` goldens and determinism.** `<name>.hero` + `<name>.expected` = the
>    program's stdout, compiled and executed by the harness at **both `-O0` and
>    `-O2`**, both required to produce the expected bytes (the queue's "one
>    corpus, N configurations", at the only cost that is nearly free). The
>    double-emit determinism diff runs over every `run/` and `ir/` case and stays
>    green forever. `print` per panel 006: monomorphic segment printers, **no
>    separator, one trailing newline** — so `hero_print_int` loses the `\n` it
>    owns today and `hero_print_end` gains it.

---

## The verdict table

| judge | verdict | section | cost / measured delta | prediction | condition to lift |
|---|---|---|---|---|---|
| compiler-engineer | **OBJECT → 4 conditions** | §1.7 (the backend column of the core), §4.8, CLAUDE.md §7/§9/§11 | emitter ≈ **850 non-test lines in ≥6 files** (24% of `ir/`); whole milestone ≈ **1350**. Point 6 = 320 of them and "the milestone's real cost and its real risk". Point 4 forces a `cli.rs` split (309 > §11's ~300) | at M5a close `emit/` reports **≥700 non-test lines in ≥5 files** and `cli.rs` exceeds 340 or has been split | the record answers, in writing: the `@` ABI · the unit-temporary rule · clang-failure classification so a missing `return` exits 1 not 2 · the gate's `Builtin` rows and the `Test` **skip** |
| llm-ergonomist | **OBJECT → 4 conditions, + 3 locality VETOes** | locality (§1.3) | zero language change; 10 hesitation points, H10 ("exit 0 ⇒ success") named the worst | exit 0 on a holed build yields a false "build succeeded" report in **≥30%** of agent runs and **≥90%** where the loop is `build && <artifact>` with a stale artifact present; exit 1 yields 0%. Variant A produces an environment probe first in ≥60% and a false "toolchain broken" claim in ≥20% | unsupported-yet is **exit 1** with code `unsupported` (not `unsupported_type`) · the note names **capability, not schedule** · holed builds **exit 1** ending in `no binary: N hole(s)` · success prints the artifact path |
| spec-warden | **OBJECT** (no budget veto) | §1.6 primary; §1.0 on `--no-line`; §1.2 on point 7 | **measured**, five candidates: A current **2139** · B1 panel 006 verbatim **2178 (+39)** · B2 **2181** · **B3 recommended 2155 (+16)** · C (+ map order) **2161**. Headroom to the hard 3000: **845** after B3 | land B3 and the spec measures exactly **2155 / 2096 / binding 2155, spread 59**; ship without it and **≥1 of the first 10 `run/*.expected`** is authored with a separator or terminator the runtime does not produce | B3 lands in the same commit as the newline relocation, before/after quoted · `--no-line` **dropped** and CLAUDE.md §7 + design.md:469 amended per panel 016's watch list · point 4's `run` warrant restated on design.md §3.5 + CLAUDE.md §6, and the false "the fixpoint invocation already types it" struck |
| ffi-pragmatist | **VETO** → lifted by one table row | §4.19 (clang verifies the signature against the real header), §1.11 | 19 C files compiled; **`-Wconditional-uninitialized` and `-Werror=format` measured: 0 additional diagnostics** over `runtime.c`, all four spikes and both hand-emitted files | at M7, with the header present, **≥4 of the 6** SQLite ladder declarations are rejected by clang on a wrong `int` — measured 2/2 rejected with the header, 0/3 without | add **`FnKind::Extern` / `Callee::Extern` → M7** to the gate table |
| historian | **OBJECT** (advisory, no veto) | the record | 30 verified rows, 6 marked `UNVERIFIED` and named as inadmissible | if `emit::Unsupported` ships outside `Diagnostic`, by M5c it grows a second implementation of caret rendering, `--json` or multi-diagnostic ordering — or is converted; and **no unsupported-form test carries a `#~` annotation before that conversion** | make it a `Diagnostic` **kind**, not a parallel type |

---

## Where the judges disagreed

**Exit code for a correct program the backend cannot emit — 1 or 2.** The
historian defends 2 from the project's own contract (grep's "an error occurred")
and from GCC: `sorry()` has existed since **GCC 2.5.8** (≈1994), prints `sorry,
unimplemented: `, and exits with `FATAL_EXIT_CODE` — which is `EXIT_FAILURE`,
i.e. **1**, the same code as an error. So GCC's own precedent, read to the end,
lands on 1. The ergonomist made the same call from the other side and measured
the cost of 2: its **first action** under variant A was `heroes --version &&
heroes doctor`, its second `grep -rn "M5b" .`, its third a message to the user
saying *"The Heroes toolchain looks broken"* — a sentence it reported verbatim as
false and generated without hesitating. **Resolution: 1.** The historian's
position is not overruled so much as completed by its own source.

**`Unsupported` as a type or as a diagnostic kind.** The engineer costed the
parallel type at ~10 duplicated lines and called the objection "not altitude".
The historian found the record answers twice, identically: GCC's `sorry` is a
*kind* in `kinds.def` beside `error`/`warning`/`ice`, `sorry_at` takes a
`location_t` exactly as `error_at` does; GHC's `Sorry` carries this proposal's
own reasoning in a code comment — *"The user tickled something that's known not
to work yet, but we're not counting it as a bug"* — and still travels the
ordinary failure path; Zig prints `error: TODO (LLVM): …` at `file:line:col`.
Nobody built the parallel channel. The decisive consequence is local to this
repo: CLAUDE.md §9's `#~ <code>` invariant keys off diagnostic **codes**, so a
type outside the diagnostic system is a test class outside the invariant.
**Resolution: a kind.**

**Milestone identifiers in shipped text.** The ergonomist vetoed `(M5b)` — the
token resolves only in `docs/ROADMAP.md`, a file the reader does not have, and
§4.17's standard is "everything needed without opening another file". The warden
reached the same verdict independently from §1.6 (the spec never names a
milestone). The proposal's own author wrote it. **Resolution: the message names
the capability; the milestone stays in the gate table in source, where the author
reads it.**

**`--no-line`.** The proposal births it; panel 016's watch list had already
refused it and scheduled §7's amendment *at M5a, where the flag would otherwise
be born*. The warden noticed the proposal does not mention that item.
**Resolution: dropped**, and the two documents that mandate it are amended in the
same commit. The replacement is three lines in the emitter's test helper — which
is the author's activity, not the tool's capability.

---

## Resolution — provisional, author ratification pending

Adopted as the most conservative reading: every veto lifted by the change its
holder named, every objection either taken or recorded with its refusal.

### R1 · The gate is a diagnostic kind, and it exits 1

`Diagnostic` gains a `kind: Kind` field — `Error` or `Unsupported`. An
`Unsupported` diagnostic carries **no fixes**, is never dropped by
`--permissive`, is never applied by `--apply`, renders through §4.17's existing
machinery with the word `unsupported` where `error` goes, and appears in
`--json` with its kind. Exit **1**. The `--help` gloss on 1 becomes *"the input
has diagnostics; no artifact was produced"*.

Message shape, fixed here: `unsupported[unsupported]: floating-point arithmetic
is not emitted yet`, with two notes — what the backend does emit today, and **"no
change to this file will fix this"**. No milestone, no version identifier.

**All** unsupported forms are reported, sorted by span (the engineer's omission
8, from `cli.rs`'s own rule that a message carries the list).

### R2 · The gate table, completed

Over IR ops and types, never surface forms. Five rows were missing:

| refused | until | why it was missed |
|---|---|---|
| `Callee::Builtin(_)` other than `print` | per row (23 builtins in `resolve/builtins.rs`) | `xs.len()` lowers to `call builtin len` while `for` lowers to `Op::Len` — gating the op alone leaves an **undefined symbol at link time**, the exact class the gate exists to prevent |
| `FnKind::Extern` · `Callee::Extern` | **M7** | **the ffi veto.** `extern function labs(x: int) -> int` lowers today, no header attachment exists, so the emitter must invent a prototype — and an invented prototype is unverifiable by *any* clang flag. Measured: `abs(-2147483649)` returned `2147483647` at exit 0 with one non-fatal warning; `sqlite3_open` accepted in **total silence** under `-Weverything -pedantic`; with the real header both are `error: conflicting types` |
| a function with no `main` reaching a *binary* | — | `no_entry_point`, exit 1. `--dump-ir` and `--emit-c` still work: a library TU emits no shim |
| `FnKind::Test` | — | **skipped, not refused.** `ir/mod.rs` already says "ordinary builds ignore it"; refusing it would make `04-loops.hero` unbuildable at M5a *and* after M5b/M5c |
| `Op::Hole` | — | refused before the gate, and the build **exits 1** ending in `no binary: N hole(s) in <file>` — the ergonomist's veto on exit 0 |

Everything else stands as proposed: str/`Cast`/`Len`-on-str and `f64` → M5b;
`Construct`/`Field`/`Index`/`MapGet`/`Tag`/`Payload`/`Switch` and every `T?` form
→ M5c; `FuncRef`, generics, `Abort::Assert` → M6. `CopyOut`/`Arg::InOut` are
emitted at M5a.

### R3 · The `@` parameter ABI, written down

The engineer's omission 2: `Op::CopyOut` writes the **caller's** place and the
callee cannot reach it in C. The convention, fixed here and inherited by
M5b/M5c/M6: an `@` parameter is **a pointer parameter**; the prologue copies in
(`int64_t l = *p_l;`), and each `CopyOut` emits `*p_l = l;`. Two `@` parameters
copy out in parameter order. `f(@x, @x)` cannot arise — panel 010 made two `@`
arguments sharing a root a compile error, and this is the second thing that rule
bought.

### R4 · The unit rule, and nine more things clang decided

Each measured, not argued:

1. **A unit-typed temporary is never declared and never named.** `ValueId(0)` is
   `()` in every function and `Function::values` is dense, so the natural hoist
   emits `void t0;` → `error: variable has incomplete type 'void'`. `Return(Some(v))`
   with `v: ()` is `return;`.
2. **A block is emitted only if some edge targets it** (`Block::preds`, already
   there). `adversarial-diverging-arms` has a predecessor-less join block today;
   emitting its label costs `-Wunused-label` **on a correct program**. Omitting
   unreachable blocks entirely keeps that warning alive as a real check that the
   emitter's `goto` edges match the IR's `preds`.
3. **`Const::Int` is `INT64_C(n)`**, and `INT64_MIN` for `i64::MIN`: measured,
   `-9223372036854775808` warns `-Wimplicitly-unsigned-literal`, and a plain
   decimal above `INT32_MAX` is `long` only on LP64.
4. **`%` is guarded like `/`.** `INT64_MIN % -1` is UB too: measured returning 0
   on arm64 at `-O0`, SIGFPE on x86. UBSan confirms all three cases. Unguarded,
   arm64 does **not** trap — it returns a wrong answer at exit 0. The spec says
   overflow and division by zero abort and is **silent on `INT64_MIN % -1`**;
   Heroes aborts it as overflow, and the silence is recorded rather than read as
   permission.
5. **`hero_panic` flushes stdout before aborting.** Measured on Darwin: 6 bytes
   of buffered output survived `abort()` through a pipe; glibc ≥ 2.27 does not
   flush. Without the flush a `run/` golden for a program that prints and then
   aborts is platform-dependent — which contradicts point 9's own determinism
   claim.
6. **`__builtin_*_overflow` decides overflow against the *destination* type**,
   not the operands: `sub_overflow(false, true)` into `bool` flags overflow,
   into `int64_t` does not. Heroes never does `bool` arithmetic, so M5a is safe —
   but Part 7's `c_int` moves the threshold from 2⁶³ to 2³¹ with no visible
   change at the call site, so the invariant goes in the emitter's module doc
   now. `bool` and `int64_t` temporaries share one hoisting scheme unchanged.
7. **`#line` is emitted when the instruction's line differs from the *current
   effective line*** — not from the previous instruction's span. `#line N`
   anchors the *next* line and C auto-increments, so a Heroes line that lowers to
   K C lines drifts by K−1. Measured on the frozen target: `tools/spike/01-first.c`
   advertises `00-first.hero:2` and clang reports `:6`, a blank line, with **C
   text shown under the `.hero` filename**. The restore directive therefore
   carries the printer's **own output line count** — which makes a
   newline-counting writer a structural requirement of point 1, not a detail. The
   source path is escaped into a C string literal; `#line 0` is invalid C11.
8. **A missing `return` must not reach clang.** `verify.rs` permits
   `Term::Return(None)` in a non-unit function and names `-Werror=return-type` as
   the net; modern clang reports it as **`-Wreturn-mismatch`**, an error by
   default, so a *program* error would surface as exit 2 naming `h_mod_f`.
   Measured live: `function f(x: int) -> int` with an `if` and no `else` **checks
   clean today**. So lowering gains its second diagnostic class, `missing_return`
   (exit 1), and **any** clang failure is classified: exit 2, `internal error:
   the generated C did not compile`, with clang's output — because it means the
   compiler is wrong.
9. **`main`'s contract**: no parameters, unit result, and its absence is
   `no_entry_point` rather than `ld: undefined symbol`.
10. **The verifier gains block-locality of temporaries** (~20 lines + a test that
    makes it fire, CLAUDE.md §9). `verify.rs` checks a value is produced
    *somewhere*, not that its definition dominates its use; with a hoisted
    prologue a violation is a read of an uninitialised C local, which
    `-Werror=uninitialized` **does** catch — as `variable 't0' is used
    uninitialized`, `#line`-mapped into the author's `.hero`. A compiler bug
    wearing a user diagnostic. Cheapest insurance in the milestone, and it lands
    before M6 starts copying blocks.

### R5 · The mangler stays `h_<module>_<name>`, made injective

The scheme is CLAUDE.md §7's and does not change. Two facts, both from judges:
it is **not injective** with `_` in both components (module `print` + name
`inst_value_name` = module `print_inst` + name `value_name`, and those are this
compiler's own file names), and it shares a namespace with spike 04's
`h_<T>_<op>` (loud: `error: conflicting types for 'h_Expr_copy'`). Resolution:
the **module component is sanitised to `[A-Za-z0-9]`** — every other character is
dropped — so the first `_` after `h_` ends the module and the parse is
unambiguous. Residual and recorded: `print_inst.hero` and `printinst.hero` are
one module, loudly. Nim's precedent (a reserved `__` separator, unambiguous
because the language forbids it in identifiers) is unavailable here: Heroes does
not forbid `__` in a name.

The **shim stands**, and it is not an invention: Vala emits a separate `int
main(int argc, char **argv)` that calls the user's entry point
(`ccode.add_return (main_call)`); Nim's generated `main` is a shim and
`NimMain()` is where init goes, with `--noMain` to suppress it. Zig's C backend
once emitted `void main(void)`, which clang rejects — the concrete failure the
shim prevents. File-stem-as-module is what Rust still does (`mod util;` ↔
`util.rs`), so it is not a stopgap that later gets paid for.

### R6 · The surface: three flags, one verb, `--no-line` dropped

| capability | shape | warrant |
|---|---|---|
| `heroes build <file>` → binary | the verb it always promised | the fixpoint: `cargo run -- build selfhost/heroes.hero -o A` |
| `--emit-c` | **flag** — same input, different answer | the fixpoint's `--emit-c -o B.c` + the determinism diff |
| `-o <path>` | **flag**, the argv table's first value-taking one | the fixpoint is unwritable without it; `> a.c` composes for the C but the binary leg cannot be redirected |
| `heroes run <file>` | **subcommand** — a different artifact class (a process) | design.md §3.5 names it; CLAUDE.md §6 sanctions it by name (`nim r` → `heroes run`); the `run/` harness needs the -O2-execute leg. **Not** the fixpoint invocation — the proposal's claim to the contrary is struck from the record |
| `--no-line` | **nothing** | fails the stopping rule; panel 016 already refused it. Replaced by three lines in the emitter's test helper |

`--dump-ir` and `--emit-c` are *stops*: they print their artifact and do not
invoke clang, so all 21 `ir/` goldens keep their exact bytes and their exit 0.
Bare `build` and `run` are the only invocations that need `main`. `run` forwards
the program's own exit status — the program is the artifact — while the tool's
own failures stay 2.

Success says `wrote build/<hash>/<stem>` on stderr. The ergonomist wrote
`heroes build first.hero && ./build/9f2a1c/first` correctly first try against
that line, and `./first` — guaranteed wrong — against silence. The count
("compiled 1 function") is dropped: it carried an unverifiable fact that made the
reader stop and re-count.

### R7 · Both `-W` flags land now, with the number that decides them

`-std=c11 -Wall -Werror=return-type -Werror=uninitialized -Werror=format
-Wconditional-uninitialized -fno-strict-aliasing`. The warden's objection is
taken: "measure at M5a, decide at M5b" re-litigates a measurement already taken —
panel 019's ffi-pragmatist found a slot stored only inside a loop and read after,
clang silent, segfault — and ships two milestones with the hole open, owned by
nobody. This panel's ffi-pragmatist then measured both flags at **0 additional
diagnostics** over `runtime.c`, all four spikes and both hand-emitted files. The
falsifiable form, adopted: **zero fires across the whole `run/`+`ir/` corpus at
M5a close → `-Wconditional-uninitialized` is promoted to `-Werror=`; one fire is
a lowering bug and gets a case named after it** (CLAUDE.md §9's `fixedbugs`
rule).

### R8 · The runtime: found, stamped, and keyed

The ordered search stands (`$HEROES_RUNTIME` · `runtime/` under the cwd ·
`runtime/` under an ancestor of the executable), with three additions the
ffi-pragmatist's decoy earned:

- **`HERO_RUNTIME_ABI` in the header, `_Static_assert`ed in every generated TU.**
  Measured: a directory named `runtime/` in the cwd silently replaced the
  contract — the same C compiled with **zero warnings under `-Weverything`** and
  printed `0x1e` where `30` was expected. With the stamp: `error: use of
  undeclared identifier 'HERO_RUNTIME_ABI'`. `runtime` is one of the commonest
  directory names there is.
- **The `build/<hash>/` key covers**: the source bytes, the compiler's version,
  the optimisation level, and the contents of `runtime.c` + `heroes_runtime.h`.
  Measured: with `hero_print_int` changed and the `.o` not recompiled, the relink
  **succeeded silently** and printed the old bytes. Without the level in the key,
  point 9's two configurations are one configuration run twice, measuring
  nothing.
- **One `.o` is shared across `-O0`, `-O2`, `-flto` and ASan** — measured, all
  four link and run. No per-level copy.
- An installed binary with no `runtime/` above it is the fourth case and reports
  exit 2 naming all three search steps. `$HEROES_RUNTIME` is an input channel
  outside the argv table, so it is listed in `--help` and checked by `doctor`.

### R9 · `print`, the spec sentence, and the spikes

`print` per panel 006, now stated where a model reads it. **B3 lands in
`spec/heroes-spec.md`** — two lines, **+16 measured** (2139 → **2155** binding,
headroom 845):

> `None of these names may be redeclared.` **`print` writes its values with no
> separator and exactly one trailing newline.**

Panel 006 priced its own amendment at "+15 est"; the verbatim text measures
**+39**. The word heuristic was ~2× low again — panel 011's failure mode, on the
record for the third time. The map-insertion-order sentence (**+6 measured**)
waits for **M5c**, the first milestone where map iteration is observable; the
deferral is priced and triggered, not assumed.

`hero_print_end` is the right shape, proven rather than argued: the alternative
(the emitter owns the newline) needs `putchar`, and `error: call to undeclared
function 'putchar'` means `#include <stdio.h>` in every generated TU — a header
collision surface on every FFI program, against CLAUDE.md §7. Segment printers
also interleave correctly with C's own `printf`/`puts` at `-O0` and `-O2` through
a pipe, because they share `stdout`'s `FILE*`. `print()` with no arguments emits
`\n`.

**All four spikes are updated in the same commit and gain checked `.expected`
files.** Measured: under the new runtime spike 04's three distinguishable lines
collapse to `10-42`, and nothing catches it — the spikes' expected output lives
in a C comment. `tools/spike/01-first.c` also has the `#line` drift of R4.7.

### R10 · `run/` goldens, in two configurations

Every case at `-O0` and `-O2`, both required to produce the expected bytes.
GHC's testsuite runs **over 30 "ways"**, so two is conservative; the class is
real (GCC PR 115492: "fails at `-O2` but passes at `-O0`/`-O1`") and for a C
*emitter* an `-O0`/`-O2` divergence is usually **our** UB rather than clang's
bug. One caveat on the record: `-fno-strict-aliasing` deliberately switches off
the largest single source of `-O2` divergence, so the signal will come from
signed overflow and uninitialised reads instead — which is exactly what R4.4 and
R7 are about. The determinism diff's ancestor is named: GCC's bootstrap compares
**stage2 against stage3 objects**, and its manual says a mismatch "normally
indicates that the stage2 compiler has compiled GCC incorrectly" — the M8c
fixpoint, thirty years earlier. (`rustc -Zverify-llvm-ir` was checked and is
**not** a determinism test; it is dropped from the precedent list.)

### What a veto would have compelled

The ffi-pragmatist's, unlifted: no `extern` in any emitted program until M7 —
which is what the added row achieves, so the veto cost one table row and bought a
measured guarantee. The ergonomist's three, unlifted: exit 2 on unsupported (an
agent loop that reinstalls the toolchain and tells the user it is broken), `(M5b)`
in stderr (a repo-wide grep for an identifier that resolves in a file the reader
does not have), and exit 0 on a holed build (a `build && ./artifact` loop that
executes **yesterday's binary** and attributes its output to today's source).

---

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | `emit/` closes at **≥700 non-test lines in ≥5 files**; `cli.rs` exceeds 340 lines or is split | M5a close |
| llm-ergonomist | exit 0 on a holed build → false "build succeeded" in **≥30%** of agent runs, **≥90%** with a stale artifact; exit 1 → 0%. Variant A → environment probe first in ≥60%, false "toolchain broken" in ≥20% | metric 2's first paced run (harness) |
| llm-ergonomist | variant B *as written* (`error[unsupported_type]`, caret on the `str`) still produces ≥1 futile source edit in ≥40% of runs; the adopted wording drops it to ≤10% | same |
| spec-warden | after B3 the spec measures exactly **2155 cl100k / 2096 legacy / binding 2155, spread 59, headroom 845** | M5a step where the spec lands |
| spec-warden | had the sentence not landed, **≥1 of the first 10 `run/*.expected`** would carry a separator or terminator the runtime does not produce | M5a close (counterfactual — score by whether any golden needed correcting) |
| ffi-pragmatist | at M7, with the header present, **≥4 of 6** SQLite ladder declarations are rejected by clang on a wrong `int`; without `c_int` landing *with* the header, 5 of 6 need a shim | M7 |
| historian | `emit::Unsupported` as a parallel type grows a second span/JSON/ordering implementation by M5c, or is converted to a kind — and no unsupported-form test carries a `#~` annotation before that conversion | M5c (moot: converted at birth) |
| compiler-engineer | `-Wconditional-uninitialized` fires **zero** times on accepted programs across the corpus | M5a close (promotes the flag to `-Werror=`) |

## Watch list

- **The spec is silent on `INT64_MIN % -1`** (R4.4). Heroes aborts it as
  overflow. The silence is a gap, not permission, and it wants a sentence
  whenever §4.14's abort list is next touched.
- **`print`'s separator is 2–1 against us in the record**: Pascal's `WriteLn`
  inserts nothing (ISO 7185: `write(f,e1,…,en)` ≡ `write(f,e1); write(f,e2,…)`)
  and is the cited precedent; Go's `println` **does** add spaces and writes to
  **stderr**, and Go's own docs call `print`/`println` "not guaranteed to stay";
  Python 3 defaults `sep=' '`. The trailing newline is unanimous. Worth a
  Go-style hedge in the spec by v1 — panel 006's historian said the same.
- **Cython's `#line` default is off**, and its reason (readable generated C) is
  the same reason `--no-line` was proposed. Heroes defaults them **on** — a
  deliberate departure, recorded here so it is not rediscovered as an accident.
- **bison disabled its own end-sync** ("lots of useless git diff … this location
  is useless for the regular user"). Point 5's restore points at the *generated*
  file rather than at a skeleton, and its byte-identical rule is the mitigation
  bison lacked — but the failure mode is real, and if the restores ever become
  diff noise this is the row that predicted it.
- **`#line` restores to the emitter's own output file appear to be original**:
  the concept exists (bison's `b4_sync_start`/`b4_sync_end`) but the historian
  found no instance of the output-file variant, in any of Nim, Cython, Vala,
  Chicken or cfront. Flagged for the DESIGN-LOG rather than claimed as
  precedent.
- **The compiler has no line budget.** `crates/heroes/src` is already **15,145**
  non-test lines — ~4× Pascal-P4 — so "does it fit one person" cannot be answered
  globally any more; the only live ceiling is §11's per-file one. Recorded by the
  engineer, unanswered by design.md.
- **Six historian rows are `UNVERIFIED`** and are inadmissible: Nim/Vala/Cython/
  Chicken/cfront/TinyGo behaviour on an unemittable form; GHC's exit status for
  `Sorry`; whether Nim's mangled names include the module; what file-stem
  mangling cost anyone when modules arrived; whether Zig's behaviour tests really
  run in all four modes; whether any C emitter restores `#line` to its own
  output.
- **`heroes measure` still vendors two of three tokenisers** (queued at panel
  019). B3's +16 is 14 tokens clear of nothing, but the next verdict inside 10
  tokens of a ceiling needs o200k vendored first.

---

## DESIGN-LOG

Appended with this session. The amendments it compels — spec B3, design.md §3.1
(`--no-line` deleted, the `#line` effective-line rule, the runtime's location and
stamp, the `@` pointer ABI), CLAUDE.md §7 (the flag set, `INT64_C`, labels only
where an edge targets them) — land in their own commit citing this file.

## Ratification — 2026-08-12, by author instruction

**RATIFIED.** The author's instruction was a blanket one — *"ratifica anche tutto
quello che c'è da ratificare"* — given after reading the session summary, not a
clause-by-clause review of this file. It is recorded that way on purpose: this
project's own rule is that a record must not say more than what happened.

What it settles: the provisional resolution above **stands as the decision**, and
work no longer proceeds on it as a default. Every resolution here had been
load-bearing since the day it landed, so this changes the record's status rather
than the compiler's behaviour.

What it does **not** settle: anything this file keys to a measurement that has not
been taken. Those stay open on their own terms, listed in `docs/debrief/QUEUE.md`,
and a blanket yes cannot make a number arrive.
