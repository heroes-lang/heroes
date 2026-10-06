# Panel 194, shared brief: panel 178 sat again on the code of 2026-10-06

Convened 2026-10-06 by the author, meant as: *there have been a thousand changes
since that sitting, so the panel should be regenerated, or at least updated, and
then brought in, so we lose nothing*; and then: *let us open the next step*. The
task is `issues/2026-10/06/2026-10-06-1118-panel-178-reaches-the-trunk-sat-again-on-the-trunk-of-today.md`,
which also carries the author's own notes for the step, written from 178's
attempt. A full panel: the forms in question are surface syntax and § 13 text
(CLAUDE.md § 4).

**This brief was repaired after the completeness critic's first pass**
(`<scratchpad>/p194/reports/completeness-critic-briefs.md`, written 12:19 to
12:35), and **the seats start only once batch 12 is on the trunk**, as the
task asks (*sat again on the trunk as it is after batch 12*): the frozen tree
is batch 12's round once gated, **`7a26a0a6`** on `lane-panel-194` (the round at
`f0b4967b`, its seed regenerated and committed at `43e6be50`, its full net
green, with lane b12-ffi13's evidence for defect 092 at `c4b27fea` merged in);
every count below was re-run on it between 15:33 and 15:35 by the clock read before and after, and is the same unless it says
otherwise. **Every fact
below was produced by the command named beside it, run by the coordinator on
2026-10-06; a number from panel 178 is marked CARRIED, with its
date, and is a question for this sitting, never a premise** (CLAUDE.md § RUN
IT; `.claude/skills/panel/SKILL.md`, CL-077). The frozen tree is
the round with panel 178's two commits merged in (`d267b56a`, `e597cfcd`, by `3088caa7` on `lane-panel-194`),
so 178's sitting, briefs and reports are readable at `docs/panel/178-*` in your
copy, its records converted to today's shapes (its six milestone items are
issues naming M-buildable-structs: `grep -l 'milestone: M-buildable-structs' issues/*/*/*.md`).

## The question

**How does a program build a C struct whose fields include long arrays, without
writing every element, and without giving up what the language guarantees?** It
is row 63 of `docs/ROADMAP.md`, **M-buildable-structs**, `scheduled`. Panel 178
asked it on 2026-09-24 and adopted a provisional resolution nobody ratified,
on a branch never merged. **1,058 commits** separate 178's base from this tree (`git rev-list --count 517b8e25..7a26a0a6`).

## What panel 178 resolved (provisional, never ratified), to be judged again

Its nine points, in its own words in `docs/panel/178-...md` § The resolution:
R1 `rest: zero` ends a construction of a group record, every unnamed field zero,
`missing_fields` unchanged elsewhere, a golden for `missing_fields` first, the
three DECIDED rows named first, the construction check out of `check/walk.hero`,
the emission a compound literal; Z2 zero admitted on every group record as
bytes, Z1 refused; Z3 defect 094 repaired so a header's initialiser binds as a
group constant; L defect 091 repaired as a lowering; § 13 says C's `char` is
`i8`; T (text into a field) and A (`[x; N]`) wait behind measurements; R2, D and
a separate R0 refused; padding not promised. **Each point is judged again here
on today's code: kept, amended, or refused, with the measurement that decides.**

## What is measured today

- **The spec**: `heroes measure spec/heroes-spec.md`: real **9,518**
  (claude-opus-5, pinned 2026-10-06), headroom **722** against 10,240, of which
  the FFI floor mortgages 60 (panel 030 R3). § 13 (`sed -n 364,371p`): a group's
  record is the header's struct; a fixed array `i32[4]` is built with
  `[a, b, c, d]`, as many elements as the type says; a bit-field is left to
  `partial`. No `rest` anywhere (`grep -n rest`); **no sentence says C's `char`
  is `i8`**, so 178's point 5 never landed; § 13 now says `s.cstr()` aborts on a
  `str` holding a zero byte (lines 386-387), new since 178.
