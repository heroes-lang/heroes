# Panel 172 — compiler-engineer

**verdict**: object (not a veto)
**section**: design.md §1.1 ("Implementation simplicity is a constraint, not a
goal. It sets the ceiling", line 174ff) read against §1.12 (line 571) and §1.7
(line 410).
**needed for self-hosting (Principle 0)**: **no**. `grep -rn '\.lease()'
selfhost --include='*.hero'` returns **18** hits and every one is a comment or a
test-string literal (the output is in § What moved); the compiler hands no lease
and declares no consuming pointer parameter.

## The objection, in one measurement

I built route A. It works, it is cheap, every suite is green — and it does not
close the defect this sitting was convened on.

    for f in lease070 consumes070 lend070 lend070b control; do
      CPATH=docs/panel/172-briefs ./heroes      check docs/panel/172-briefs/$f.hero   # control
      CPATH=docs/panel/172-briefs ./heroes-next check docs/panel/172-briefs/$f.hero   # route A
    done

| shape | control `./heroes` | route A `./heroes-next` |
|---|---|---|
| `lease070.hero` — **the filed reproducer**, a lease into an UNMARKED freer | exit 0 | **exit 0, unchanged** |
| `lend070.hero` — a freer whose binding falsely says `lent` | exit 0 | **exit 0, unchanged** |
| `consumes070.hero` — the word written | exit 1 `error[unread_mark]` | exit 1 `error[consumed_by_c]` |
| `lend070b.hero` — a bare lend into an unmarked freer | exit 1 `error[lend_kept]` | exit 1 `error[lend_kept]`, unchanged |

Of the four shapes of defect 070, route A moves **one**, and it is the one that
**already failed to compile**. The two that reach `main` and die with an empty
stderr — `lease070.hero` and `lend070.hero`, both exit 0 at `check` — are
untouched. Route A is 96 compiler lines that change the TEXT of a refusal on a
program clang never saw, and change nothing about the program that corrupts
memory. That is the trade §1.1 asks about: the ceiling is spent and §1.12 is
where it was.

`lease070.hero` is unchanged for a reason that is structural, not a prototype
gap: its declaration is `function eat(s: cstr)`, with no word at all. Route A
reads a word. A defect whose reproducer writes no word cannot be closed by
giving a word a meaning.

## Cost, with file citations

`git diff --numstat` in my copy, after `./heroes fmt <file> --in-place` on all
five (formatting was already stable — the diff did not move):

| file | now (`wc -l`) | added / removed |
|---|---|---|
| `selfhost/check/leasing.hero` | 283 | +24 / 0 |
| `selfhost/check/lend_landing.hero` | 263 | +31 / -6 |
| `selfhost/lend_errors.hero` | 206 | +21 / 0 |
| `selfhost/check/marks.hero` | 133 | +18 / 0 |
| `selfhost/check/lend_extent.hero` | 201 | +2 / 0 |
| `tests/golden/check/unread-mark.hero` + `.expected` | | +2 / -4 |

**96 insertions, 6 deletions in the compiler, across five files, one new
diagnostic (`consumed_by_c`).** No lexer change, no parser change, no
descriptor change, no ownership change, no emitter change — the word already
parses (`spec § 13` grammar line 399). It is **sugar by §1.7**: one checker
clause per landing site plus one diagnostic constructor, erased before lowering.
It fits one person; it is nowhere near the Pascal-P4 scale argument.

**Layout**, my own re-implementation of `code_lines` from
`tests/harness/suite_layout.hero:520ff` (everything outside a `test` block that
is not blank), ceiling 300 at `suite_layout.hero:43`, and **no allowance row
names any of these files** (`grep -n 'lend_landing\|leasing\|marks\|lend_errors\|
lend_extent' tests/harness/suite_layout.hero` returns one unrelated line, 206):

    marks.hero        105 -> 121      lend_landing.hero 171 -> 189
    leasing.hero      202 -> 218      lend_errors.hero  176 -> 196
    lend_extent.hero  161 -> 163

All five under 300, largest 218. `suite_layout` reads **2 passed, 0 failed**.
Layout is not the constraint here and I will not pretend it is.

## The blast radius route A actually has, and it is a fourth file

`selfhost/check/consuming.hero:45`, `consuming_positions`, reads `p.consumes`
with **no type test and no extern test**. Until today `marks.hero` guaranteed
that only a handle-typed parameter could carry the word, so `refuse_borrowed`
could only ever see handles — which is why its code is spelled
`consumed_borrowed_handle` (`consuming.hero:119`). Route A makes the word legal
on a `cstr`, and that rule starts deciding `cstr` programs.

Measured, on `p3-wrapper.hero` (a Heroes `wrap(s: cstr)` handing `s` to
`eat(s: cstr consumes)`):

    control  exit 1: consumed_borrowed_handle + lend_needs_a_header + unread_mark
    route A  exit 1: consumed_borrowed_handle + lend_needs_a_header

So the firing is not new — it was already there, standing behind a dead
declaration. What IS new is that it becomes **load-bearing**, with a code word,
`handle`, that is now false for the value it refuses. A diagnostic class is
panel territory (CLAUDE.md §4) and the brief named three files; the fourth is
this one. Its repair note also sends the reader somewhere the compiler refuses:
`p9-at-repair.hero`, which takes the note's advice (`wrap(@s: cstr)`,
`wrap(@c)`), is `error[lease_written]` under route A.

