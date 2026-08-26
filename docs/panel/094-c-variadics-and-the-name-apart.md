# Panel 094 — C variadics, and a name apart from the symbol

**Convened** 2026-08-26, full five judges. **Author instruction**: `/decide`
answers `1c` (grow the language until `...` can be written) and `2a` (a way to
spell the C name apart from the Heroes name), taken as one sitting.

**Status:** **RATIFIED 2026-08-26** (author instruction *"fai la cosa migliore e piu robusta"*).

## The proposal, verbatim

1. **`...`** — a C variadic may be declared as variadic:
   `extern "stdio.h" function printf(format: cstr, ...) -> i32`
2. **A C name apart from the Heroes name**, so one C symbol can carry more than
   one binding:
   `function printf_i(format: cstr, value: i32) -> i32 = "printf"`

## Corrections to the convener's brief, recorded first (panel 089's rule)

**Three, and two of them are the named failure shapes in CLAUDE.md §1.** The
brief was written by the coordinator and carried into five briefs; every judge
who could falsify a claim did.

1. **"One C symbol carries one arity, and a library whose calls differ only in
   arity is bindable once" — FALSE.** The spec-warden built two modules binding
   `printf` at `i64` and at `cstr`; the ffi-pragmatist built the same for
   `curl_easy_setopt` at two value types with real `curl/curl.h`; the coordinator
   reproduced it a third time — `n = 42` / `s = forty-two`, exit 0, no language
   change. What was measured is that `= "printf"` does not **parse**, and the
   *capability* was inferred from the *syntax*. CLAUDE.md §1: **a failed search
   written as an impossibility.** The whole stated case for half 2 rests on this
   and it does not hold.
2. **"One `_Static_assert` from the author's declaration separates a variadic
   bound at fixed arity from an ordinary binding, so the refusal costs one
   line" — FALSE on real bindings.** The compiler-engineer measured **4 false
   fires of 5** (`sqlite3_open`, `sqlite3_close`, `sqlite3_column_int`,
   `strlen`; only `abs` clean); the coordinator re-measured **3 of 3**. The cause
   is structural: §4.19 *mandates* that the Heroes spelling differ from the
   header's (`ptr` is `void *`, `i64` is `long long`), and a whole-type
   comparison cannot isolate variadicity from that difference. The coordinator's
   original measurement used the header's **exact** signatures, which no real
   binding ever writes.
3. **"No file uses `args_checked` and the instrument was never built"** — true
   when written at 14:05, false by 15:39, when commit `90fa47b` built it. The
   spec-warden caught it and was right to: the brief went out stale by hours.

## Verdicts

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| **compiler-engineer** | **veto** `...` · adopt-with-condition rename | §1.7, Part 5; §4.19 | `...` **250–400 lines / 11 modules**, three of them checker+lowering+backend — `check_walk.hero:1060-1149` indexes `params[position]` five times and a variadic argument has none. Rename **60–80 lines / 7 modules**, zero new IR, zero new C shape: sugar by Part 5's own test | at M-package-layout close, a landed `...` forces either a new `-Wformat-security` pragma in a blessed emission or the removal of `-Werror=format` from `cli_flags.hero:32` | lifts on (a) a compiled C mechanism that checks a variadic's fixed prefix without re-declaring and without tripping `-Wformat-security`, **or** (b) a program `...` admits that N rename clauses do not |
| **ffi-pragmatist** | **veto** `...` · approve-with-condition rename | §4.19 and its rung 4 | re-earned the 052 veto on one machine: re-declared prefix gives `a=333 b=8393946720 c=6137472480`, `intact=0` on arm64 and `intact=1` on x86-64, **zero warnings both legs** | under the rename with `emit_ffi_lookup` re-keyed on the link name, both arities of POSIX `open` compile in **one module, one group, no shim**; under `...` exactly one assertion is emitted and the three-argument form cannot be checked at all | lifts on a generated unit for a `...` binding of `curl_easy_setopt` whose return assertion **compiles against the real header** — could not construct it |
| **llm-ergonomist** | **veto** `...` · adopt-with-condition rename | locality | wrote the `...` program in one pass with no friction and **no diagnostic** — *"that is the problem"* | over a fresh corpus under `...`, explicit width conversions after the format string fall **below 20%**, against ~100% where a width is declared | lifts if `...` accepts **only a string literal on the same line** and the compiler reads it |
| **spec-warden** | **veto** `...` · **object** rename | §1.6, §1.2 | measured, `./heroes measure`: baseline **3592**, noise floor +1. `...` cheapest **+20**, cheapest **true** wording **+62**. Rename **+18** (`= "cname"`) to **+26**; spelled with the existing word `tag`, **+23**. Both honest: **+78**, leaving 366 free | if `...` lands under +40, the ledger buys the missing half within two amendments for ≥ +30 more — the V1a→V1c gap is **+32** | half 2 flips the day someone shows a binding the two-module route refuses; if it lands it lands spelled `tag`, not `=` |
| **historian** (advisory) | **object** `...` · approve rename | precedent | Go/cgo refuses variadics — **open since 2010-07-29, 16 years**. Swift refuses the import. GHC's `ccall` refuses and `CApiFFI` was added to fix it. D added `pragma(mangle)` in **2.063, 2013**, Ada added `C_Variadic_n` in **Ada 2022** — both years after their FFI | disassembled on arm64, the emitted call stores the argument to the **stack** before `bl _printf`, not `x1` — if it uses `x1` or prints `0`, the objection collapses | flips on any project that broke on a fixed-arity binding **while emitting C through the real header** — every case found synthesised the call |

