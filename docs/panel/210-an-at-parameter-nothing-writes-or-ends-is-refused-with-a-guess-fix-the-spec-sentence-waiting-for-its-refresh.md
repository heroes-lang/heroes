# Panel 210: an `@` parameter nothing writes or ends is refused with a guess fix, the spec sentence waiting for its refresh

Convened 2026-10-11 at about 02:10 by the coordinator (session heroes-lang-98)
for the open decision `issues/2026-10/10/2026-10-10-1737-an-at-parameter-the-callee-never-writes-is-accepted.md`
(panel 209's completeness critic, second pass), on the author's choice at
about 02:07 in the question widget (*Seduta 210 stanotte*). **A lean
sitting**: the compiler-engineer, the spec-warden and the historian, one
completeness critic pass after the reports, no blind seat and no paid run.
The tree frozen at **`1dd890751`** (batch 20's round), each seat in its own
copy under `.claude/worktrees/scratch-b15/210-<seat>/`. The historian's report
written by about 02:2x (no shell, no clock), the spec-warden's by 02:26 by its
`date`, the compiler-engineer's by about 02:50; the critic from about 02:51 to
03:04; this synthesis from 03:05, every time read from `date` but the
historian's. What the lean sitting gave up: a measurement of what a reader of
the spec expects.

## The verdict table

| seat | verdict | on what |
|---|---|---|
| compiler-engineer | **veto** the rule as the decision words it; **approve** the rule built with eight writes and a `guess` fix | `route.diff`, +154/-14 in `selfhost/resolve/`; the census over 2,035 roots: 15 sites in 3 files, 4 in shipped code (three of the compiler's own, `examples/ledger/db/sqlite.hero:322`), 14 in two golden cases; the brief's four writes refuse 13 correct programs, helpers handing a handle to a `consumes` or `transfers` C parameter, where § 13 requires the `@`; `check selfhost/main.hero` +0.39% instructions |
| spec-warden | **veto** the rule as worded (Principle 0: 39% of what it flags is correct code); **approve, provisional**, the corrected rule with D2c in § 5 | its own probe rule (33 sites, the same 13 false); `heroes measure`: D2c +14 cl100k, +13 legacy; § 5 today derives acceptance (*a read is a use*), so refusing needs a word; both repairs offered, each a `guess`, since removing the `@` keeps the bug of a copy changed and never written back |
| historian (advisory) | **approve** | every language that tried stumbled on a signature forced from outside (function values, generics, traits, protocols) or on closures and FFI (Rust's `needless_pass_by_ref_mut`, moved to `nursery` within one release; GNAT `-gnatwk`; Swift's open SR-9883); Heroes excludes the first three by construction, leaving the C boundary and public functions |
| critic (one pass) | no verdict | the built rule refuses a correct program at the C boundary through the UFCS receiver (`n.node_free()`), and the trunk accepts the result of its guess (`consumed_borrowed_handle` only on the named-call path, `check/walk.hero:1202`): a trunk defect whatever the sitting decides; on the one bug-shaped case (p12) the message prescribes the repair that keeps the bug; D2c is true of the built rule, the +5 sentence false on the 13 consuming helpers; `function f(@_: i64)` accepted and it lets a caller's cell escape panel 209's rule; panel 209's `certain` `=` leads away from the repair on `xs @= [1]` then `xs.push(4)`; an unlisted route, a value line rooted at the local counted as a meant write, closes that trap |

## What the sitting measured

- **The class is real and small**: 15 sites in 3 files by the built rule, 4
  of them in shipped code, harmless extra `@`s; the 8 in `fixedbugs-135` are
  dropped writes, the one shape that is a bug.
- **What counts as a write** (the built rule's eight, the critic's
  additions): `@` to the name, a field or an element; an `@` argument; a
  `.ptr()` lend for C to write; a `consumes` or `transfers` argument, **by a
  named call and by the UFCS receiver** (the critic's finding); an `@` the
  checker will add or refuse; a dotted call whose first parameter is `@`.
- **A function with an `@` parameter cannot be a value**
  (`mutable_parameter_as_value`, the critic, measured): the historian's main
  false-positive class does not exist here.
- **The fix cannot be `certain`**: removing the `@` keeps the bug of a copy
  changed and never written back (p12 prints 0 where writing back prints 1);
  `check --apply` writes the root file only (defect 607). Both repairs are
  offered, each a `guess`.
- **The spec**: § 5 (`:137-138`) today derives acceptance; D2c, *... or an
  `@` parameter nothing writes or ends (section 13), an `@` argument
  counting*, is true of the rule once the UFCS receiver is counted and `@_` is
  refused; +14 cl100k and +13 legacy vendored, the real count unrun (a
  `--refresh` is a paid run, which this night does not make).

## Disagreements, stated plainly

- **The write list**: the compiler-engineer's eight against the spec-warden's
  list; they agree on every correct program of the census and differ only on
  programs already refused (the critic). The resolution takes the eight plus
  the UFCS receiver, and the critic's line-rooted meant write where it builds.
- **The sentence**: D2c against the compiler-engineer's +5 *a cell or `@`
  parameter nothing re-binds*, false of his own rule on the consuming helpers
  (a `consumes` call is not a re-binding in § 5's words). The resolution
  takes D2c.

## The resolution, provisional (author ratification pending)

The most robust and complete route at every question (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, an `@` parameter nothing writes or ends is refused**, code
   `never_rebound`, at the parameter, with both repairs offered as `guess`
   fixes (remove the `@` here and at every call; or write the value back),
   counting as writes the built rule's eight and the UFCS receiver of a
   `consumes` or `transfers` call, and the critic's line-rooted meant write
   where it builds clean.
2. **R2, `function f(@_: i64)` is refused** in the same lane, defect 608's
   twin, so no caller's cell escapes panel 209's rule through it (filed as
   defect 631).
3. **R3, spec § 5 takes D2c**; the real count by one `--refresh` at the
   landing, **on the author's yes in the morning**, since this night makes no
   paid run. **R1 and R2 land with R3, not before it**: a rule the spec
   contradicts is a false spec, so the landing waits for the refresh.
4. **R4, the trunk defects the sitting found, filed now whatever R1 to R3
   become**: the UFCS receiver of a `consumes` call not judged by
   `consumed_borrowed_handle` (defect 629, `blocking`), and panel 209's
   `certain` `=` leading away from the repair on a discarded method call
   (defect 630, `adjacent`, the critic's class).
5. **R5, refused**: the rule as the decision worded it (13 correct programs
   refused, two vetoes); a `certain` fix at the signature and every call; the
   +5 sentence (false of the rule); a warning (this language has none).

**The conservative alternative, the author's to choose instead**: accept, as
today, the decision closed with no rule, the trunk defects of R4 filed all
the same.

## Process notes

- The historian has no shell and could not read the clock; it left a stray
  12-byte file in `210-reports/`, removed by the coordinator at its request.
- The open decision of 2026-10-10 closes with this synthesis, its question
  carried by this sitting's ratification.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | at the landing's gate the census over the same 2,035 roots names exactly 15 sites in 3 files after the three compiler edits, none a parameter a `consumes`/`transfers` call ends or a `.ptr()` lend hands to C; `check selfhost/main.hero` under +0.5% instructions | the landing |
| spec-warden | P2: `p12_shadow_copy.hero` gets exactly one error, at the parameter, with no `certain` fix; P3: D2c's real delta at the landing's `--refresh` between +9 and +20 | the landing |
| historian | if R1 lands, no false-positive defect against it in its first two milestones involves closures, traits, async code or function values | two milestones |

## Author's verdict

(pending)
