# 031 — M-robustness-guards: the guards that shut the holes

## Goal

design.md §1.12 makes not crashing and not corrupting memory a **goal** of the
language, and CLAUDE.md §12 gives that goal the tie-break over every other
criterion in the contract. On 2026-09-03 a `/decide` sitting read twelve open
items, verified each against the repository, and measured four places where the
promise did not hold: `@` on an immutable binding accepted and corrupting the
heap; a `certain` fix that reproduces itself on the FFI's most natural spelling
while the instrument meant to catch it cannot see module diagnostics; an FFI
pointer verdict that differs by which clang reads it, with a `size_t *`
out-parameter declared `i32` accepted at exit 0; and deep recursion dying at
exit 139 without a word. The author ratified every recommendation and then set
the criterion in four sentences — *"scegli le soluzioni più robuste e complete
rispetto a quelle più economiche"*, *"privilegia il consolidamento e le
soluzioni migliori non le scorciatoie"*, *"più storia e meno attenzione al
token"*, *"più robustezza su tutte le piattaforme, non silenziare errori"* —
which reversed one recommendation (the FFI flag goes strict, never `-Wno-error`)
and strengthened three.

This milestone is those four holes shut in their complete form, plus the harness
scratch two runs could share and the diagnostic panel 099 R5 owed. Six steps,
two sittings, every landing measured on the Mac, the Linux image and the
Windows box before its commit.

## What surprised

**The eight shapes were attacked before the repair, and the attack found two
more.** CLAUDE.md §1 says a repair is attacked at the shapes next to the one
that provoked it; here the shapes were measured on the seed of 2026-09-02 17:52
before a line was changed. Seven of eight passed `heroes check`: a by-value
parameter, a `=` local, a field, an index, a nested record, a `for` variable, a
`match` payload and a generic `@x: T`. Three corrupted memory (field exit 134
with the leak panic, index **exit 138**, nested record 134), three mutated an
immutable in silence and printed `2` at exit 0, and the one refusal —
`add(@m["k"], "x")` — was `type_mismatch`, because `m["k"]` is a `V?`: the right
verdict for a reason that proved nothing about the rule. Then the sweep of the
positive side, which was meant to confirm that a cell through a path works,
found that `add(@make(), "x")` — `@` on something that is not a place at all —
also passes `heroes check`, and that the **legal** `add(@bs[0], "y")` on a cell
`bs: [Bag] @ …` dies at exit 138, ASan `BUS in hero_array_incref`.

**The emitter's comment said the crash could not happen, and said why.**
`emit/aggregate.hero`'s `place` renders an inout place as a C lvalue, and its
comment read: *"An index step cannot appear yet — the container store owns it —
and the walk stops rather than producing something that compiles."* The walk
stopped, the text compiled, and the callee received `&h0_bs` — the address of
the WHOLE array where a `Bag *` was expected — with a clang warning nobody was
looking at. CLAUDE.md §11's shape exactly: the argument stayed valid while the
premise died, and the comment went on reading as correct. The corpus held 150
`@x.field` arguments and zero `@x[i]`, which is why nobody saw it, and design.md
§4.8 (panel 010) names `f(a @ xs[0])` as a legal form, so it was a compiler
defect and not a form to refuse. The strict `-Werror=incompatible-pointer-types`
that step 3 brings would have turned that warning into an exit 2 — a compiler
bug named as one — which is one more argument for the direction the author chose.

**One rule closed all of it, because every place has exactly one root.** The
resolver's rule for the left of `@` — `write_root` — had exactly one caller, the
mutation statement. The checker's `marker_mismatch` asked only whether an
argument's marker matched the parameter's. Holding the root of every `@`
argument to `write_root`'s rule closed the seven shapes with the diagnostic that
already existed, `not_mutable`, each kind with its own wording; the parser's
`not_a_place` — which already refused `f(x) @ 1` — refuses `f(@g(x))` beside the
same `is_place`; and the index path became a copy-back through a temporary, the
element read once before the call and stored back once after it through the
indexed store the language already had. §4.10's *no aliasing exists anywhere*
is what made the root comparison a complete test rather than a dataflow
analysis, and it is what made this one rule rather than eight.

**The knot was at its ceiling and the mechanism did not have to enter it.**
`ir/lower.hero` sat at exactly its decided 1483 lines of code. The temporary and
its write-back need nothing from the knot — the caller computes the place; the
new `ir/inout.hero` only knows how to read one and how to write one back — so it
is its own module at 158 lines, and the knot gained a hand-over (28 lines, 14 of
them the formatter breaking two `emit_call` sites whose argument list grew by
one word). The same move in the other direction: `write_root` was a leaf of the
walker's ring, and when its second caller took `resolve/walk.hero` past 300 the
leaf left, into `resolve/writes.hero` at 91 lines. Two files grew past a ceiling
and two decided numbers moved with dated reasons; two files were split at a seam
that was there all along.

