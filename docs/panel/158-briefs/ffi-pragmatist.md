# Panel 158 — ffi-pragmatist brief

Read `docs/panel/158-briefs/00-shared.md` first. You judge design.md §1.11 and
§4.19 — everything comes from C — and you have a veto on ABI breakage.

## Your half, and it decides whether *consistency, not safety* survives

The shared brief asserts that footing on four measurements, one of which is
yours: `hero_runtime_check_leaks()` clean on a nested fallible carrying a `str`.
**Re-measure it and then attack the shapes beside it**, which is where this
project has found every recent defect:

- a nested fallible carrying a `cstr`, and one carrying a **handle** with an
  `acquires` mark — does the obligation counter still see it through two levels?
- a nested fallible crossing the C boundary in either direction, if the language
  allows it at all — say so if it does not;
- `--sanitize` on each, on this Mac, and on the **Linux container** where
  LeakSanitizer exists and on this Mac does not (`.claude/rules/c-boundary.md`);
- the three-level case, `i64???`, which a user generic produces twice over.

**If any of those leaks, corrupts, or gives a wrong answer at exit 0, this stops
being a consistency question and becomes §1.12**, which outranks every other
consideration in the sitting. Say so plainly and it decides.

## And the emitted C

The shared brief says `?` peels the outer level in the emitted C. Read the C
rather than trusting that line: what does a two-level `?` chain actually emit,
and is the inner fallible's payload freed on the error path of the outer one?
That is the shape a leak would take and a runtime counter might not see.

Build in a copy; `rm -rf target build`. No command over ~60 seconds; the Linux
container is `docker run --rm -v "$PWD":/src:ro heroes-linux …` per
`docs/ref/environment/linux/LINUX-MACHINE.md`. Never read `archive/bootstrap-rs/`.
Capture exit codes directly.

Your verdict owes R1-R4, the C you compiled and ran, a prediction, your
condition, what you left UNRUN, and whether you cast your veto. Write to
`docs/panel/158-reports/ffi-pragmatist.md` first.
