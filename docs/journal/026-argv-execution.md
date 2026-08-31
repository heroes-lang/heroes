# 026 — M-argv-execution: the shell stops being the boundary

Opened 2026-08-30 on the ratification of panels 097 and 098, closed
2026-08-31. Eight steps plus the two commits that opened it. The one-line
version: the compiler used to talk to the operating system by composing a
sentence for `/bin/sh`, and now it hands the operating system a list of
words.

## Goal

Windows died at the compiler's **first** contact with the machine. `mkdir -p
build` reached `cmd.exe`, which answered *"A subdirectory or file -p already
exists"* and created a directory called `-p`. Everything after that was
downstream of a POSIX sentence arriving at a shell that does not speak POSIX.

The route the two sittings ratified is not "quote better for cmd.exe". It is
to stop composing sentences: a program is executed by **argument list**, and
the platform arm lives in the runtime, in C, where a `#if defined(_WIN32)`
is an ordinary thing to write. `selfhost/` gets no platform axis at all
(DESIGN-LOG:280 refused one in the language; :282 settled that the same class
of question is answered in the driver).

What that buys, measured at close:

| | before | after |
|---|---|---|
| `sq()` — the POSIX quoter, maintained by hand | 189 uses repo-wide | **0** (the 5 remaining greps are comments naming it in the past tense) |
| `shell()` calls in `selfhost/` | 20 across 7 modules | **0** |
| `system()` in `tests/harness/shell.hero` | its own `extern`, its own quoter, its own wait-status decode | **0** — all three gone |
| Windows CI | dies at step 8 of 18 | passes `doctor`, compiles, runs, diagnoses, checks, **emits** |

## What surprised

**A shell was hiding three platform-independent defects, and only Windows
could expose them.** Every one of them was in code that runs on Darwin and
Linux every day, and none of them could be seen there:

- `exit_of` read a failure's **code** and discarded its **message**. On a
  platform where nothing fails, a discarded message is invisible.
- `_ = art?` propagated an emission failure **without a word**. `heroes
  build --emit-c` exited 1 with an empty log, and four rounds of CI went into
  finding out which of five stages it had died in.
- All **eight** stopping points in `cli_compile` named themselves "a stage".

The rule underneath is worth keeping: a diagnostic channel that is never
exercised is a diagnostic channel that does not work. Windows was not the
bug. Windows was the first reader.

**`CreateProcess`'s two ways of finding a program are not variations on one
theme.** With `lpApplicationName` set, the string is an exact path: no PATH
search, no extension appended. With it `NULL`, the documented search runs and
executable extensions are tried. The compiler needs the second one, because
`clang` on Windows is `clang.exe` somewhere on PATH — so the `NULL` is
load-bearing, and the command line has to carry the program name as its
first word, quoted by the CRT's own rules (doubled backslashes before a
quote). That is why `win_quote` exists and why panel 098 made it ship with a
round-trip test: it replaces a function that had one.

**A bootstrap ABI bump propagates over two generations.** Raising
`HERO_RUNTIME_ABI` before regenerating the seed locks the compiler out of its
own build. And the fix is not "reverse the order" — a compiler that knows how
to *write* the new number still carries the old one on its own forehead, so
the number moves through the seed in two passes, not one. It sits at 17.

**Windows has no argv at the operating-system level.** A process receives one
string and parses it itself. Every language that looks like it passes a list
on Windows is running somebody's implementation of the CRT's rules — and
those rules are not the shell's.

## What broke and why

- **`records/c` caught the diagnostic probe littering.** A CI step written to
  find out what Windows was printing wrote `emitted.c` into the repository
  root, and the net — which runs on the same working tree one step later —
  failed on **all three** legs with *"a C file outside the directories that
  own C"*. It was right. A diagnostic probe is not exempt from a record rule,
  and a probe that litters breaks the run it exists to diagnose. Fixed by
  writing everything under `build/`, which is gitignored.

- **A fixed 8192-name ceiling, copied from argv reasoning into a directory
  walk.** 8192 is a sane bound for a command line and nonsense for a file
  listing: `build/` holds **37,240** files. The list grows now. This is
  CLAUDE.md §11's shape exactly — a premise that was true where it was
  written and false where it was copied.

