# Panel 213, the completeness critic, pass 1: the briefs

Run 2026-10-12 to 13 in my own copy, `.claude/worktrees/scratch-panel-213/critic`
at `0b8307daf` (`git log --oneline -1`), its compiler built from the seed
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, real 7.73 s).
Every finding below names the command that settles it and what it printed.
Ordered most consequential first. No verdict on the proposal.

## 1. The ergonomist's experiment is steered, four ways

**1a. frame-A shows a variable the program cannot name at that stop.** Line 12
is `return winner`, after the loop; frame-A lists `p = Player(name: "alan",
score: 4)`. In Heroes `p` is out of scope there:

```
$ printf 'function main()\n    xs = [1, 2]\n    for p in xs\n        print(p)\n    print(p)\n' > scope.hero
$ ../../heroes check scope.hero
error[unknown_name]: nothing named `p` is in scope
  at scope.hero:5:11
exit 1
```

So frame-A is not "the author's view of the same stop": it is one rendering
choice (hoisted locals shown regardless of scope), which is exactly the
question the milestone page leaves to the sitting (*"a slot table beside the
binary is what buys scope, and that is a panel question"*, M-typed-inspection.md
§ the second step).

**1b. frame-A renders the maximal route, so the experiment can only rank "all
against nothing".** It hides `h3_xs0` and `h4_i0` (the lowering's `$` slots,
which needs the `$` distinction kept past the mangler, an open question), hides
`t1` to `t27` (route F), renames (E or a formatter) and renders `[Player]` (B,
C or D). No frame stands for E alone (author names, C values) or F alone, so the
seat's result cannot separate the routes the sitting must choose between.

**1c. The program gives the bug away without any frame.** `winner @ p.name`
sits at the `if`'s indentation in `program.hero:11`; a reader of the source finds
it before opening a frame, and the brief hands the seat `program.hero` with the
frames. The comparison then measures nothing about the frames.

**1d. The label is in the folder the seat reads.**
`samples/README-for-the-record.md` says *"frame-A.txt is hand-written ...
frame-B.txt is lldb-2103.0.34.103's real output"*. The seat's tools are Read and
Write, so it sees it only if it guesses the name; the file still has no reason
to stand there. And the brief names the spec **in the tree**
(`.../lane-panel-213/spec/heroes-spec.md`); `.claude/rules/spec-shape.md`'s
`paths:` opens with `"spec/**"`, which `.claude/skills/panel/SKILL.md:133-139`
(panel 175 item 7) names as the leak that made a blind seat not blind. A copy at
a path no rule's `paths:` matches, inside the repository, avoids that one.

## 2. Two framing facts are wrong

**2a. `heroes help build` and `heroes help run` print the whole usage, and the
flags the brief lists are mostly other verbs'.**

```
$ ./heroes help run > build/help-run.txt; ./heroes help build > build/help-build.txt
$ cmp build/help-run.txt build/help-build.txt && echo same     -> same
$ ./heroes --help > build/help-all.txt; cmp build/help-all.txt build/help-build.txt -> same
```

`build`'s own flags: `--dump-ir`, `--emit-c`, `-o`, `--include`, `--library`,
`--sanitize`, `-O0` (its default), `-O2`. `run`'s: `-o`, `--include`,
`--library`, `--sanitize`, `-O0`, `-O2` (its default). `--dump-tokens`,
`--dump-ast`, `--dump-scopes`, `--json`, `--brief`, `--permissive`, `--apply`,
`--in-place` belong to `lex`, `parse`, `check` and `fmt`. **What the brief
omits and route D turns on**: `run` defaults to `-O2`, whose debug word is
`-gline-tables-only` (`selfhost/cli/flags.hero:169-172`, `debug_words`), so a
`heroes run` binary has no variables at all; a `run --debug` must imply `-O0`
(issue 939 says so; the brief does not).

**2b. "The extension's README § Debugging"** is `editors/README.md` § Debugging
(line 151; `editors/vscode/` holds no README, `ls editors/vscode/`). Its words
at 230-234: *"Showing `[T]`, `{K: V}` and `T?` as themselves, and the names
without the index, is M-typed-inspection's work, and the Variables view shows
whatever lldb's formatters show, so it improves when that lands, **with no
change here**"*. That last clause is an unrun premise about the editor question
the sitting must rule; route B loaded through `initCommands` would falsify it.

## 3. Facts missing from "what a build writes", which routes B and C rest on

```
$ touch .stamp; sleep 1; ../../heroes build locals.hero -O0 -o locals-bin; find . -newer .stamp -type f
./build/15ac7c4c467c3452/link-words.rsp
./build/runtime-...o  ./build/runtime-...o.key.txt  ./build/runtime-....deps.txt
./build/tu-.../locals-...o  .o.key.txt  locals.c  locals.deps.txt  warnings-....txt
./build/tu-.../library-...o  .o.key.txt  library.c  library.deps.txt  warnings-....txt
./build/words/words-....rsp
./locals-bin
$ find . -name '*.dSYM' | wc -l          -> 0
$ nm -ap locals-bin | grep ' OSO '        -> the two absolute paths of build/runtime-...o and build/tu-.../locals-...o
```