- **Layout, the instrument's unit** (`code_lines` and DECIDED in
  `tests/harness/suite_layout.hero`): `check/walk.hero` **1,858 of 1,870**,
  `print/fmt.hero` **1,096 of 1,175**, `ast.hero` **550 of 550, no line left**.
  CARRIED: 178's prototype moved all three rows (2026-09-24).
- **`missing_fields`**: emitted at `selfhost/check/walk.hero:1986`
  (`data_errors.missing_fields`, `selfhost/data_errors.hero:57`). 18 goldens
  under `tests/golden/unsupported/` name it for group records; **none under
  `tests/golden/check/`**, so no golden pins it on a Heroes record
  (`grep -rl missing_fields tests/golden`).
- **The header census**, 178's `census.sh` (35 headers; `docs/panel/178-briefs/census.sh`):

  | leg | array fields | longer than 8 | 64 or more | records with one longer than 8 | public |
  |---|---|---|---|---|---|
  | Darwin arm64, Apple clang 21.0.0, `-I/opt/homebrew/include` | 108 | 81 | 27 | 62 | **42** |
  | Linux arm64, Debian clang 22.1.8 (`heroes-linux-arm64`) | 77 | 50 | 14 | 34 | **29** |
  | Linux x86-64, Debian clang 22.1.8 (`heroes-linux`, amd64, no `--platform`) | 86 | 44 | 11 | 29 | **23** |

  Today's clang on this Mac does not search `/opt/homebrew/include` by default:
  without the flag OpenSSL's `sha.h` and `hmac.h` are missing; Homebrew's OpenSSL
  is 4.0.3. **The column mixes two rules** (the critic): 62 counts an
  anonymous record and 42 leaves it out; the no-flag count is 38 by 42's rule.
  Counted at the freeze by one rule: *public* is a named record whose name
  does not open with `_`; Darwin's 62 is 61 named and one anonymous. Every leg
  reproduces panel 178's table of 2026-09-24 but that one count.
- **Defects 091 to 094**, filed by 178 and never on the trunk, re-run on this
  code on 2026-10-06 from 11:37 by the agent that brought 178 in (its report,
  `<scratchpad>/p194/bring-178-in-report.md`; each issue file carries the line):
  **091 closed**, defect 097's repair of 2026-09-25 (a compiler from the seed at
  `9e17d471` runs it into *entered unreachable code*, one from `d64da8ff`
  prints `72`); **093 closed** into defect 245 (`ca5fa51e` prints the cut
  string at exit 0, `6b33db23` aborts at the `cstr`); **092 open, blocking**
  (a whole group record lent through `@` to a `void *` with `n: 4096` into 48
  bytes: run 138, ASan *stack-buffer-overflow, WRITE of size 4096*); **094
  open, blocking** (a struct-initialiser `constant` stops the build with
  *internal error*). Lane b12-ffi13 repairs 094 as a lowering in batch 12 and
  brings 092's routes to this sitting unlanded (below).
- **Changes since 178 in the files it measured** (`git log --oneline
  517b8e25..bef739dd -- <file> | wc -l`): the spec 11 commits, `check/walk.hero`
  19, `print/fmt.hero` 9, `ast.hero` 2, `emit/container.hero` 2 (`81532acc` and `d64da8ff`; batch 12's `f7576a01`,
  every array literal built in one block, rewrites that file on lane ir12 and
  reaches this tree with the batch), `emit/ctype.hero` 1.

## The routes on the table

178's: R1, R0, Z1, Z2, Z3, L, T, A, R2, D (each defined in 178's sitting). The
author's notes add, as questions: the three kinds of array (bytes the program
never touches, buffers C fills, bytes the program writes); zero as bytes, never
validity (a zeroed mutex locks on Linux and returns EINVAL on Darwin, CARRIED);
the header's own `*_INITIALIZER`; `memcpy` into `field.ptr()` declared
`ptr counted_by <n> lent` for text; a zero default for every type refused.
**And nothing**: the stopping rule's third shape, if today's forms already
compose to it. A route nobody listed is the critic's to name.

