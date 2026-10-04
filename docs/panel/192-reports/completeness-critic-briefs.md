# Panel 192, the completeness critic, first pass: over the briefs

Started 2026-10-04 17:38 (from `date`). Written as I go; the sections fill in
the order the brief asks for. My role is the one `/panel` gives it: name what
is missing, never a verdict on the proposal.

**My copy**: `<scratchpad>/192-critic/`, from `git -C <trunk> archive
4c3524fb | tar -x`, its compiler built from the seed at 17:38:00 to 17:38:05:
`shasum -a 1 heroes` reads `8084f018f53875362048cc0230d23207976d8b00`,
`shasum -a 256 seed/heroes.c` reads `c79ffd5ad005c301...`, `wc -c` reads
35,206,983. All three match `00-facts.md`'s base.

`4c3524fb` is batch 10's closing commit on branch `lane-round-b10`, one
commit above `1dad1ac9` (`git rev-list --count 1dad1ac9..4c3524fb` reads 1);
`git diff --stat 1dad1ac9 4c3524fb` moves 37 record files and the seed only.

## 1. What I checked and found as written

Every command ran in my copy, `<scratchpad>/192-critic/`, or in
`<scratchpad>/192-critic-work/`, between 17:38 and the times below.

- **F1** (17:38:59 to 17:39:02). All 110 of `<scratchpad>/192-facts/cp*.hero`,
  copied into `192-critic-work/f1/`, each through `heroes check --brief`. A
  string: 54 at exit 0, U+000D alone at exit 1 `raw_carriage_return`. A
  comment: all 55 at exit 0. Same as `table-base.txt`, line for line.
- **F2's spec lines**: `:35-36`, `:324-325` and `:382` read as quoted.
- **F3**: design.md `:547`, `:992-993`, `:1016-1018` and `:2629` read as
  quoted. `hero_str_cstr` (`runtime/parts/str.c:434`) calls
  `hero_str_require`, which tests `s.ptr == NULL` and reads no byte.
- **F4** (17:43:21 to 17:43:25). The four programs rebuilt with my compiler
  in `192-critic-work/f4/` from copies of `192-facts/nul/`. Each builds at
  exit 0:
  - `nulc` prints `1`;
  - `rf` prints `3`, `1`;
  - `qi-write` prints `11`, `false`, and writes `out-a` holding `written`;
  - `qi-read` prints `3`, `false`, `the file named a`.
- **F5's counts**, re-scanned. `git ls-tree -r --name-only 4c3524fb` lists
  2,014 `.hero` files, the same list as `192-facts/tracked-hero.txt`, all
  present in my copy. A byte count gives C0 but tab, CR and LF, plus DEL: 19
  in 5 files. A Python scan
  (`192-critic-work/scan.py`, output `f5-scan.txt`) gives:
  - tab: 69 in 8 files;
  - bidi: 1;
  - other Default_Ignorable: 3 in 3 files;
  - U+2028/2029: 1;
  - NUL: 0;
  - not UTF-8: 4 files, all defect 227's.
  All as F5 says. `selfhost/cli/compile.hero:83` holds the two raw U+0001.
- **F6**: `head_names.hero:191` is `unshowable_character`.
  `shown_char.hero:204` holds `DEFAULT_IGNORABLE`, 17 ranges, read from
  Unicode 15.0.0 by its comment. `UNSEEN` (`:95`) is the wider list.
- **F7's total**: `xargs grep -c '\.cstr()'` over the 2,014 gives 215 lines in
  100 files. `spawn.c` starts threads (`parts/spawn.c:10`) and hands no name to
  the system, so an empty match there is right.
- **F8** (17:49:23): `heroes measure spec/heroes-spec.md` prints `real 9392`,
  headroom 848, FFI floor 60, byte for byte as
  `192-facts/measure-spec.txt`.
- **The compiler-engineer's pointers**: `literals.hero:83` (`raw_carriage_return`), `:119`
  (string), `:162` (character literal), `lex_interp.hero:104` (f-string).
- **The funding**: `docs/records/log/2026-10-04-1633-...` records *3a*, the full
  panel on 245, 251 and 283, the blind seat capped at 5 USD.
- **Panel 191's R6** hands Q-c and Q-i as the shared brief says. The three
  defect files read as the brief summarises them.
