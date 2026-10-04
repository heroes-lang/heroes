# Panel 192, the ffi-pragmatist's brief

Read `00-shared.md` and `00-facts.md` in this directory first; they bind you.
You judge the founding constraint: everything comes from C, design.md §1.11
and §4.19 (your charter, `.claude/agents/ffi-pragmatist.md`). You have a veto
on ABI breakage. You take **Q4 and Q5**, and you **build** what you recommend.

## Where to look, in your copy `<scratchpad>/192-ffi-pragmatist/`

- **The lend**: `hero_str_cstr` in `runtime/parts/str.c`, declared at
  `runtime/heroes_runtime.h:214`; the held copy, `hero_str_held`, beside it.
  How the emitter calls them: `selfhost/emit/ffi*`, found by a command.
- **The doors**: `runtime/parts/` (`fs.c`, `os.c`, `dir.c`, `replace.c`,
  `run.c`, `spawn.c`, `str.c`, F7). **Enumerate every call that hands a name
  or a word to the operating system yourself, from the code**: F7's grep is
  the coordinator's vocabulary and found nothing in `spawn.c`.
- **`HERO_RUNTIME_ABI`**: in the header; panel 089 held it for a new
  function. Say whether your route moves it.
- **F4's four programs**: `<scratchpad>/192-facts/nul/`, red on the base.

## What to do

1. **Q4, `.cstr()`.** Build the routes you can of (a), (b), (c) and (d) in
   your copy. For each, run F4's `nulc.hero` and `rf.hero` and say what the
   program sees. Then:
   - **Measure its price** as a count, not a duration: instructions retired
     by `/usr/bin/time -l` on this Mac, over a program that lends a long
     `str` many times and one that lends many short ones, base against route;
   - say what each costs the 215 sites of F7, the emitter, and a binding's
     author;
   - **the runtime's own calls of `hero_str_cstr`**, seven lines by `grep -rn
     'hero_str_cstr(' runtime/`: `failure.c:116`, `:135` and `:145`, each
     through a `%s` (so a failure's message holding a NUL is cut there, a
     shape beside, unrun), `replace.c:440` and `:507`, `os.c:864` (with a
     length) and `:913`. Say what each needs.
2. **Q4, the doors.** Every door you enumerated, with a name holding a NUL,
   on the base and on your route. The floor is a failure the program reads,
   never another file. F4's `qi-read.hero` and `qi-write.hero` must read that
   failure on your route. Say which failure code each door answers, and
   whether that code is in `spec/heroes-spec.md` today (`grep`).
3. **Q5, `args()` on Windows**, on the box (`ssh win`, `/c/w/192-ffi/`):
   - an argument holding a lone surrogate, through the base's narrow door;
   - through panel 191's manifest (its reports say how they built it);
   - through the UCRT's wide `argv` with a strict conversion
     (`_configure_wide_argv` or what you find).

   Say what `args()` and `args_checked()` answer under each. Your route must
   compose with panel 191's `c-dirwide`, landing beside this sitting in
   batch 11: say how. And name what the Linux and macOS halves do with an
   argument that is not UTF-8 today (`:324-325`'s abort): run it.
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