## The questions, each with its command

**1. Which expressions produce a Heroes-made `cstr` or `ptr`?** From
`selfhost/check/lend_types.hero:34-95` (`lend_call`, the one place these three
types are minted) plus `selfhost/check/lending.hero:84`:

- `s.cstr()` — the lend, `.cstr` (`lend_types.hero:36`)
- `s.lease()` — the lease, `.cstr`, same arm (`lend_types.hero:34`)
- `f.ptr()` — a fixed byte run's address, `.ptr` (`lend_types.hero:89`)

and **nothing else**. `validated_bytes` is in the same function and answers
`str?`, never a `cstr` (`lend_types.hero:68-72`) — it COPIES into the language,
so it is the opposite direction and not in the set. `nullptr` is `.null_ptr`,
neither made. Every other value of type `cstr` or `ptr` is a C result, an `@out`
cell C filled, or a field of an `extern` record — C's, and C's to free.

**The shapes beside it (CL-061), each run under route A:**

| shape | file | route A |
|---|---|---|
| a name bound to a lend, then consumed | `p1-name-bound-to-lend.hero` | `error[cstr_escapes]` — refused already |
| a lease copied out of its cell, then consumed | `p2-lease-copied.hero` | `error[lease_escapes]` — refused already |
| a lend through a Heroes wrapper | `p3-wrapper.hero` | `lend_needs_a_header` + `consumed_borrowed_handle` |
| a lend direct to a consuming parameter | `p5-direct-lend.hero` | `error[consumed_by_c]` — the new one |
| a lease direct to a consuming parameter | `p6-direct-lease.hero` | `error[consumed_by_c]` — the new one |

A group record's `cstr` field and an `@out: cstr` C filled: the group-record
lend is `tests/golden/check/ffi-a-lend-parked-in-a-group-record.hero` and it is
in the green `check` run below; I did **not** write a probe that parks a lend in
a group record and then hands the record's field to a consuming parameter, so
that shape is **UNRUN** and I will not claim it.

**Is refusing a lend and a lease at a consuming parameter sufficient for
soundness of the word?** For the three minted forms and their two escape
shapes, yes — those five are all refused (table above), and all three escapes
were already refused before this sitting. The honest statement is narrower than
"sufficient": it is *sufficient given that `lend_types.hero:34-95` is the only
mint*, and that premise is one grep away from expiring the day a fourth arm is
added there.

**2. The double give-away.** Not this sitting's, and the control says why.

    p10-unmarked-double-free.hero:  extern make() -> ptr; extern release(p: ptr)
                                    p = make(); release(p: p); release(p: p)
    control ./heroes      exit 0
    route A ./heroes-next exit 0

A double free through a C-made pointer is **already writable today with no new
word**, and route A neither opens it nor closes it. (`p7-c-made-twice.hero`, the
same with `consumes`, is control exit 1 `unread_mark`, route A exit 0 — so the
word makes ONE spelling of it newly legal, but the shape beside it was legal all
along, so this is not a regression the sitting causes.) The handle live set
(`runtime/parts/alloc.c`) keys on a handle's DECLARATION; a raw `ptr` has none,
which is `marks.hero`'s founding rule and is why panel 139's answer stands.
**Not this sitting's.**

**3. The residual, and can the runtime catch it?** The residual is
`lease070.hero`, measured above: exit 0, unchanged — **defect 070 as filed
survives route A**. The runtime cannot catch it, and the repository already says
so in the code, `runtime/parts/str.c:430-444`, panel 125's own comment:

> a double release cannot be told from a never-held pointer by any such read,
> because the block is gone … "already released", from a zeroed magic in freed
> memory; five runs printed the wrong message five times, because freed memory
> owes nobody its contents.

The **leading** header does not help either, for a second and independent
reason readable at `str.c:419-428`: `hero_str_held` allocates
`sizeof(HeroHeldHeader) + len + 1` and hands back `(char *)(h + 1)`. The pointer
C receives is **not the malloc base**, so C's `free` on it is a free of an
interior address — which is exactly the shared brief's ASan line, *attempting
free on address which was not malloc()-ed*, and it aborts INSIDE the call.
Control never returns to the runtime, so there is nothing to detect with. I did
not re-run the ten-run exit-code table; that number is the brief's, **UNRUN by
me**.

