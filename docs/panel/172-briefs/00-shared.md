# Panel 172 — shared brief: the parameter C frees, and what reaches it

**Full panel**, five seats and a completeness critic. It gives a MEANING to a
word `spec § 13`'s grammar already has (`consumes`, on a `cstr` or `ptr`
parameter), adds a sentence to the prose, adds a refusal to the checker, and
may change what an UNMARKED pointer parameter admits. Surface semantics, a
diagnostic class and the spec move, so no lane is narrow enough.

**Every number below was produced by a command run on 2026-09-21 between 00:40
and 01:15, on commit `abf9a17e` (M-declared-extents step 24, the tree clean),
while this brief was written, and the command is beside it.** Re-run anything
you rest a verdict on. The reproducers are files beside this brief, in
`docs/panel/172-briefs/`, and `CPATH=docs/panel/172-briefs` makes the header
visible to a build.

## The defect this sitting is convened on

`docs/work/DEFECTS.md`, item **070**, the one open defect of the milestone: *a
lease handed to a C function that frees it dies with an empty stderr and an
unstable exit code.* Filed by panel 168's compiler-engineer, 2026-09-20.

    extern "giveaway.h"                  # static inline void eat(const char *s)
        function eat(s: cstr)            # { free((void *)(uintptr_t)s); }

    function main()
        x = "payload"
        c: cstr @ x.lease()
        eat(s: c)
        end_lease(@c)

Measured today with the shipped compiler (`CPATH=docs/panel/172-briefs ./heroes
check|build docs/panel/172-briefs/lease070.hero`, then ten runs of the binary):

| | |
|---|---|
| `check` | **exit 0**, no diagnostic |
| `build` | **exit 0** |
| ten runs, exit code / stderr bytes | **133/0 133/0 133/0 133/0 134/0 133/0 133/0 134/0 133/0 133/0** |
| `build --sanitize`, then run | exit 134, **2** AddressSanitizer lines: `attempting free on address which was not malloc()-ed` |
| the control, `control.hero`: the same lease, handed to `look(s: cstr lent)` which does not free, and never ended | exit 134, stderr **111** bytes: `panic: 1 lease(s) never ended — every .lease() owes one end_lease, and this program is missing that many` |

So the run-time instrument works and the corruption outruns it: C's `free` of
the runtime's block aborts inside the call, before `main` returns and before
the language's own report can print.

## What the flip of 2026-09-20 did to this defect, measured

Panel 171 landed `lent` and the pessimistic default (a C parameter is assumed
to KEEP what it is handed unless declared `lent`; a lend reaches only a `lent`
parameter of an `extern` function). Against the four shapes of 070:

