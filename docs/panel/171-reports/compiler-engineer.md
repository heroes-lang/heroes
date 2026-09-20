# Panel 171 — compiler-engineer

**Every number below was produced by a command run 2026-09-20, Darwin arm64, in
a copy at `<scratchpad>/ce-copy` made by `cp -r` of the working tree, `target`
and `build` removed, `git log -1` reading `b6e26fcc` (step 21). Seed compiler:
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 3.24`.
The word is spelt `lent` throughout as a PLACEHOLDER: the ergonomist chooses
the spelling, and every cost here is one string compare away from any other.**

---

- **verdict**: approve — and it becomes a **veto on soundness** if the adopted
  rule omits the header clause for the `cstr` lend (§ The rule's edges, b10).
- **section**: design.md §1.7 (core plus elaboration) and Part 5 for the cost;
  §1.12 (robustness) and §4.19 for the rule; §4.17 for the diagnostic.
  **design.md does not yet state the flipped default anywhere** — grepped
  *assumed to keep*, *keeps what it is handed*: zero hits. The ruling lives in
  `docs/records/log/2026-09-20-1930-heroes-must-be-robust-and-the-default-flips.md`
  and §4.19 owes a sentence.
- **needed_for_self_hosting**: no (Principle 0). The closure list compiles
  today without the word. **After the flip the compiler needs it on 13 of its
  own parameters, the library on 2, the net's harness on 13** — a need the
  flip creates, not one the closure has, and it is the whole landing story.

## implementation_cost

**Frontend only. Zero lines in the IR, the descriptor pass, the ownership pass
or the emitter**, and that is measured rather than argued: `selfhost/ir.hero`
and `selfhost/ir/*.hero` carry 0 mark words in code and 0 `record Param`
(grepped this session); `--dump-ir` of a marked program shows 0 `lent`; the
emitted C of a marked program and of `tests/golden/emit/scalars.hero` is
**byte-identical between the compiler without the rule and the one with it**.
By §1.7's own test — *implemented in the type checker AND the lowering AND the
backend* — this is not core. It is not Part 5 sugar either; it is a declaration
mark one checker rule reads and lowering drops, the shape `counted_by` already
has.

Measured in `tests/harness/suite_layout.hero`'s own unit (`code_lines`,
`:525`; my awk replica reproduces 298/524/1174/207/293/216/277 exactly), then
confirmed by running the `layout` suite itself:

| file | before | after | ceiling | |
|---|---|---|---|---|
| `selfhost/ast.hero` | 524 | **527** | 525 DECIDED | **the one red line `layout` printed**; a row edit named here in advance |
| `selfhost/parse/members.hero` | 298 | 299 | 300 | the word's marker pays for itself: `consumes_marker` and `borrows_marker` were the same five lines with one string changed, and become one `bare_marker(word:)` with three calls |
| `selfhost/parse/tails.hero` | 288 | 288 | 300 | one caller renamed |
| `selfhost/print/fmt.hero` | 1174 | **1172** | 1175 DECIDED | `consumes_suffix` and `borrows_suffix` collapse the same way |
| `selfhost/print/dump.hero` | 207 | 204 | 300 | same |
| `selfhost/check/lend_extent.hero` | 216 | 278 | 300 | `Landing` replaces `Uncounted` and answers both marks; `declared_lends` (the shape rule); `landed_on_a_header` MOVED here from the sweep, because this module owns the landing |
| `selfhost/check/lending.hero` | 293 | 278 | 300 | the sweep asks one question of every lend and delegates |
| `selfhost/lend_errors.hero` | 277 | 268 | 300 | two new diagnostics put it at **315**; the lease trio leaves along the seam its own module doc names |
| `selfhost/lease_errors.hero` | — | **63** | 300 | new: `lease_escapes`, `lease_written`, `end_lease_of_no_lease` |
| `selfhost/check/leasing.hero` | 202 | 202 | 300 | `use lease_errors`, three prefixes |
| `selfhost/ffi_errors.hero` | 197 | 209 | 300 | `lent_shape` |
| `selfhost/check/ffi.hero` | 281 | 282 | 300 | one call |

**Net +113 code lines across eleven compiler files**, of which about 60 are
the two moves and about 55 are new logic and two diagnostics. Lexer: 0 (the
word is contextual, like the five beside it). Two `Param(...)` construction
sites, both one line. Before the two moves, `lending.hero` read 304 and
`lend_errors.hero` 315: **the ceiling was priced by splitting at named seams,
not raised.**

**The spec**, drafted in the copy and reverted (`diff` identical): the
`CParam` slot beside `counted_by` plus one sentence, **8201 → 8247 real (+46),
6159 → 6194 vendored (+35)**, ceiling 10240, `DELTA_GATE` 50. It moves two
pins: `selfhost/measure/pinned.hero:54,63` and `tests/harness/suite_spec.hero:187`.

**The compiler's own tests**: 673 at step A, all passed, 67.91 s; **675 at
step B, all passed**, after four emitter fixtures in `selfhost/emit/ffi_lend.hero`
and `selfhost/emit/ffi_mutable.hero` gained the word (they assert zero
diagnostics on programs lending into unmarked parameters). `order` 3/3,
`canonical` 2/2, `layout` 1 failed (the `ast.hero` row above).

**Speed** (CLAUDE.md § Verification, machine still): `check selfhost/main.hero`,
no rule vs rule live, **19.52 → 19.41 s and 19.55 → 19.56 s**, user ≈ real.
No slowdown the clock can see.

## THE LANDING, tested, and it is three trees rather than one

**The brief counts nine bindings at twelve call sites in `selfhost/`. I count
eleven functions and fifteen parameters in `selfhost/`, and a third tree
outside it**, and the two the brief missed are the ones that kill every program:

| tree | bindings | parameters | lend sites | what happens if they are not marked |
|---|---|---|---|---|
| `selfhost/cli/process.hero:42-53`, `selfhost/emit/literal.hero:42` | 9 | 13 | 12 | the flipped compiler refuses its own source; the fixpoint cannot be reached |
| **`selfhost/library_source.hero:152-153`** (`hero_file_read`, `hero_file_write`), lent into at `:208`, `:222` | **2** | **2** | 2 | **every `heroes check` of every program**, marked or not, exits **2**: `internal error: a diagnostic landed inside the Heroes library, at its line 168: [lend_kept]` — measured on a five-line program whose own binding WAS marked, and on `selfhost/main.hero` |
| **`tests/harness/shell.hero:44-57`** | **9** | **13** | 15 | `layout`, `order`, `canonical` all red with `lend_kept` at `tests/harness/shell.hero:526` and `:531`; **no suite of the net can run** |

The library is embedded text: the word inside it costs the old seed nothing to
compile (it is a string), so it may go in either commit; it MUST be in the flip
commit. The harness is outside `selfhost/`, so the `selfhost/**` row of
`.claude/rules/verification.md` would never have named it: the rule that catches
it is the same file's *a change to what the checker refuses is judged by every
golden tree* — and, this sitting adds, **by the harness that judges them**.

**Commit A — the word, no rule.** `ast.hero`, `parse/members.hero`,
`parse/tails.hero`, `print/fmt.hero`, `print/dump.hero`, their round-trip tests,
the spec line and its two pins, and **the seed regenerated**. Judged by the
`selfhost/**` row, the own tests, `spec` `special`, and the fixpoint. Measured:
the b6e26fcc seed builds it in **73.79 s**; heroes-A emits the seed in **76.37 s**;
clang rebuilds from that seed in **3.16 s**; the rebuilt compiler emits again in
**76.44 s**; `cmp`: **byte-identical**. `fmt` round-trips the word, `--dump-ast`
prints it, `lent` as an ordinary function name outside a group runs and prints
`2`, the word outside a group is left alone (`expected_params_close`), and
`atoi(s: cstr consumes)` is still `unread_mark` — the two slots are independent.

**Commit B — the rule, the shape check, the two diagnostics, and the 28
parameters marked in the three trees**, plus the corpus rewrites below, and
**the seed regenerated again**. Judged by every golden form (`check` `emit` `ir`
`unsupported`), `run` `emission` `determinism` `corpus` `annotations` `fixes`
`surface` `canonical`, the own tests, the net's own tests, `records`. Measured:
the commit-A seed's compiler (heroes-A2) builds the fully marked, rule-carrying
source in **61.11 s**; heroes-B emits; clang rebuilds; the rebuilt compiler
emits: **byte-identical**. Own tests 675/675.

**One commit — where the seed breaks, exactly.** The b6e26fcc seed's compiler
on the commit-B source:

    error[expected_params_close]: expected `)`, or `,` and another parameter, found a name (`lent`)
      at selfhost/cli/process.hero:42:40
      at selfhost/cli/process.hero:43:40
      at selfhost/cli/process.hero:44:46   (and on, one per marked parameter)

No binary. **Be precise about what breaks.** CI's `cmp seed/heroes.c fresh`
(`ci.yml:747`) is a FIXPOINT check and a squashed commit can pass it, if the
committer ran the two steps in the working tree and committed the second seed.
What a one-commit landing breaks is the **chain** `seed/README.md` § *If the seed
is already broken* rests on — *the tags are the chain*: the rung before this
commit can no longer build this commit's source, and the message that says so
names a parameter, not a stale compiler (`.claude/rules/verification.md` § *The
compiler that judges is a build artifact*, word for word). Two commits, each with
its seed, keep every rung buildable from the rung before. That is the whole
reason for two, and it is enough.

## The rule, as built, and what it costs the corpus

**Refused, at `check`, on the AST:** a lend expression (`s.cstr()`, `cstr(s)`,
`f.ptr()`) standing as an argument of a call whose callee is (1) not a function
of an `extern` group — `lend_needs_a_header` for `cstr` (new; the `ptr` twin
`field_lend_needs_a_header` is panel 166's), or (2) an extern whose parameter at
that position lacks the mark — `lend_kept`, new class, no `Fix`, two notes both
`guess`: the mark is a claim about C nothing here can check, and a lease is
wrong where C frees (panel 170's veto, kept). The `ptr` lend keeps its
`counted_by` question too, asked after: b8 (counted, unmarked) is `lend_kept`;
b9 (marked, uncounted) is `field_lend_uncounted`. **And the word itself is
refused where nothing reads it** — `lent_shape` on an `@` out-parameter, on an
`i64`, on a handle — which is `check/marks.hero:18`'s own rule applied to the new
word.

**Admitted at an unmarked parameter, each one run:** a lease name (exit 0),
`nullptr` (exit 0), a `cstr` C handed back (`keep(s: getenv(...))`, exit 0), any
name. **Not reachable:** a lend in an `@` position is `not_a_place` today; a lend
bound to the cell an `@` takes is `cstr_escapes` today; a lend at a handle
parameter is `type_mismatch`. The flip touches none of the three, and should not.

**Why the header clause for `cstr` is soundness and not taste (b10).** The rule
as briefed — *a lend at an unmarked `cstr`/`ptr` parameter is refused* — reads
the EXTERN's parameter. A Heroes function taking `cstr` carries no mark and
cannot: `wrap(s: cstr)` forwards `s` (a name, never a lend) to an unmarked C
parameter, and `wrap(s: "12".cstr())` then hands C a pointer it keeps, at exit 0,
through a route the literal rule cannot see. Panel 166 closed this route for
`ptr`; panel 122 opened it for `cstr` on the ground that legality depended on
what another file declares — **which is now true of every lend under the
flip**, so that ground is spent. Built: b10 exits 1 with `lend_needs_a_header`,
and the wrapper that takes `str` and lends inside itself is quiet.

**Which layer.** `check`, on the AST, and panel 169's open-set worry does not
apply: the rule reads a declaration's parameter (`fd.value.params[position]`)
and an argument's kind, `record_arguments` (`lending.hero:104`) is an exhaustive
match the language closes, and the IR has no `Param` to move the rule to.

**The corpus, measured with both compilers over 433 programs (`tests/golden/**`
plus every `examples/*/main.hero`): 35 change verdict, no diagnostic is lost.**
64 `lend_kept` sites and 4 `lend_needs_a_header`, on **50 distinct
declarations** (file, callee, parameter). By tree: `check` 8 (their `.expected`
and annotations move), `fixedbugs` 8, `run` 11, `ir` 1, `unsupported` 1,
`surface-fixtures` 3, `examples` 3 programs at 8 sites (`curl` 1, `ledger` 4,
`sqlite` 3 — panel 170 said eight, and it is eight). Two shapes need rewriting
rather than marking: `surface-fixtures/twoarity`, whose `as_text.say(format:
cstr, value: cstr)` is the b10 wrapper (3 refusals, `suite_surface.hero:277`
expects exit 0), and `check/fixedbugs-a-lend-laundered-through-a-helper.hero`,
which gains one more true refusal.

**Unrun, and owed by the landing commit** (CL-036's list): the two
highlighters, `editors/vscode/syntaxes/heroes.tmLanguage.json` and
`site/src/lib/highlight.ts`, and `heroes mutate`.

## argument

The mark is frontend-only: +113 code lines in the layout unit, zero in IR,
descriptors, ownership or emitter, and the emitted C is byte-identical with and
without the rule. The ceiling is met by two splits at seams the module docs
already named, not raised; `ast.hero` 525 → 527 is a row edit. The compiler is
not measurably slower. What decides the landing is a count the brief got wrong:
**twenty functions and twenty-eight parameters across three trees**, and the
two in the embedded library refuse every program at exit 2 if forgotten. Two
commits, each with its seed, are what keep the bootstrap chain buildable rung
by rung. And the `cstr` lend must stop at a Heroes wrapper, or the flip closes
066 through the front door and leaves the side door open.

## prediction

At the commit that flips the rule: **its pathspec contains
`selfhost/library_source.hero` and `tests/harness/shell.hero`.** If the first is
absent, `./heroes check tests/golden/run/ffi-cstr.hero` exits **2** printing
`internal error: a diagnostic landed inside the Heroes library`; if the second
is absent, `./heroes run tests/harness/main.hero -- ./heroes layout` prints
`lend_kept` at `tests/harness/shell.hero:526`. At that milestone's close,
`tests/harness/suite_layout.hero`'s `DECIDED` reads `selfhost/ast.hero 527`
(±1), none of `members.hero`, `lending.hero`, `lend_extent.hero`,
`lend_errors.hero` is on the table, `selfhost/lease_errors.hero` measures
63 ± 10 in `code_lines`, and the `check` form carries **8 ± 1** changed
`.expected` files.

## condition

**To veto:** a resolution whose rule reads only the extern's parameter and
admits a lend into a Heroes function taking `cstr`. The instrument is b10 —
`wrap(s: cstr)` forwarding to an unmarked C parameter, called with
`"12".cstr()` — and it must exit 1. **To object:** a landing plan that puts the
word and the rule in one commit, or that omits the library or the harness from
the flip commit; the runs above are the instrument. **What would make my cost
wrong:** `layout` printing a second red line, or `check selfhost/main.hero`
reading more than a second slower with the rule — neither did today.

## What I did not do, said plainly

I built in a copy and never rebuilt from `selfhost/` in the main tree. I did
not run the full net or the `check`/`run`/`emission` forms on the copy: the
corpus diff above says which 35 programs move and how, but the `.expected`
files were not regenerated and the four goldens that must be REWRITTEN rather
than marked were not rewritten. The spec sentence is a draft for the warden. The
word is a placeholder. One row of my FIRST corpus pass read fewer diagnostics on
`ffi-a-lent-field-needs-a-place.hero`; it was taken while three other jobs ran,
the re-run alone reads the four old codes plus two `lend_kept`, and the first
reading is discarded (CL-025's shape, in a `check` rather than a clock).
