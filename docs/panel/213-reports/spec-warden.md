# Panel 213, the spec-warden's report

Written 2026-10-13 in my own copy, `.claude/worktrees/scratch-panel-213/spec-warden`
at `0b8307daf` (`git worktree add --detach ... 0b8307daf`), its compiler built
from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`).
Every number below carries the command that produced it, run in that copy.
No paid run: `heroes measure` offline only, so every `real` figure is the
pinned one (`REAL_TOKENS`, taken 2026-10-12, digest-matched).

- `verdict`: **object**, not veto. No spec token moves, so §1.6 has nothing to
  veto, and Principle 0 governs language forms, of which none enters. The
  objection is to the routes that add surface (D, a flag or a verb; B, a second
  artifact per build) and to any sentence in the spec. **I would adopt C**,
  formatters carried in the binary, which I ran tonight on this Mac's lldb for
  the record shape with no flag, no file and no script (§ 4). F as an emitter
  change goes with it. G or J only for what C cannot reach, once that is measured.
- `section`: design.md §1.6 (the budget, line 255: 10240, grepped today); §1.0
  and CLAUDE.md § 2 (Principle 0); `.claude/rules/cli-surface.md` § One command
  (panel 016's stopping rule); design.md Part 2, line 630; §3.1's mangling bullet.
- `spec_token_delta`: **0 for every route, measured before**: 7520 legacy /
  7655 cl100k / `real 10050`. No after exists, because no route writes the spec.
- `removal`: nothing. Nothing is added to the spec, so §1.6 is owed nothing.
  The surface has no payment rule. It has an admission rule, and D fails it (§ 3).
- `needed_for_self_hosting`: **no**. The fixpoint was reached 2026-08-18 without it.
- `argument`: the spec is the prompt for writing programs, and a debugger reaches only a program that already compiled and ran, so a spec sentence on it would buy nothing under §1.2 and spend part of 130 free tokens. No form enters, so Principle 0 is not engaged. Panel 016's stopping rule refuses D: nothing must type a debug word, and `build -O0` then `lldb` already compose. B moves the typing to lldb's prompt, in Python, and a stale file shows false values. C ran tonight: stock lldb reads a record summary from the binary's own section, with no flag, file or script. The warrant is the author's, and the record should say so.
- `prediction`: § 6.
- `condition`: § 7.

## 0. The ceiling, grepped today

`grep -n "### 1.6" docs/design.md` -> 253; line 255: *"must fit in 10240
tokens, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`"*.
That agrees with the brief.

## 1. Does any route move a spec token? No.

- `grep -n -i -E "debug|lldb" spec/heroes-spec.md` -> nothing, exit 1.
- A wider search, `grep -n -i -E "dwarf|-O0|-O2|breakpoint|inspect|formatter|\bgdb\b|#line|mangl"`
  -> nothing, exit 1.
- The spec names no `heroes` verb and no flag:
  `grep -n -E "heroes (build|run|test|check|fmt|doctor|measure)|\-\-[a-z]"` -> nothing.
  Its thirteen sections (`grep -n "^#"`) cover files, literals, types,
  declarations, bindings, `T?`, operators, control flow, calls, strings/arrays/maps,
  built-ins, tests and holes, and FFI. The tool surface has never been the spec's.
- `./heroes measure spec/heroes-spec.md` -> `claude-legacy 7520`,
  `cl100k_base 7655`, maximum 7655, spread 135, `real 10050` (claude-opus-5,
  2026-10-12; no STALE line, so the digest matches), *"Headroom: 190 against
  the 10240 ceiling — but the FFI floor mortgages 60 of it"*. That leaves
  **130 free tokens.**

Routes A to J, B without Python and gdb each act on one of these: the emitted
C, the debug information, a section of the binary, a file beside it, the
toolchain, the `heroes` argv table, or the editor extension. None changes what
a Heroes program means, what the lexer, grammar or checker do, or a diagnostic
class. **The strongest reason against putting anything in the spec**: the spec
is the prompt for writing a program, and a debugger only reaches a program that
already compiled and ran. A sentence about stopped frames would buy no
first-attempt compile and would spend part of the 130 free tokens. So any
resolution that writes debugger text into `spec/` is refused here: it would be
cost with no §1.2 return.

