# Panel 159 — ffi-pragmatist brief

Read `docs/panel/159-briefs/00-shared.md` first. You judge §1.11 and §4.19, and
you have a veto on ABI breakage.

**Your seat is here for one of the three and a sweep, and the brief says so
rather than inventing work for you.** `sort`'s direction and `xs[i] @ v` have no
C boundary in them as far as the coordinator can see — **say if that is wrong**,
because it is exactly the kind of claim a seat has corrected in each of the last
four sittings.

## The one that is yours: `main` and the process boundary

`main` may not be `-> ()?` — `error[main_returns]` says *"a program reports
failure by what it prints, not by what it returns"*. That sentence is about the
PROCESS boundary, which is §1.11's edge: a Heroes program's exit status is what
a shell, a CI leg and a parent process read.

- What does a Heroes program's exit status actually carry today? Measure it: a
  clean run, an abort, `exit(code:)`, a `?` that propagates out of a function
  into `main`'s caller — and whether any of those disagree with
  `.claude/rules/cli-surface.md`'s three-code contract for the COMPILER, which is
  a different contract from a compiled program's.
- If `main` could return `()?`, what would the runtime do with the failure —
  print it where, exit with what? Read `runtime/parts/os.c` and the generated
  `main` rather than reasoning: the answer is either already there or visibly
  absent.
- **And the sweep**: does any shipped binding in `examples/` depend on the
  current refusal — a program whose `main` calls a fallible C wrapper and
  handles it because it must? Name the count.

## The standing question for your seat

Is there anything in these three silences that a BINDING AUTHOR would get wrong?
A reader writing `extern` declarations is the reader §1.11 says is the only kind
there is, since everything comes from C. If a silence here costs them, say what
it costs and that is your R-answer; if it costs them nothing, say that plainly —
a seat reporting *nothing on my boundary* is a finding and not a failure.

Build in a copy; no command over ~60 seconds; never read `archive/bootstrap-rs/`;
capture exit codes directly. Write to
`docs/panel/159-reports/ffi-pragmatist.md` **first**.