**4. Route B's radius, by grep.** `grep -rn '\.lease()' examples tests/golden
--include='*.hero'` reproduces the brief's **12 sites**. I then grepped each
file's `extern` block for the parameter each lease reaches:

    sqlite3_bind_text(text: cstr)   keep_label(s: cstr)   curl_easy_setopt(value: cstr)
    strlen(s: cstr)                 stash_put(s: cstr)    after_dash(s: cstr, @tail: cstr)

**Six distinct callees, not one of them marked** — no `lent`, no anything. So
B's refusal count is every corpus lease site that reaches these six; the
remaining `tests/golden/check/` callees I did not open, **UNRUN**. Of the six, at
least two could NOT truthfully say a keeps word: `strlen` reads and returns, and
`sqlite3_bind_text` at `examples/ledger/db/sqlite.hero:355` is called with
`SQLITE_TRANSIENT`, which makes SQLite copy. Under B those two would be rewritten
to `lent` — which means B does not merely add words, it reveals that two shipped
sites use a lease where a lend would do. That is the ffi-pragmatist's finding to
price, not mine, but it is measured and it is on B's side of the ledger, not
against it.

**5. `lent consumes`, the pair the coordinator asked about.** My prototype
refuses it **twice**: `p4-lent-consumes.hero` is `error[lent_shape]` at the
declaration (`selfhost/check/lend_extent.hero`, `declared_lends`, the two lines
in my diff) **and** `error[consumed_by_c]` at the landing. Control today is
`error[unread_mark]` only. **It should be `lent_shape` alone.** The contradiction
is in the DECLARATION — one of the two words is false on the header — and a
reader who fixes the declaration sees the second message vanish; reporting the
use site as well is two messages for one mistake, which is the shape
`.claude/rules/diagnostics-and-goldens.md` exists to prevent. The landing check
should stay in the code as the loud fallback (the prototype's own comment says
this) but be ordered so only one diagnostic reaches the reader. That is a
correction to my own prototype, not to the route.

Where in the order: the prototype asks `consumes` **second**, after the header
and before the extent. That is right and I would keep it. Sending a reader to
write `counted_by` or `lent` on a freer is sending them to make a false
declaration truer.

## What I built and what moved

    cd <scratch>/compiler-engineer-172
    ./heroes fmt selfhost/check/{marks,lend_landing,leasing,lend_extent}.hero --in-place
    ./heroes fmt selfhost/lend_errors.hero --in-place
    ./heroes build selfhost/main.hero -o heroes-next          # wrote heroes-next

| gate | command | result |
|---|---|---|
| the compiler's own tests | `./heroes-next test selfhost/main.hero` | **676 tests, all passed** |
| check | `./heroes run tests/harness/main.hero -- ./heroes-next check` | **135 passed, 0 failed** |
| annotations | same, `annotations` | **174 passed, 0 failed** |
| layout | same, `layout` | **2 passed, 0 failed** |
| canonical | same, `canonical` | **2 passed, 0 failed** |
| run | same, `run` | **132 passed, 0 failed** |

**Exactly one golden moved**: `tests/golden/check/unread-mark.hero`, lines 18 and
26, both losing their `#~ unread_mark` annotation, and two lines out of
`unread-mark.expected`. Line 18 (`function free(p: ptr consumes)`) becomes a
TRUE declaration; line 26 (`function strdup(s: cstr consumes) -> cstr`) becomes
a legal and **false** one — `strdup` does not free its argument, and nothing in
the language or in these 96 lines can say so. That golden is now a file that
teaches a wrong binding as legal, and whichever route lands owes it a rename or
a second case that says out loud which of the two is a lie. No timing claims:
the machine is shared.

## Prediction (falsifiable, named instrument, named milestone)

At the close of the milestone that lands route A as briefed, with the tagged
compiler:

    CPATH=docs/panel/172-briefs ./heroes check docs/panel/172-briefs/lease070.hero

**will exit 0**, and `docs/work/DEFECTS.md` will still carry item 070 open — or
will carry it closed with a body that redefines it, since the program that
provoked it still compiles. Falsified if that command exits non-zero, which
requires the resolution to refuse a lease at an **unmarked** pointer parameter,
which route A as briefed explicitly does not do.

## Condition that changes my verdict to approve

Either of these, and the first is cheap:

1. The resolution pairs route A with **B's leasing half only** — a lease name
   refused at an unmarked pointer parameter of an `extern` function, the lend
   rule left exactly as panel 171 left it. My prototype shows where it goes:
   one more branch in the clause I already added at
   `selfhost/check/leasing.hero`, reading `!landed.lent && !landed.consumes`
   instead of `landed.consumes`. Cost: I estimate under 20 further lines in that
   one file — **an estimate, not a measurement; I did not build it**. Price:
   the 12 corpus sites above need a word.
2. Or defect 070 is re-filed, in `docs/work/DEFECTS.md`, as *a binding that
   WRITES `consumes` and is then handed language-owned bytes* — the shape route
   A does close — and the shape it does not close (`lease070.hero`, no word at
   all) is opened as its own numbered defect rather than left inside a ticked
   one. CL-078 is the rule: the defect is what the finder saw, and the shapes
   beside it are where what it actually is becomes visible.

**No veto.** Route A adds no core construct, touches no lexer, lowering or
emitter, and breaches no ceiling: 96 lines, five files, largest file 218 of 300
in `suite_layout`'s own unit. It is sound — it refuses strictly more than today
and every suite is green. My objection is that it is sold as the repair of 070
and, measured, it is not.
