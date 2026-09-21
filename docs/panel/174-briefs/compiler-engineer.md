# Panel 174 — brief: compiler-engineer

Read `00-shared.md` in this directory first. It carries the measurement and the
five routes. This brief carries only what is yours.

## Your seat

design.md §1.1, §1.7 and Part 5: the ceiling, and core-vs-sugar. You judge
**implementation cost against the live compiler and runtime**, and you cite
files and line counts. You have a veto on soundness.

## The tree you are judging, and where NOT to look

- `runtime/parts/run.c` — **572 lines** (`wc -l`), the process runner. Its
  Windows arm is `#if defined(_WIN32)` from `:335`; the POSIX arm follows the
  `#else` at `:465`.
- `runtime/hero_os.h` — the declarations, including `hero_run_why` which landed
  tonight in `497a048f`.
- `tests/harness/shell.hero` — **756 lines** (`wc -l`), `spawned()` at `:200`.
- **Never `crates/`.** That tree is `archive/bootstrap-rs/`, nothing builds it,
  and a seat sent there measures a compiler that no longer ships.

## The cheap route to a real number

The seed builds in a few seconds:

```
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes
```

Rebuilding from `selfhost/` is ~20 minutes and has killed seats on the
watchdog. **You do not need it**: nothing this sitting proposes touches
`selfhost/`, so the seed-built compiler is the compiler under test.

The harness is judged by `./heroes test tests/harness/main.hero` (**161 tests**
tonight) and by named suites, `./heroes run tests/harness/main.hero -- ./heroes
<name>`.

## What this seat is asked

1. **Price route B**, one redirect path per spawn, in lines and in call sites.
   `spawned()` is one function; say what else moves.
   `tests/harness/suite_determinism.hero:149-154` is the only reader outside
   `shell.hero` (grepped; the command is in the shared brief) — is telling it
   the path a parameter, a field on `Run`, or something else?

2. **Price the removal.** B implies deleting two files per spawn or letting
   them accumulate. The net makes **81 measured call sites**' worth of calls,
   thousands of spawns per run. Is `hero_fs_remove` per spawn measurable
   against the net's own duration? **Time it** rather than reasoning about it:
   the full net on this Mac read `1980 passed, 0 failed` tonight, and CLAUDE.md
   § Verification's clock rule binds you — while a clock runs the machine stays
   still.

3. **Price route C**, killing the tree. On POSIX the arm already forks; a
   `setpgid` in the child plus `killpg` on timeout is small. On Windows a Job
   Object is `CreateJobObject` + `SetInformationJobObject` +
   `AssignProcessToJobObject` + `TerminateJobObject`. Give line counts you have
   actually written, not estimates.

4. **The trap in route A** (§ 6 of the shared brief): with wide sharing, an
   orphan holding an inherited handle can write into a file the harness is
   capturing a different case into. Is that reachable in this code, or does
   something already prevent it? Say which, and how you checked.

5. **Is there a route nobody listed?** The shared brief asks it and it is a
   claim about the option SET. What would have to be true for one to exist?

## What would make your verdict wrong

Name it. If your cost figure rests on a premise about what the net does per
spawn, say which command would falsify it.