## Where the seats disagreed, unsmoothed

**On half 2 the panel splits 3–1–1.** Engineer, ergonomist and pragmatist say
adopt-with-condition; the historian approves on precedent; **the spec-warden
objects**, and its ground is the strongest fact in the sitting — the capability
is already there. The three who approve are pricing *ergonomics* (three lines in
one file against eight in two); the warden is pricing *tokens against
capability* and finds capability zero. Nothing reconciles these: they are
different questions, and the author is choosing which one the +23 buys.

**On the one thing the sitting could have built, two judges contradict each
other outright.** The warden calls the variadicity `_Static_assert` *"the part
of this sitting worth building"*, at 0 spec tokens. The engineer measured it
false-firing 4 times in 5. **The engineer is right**, re-measured by the
coordinator at 3 of 3 — and the warden's error is the coordinator's own,
inherited from the brief.

## What actually kills `...`, and it is not what convened the sitting

Three independent grounds, in the order they were found:

**The ffi-pragmatist's, and it is decisive.** `curl_easy_setopt` is a
**function-like macro nailed at three arguments** (`typecheck-gcc.h:43`). A `...`
binding can name only the fixed prefix, so the generated unit's return assertion
is `_Static_assert(HERO_RET_INT(curl_easy_setopt(0, (unsigned)0)), …)` and clang
answers `error: too few arguments provided to function-like macro invocation`,
exit 1. **§4.19's own acceptance rung 4 becomes unbindable.** The seat notes,
against its own interest, that its standing panel-052 veto does **not** trip
here: a probe over the fixed prefix compiles fine and is not a re-declaration.
*"Honesty outranks a satisfying story."*

**The ergonomist's.** After `...` there is no context, so `spec:66` defaults a
computed integer to `i64` and `%d` reads 32 bits. The proposal's own two example
lines are C undefined behaviour, and the seat produced a third **by obeying the
spec correctly**.

**The historian's, which reframes the whole question.** Every project that broke
on variadics — Terra #508, CPython #92892, cranelift #1451, frida-gum #439, Ada's
`Convention => C` — **synthesised the call itself**. Heroes routes through the
header, which is exactly what GHC added `CApiFFI` for after `ccall` proved
unsound, and what OCaml's ctypes tells its users to switch to. **Heroes already
is `capi`.** So `...` buys ergonomics, not correctness, and imports the one hole
`capi` does not have: unchecked default argument promotion, which is Rust #21812
printing `0.000000`.

## Two live defects found on the way, owed regardless of the verdict

Both reproduced by the coordinator after the seats reported.

1. **A correct binding refused, with a message that misstates the header.**
   `function printf(format: cstr, value: f32) -> i32` is
   `error[ffi_parameter_type]: value of printf is declared narrower than the
   header's double`, exit 1. The header says `...`, not `double`; C's default
   argument promotion makes `f32` correct, and the C equivalent prints
   `0.500000`. A working program the compiler will not compile.
2. **A format function bound at arity 1 is exit 2, the compiler blaming
   itself.** `function printf(format: cstr) -> i32` gives `internal error:
   compiling the generated C failed` from `-Wformat-security` firing on the
   probe. CLAUDE.md §7 says a failure the author's own `extern` caused is exit 1
   on the `.hero` line.

## Resolution — provisional, author ratification pending

Most conservative composition of five seats:

- **R1 · `...` does not land.** Four vetoes and one objection, on three
  independent grounds, of which the pragmatist's is dispositive: it makes
  §4.19's own rung 4 unbindable, measured against the real header.
