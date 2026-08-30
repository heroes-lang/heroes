# Panel 098 — the shell stops being the boundary, and the argument list cannot cross

**Lane: SOUNDNESS** (`compiler-engineer` + `ffi-pragmatist`). Both seats were
asked to say loudly if a diagnostic class moved, which would make this a full
panel; the engineer measured that none does — `emit_ffi_lookup.hero` decides
CLAUDE.md §7's four named exceptions by reading clang's **stderr text**, and the
same bytes arrive either way, so the class boundary is invisible to the change.
What the lane gives up is the spec-warden's count, which is zero here: no spec
token moves.

Convened 2026-08-30, the same day as panel 097 and out of its condition 7.

## The proposal, verbatim as briefed

> The compiler stops speaking to the operating system through a shell. Today
> `cli_shell.shell()` builds a COMMAND LINE as a string and hands it to
> `system()`. The proposal is that the runtime gains a way to EXECUTE A PROGRAM
> WITH AN ARGUMENT LIST — `posix_spawn`/`fork`+`execv` on POSIX, `CreateProcess`
> on Windows — declared behind `runtime/hero_os.h`, capturing stdout and stderr
> and returning a real exit code. `cli_shell.sq()` then has nothing to quote and
> is deleted.

## The precedent the brief did not name, and it is the decisive one

Found by the ffi-pragmatist, in `docs/work/DONE.md:760-768`. When `system()` was
chosen at M-selfhost-port it was taken **revocable**, and it named this sitting's
proposal by name:

> "taken (revocable): bind C's own `system()` via the language's FFI … The
> alternative (a runtime entry point wrapping posix_spawn with argv separation)
> is safer against quoting but grows the runtime; system() + careful quoting of
> the few paths we pass **is v1**. Veto or confirm."

v1 was reached at M-selfhost-fixpoint on 2026-08-18. The clause expired on its
own terms, and both halves of the trade it named are now measured rather than
predicted.

## Verdicts

| judge | verdict | section | measured cost | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **accept-with-condition**; veto stands if the argument list is spelled `[str]` | design.md §1.7 (core plus elaboration) | **zero core constructs**; `check_ffi.hero` 376 lines, 0 changed; five `selfhost/` modules shrink; `runtime/parts/spawn.c` 171 lines, POSIX arm measured | `sq(` repo-wide is 189, of which **121 are in `tests/harness/`**; a milestone touching only `selfhost/` leaves ≥115 of them and the net red on Windows | five |
| ffi-pragmatist | **accept-with-condition**; the briefed single-call form does not compile | design.md §1.11, §4.19; spec:204-207, :222; CLAUDE.md §7, §12 | `hero_os.h` 134 → 148 lines, 6 → 9 declarations; C is 351 lines across POSIX, Windows and dir; ABI need not move but by §7 convention should | `examples/sqlite/main.hero`'s emitted C stays byte-identical — measured 17927 bytes both ways, `cmp` clean; one byte of difference falsifies | eight |

**Neither seat vetoed the direction. Both vetoed the same spelling**, from
different sections, and that agreement is the sitting's firmest result.

## Why the briefed shape cannot be written

The ffi-pragmatist compiled every attempt:

- **`argv: [str]` is `error[ffi_type]`** — `check_ffi.hero:45`. A Heroes array
  does not cross the boundary.
- **The escape hatch is shut too**: packing arguments into one NUL-separated
  string is `error[unknown_escape]` on `"a\0b"`.
- **`posix_spawn_file_actions_t` has no struct tag on either platform.** Darwin
  spells it `typedef void *`; glibc an anonymous struct typedef. Every shape
  tried, `partial` included, is `error[ffi_unknown_tag]`. Its size is **8 bytes
  on Darwin and 80 on glibc**, bisected with `_Static_assert` under `zig cc`.

This is a **higher** wall than panel 097's and fails differently, which is the
pragmatist's second prediction: `st_mode` is refused on exactly one of
{Darwin, glibc}, while `posix_spawn_file_actions_t` is refused on **both**.

The expressible form, which both seats converged on independently and the
engineer ran end to end:

```
hero_run_reset()
hero_run_arg(arg: str)
hero_run_go(program: cstr, out_path: cstr, err_path: cstr, @status: i64) -> i64
```

Three declarations inside the seven boundary types the FFI already admits.

## What was measured

**The quoting class, on POSIX, is deleted rather than moved.** The same argument
`a b 'c' $HOME` through both routes, measured side by side:

| route | stdout |
|---|---|
| argv | **14 bytes**, `a b 'c' $HOME`, untouched |
| shell with `sq()` | 14 bytes, correct — the hand-written defence working |
| shell without `sq()` | **20 bytes**, `a b c /Users/joseph` — three words, quotes eaten, `$HOME` expanded |

A real file named `/tmp/p098-dir/it's $HOME.hero` was created and read through the
argv route with no quoting anywhere.

**Exit codes gain a channel they did not have.**

| case | argv route | `system()` raw | decoded |
|---|---|---|---|
| exit 1 | 1, OK | 256 | 1 |
| exit 3 | 3, OK | 768 | 3 |
| missing program | **-1, NOT_FOUND** | 32512 | **127** |
| killed by SIGKILL | **137**, OK | 9 | `WIFEXITED=0`, **`WEXITSTATUS=0`** |