- **The blind inputs**:
  - `cmp` finds `192-blind-src/spec-a.md` equal to `4c3524fb`'s spec;
  - `diff spec-a.md spec-b.md` and `diff spec-a.md spec-c.md` regenerate
    `blind/spec-b.diff` and `blind/spec-c.diff` byte for byte;
  - `heroes measure`, vendored tables only, gives a maximum of 7117, 7171 and
    7165 for A, B and C, so B is +54 and C is +48, lower bounds;
  - the command's flags (`--restricted --safe-mode --strict-mcp-config`, and
    `--tools` with `--allowedTools`) are the ones panels 184, 188 and 189 ran.
    Panel 184's run without `--allowedTools` could not write its report;
    this one has it.

## 2. Facts I could not verify, or found false

**F2, the grep, is misdescribed; its conclusion holds.**
`grep -n -i 'nul' spec/heroes-spec.md` prints six lines: 69, 212, 377, 383,
384 and 416. All six are `null`, `nullptr` or `null_cstr`. None is `:386`.
The field rule is found by `zero`: `grep -n -i zero` prints `:385`, *reading
to its first zero*, and `:386`, *`cstr`, which promises a zero the field does
not*. A wider grep (`zero|\\0|interior|embedded|truncat|\bNUL\b`) finds no
sentence on an interior NUL at a lend, so the substance stands. Two more
lines a seat pricing Q6 needs, which F2 does not quote:
- `:386` already says a `cstr` *promises a zero*;
- `:64` says a `str` is an *immutable UTF-8 string, indexed and measured in
  bytes*, and U+0000 is UTF-8.

**F4's last sentence is false**: *"no door asks a `str` for an interior
NUL"*.
- `runtime/parts/run.c:76`, in `hero_run_arg(HeroStr word)`, reads
  `if (strlen(word.ptr) != length) hero_panic("an argument contains a NUL
  byte");`.
- Its comment, `:68-70`: *a `str`'s `len` is authoritative (spec § 3 Types)
  and `execvp` reads to the first NUL, so the two disagree exactly when an
  argument would silently become a shorter one.*

It is Q4's question, already answered once, by an abort, at the door that
starts a process. F4's `grep memchr` is the vocabulary that missed it.
Q4's routes should cite it as precedent, both for the scan (one `strlen` a
word) and for its answer: a panic, not *a failure the program reads*.

**The shared brief's Q4 premise is half unrun.** It says *"a `str` still gets a
NUL from a file, an argument or C itself (F4)"*. F4 ran a file only. What the
code says about the other two:
- **C itself**: `validated()` copies with `strlen`
  (`hero_str_try_from_cstr`, `str.c:333`, its `strlen` at `:338`). `validated_bytes()`
  stops at the first zero (`memchr`, `str.c:379`). Both stop at a zero, so
  neither can hand a program a `str` holding one. My run: `b: [u8] = [97, 0,
  98]`, `b.validated_bytes().must()` has `len` 1 (`192-critic-work/rt/nul.hero`).
- **An argument**:
  - POSIX `argv` words are C strings;
  - `hero_args_at` builds with `hero_str_from_cstr`, which is `strlen`
    (`os.c:530`, `str.c:431`).

  On Windows, panel 191's ffi-pragmatist says `hero_win_wide` stops at a NUL
  (*Handed on*). Unrun by me.

By the vocabulary `hero_str_from_bytes(`, `hero_str_try_from_bytes(`,
`hero_str_from_cstr(` and `hero_str_try_from_cstr(` over `runtime/parts/*.c`,
the doors that build a `str` from outside bytes with an explicit length are:
- the file reads, `hero_file_read` (`os.c:814`) and `hero_file_read_shown`;
- the *shown* builders, `os.c:649` and `:703`.

Every other door goes through `strlen`. String operations copy bytes, so
none makes a zero from inputs without one. After Q1, a program's NUL comes
from `read_file`. This is a negative claim resting on my vocabulary, so it
goes to the ffi-pragmatist as a question.

**F7's breakdown does not sum to its total, and the total counts lines, not
call sites.** The same `xargs grep -c` per top-level directory:

| tree | lines | files |
|---|---|---|
| `selfhost/` | 81 | 18 |
| `examples/` | 9 | 3 |
| `tests/golden` | 77 | 58 |
| `tests/harness` | 24 | 4 |
| `docs/panel` | 22 | 16 |
| `archive` | 2 | 1 |

