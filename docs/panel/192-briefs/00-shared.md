# Panel 192, the shared brief: what a string or a comment may hold, and what reaches C from a `str`

Written 2026-10-04 from 17:18 by the coordinator, on batch 10's closing
commit `4c3524fb`, frozen from the briefs to the synthesis; **repaired from
18:08 after the completeness critic's first pass**
(`docs/panel/192-reports/completeness-critic-briefs.md`; the text it read is
`00-shared-before-the-critic.md`). Every number names the command or the
file that produced it; `00-facts.md` holds the facts, cited as F1 to F12.
What was not run says so. **Read the critic's first pass too**: it is part
of your brief.

## Why this sitting

The author's *3a* of 2026-10-04 funded one full panel for defects 245, 251
and 283 (`docs/records/log/2026-10-04-1633-the-author-ratifies-panel-190-funds-sitting-192-and-asks-for-the-push.md`),
with **the blind seat's paid sessions capped at 5 USD in all**. Panel 191
handed it two more questions, Q-c and Q-i (its R6). Read each item whole:
`docs/work/defects/245-*.md`, `251-*.md`, `283-*.md`.

- **245** (`systemic`): a `str` holding a NUL is lent to C by `.cstr()` as it
  is, so C reads a shorter string than the program holds (F4).
- **251** (`adjacent`): a raw control character inside a string literal,
  other than a carriage return, is accepted in silence, invisible in the
  source and lost when the line is retyped (F1). Panel 066 measured that
  trap for CR alone and refused CR alone.
- **283** (`systemic`): the bidirectional controls and U+2028 are accepted
  in comments and strings, so a line can show one order and mean another
  (F1), Trojan Source's shape.
- **Q-i** (panel 191): the runtime's own doors lend a path's `cstr`, so a
  path holding a NUL reads or writes a file the program never named, at
  exit 0 (F4, run for this brief).
- **Q-c** (panel 191): spec `:324-325` says an argument that is not UTF-8
  aborts `args()`; on Windows a lone surrogate arrives as valid UTF-8 and
  does not (F9, carried, to be run again).

The tag the author's goal asks for waits on seven items (F12); **245 and 283
wait on this sitting**.

## The proposal to judge

The coordinator's starting point, written to be refuted. Each part is a
route among others, and the questions ask for the others.

1. **A string refuses what a reader cannot see.** A string literal refuses
   the raw characters panel 188's predicate refuses in a group head's string
   (F6): a C0 or C1 control, a Default_Ignorable code point, U+2028, U+2029.
   The line end ends the string already, and CR keeps
   `raw_carriage_return`.
2. **A comment refuses what reorders a line**: the bidirectional controls,
   U+2028 and U+2029.
3. **A refused character stays writable**, by a spelling that shows it.
4. **No `str` lends a NUL to C without being told**, and no runtime door that
   hands a name to the operating system opens a file the program did not
   name: the program reads a failure, never another file.
5. **`args()` keeps its sentence on Windows**: an argument that is not
   Unicode aborts there too.

## The questions

**Q1. A string.** Which raw characters a string literal refuses: all of F1's
54, a subset, or none.
- **Which list.** Panel 188's (`DEFAULT_IGNORABLE`, C0, C1, U+2028, U+2029)
  or the compiler's wider `UNSEEN`, which adds NBSP, U+2000 to U+200A,
  U+202F, U+205F, U+3000 and U+FFF9 to U+FFFB (F6). The tab's class,
  *visible as space and not a space*, has all these members, not the tab
  alone.
- **What real text needs.** ZWJ and ZWNJ, which Persian and Indic text spell
  with, the variation selectors of emoji, and the tag characters of a
  subdivision flag are Default_Ignorable (F6). Say what a program printing
  each writes under your route.
- **The class.** The precedents disagree (F6): panel 066's raw CR is not a
  thesis rule, panel 188's unshowable character is one, and the group head's
  NUL, `unwritable_name`, is not. Say which a string's ESC follows, and which
  its NUL, and whether `check --permissive` drops it.
- **The fix**: `certain` where a spelling writes the character, a `guess` or
  none elsewhere, by `.claude/rules/diagnostics-and-goldens.md` (*a
  `certain` fix repairs the defect the diagnostic names*).
- **One spelling.** A raw tab in a string is today a second spelling of `\t`
  (F3, design.md `:994-995`): is it refused on that ground alone, whatever
  you rule on visibility?
- **`selfhost/cli/compile.hero:83`** (F5): what replaces its two raw U+0001.

**Q2. A comment.** Which raw characters a comment refuses: the
bidirectional controls and U+2028 and U+2029 alone, every Default_Ignorable
code point, every control, or none.
- **Whose reader.** A model reads code points in logical order; an override
  reorders what a person's editor or browser draws. A refusal for the person
  is robustness (CLAUDE.md § Precedence, rank 3) rather than a thesis rule,
  and that decides whether `check --permissive` drops it. Say which.
