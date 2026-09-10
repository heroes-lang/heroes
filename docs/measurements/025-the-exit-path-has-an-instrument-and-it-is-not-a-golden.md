# 025 — The exit path has an instrument, and a golden cannot be it

Panel 125's point (iii), ratified 2026-09-09: a lease outstanding when the
program leaves through `exit(code:)` is not accused, because the runtime's gate
runs at the end of `main`, and **the Linux `--sanitize` leg is the instrument
named instead**. The ratification's payment was a golden that pinned the path,
because no case in `tests/golden/run/` named both `lease(` and `exit(`.

The golden was written, it went green on this Mac, and **the CI's Linux leg
turned it red within the hour**. This file is why, and the conclusion is that
the sitting's own sentence was right and the golden was the wrong instrument.

## The two commands, and they answer differently

Both in the container `docs/environment/linux/` describes, on the same program,
the same tree, the same compiler:

```
$ ./heroes run tests/golden/run/lease-open-through-exit.hero --sanitize
==22==ERROR: LeakSanitizer: detected memory leaks
Direct leak of 29 byte(s) in 1 object(s) allocated from:
    #0 malloc
    #1 hero_malloc_raw /w/runtime/parts/alloc.c:145:15
    #2 hero_alloc_held /w/runtime/parts/alloc.c:179:15
    #3 hero_str_held   /w/runtime/parts/str.c:359:25
exit 1

$ ./heroes build --sanitize tests/golden/run/lease-open-through-exit.hero -o /tmp/lx && /tmp/lx
12
exit 0
```

`suite_run.hero` runs the third configuration as `run … --sanitize`, which is
the first command. The session that wrote the golden probed it with the second,
read the silence, and wrote a dated correction into the case's own comment
saying the Linux leg does not see the block. **That correction was false, and it
was false because the command was not the suite's.** At the `build` level the
lease cell is still a live local of `main` when `exit` is called, so LSan finds
a pointer to the block in a root region and calls it reachable; at the level
`run` compiles, it does not.

**LeakSanitizer is not broken in that container, which the same session had no
evidence for either way.** Probed afterwards, three C programs: `malloc` and
return, `malloc` and `exit(0)`, both reported at 64 bytes, exit 1, default
options and with `detect_leaks=1`. So the tool works there and the earlier
silence was the program's optimisation level, not the instrument's absence.

## What follows

**A `tests/golden/run/` case may not leak.** The suite's third configuration is
`--sanitize` and its own check refuses any case whose stderr carries
`AddressSanitizer` — the leak report included, since LSan reports under that
banner. A program that demonstrates the `exit()` path has an outstanding lease
by construction, so it leaks by construction, so it cannot be a case there. The
case is removed, with its `.expected` and its blessed emission trace.

**And the payment for the spec's +4 is a measurement rather than a prediction,
which is stronger.** `docs/measurements/010`'s row 62 registered the golden;
what stands in its place is the run above: on the Linux leg the outstanding
lease IS reported, by name of the allocation site, while the runtime's own count
stays silent. That is panel 125 (iii) as written, and the instrument exists
today and fired today, in CI, on a commit that had just been pushed.

**What is owed is a sentence on the site rather than a case in the net.** The C
boundary chapter said the `exit()` path "is not checked", which is true of the
count and silent about the leg that does check it. It says both now.

## The lesson, and it is about a session's own probe

The rule this cost was CLAUDE.md § RUN IT's third shape, the list is a
measurement too: *a negative claim rests on the searcher's vocabulary rather
than the world*. The session ran one command, got silence, and generalised to
"nothing accuses this program on any of the three platforms" — a claim about
every instrument, from one invocation of one of them. The suite's own command
was one line away in the file the claim was being written into.

## Corrected 2026-09-10, later the same day, by the workflow that turned this into case law

Three sentences above are wrong, and CL-074 quotes them, so they are corrected
here rather than edited:

1. **"three C programs"** counts an enumeration of two. The probe was two
   programs, `malloc` and return and `malloc` and `exit(0)`, run three times,
   the first of them twice, once with `ASAN_OPTIONS=detect_leaks=1`. Two
   programs, three runs.
2. **"The suite's own command was one line away in the file the claim was being
   written into"** is false. The command is in `tests/harness/suite_run.hero`,
   a different file in a different directory, at the line that runs the third
   configuration. What stood one line away, in the case's own comment, was the
   sentence the correction was replacing.
3. **"CLAUDE.md § RUN IT's third shape"** names the wrong shape. The sentence
   quoted beside it, *a negative claim rests on the searcher's vocabulary
   rather than the world*, is the SECOND shape's, CL-018's; the third is *the
   list is a measurement too*, CL-057. Both were in play and neither is the
   whole of it, which is why CL-074 exists: the command ran and answered a
   question nobody had asked, and the remedy is the gate's own invocation
   rather than a hedge.

`DESIGN-LOG.md`'s row for this file carries the same third-shape mislabel and
the same closing sentence; it is a record too, and CL-074 corrects both where a
reader of the contract will meet them.
