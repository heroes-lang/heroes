# Panel 169 — shared brief: what closes defects 066 and 068

**Full panel**, five seats and a completeness critic. It changes what the checker
refuses and may change `spec § 13`, so the soundness lane is not enough.

**Every number below was produced by a command run while this brief was being
written, on 2026-09-20, on Darwin arm64**, and the command is named beside it.
Nothing is copied from `docs/`, from an earlier sitting, or from a defect entry.
**Re-run anything you rest a verdict on.** The coordinator's brief at panel 168
was wrong twice and both were caught by seats; one of the two was a **scope
sentence** rather than a number, and it cost that sitting its whole subject.

## The question

**What closes defect 066 and what closes defect 068?** Not *which route is
cheapest*, and not *is route A sound*: panel 168 settled that route A is sound
and closes **zero of two**.

## The state, as of this brief

`docs/work/DEFECTS.md` reads `**OPEN: 4**`. CLAUDE.md § Verification: a milestone
is tagged only over a clean list, so **M-declared-extents cannot close until
these are answered**.

- **066** — nothing in the language relates C's retention to the moment the
  bytes die. Corrected twice in one day: from *field lend* to *lend*, then to
  **lend AND lease**.
- **068** — a record rewritten while C holds a field's address. A wrong answer at
  exit 0 with **zero** AddressSanitizer reports, on any platform, because no
  memory rule is broken.
- **069** — a C function name passed as a callback: `check` 0, `build` 0, `run`
  **134 three of three**, `panic: entered unreachable code — this is a compiler
  bug, please report it`. The emitted C holds
  `hero_unreachable(); /* the gate refuses this form */` and then calls
  `cb_take(t3, t4, t5)` four lines later with `t4` never assigned.
- **070** — a lease handed to a C function that frees it: `check` 0, `build` 0,
  ten runs **133 133 133 133 133 133 133 133 133 134**, **stderr empty every
  time**.

**069 is an input to this sitting rather than a side issue**: a destructor
callback is the one mechanism every real C library offers for retention, and
Heroes cannot pass one.

## The reproducer that scores the last sitting, and it is nine lines

`docs/panel/168-briefs/gallery-example-reordered.hero` is this repository's own
shipped example, `examples/gallery/13-lease.hero`, with the print moved two lines
down — after `end_lease` instead of before it:

    ./heroes check   -> exit 0
    ./heroes run     -> exit 0, five of five, "C still reads 0 bytes"
                        where the honest answer is 13
    --sanitize       -> heap-use-after-free, 13-lease.h:7 in kept_label_length

**A copy moves the moment the bytes die from *the frame returns* to *`end_lease`
runs*. Both are Heroes-side events, and C's retention is unrelated to either.**

## THE ROUTE THIS BRIEF ADDS, and it is measured rather than proposed

**The language already owns a retention vocabulary, and two sittings priced it as
if it did not.** Panel 168's completeness critic named it; the coordinator then
ran it, and the compiler answers in its own words.

`borrows` on a `cstr` parameter, `./heroes check`:

    error[unread_mark]: `borrows` on `s` is a word nothing reads: `cstr` reaches
      no handle, so no rule can say which call hands the value over or takes it
      back, and the word could be false without anything noticing
    note: the marks are read on a handle — a group `record` with a `tag` and no
      fields — and on a value reaching one through its fields. Name the C type
      the pointer stands for and mark that: `record Block tag void` is C's
      `void *` …
    note: on a handle the word does work: `acquires` must name an `extern` that
      takes one back with `consumes`, `consumes` refuses a value the caller only
      borrowed, and a handle nobody gives back aborts when `main` returns,
      naming its address.

**And the instrument two sittings said did not exist was then run.** A handle
acquired and never consumed, `./heroes run`:

    panic: 1 C handle(s) never given back — every call marked `acquires` owes one
    marked `consumes`, and this program is missing that many. The first is at
    0x100dc9f20

So: a **run-time, pointer-keyed, aborting live set already ships**
(`runtime/parts/alloc.c:368-430`, `hero_handle_acquired` / `hero_handle_consumed`),
wired to marks the grammar already parses
(`selfhost/parse/members.hero:110`, `selfhost/ast.hero:374-379` and `:467-469`),
refused on `cstr`/`ptr` by a diagnostic that **tells the author to use a handle
instead** (`selfhost/check/marks.hero:98-105`).