The contract, in case the ruling amends CLAUDE.md § 10:
`./heroes measure CLAUDE.md` -> cl100k 6186, `real 8167` (2026-09-29),
*"Headroom: 4121 against the contract's own 12288"*. The rule there is to move
a rule, never to grow the number. So whatever the sitting rules about *never a
script* belongs in `.claude/rules/cli-surface.md`
(`./heroes measure .claude/rules/cli-surface.md` -> 795 vendored, *"No ceiling
judges"* it), not in CLAUDE.md.

## 2. Principle 0, read

- **Not engaged as written.** CLAUDE.md § 2 and design.md §1.0 decide which
  **forms** enter the language. No route adds a form, a rule of the checker or a
  diagnostic class. On the tool side the twin rule is panel 016's stopping rule
  (§ 3). The compiler has **no size budget**: design.md §1.6's last paragraph
  says so, and only the per-file ceiling binds.
- **The thesis branch has no instrument, and the milestone page cites one that
  cannot see this.** The page says *"the only instrument that could measure it
  is Part 11's metric 4"*. Metric 4 (design.md Part 11, item 4) is
  *"Given a broken program, how many exchanges to make it compile"*. A program
  that does not compile never stops in a debugger, so metric 4 cannot see a
  stopped frame, and no other Part 11 metric (tokens, first-try, silent-error)
  can either. Any claim that typed inspection lowers a rewrite rate is
  therefore **an observation that pays nothing** (§1.6, panel 046's standard).
- **So the warrant is the author's**: the scheduling of 2026-09-06 and the
  answers of 2026-10-12 (issue 2029). The resolution should say exactly that,
  as panel 179 did for `heroes probe` (*"The admission is the author's decision
  of 2026-09-26, stated as that and not stretched from the stopping rule"*,
  read in `docs/panel/179-*.md`). It should not dress the milestone up as thesis.
- **Compiler code each route would cost, with nothing admitting it:**
  - D: an argv row, spawning lldb, finding lldb on three platforms, and a
    doctor row.
  - B: a second generated language, Python, which must be printed
    byte-deterministically (CLAUDE.md § 7) and tested.
  - C and F: an emitter module and no new surface.
  - G and J: no compiler code.
  - E is refused on robustness before cost: design.md §3.1 mangles every user
    name *because* `default`, `register`, `index` and `y1` collide with C, and
    a field named `errno` or `stdin` would meet a libc macro.
- **design.md Part 2, line 630** (*"No typed variable inspection in v1"*;
  `grep -n "typed variable" docs/design.md` -> 630, inside Part 2 at 614 to
  639). This sitting has to rule it, not price it. My reading: **amend it, do
  not retire it.** Adopting C makes the sentence's *"`p x` shows a mangled C
  temporary"* false where lldb reads the section, so the same change rewrites
  it. Three things stay true and stay in the sentence:
  - the line half;
  - *"printing rich values is still `print`'s job"*, which is
    M-reflection-verdict's question by the milestone page's own words;
  - what is **not** delivered: `-O2` (line tables only, `flags.hero`
    `debug_words`), Windows (`lldb.exe` exits 0xC0000135, `ci.yml:487`), and
    in-process rendering.
  Its *"in v1"* has been history since the fixpoint. None of this is a spec token.

## 3. The stopping rule, surface by surface

Panel 016's rule (`.claude/rules/cli-surface.md`): *"A capability enters the
surface only if the fixpoint invocation, the golden harness or the design.md
Part 11 harness must type it, or it has a measured Part 11 effect"*. Then
subcommand, flag, or **nothing if two existing invocations already compose to
it**.

The golden harness's own debugger suite already composes. Read in
`tests/harness/suite_lldb.hero`, it runs `[compiler, "build", source, "-O0",
"-o", binary]` (line 151) and then `lldb --no-lldbinit --no-use-colors
--batch` (line 180). Nothing in the fixpoint or the harness has to type a
debug word, and Part 11 has no effect to measure (§ 2).

- **A flag** (`heroes run --debug`): **not admitted.** Nothing must type it,
  and `heroes build x.hero -O0 -o x` followed by `lldb x` composes to it, which
  is the rule's *nothing*. `run` defaults to `-O2` with
  `-gline-tables-only` (`flags.hero:169-172`), so the flag would also silently
  change `run`'s level. It could enter only by the author's admission, stated
  as such (panel 179's form).
- **A verb** (`heroes debug`): **not admitted**, for the same reason and a
  second one. A new verb needs *"a proven overload of an existing one"*, and
  nothing is overloaded here.
- **A file beside the binary** (B): adds no argv, but it is a second artifact
  from `build`, whose row says *"compile to a native binary"* and whose `-o`
  says *"write the artifact here"*. On *never a script*:
  - **The letter** of CLAUDE.md § 10 names three ways of reaching a capability
    outside the one command (a second binary, a script, a Makefile). A file the
    build writes is an output, like the `.c` under `build/`, so the letter does
    not reach it.
  - **The substance** reaches it. The premise *"nobody types"* is false on the
    command line: lldb loads B only through `command script import`, a
    `.lldbinit` line, or the extension's `initCommands`. The suite above runs
    `--no-lldbinit`, so it would have to type the import, and so would the
    author at lldb's prompt. B moves the typing from argv to lldb's prompt, in
    a second language.
  - **Its failure mode is a false display**: a `.py` from one build beside the
    binary of another shows a value that is not there. That is the
    *false message* class `.claude/rules/verification.md` § Bounded discovery
    calls `blocking`.
  - **Word collision, so nobody cites it either way**: design.md §3.5's
    *"a separate formatter"* is the code formatter (`heroes fmt`), not an lldb
    formatter.
- **Nothing** (C, F, H, I as emission changes; A): **admitted by
  construction.** `build -O0` answers the same question about the same input,
  and no argv, file or script is added. Their cost is emitter code and
  binary bytes, neither of them budgeted. Determinism (CLAUDE.md § 7) holds,
  because a summary is derived from type names alone.
- **The editor** (J): outside § 10. `editors/` ships its own artifact, as the
  site does. If C holds through lldb-dap, `editors/README.md`'s *"with no change
  here"* becomes true rather than a premise (unrun, § 5).
- **A doctor row for Windows**: no argv and no spec token, so it is surface
  only in `doctor`'s output and needs no admission. Its shape (a
  platform-conditional row, as `linker_home` already makes) is the critic's
  question, not mine.

## 4. Route C, run: the binary carries its record summary, and stock lldb reads it

`build/routec/emb.c` in my copy is a C program shaped like the emitter's
output, `typedef struct h_rooms_Point { long long f_x; long long f_y; }` with
a local `h4_p`, plus one record in `__TEXT,__lldbsummaries`:

```c
__attribute__((used, section("__TEXT,__lldbsummaries")))
unsigned char h_rooms_Point_summary[] =
    "\x01" "\x34" "\x0e" "h_rooms_Point\0" "\x24" "Point(x: ${var.f_x}, y: ${var.f_y})";
```

`clang -g -O0 emb.c -o emb` gives `otool -l emb` a `__lldbsummaries` section of
size 0x36. Then:

```
lldb --no-lldbinit --no-use-colors --batch -o "breakpoint set --file emb.c --line 12" -o run -o "frame variable" -o "p h4_p" emb
(h_rooms_Point) h4_p = Point(x: 3, y: 4) {
  f_x = 3
  f_y = 4
}
```

This was **lldb-2103.0.34.103, with no `.lldbinit`, no Python, no file beside
the binary and no command typed.** The record half of the milestone's four
failures is reachable with zero tool surface. The section lives in `__TEXT`,
not in the DWARF, so it moves with the binary. The DWARF itself stays in the
`build/` objects, as the critic found.

What this does **not** show:
- **Containers**: a summary string cannot walk a `[T]` or `{K: V}` of run-time
  length, and cannot choose a `T?`'s live arm. Those need synthetic children,
  which means Python (B, G) or LLDB's formatter bytecode in
  `__lldbformatters`. The critic found that string in this Mac's LLDB framework.
  I did not run the bytecode.
- **The frame's flood** is F's and the `$` distinction's, untouched by C.

## 5. What I could not run

- lldb-dap reading the embedded section, so the extension half is unrun.
- The Linux image's lldb on the ELF `.lldbsummaries` section.
- `__lldbformatters` bytecode.
- Windows, where `lldb.exe` dies before any command, so no route reaches it.
- The `real` count of any after-state: none exists, and `--refresh` is paid.

## 6. Prediction (instruments that exist today, scored at M-typed-inspection's close)

1. **Tokens**: no commit of M-typed-inspection touches `spec/`. Scored by
   `git diff <milestone base>..<close tag> -- spec/heroes-spec.md` limited to
   the milestone's commits, which should be empty, and by `heroes measure`
   reading the same vendored pair, 7520 / 7655, before and after those commits.
   Any non-zero delta falsifies it.
2. **Surface**, if C is adopted:
   - `grep -c "Command(" selfhost/cli/table.hero` stays **13**.
   - `build`'s and `run`'s flag lists in `heroes help` stay what they print
     today.
   - The `lldb` suite's `locals` case shows the record `p` with the author's
     field names `x` and `y` in the line that today pins
     `h0_p = (f_x = 3, f_y = 4)`.
   - The suite's invocation (`build -O0`, then `lldb --no-lldbinit --batch`) is
     unchanged.
   Scored by `./heroes run tests/harness/main.hero -- ./heroes lldb`.
3. **Rewrite rate**: registered as an **observation that pays nothing**. No
   Part 11 instrument sees a stopped frame (§ 2).

## 7. What would change my verdict

- **To veto**: a resolution that writes debugger, formatter or flag text into
  `spec/heroes-spec.md` with no named removal or admissible prediction. Or one
  whose priced after-state (a paid `--refresh` the author approves) passes
  10240 minus the FFI floor's 60.
- **To approve D**: the author admitting the flag or verb by decision, stated
  as such, or a Part 11 instrument that measures a debugging effect and a
  measurement through it.
- **To approve B over C**: a measurement that `__lldbformatters` bytecode is
  unsupported where the milestone must deliver (this Mac or the Linux image),
  so that containers need Python. In that case I would take **G** (one file per
  toolchain, keyed on `HERO_RUNTIME_ABI`, which cannot drift per build) before
  B per build.
