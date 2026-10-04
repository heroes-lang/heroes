# Panel 192, the shared brief: what a string or a comment may hold, and what reaches C from a `str`

Written 2026-10-04 from 17:18 by the coordinator, on batch 10's closing
commit `4c3524fb`, frozen from the briefs to the synthesis. Every number
names the command or the file that produced it; `00-facts.md` holds the
facts, cited as F1 to F9. What was not run says so.

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

Two of these are `systemic`: their remedy needs a ruling no rule reaches,
which is why they come here. **The tag the author's goal asks for waits on
them**: zero `blocking` and zero `systemic` items.

## The proposal to judge

The coordinator's starting point, written to be refuted. Each part is a
route among others, and the questions ask for the others.

1. **A string refuses what a reader cannot see.** A string literal refuses
   every raw character panel 188's predicate refuses in a group head's
   string (F6): a C0 or C1 control, a Default_Ignorable code point, U+2028,
   U+2029. The line end ends the string already, and CR keeps
   `raw_carriage_return`.
2. **A comment refuses what reorders a line**: the bidirectional controls,
   U+2028 and U+2029.
3. **A refused character stays writable by an escape that shows it**:
   `\u{...}` opened from design.md §4.3's freeze (F3), refusing `\u{0}` and
   a surrogate, so a program that prints a terminal's ESC writes `\u{1b}`.
4. **No `str` lends a NUL to C without being told.** `.cstr()` refuses a
   `str` holding a NUL, and every runtime door that hands a name to the
   operating system refuses a name holding one, as a failure the program
   reads, never another file.
5. **`args()` keeps its sentence on Windows**: an argument that is not
   Unicode aborts there too.

## The questions

**Q1. A string.** Which raw characters a string literal refuses: all of
F1's 54, a subset, or none.
- The tab is the hard case: visible as space, and it has an escape, `\t`.
- The code and its class: one code or several; a thesis rule (dropped by
  `check --permissive`) or not; panel 066's `raw_carriage_return` and
  panel 188's `unshowable_name` are the precedents.
- The fix: `certain` where an escape writes the character (`\t`, and `\u`
  if Q3 opens it), a `guess` or none elsewhere, by
  `.claude/rules/diagnostics-and-goldens.md` (*a `certain` fix repairs the
  defect the diagnostic names*).
- What the compiler's own `selfhost/cli/compile.hero:83` becomes (F5).

**Q2. A comment.** Which raw characters a comment refuses: the
bidirectional controls and U+2028 and U+2029 alone, every Default_Ignorable
code point, every control, or none. A comment carries no value, so the
reason to refuse there is what the reader sees, not what the program does:
say whether that is a thesis rule. Routes to be listed, even to be refused:
- a refusal;
- a warning;
- `fmt` writing the character visibly (impossible without an escape in a
  comment);
- a refusal of only an UNBALANCED bidirectional run, as some compilers do.

**Q3. Writing what is refused.** If Q1 refuses a character, how a program
that needs it writes it. Routes:
- (a) it cannot, in a literal: built at run time if the language has a way
  (say which, or that none exists, by a command);
- (b) `\u{...}`, as Rust, Swift and JavaScript write it;
- (c) `\xNN`, as C, Python and Go write it, which writes bytes and so
  can write half a character;
- (d) both;
- (e) a named constant for the few that matter.

design.md §4.3 froze these because they can produce an interior NUL (F3):
say how each route keeps a NUL out, and what it costs in the spec (F8). The
languages named beside (b) and (c) are the coordinator's recollection, not
run: the historian verifies them.

**Q4. A NUL at the C boundary, and the runtime's doors.** After Q1, a `str`
still gets a NUL from a file, an argument or C itself (F4). Say what
`.cstr()` does then. Routes to be costed, each built where it can be:
- (a) a scan at the lend that aborts, which turns §4.20's free lend into
  one scan per lend (F3);
- (b) a fact kept per `str` from its construction, so the lend stays O(1)
  and every constructor pays;
- (c) a fallible lend, `cstr?`, at 215 call sites (F7);
- (d) refusing the NUL at the doors that make a `str`.

**And the runtime's own doors** (Q-i, F4): every call that hands a name to
the operating system. Enumerate them from the code, since F7's grep is the
coordinator's vocabulary. Say what each does with a name holding a NUL,
with which failure, and whether that failure is a spec word (read_file's
codes are `spec/heroes-spec.md`'s). A failure the program reads is the
floor: **never another file**. Robustness ranks above speed here
(CLAUDE.md § Precedence), so a scan's cost is measured and reported, not
argued.

**Q5. `args()` on Windows** (Q-c, F9). Run it again on the base:
- an argument holding a lone surrogate, through the trunk's narrow door;
- through panel 191's adopted route, the UTF-8 manifest;
- through the UCRT's wide `argv` with a strict conversion.

Then choose: make `:324-325` true on Windows, or change the sentence; and
say what `args_checked()` answers for that argument. Panel 191's route
lands in batch 11, beside this sitting: a route here must compose with it.

**Q6. The specification.** What each route owes `spec/heroes-spec.md`:
- `:35-36` says strings hold *any but a raw carriage return or line end*,
  and comments *any UTF-8*;
- no sentence says what `.cstr()` does with a NUL (F2).

Price each sentence on a copy (F8: real 9,392, headroom 848) against
design.md §1.6's payment: a named removal, or a registered prediction naming
an existing instrument and the milestone at which it is scored.

**Q7. The shapes beside.** Every other place an invisible or reordering
character reaches a reader, and what each does today:
- a character literal;
- an f-string's text;
- a `test` title;
- a doc comment;
- a name (ASCII-only, `:35`);
- a diagnostic that quotes the line;
- `fmt`'s output;
- the site's highlighter.

Only a shape with this repair's own cause belongs here; any other is filed
apart (`.claude/rules/verification.md` § Bounded discovery).

## Seats convened

- `compiler-engineer`: Q1, Q2, Q3 and Q7, building the refusal and the
  escape in its copy;
- `ffi-pragmatist`: Q4 and Q5, building the lend and the doors, and the
  Windows box for Q5;
- `spec-warden`: Q6, and Principle 0's burden for every sentence;
- `historian`: precedent, by web search, for each route of Q1 to Q4;
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
`./heroes build selfhost/main.hero -o heroes`, under a minute. F4's
programs are `<scratchpad>/192-facts/nul/`, F1's `<scratchpad>/192-facts/`
(read only; copy what you need).

**What you may not do:**
- **Copies.** Never build, run or read inside another seat's copy or a
  lane's worktree (`.claude/worktrees/`), and in the trunk write nothing
  but your own report.
- **Deletion.** **Never `rm` anything**, `-f` or `-rf`: a destructive
  command waits for the author's permission. Use a new folder name instead.
- **Paid runs.** **No paid run**: the llm-ergonomist's sessions are the
  coordinator's, within the author's 5 USD, and no seat runs `heroes measure
  --refresh`.
- **Load and containers.** At most three processes at once, and no timing.
  Docker: one container at a time, `docker ps -q` empty first, and stop
  what you start.
- **The Windows box.** `ssh win`, one folder of your own under `/c/w/`,
  files sent as `<scratchpad>/platforms/windows-b9.sh` sends them (read it,
  never run it). The box stopped four times today from outside the guest
  (panel 191's critic), so write your results as you go.
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