The brief's list omits the emitted `.c`, the `.deps.txt` and the warnings files
(harmless). What matters: **no dSYM is made, and on Darwin the variables' DWARF
lives in the objects under `build/`, reached by absolute path through the
binary's debug map.** A binary moved or a `build/` cleaned loses its variables;
lldb's auto-load of `<dSYM>/Contents/Resources/Python/` (the ffi brief's item 3)
needs a dSYM the build does not make. `target.load-script-from-symbol-file` reads
`trusted` here (`lldb --no-lldbinit --batch -o "settings show
target.load-script-from-symbol-file"`), `target.load-cwd-lldbinit` reads `warn`.

## 4. Premises handed to seats as settled

**4a. Issue 939's pre-rulings reach the seats unmarked.** The shared brief tells
every seat to `gh issue view 939` and marks only the prototype as unrun. The
issue's Notes (read with `gh issue view 939 --json body`, no write) also rule two
routes out before the sitting: *"a `name` field on `HeroDesc` is refused before
it is proposed"* and *"**nothing** (the conservative reading, refused on §12)"*.
The first rests on *"a string the linker already holds"*, which holds only
unstripped:

```
$ nm rooms-bin | grep Room_desc     -> 000000010001c2a0 s _h_rooms_Room_desc (and _copy, _drop, _eq, all t)
$ nm rooms-o2  | grep Room_desc     -> same four, at -O2
$ cp rooms-bin rooms-strip; strip rooms-strip; nm rooms-strip | grep -c Room_desc   -> 0
```

**4b. The spec-warden is told the milestone "retires" design.md's sentence.**
`grep -n "typed variable" docs/design.md` -> `630`, inside Part 2, Non-goals
(`## Part 2` at 614, `## Part 3` at 639). Retiring a Part 2 line is a design.md
change CLAUDE.md § 4 puts to this sitting, so it is a ruling to make, not a
premise to price. (The milestone page cites `design.md:594`, issue 939 `:594`
then `:613`; it is `:630` today.)

**4c. The Windows doctor row "needs a third" outcome** is an inference (the tell
is the brief's own *so*). `doctor.hero` already makes rows per platform
(`linker_home`: an MSVC row on Windows, `xcode CLT` on Darwin, `cc` elsewhere),
and `./heroes doctor` here prints six rows, no MSVC row. A row present only
where it applies, or `ok: true` with an "absent, optional" detail, are two other
shapes; the brief should put it as a question.

**4d. The compiler-engineer's item 4** asks for *"at least one shape shown with
the author's name"*, which a seat adopting A, or F alone, cannot meet. Minor;
"the route you would adopt, run" says the same without steering.

## 5. Routes missing from A to F

Enumerated from the briefs, issue 939, the milestone page, `editors/README.md`
and the strings of this Mac's LLDB framework; not exhaustive.

- **G, one fixed formatter per toolchain, not per build**: keyed on the
  runtime's C type names (`HeroArrayHeader`, `HeroStr`, the `h_*` record and
  option types by regex) and resolving element types through the descriptor at
  run time, so it cannot go out of step with a binary except across
  `HERO_RUNTIME_ABI` (`runtime/heroes_runtime.h:108`, today 30); carried by
  `heroes` or by the editor extension. B's soundness risk (*a formatter out of
  step with the binary*) and § 10's question both read differently on it.
- **H, types a debugger reads with no formatter**: E speaks of names only; the
  emitted C's **types** could carry shape (a per-element array view, a `T?` or a
  variant as a tagged struct whose arms are named), which DWARF then shows.
- **I, a self-describing binary**: a type table emitted as data (names, field
  offsets, tags) that one generic formatter reads; distinct from C (formatters
  embedded) and from a `name` field on `HeroDesc` (no ABI move).
- **J, the editor's side alone**: lldb-dap's own settings
  (`enableAutoVariableSummaries` is already turned on by the extension,
  `editors/README.md:224-229`) or a DAP-level rewrite in `extension.js`.
- **B without Python**: lldb's `type summary add --summary-string` and command
  files need no Python, which matters where lldb's Python is the failure
  (Windows: `lldb.exe` exits `0xC0000135`, *"a Python DLL"*, `ci.yml:487-489`).
  B is written as Python only; the ffi brief's item 1 allows both.
- **gdb on Linux** (`.debug_gdb_scripts`): the routes name lldb alone; the
  historian is asked about it, the option set is not.

Route C is a question the brief keeps open, rightly; one hint for the ffi seat,
not a measurement: `strings .../LLDB.framework/Versions/A/LLDB | grep -E
"lldbformatters|lldbsummaries"` prints `__lldbformatters` and `__lldbsummaries`.

## 6. Verified as written

- `selfhost/emit/mangle.hero` 265 lines; `slot` at 147 to 177, `"h" + index +
  "_" +` the name with non-word characters made `_` and leading `_` stripped
  (`wc -l`, `grep -n`, read).