| shape | file | today |
|---|---|---|
| a lease into an unmarked freer (the reproducer) | `lease070.hero` | **`check` 0**, dies as above — unchanged, because a lease is admitted at an unmarked parameter by design (that is what a lease is for: C keeps a copy) |
| a bare lend into an unmarked freer (070's fourth shape) | `lend070b.hero` | **`check` 1, `error[lend_kept]`** — closed by the flip, for the wrong reason: the message says C keeps, and C frees |
| a bare lend into a freer whose binding says `lent` (the word is false) | `lend070.hero` | **`check` 0**, run **exit 134, stderr 0 bytes**; `--sanitize` 4 lines, `DEADLYSIGNAL` |
| `consumes` written on the `cstr` parameter, as panel 170 proposed | `consumes070.hero` | **`check` 1, `error[unread_mark]`**: *`consumes` on `s` is a word nothing reads: `cstr` reaches no handle* — the word is REFUSED there today, so giving it a meaning is a real change, not a widening of silence |

`borrows` on a `cstr` is `unread_mark` the same way (`borrows070.hero` is not
kept beside this brief; the message is identical with the word swapped).

## What the language says today, verbatim

`spec/heroes-spec.md`, § 13, lines 370-387 at `abf9a17e`
(`sed -n '370,387p' spec/heroes-spec.md`):

> lives for its call and no longer: a parameter is taken to keep what it is
> handed unless declared `lent`, and a lend reaches only one so declared.
> `x: cstr @ s.lease()` is a COPY of the bytes that C may read for as long as the
> program says, and `end_lease(@x)` frees it and empties the cell. A lend and a
> lease name stand only as an argument of a call, nothing else writes a lease's
> cell, and a lease nobody ends, like a handle nobody consumes, aborts when
> `main` returns, saying how many.
> `owned sqlite3_free` after a `cstr` result or a `char **` out-parameter: the
> compiler frees that string with that function, hands it over as a `str?` (the
> `@` cell is only written), and refuses your own call of it. `consumes` after a
> parameter says the call ends that value's
> life, so passing one the function borrowed is an error: mark the parameter `@`
> and the value does not survive the call. `acquires sqlite3_finalize` after a result
> or `@` out-parameter reaching a handle says the call begins that handle's life and names
> the one that ends it, which the program owes it. The live handles are a set, so
> giving one back twice aborts on its own. `borrows` says the call hands
> back one it keeps, and where any `extern` consumes a handle type every call handing
> one back says which it is.

And the grammar, line 399-400: `CParam = [ "@" ] ident ":" Type [ "counted_by"
ident ] [ "lent" ] [ "owned" ident ] [ "consumes" | "acquires" ident |
"borrows" ] .` — so `consumes` on a `cstr` PARSES today and is refused one
stage later, by `selfhost/check/marks.hero` (115 lines; `marks_are_read` at
line 39, `refuse_unread` at 90, `unread_mark` at 103), whose rule is *the marks
are read on a handle and nowhere else*, because the live set keys on the
handle's DECLARATION and a `ptr` has none.

`docs/design/design.md:2333-2337` (`grep -n 'takes ownership' docs/design/design.md`):
*"A binding annotation vocabulary will eventually be needed … Three cases to
cover: a pointer you must free (and with which function), a borrowed pointer you
must not touch, and a buffer that C takes ownership of. … Reserve a keyword."*
The first two landed (`owned`, and the lend); **the third is this sitting's**.
Line 2340: *all three reserved cases are about a pointer C made*, and the
sitting's reproducer is about bytes HEROES made, handed to a C function that
frees them — so the third case as written is not quite this defect either, and
a seat that notices the gap should say so.

## What panel 170 settled about the repair, and panel 171 left

Panel 170 (`docs/panel/170-the-mark-is-refused-and-the-only-thing-that-closes-a-defect-today-is-a-narrowing.md`,
lines 246-250 and 278-280): *defect 070's answer is a MEANING for a word that
already parses — `borrows` and `consumes` both parse on a `cstr` today and are
thrown away by the handle-only sweep. Giving one of them a meaning there is
smaller than a new word and is the shape the engineer's veto points at:
give-away REFUSES a lease.* The engineer's veto ground, in the same file:
**retention must ADMIT a lease and give-away must REFUSE one**, so no single
word carries both. The spec-warden's finding there: **the three existing marks
fail safe when omitted, and this one fails unsafe** — a forgotten `owned` leaks,
a forgotten `acquires` leaks, a forgotten `consumes` on a freer is 070 itself.

Panel 171 (`docs/panel/171-lent-and-the-pointer-c-hands-back.md`, resolution
item 6): *defect 070 stays exactly where it was, measured: a lease into an
unmarked freer is `check` 0, run 133, empty stderr. Its fourth shape becomes
`lend_kept`.*

## The routes on the table, and the seats may add one

**Route A — the minimal meaning.** `consumes` on a `cstr` or `ptr` parameter of
an `extern` function says *C frees, or takes ownership of, what it is handed*.
The marks sweep admits it there (and only there: not `borrows`, not `acquires`,
not on a result). A lend expression and a lease cell's name are refused at such
a parameter, with a new diagnostic whose message says C frees and whose notes
say what may reach it: a pointer C handed back, `nullptr`, a value the program
allocated through the header's own allocator (`docs/measurements/037-…`'s R5).
Everything else about an unmarked parameter is unchanged: assumed to KEEP, so a
lend is refused and a lease is admitted. **The residual**: a freer whose binding
FORGETS the word is exactly 070 today — `check` 0 and silent death — which is
the warden's *fails unsafe when omitted*.

**Route B — the pessimistic default carried to freeing.** An unmarked pointer
parameter admits NEITHER a lend NOR a lease; only a pointer C made and `nullptr`
reach it. Three words then say what a parameter does: `lent` (reads while it
runs and lets go: a lend and a lease are admitted), a KEEPS word (keeps a
pointer past the call and never frees it: a lease is admitted, a lend refused —
spelling open; `borrows` is the candidate that already parses), and `consumes`
(frees: neither). Nothing is assumed, so a forgotten word refuses rather than
kills, which is the direction of the author's ruling; the cost is that every
binding must say a word on every pointer parameter it hands a lease to, and the
binding author's word is a claim the compiler cannot check either way.

**Route C — write the limit down.** No new meaning; `spec § 13` and the
defect say that a lease into a function that frees it is undefined and the
language does not see it. CL-005 then requires the sentence to name what would
make it wrong, and the defect stays open or is closed as a documented limit.

## The blast radius, counted

