# Panel 167 — shared brief: the lend has no lifetime rule

Convened 2026-09-19, at M-declared-extents, on **defect 066**, which panel 166
filed and explicitly did not price: *"its two routes are the shape three seats
vetoed for the write direction, and pricing them is its own sitting."*

**Every number, path and quotation below was produced by a command run while
this brief was being written, on `arm64-apple-darwin25.6.0`, against the tree at
M-declared-extents step 6 — with routes H and C both landed.** Where a claim
could not be run it says *unrun* in those words. The commands are at the foot.

## The question

**How long is a lent address valid, and what stops C from keeping it?**

`f.ptr()` hands C the address of a fixed byte field. Panel 164 opened that route
and carried across no lifetime rule; panels 165 and 166 answered *how far* C may
read (route C, `counted_by`) and *whether* C may write (route H, the header's
`const`). Nothing answers *for how long*.

## The defect, reproduced on today's tree — not on the tree that filed it

This ran **after** both repairs landed, so neither closes it. The lend is at a
`const void *` parameter declared `counted_by n`: every rule the language has
about lends is satisfied.

```
function stash()
    s = k_make()
    k_register(p: s.name.ptr(), n: 8)    # C parks the address in a static

function main()
    stash()                              # the frame dies here
    print(noise())                       # 10   — the frame is reused
    print(k_read_later())                # 0    — the honest answer is 72
```

```
$ ./heroes run keep.hero                 ->  10 / 0,   exit 0, no diagnostic
$ ./heroes run keep.hero --sanitize
==98607==ERROR: AddressSanitizer: stack-use-after-scope on address 0x00016f8c6480
SUMMARY: AddressSanitizer: stack-use-after-scope keep.hero:17 in h_keep_main
```

## What the compiler already refuses, and why none of it reaches this

Four clauses, all on the **Heroes** side of the boundary:

| clause | what it refuses |
|---|---|
| `field_lend_escapes` | the lend bound to a name instead of passed |
| `field_lend_needs_a_place` | a lend rooted at a value nothing holds |
| `field_lend_needs_a_header` | a lend handed to a function of this program |
| `field_lend_uncounted` | a lend at a parameter declaring no extent |
| `field_lend_written` | a write through a lend from a non-`@` root (clang's) |

**The escape that happens at an FFI boundary is on the C side**, and nothing
looks at it. Every registration API in C keeps the pointer.

## The shape with a real library behind it, in this repository

`examples/ledger/db/sqlite.hero:107`, read today:

```
function sqlite3_bind_text(statement: CStmt, column: i32, text: cstr, length: i32, destructor: ptr) -> i64
```

**And the retention is not in the header.** `sqlite3_bind_text`'s fifth
argument decides it — `SQLITE_STATIC` means *I keep your pointer*,
`SQLITE_TRANSIENT` means *I copy it*. Panel 124 measured the same shape for
`cstr` and recorded the number: **0 of 71 `cstr` parameters in this tree are
decidable from a header.** So route H's mechanism — ask clang, the header knows
— has no counterpart here. Whatever answers this must be a word the binding
author writes, or a copy.

## What the language already owns, and it is the answer's shape

`spec/heroes-spec.md:370-374`, quoted verbatim:

> `x: cstr @ s.lease()` is a COPY of the bytes that C may read for as long as
> the program says, and `end_lease(@x)` frees it and empties the cell. A lend
> and a lease name stand only as an argument of a call, nothing else writes a
> lease's cell, and a lease nobody ends, like a handle nobody consumes, aborts
> when `main` returns, saying how many.

The `cstr` direction has **two** forms, a lend and a lease: one for the call's
length, one for as long as the program says. The field direction has **one**,
the lend. That asymmetry is the defect stated as a design fact, and panel 124
built the lease for exactly this reason on the other side.

## The corpus this binds, counted today

`grep -rc '\.ptr()' --include='*.hero' tests/golden examples` — **eight files,
33 occurrences, all under `tests/golden/`, and zero in `examples/`.** The four
that are call sites in a running program are
`tests/golden/run/ffi-a-byte-field-crosses-to-c.hero`. **No shipped example
lends a field at all**, so a rule here costs the corpus what the migration of
those goldens costs and nothing more.

## The budget, measured today

`./heroes measure spec/heroes-spec.md`: **8214 against the 10240 ceiling with
the FFI floor's 60 mortgaged, 2086 free.** The spec moved this morning, +48
real, for route C. §1.6's payment rule is unconditional at every level.

## The file sizes a route lands in, by `wc -l` (the suite counts narrower)

`check/lending.hero` 357, `check/lend_extent.hero` 304, `lend_errors.hero` 275,
`emit/field_lend.hero` 245, `check/leasing.hero` 245. **`layout` is green today
at 2 passed, 0 failed**, so every one of these is inside its decided ceiling in
the unit that judges it; `wc -l` is the coarser number and is given only to
rank them.

## Four routes, and the sitting is asked for the ones nobody listed

- **A — the field lease.** A second form beside the lend, the `cstr` lease's
  shape applied to a field: it COPIES the bytes somewhere the program owns,
  hands C that, and the program ends it. Closes 066 by construction, since C
  never sees a frame. Costs a built-in, a release, spec text, and the abort
  the lease already has.
- **B — a mark the binding author writes.** `keeps` (or the like) on a `ptr`
  parameter, saying C retains the address past the call; a lend may not reach
  one, and only a copy may. Declaration-site, the shape `consumes`/`acquires`
  already have — and the author's word rather than the header's, which is the
  fact above.
- **C — write the hole into the specification.** *The address dies with the
  frame; a C function that keeps it is on its own.* **Three seats vetoed this
  shape at panel 166** for the write direction, on soundness and locality.
- **D — withdraw the field lend.** Refused at panel 166 as route F, priced at
  −44 real, on §1.12's completeness clause.

**A route nobody has listed is worth more than a verdict on these four.**

## Commands, so every number above can be re-run

```sh
./heroes run keep.hero                 # 10 / 0, exit 0
./heroes run keep.hero --sanitize      # stack-use-after-scope
grep -rc '\.ptr()' --include='*.hero' tests/golden examples
sed -n 105,110p examples/ledger/db/sqlite.hero
sed -n 370,374p spec/heroes-spec.md
./heroes measure spec/heroes-spec.md
./heroes run tests/harness/main.hero -- ./heroes layout
```