- **R2 · The rename clause does not land either, and for a different reason than
  it was refused.** Its stated case — that one C symbol carries one arity — is
  false: two modules do it today. What remains is an ergonomic saving of five
  lines and one file, against **+23 spec tokens** at the cheapest honest
  wording. Principle 0: the compiler does not need it (the compiler binds five C
  functions, none variadic, none needing an alias) and no measured Part 11 effect
  is offered. It waits.
- **R3 · If the rename ever lands it is spelled `tag`**, not `= "cname"`: the
  document already has one word for *the C spelling of this name*, `=` already
  means *binds a value*, and +23 against +18 is nothing against a second notation
  for one idea.
- **R4 · The two-module route is documented rather than left to be
  rediscovered.** It was rediscovered three times in one sitting, twice by
  judges and once by the coordinator, after the brief asserted it impossible.
- **R5 · The two live defects get `fixedbugs/` cases named after them**,
  independent of this verdict.
- **R6 · The variadicity `_Static_assert` is NOT built.** It false-fires on real
  bindings for a structural reason, and the sitting records that so the next
  convener does not re-propose it.

**What a veto would compel**: the ffi-pragmatist's condition names the only path
back — a generated unit for a `...` binding of `curl_easy_setopt` whose return
assertion compiles against the real header. It could not be constructed in this
sitting.

## Predictions to score

| seat | prediction | scored at |
|---|---|---|
| compiler-engineer | a landed `...` forces either a new `-Wformat-security` pragma in a blessed emission or removal of `-Werror=format` from `cli_flags.hero:32` | if `...` ever lands |
| ffi-pragmatist | under the rename with `emit_ffi_lookup` re-keyed on the link name, both arities of POSIX `open` compile in one module, one group, no shim | the day the rename lands |
| llm-ergonomist | over a fresh corpus under `...`, explicit width conversions after the format string fall below 20% | if `...` ever lands |
| spec-warden | if `...` lands under +40, the ledger buys the missing half within two amendments for ≥ +30 more | second spec amendment after `...`, or M-ffi-ladder close |
| spec-warden | `grep -c '\.\.\.' selfhost/*.hero` returns 0 | M-selfhost-fixpoint |
| historian | the emitted arm64 call stores the variadic argument to the stack, not `x1` | M-package-layout close |

## Author's verdict

**RATIFIED 2026-08-26**, author instruction *"fai la cosa migliore e piu
robusta"* — a mandate to take the most robust option rather than the one already
answered. R1 through R6 stand as adopted: **neither half lands**.

**Why the robust reading is the refusal, and not the language growth.** §1.12
makes not corrupting memory a goal that outranks ergonomics, and `...` moves in
the wrong direction on exactly that axis: it removes the declared width that
makes a wrong argument a compile error, in the one call form where C punishes
hardest. The three lines the rename would save are ergonomics; the +23 tokens
and the second notation are permanent. Both wait.

**What the sitting is asking the author to take again, and why it is worth
asking twice.** The answers `1c` and `2a` were given before it was measured that
one of the two premises they rested on is false and the other costs §4.19's own
acceptance ladder:

- `2a` was answered because *"a library whose calls differ only in arity is
  bindable once"*. **Two modules bind one C symbol at two arities today**, built
  three times independently during the sitting. What the rename buys is five
  lines and one file, for +23 spec tokens.
- `1c` was answered as the more thorough of two ways to close a hole. **The hole
  is not there** — the fixed-arity binding routes through the header's own
  prototype, which is the design GHC added `CApiFFI` to reach and OCaml's ctypes
  tells its users to switch to. And `...` makes `curl_easy_setopt` unbindable,
  compiled against the real header.

A yes to the provisional resolution settles that neither half lands now, that
the rename is spelled `tag` if it ever does, and that the two live defects get
`fixedbugs/` cases regardless. A no reopens `1c`, `2a`, or both — in which case
the ffi-pragmatist's condition is the thing to bring: a generated unit for a
`...` binding of `curl_easy_setopt` whose return assertion compiles.

**What the yes does not settle**: whether the width of a value in a variadic
slot should ever be checkable. Panel 051 measured that hole, this sitting
confirmed it (`%d` fed an `i64` prints `7` for `4294967303` at exit 0, where the
same program at fixed `i32` is exit 1), and neither half of this proposal closes
it. The ffi-pragmatist named a lever nobody has pulled: `-Wformat-nonliteral`
does fire, twice, mapped onto the author's own `.hero` lines, because clang knows
the callee is a format function.