Panel 167 concluded *"no declaration-site mark can express retention at all"* and
*"there is no instrument at all"*. **Both sentences are false of handles**, and
no seat in either sitting checked.

## The four routes on the ballot

**R1 — the handle route.** A C function that RETAINS takes a **handle** rather
than a `cstr`/`ptr` lend, and the marks that already work do the work. What is
owed, and it is what you are judging: a rule saying a lend may not reach a
parameter that retains, and a route from a Heroes `str` or a record field to a
handle the program owns. Ask what it costs a binding author, and whether
`unread_mark`'s note is already telling them to do this.

**R2 — the caller-side rule, for 068 alone.** *A binding whose field's address
has been lent in this function is not re-assigned in this function.* In 068's
reproducer the corrupting line is a **Heroes assignment**, two lines below the
lend, in the same function, to the very binding whose field was lent. **No C code
participates in the corruption; C only observes it.** Panel 167's historian
proved static enforcement for FOREIGN retention exists nowhere; this is
**caller-side**, where the same survey found it exists everywhere. The machinery
is in the tree: `field_lend_escapes` and `field_lend_needs_a_place`
(`selfhost/check/lending.hero:194-301`) already relate a lend to its root
binding. It over-approximates, which is the loud direction.

**R3 — a group-level mark.** *This library may retain what it is given*, on the
`extern` group head line, refusing the lend for the whole group. It decides
nothing per call, so panel 167's run-time-`bool` refutation of a parameter mark
does not reach it. It lands in `selfhost/parse/group.hero` (235 of 300 in
`suite_layout`'s own unit) rather than `parse/members.hero` (298 of 300), and
hits `ast.hero` 524 of 525 and `print/fmt.hero` 1174 of 1175.

**R4 — withdraw the field lend**, panel 167's route D. Refused there on the ffi
seat's veto — 28 `const void *` declarations unbindable without shims — measured
on a tree where nothing but a lend reached such a parameter. **Panel 168 changed
the ground**: the give-away needs the library's own allocator, so route A does
not open that door either.

## Four things a verdict must not assume, each run

1. **`check` is not `build`.** `free(p: cstr)` declared with **no call in the
   file** is `check` **0** and `build` **1**, caret on the declaration.
   `unsupported[pointer_element]` is the same shape. **Any sentence of the form
   *"the checker refuses X"* about FFI names its stage or it is unrun.**
2. **The position rule already refuses five escape shapes at check** — a lend
   returned, a lend in a variant payload, a lease in `[cstr]`, in a map, in a
   variant payload — all `check` exit 1 today.
3. **Panel 167's clause 2 is SUSPENDED** by panel 168: what it would newly refuse
   is safe (`function mk_block() -> ptr` returning a `malloc`, and
   `record Arena { base: ptr }`, both `check` 0 / `run` 0), and it is bypassed in
   one line by `record Box { h: CThing }` holding a tagged handle, which is the
   idiom `unsupported[pointer_element]`'s own note recommends.
4. **A ceiling is rank 6 and robustness is rank 3.** CLAUDE.md § Precedence, and
   CL-012 names **compiler size** among what robustness beats. Panel 167 refused
   route B partly on a ceiling. **Do not refuse a robustness route on a file
   length**; price the split instead, which is what
   `.claude/rules/module-shape.md` prescribes.

## The spec's room, measured now

`./heroes measure spec/heroes-spec.md`: **real 8154** on `claude-opus-5`,
vendored maximum 6126, ceiling **10240**, headroom **2086**, and the FFI floor
mortgages 60 so what is measured against the ceiling is 8214. A `--refresh` is
what settles a change, and `.env` is present in the tree.

## Working rules

- **Build in a copy.** `cp -r` the tree to your own scratchpad, then
  `rm -rf target build` in the copy.
- A compiler in **3.80 s**:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. Rebuilding from
  `selfhost/` is roughly twenty minutes and will kill you on the watchdog.
- Never `archive/bootstrap-rs/`.
- Write your report to `docs/panel/169-reports/<your seat>.md`.
- **A negative sentence is run, or it goes out as a question naming what you
  searched for.** Two sittings have now written *"nothing can express retention"*
  over a language that ships three words for it.