Two defects sit in that table. `system()`'s 127 is indistinguishable from a
program that legitimately exits 127 — and the engineer measured what that costs
today: with `clang` absent from PATH, `heroes build` answers **`internal error:
the runtime did not compile`**, exit 2, *the compiler blaming itself for a machine
with no C compiler*. And a program killed by a signal decodes to `WEXITSTATUS=0`,
which reads as success. The argv route puts "never started" and "killed" on their
own channel.

**CLAUDE.md §7's cache key is not weakened; it is improved.** The one true shell
feature in all 22 sites is the glob at `cli_toolchain.hero:129`. Its readdir
replacement was measured against it: **163,739 bytes both ways, byte-identical.**
And where the glob's `2>/dev/null` plus `_ =` returns `""` in silence — panel
097's third finding — the replacement returns a status.

**The shell was doing almost nothing.** Across all 22 sites: `>` (2), `2>` (2),
`2>/dev/null` (2), one glob. **Zero pipes, zero `&&`, zero `;`, zero `$(...)`,
zero backticks.**

## The honest cost, and it is not a footnote

**Windows has no argv at the operating-system level.** `CreateProcess` takes one
command-line *string* which the child's CRT parses back. So the quoting the
proposal deletes does not vanish — it **moves into C and changes algorithm**:
double quotes, `\"`, doubled backslashes before a quote. The pragmatist wrote
`win_quote`, compiled it clean on two mingw triples with `-Werror`, and
round-tripped seven words exactly, including `C:\Program Files\LLVM\bin\clang.exe`,
`a"b` and `trailing\\`.

That relocation moves a function that **has a test** (`cli_shell.hero:50-56`) into
C, which does not. Condition 5 answers it.

## The disagreement, recorded

The two seats differ on the ABI. The engineer prices `HERO_RUNTIME_ABI` 16 → 17 as
a cost of the change. The pragmatist measured that it **need not move**:
`examples/sqlite/main.hero`'s emitted C is 17,927 bytes with and without the three
new declarations, `cmp` clean — and then argued it should move anyway, by §7's
convention, at the price of a seed regeneration in the same commit. **Both are
right**; the resolution is that the bump is a convention being honoured, not a
compatibility break being repaired, and the commit says which.

They also differ on scope without contradicting: the engineer measured that
`tests/harness/shell.hero` is 392 lines with its **own** `system()` at `:39`, its
own `sq()` at `:53`, its own wait-status decode at `:65` and four `find -print0`
walks. Repo-wide `sq(` is **189, not the 68 the brief stated** — 121 of them in
`tests/harness/`, across 115 `shell.*` call sites in 22 modules. The brief
undercounted by a factor of nearly three, and the coordinator's own number was the
one that was wrong.

## Author's verdict

**Pending.** Queued as an open item in `docs/work/DECIDE.md` naming `panel 098`.

**What a yes settles**: the compiler stops building command lines. The route out
is `hero_run_reset` / `hero_run_arg` / `hero_run_go` behind `hero_os.h`, in its own
runtime part, with the Windows arm in C beside it; `sq()` is deleted from
`selfhost/`; the cache-key glob is replaced by a readdir that returns a status;
and `docs/work/DONE.md:766`'s revocable clause is answered *confirm* rather than
*veto*, thirteen days after v1 made it checkable.

**What a yes does NOT settle**: the scope. 121 of the 189 `sq(` uses are in
`tests/harness/`, which has its own `system()` and its own decode. Both seats say
in their own words that a milestone touching only `selfhost/` leaves Windows red
for a reason that milestone never looked at — the engineer as a prediction, the
pragmatist as panel 097's condition 7 one level out. **The author is choosing a
milestone's size, not only its direction.**

**What a no compels**: `system()` stays, `sq()` stays with it, and CLAUDE.md §12's
C-boundary paragraph keeps a hand-written parser defence on 189 filenames. The
measurement that would move either seat is the same one panel 097 named: a green
`heroes build` on Windows with `sq()` unchanged.

**What converts this to a veto, from both seats independently**: widening
`crosses_the_boundary` to admit `[str]` so the single-call form becomes writable.
That changes what a Heroes value looks like at the C boundary for **every**
binding, not just this one.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | a `selfhost/`-only milestone leaves ≥115 `sq(` uses and `tests/harness/shell.hero:39`'s `system()` intact, and the net red on Windows | the close of the milestone that lands this |
| ffi-pragmatist | `examples/sqlite/main.hero`'s emitted C stays byte-identical (17,927 bytes, `cmp` clean); one byte of difference means the rule reached into the `extern` vocabulary | the milestone that lands this |
| ffi-pragmatist | `record … tag posix_spawn_file_actions_t` is `ffi_unknown_tag` on **both** Darwin and glibc, never on one only — unlike panel 097's `st_mode` | today, no Windows needed |

## The finding that outlives the decision

**A leak in a new runtime part is invisible to both of this project's leak
instruments.** The pragmatist deleted the `free` from its prototype's reset,
rebuilt, and ran under `--sanitize`: **exit 0, empty stderr, leak gate silent.**
`hero_runtime_check_leaks()` counts `hero_alloc_block` only — `alloc.c:47` says
`hero_alloc` is *"Never counted"* — and ASan has no leak detector on Darwin arm64
(panel 021). Neither instrument is wrong about what it measures; together they
leave a hole exactly where a new runtime part allocates. That is true today,
independent of this proposal, and it is why condition 6 is written as a rule about
the gate rather than a note about care.
