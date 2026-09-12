# 036 — M-declared-freer: the string C hands you, freed by name

## The goal

`owned <C function>` on an `extern`, so that a `cstr` a C library allocates and
hands to the program is freed by the name its own declaration gives.

The fact underneath it was measured three ways before the sitting and it is the
whole warrant: **a caller-owned C string could be freed or read, never both.**
`@error: cstr` against `char **` was `error[ffi_writable_parameter]`; `@error:
ptr` followed by `.validated()` was `type_mismatch`; and from the spec alone no
route ran from `cstr` to `free(void *)`. So the leak that convened panel 108 was
not carelessness — it was a program the language could not express, which is a
hole in design.md §1.11's *everything comes from C*.

Panel 109 ratified the mark on 2026-09-04 with eight conditions. Panel 116 was
convened on the one question it left open — where the release is BUILT — and
answered it on 2026-09-06 with two vetoes in opposite directions whose
intersection was the resolution.

## What surprised

**A rule can be right in general and wrong at exactly one point, and finding
that point is the work.** Panel 058 refuses a writable `cstr` parameter because
lending a string's own bytes to a C that may write through them changes values
the program never passed to C. That reasoning is sound and the refusal is
correct — except where the mark says the program will OWN what C writes there,
which is the one case where C is not writing through the program's bytes at
all. The carve-out is three lines and it took a whole step, because the general
rule had to be understood well enough to say precisely where it stops.

**The spelling of a question is the question.** Three separate defects in this
milestone were one shape: a probe typed `void *` accepts every freer silently
where `char *` refuses six of ten; a marked cell spelled `const char **` in the
probe is itself what stops compiling; and a marked cell handed to C as
`const char **` at the call site is refused for the same reason one level over.
Each time, the fix was one cast or one word, and each time the wrong spelling
had asked clang a question whose answer was true and useless.

**An instrument's silence is worth what its positive control is worth.** The
Linux `--sanitize` leg is this project's judge for a leak in a C binding, and
the witness written for this milestone was green there on the first run. That
proved nothing until the same allocation, unmarked, was run beside it: **21
bytes, exit 1**. And the road to that control ran through two false negatives
worth writing down — a pointer still live in a local is *reachable* rather than
leaked, and LeakSanitizer scans the stack conservatively, so a stale pointer in
a dead frame is reachable too.

**A structural instrument can be the only one there is.** Panel 116 measured
that no configuration of this Mac can see the release's NULL guard missing —
`fclose(NULL)` survives here and a guardless release exits 0 even under ASan.
The same turned out to be true of the null store this milestone added before a
marked call, for a different reason: the C local is indeterminate and happened
to hold zero. Deleting it left the run case printing the same four lines at
`-O0` and under the sanitizers. What sees it is `tests/golden/ir/owned-cell.hero`,
where `UPDATE_GOLDEN=1` is forbidden.

**A count in prose is a claim, and the command beside it is the instrument.**
`examples/ledger/db/sqlite.hero` has said *eighteen functions* since it was
written, with `grep -c '^    function' db/sqlite.hero` written on the next line
as the way to check it. The answer was **17**. `sqlite3_free` arriving with the
mark made the sentence true by accident, on the day somebody finally ran the
command it names.

## What broke and why

**Three shapes of a broken mark reached clang as generated C.** `owned
bogus_name`, `owned` naming a **Heroes** function, and `owned` naming a
two-parameter `extern` its own header genuinely declares were each `internal
error` at exit 2 — the compiler blaming itself for a mistake in a `.hero` file.
Each type-checked, because the mark's presence is what makes the result a
`str?`; then the lowering found no usable freer, fell back to a plain call, and
left clang refusing `assigning to 'h_0opt_…' from incompatible type 'const char
*'`.

The cause is structural rather than an oversight: the probe is what checks a
freer, and **a probe is emitted per `extern`**, so a name nobody declares has no
probe at all, and a two-parameter freer's probe compiles perfectly well because
the probe calls it with two arguments while the release calls it with one. The
repair reads the declarations only and lives in the checker, which is also why
it fires in a bindings module nobody has compiled.

**The checker half and the emission half could not land separately, twice.**
Typing a marked `cstr` result as `str?` alone made clang refuse the generated C;
one day later, typing a marked `@` cell as `str?` alone did the identical thing
with a different message. Both were written, measured, and parked until the
lowering existed. The lesson is the same both times: **the type a program sees
and the code that makes it true are one change.**

**The write-back could not build its own release, and the reason was the
language.** A marked cell's release needs the freer, the library's `validated`
and the type table, which live in `ir/owned_release.hero` — and that module
already uses `ir/inout.hero`, where the write-back is paid. Heroes refuses
module cycles, so the release could not be built where it was owed. The shape
that came out of that constraint is better than the one that went in: the cell
records what it is, `emit_call` pays the ordinary stores, and `call_or_release`
pays the releases afterwards, on every path — which matters, because the two
marks are independent and `sqlite3_exec` returns an `i32`, so a release hung off
the result mark's branch would have been skipped for exactly the program the
milestone exists for.

