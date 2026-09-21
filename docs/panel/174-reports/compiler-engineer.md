# Panel 174 — compiler-engineer (SOUNDNESS lane)

Every number below came from a command run in this seat's session on
2026-09-21, on this Mac (Darwin arm64), in a copy of the tree at HEAD
`497a048f`. The command is named beside each one. Where something could not be
run it says so in those words.

**A correction I owe up front.** My first copy nested inside a stale copy left
by an earlier session, so the first thirty minutes of this seat measured
`bec2de33` — twenty-four commits back, without `hero_run_why`. `git log
--oneline -1` in the copy is what caught it. Everything below was re-run
against a fresh copy whose `git log --oneline -1` reads `497a048f` and whose
`wc -l runtime/parts/run.c` reads **572**.

---

## verdict

**object** to route A standing alone, in any combination that does not also
carry B. **Adopt B + C, in that order of necessity**, as one falsifiable
sentence:

> Giving every spawn its own pair of redirect names (**B**) removes defect 074
> whoever the holder turns out to be, and killing the process *tree* rather
> than the process (**C**) removes a second defect this seat measured tonight
> — the watchdog answers 124 while the descent it was supposed to stop is
> still running. Neither touches the language. Route **A alone converts a loud
> refusal into a silent wrong answer**, and I have the bytes.

No veto. A veto is for a core construct or a breached ceiling, and this
proposal adds **no construct at all**: zero lines in the lexer, the checker,
the descriptor pass, the ownership pass or the emitter, and zero spec tokens.

## section

**design.md §1.1**, the hierarchy, and its fourth force in the last paragraph:

> "And above tokens there is now a fourth force: **§1.12's robustness**, which
> decides between two admissible forms when one of them can be made to crash."

with **§1.12** itself as the ground — *"a guarantee that ends quietly is not
one"* — and CLAUDE.md § Precedence rank 3. Route A is an admissible form that
can be made to produce a wrong answer; B is one that cannot. §1.1 settles it
before cost is discussed.

**§1.7 and Part 5 do not reach this sitting, and I say so explicitly rather
than inventing a rationale.** Part 5's seven constructs are untouched; §1.7's
core/sugar test has no purchase on a runtime process launcher. The only §1.1
clause that binds here is the ceiling, and I price it below.

---

## implementation_cost

Written in my copy, compiled, and gated. `git diff --numstat` after the work:

| file | +/- | at HEAD | after |
|---|---|---|---|
| `runtime/parts/run.c` | **+79 / -3** | 572 | **648** |
| `runtime/hero_os.h` | **+4 / -0** | 271 | 275 |
| `tests/harness/shell.hero` | **+26 / -2** | 756 | 780 |
| `tests/harness/suite_determinism.hero` | **+14 / -12** | 206 | 208 |

Split by route (`git diff --numstat` taken at each stage):

- **Route C**, both halves, one file: **+65 / -3** in `runtime/parts/run.c`.
  **31 of the 65 are code**, the rest comment
  (`git diff -U0 | grep '^+' | grep -vE '^\s*(\*|/\*|//)' | grep -vE '^\s*$' | wc -l`).
  Of those 31, **6 are the POSIX half** and **25 are the Windows Job Object**.
- **Route B**, four files: **+58 / -14**, of which **~18 are code**
  (`hero_os.h` 1, `shell.hero` 6, `suite_determinism.hero` 6, `run.c` ~5).

**Where they land: nowhere the compiler's front end can see.** Not the lexer,
not `selfhost/check/`, not the descriptor pass, not `selfhost/ownership/`, not
`selfhost/emit/`. One C file in the runtime and two harness files.

### Route C, the sketch, since my brief asked for it owed

POSIX (`runtime/parts/run.c`, the `#else` arm at `:465`):

- `hero_run_child` takes a `lead_group` parameter and calls `setpgid(0, 0)`;
- the parent calls `setpgid(child, child)` too, so neither side can lose the
  race;
- the timeout branch calls `kill(-child, SIGKILL)` **beside** the existing
  `kill(child, SIGKILL)`, not instead of it.