That is 215 lines in 100 files. F7's breakdown sums to 211 in 96 and gives
`tests/` 121 in 75; I count 101 in 62. `grep -o` counts 234 occurrences.
Placed by my approximate lexer (`192-critic-work/scan.py`'s state machine):
- 163 in code, in 76 files;
- 45 in comments;
- 26 inside string literals.

Two of the 26 are real lends: the prelude's `read_file` and `write_file`,
written as text in `selfhost/library_source.hero:208` and `:222`. Of the 163
in code, 28 are in `docs/panel/` and `archive/`, which no gate builds. *"the
215 sites of F7"* (ffi-pragmatist brief, item 1) is therefore not a count of
sites.

**F7's system-call grep is 34 only once comment lines are dropped.** As
written, `grep -n -E '<F7's list>' runtime/parts/*.c` prints 60 lines, and
`alloc.c` among the files. Dropping lines that open `*`, `/*` or `//` gives
34 with F7's per-file split: dir 2, fs 10, os 4, replace 15, run 2, str 1.
F7 says the list is a vocabulary. A wider one, comment lines dropped, finds
21 more lines that take a name the program supplies:
- `open(`: run.c 3 (`:378`, `:389`, `:400`), replace.c 8;
- `CreateFileA`: run.c 3, replace.c 5;
- `chmod` (`replace.c:695`) and `link(` (`:709`).

A 22nd hit, `run.c:623`, opens the literal `"NUL"`. So it is at least 55
lines, still a vocabulary.

**F6 is imprecise about the NUL and the line end.** It says a group head's C0
or C1 control is refused as `unshowable_name`, a thesis rule. Run 17:44:40 in
`192-critic-work/head/`:
- a NUL in a head string is `unwritable_name` (`head_names.hero:155`,
  `unwritable_byte`, with the line end), and it survives `check --permissive`;
- ESC, tab and U+202E are `unshowable_name`, and `--permissive` drops them;
- a string's raw CR is `raw_carriage_return`, which `--permissive` keeps;
- `diag.hero:129-153`'s thesis list (its comment at `:121`) holds
  `unshowable_name` and not `raw_carriage_return` or `unwritable_name`.

So **the two precedents Q1 names disagree on the class**:
- panel 066's raw CR is not a thesis rule;
- panel 188's unshowable character is one;
- a third precedent, `unwritable_name` for the NUL, is named nowhere in the
  briefs, though defect 245's own item cites it.

**F9 inherits a conflation from panel 191's R6, unrun here (the box is held).**
F9 says *"The UCRT's wide `argv` with a strict conversion refused it with
error 1113 in both seats' probes"*. Panel 191's R6 says the same. Only the
ffi-pragmatist's report has 1113 (`191-reports/ffi-pragmatist.md:447-448`,
`WC_ERR_INVALID_CHARS`). The compiler-engineer's report has no `1113`
(`grep`). Its `__wargv` row (`191-reports/compiler-engineer.md:258`, `:454`)
converts by `hero_win_name_bytes`, *a lone surrogate as its WTF-8 bytes*, and
reads *exact, `\ud800` kept*, so *the runtime can refuse it* (`:270`). Two
conversions were run; one refused. That second conversion is section 3's
first Q5 route.

**The shared brief's *"the tag ... waits on them"* is partial.**
`docs/work/defects/` at the base holds 78 files:
- 5 `blocking`: 231, 238, 292, 323, 336;
- 2 `systemic`: 245, 283;
- 32 `adjacent` and 39 `improvement`.

The tag waits on seven.

**Two facts I added, run on the base, which a seat may need**:
- **F1 at `build` and `run`** (18:00:08 to 18:00:44, `192-critic-work/f1b/`,
  `f1-build.txt`). Each of the 55 string files went through `heroes build`,
  and each built program ran.
  - 54 build at exit 0 with no warning line; U+000D alone is refused.
  - Each program prints exactly its literal's bytes, the NUL included (a
    Python comparison, 0 differ).
  - The seed holds no raw control byte and no byte above 127 (a Python
    count over `seed/heroes.c`). The emitter escapes all of them;
    `compile.hero:83`'s U+0001 is `"\001I"` at `seed/heroes.c:3196`.
- **The NUL beside F4** (17:43:38, `192-critic-work/f4/`):
  - `print(s)` of `read_file("nul.txt")` writes `61 00 62 0a`;
  - `write_file` of that text writes the three bytes;
  - a `.must()` panic whose message holds the NUL prints *no file at
    missing-a* and loses `\0b-end`;
  - an `assert` prints *left:  a*.

  So the ffi-pragmatist brief's *"a failure's message holding a NUL is cut
  there, a shape beside, unrun"* is now run, and true.

