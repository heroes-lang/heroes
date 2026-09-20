# Panel 169 — compiler-engineer

Every number below came from a command run in this session, on 2026-09-20, on
Darwin arm64, in a `cp -r` copy of the tree at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e64edfa2-e1eb-4a90-b26e-8635f02430e5/scratchpad/tree`,
with the compiler built from the seed in **4.16 s**
(`/usr/bin/time -p clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`).
Nothing was measured in the main tree and nothing there was modified but this
file. `archive/bootstrap-rs/` was never opened.

**I built route R2 and it is running.** The three compiler files it touches are
listed under `implementation_cost`; the working copy is in the scratchpad above.

---

- **verdict**: `object`

- **section**: design.md **§1.7** ("Anything in the core must be implemented in
  the type checker *and* the lowering *and* the backend"), **Part 5** (the seven
  core constructs), and **§1.12** ("A Heroes program must not segfault and must
  not corrupt memory … that is a goal of the language"). The objection itself
  rests on CLAUDE.md § RUN IT — *"A repair is attacked at the shapes next to the
  one that provoked it … A class is not a class until its exceptions have been
  looked for"* (CL-061) — and on `docs/work/DEFECTS.md`'s own preamble, *"A
  repair is owed at the class and not at the witness."*

- **implementation_cost**

  Measured with the instrument, not with `wc -l`: `tests/harness/suite_layout.hero`'s
  `code_lines` (`:526-560`), read by setting `CEILING` to 0 and every `DECIDED`
  number to 0 in my copy and running
  `./heroes run tests/harness/main.hero -- ./heroes layout`, which then prints
  every file's count.

  **R2, built and running, in the widened form I ask for:**

  | file | before | after | delta | ceiling | spare after |
  |---|---|---|---|---|---|
  | `selfhost/check/lend_alias.hero` (NEW) | — | **145** | +145 | 300 (§11) | 155 |
  | `selfhost/lend_errors.hero` | 277 | **298** | +21 | 300 (§11) | **2** |
  | `selfhost/checker.hero` | 103 | **108** | +5 | 300 (§11) | 192 |
  | `selfhost/check/lending.hero` | 293 | **293** | 0 | 300 (§11) | 7 |
  | **total** | | | **+171** | | |

  **Zero ceilings breached. Zero `DECIDED` entries move.** `git diff --stat` on
  the two edited files reads `2 files changed, 28 insertions(+)`; the new module
  is 211 lines by `wc -l` and 145 in the instrument's unit, the difference being
  its ten tests, which `code_lines` excludes.

  **Where it does NOT land, and this is the whole argument.** R2 touches no
  lexer, no grammar, no AST node, no type rule, no descriptor pass, no ownership
  pass, no IR, no emitter, no formatter, no highlighter, no `heroes mutate`, no
  `heroes measure`. It is one checker sweep, one diagnostic code, one call line.
  By §1.7's own test — *"does it move something from the core to the sugar, or
  remove a special case from the compiler?"* — R2 adds **nothing to the core**:
  the seven constructs of Part 5 are untouched, because a refusal has nothing to
  lower.

  **Re-measured, not carried** (panel 168 named three files; panel 167 named
  three different ones a day earlier):

  | file | measured now | `DECIDED` | headroom |
  |---|---|---|---|
  | `selfhost/emit/inst.hero` | 350 | 350 | **0** |
  | `selfhost/emit/ctype.hero` | 395 | 395 | **0** |
  | `selfhost/ir.hero` | 310 | 310 | **0** |
  | `selfhost/emit/gate.hero` | 365 | 365 | **0** |
  | `selfhost/ast.hero` | 524 | 525 | 1 |
  | `selfhost/print/fmt.hero` | 1174 | 1175 | 1 |
  | `selfhost/parse/members.hero` | 298 | — (§11's 300) | 2 |
  | `selfhost/check/walk.hero` | 1863 | 1870 | 7 |

  Panel 168's three zeros are confirmed and a **fourth** is added,
  `emit/gate.hero` at 365 of 365 — which matters, because it is the file the
  word "gate" sends a reader to for defect 069.

  **R1, the handle route**, priced on files I opened: `selfhost/check/marks.hero`
  **105**, `selfhost/check/acquiring.hero` **249**, `selfhost/handles.hero`
  **172**, `runtime/parts/alloc.c` (the live set, `hero_handle_acquired` /
  `hero_handle_consumed`). All three have room. R1's cost is not lines; see
  `argument`.

  **069's repair**, priced by hand-patching the emitted C rather than by
  reasoning: `selfhost/emit/inst.hero:295-312`'s `.func_ref` arm needs the
  `.extern_fn` case to render `source.span_text(s, decls[e.decl].name)` — the
  same expression `selfhost/emit/ops.hero:138` already uses for an extern call.
  **About six lines, into a file with zero headroom.** Either raise the
  `DECIDED` 350, or move the 18-line `.func_ref` arm into its own module, which
  frees about twelve lines net. That is the split `.claude/rules/module-shape.md`
  prescribes and it is cheap.

  **The spec sentence R2 owes**: `./heroes measure spec/heroes-spec.md` reads
  `cl100k_base` **6126** today and **6141** with the sentence added, **+15**.
  The `real` row re-pin is **unrun**: it needs `--refresh` and `.env`, which a
  scratchpad copy does not carry.

- **needed_for_self_hosting**: **no**. `selfhost/` was in the 752-file scan and
  holds no lend the rule refuses; the compiler compiles itself without R2. R2
  enters under Principle 0's **second** door — a measured design.md Part 11
  effect — and specifically under §1.12, which the sitting must state as the
  entry ticket rather than leaving it to a reader. §1.12's own text refuses the
  shortcut: *"It does not suspend Principle 0 … 'it would be safer' is not an
  entry ticket."* What makes R2 admissible is not safety in the abstract; it is
  that a **wrong answer at exit 0 with no sanitizer able to see it** is the
  failure §1.12 names as the one the language exists to make impossible, and
  the form is already admitted — R2 decides its *shape*, which is exactly what
  §1.12 says it is for.

- **argument** (≤120 words)

  R2 works. I built it, and the corpus scan reads **0 of 752** `.hero` files
  refused, with three planted reproducers each found. But the wording on the
  ballot — *"not re-assigned in this function"* — repairs the **witness**, not
  the class. `fill(@t)` is a call, not a `@` statement, so the stated rule walks
  past it; I ran that program and it printed **72 then 1 at exit 0**, defect 068
  exactly, one shape over. Widening to *"not written"* — a `@` statement **or**
  a `@` argument — costs twenty-two lines, catches both, and still refuses **0
  of 752**. Adopt the widened wording or the sitting closes a reproducer and
  ships the defect.

- **prediction**

  At **M-declared-extents close**, with R2 landed in the widened form:
  `tests/harness/suite_layout.hero` will carry **no new `DECIDED` entry and no
  raised number**, and `selfhost/lend_errors.hero` will measure **298 or less**
  in `code_lines`' unit. Falsified by any `DECIDED` line added or changed in the
  commit that lands R2. And: the full net
  (`./heroes run tests/harness/main.hero -- ./heroes`) will show **zero** newly
  failing cases attributable to `lent_root_rewritten`, because the sweep pushes
  that diagnostic on no file in the tree today.

  Second, on 069, checkable the day its repair lands: the fix will be
  **emission, not refusal**, and the emitted line will be an assignment of the
  header's own unmangled name — `t4 = cb_free_it;` — because I compiled and ran
  exactly that line (clang exit 0, program exit 0) and clang refuses the
  mismatched shape (exit 1, `incompatible function pointer types`).

- **condition**

  I withdraw the objection and approve on the spot if the resolution's wording
  is *"a binding whose field's address has been lent in this function is not
  **written** in this function — by a `@` statement or as a `@` argument"*, and
  the golden case beside it is the `fill(@t)` program, not the `s @ …` one.

  I would move to **veto** on one finding only: a corpus program that R2-widened
  refuses and that is sound. I searched for one and found none in 752 files; if
  another seat produces one, the rule is over-refusing where it was measured
  free and the trade changes.

  I would **approve R2 as worded** if somebody runs `fill(@t)` and it does not
  corrupt. It does; the command and the output are in the appendix.

---

## Appendix — every run, because a scratchpad path dies

### A. The reproductions, on the seed-built compiler (no R2)

**Defect 068**, `alias.hero` (panel 168's completeness critic's program, retyped
from `docs/panel/168-reports/completeness-critic.md:520`):

    ./heroes check alias.hero            -> exit 0
    ./heroes run alias.hero              -> "72" "1", exit 0

The honest answer is 72 twice. **Reproduced.**

**Defect 069**, the callback. Note the first attempt failed on an unrelated
`ffi_parameter_type` for `malloc(size: i64)` — `build` exit 1, `u64` is the
answer — so the reproducer here declares `malloc(size: u64)`:

    ./heroes check cb.hero               -> exit 0
    ./heroes build cb.hero -o cbprog     -> exit 0, "wrote cbprog"
    ./heroes run cb.hero                 -> exit 134
       panic: entered unreachable code — this is a compiler bug, please report it

The emitted C, `--emit-c`:

    cb.c:110     h_0fn_406f9b0 t4;
    cb.c:123     hero_unreachable(); /* the gate refuses this form */
    cb.c:127     (void)cb_take(t3, t4, t5);

**And the repair is one line of C.** `sed`-replacing line 123 with
`t4 = cb_free_it;`:

    clang -I runtime -I . cb_fixed.c runtime/runtime.c -o cbfixed   -> exit 0
    ./cbfixed                                                        -> exit 0

And the mismatched shape is refused by clang, which is the discipline
`.claude/rules/generated-c.md` already relies on:

    clang -I . -fsyntax-only mismatch.c  -> exit 1
      error: incompatible function pointer types assigning to 'h_fn'
             (aka 'void (*)(void *)') from 'int (int)'

`selfhost/cli/clang_floor.hero:18` already documents that
`-Wincompatible-pointer-types` flips to a default error only at clang 22, which
is why the project pins a floor of 18 and carries the flag separately. So this
repair's type safety rests on machinery already installed.

### B. The retention vocabulary, re-run rather than taken from the brief

`borrows` on a `cstr` parameter: `./heroes check borrows.hero` **exit 1**,
`error[unread_mark]`, with the note that tells the author to name the C type and
mark a handle.

The handle live set, a handle acquired and never consumed:

    ./heroes run leak.hero   -> prints "16", then
      panic: 1 C handle(s) never given back — every call marked `acquires` owes
      one marked `consumes`, and this program is missing that many. The first is
      at 0x105589db0
    exit 134

**Read the order.** The program printed its answer **first** and aborted
**after**. The live set is an **exit** check, not a **use** check. That is R1's
measured limit for defect 066: it converts *wrong answer, exit 0, silence* into
*wrong answer, exit 134, an address* — a real gain under §1.12 and **not a
closure**, because the read has already happened. A seat arguing R1 closes 066
owes a run that shows the abort landing before the read.

Also measured on the way: the first `leak.hero` was refused with
`error[ffi_return_type]: h_open does not return HBlock — that is what h.h says,
and clang read it`. The tag has to name the C type (`record HB tag HBlock`).
That is the binding-author cost R1 carries and it is one line.

### C. R2, built

Three compiler files in my copy:

- `selfhost/check/lend_alias.hero` — **new**. `lend_receiver`, `lend_root_local`,
  `lent_roots`, `lent_roots_are_not_rewritten`, `judge_statement`,
  `judge_handover`, `judge_arguments`, `judge_write`, plus ten tests.
- `selfhost/lend_errors.hero` — one diagnostic, `lent_root_rewritten`, no `Fix`
  (that file's own standing reason: the repair is a different shape of program).
- `selfhost/checker.hero` — one `use`, one call, one comment.

**Three findings from building it that no brief carried:**

1. **No function-boundary walk is needed.** `resolved.Local` is per binding site
   and a local can only be named inside its owner declaration, so two roots
   sharing a `locals` index are in one function by construction. *"In this
   function"* is `.local l => l.at`. That removes the only piece of new analysis
   the route looked like it needed.
2. **The machinery is already written, in `check/leasing.hero`.** That module's
   clause 2 — *"the only write a lease cell takes after its declaration is its
   own `end_lease`"* — is R2's exact shape applied to a lease cell, and its
   `place_root` (`selfhost/check/leasing.hero:191`) is the walk R2 wants. R2
   calls it rather than copying it. So R2 is not a new kind of rule in this
   compiler; it is an existing rule pointed at a second root.
3. **Value semantics closes the aliasing hole for free.** `u = s` is a copy, so
   writing `u` cannot reach the bytes lent from `s`. A language with references
   would owe R2 an alias analysis; this one owes it nothing.

Gates run:

    ./heroes check selfhost/main.hero          -> exit 0, real 23.24
    ./heroes test selfhost/checker.hero        -> 397 tests, all passed
                                                  (ten of them this rule's own)

### D. The blast radius, measured with a program and a positive control

`selfhost/r2scan.hero` (my probe, not proposed for the tree) walks
`tests/golden`, `examples`, `selfhost`, `spec`, `tests/harness`, `site` and
`docs` with `hero_dir_scan`, parses, resolves and runs the new sweep on every
`.hero` file, counting only the diagnostics the sweep itself pushes.

    files scanned: 752
    files refused: 0

**The instrument is not watching itself** — three positive controls, each
planted in `tests/golden/`, scanned, found, removed:

| planted | scanned | refused |
|---|---|---|
| `alias.hero` (068, a `ptr` lend) | 753 | **1** |
| a `cstr` lend with a write to its root | 753 | **1** |
| `hole.hero` (the `@`-argument write) | 753 | **1** |

**Seven of 752** files hold a `ptr` lend rooted at a local at all, so 745 pay one
predicate-only pass over the expression arena with no allocation:

    tests/golden/fixedbugs/ffi-a-lent-field-extent-overstated.hero
    tests/golden/fixedbugs/ffi-a-lent-field-c-would-write.hero
    tests/golden/run/ffi-a-lent-field-counted-by-a-constant.hero
    tests/golden/run/ffi-a-byte-field-crosses-to-c.hero
    tests/golden/run/ffi-a-lent-field-reads-through-const.hero
    tests/golden/check/ffi-a-lent-field-needs-a-place.hero
    tests/golden/check/ffi-counted-by-shape.hero

**And the `cstr` widening is free too.** Swapping
`is_builtin_named(…, name: "ptr")` for `lending.is_lend` — which is `ptr` **or**
`cstr`, the half the corpus actually uses — also reads **0 of 752**, with its own
control found. So the sitting may take R2 over both lends without paying for the
wider one. That is a measurement the ballot did not have.

### E. The shapes beside the rule, which is where the objection comes from

Each of these is a test in the module and each was run.

| shape | R2 as worded | R2 widened |
|---|---|---|
| `s @ KSlot(…)` after `s.name.ptr()` — defect 068 itself | refused | refused |
| no write to the root | quiet | quiet |
| write to a **different** binding | quiet | quiet |
| write to a **field** of the lent root, `s.id @ 7` | refused | refused |
| write inside a `while`, textually below the lend | refused | refused |
| write **above** the lend | refused | refused (stated over-refusal) |
| two writes, one root | two messages | two messages |
| the §4.19 wrapper: lend and write in two functions | quiet | quiet |
| lend rooted at a `@` **parameter** | refused | refused |
| **write through a `@` ARGUMENT, `fill(@t)`** | **quiet** | **refused** |

**The last row is the objection.** I ran it as a program on the seed compiler:

    ./heroes check hole.hero   -> exit 0
    ./heroes run hole.hero     -> "72" "1", exit 0

Byte for byte the same failure as defect 068, in the same function, to the same
binding, through a syntax the stated rule does not look at. Closing it is
`judge_handover` and `judge_arguments`, **twenty-two lines**, and it asks the
argument's **root** so that `fill(@t.name)` is the same write as `fill(@t)`.

**Flow insensitivity is deliberate and I state the over-refusal rather than hide
it.** A write above the lend is harmless once and fatal on the second turn of a
loop, and the expression arena carries no order a rule could trust. Refusing
both is the loud direction `.claude/rules/module-shape.md` prescribes, and it
costs the corpus nothing — 0 of 752.

**R2's declared boundary, measured rather than asserted.** A lend made inside a
callee that took the binding as `@`, with the write in the caller, is **not**
068 and R2 correctly does not claim it:

    ./heroes run cross.hero    -> "128" "128", exit 0

Both reads are already garbage, because a `@` parameter is copy-in/copy-out and
the lent address was the callee's temporary. That is defect **066** — a dead
frame — and it belongs to whatever route the sitting adopts for 066.

### F. What I did not run, named

- **`./heroes build selfhost/main.hero -o heroes-next` and the golden suites
  against a compiler carrying R2.** My brief forbids the twenty-minute selfhost
  rebuild. The 752-file sweep is the substitute and it is **narrower**: it proves
  the sweep pushes no diagnostic on any file in the tree, so no `check` verdict
  moves and therefore no `build`, `run`, `emit` or `ir` verdict moves. It does
  **not** prove the seed fixpoint, and it does not prove the timing.
- **`/usr/bin/time -p` on `heroes check` with R2 compiled in**, against the same
  without. Unrun, same reason. The shape is one extra pass over the expression
  arena plus one over the statement arena, no allocation where no lend exists;
  `check/lending.hero:198-200` records that its own comparable pass cost 1.3%,
  which is the number to beat and not a number I measured.
- **`./heroes measure spec/heroes-spec.md --refresh`** after adding R2's
  sentence. `.env` is not in a scratchpad copy. The vendored delta is +15.

### G. Question 4 — is there a route nobody listed?

Two, and both are measured rather than proposed.

**R5, and it is dominated.** Give the lend a lexical extent in the source — a
`lend … as p` block whose end is where C must be done. It is the honest answer
to 066 and it is a **new surface form**: lexer, grammar, an AST node, and then
every tool in `.claude/rules/diagnostics-and-goldens.md` § A new surface form —
the formatter at `print/fmt.hero` **1174 of a DECIDED 1175**, `ast.hero` **524 of
525**, the TextMate grammar and `site/src/lib/highlight.ts`. Two of those files
have one line of headroom each. R5 buys over R2 only the cross-frame case, and
costs the core a construct that §1.7 says must be carried through the checker,
the lowering **and** the backend. **R2 dominates it for 068 and does not compete
with it for 066.**

**R3 with teeth, which is the one I would actually put on a later ballot.** R3 as
written is a refusal: the group says *this library may retain*, and the lend is
refused. But the run-time half already exists and nobody joined them — the
pointer-keyed aborting live set at `runtime/parts/alloc.c` and the lease counter
beside it. A group marked as retaining could **register** the lend instead of
refusing it, and the give-back **consume** it, and 066 becomes the abort I
measured in `leak.hero`. The reason I am not proposing it today is the same
measurement: that abort fires at `main`'s exit, **after** the read. Making it
fire at frame exit is where the work is, and it is a milestone rather than a
clause.

**And the thing both earlier sittings got wrong is worth naming once more, from
the compiler's side.** Panel 167 wrote *"there is no instrument at all"* over a
runtime that ships two of them and a checker that ships a diagnostic whose own
note tells the author which one to use. The tell was available for the price of
one `./heroes check` on a five-line file. `borrows` on a `cstr` exits 1 and says
so.
