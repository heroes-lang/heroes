# Not one word for two opposite admissions

2026-09-20. M-declared-extents step 17, panel 170, the full panel: five seats and
a completeness critic, **the critic run before the synthesis this time**.

## The decision

| | |
|---|---|
| date | 2026-09-20 |
| decision | **the retention mark is REFUSED** on two vetoes; **`one_tag_one_type` is narrowed at `tag void`**, which closes defect 072 and is the only thing in the sitting that closes an open defect at `check` with the language it already has; **the default flip is queued rather than taken**, with its price measured at eight refusals of eight correct programs; defect 070's route is named |
| reason | the compiler-engineer: *`keeps`'s only honest repair is a lease, and on a parameter that FREES that repair IS defect 070. Retention must ADMIT a lease; give-away must REFUSE one. Opposite admissions, so not one word* |
| design.md § | §1.7, §4.17, §4.19, §1.6, §1.11 |
| panel | 170, provisional |

## Five seats, five inputs, one answer

Nothing like this has happened in the four sittings of this milestone: **every
seat came back against the proposal, and each from a different direction.**

- **The engineer vetoed** on the two opposite admissions, having run both halves.
- **The ffi seat vetoed** any mark naming an ending call, having written `keeps
  sqlite3_finalize` against the real `libsqlite3` in **both possible readings**
  against **all three modes** of argument five. **No reading is right for more
  than two of three, and the three are one declaration.** And the named call is
  the wrong call: SQLite disposes at the next bind, not the finalize, measured at
  `frees_before_finalize=999` over a thousand rows.
- **The warden objected** with the finding that would have made the mark close
  nothing: inside the swept `consumes|acquires|borrows` alternation it is refused
  on exactly the `cstr` and `ptr` types the defects live on. Its own grammar slot
  costs **+3 real** and is the cheapest correctness in its table.
- **The ergonomist vetoed the group form** on locality and objected to the
  parameter form with the sentence the sitting turns on: **the proposals do not
  add a check to the unmarked case, they subtract the warning from it.** Six of
  seven optional marks in § 13 have a compiler-enforced absence, so the document
  teaches that absence is the checked default.
- **The historian approved with two corrections**, and the first is the sitting's
  reorientation: **the lesson is the DEFAULT, not the mark.** Swift made
  non-escaping the native default and, in the same proposal, made the **foreign**
  default the opposite. Heroes proposed the optimistic default in the one region
  Swift refused it.

## And the answer to the symmetry question came twice, from isolated inputs

The brief asked whether `keeps` is symmetric with `owned`, `consumes` and
`acquires`, which are already the author's words. **The warden**: *the three
existing marks fail safe when omitted and `keeps` fails unsafe.* **The
ergonomist**, reading nothing but the spec, got there by counting the marks whose
absence the compiler checks.

## What actually closes something, and it is a narrowing rather than an addition

`one_tag_one_type` at `selfhost/check/decls.hero:313` — **not at
`selfhost/handles.hero`, where defect 072's entry pointed; the entry is
corrected.** The rule's own stated reason is written about `struct s *`, where
clang is a second judge of the spelling. **At `void *` no second judge can
exist**, so the rule buys nothing there and costs defect 072. Its own claim that
the mutant survives every instrument is **falsified by a run**: two distinct tags
give `error[type_mismatch]` at check, exit 1.

**A relaxation of a refusal, so CL-005's burden runs the other way.**

## The one thing the author owns, and it is priced

The historian's default flip is **the only candidate measured to close 066 and
068**, and it needs no author action on any correct program. Its price, measured
by the critic because no seat was assigned it: **eight lend call sites in
`examples/`, four files, all eight correct today.** Eight refusals of eight
correct programs.

**The recommendation is not to flip it yet**, and not because eight is many: the
sitting has no measurement of what the ninth costs, because the corpus is
`examples/` and `tests/golden/` and the author's own programs are not in it.
**What would settle it is one binding the author cares about, written both ways.**

## The emission divergence was a stale binary, twice in one sitting

A seat reported, as a question rather than a defect, that the compiler once
emitted the old gate line where minutes later it emitted the repaired one, and
that it could not reproduce it in twenty cold-cache emissions. **It preserved
both files, which is what made the answer findable.** The critic traced it: a
step-10 compiler emits one, step-12 emits the other, and **two seats shared one
scratchpad so one rebuilt the other's compiler underneath it**. Determinism holds
at one hash per twenty emissions. The compiler-engineer independently caught the
same trap against itself, six commits behind.

**It is filed rather than fixed**, because CLAUDE.md § 4 says the skills are
amended by author instruction and not by a panel.

## Defects filed

**073**, `owned <fn>` on an input parameter: grammar-admitted, prose-undefined,
`check` 0, `build` 2, and **two candidate routes wanted to build on that slot**.
A fourth give-away shape is appended to **070** rather than numbered, because a
repair is owed at the class.