## 3. Routes nobody listed

**Q3 (a) exists today, and the compiler uses it for exactly this character.**
- A `[u8]` answers `validated_bytes()`. `esc: [u8] = [27]` followed by
  `esc.validated_bytes().must()` builds ESC at run time on the base.
- My `192-critic-work/rt/esc.hero` builds at exit 0 and prints
  `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 30 6d 0a`, the blind experiment's
  target byte for byte.
- It cannot make a NUL: `[97, 0, 98]` gives `len` 1, read to the first zero.
- The compiler writes its own control characters this way, 13 calls in 9
  files: `ir/print.hero:591` `escs: [u8] = [27]`, `refused_runs.hero:182`
  `controls: [u8] = [1, 1, 1, 2, 32, 1]`, `lexer.hero:354` the BOM, and
  `cli/json_text.hero` 4. It writes them raw in one place only,
  `compile.hero:83`.
- The spec does not say it: `:384` gives `validated_bytes()` to *a field of
  bytes* and names no `[u8]`.

So route (a′) is a few words at `:384` and no new escape. It keeps one
spelling per character in a literal, it writes every byte but NUL, and
`compile.hero:83` can take it on today's seed. Q3 asks a seat to *"say
which, or that none exists"*, and the compiler-engineer brief asks it only
*"if you recommend (a)"*. The briefs should say it exists.

**An escape limited to the refused set.**
- design.md `:994-995`: *each character has exactly one spelling, §4.15's
  canonical-form rule survives into the literals*.
- `:2029`: *there is exactly one correct way to write any program*.

Spec B's draft opens `\u{...}` to every code point; its own example,
`\u{e9}` for `é`, is a second spelling of a character a literal holds raw.
Spec C's draft lets `\x41` stand for `A`. A route nobody wrote down: the
escape is legal only for a code point the literal refuses raw, so `\u{e9}`
is refused with a `certain` fix to `é`. Today's raw tab inside a string is
already a second spelling of `\t`, which bears on Q1's *hard case*.

**The formatter writing a refused character by its escape, in a string.**
Q2 lists *"`fmt` writing the character visibly"* for a comment, where no
escape exists. For a string, where `\t` exists today and `\u{...}` might,
the same route is unlisted: §4.15's one spelling, applied by `fmt`, beside
or instead of a refusal.

**Q4: the NUL refused in the pass every constructor already makes.**
- `hero_str_from_bytes` (`str.c:301-304`) runs `hero_utf8_valid` over every
  byte and panics on ill-formed UTF-8.
- `hero_file_read` (`os.c:791-818`) runs it twice: its own pre-check at
  `:809`, then inside `hero_str_from_bytes`.

So route (b)'s *"every constructor pays"* is, measured by the code, *every
constructor already pays a full scan*. A zero test in that loop's ASCII
branch (`str.c:290`) keeps the invariant *no `str` holds a NUL*. After that,
`.cstr()` stays free, and `read_file` answers `not_text` (or a code of its
own) where the file holds one. Its cost is one compare a byte inside an
existing scan, to be measured, and a program reading NUL-separated data
(`find -print0`) loses `read_file`. Section 2 found the doors such an
invariant needs: the file reads and the shown builders, by my vocabulary.

**Q4 and Q-i: a door that takes the `str`, not a `cstr`.** Every
`hero_os.h` door that takes a name (`:60` to `:338`, about thirty) takes
`const char *`, so the prelude and the compiler lend `.cstr()` first:
- `library_source.hero:208`, `:222`;
- `cli/files.hero` 14 lines;
- `cli/process.hero` 14 lines;
- `module/reading.hero` 3 lines.

One door already takes `HeroStr` and refuses the NUL itself:
`hero_run_arg`, `run.c:76`. Doors that take the `str` could answer a failure
the program reads at each door, and leave `.cstr()` free for bindings. Its
price is the header's shapes. The ffi-pragmatist should say whether that
moves `HERO_RUNTIME_ABI`, which is 26 at `heroes_runtime.h:58` and asserted
at `selfhost/emit/decls.hero:83`.

**Q5: the wide `argv` converted losslessly to WTF-8, so the existing check
refuses.** Panel 191's compiler-engineer ran it
(`191-reports/compiler-engineer.md:454`): *a lone surrogate as its WTF-8
bytes*, *`\ud800` kept*. On this Mac (17:52:09, `192-critic-work/args/`):
- `args()` over the bytes `78 ed a0 80 79` (the WTF-8 of `x<D800>y`) and
  over `x<FF>y` panics at exit 134 with *hero_str_from_bytes: not
  well-formed UTF-8*;