- **Routes to be listed, even to be refused**:
  - a refusal;
  - a warning;
  - a refusal of an UNBALANCED bidirectional run only;
  - showing it: every tool that prints the comment writes `<U+202E>`, as
    defect 244 does in a diagnostic and defect 290 in the dumps (F11). It
    does not reach an editor or a web page, where Trojan Source bites: weigh
    that.

No tracked comment holds any of these characters (F5, approximate).

**Q3. Writing what is refused.** If Q1 refuses a character, how a program
that needs it writes it. Routes:
- **(a′) The run-time route, which exists today** (F10):
  `esc.validated_bytes().must()` over `esc: [u8] = [27]` prints ESC, cannot
  make a NUL, and the compiler writes its own control characters so 13 times.
  The spec does not say a `[u8]` answers it (`:384`). A few words there, no
  new escape, one spelling per character in a literal.
- **(b) `\u{...}`** for every code point but 0 and a surrogate.
- **(c) `\xNN`**, which writes bytes, so it can write half a character, and
  `01` to `7f` writes none of F6's real-text characters.
- **(d) An escape only for what a literal refuses raw**, so `\u{e9}` is
  refused with a `certain` fix to `é`: design.md `:994-995`'s one spelling
  kept.
- **(e) A named constant** for the few that matter.
- **(f) `fmt` writing a refused character by its escape**, beside or instead
  of a refusal, once an escape exists.

For each, say:
- how it keeps a NUL out (design.md `:1016-1018` froze the escapes for
  that, F3);