**A second question, defect 092's**, since it is the same boundary: what must
a binding say to lend a whole group record to a `void *` parameter whose
extent C is told by another argument? Today it says nothing, `check` is 0 and
C writes past the record. Lane b12-ffi13 builds the candidate routes (a run
time check of a named count against the record's size, as a field's
`counted_by` lend checks one past the field; a refusal of such a lend that
names no extent; a lend only through a declared `counted_by`) and counts the
bindings each would change; its evidence reaches the seats as
`<scratchpad>/batch12/ffi13/` when the lane reports, and the synthesis rules
on it.

## How the sitting runs

Each seat works in its own copy, `<scratchpad>/194-<seat>/`, made by
`git -C /Users/joseph/Temp/heroes/heroes-lang archive 7a26a0a6 | tar -x`, its
committed seed the round's own (SHA-256 `fc9751a29a1ecb8a`), and its compiler built
there:
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. Never another
seat's copy, never the repository. No timing: other work runs on this machine;
an instruction count is allowed. **Paid runs**: the blind seat's `claude -p`
sessions (approved by the author 2026-10-06) and the spec-warden's `heroes
measure --refresh` on a draft (approved the same day), each bounded in its
brief; no other. Write your report as you go in `<scratchpad>/p194/reports/<seat>.md`; the
coordinator copies it into `docs/panel/194-reports/` with the sitting.

## What the critic's first pass found, to be built on

- **Panel 186 (ratified 2026-10-02) is the base R1 stands on.** A construction
  of a group record that leaves fields out passes `check` since that sitting
  (`selfhost/check/walk.hero:1772-1778`); `build` judges it from clang's layout
  and tells it at `selfhost/emit/ffi_built.hero:160`. So `missing_fields` at
  `walk.hero:1986` serves Heroes records alone, and 178's condition to move the
  construction check out of `walk.hero` is half met already. **R1 must say what
  `rest: zero` does to a union**, which 178's sentence does not.
- **R1 changes no emission.** A group record's construction is already a C
  compound literal with named fields, `(struct utsname){.sysname = {...}}`,
  which C fills with zeros; the one-field program emits 1,160 lines today
  (178's 5,555 is CARRIED and stale).
- **178's R1 diff no longer applies**: 14 of its 36 hunks fail on this code.
- **Principle 0's count reads 0**: no `.hero` binding outside `tests/`,
  `docs/` and `archive/` constructs a group record holding an array longer
  than 8 (178's lapse condition was *fewer than 3*). Whether a scheduled
  milestone's need (M-core-packages, M-buildable-structs itself) counts is the
  spec-warden's ruling to make, measured, never assumed.
- **Routes nobody listed**: the `ffi_field_type` note, or a `certain` fix,
  naming `i8` for a C `char` field, at 0 spec tokens (today's note does not);
  R1 built as a relaxation of 186's `build` marker rather than 178's
  `check`-side prototype; `fixed_array_length` offering `rest: zero` in its
  message; and waiting.
- **Defect 092 is wider than `void *`**: a 16-byte `Sockaddr` lent to `connect`
  with `len: 106` passes `check`, runs at exit 0, and `--sanitize` is silent
  (the over-read itself an inference, unproven).
- **The zeroed mutex holds today** (the critic, re-run): Darwin 22/22,
  aarch64 0/0, x86_64 0/0.
- **A brace list has no type** (lane b12-ffi13, 2026-10-06, after its repair of
  094): `PTHREAD_COND_INITIALIZER` bound as a mutex record's constant still
  builds and runs (prints 1018212795, panel 178's critic's program); C cannot
  say which struct a brace list was written for, and the enum and pointer-sign
  errors the repair turns on catch some such values, not all. Z3's question.
- **Defect 092's cause is not `void *`** (lane b12-ffi13): a typed struct
  pointer with a byte count crashes (133), and `poll`'s element count with 512
  elements into one record overwrites the stack silently at exit 0; a route
  keyed on `void *` leaves both open. Its routes and counts:
  `docs/panel/194-evidence/092-routes.md` when the lane commits it.