- `args_checked()` answers `not_text` for both and `ok` for a good word.

So this route makes `:324-325` true with no refusal at startup, and gives
`args_checked()` its answer through code that exists. Q5 lists only the
strict conversion.

**Q2: show it rather than refuse it, as the compiler already does
elsewhere.** Defect 244 writes a quoted control character by its code in a
diagnostic, and defect 290 in the dumps (`print/dump.hero:240-250`). A route
where a comment keeps the character and every tool that prints it shows
`<U+202E>` is that precedent widened. It does not reach an editor or a web
page, which is where Trojan Source bites, and that is the argument a seat
should weigh.

**The blind experiment could have tested (a′) within the cap.** Section 6.

## 4. Questions the sitting should ask and does not

1. **One spelling.** Does the escape write every code point, against
   design.md `:994-995` and `:2029`, or only what a literal refuses? Is a
   raw tab in a string, a second spelling of `\t` today, refused on that
   ground alone, whatever Q1 rules on visibility?
2. **Which list is "what a reader cannot see"?** The proposal takes
   `DEFAULT_IGNORABLE` (17 ranges), C0, C1, U+2028 and U+2029. The
   compiler's own `UNSEEN` (`shown_char.hero:95`, 38 ranges) is what it
   calls *shown as nothing, or changing how it shows what follows*. It adds:
   - NBSP;
   - U+2000 to U+200A;
   - U+202F, U+205F, U+3000;
   - U+FFF9 to U+FFFB.

   F1 tests NBSP alone of these. In a group head, `extern "a<NBSP>b.h"` and
   `extern "a<U+3000>b.h"` check at exit 0 (run 18:05:11,
   `192-critic-work/head/`). The tab's class, *visible
   as space and not a space*, has these members, and Q1 names the tab alone
   (CL-061).
3. **What text needs the refused characters?** `DEFAULT_IGNORABLE` holds
   ZWNJ and ZWJ (U+200C, U+200D), which Persian and Indic text spell with.
   It holds the variation selectors (U+FE00 to U+FE0F), as in `❤️`, and the
   tag characters (U+E0000 onward), as in a subdivision flag. Proposal part 1
   refuses all of them raw in a string. What does a program printing those
   write under each Q3 route? Spec C's draft, `\xNN` from `01` to `7f`,
   writes none of them; route (c) as Q3 lists it can, and so writes half a
   character. F1 holds U+200C and U+200D, but not U+FE0F or U+E0001; the
   tracked corpus holds none of these four (`scan.py`), so no test of the
   repository would notice.
4. **The class.** Section 2 measured three precedents with three answers:
   - CR is not a thesis rule;
   - the unshowable character is a thesis rule;
   - the unwritable NUL is not one.

   Which does a string's ESC follow, and which a string's NUL? A NUL is
   carried by `print` and `write_file` whole (section 2) and cut by C alone.
5. **Whose reader is Q2's?** A model reads code points in logical order. An
   override reorders what a person's editor or browser draws. A comment's
   refusal for the person is a robustness or security rule (CLAUDE.md §
   Precedence rank 3), not obviously a thesis rule. That decides whether
   `check --permissive` drops it and whether Part 11's control arm measures
   it.
6. **The message a model meets first.**
   - Today `unknown_escape` answers `\x1b`, `\u{1b}`, `\u001b`, `\e` and
     `\033` with *"a string holds its characters as themselves, and this
     language has no escape for one by its code"*, and a `guess` fix to
     `\\` (my run, 17:47:17, `192-critic-work/esc/`).
   - That sentence points the writer at a raw control character: defect
     251's trap.
   - Under each route, what does it say, and may it carry a `certain` fix to
     the route's spelling (`\x1b` to `\u{1b}`, or to the `[u8]` form)?

   No brief lists this message among what a route owes, and the blind
   experiment cannot see it.
7. **The order of landing.** Under route (b), `compile.hero:83` needs a seed
   that reads `\u{...}` before the compiler's source may write it. The old
   seed answers `\u{1}` with `unknown_escape`, as above. A refusal of raw
   U+0001 must land after `:83` changes. The brief asks what `:83` becomes,
   not in which order the escape, the seed and the refusal land.
   `[1].validated_bytes().must()` keeps the cache key's bytes on today's
   seed.
