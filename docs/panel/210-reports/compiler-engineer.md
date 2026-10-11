# Panel 210, compiler-engineer: an `@` parameter the callee never writes

Seat: compiler-engineer (cost, soundness, core against sugar; design.md §1.1,
§1.7, Part 5). Tree: my copy of `1dd890751` under
`.claude/worktrees/scratch-b15/210-compiler-engineer/tree/`. Started 02:11,
measurements closed 02:44, by `date`. Every number names its command or file;
what I did not run says *unrun*.

## Summary block

- `verdict`: **veto** on the route as the brief words it (writes = `@` to the
  name, a field or an element, or an `@` argument); **approve** the route as
  built here (those four plus four more writes, `guess` fix); **object** to the
  `certain` multi-site fix as priced; **object** to R0 (accept, as today).
- `section`: design.md §1.12 (robustness; a fix must not lead a correct
  program into a refusal or a false message), §4.8 (copy in, copy out), §4.17
  (one mistake, one message), Part 5 (the rule is resolver-side, outside the
  seven core constructs). Design.md has no section on which calls *write* an
  `@` parameter beyond §4.8's copy-out; spec § 13 is where the lend and the
  consuming call say it, and I stand on those.
- `implementation_cost`: built, **+154 / -14 lines, all in `selfhost/resolve/`
  plus 14 changed lines to make the compiler obey its own rule**; 0 lines in
  the checker's type pass, the IR, descriptors, ownership, the emitter or the
  runtime (section 1). `check selfhost/main.hero`: **+0.39% instructions
  retired**; a cold build of a 7-line program: no difference measurable
  (section 5).
- `needed_for_self_hosting`: **no**. The trunk compiler compiles itself
  without the rule; with it, three of its own functions must lose an `@`
  (section 3).
- `argument`: Built the way the brief counts writes, the rule refuses 13
  correct handle helpers whose `@` the checker itself requires
  (`consumed_borrowed_handle`), tells them a false message (the copy-out hands
  back a dead handle, not "the value it passed"), offers on a `.ptr()` lend a
  fix that passes `check` and fails `build`, and hides `marker_mismatch` and
  `ufcs_on_mutable` behind the opposite repair. That is the veto. Counting the
  four missing writes, the rule is about 130 lines of resolver, sugar-side,
  finds 4 real sites in shipped code (3 in `selfhost/`, 1 in an example) and
  pays +0.39% on a check. The `certain` multi-site fix collides with
  `check --apply` writing the root file alone.
- `prediction`: at the landing lane's gate, the census of `heroes check` over
  `selfhost/main.hero` and every `.hero` under `examples/` and `tests/` names
  exactly **15 sites in 3 files** (fixedbugs-135: 8, fixedbugs-260: 6,
  `examples/ledger/db/sqlite.hero:322`: 1) once the three `selfhost/` edits
  land, and **none** is a function whose `@` parameter a `consumes` or
  `transfers` call ends or a `.ptr()` lend hands to C; and `check
  selfhost/main.hero` retires **under 0.5% more instructions** than the trunk.
