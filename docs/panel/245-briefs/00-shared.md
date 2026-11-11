# Panel 245, the brief every seat reads first

Convened 2026-11-10 by the coordinator of batch 30, in M-core-packages, on the author's instruction
of that day, meant as: *close all the systemic defects too; hold one sitting with all of them in it*.
Without asking again for the gate (CLAUDE.md § 4: asked once per milestone, at panel 242). Seats as
panels 242 to 244 sat by the author's answer *four seats, no blind seat*: compiler-engineer,
ffi-pragmatist, spec-warden, historian, and the completeness critic before and after. The
llm-ergonomist's blind reading is a paid run and is not named here; if a route needs a specification
sentence, the synthesis queues it for the author's yes.

## The base

Batch 30's round at `7fb6868b20dfaecbdc7cf501b75b79aef7b14cf3` (`git rev-parse HEAD` in
`.claude/worktrees/b30-round`, 2026-11-10 at about 22:58; moved here from `a4b89740b` after the
critic's first pass, item 1), which holds panel 243's R1 (defect 2421, every argument passed by value
past a header's `...` refused, a door required), its R2 (defect 2428, `2a079fb89`) and its R3 (defect
2447, `b1cf5c043`), and the batch's other repairs; lane b30-shadow's defect 2458 (the notes a
package's consumer reads) is not in it. Not `main`: the questions below arise from those landings.
Panel 243 is provisional, its ratification pending (decision 2449), so Q1 and Q2 rest on a
provisional R1; under its conservative alternative, numbers only, `sprintf`'s `n: i64` is still
refused past `...`, so 2464 stands whichever way the author rules. Each seat with Bash works in its
own detached worktree `.claude/worktrees/245-<seat>` at that commit and builds its own compiler
there in two stages, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then `./heroes
build selfhost/main.hero -o heroes-next`; never another tree's binary. Probes in
`.claude/worktrees/p245-scratch/<seat>/`. No file is written outside the repository's root.

## Why these three

`heroes run tests/harness/issues.hero -- open --kind defect --class systemic`, run 2026-11-10,
lists two open `systemic` defects, 2464 and 2465. The coordinator adds 2448, `blocking`: panel 243
(§ Left open, R4) filed it as a mismatch no route of that sitting touches, so no lane can repair it
without a ruling, the case a `systemic` class exists for (`.claude/rules/verification.md` § Bounded
discovery). All three stand at the C boundary.

## The three questions

**Q1, defect 2464 (systemic)** (`gh issue view 2464 --repo heroes-lang/heroes`). A door that calls a
deprecated C function, defect 584's `sprintf` with `_FORTIFY_SOURCE 0`, builds at exit 0 and prints
clang's raw `-Wdeprecated-declarations`. Since panel 243's R1 a door is the only route to a variadic
such as `sprintf`. The unit's opening (the text `selfhost/cli/order_trial.hero:161`'s test pins)
raises `-Wformat` and `-Wformat-nonliteral` to errors and ignores `-Wformat-security` inside the
header region (`selfhost/emit/header_region.hero`), and ignores `-Wdeprecated-declarations`,
`-Wdeprecated-pragma` and `-Wattribute-warning` only after the region's close
(`selfhost/emit/deprecation.hero`), which is panel 208's R1 (`docs/panel/208-*.md:93-97`: *"ignored
over the whole unit after the groups' close, nothing else widened"*). A door's body stands inside the
region. Lane b30-variadic's recommendation: quiet the three words in the region too; its census,
*0 of 2,400 Homebrew headers in the region's shape and 1 tracked header warn today*, is the lane's,
unrun by the coordinator: run it. The lane's probe: `.claude/worktrees/b30-scratch/variadic/dep584/`
(read only). Question: what a door's call of a deprecated or attribute-warned function prints, and
where the rule lives.