- what it costs in the spec (F8);
- what it does in a character literal (`'\u{e9}'` against design.md
  `:996`'s *one ASCII character or one escape*) and in a group head's string;
- **what today's `unknown_escape` message becomes** (F10): it tells a writer
  of `\x1b` that *a string holds its characters as themselves*, which points
  at a raw control character, and its fix is a `guess` to `\\x1b`. Under your
  route, what does it say, and may its fix be `certain`?
- **in which order the escape, the seed and the refusal land**: the
  compiler's own source can use a new escape only after the seed reads it,
  and a refusal of raw U+0001 must land after `compile.hero:83` changes.
  (a′) works on today's seed.

The languages named beside (b) and (c) in the first version of this brief
were the coordinator's recollection: the historian verifies them.

**Q4. A NUL at the C boundary, and the runtime's doors.** After Q1, where
does a program's `str` still get a NUL? Measured: `read_file` (F4). By the
code, `validated()`, `validated_bytes()` and `args()` stop at the first zero
(F4): a negative claim on the critic's vocabulary, **so measure it**. Then
say what `.cstr()` does with a `str` holding one. Routes to be costed, each
built where it can be:
- (a) a scan at the lend that aborts, which turns §4.20's free lend into one
  scan per lend (F3); **not inside `hero_str_cstr`**, whose four internal
  callers pass content with its length, where a NUL is correct (F4):
  `write_file` would abort on a correct program;
- (b) a fact kept per `str` from its construction, so the lend stays O(1);
- (c) a fallible lend, `cstr?`, priced by the lends in code a gate builds
  (F7: count them with `heroes lex`);
- (d) **the NUL refused in the scan every constructor already makes**: a
  `str` from bytes is scanned whole for UTF-8 (`str.c:301-304`, and twice in
  `hero_file_read`), so a zero test there keeps the invariant *no `str`
  holds a NUL*. `.cstr()` stays free, and `read_file` answers a failure for a
  file holding one, losing NUL-separated data (`find -print0`). Measure the
  compare's cost;
- (e) **doors that take the `str`**, not a C string, and answer a failure at
  each door (F7: every door of `hero_os.h` takes `const char *`;
  `hero_run_arg`, `run.c:76`, takes the `str` and answers a panic).

**And the runtime's own doors** (Q-i, F4): every call that hands a name to
the operating system, enumerated from the code (F7's grep is a vocabulary).
Say what each does with a name holding a NUL, and with which failure, and
whether that failure is a spec word.
- `hero_run_arg`'s answer is an abort. Is that the precedent, or *a failure
  the program reads*?
- `read_file` and `write_file` reach their door through `path.cstr()`
  (`library_source.hero:208`, `:222`), so a lend-time abort fires before the
  door can answer. Which check runs first, and in what code?

The floor is **never another file**. Robustness ranks above speed here
(CLAUDE.md § Precedence), so a scan's cost is measured and reported, not
argued.

**Q5. `args()` on Windows** (Q-c, F9). Run it again on the base:
- an argument holding a lone surrogate, through the trunk's narrow door;
- through panel 191's adopted route, the UTF-8 manifest;
- through the UCRT's wide `argv` with a strict conversion (1113);
- through the wide `argv` converted losslessly to WTF-8, so the existing
  check refuses (panel 191's compiler-engineer's `hero_win_name_bytes`). On
  this Mac those bytes already abort `args()` and give `args_checked()`
  `not_text` (F9).

Then choose: make `:324-325` true on Windows, or change the sentence, and
say what `args_checked()` answers. Panel 191's route lands in batch 11,
beside this sitting (lane b11-windows): a route here must compose with it.

**Q6. The specification.** What each route owes `spec/heroes-spec.md`:
- `:35-36`, what a string and a comment hold;
- `:47-48`, the escapes;
- `:384`, whether a `[u8]` answers `validated_bytes()` (route (a′));
- what `.cstr()` does with a NUL, and what a runtime door answers;
- `:324-325`, `args()`.

Price each sentence on a copy (F8: real 9,392, headroom 848) against
design.md §1.6's payment: a named removal, or a registered prediction naming
an existing instrument and the milestone at which it is scored.

**Q7. The shapes beside.** Every other place an invisible or reordering
character reaches a reader, and what each does today:
- a character literal (a raw ESC there checks at exit 0, the critic);
- an f-string's text;
- a `test` title, and **`heroes test`'s own output**, which prints a failing
  title's ESC raw (F11);
- **the runtime's panic and `assert` messages**, which print ESC raw (F11);
- a doc comment;
- a name (ASCII-only, `:35`);
- a diagnostic that quotes the line;
- `fmt`'s output;
- the site's highlighter.

And **who refreshes `DEFAULT_IGNORABLE`** when Unicode moves (read from
15.0.0, F6), under which gate: a later table widens what the checker refuses.

Only a shape with this repair's own cause belongs here; any other is filed
apart (`.claude/rules/verification.md` § Bounded discovery).

## Seats convened

- `compiler-engineer`: Q1, Q2, Q3 and Q7, building the refusal and the
  spelling in its copy;
- `ffi-pragmatist`: Q4 and Q5, building the lend and the doors, and the
  Windows box for Q5;
- `spec-warden`: Q6, and Principle 0's burden for every sentence;
- `historian`: precedent, by web search, for each route of Q1 to Q5;
- `llm-ergonomist`: a blind experiment within the 5 USD cap, run by the
  coordinator as fresh sessions outside the repository;
- the completeness critic: over the briefs first and over the reports after.

## The frozen tree and your copy

`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
Your copy is `<scratchpad>/192-<seat>/`, made with `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 4c3524fb | tar -x -C <your
copy>`. Build your compiler inside it from the seed, `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`, a few seconds; the seed's sha256
begins `c79ffd5ad005c301` and the compiler built from it has sha1
`8084f018f5387536`: check yours. Rebuild from `selfhost/` after an edit with
`./heroes build selfhost/main.hero -o heroes`, under a minute. F4's programs
are `<scratchpad>/192-facts/nul/`, F1's `<scratchpad>/192-facts/`, F10's
`<scratchpad>/192-facts/repair/` (read only; copy what you need).

**What you may not do:**
- **Copies.** Never build, run or read inside another seat's copy or a
  lane's worktree (`.claude/worktrees/`), and in the trunk write nothing
  but your own report.
- **Deletion.** **Never `rm` anything**, `-f` or `-rf`: a destructive
  command waits for the author's permission. Use a new folder name instead.
- **Paid runs.** **No paid run**: the llm-ergonomist's sessions are the
  coordinator's, within the author's 5 USD, and no seat runs `heroes measure
  --refresh`.
- **Load and containers.** At most three processes at once, and no timing:
  a cost is a count of instructions retired (`/usr/bin/time -l`), never a
  duration. Docker: one container at a time, `docker ps -q` empty first, and
  stop what you start. Batch 11's lanes and the Windows lane run beside you.
- **The Windows box.** It is held by batch 10's leg and then by lane
  b11-windows, so **the ffi-pragmatist alone uses it, and only once the
  coordinator's message says it is free**. Then: `ssh win`, one folder of
  your own under `/c/w/192-*`, files sent as
  `<scratchpad>/platforms/windows-b10.sh` sends them (read it, never run it).
  The box stopped four times today from outside the guest (panel 191's
  critic), so write your results as you go.
- **The clock.** **Every time you write is read from `date` at that moment**,
  never estimated.

**Your report** is `docs/panel/192-reports/<seat>.md` in the TRUNK, written
as you go. It holds:
- a verdict per question you take;
- what you built and ran;
- your cost;
- a falsifiable prediction;
- the condition that would change your verdict.

English, no em dashes.
