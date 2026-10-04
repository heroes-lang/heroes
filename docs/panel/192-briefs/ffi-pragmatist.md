# Panel 192, the ffi-pragmatist's brief

Read `00-shared.md`, `00-facts.md` and the critic's first pass
(`docs/panel/192-reports/completeness-critic-briefs.md`) first; they bind
you. You judge the founding constraint: everything comes from C, design.md
§1.11 and §4.19 (your charter, `.claude/agents/ffi-pragmatist.md`). You have
a veto on ABI breakage. You take **Q4 and Q5**, and you **build** what you
recommend. *Repaired after the critic's first pass: the text it read is
`ffi-pragmatist-before-the-critic.md`.*

## Where to look, in your copy `<scratchpad>/192-ffi-pragmatist/`

- **The lend**: `hero_str_cstr` (`runtime/parts/str.c:434`, declared at
  `runtime/heroes_runtime.h:214`), and the held copy, `hero_str_held`,
  beside it. **The emitter writes the lend at `selfhost/emit/inst.hero:316`**
  and the null guard on every `cstr` argument at `selfhost/emit/ops.hero:305`;
  `emit/builtins.hero` and `check/lend_types.hero` name them too (the
  critic; the first brief's `selfhost/emit/ffi*` holds none of them).
- **The runtime's own calls of `hero_str_cstr`**: `grep -rn 'hero_str_cstr('
  runtime/` prints nine lines, the declaration, the definition, and seven
  call lines holding ten calls (F4):
  - **four pass content with its length, where a NUL is correct today**:
    `os.c:864` (`hero_file_write`), `:913` (`hero_write_err`),
    `replace.c:440` (`hero_stage_fill`) and `:507` (`hero_file_stage`).
    **A check-and-abort inside `hero_str_cstr` makes `write_file` abort on a
    correct program**;
  - **three print through `%s`** in `failure.c` (`:116`, `:135`, `:145`), so
    a failure's message holding a NUL is cut there (run by the critic, F4).
- **The doors**: `runtime/parts/` (`fs.c`, `os.c`, `dir.c`, `replace.c`,
  `run.c`, `str.c`). **Enumerate every call that hands a name or a word to
  the operating system yourself, from the code**: F7's grep is a vocabulary,
  and the critic's wider one (`open(`, `CreateFileA`, `chmod`, `link(`) found
  at least 21 more lines. Every door of `hero_os.h` (`:60` to `:338`) takes
  `const char *`, so the prelude (`selfhost/library_source.hero:208`, `:222`)
  and the compiler (`cli/files.hero`, `cli/process.hero`,
  `module/reading.hero`) lend `.cstr()` first. **One door takes the `str`
  and refuses a NUL itself, by a panic**: `hero_run_arg`, `run.c:76`.
- **Where a `str` is built from outside bytes**: by the critic's vocabulary,
  the file reads (`hero_file_read`, `os.c:814`, and `hero_file_read_shown`)
  and the shown builders (`os.c:649`, `:703`) take an explicit length; every
  other constructor goes through `strlen` or `memchr` (F4). **Measure it**:
  it is a negative claim.
- **`HERO_RUNTIME_ABI`**: 26 at `heroes_runtime.h:58`, asserted at
  `selfhost/emit/decls.hero:83`; panel 089 held it for a new function. Say
  whether your route moves it.
- **F4's four programs**: `<scratchpad>/192-facts/nul/`, red on the base.

## What to do

1. **Q4, `.cstr()`.** Build the routes you can of (a) to (e) in your copy.
   For each, run F4's `nulc.hero` and `rf.hero` and say what the program
   sees. Then:
   - **measure its price** as a count, not a duration: instructions retired
     by `/usr/bin/time -l` on this Mac, over a program that lends a long
     `str` many times and one that lends many short ones, and for route
     (d) over `read_file` of a large file, base against route;
   - say what each costs the lends in code a gate builds (count them with
     `heroes lex`; F7's 215 is a count of lines, comments and docs
     included), the emitter, and a binding's author;
   - say what each needs at the seven call lines above.
2. **Q4, the doors.** Every door you enumerated, with a name holding a NUL,
   on the base and on your route. The floor is a failure the program reads,
   never another file. F4's `qi-read.hero` and `qi-write.hero` must read that
   failure on your route.
   - Say which failure code each door answers, and whether it is in
     `spec/heroes-spec.md` today (`grep`).
   - `hero_run_arg` answers a panic: precedent or not?
   - The prelude lends `path.cstr()` before the door runs, so say which
     check runs first, and in what code.
3. **Q5, `args()` on Windows, on the box**, only once the coordinator's
   message says the box is free (`ssh win`, `/c/w/192-ffi/`):
   - an argument holding a lone surrogate, through the base's narrow door;
   - through panel 191's manifest (its reports say how they built it);
   - through the UCRT's wide `argv` with a strict conversion (1113);
   - through the wide `argv` converted to WTF-8 (panel 191's
     compiler-engineer's `hero_win_name_bytes`), so the existing check
     refuses.

   Say what `args()` and `args_checked()` answer under each. Your route must
   compose with panel 191's `c-dirwide`, which lane b11-windows is landing
   beside this sitting (`.claude/worktrees/lane-b11-windows`; read it,
   never write or build there): say how. On this Mac and on Linux arm64, run
   what `args()` does with an argument that is not UTF-8 today (`:324-325`).
4. **The platforms.** A change in `runtime/` runs its cases on each platform
   (`.claude/rules/platforms.md`): this Mac, Linux arm64 in Docker (one
   container, stopped after), and the box. Say which you ran.

## Your report

`docs/panel/192-reports/ffi-pragmatist.md` in the trunk, written as you go. A
verdict per question, with:
- the C you wrote and compiled;
- what each route does to F4's programs;
- your cost;
- a falsifiable prediction;
- the condition that would change your verdict.