8. **Where Q4's check lives, and which answer wins.** Proposal part 4
   wants an abort at `.cstr()` and a readable failure at the doors. But
   `read_file` and `write_file` reach their door through `path.cstr()`
   (`library_source.hero:208`, `:222`), so a lend-time abort fires before
   the door can answer. The prelude would have to ask the `str` first, and
   the language has no built-in that asks a `str` for a zero. Which check
   runs first, and in what code?
9. **The ESC in the reader's terminal from the runtime.** Q7 lists *a
   diagnostic that quotes the line*, which defect 244 already writes by
   code. Run 18:01 in `192-critic-work/f4/` and `q7/`:
   - a `.must()` panic message holding ESC writes `1b 5b 32 4a`, a clear
     screen, raw to stderr;
   - so does an `assert` message;
   - `heroes test` prints a failing test's title with its ESC raw:
     `FAIL "title <1b>[2J here"`.

   These are the runtime's printer (`failure.c:116`, `:135`, `:145`) and
   the test runner, the same cause. Q7 names the `test` title as a shape to
   run, not the runner's output.
10. **A refusal by data.** `DEFAULT_IGNORABLE` is read from Unicode 15.0.0
    (`shown_char.hero:199`). A later table widens what the checker refuses.
    `.claude/rules/verification.md` § *A new checker rule* says such a
    change is judged by every golden tree. Who refreshes it, and under what
    gate?
11. **The group head and the character literal under the new escape.** Does
    `\u{...}` or `\xNN` work in an `extern`, `link` or `package` string,
    which `emit/externs.hero:69`'s `unquoted` decodes? And what integer is
    `'\u{e9}'`, given design.md `:996`'s *one ASCII character or one
    escape*? Spec B's sentence opens the escape inside a character literal,
    unremarked.

## 5. Facts a seat is handed that it should measure itself

- **F7's 215, handed to the ffi-pragmatist as *"the 215 sites"*.** It is a
  line count over every tracked `.hero` file, comments included (section 2).
  Route (c), a fallible lend, is priced by the lends in code that a gate
  builds: by my approximate lexer, 163 occurrences in code, minus 28 in
  `docs/panel/` and `archive/`, plus the prelude's two written as text.
  The seat should count with the compiler's own lexer (`heroes lex`) rather
  than a regular expression.
- **The ffi brief's *"seven lines by `grep -rn 'hero_str_cstr(' runtime/`"*.**
  The command prints nine: the declaration at `heroes_runtime.h:214`, the
  definition at `str.c:434`, and seven call lines holding ten calls. Four of
  those calls pass content with its length, and a NUL there is correct
  today, so route (a) placed inside `hero_str_cstr` makes `write_file`
  abort on a correct program:
  - `os.c:864` in `hero_file_write`;
  - `:913` in `hero_write_err`;
  - `replace.c:440` in `hero_stage_fill`;
  - `:507` in `hero_file_stage`.

  My `pr.hero` wrote `61 00 62` whole.
- **The ffi brief's *"How the emitter calls them: `selfhost/emit/ffi*`"*.**
  No `selfhost/emit/ffi*` file names `hero_str_cstr`, `hero_str_held` or
  `hero_cstr_nonnull` (`grep -l`, empty). The lend is emitted at
  `selfhost/emit/inst.hero:316`. The null guard on every `cstr` argument is
  at `selfhost/emit/ops.hero:305`. `emit/builtins.hero` and
  `check/lend_types.hero` name them too. The brief says *found by a
  command*; its pointer sends the seat to the wrong files.
- **The compiler-engineer's item 4 list.**
  `.claude/rules/diagnostics-and-goldens.md` § *A new surface form* also
  names *the formatter's own self-check* (`selfhost/cli/syntax_cmds.hero`),
  which the list omits. A new escape also meets every decoder of a string
  literal, beyond the re-printers:
  - `selfhost/escape.hero`;
  - `selfhost/emit/externs.hero:69` (`unquoted`);
  - `selfhost/check/contextual.hero:140` (`literal_value`);
  - `tests/harness/suite_lines.hero:327`;
  - `tests/harness/suite_records.hero:1919`;
  - `tests/harness/suite_spec.hero:1343`, which reads *every escape the
    compiler accepts* out of `escape_text`'s single-letter arms, a pattern
    a `\u{...}` with a payload does not fit.
- **F5 is counted per file, string and comment alike; the routes act by
  context.** My lexer approximation places the 19 C0 and DEL bytes as 4
  inside string literals and 15 outside any literal, and every other class
  likewise:
  - **in strings**:
    - `compile.hero:83`;
    - 244 and 289, whose ESC follows a backslash, already `unknown_escape`;
    - `surface-fixtures/json102/tab.hero:2`, the one raw tab in a string;
    - 216's two group heads;
  - **in comments**: none of any class. So Q2's refusal moves no tracked
    file, by this approximation;
  - **in code**: 282, `json102/control.hero` and `json102/unseen.hero`,
    already `unexpected_character`;
  - **the BOM** at 242's first byte.

  The compiler-engineer should count with `heroes lex`, which knows a
  comment.
- **F9, handed to the ffi-pragmatist as one mechanism.** It is two
  conversions, and only one refused (section 2).
- **The blind brief's *"0.17 to 0.18 each"*.** Panel 189's four reports give
  0.1623 to 0.1794 USD.
- **`.claude/rules/verification.md`'s map** names five forms and has no
  `tests/golden/full/` row. `tests/harness/main.hero:179-186` runs six
  forms, `full` the sixth, and F5's case 289 lives under
  `tests/golden/full/`. The compiler-engineer brief's *"all 27 suites and
  `cache`"* is right (21 names registered by `only ==` beside `cache`, plus
  six forms), so this binds only a seat that reads the map instead of
  running everything.