## Predictions, scored

**Panel 109's coordinator**, on the two splits that had to come first: *"each
land at ≤ 900 and ≤ 400 code lines respectively with `DECIDED` rows lowered,
before the `owned` step opens."* **HOLDS, and by a wide margin.** `ir/lower.hero`
is **174** and `parse/decl.hero` **217**, both under §11's own 300, so neither
row was lowered — both left `suite_layout.hero`'s `DECIDED` table altogether.

**Panel 109's compiler-engineer**, on the size of the feature: *"the `selfhost/`
diff measures 180–300 code lines."* **FALSE, and low.** Steps 2 to 5 add **411**
code lines to `selfhost/` (comments and `test` blocks excluded, the suite's own
`code_lines` rule). The seat's estimate was made against the `cstr` half alone —
*"~270 lines through three files"* — and what it did not price is that the mark
has two positions and each one costs a checker rule, a lowering, an emitter
spelling and a probe spelling. The step-by-step split is the useful record: step
5 alone is **+164/−23**.

**Panel 109's ffi-pragmatist**, on the acceptance: *"`examples/ledger/db/sqlite.hero`
with `@error: cstr owned sqlite3_free` reports 0 leaks on the Linux `--sanitize`
leg."* **HOLDS, measured 2026-09-07 in the Linux container**: 16 tests pass and
`heroes run` reports zero leaks. The green is admissible because the instrument
was proved live beside it — the same allocations unmarked are **exit 1, 21
bytes** — and because a plain C leak is reported there at both optimisation
levels. Its second clause is about `ptr owned`, which did not land.

**Panel 109's spec-warden**: *"gross ≥ +49 whatever the wording."* **HOLDS** —
step 2 measured gross **+74**, net **+47**. Its `examples/ledger` clause is
conditional on the `ptr` half and is therefore satisfied vacuously: that half did
not land and the program prints no such line.

**Panel 116's compiler-engineer**: *"`--dump-ir | grep -c '<freer>'` is ≥ 1 and
`ls tests/golden/ir/ | grep -c owned` is ≥ 2."* **HOLDS**, measured **3** and
**4**. Its third clause, *"`selfhost/ir/flatten.hero` measures ≤ 1090 with the
three-site collapse"*, is **FALSE by 12 lines**: it is **1102**, under its
decided 1110 and over the seat's figure, because the three-site collapse the
prediction assumed was paid back by threading a callee and a position through
`lower_args` and `with_receiver`.

**Panel 116's coordinator**: *"`assert DECIDED.len() == 16` stays green with no
row raised."* **HOLDS** — the table is 16 entries and no row moved in either
direction after step 1.

**Not scorable here, and standing rather than lapsed.** Panel 109's
llm-ergonomist registered a first-try rate against a Part 11 reader harness,
whose instrument has never run; its stated scoring point is the first Part 11
run, so it keeps its date. Panel 109's historian registered a use-after-free
within the first ten corpus programs using `ptr owned`; that half is refused
under a standing veto, so the prediction returns with it or not at all.

## What the numbers were at the close

Three suites on this Mac: the compiler's own **588**, the net **1609**, and the
net's own tests **113**. The spec **3871** of a hard 4096, headroom **225** —
unmoved since step 2, because the sentence that step bought already described
the `@` cell this milestone finished with. The compiler **55,050** lines of
Heroes in **189** modules; the seed **747,095** lines of C, regenerated in
**36.30 s** and the fixpoint verified byte for byte. Runtime ABI **21**,
unmoved: the release is built out of instructions the IR already had, so the
mark needed no runtime entry point.

**Three blessed emissions moved and each diff was read.**
`tests/emission/examples-ledger-main.c` gained the whole feature — the
`sqlite3_free` return assert and probe, `const char * h3_cell0;`, the null
store, `(char **)&h3_cell0` at the call, `h_library_validated`, and
`(void)sqlite3_free(t19)`; two files are new, for the two goldens this step
added.

**The memory, measured twice in two positions.** The result position frees
**12.8 MB** per 200,000 strings (step 4). The `@` position frees **1.60 MB** per
50,000 SQLite error messages: the binding that asks for the message and never
frees it peaks at 4,587,520 / 4,571,136 / 4,587,520 bytes over three runs, the
marked one at **2,981,888** three times — and the marked arm is the one that
also copies every message into a Heroes `str`.

**And one number that was wrong before this milestone touched it.**
`examples/ledger/db/sqlite.hero` has said *eighteen functions* since it was
written, with the command that checks it written on the next line. The answer
was **17**. `sqlite3_free` arriving with the mark made the sentence true by
accident, on the day somebody ran the command.