**Q2, defect 2465 (systemic).** A binding with nothing past a header's `...`, such as
`printf(format: cstr lent) -> i32`, called with a format holding `%d` given at run time, builds at exit
0 and prints what the callee reads; the lane reported `6099856048` and `6138276528`, one value per
run; no file in its scratch holds them (`grep -rl` found none): reproduce it. A door has the same gap
when its format comes from outside it. Panel 243 refused the route that would read a literal
format's conversions in the compiler (its (R), the spec-warden's veto on Principle 0), kept
`-Wformat-nonliteral` an error in the region, and kept `-Wformat-security` ignored there (defect
2036; its four `run` cases, `ls tests/golden/run/ | grep fixedbugs-2036`, call `printf` or
`snprintf` in a header's own code, one with a format held in a variable). Question:
whether a format function may be bound directly at all, or only through a door holding its format
literal where `-Wformat` judges it; and what a program that prints a run-time string writes then.

**Q3, defect 2448 (blocking, no route).** A program binding `extern "curl/curl.h" link "curl"` built
with `CPATH=/opt/homebrew/opt/curl/include` (Homebrew's keg-only libcurl 8.23.0) prints `header 530176`
beside `library libcurl/8.7.1`, linked to `/usr/lib/libcurl.4.dylib`, at exit 0; panel 243's critic
measured readline 8.3's header against libedit under `CPATH`, its ffi-pragmatist `--include`. The
coordinator's reproducer: `.claude/worktrees/b30-scratch/repro/mismatch/` (read only). design.md
§4.19 (`docs/design.md:3186` on, *"The environment is a named folder"*, defect 2344, panel 055)
reads `CPATH`, `C_INCLUDE_PATH` and `CCC_OVERRIDE_OPTIONS` from clang's own answer and treats such a
folder as named; panel 243's R2 made a folder a `pkg-config` answer names its library's. Question:
when a header found through the environment or `--include` describes another release than the
library `link` names, what the build does, and on what it rests (a header and a library carry no
shared version in general).

## How the sitting runs

- Every number, path and count you write is produced by a command you run while writing it, and you
  say which; a negative sentence is run or written as a question.
- One heavy command at a time in your worktree; never two in parallel. A run that may not end is a
  built binary under `timeout`, its output bounded.
- No paid run, no credential printed (never `gh auth token`), no GitHub write, no push, no edit of
  the tree outside your own worktree and your report.
- Write your report to `docs/panel/245-reports/<seat>.md` in `.claude/worktrees/p245` as you go; if
  the tool refuses that write, your final message is the report, whole.
- A route you recommend is one you built and ran, in your worktree, on the cases you name.

## After the critic's first pass (2026-11-10, `245-reports/critic-briefs.md`, 25 items)

Read the critic's report whole; it overrides what is written above where they differ. Every
platform sentence in this sitting is this Mac's; Linux and Windows are unrun by every seat (item 24).

**The class of 2448.** It stays `blocking` as filed; the sitting may reclassify it, and says so
(item 5).

**Q1, widened (items 6 to 10).** Reproduced: `dep584` exits 0 with clang's raw
`-Wdeprecated-declarations` and prints `2`. Panel 208's R1 already rules the outcome, *"a name its
header marks deprecated binds and builds silent"*, one of its own `run` cases `sprintf` with
fortification off building silent; its mechanism, *"after the groups' close, nothing else widened"*,
now disagrees with that outcome for a door's body, so the sitting says which half binds. The class
is wider than the three deprecation words: inside the region clang's raw words at exit 0 for
`-Wattribute-warning` (default level only), `-Wshift-count-overflow` (`1 << 40` prints `2` at `-O0`
and `10` at `-O2`), `-Warray-bounds` (`a[5]` of an `int[2]` prints `2` and `0`),
`-Wconstant-conversion`, `-Wbuiltin-memcpy-chk-size`, `-Wpointer-sign`, a header's `#warning` and
`#pragma message`: warnings on by default and outside `-Wall`, which panel 205 measured. Two are
undefined behaviour at exit 0, robustness rank. Routes nobody listed: (a) ignore `-Weverything` in
the region, then raise the errors again; (b) the undefined-behaviour words made errors and the
advice words quieted, with a test that fires when clang's default list moves; (c) the region read
in clang's system-header mode, after measuring whether a pragma `error` still holds there. The
lane's census counted the deprecation words in its saved `-Wformat-nonliteral` census outputs: 0 of
the 1,063 Homebrew headers that compile in the region's shape (1,337 of the 2,400 are refused), and
1 tracked header, `unsupported/fixedbugs-158-noted-outer.h`; re-run it counting every word above.
Five carrier headers of defect 158's `unsupported` cases rely on the region speaking a warning
(`noted-mid.h`, `warned.h`, `warned-mid.h`, `warned-outer.h`, `sibling-a.h`): a route that quiets
more must say what those cases then test.

**Q2, rewritten (items 11 to 16).** The lead fact above is false at the base: a direct binding
`printf(format: cstr lent) -> i32`, called with a run-time, literal, leased or `getenv` format, or
never called, exits 1 `ffi_call_shape` (*"format string is not a literal at this shape"*,
`-Wformat-security`, `emit/ffi_call.hero:111-155`, since `ebed97b04`, 2026-09-03), and so do
`dprintf`, `fprintf`, `snprintf`, `syslog`, `sscanf`, `curl_mprintf` and a door of the program's own
marked with clang's `format` attribute. What reproduces: (i) a door forwarding a format with no data
arguments, `say(f) { return printf(f); }`, builds at exit 0, `n=%ld` printing `4305175264` then
`4366533344`, eight `%s` dying at exit 134 (*"a null pointer was read through"*), `%n` of SIGABRT,
and a door handing `sscanf` a run-time `%d %d` writes an `int` through an address nobody passed,
exit 0, ASan silent under `--sanitize`; (ii) a variadic with no `format` attribute bound with
nothing past `...`, `sqlite3_mprintf` (SDK `sqlite3.h:3273`) printing `n=361761723`. The class is
not format functions: `open(path: cstr lent, flags: i32)` with `O_CREAT + O_WRONLY` and no mode
exits 0 and creates the file with a mode read from the `va_list`; the question is every variadic
whose callee reads past `...` by a run-time value (`open`, `fcntl`, `ioctl`, `execl`,
`sqlite3_config`), a rule keyed to `format` missing `open`. design.md's Part 7 row 10 said a
forwarding door is refused; corrected at `7fb6868b2` to *with data arguments*. The trade with
defect 2036 is between two `blocking` classes: raising `-Wformat-security` in the region refuses
2036's correct program (a format held in a variable), keeping it ignored keeps a memory fault at
exit 0, and CLAUDE.md § Precedence ranks robustness first; panel 243's spec-warden veto fell on a
format exception that admits a form, not on a refusal. The lane's figures `6099856048` and
`6138276528` stand in no file of `b30-scratch/variadic/`.

**Q3, widened (items 17 to 23).** Reproduced, and in both directions, every build at exit 0:
`CPATH`, `C_INCLUDE_PATH` or `--include` alone gives header 8.23.0 beside library 8.7.1;
`--library` or `LIBRARY_PATH` alone gives header 8.7.1 beside library 8.23.0; both halves named pair
them. Panel 055 (design.md:3125) named `LIBRARY_PATH` beside `CPATH`, and no module of the compiler
reads it. The compiler's own note leads into 2448: `machine_locked_path` says only *set `CPATH`*
(`cli/head_names.hero:222`), and followed for readline builds at exit 0 printing header `2051`
linked to `/usr/lib/libedit.3.dylib`. Under `CPATH` a door including `<curl/curl.h>` is refused
`shadowed_header` while a group naming that header directly builds. A paired build run with
`DYLD_LIBRARY_PATH=/usr/lib` prints header 8.23.0 beside library 8.7.1 at exit 0: what a build can
promise stops at the link. The harm 2448's class rests on is an inference: what the critic measured
is a function only the newer header has refused at the link, and `CURLVERSION_NOW` reading `11`
against a library whose newest stamp is `10`; a seat brings one measured wrong value or memory
fault from a mismatch, or says none was found. Routes nobody listed, each checkable without knowing
the library: (P1) pair by provenance, where clang found the header (`clang -v -M`) against where the
linker found the library (panel 239's R1 link trace, `cli/supplied.hero`), Windows weakening a
`LIBRARY_PATH` half (lld-link ignores it, design.md:3128-3133); (P2) pair by prefix,
`<p>/include` with `<p>/lib` after resolving links, which Nix's split packages would refute (unrun);
(P3) ask `pkg-config` by the folder, a `.pc` whose `includedir` is that folder giving the `libdir`
the link must take; (P4) `machine_locked_path`'s note naming both halves, whatever the ruling.

**Found beside, filed by the coordinator, not this sitting's questions**: `ffi_missing_link` under
`CPATH` for a function only the newer header has, saying no group names its library though the
group says `link "curl"` (item 20); `ffi_return_type` naming a function type as a result type for
`curl_easy_setopt(handle: ptr, option: u32) -> u32` (item 25).