## 6. The blind experiment

Checked as written:
- the twelve folders `192-llm-ergonomist/{a,b,c}{1..4}/` exist, each holding
  exactly `brief.md` and `spec.md` (`ls -A`);
- each `spec.md` matches its arm's `192-blind-src/spec-<arm>.md`, and each
  `brief.md` matches `docs/panel/192-briefs/blind/brief.md` (`cmp`);
- walking every parent of the scratchpad finds no `CLAUDE.md`,
  `CLAUDE.local.md`, `.git`, `.claude` or `.mcp.json`;
- `/Users/joseph/.claude/settings.json` has no `hooks`, and there is no
  `/Users/joseph/.claude/CLAUDE.md`.

What gives the answer away, or cannot separate the arms:

1. **Every arm holds a route the brief says is absent.** `blind/brief.md` of
   the seat says *"A raw ESC is legal there and nothing writes it visibly"*
   and *"so it isolates the route"*.
   - `esc: [u8] = [27]` and `esc.validated_bytes().must()` writes ESC
     visibly, builds on the base, and prints the target bytes (section 3).
   - A raw ESC inside a character literal checks at exit 0 too (my
     `192-critic-work/blind/charesc.hero`, 17:59:26), and with
     `validated_bytes()` it prints the target.
   - Spec A, B and C state `validated_bytes()` for *a field of bytes* only
     (`:384`, `:387` and `:386` respectively), so the route is there by
     inference in every arm.

   A program on it passes in every arm, since it holds no raw control
   character and no escape. Unless the scoring records which spelling each
   program used, a high score in any arm cannot be credited to its escape.
   Record per program: raw byte, the arm's escape, `[u8]`, or other.
2. **A against B and C changes two things, not one.** B and C each add the
   refusal at `:35-37` (*"strings any but a raw control character"*) as
   well as the escape. The refusal tells a reader that a control character
   is a category the spec cares about. A difference between A and B or C
   cannot be split between the cue and the escape. B against C is matched on
   the refusal, so that comparison alone isolates the spelling.
3. **The two examples are not the same distance from the answer.** C's
   example, `\x07` *is the bell*, is a C0 control, the answer's own class,
   and written the way the answer is written. B's, `\u{e9}` *is `é`*, is a
   printable letter, from which a reader must infer that a control is
   allowed. *"Neither is the answer"* holds; *"neither is near it"* does
   not.
4. **The scoring rule's codes are in two notations, and either reading
   misjudges.** The brief's items:
   - *"`1b 5b`, then `31` or `91` ... then `6d`"*;
   - *"`1b 5b`, then nothing or `30` or `39`, then `6d`"*.

   `1b 5b` and `6d` are hex bytes. `31`, `91` and `1;31` must be SGR
   parameters written in decimal, or `31` is the digit 1 and the colour is
   bold. Read the same way, `30` in the second item is *black*, and the
   commonest reset, `ESC[0m`, is not listed. A scorer of both readings over
   eight outputs (run 18:05):

   | output | decimal | hex |
   |---|---|---|
   | `ESC[31m ERROR ESC[0m` | fail | fail |
   | `ESC[31m ERROR ESC[39m` | pass | fail |
   | `ESC[31m ERROR ESC[30m`, black | **pass** | fail |
   | `ESC[1;31m ERROR ESC[0m` | fail | pass |
   | `ESC[1m` bold, `ESC[9m` strikethrough | fail | **pass** |

   Only an unwritten mixed reading scores the canonical program right. Also:
   - *"a list holding one"* accepts `38;5;31`, which is palette entry 31,
     not red;
   - nothing says whether `\u{1B}` or `\x1B` (upper case), `\u{001b}`
     (leading zeros) or `\u001b` (no braces) count as the arm's escape.

   All of it should be fixed in writing before the first session runs, not
   decided over twelve outputs.
