# 004 — `heroes fmt` was not a fixpoint on 31 of its own 165 modules

Date: 2026-08-26, found by panel 095's compiler-engineer seat and independently
re-measured by the coordinator at the same number in the same session.

**Status: fixed 2026-08-26** — two repairs, both in `selfhost/print_fmt.hero`.
See § The repair.

Severity: **★★★** — it made the repo-wide sweep the sitting was convened for
**impossible**, and before the guard existed it silently wrote source that
changes again on the next format. The morning's reverted sweep did exactly that
to 118 files.

## The symptom

```
$ for f in selfhost/*.hero; do ./heroes fmt "$f" >/dev/null || echo "$f"; done | wc -l
31
$ ./heroes fmt selfhost/check_map_keys.hero
error: `fmt` is not a fixpoint on its own output for `selfhost/check_map_keys.hero`
error: this is a compiler bug — `selfhost/check_map_keys.hero` was NOT changed
$ echo $?
2
```

31 of 165, among them `print_fmt.hero` itself, `check_walk.hero`,
`check_map_keys.hero`, `ir_lower.hero`, `emit_gate.hero`. `examples/` is clean,
which is why nothing in the harness net ever saw it.

**The defect is older than the guard.** `docs/defects/003`'s output guard, added
earlier the same day, is the only reason it is visible: before it, `fmt
--in-place` wrote non-fixpoint source and exited 0. The guard did not cause this;
it made a defect loud that had been in the tree since the alignment rule landed
on 2026-08-04.

## The two causes, and they are the same mistake twice

Both are the mistake `spans_lines`'s own comment already names — *"true of source
a person writes and false of source THIS FORMATTER writes"* — and defect 001's D4
paid for it once already. It came back in two places the lesson had not reached.

### (a) The measurement did not measure what the printer prints

`fits_on_one_line` decides whether a `match` arm joins an alignment run and
reports the width that decision was made on. `close_run` then vetoes the whole
run if padding any member would cross the 88-column margin — that guard was
already there and already correct. **It was reading a wrong number.**

Two errors in the sum:

- `statement()` prints `return ` before a `.return_stmt` arm's value
  (`print_fmt.hero`'s `.return_stmt` arm passes `head + "return "`).
  `fits_on_one_line` never added those **7 characters**.
- the no-value path returned `indent + left + 4` while the measured path returned
  `left + 4 + …`, and `close_run` adds `indent` to whatever comes back — so that
  path counted one indent level **twice**.

The loop this opened, on `selfhost/check_map_keys.hero:107`:

```
        .array arr => return reaches_float(c, s, decls, ty: arr.element, depth: depth + 1)
```

Measured as 82 columns, printed at 90. So it joined the run, `close_run`'s veto
did not fire, it was padded to `.array arr  =>`, and `plain_valued` then broke it
across lines because the real line was over the margin. On the **next** format
that value spans lines in the source, `spans_lines` is true, `fits_on_one_line`
refuses it, the run loses its widest member, and the padding changes:

```
pass 1:         .array arr  => return reaches_float(
pass 2:         .array arr => return reaches_float(
```

**Fix: add the head's width, taken from the literal rather than typed, and drop
the double `indent`.** Measured effect alone: **31 → 29**.

### (b) `last_line` stopped at the arm's first line, and the arm occupied four

After printing a one-statement arm, `print_fmt.hero` set `f.last_line @ line` —
the line the arm **starts** on. The comment above it explains, correctly, why it
is not `arm.span.end`: that span reaches the terminator and one line too far once
made `trailing_comment` steal the next declaration's doc comment.

But a value **this formatter** broke across lines does occupy those lines. So
after a `match` whose last arm had been broken, `f.last_line` was three lines
short, `fmt_block`'s *"a blank line the author left survives"* test
(`line > f.last_line + 1`) saw a gap that was not there, and grew a blank line:

```
        )
+                                      <- appears on every re-format
    return fail("absent", "unreached")
```

**Fix: ask `literal_end_line` for the value's own extent, which is the same
question `fmt_block` already asks about a statement, and take it when it is
further than the arm's first line.** This is the narrower span the original
comment was reaching for.

Measured effect: `check_map_keys.hero` exits 0, and the remaining single-line
disagreement disappears.

## The repair

`selfhost/print_fmt.hero`, two edits:

1. `fits_on_one_line` grows a `head` term — `"return ".chars().len()` for a
   `.return_stmt` with a value, 0 otherwise — and its no-value path returns
   `left + 4` rather than `indent + left + 4`. The width comes from the literal
   so the two sites cannot drift apart in characters.
2. the `.one_stmt` arm consults `literal_end_line(tree, s, id: one.at)` after
   printing, and advances `f.last_line` to it when it reaches further.

514 selfhost tests pass. The census is the acceptance criterion and it is in the
commit body.

## What this bought, beyond itself

**Panel 095's stage 1.** The sitting ruled that nothing about the author's
blank-line rules could be applied until `heroes fmt` exits 0 on all 165 selfhost
modules, because `--in-place` refuses a non-fixpoint and leaves the file
untouched. This is that gate.

**And the sitting measured that the new rules make it better, not worse**: the
compiler-engineer's rules-1-4 prototype refused **7** of the same 165 against
HEAD's 31 — a strict subset. So the two problems were independent, and this one
was always the blocker.

## What is still owed

`assert_canonical` — `dump(text) == dump(fmt(text))` — still does not exist
(`selfhost/print_dump.hero:12` claims it does). Both repairs here are about text,
and text is the weaker property. The defect logged the same day at
`docs/work/DECIDE.md` — `fmt` deleting the blank under a record-field remark, so
that a remark becomes documentation — **parses, is a fixpoint, and passes both of
this file's repairs.** Only the tree comparison sees it.