- `condition`: the veto lifts when the landed rule counts the four writes of
  section 2 (or proves one unnecessary with a probe of section 2's table). I
  would approve the `certain` half if the fix is withheld whenever any edit
  lies outside the root file and a case shows `check --apply` converging on a
  two-module program with no oscillation (section 4).

## 1. What I built and where it lands

The rule reads two counters the walk already keeps (`resolved.Local`'s `reads`
and `writes`, `selfhost/resolved.hero:102-103`) in the sweep that already tells
`never_rebound` (`selfhost/resolve/unused_sweep.hero`). It reuses that code
(`selfhost/resolve/errors.hero:322` before), so `selfhost/diag.hero:185`'s
registry does not move. **Sugar-side, not core** (Part 5): nothing the checker,
the lowering or the backend must learn.

`route.diff` (322 lines, beside `tree/`), per module, `wc -l` before and after
and `diff | grep -c '^[<>]'`:

| module | lines | changed | what |
|---|---|---|---|
| `selfhost/resolve/unused_sweep.hero` | 109 -> 159 | 54 | `written_nowhere`, `spared` shared with `rebound_nowhere`, `marker_before` (the `@`'s byte read from the text, so `ast.Param`, whose file says it is at its ceiling, is untouched), one unit test |
| `selfhost/resolve/errors.hero` | 365 -> 396 | 31 | `never_written`, code `never_rebound`, one `guess` fix at the parameter's `@`, one unit test |
| `selfhost/resolve/meant.hero` | 118 -> 146 | 38 | `meant_write` counts any mutable local, not only `.cell` (line 48 before); `ended_by_signature` and `ended_write` |
| `selfhost/resolve/built_marks.hero` | 227 -> 257 | 30 | `callee_params`, `receiver_meant` |
| `selfhost/resolve/walk.hero` | 506 -> 507 | 1 | the call to `receiver_meant` |
| `selfhost/check/{leasing,lending,path_end,walk}.hero`, `selfhost/parse/signature.hero` | unchanged | 14 | the compiler's own three sites (section 3) |

Of the +154, 27 are the two unit tests. Gates run on my copy with the route
compiler (`heroes-route4`, built by the trunk's from the edited tree):
`heroes test selfhost/main.hero` **1609 tests, all passed**; `layout` **6
passed, 0 failed**; `check` form **679 passed, 2 failed**, the two being
fixedbugs-135 and fixedbugs-260 (section 3). No seed regenerated, no fixpoint
(unrun).

## 2. What counts as a write: every shape, probed

Probes in `probes/`, each run with `heroes-base check` (the trunk) and
`heroes-route4 check`; outputs `probes/out-*-<probe>.txt`.

| shape | probe | trunk | route | verdict |
|---|---|---|---|---|
| read, never written (the critic's) | p01 | 0 | 1 `never_rebound` | refused, right |
| `n @ …` | p02 | 0 | 0 | write |
| `p.x @ …` | p03 | 0 | 0 | write (root) |
| `xs[0] @ …` | p04 | 0 | 0 | write (root) |
| `@n` handed on to a writer | p05 | 0 | 0 | write (`writes.inout_root`) |
| `@n` handed on to a callee that never writes | p06 | 0 | 1, the callee's | the caller counts as written: one round per level |
| `for x in xs` only | p07 | 0 | 1 | read, refused, right |
| `xs @ xs.push(3)` | p08 | 0 | 0 | write |
| **field lent to C, `s.nsap.ptr()`, C writes** | p09 | 0 | 0 | **write, but only with `meant_write` widened**: without it (`ctl/heroes-nomeant`) it is refused, and its `@` removed (p09b) passes `check` at exit 0 and fails `build` with `field_lend_written` |
| `@out` to an `extern` | p10 | 0 | 0 | write (`inout_root`) |
| a hole in the module | p11 | 0 | 0 | spared (§4.16) |
| only a self-call `f(@n)` | p12 | 0 | 0 | counted as a write: a false negative, the loud direction |
| neither read nor written | p13 | `unused_binding` | same | one message, `unused`'s |
| **`bump(k)` for `@c`** | p14 | `marker_mismatch` | same | **write meant**: without the widening the route says only `never_rebound` and the checker's certain `@` is never reached |
| `function f(@_: i64)` | p15 | 0 | 0 | **gap**: the wildcard twin of defect 608, unbuilt |
| generic `@xs: [T]` | p16 | 0 | 1 | refused, right |
| stray `g(@n)` for a by-value `m` | p17 | `marker_mismatch` | same | write meant, one message |
| `@ n` spaced | p18 | 0 | 1 | the fix's span is the `@`, line 1 columns 12 to 13 (`check --json`) |
| `n @= 5` over the parameter | p19 | `cell_redeclared` | same | one message |
| the caller's cell held only by the false `@` | p20 | 0 | 1 at `n` | after the fix the caller's `k` is `never_rebound`: a second round |
| field lent to a `const void *` (C only reads) | p21 | 0 | 0 | counted as a write: the resolver cannot see `const`, clang can; loud direction |
| **a handle ended through `consumes`** | p22 | 0 | 0 | **write**: the runtime empties the parameter; its `@` removed (p22b) is refused `consumed_borrowed_handle`. Without `ended_by_signature` the route refused p22 (route v1, census section 3) |
| dropped push on a cell (trunk's R1b) | p23 | `never_rebound` | same | see section 4 |
| dropped push on an `@` parameter | p24 | 0 | 1 `never_rebound` | the checker's `discarded_value` is hidden (section 3) |
| **`n.bump()` for `@c`** | p25 | `ufcs_on_mutable` | same | **write meant**: before `receiver_meant` (route v3) the route said `never_rebound` alone |
| consumed in one branch | p27 | 0 | 0 | write |

**The four writes the brief's list misses**, each a probe above: a lend through
`.ptr()` (p09), a `consumes` or `transfers` argument (p22, 13 census sites), an
`@` the checker will add or refuse (p14, p17), and a UFCS receiver of an `@`
first parameter (p25). A method with `@` self does not exist (spec § 9: UFCS
does not apply when the first parameter is `@`), and no function type carries
an `@` (`selfhost/data_errors.hero:172-186`, defect 398), so every call of an
`@` function is by name. `end_lease(@x)` is an `@` argument
(`built_marks.hero`'s `END_LEASE_WRITES`); my probe p26 did not reach that
shape (`lease_written`), so that row is a reading, not a run.

## 3. The class: how large, counted by the built rule

The census: `census/one.sh` over `census/files.txt`, which is
`selfhost/main.hero` and the 2034 `.hero` files `find examples tests` lists,
2035 roots, each checked by the trunk's compiler and the route's, `xargs -P 8`.

- **Route v1** (the brief's writes plus the lend and the marks): 31 distinct
  sites in 17 roots, 13 roots newly refused. 13 of the 31 are handle helpers
  ended by `consumes` or `transfers` (`examples/ledger/db/sqlite.hero:253`
  and `:298`, three `dead-address-copy-*`, four `dead-handle-*`,
  `handle-a-helper-…`, `structtag/net.hero:15`, `ffi-consumes-…:39`,
  `ffi-transfers-…:22`): **every one a correct program the brief's rule
  refuses**, since `consumed_borrowed_handle` (`selfhost/check/consuming.hero:140`)
  requires that very `@`. And 3 in `selfhost/`.
- **Route v4** (`census/result4.txt`, after the 3 `selfhost/` edits): **15
  sites in 3 files, 4 roots, 2 roots newly refused, 0 the other way**:
  - `examples/ledger/db/sqlite.hero:322`, `stepped(@statement: Statement)`:
    `sqlite3_step(statement: CStmt)` takes no word, so the `@` is a false
    promise (spec § 3, `:78-80`: a copied address reaches one foreign thing,
    so a function taking one without `@` may still change what C holds). A
    real site, in a published example.
  - `fixedbugs-260`: 6 parameters read only, there to feed the alias check:
    the case needs a write per parameter or a narrower signature.
  - `fixedbugs-135`: 8 parameters whose only write is a dropped value of the
    place's own type. Under the route the case reads 8 `never_rebound` and
    **0 of its 15 `discarded_value`** (the resolver's error stops the checker),
    and the guess says *remove the `@`*, the opposite of the repair the
    checker would have offered (`write b @ …`).
- **The compiler's own**: `selfhost/parse/signature.hero:85` `arrow_habit(…,
  @a: ast.Ast, …)`, `selfhost/check/lending.hero:184`
  `keeps_its_argument(@c: …)`, `selfhost/check/path_end.hero:69`
  `broke_of(@c: …)`. Removed in my copy with their 4 calls, 14 changed lines;
  a control compiler built with only those edits (`ctl/heroes-ctl`) retires
  the same instructions as the trunk (section 5), so they cost nothing.

So the class in shipped code is **4 sites** (3 compiler, 1 example), and the
golden cost is 2 check cases rewritten.

## 4. The fix: `guess`, and why the `certain` half is priced and objected

Built: one `guess` at the parameter's `@`, only where `marker_before` finds the
`@` byte. A `guess` because the calls keep their `@`, which `marker_mismatch`
refuses without a fix where the parameter takes none
(`selfhost/data_errors.hero:150-163`: a fix only when `wants`).

**The `certain` multi-site fix, unbuilt, priced.** It needs each call's `@`
span per (declaration, position): a table in `resolved` (`selfhost/resolved.hero`,
415 lines), a record per marked argument at `built_marks.of_a_call`, the
local's position among its owner's parameters, and a fix per site in the
sweep. Estimate **+60 to +90 lines in `selfhost/resolve/` and
`selfhost/resolved.hero`, unrun**. Three facts measured or read that bound it:

1. `check --apply` writes the root file alone; a certain fix outside it is
   `elsewhere`, never written (`selfhost/cli/certain.hero:43-60`). A function in
   one module with calls in another is therefore applied in part, and a call
   left bare where the parameter still takes `@` draws `marker_mismatch`'s
   certain `@` (`data_errors.hero:157-162`): the two fixes undo each other
   round after round, the shape `selfhost/resolve/meant.hero:7-12` names.
   So **certain only where every edit lies in the root file**, a fact about the
   value in hand (`source_extent.root_end`).
2. Heroes has no `pub`: a module checked as its own root may be `use`d by
   another program whose calls the fix cannot see. Those calls then fail
   loudly (`marker_mismatch`, no fix), never silently: the meaning is
   preserved everywhere, since a parameter never written copies out what came
   in (§4.8). Whether *compiles in this file set* is enough for `certain` is a
   question for the spec-warden, not a measurement.
3. The trap R1b already ships for cells, measured on the trunk (p23):
   `xs: [i64] @= [1]` then `xs.push(4)` gets a **certain** `=`; applied, the
   checker's `discarded_value` offers *assign it back to `xs`*; written,
   `not_mutable`. A `certain` removal of a parameter's `@` walks the same loop
   on p24. This is beside the question and is a finding for the coordinator:
   R1b's certain fix removes the very `@=` the dropped value's repair needs.

## 5. Cost, instructions retired (`/usr/bin/time -l`)

The machine carried other seats (load 5 to 15), so instructions, not seconds.

| run | trunk / control | route | delta |
|---|---|---|---|
| `check selfhost/main.hero`, 3 pairs | control 84.66 to 84.70 G | route v4 84.98 to 85.04 G | **+0.33 G, +0.39%** |
| cold `build` of p02 (7 lines), fresh directory, 3 pairs | 534.5 to 546.9 M | 534.0 to 534.8 M | within the spread |

Attribution, the same command: the control with only the `selfhost/` edits
equals the trunk (84.58 to 84.63 G against 84.55 to 84.63 G, three pairs); a variant that
calls `callee_params` and skips the scan (`heroes-ctl2`) is +0.27 G, so most of
the cost is the per-call lookup of the callee's parameters, which
`takes_at`'s existing per-call match (`built_marks.hero:104`) could share.
Merging the two is unrun.

## 6. The spec

§ 5 (`spec/heroes-spec.md:137-138`) does not cover it: *a cell nothing
re-binds* names cells, and *a write is not a use, except through an `@`
parameter* makes a parameter written and never read legal, the opposite
direction. The smallest word, applied to a copy and measured with
`heroes measure` (vendored, no `--refresh`): *so is a cell **or `@`
parameter** nothing re-binds* moves `maximum` **7636 to 7641 (+5)**,
`claude-legacy` 7502 to 7507. The `real` row is unrun (paid). Whether a reader
takes *re-binds* to include a handle a `consumes` call ends or a lend C writes
through is the spec-warden's to judge; § 13 already says both need `@`.

## 7. Verdict per route

- **R0, accepted as today: object.** 4 real sites in shipped code, and p20
  shows a false `@` making its caller hold a cell nothing else re-binds.
- **R1 as the brief words it: veto** (§1.12, §4.17). Its write list refuses
  13 correct programs, tells them a false message, and on p09 offers a fix
  that `check` accepts and `build` refuses.
- **R1 as built (eight writes, `guess` fix): approve**, with the
  fixedbugs-135 masking written down as the same class R1b has for cells.
- **R1 with the `certain` multi-site fix: object**: certain only where every
  edit lies in the root file, and even then it walks p23's loop on a dropped
  value; +60 to +90 lines unrun.
- **The `@_` parameter (p15)**: refuse it with the rule, about 10 lines, unbuilt.

## 8. Recommendation

Land the rule as built (`route.diff`): eight writes, `never_rebound`'s code,
the fix a `guess`, the three `selfhost/` edits and the example's one, the two
check cases rewritten, the § 5 word. Add the `@_` parameter in the same lane.
File as a defect the shared class *a dropped value of the place's own type is
a write the resolver cannot see* (p23 on the trunk, p24 on the route), whose
repair decides where both verdicts run.