5. **Substitution scoring is exact only for an ASCII target.** B and C are
   scored by replacing each escape by its character and compiling with the
   base. Spec B's sentence opens `\u{...}` inside a character literal too.
   The base refuses `'é'` (`char_literal`, my `chare9.hero`), so a B program
   writing `'\u{e9}'` would be failed by the instrument, not by B's spec. It
   does not bite for ESC, which is ASCII. It would bite the moment the
   experiment is reused for a character above it.
6. **Arm A's success depends on the harness, not only on the spec.** A raw
   ESC reaches `c.hero` only if the session's `Write` call carries the JSON
   escape `\u001b` for that byte. The coordinator should read each failing
   `c.hero` with `xxd`, to tell *wrote `\x1b` as text* (the spec misread)
   from *meant a raw byte and the channel lost it*.
7. **Four an arm separate only 4 against 0.** Fisher's exact test,
   two-sided, computed at 18:05:
   - 4-0 gives p = 0.029;
   - 4-1 and 3-0 give 0.143.

   Five an arm separate 5-1 and 4-0 (0.048). The cap allows more: twelve
   sessions are 3.00 USD at the 0.25 cap and panel 189's cost 0.16 to 0.18,
   so twenty sessions are 5.00 at the worst. Twenty is five an arm over
   four arms, or about six an arm over three. A fourth arm A′, spec A with
   `:384` saying a `[u8]` answers `validated_bytes()`, would test route (a′)
   at no new escape. The brief also states no expected result and no rule
   from scores to a route. A pre-registered *"B beats C by at least 3 of 4,
   else ..."* is what makes four sessions decide something.
8. **The `context` rule needs its threshold before reading.** Panel 188's
   sessions answered *"the standard harness system prompt and environment
   details (macOS platform, working directory, date) and an automatically
   attached user email address"*. Each session's working directory here is
   `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/.../192-llm-ergonomist/a1`,
   which names the project. Is that *a yes*? If it is, every honest session
   is void; if not, say so in the brief.
9. **What no arm can show**, beside the brief's own list: today's
   `unknown_escape` message, which tells a writer of `\x1b` that *a string
   holds its characters as themselves* (section 4, question 6). A model
   working with the compiler meets that message second, after the spec.

## 7. What I did not run

- **F9 and every Windows fact**: the box is held by batch 10's leg. I read
  panel 191's two reports and its R6 and quote them in section 2; the lone
  surrogate on the box is unrun by me.
- **Linux arm64 in Docker**: not started, by the brief's constraint.
- **No paid run**: no `claude -p`, no `heroes measure --refresh`. Every
  delta in section 1 is a vendored-table lower bound.
- **The net**: no suite ran. My copy's compiler is the seed's, unedited, so
  there was nothing of mine to gate.
- **My context split** of F5 and F7 rests on a per-line state machine, not
  the compiler's lexer. It is approximate, stated as such, and it missed an
  ESC that follows a backslash until a byte count found it.
- **The negative claims are each a question**: no NUL from an argument or
  from C, the file reads as the only user-reachable door that makes one, no
  tracked comment holding these characters. Each names its vocabulary
  above.
- **Not run**: the Q7 shapes beyond the test runner, the panic and the
  assert, which are the compiler-engineer's (character literal at `fmt`,
  f-string text, doc comment, a diagnostic quoting a line, `fmt
  --in-place`); and route prices, which are the seats'.
- **Found beside, not this sitting's**: `runtime/parts/str.c:354`'s comment
  says *"its line 304 is `size_t n = strlen(p);`"*, and that line is now
  `:338`. A stale line citation, CL-037's shape, for a filing apart.

Finished at 18:07 (from `date`, read at 18:07:16 before this last write).
Three line citations were corrected at 18:07 after a re-check: `run.c:68-70`,
`diag.hero:129-153` and `suite_spec.hero:1343`.