- `runtime/heroes_runtime.h` 825 lines; `HeroDesc` typedef at 486, the struct at
  487 (`size`, `copy`, `drop`, `eq`, `hash`, no name); `HeroArrayHeader` 535 to
  541; `HeroMapHeader` 755 to 764, fields as the brief says.
- `.github/workflows/ci.yml:487` holds `0xC0000135`; `selfhost/cli/doctor.hero`
  402 lines, `record Check` with `name`, `ok: bool`, `detail` at 21 to 24;
  `selfhost/cli/flags.hero` 493 lines, `debug_words` 169, `level_words` 183.
- The nm claim: `_h_rooms_Room_desc` (`s`) beside `_copy`, `_drop`, `_eq` (`t`)
  and the runtime's `_hero_desc_array`, `_bool`, `_f32` and the rest (`S`).
- The prototype is not in the tree: the brief's `git grep` over everything but
  `archive/` prints nothing (exit 1); so does it inside `archive/`; and `git log
  --all -S "__lldb_init_module"` and `-S "SBValue"` print nothing.
- `lldb --version` prints `lldb-2103.0.34.103` (Xcode's, `xcrun --find lldb`).
- `docker images` lists `heroes-linux-arm64:latest` (and `heroes-linux:latest`).
- The `lldb` suite: `./heroes run tests/harness/main.hero -- ./heroes lldb` ->
  `lldb: 4 passed, 0 failed` (real 25.72, user 22.69); three cases in
  `tests/golden/lldb/`, `locals.expected` pins `h0_p = (f_x = 3, f_y = 4)`.
- `grep -n -i -E "debug|lldb" spec/heroes-spec.md` prints nothing (exit 1).
- CLAUDE.md § 10 is quoted word for word (lines 227-228; the brief drops the
  bold only).
- `editors/vscode/extension.js` builds at `-O0` and hands an `lldb-dap`
  configuration (lines 8-14, 30-31).
- SKILL.md § 2's rebuild: 60.97 s from the plain seed, measured 2026-09-24.
- **frame-B is real**: rebuilt (`heroes build program.hero -O0 -o prog`, prints
  `alan`) and rerun with the exact lldb command; its 51 frame-variable lines are
  identical to `frame-B.txt` once the heap addresses are normalised (`diff`
  empty). Its first line is the coordinator's, not lldb's: lldb prints
  `prog`h_program_best(h0_players=...) at program.hero:12:11`, and `bt` shows
  `h_program_main` at `program.hero:16:11` as frame 1 (consistent).
- **frame-A's values are those of the stop**: `p *(h_program_Player(*)[3])((char*)h0_players
  + sizeof(HeroArrayHeader))` prints ada 7, grace 9, alan 4; `h1_top = 9`,
  `h2_winner = "alan"`, `h5_p` alan 4; `image lookup -a h0_players->elem` names
  `h_program_Player_desc`. Faithful in values; not in scope (1a).

## 7. Not verified

- The ergonomist brief's *"the author chose, on 2026-10-13"*: the brief's file
  was last written 2026-10-12 23:57 (`ls -la`); the author's words are not in
  the tree for me to read.
- Measurement 042's counts (the brief says so itself); the Linux image's lldb
  (unmeasured in the brief, named so).
- The historian's premises (RFC 3191, `rust-lldb`, natvis) are that seat's to
  source by web; nothing here contradicts them.

## Brief edits recommended

1. **llm-ergonomist**: drop `p` from frame-A (or rule first whether an
   out-of-scope local shows, and say which); add a third frame for E alone
   (author names, C values, temporaries kept) so routes can be told apart; give
   the frames before the program, or pick a bug only the values show; move
   `README-for-the-record.md` out of `samples/`; copy the spec to a path no
   `paths:` matches and name that path; confirm or correct *2026-10-13*.
2. **00-shared, The command's surface**: replace the flag list with build's and
   run's own, and add that `run` defaults to `-O2` with line tables only
   (`flags.hero:169-172`), so route D's flag implies `-O0`.
3. **00-shared, What a build writes**: add *no dSYM; the DWARF stays in
   `build/` objects reached by absolute path through the debug map (OSO)*, and
   the `.c`, `.deps.txt` and warnings files.
4. **00-shared, The editor**: the path is `editors/README.md` § Debugging, and
   quote its *"with no change here"* as an unrun premise.
5. **00-shared, issue 939**: mark its two pre-rulings (no `name` field on
   `HeroDesc`; route A refused on § 12) as the issue's, not the sitting's, with
   the strip measurement beside the first.
6. **00-shared, Routes**: add G (one formatter per toolchain), H (C types that
   carry shape), I (a self-describing type table), J (the editor's side alone),
   B without Python, and gdb on Linux, as the coordinator's additions after the
   critic, unpriced.
7. **00-shared, Windows**: put the doctor's third outcome as a question; name
   the platform-conditional rows `linker_home` already makes.
8. **spec-warden**: *"whether this milestone retires"* the Part 2 sentence at
   `design.md:630`, not *"the one this milestone retires"*.
9. **compiler-engineer**: the prologue is `selfhost/emit/prologue.hero`
   (`body.hero:5` says so); item 4 as *"the route you would adopt, run"*.