**The gate is the narrowing, and it is the one design decision in route C.**
Both `setpgid` calls are conditioned on `hero_run_limit_seconds > 0`. A process
group of its own is detached from the terminal's foreground group, so a child
that reads the tty stops on SIGTTIN — which is exactly what `heroes run` hands
the program (`selfhost/cli/verbs.hero:87`, `out: ""`, meaning inherit). That
would have been a regression nobody listed. It cannot happen, and the reason is
a fact about the value in hand rather than a premise about the world
(`.claude/rules/module-shape.md`): **`hero_run_limit` is called in exactly one
place in this repository**, `tests/harness/shell.hero:214`
(`grep -rn 'hero_run_limit' selfhost/ tests/harness/`). The compiler never arms
the watchdog, so `heroes run` takes neither the group nor the risk.

Windows: `CreateJobObjectA` + `SetInformationJobObject` with
`JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE`, `CreateProcessA` with `CREATE_SUSPENDED`
so the child is assigned **before** it can spawn anything, then
`AssignProcessToJobObject` + `ResumeThread`; `TerminateJobObject` on the
timeout and `CloseHandle(job)` on every exit including the ordinary one. A
failed `CreateJobObject` leaves `job == NULL` and the code degrades to today's
behaviour rather than failing the launch.

**The Windows half is UNCOMPILED and I say so rather than implying otherwise.**
There is no Windows SDK on this Mac and no cross-compiler
(`which x86_64-w64-mingw32-gcc i686-w64-mingw32-gcc` → not found), and the
author's box is owned by another session.

### What I ran to gate it

- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` — **SEED_OK**,
  3.81 s.
- `./heroes check tests/harness/main.hero` — **rc=0**.
- `./heroes test tests/harness/main.hero` — **161 tests, all passed.**
- `./heroes run tests/harness/main.hero -- ./heroes determinism` —
  **166 passed, 0 failed**, the suite whose only outside-`shell.hero`
  dependency route B rewrites.

### The removal, timed rather than reasoned (my brief item 2)

`removecost.c`, linked against the shipped runtime, machine still, nothing else
running:

- **4000 `hero_fs_remove` calls on existing files: 0.1393 s → 34.83 µs each,
  69.65 µs for the two a spawn owes.**
- The **floor** of a spawn, 200 × `/usr/bin/true` through `hero_run_go`:
  **1222.57 µs each.** Two removes are **5.70%** of the cheapest spawn that can
  exist.
- The **real** spawn the harness makes, `/usr/bin/time -p ./heroes build
  build/bench/x.hero -o build/bench/x`, warm: **real 0.19**. Two removes are
  **0.037%** of it.

So the removal is not measurable against the net's duration, and the answer
does not rest on how many spawns the net makes. **At ten thousand spawns the
whole removal budget is 0.70 s.**

### The three failure modes my brief asked me to hunt for in B

- **MAX_PATH**: the longest path route B can build, with an absurd pid and a
  ten-million serial, is `C:\w\heroes\build\harness-2147483647\stdout-10000000`
  — **52 characters of 260** (computed from `tests/harness/main.hero:98`,
  `SCRATCH` is `"build/harness"`). Not a risk.
- **A suite that globs the scratch**: I searched
  `tests/harness/*.hero` for `list_entries(… dir: scratch)`,
  `found_under(dir: scratch`, `list_tree(… dir: scratch` and found **nothing**.
  That is a negative claim resting on my vocabulary, so it goes out as a
  question: *is there a reader of the scratch directory spelled some way I did
  not search for?*
- **The one dependency outside `shell.hero`**: `suite_determinism.hero:149`'s
  `last_error(scratch)`. It is **neither a parameter nor a field on `Run`** —
  it is a **deletion**. `dumped()` already returns `fail(msg: said.err)`, so the
  words are in the value the caller is holding; `last_error` becomes
  `last_error(got: str?) -> str` returning `e.msg`. That removes a `read_file`
  and removes the bug where the function returned whatever process wrote there
  last. Net **+14 / -12** in that file.

---

## needed_for_self_hosting

**no**, by Principle 0's letter: it is not a language form, so nothing here
enters v1 and the closure list is untouched. The compiler compiles itself
today.

**And Principle 0 is the wrong question for this sitting, which I say rather
than stretching it.** This is the instrument that *gates* self-hosting on one
of the three platforms. A harness that cannot tell a machine problem from a
program's verdict makes every Windows gate unreadable — 120 red lines on
2026-09-21, not one of which named the machine until `497a048f`.

---

## argument

Route A is refused on §1.1's fourth force, and not in the abstract: the trap is
**already live on POSIX**, where sharing is wide by construction. Measured at
HEAD, on this Mac — a case's capture arrives as **150 bytes where 17 are
owed**, 58 of them NUL, three lines written by a *different* case's orphan.
Widening the share mode ports that to Windows and deletes the only thing
currently making it loud. B makes it impossible: an orphan holds a name nobody
asks for again, whoever the orphan is. C is owed separately because the
watchdog is broken on its own terms — it answers 124 with the descent still
running, which is the ninety-minute modal dialog `run.c:271` exists to prevent.

*(118 words)*

---

## prediction

**At the close of the milestone that lands B, on its pre-push Windows CI leg
and the two after it:**
`gh api repos/heroes-lang/heroes/actions/jobs/<id>/logs | grep -c "operating
system's own reason is 32"` will read **0** on all three, and `harness:` will
read **0 failed** on all three.

**And a second one, which separates C from B rather than riding on it**, since
B alone would also produce the zero above: at that same close,
`$SC/probe/orphan` — the probe in this report — will print **`gone`** on
Darwin and on Linux. It prints **`STILL ALIVE`** at `497a048f` today. If C
lands and that probe still prints `STILL ALIVE`, C did not do what it was
bought for.

**And the ceiling number, so it can be scored against me:**
`wc -l runtime/parts/run.c` will read **≥ 640** (it is 572 today and 648 in my
copy). If the panel adopts this and that file comes back under 600, somebody
dropped the Windows half.

---

## condition

**My verdict on C's Windows half depends on one thing I could not run**: that
`AssignProcessToJobObject` succeeds on the GitHub runner. Runners place their
processes in a job object of their own, and a nested assignment fails on
pre-Windows-8 semantics or where the outer job forbids breakaway.

The command that falsifies it: build the patched `run.c` on the author's box
and run the orphan probe there. If it prints `STILL ALIVE` with
`GetLastError()` = 5 after `AssignProcessToJobObject`, C's Windows half is
**inert** and B carries the entire resolution alone — which it can, and that is
why B is first.

**The second condition is the one that would change the ORDER, not the
verdict**: if the CI holder turns out not to be a descendant of the harness
(Defender, WerFault, the runner agent), C does nothing for defect 074 at all.
The shared brief § 4 already says *what creates the orphan on the GitHub runner
is not measured*. **This is precisely why B and not C is the load-bearing
half**: C rests on a premise about who the holder is, and
`.claude/rules/module-shape.md` says a premise about the world expires in
silence. B asks only about the value in hand — this spawn's own name.

---

## Is route A safe alone? (the trap, § 6 of the shared brief)

**No. It is reachable in this code, nothing prevents it, and it is already
happening on POSIX today.** How I checked, rather than argued:

`interleave.c`, linked against the shipped runtime at `497a048f`, reproduces
`spawned()`'s exact loop. Case A spawns a grandchild that keeps writing and a
parent that hangs; the 2 s watchdog fires; case B then runs `echo
CASE_B_ONLY_LINE` capturing into the **same two paths**, exactly as
`tests/harness/shell.hero:206-207` does for all 81 call sites.

```
case A: code=124 (124 = watchdog)
case B: code=0
case B's capture is 150 bytes
```

```
00000000: 4341 5345 5f42 5f4f 4e4c 595f 4c49 4e45  CASE_B_ONLY_LINE
00000010: 0a00 0000 0000 0000 0000 0000 0000 0000  ................
...
00000040: 0000 0000 0000 0000 0000 004f 5250 4841  ...........ORPHA
00000050: 4e5f 4259 5445 535f 4652 4f4d 5f43 4153  N_BYTES_FROM_CAS
00000060: 455f 410a 4f52 5048 414e 5f42 5954 4553  E_A.ORPHAN_BYTES
```

Seventeen bytes were owed. The file the harness judges case B on holds case B's
line, a 58-byte NUL hole punched by `CREATE_ALWAYS`/`O_TRUNC` against an orphan
that kept its own file offset, and **three lines written by a process belonging
to another case.**

**POSIX has no `FILE_SHARE_READ` and never refused anything.** So route A does
not invent this hazard — it *imports the POSIX behaviour to Windows* and
deletes error 32, which is the only reason anybody noticed. That is a loud
failure traded for a silent one, which §1.1's fourth force and § Precedence
rank 3 refuse.

**Where it stops being cosmetic**, and I state this as a reachable path rather
than a demonstrated flip, because I did not demonstrate a suite changing its
verdict: `tests/harness/shell.hero:535`'s `machine_lacks_the_library` is a
`strings.contains` over exactly this capture, and a hit turns a FAILURE into a
SKIP. `tests/harness/suite_run.hero:185` is a `contains` for
`"AddressSanitizer"` over the same stream — and cases 35 to 37, the boundary
the shared brief identifies, are the only three in the tree carrying
`!sanitizer:`. An orphan from a sanitiser case writing into a later case's
capture is not a stretch.

**With route C applied, the same probe prints `case B's capture is 17 bytes`,
0 NULs.** With route B applied, case B's capture is a file the orphan has never
heard of.

**One thing route A is not**: harmful *with* B. Once each file has exactly one
writer, the share mode stops mattering — which also means A becomes
unnecessary. I would not spend the three lines.

## Route D, and why I do not believe it reaches the holder

`PROC_THREAD_ATTRIBUTE_HANDLE_LIST` constrains **our** `CreateProcessA` call
and nothing else. The direct child *must* receive those handles — capturing its
output is the point. Once `heroes.exe` holds them as its std handles, its own
`CreateProcessA(…, TRUE, …)` passes them on, and our attribute list has no
say. So D cannot reach `clang.exe`, the holder `shell.hero:414-417` names.

**This is reasoning from the Win32 contract and not a measurement** — I have no
Windows. It should be checked before anyone spends on D.

## Route E is now refuted by the coordinator's update

The coordinator reports the window reached `determinism` tonight where it
closed inside `annotations` this morning: **a lifetime of minutes, unbounded**.
A bounded retry with backoff has to name a bound, and no bound covers an
unbounded holder. E would convert 120 loud failures into 120 slow loud
failures. Not owed.

---

## What would have to be true for a route nobody listed to exist?

The five routes on the table all assume **the harness's own `CreateFileA` is
the operation that must change**. A sixth exists wherever a *different* process
in the chain is the one doing something avoidable. Two are real, and I measured
both:

**Route F — the compiler stops handing its inherited stdout to clang.**
`selfhost/cli/toolchain.hero:79` reads

```heroes
ran = process.run(program: "clang", words: line, out: "", err: err_path)
```

and `out: ""` **means inherit** (`runtime/parts/run.c:131`,
`hero_run_inherits`). So under the harness, `clang.exe` is handed the harness's
`scratch/stdout` handle — which is exactly the holder recorded at
`shell.hero:414-417`. `process.discard()` already exists
(`selfhost/cli/process.hero:104`) and clang writes its diagnostics to the
stderr file it is already given. **One word, one line.** I did not adopt it,
for the same reason I put C second: it narrows the holder set and rests on a
premise about who the holder is. Worth naming; not worth standing on.

**Route G — the compiler's own `Ran` cannot say 32, and `497a048f` did not
give it the ability.** Tonight's repair taught `tests/harness/shell.hero`'s
`Run` a `why` field. `selfhost/cli/process.hero:67-70` is the same record in
the compiler and still reads

```heroes
record Ran
    code: i64
    started: bool
```

so `selfhost/cli/toolchain.hero:99-104` prints **"clang is not on this
machine's PATH"** for every `!ran.started` — including an ERROR_SHARING_VIOLATION.
That is the identical false sentence the panel just repaired one layer up, and
it is the class `runtime/parts/run.c:21-28` says this file exists to delete.
**Cost: one field, one line at `process.hero:97`, one extern declaration, and
the message.** ~6 lines. This is a residual of my own verdict, and I name it
because B as I wrote it lives in `shell.hero` and leaves the compiler's shared
paths (`build_dir + "/clang-version.txt"`, `"build/selftest_err.txt"`,
`build_dir + "/doctor-out.txt"`) exactly as they are — and **route C does not
cover them either**, because the compiler arms no watchdog, so the job object
is never created for a `heroes build`.

The generalisation, since the question is about the option **set**: a route
nobody listed exists whenever the failing operation is not the one being
patched. Here there are three processes in the chain and the five routes all
name the first.

---

## The coordinator's point 3: the fourth liar, and whether the thirteen ride along

**Separately, with one exception that rides along free.**

The instrument named in the update — `grep -c '\.code' tests/harness/suite_*.hero`
— measures the wrong thing now, and I say so rather than repeating it. It reads
**70 `.code` reads across 15 suite files**, of which only `suite_records.hero`
and `suite_special.hero` also read `.started`. But since `497a048f`,
`insisted()` **refuses** rather than returning a `Run` with `started: false`
(`shell.hero:183-190`), so `.code` is now only reachable on a `Run` that
started. Those thirteen are no longer lying by reading `.code`.

**The real liar is a suite that discards the error VALUE**, and that is what
`suite_determinism.hero:58-59` does: `if first.is_err() || second.is_err()`
then `detail: "  cannot dump " + item.source`, with `e.msg` thrown away.
Measured:

- **457 `is_err()` branches across 20 suite files**
  (`grep -c 'is_err()' tests/harness/suite_*.hero`);
- **269 `keep_failure(` sites**, of which **19** details carry an error value
  (`refusal(`, `complaint(`, `last_error(`, `.msg`, `.err`);
- `shell.hero:155-173` already ships `refusal()` and `complaint()` for exactly
  this, and **seven suite files** ever call either.

So the blanket repair is up to **250 report sites** and is a milestone, not a
step. It changes what is printed and never whether the defect happens, so it
must not gate this resolution.

**The exception**: `suite_determinism.hero:59` is inside the file route B
already touches, and route B's `last_error(got: str?)` is what makes the repair
a one-liner. I wrote it, and the suite is green with it:
`./heroes run tests/harness/main.hero -- ./heroes determinism` →
**166 passed, 0 failed**. Final cost of that file **+14 / -12**.

---

## The one thing I object to in my own verdict

`runtime/parts/run.c` goes **572 → 648 lines**. CLAUDE.md §11's threshold is
~300 and this file is already past twice it; `runtime/parts/` exists *because*
`runtime.c` passed 1200 lines and that rule was cited.

**And nothing would tell anybody.** `tests/harness/suite_layout.hero` contains
no occurrence of `runtime` and none of `.c"` (grepped), so the instrument that
polices file length does not read the runtime's C at all. That is the shape
`.claude/rules/module-shape.md` names — a rule with no instrument — living in
the one directory where robustness is decided.

I do not veto on it, because the seam is real and cheap: `run.c`'s
`#if defined(_WIN32)` arm begins at `:335` and is now ~155 lines. It can become
`runtime/parts/run-win.c`, `#include`d by `run.c` so the one-translation-unit
rule `runtime/runtime.c:19` states is untouched. **If this resolution lands
without that split, the file is 648 lines and no check in the tree will ever
mention it.**

---

## Commands, so every number above can be re-run

```
git log --oneline -1                                    # 497a048f
wc -l runtime/parts/run.c runtime/hero_os.h tests/harness/shell.hero
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes
clang -I runtime <probe>.c runtime/runtime.c -o <probe> # orphan, interleave, removecost
./heroes check tests/harness/main.hero
./heroes test tests/harness/main.hero                   # 161 tests, all passed
./heroes run tests/harness/main.hero -- ./heroes determinism   # 166 passed, 0 failed
/usr/bin/time -p ./heroes build build/bench/x.hero -o build/bench/x
grep -rn 'hero_run_limit' selfhost/ tests/harness/
grep -c 'is_err()' tests/harness/suite_*.hero
grep -ho 'keep_failure(' tests/harness/suite_*.hero | wc -l
```

The three probes live at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e64edfa2-e1eb-4a90-b26e-8635f02430e5/scratchpad/probe/`
and the patched tree at `…/scratchpad/t174`. Neither is under this repository.