Lease sites in code (`grep -rn '\.lease()' examples tests/golden --include='*.hero'`,
comment lines removed by hand): **12 sites in 11 files** —
`examples/ledger/db/sqlite.hero:355` (`sqlite3_bind_text`, `SQLITE_TRANSIENT`),
`examples/gallery/13-lease.hero:23` (`keep_label`, C keeps),
`examples/curl/main.hero:77` (`curl_easy_setopt`),
`tests/golden/run/lease-tail-points-into-the-bytes.hero:22`,
`tests/golden/run/abort-lease-never-ended.hero:12`,
`tests/golden/run/lease-c-keeps-the-pointer.hero:19`,
`tests/golden/check/ffi-a-lend-parked-in-a-group-record.hero:36,48`,
`tests/golden/check/lease-cell-written-by-another-hand.hero:13`,
`tests/golden/check/lease-copied-out-of-its-cell.hero:19`,
`tests/golden/check/ffi-a-lend-reaches-only-a-lent-parameter.hero:40`,
`tests/golden/check/end-lease-takes-only-a-lease.hero:12`.
Under route B every one of them needs a word on the parameter it reaches, or
becomes a refusal; under route A none moves.

Raw-pointer parameters whose C frees, declared in the corpus
(`grep -rn 'function free(p: ptr)\|function my_own_free\|function destroy(handle: ptr)\|function sqlite3_free(p: ptr)' examples tests/golden`):
`free(p: ptr)` in **7** files (named as a freer by `owned`/`acquires`, or handed
as a disposer VALUE in `docs/measurements/037`'s R5; never called on a lease),
`sqlite3_free(p: ptr)` in 2 (declared and never called, the compiler calls it),
`my_own_free(p: ptr)` and `destroy(handle: ptr)` once each. **No shipped program
hands a lease to a function that frees it**; the defect is a shape a program
CAN write, not one the corpus writes.

**One golden pins today's refusal and flips under A or B**:
`tests/golden/check/unread-mark.hero:18` (`function free(p: ptr consumes)  #~
unread_mark`) and `:26` (`function strdup(s: cstr consumes) -> cstr  #~
unread_mark`). Both lines would become legal declarations — the first true,
the second FALSE (`strdup` does not free its argument), which is the
binding-author's-claim problem the historian found in D's WASI `putenv` at
panel 171.

## The budget

`spec/heroes-spec.md` today: **8216** real (`claude-opus-5`), **6172** vendored,
digest `2b1556634e73455a` — measured 2026-09-21 with `heroes measure
spec/heroes-spec.md --refresh` and pinned in `abf9a17e`. `DELTA_GATE` is 50
vendored. `.claude/rules/spec-shape.md`: **a vendored delta is not a price** —
the warden applies each draft to the real path in its copy, runs `--refresh`
(`. /Users/joseph/Temp/heroes/heroes-lang/.env` first, never print the key) and
reverts.

## Where the machinery is, for the seats that build

- `selfhost/check/marks.hero` (115 lines): the sweep that refuses `consumes` on
  a non-handle; the widening lands here.
- `selfhost/check/lend_landing.hero` (239 lines by `wc`): `landed_on_a_header`
  at line 164 asks the three questions of a LEND's landing — header, extent,
  `lent` — in order; a fourth, *does the parameter consume*, would go beside
  them, and `Landing` (line 118) would carry `consumes`.
- `selfhost/check/leasing.hero`: `lease_name_at` at line 91 judges a LEASE
  name's position; a consuming callee would be refused there.
- `selfhost/lend_errors.hero` (183 lines), `selfhost/lease_errors.hero` (73):
  where the new diagnostic's text would live; `layout`'s ceiling is 300 in its
  own unit.
- `tests/golden/check/unread-mark.hero`: the golden that moves.
- The compiler builds from the seed in about three seconds: `clang -I runtime
  seed/heroes.c runtime/runtime.c -o heroes`. A rebuild from `selfhost/` is a
  minute and a half and needs the seed compiler, which parses every word here.

## What is not on the ballot

The flip itself (panel 171, `provisional — author ratification pending`, landed
at `83a8c92c` and `abf9a17e`). The lease's existence and spelling (panels 124,
125). Whether a lend may reach an unmarked parameter (no, since `abf9a17e`).

## Procedure, and it binds every seat

- **Build in a copy.** `cp -r /Users/joseph/Temp/heroes/heroes-lang
  <your scratch>/<seat>-172 && cd <there> && rm -rf build && clang -I runtime
  seed/heroes.c runtime/runtime.c -o heroes`. Never edit the real tree. The one
  file you write in the real tree is your report.
- **Your report goes to `docs/panel/172-reports/<seat>.md`** in the real tree
  (the historian, who has no write tool, returns it as text and the coordinator
  writes it there verbatim). Every number in it is produced by a command you
  ran, and the command is beside the number (CL-077).
- **Negative sentences are run or go out as questions.** *The compiler cannot
  see X* is a claim about the option set; say what you searched.
- Verdict shape: verdict · section · cost/delta · one falsifiable prediction
  naming an instrument that exists today · condition. Name a veto as a veto.
- No timing claims: the machine is shared while the seats sit.