- **168 leaked scratch buffers in `dir.c`**, caught by the leak counter added
  that same morning (commit `19b024c`, panel 098's condition 6). The gate was
  built for this milestone and fired on this milestone's own code within
  hours. `hero_dir_release` is the fix.

- **A sentence pushed as one word.** The surface table's `argv` is prose and
  the runtime wants words, so `"lex file --dump-tokens"` arrived as a single
  argument and 240 checks failed. Split on spaces — and the split is exact
  rather than a guess, because all 91 rows were checked for an argument
  containing a space before it was written.

- **A cross-compile with `-Wall` only.** The project builds with 13 flags,
  and `-Werror=sign-conversion` was the one that found the real defect in the
  Windows arm. From here on the cross-compile uses the real flag set.

- **`> NUL` under PowerShell.** cmd's black hole is PowerShell's ordinary
  file, so a probe redirecting there may have been discarding the very output
  it was asking for.

## Predictions, scored

| sitting | judge | prediction | outcome |
|---|---|---|---|
| 097 | compiler-engineer | `runtime/parts/os.c` crosses 300 lines if the five operations land there (245 today) | **confirmed by avoidance** — `os.c` is still **245**, because 097's file rule sent the arm to new parts: `fs.c` 160, `run.c` 367, `dir.c` 227 |
| 097 | ffi-pragmatist | `extern "sys/stat.h"` in `selfhost/` would put `#include <sys/stat.h>` into `seed/heroes.c` and kill the Windows leg at the seed build | **honoured by construction** — condition 9 held; the only `sys/stat.h` in `selfhost/` is inside test fixtures written as strings |
| 097 | ffi-pragmatist | a `record … tag stat` naming `st_mode` at any width is `ffi_field_type` on exactly one of {Darwin, glibc} — never zero, never both | **confirmed at the sitting**: 2 bytes on Darwin against 4 on glibc |
| 098 | compiler-engineer | a `selfhost/`-only milestone leaves ≥115 `sq(` uses and `shell.hero:39`'s `system()` intact, and the net red on Windows | **the counterfactual was not run, and it is why**: the prediction is what widened the scope to `tests/harness/`. Measured at close: 0 live `sq(`, `system()` gone, `shell.hero` rewritten at 530 lines |
| 098 | ffi-pragmatist | `examples/sqlite/main.hero`'s emitted C stays byte-identical at **17,927** bytes; one byte of difference means the rule reached into the `extern` vocabulary | **confirmed to the byte** — 17,927, `cmp` clean |
| 098 | ffi-pragmatist | `record … tag posix_spawn_file_actions_t` is `ffi_unknown_tag` on **both** Darwin and glibc, never on one only | **confirmed at the sitting** — no struct tag on either platform, 8 bytes against 80 |

The 097 prediction about the Windows leg failing at *"`cc`/`clang` not found
rather than at a directory named `-p`"* was made about **the proposal as
written** — the narrow FFI-only route. That route was not taken, so the
prediction has no run to be scored against. It is not renewed under a new
milestone name (panel 046 R2): it is marked **lapsed**, and what it was
pointing at was answered anyway by step 3, which is the step that exists
because `CreateProcess` has to be told to search PATH.

## What landed, and what carried forward

Thirteen ratified conditions, all met. The runtime gained three parts —
`run.c` (execute by argument list, both arms), `fs.c` (the five filesystem
operations), `dir.c` (the directory walk that replaced four `find -print0`
pipelines) — and `hero_os.h` went from 6 declarations to 23. `cli_shell.hero`
became `cli_process.hero`, `cli_verbs.hero` shed 164 lines to a new
`cli_produce.hero`, and `cli_toolchain.hero` gave up its cache-key reader to
`cli_runtime_key.hero`.

What carried forward is the one thing nobody here can measure: **nobody on
this project has a Windows machine.** Every Windows fact in this journal came
from a `workflow_dispatch`, and the loop is minutes long rather than seconds.
That is the cost of the platform, not of the design, and it is the reason the
milestone's last step is a CI run rather than a local one.