**A promise with no executor, one department over (step 2).** CLAUDE.md §8
promises that CI checks an applied `certain` fix, and `tests/harness/
suite_fixes.hero` made it executable — for `check/`, where a case is one
file. Every diagnostic about modules needs two files to provoke, so the
promise had never once run on the module class, and that is how
`needs_qualifying` shipped a `certain` fix that reproduces itself: `use window`
beside `window: ptr`, `_ = window.destroy()`, and `--apply` writes
`window.window.destroy()`, then `window.window.window.destroy()`. The `/decide`
item that scheduled this step said panel 102's file-wide reservation had
closed the path; measured on the seed of 2026-09-02 17:52, it had not — the
reservation *reports and then binds*, so the dot still falls through a `ptr`
with no fields to the module path. Both halves landed: the fix is a `guess`
with the measurement written where the certainty used to be argued, and the
fixes suite walks `surface-fixtures/` for an `X.fixed` beside its `X.hero`,
applies through the real binary, copies the directory to scratch with the one
file replaced, and re-checks EVERY `.hero` in the copy, because the fix is
applied to one file and the program is all of them. The first `.fixed` is
`unknown_in_module`'s one-candidate repair, `geom.dist` → `geom.dist2`,
which only checks clean beside its module; the second fixture is the `window`
program itself, pinned by two surface rows that assert `fix (guess)` and an
`--apply` that changes nothing.

**The seat that compiles corrected the coordinator on the same point twice
(step 3).** The brief cast only `ptr`. Both compiling seats measured that the
strict flag then refuses `time(@t: i64)` and `getline(@n: u64)` on macOS alone
— `long *` and `long long *` are the same 8 bytes and two types there — and the
engineer went one further: the probe's typed `uint64_t *` against `size_t *`
was already being claimed by the width reader as a FALSE `ffi_parameter_type` on
the Mac. So the cast is on every opaque or numeric `@`, in the probe, the call
and the assertion alike, and the pointee check is the compiler's own. A `cstr`
`@` stays typed on purpose: its `const char **` against a header's `char **` is
panel 058's writability refusal, and a cast would have silenced it — the warden
measured that the ergonomist's *`ptr` or a `cstr`* wording would have written
the same falsehood into the spec.

**The header's type text comes from clang, and the verdict is asked of clang,
and the two are different processes on purpose.** `-ast-dump=json` gives the
parameter's `qualType` — `size_t *restrict`, the same shape on clang 18, 21 and
22, measured on all three plus the Windows box — but not its canonical width;
`desugaredQualType` is absent on parameters. So the JSON is only where the
TYPE TEXT comes from; the verdict is two `_Static_assert`s in a second, tiny
unit, `sizeof` and the sign of `(T)-1`, C11 arithmetic on the header's own
type and never a width table this compiler carries. The check runs once per
build, is cached like an object with the headers it read, and flows its failure
through the same reader path as every other clang failure — which is how
`ffi_parameter_type` lands on the parameter's name with `@n: u64` as the guess
without a line of column arithmetic. The emitter gained nothing but the casts.

**The spec sentence landed in the same commit as the check, and not a minute
before**, which is the warden's condition and panel 060's lesson: a document
that says the compiler refuses something it accepts is false by fiat. w5 at
3718, the clause's third size in nine days, and the 172 blessed emissions moved
with it in exactly four line shapes.

## What broke and why

- **`keys` is a built-in.** `ir/inout.hero` named a local `keys` and the
  compiler refused it four ways at once (`builtin_name_taken`,
  `no_mutable_globals`): a built-in's name is taken everywhere. Renamed
  `indices`. The port note in `resolve/walk.hero` had recorded the same lesson
  for `args`.
- **`lowered` was the lowering's own test helper.** A local named `lowered` in
  seven call sites shadowed `function lowered(text)` at the bottom of the same
  file — panel 015's rule, a declaration is in scope everywhere. Renamed
  `handed`.
- **A cut that took a neighbour with it.** Moving `inout_root` out of the walker
  by slicing from its comment to the next section heading also removed
  `bind_payload`, which sat between them; the build said `bind_payload` was
  *declared in module `ir/lower`* — the name it found next — and that `use token`
  was unused, which was the tell. Restored verbatim; the orphaned `use
  inventory` (write_root's only user) removed.
- **A golden that broke two rules at once.** The run case's `swap(@bs[0],
  @bs[1])` needed labels (two parameters of one type) and would then have been
  panel 010's alias refusal, two `@` arguments with one root whatever the
  indices. Two cells, `swap(a: @bs[0], b: @cs[0])`.

- **Two pins and a label (step 2).** `resolve/qualified.hero` had a test
  asserting the fix certain — the pin that made the downgrade a visible
  decision rather than a quiet one; `suite_surface.hero` counts its own rows
  (33 → 35) and `report.tally` takes two `Report`s, so the labels are
  mandatory. Each red run said exactly which line to read.

- **Two more built-in names, and the label rule, four times (step 3).**
  `find` is a built-in like `keys`; `strip_word`, `locate_text`,
  `parameter_types` and `with_search` each take two parameters of one type,
  so every call names them. Each red build said the line. The formatter
  reflowed one call before the fix reached it, so the replacement missed once.
- **Two pins guarded the old shape, correctly.** `extern_probe.hero`'s test
  expected `(void)(fill)(a0, a1)` and `assert_spelling.hero`'s expected
  `(int64_t *)0`; both now say what the emitter says, `(void *)a0` and `0`.
- **A caveat that was true for `long` and false for `size_t`.** The typedef
  rows first reused `long`'s note — *Windows is 32 where the Unixes are 64* —
  which is false for `size_t` on 64-bit Windows; the object-size typedefs got
  their own note before the first golden could print the wrong one.

## What landed, and what carried forward

*(appended at the milestone's close)*
