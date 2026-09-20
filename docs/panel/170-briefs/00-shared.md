# Panel 170 — shared brief: the mark that says a parameter retains

**Full panel**, five seats and a completeness critic. It adds a contextual word
to `spec § 13`'s grammar and a refusal to the checker, so the soundness lane is
not enough.

**Every number and path below was produced by a command run while this brief was
written, 2026-09-20, Darwin arm64.** Re-run anything you rest a verdict on. The
coordinator has been wrong in a brief twice this milestone and both were caught
by seats: a scope sentence at panel 169 that cost that sitting its subject, and
a stage claim at panel 168.

## Why this sitting exists, and it is the shape the milestone arrived at

Four open defects converge on one missing thing. **Nothing lets a declaration
say a parameter RETAINS, and nothing refuses a lend that reaches one.**

- **066** — a lend whose bytes C keeps past the call. Now that `spec § 13` says
  *a lend lives for its call and no longer*, the program is right and the
  **binding** is wrong, and there is no way to write the right binding.
- **068** — the record rewritten under C's held address. Reclassified
  2026-09-20: under the stated extent this is the same defect as 066 with a
  different witness, not the separate exclusivity class panel 169 filed it as.
- **070** — a lease handed to a C function that frees it: `check` 0, `build` 0,
  and the program dies with **an empty stderr** and an exit code that is 133 nine
  times in ten.
- **072** — two C allocator families collapsed onto one handle type by
  `one_tag_one_type`, each freeing the other's blocks at `check` 0 / `build` 0 /
  `run` 0 in silence.

## What already ships, and it is more than two sittings believed

`spec § 13`'s grammar, read today:

    CParam = [ "@" ] ident ":" Type [ "counted_by" ident ] [ "owned" ident ]
             [ "consumes" | "acquires" ident | "borrows" ] .

**Four marks already parse on an FFI parameter.** `borrows` already says *the
call hands back one it keeps*; `acquires` already carries a **pointer-keyed live
set that aborts**, run at panel 169 and again today. They are read **on a
handle** — a group `record` with a `tag` and no fields — and refused on `cstr`
and `ptr` with `error[unread_mark]`, whose own note tells the author to name the
C type and use a handle instead.

**And the give-away route now works with no new form at all**, measured after
defect 069's repair and recorded as
`docs/measurements/037-the-give-away-case-was-writable-the-moment-the-callback-was.md`:
the author allocates, hands C the pointer **and** the disposer, `check` 0,
`run` 0, **zero AddressSanitizer lines** — and it survives the replaced-allocator
measurement that killed panel 168's trailing header, because the author's
destructor is paired with the author's allocator.

**So this sitting is not asked to invent a route. It is asked what a DECLARATION
should be able to say, and what the compiler should refuse once it says it.**

## The question, in four parts

1. **What is the mark?** A word on the parameter (`keeps`, `retains`, or the
   mirror of `owned`), a word on the group, or something no sitting has named.
   Panel 169's llm-ergonomist proposed `keeps end_fn`, naming the call that gives
   the bytes back: *it refuses a lend, takes a lease, and that lease is ended by
   `end_fn` and not by `end_lease`*.
2. **What does it refuse?** At minimum a lend at a marked parameter. Say whether
   it also refuses a lease, and what it admits instead.
3. **What does an UNMARKED retaining parameter cost?** Panel 169's spec-warden's
   objection stands and must be answered rather than restated: *in all three of
   066's reproducers the mark is absent, so the rule closes the case where the
   author already knew.* The counter-argument on the table is that `owned`,
   `consumes` and `acquires` are already the author's words and the compiler
   already enforces what they say rather than auditing their truth. **Judge that
   symmetry rather than assuming it.**
4. **Does it also answer 072?** That defect needs a handle to carry **which
   producer made it**, which is a different fact from retention. Say whether one
   mechanism serves both or whether they are two.

## Four things a verdict must not assume, each run

1. **`check` is not `build`.** Several pointer and FFI refusals fire at `build`,
   with the caret on the declaration. Any sentence of the form *"the checker
   refuses X"* about FFI names its stage or it is unrun.
2. **The AST's write set is an open set.** Panel 169 watched it go from one
   spelling to two to three inside one sitting, and its resolution moved any such
   rule to the **IR**, where it is closed and it is two. If your route states a
   rule over program structure, say which layer.
3. **A ceiling is rank 6 and robustness is rank 3.** CLAUDE.md § Precedence, and
   CL-012 names compiler size among what robustness beats. **Do not refuse a
   robustness route on a file length**; price the split.
4. **Panel 169's resolution is the default in force**, including that the
   caller-side rule is **not** built and that clause 2 of panel 167 is
   **suspended**. Both are queued for the author and neither is settled.

## The spec's room, measured now

`./heroes measure spec/heroes-spec.md`: **8201 real** on `claude-opus-5`, **6159**
vendored, ceiling **10240**, headroom **2039**, FFI floor mortgaging 60. `.env`
is present, so `--refresh` is available and a draft is priced by applying it to
the real path and reverting.

## Working rules

- **Build in a copy.** `cp -r` the tree to your own scratchpad, then
  `rm -rf target build` in the copy.
- A compiler in about four seconds:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. **Never rebuild
  from `selfhost/`** — it is minutes and the watchdog will kill you.
- Never `archive/bootstrap-rs/`.
- Write your report to `docs/panel/170-reports/<your seat>.md`.
- **A negative sentence is run, or it goes out as a question naming what you
  searched for.** Two sittings wrote *nothing can express retention* over a
  language that ships three words for it.
