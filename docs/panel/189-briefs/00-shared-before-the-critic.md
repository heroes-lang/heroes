# Panel 189, the shared brief: a source byte that is not UTF-8, and the code that tells it

Written 2026-10-03 by the coordinator on the trunk at `7d9f2e8f`, frozen from
the briefs to the synthesis. Every number names the command or the file that
produced it; `00-facts.md` holds the facts, cited as F1 to F7. What was not
run says so.

## Why this sitting

Defect 227 (`blocking`, `docs/work/DEFECTS.md`): a `.hero` file holding a byte
that is not UTF-8 is answered *error: cannot read `p.hero`* at exit 2 by
every verb, where the file was read and the author can be told which line
holds the byte (F1, F2); and a `use`d module holding one is told falsely that
it is not there (F1, `used`). Lane b8-source stopped before writing a code,
because a new code is a diagnostic class (CLAUDE.md § 4). The author's answer
*3a*, 2026-10-03 (`docs/records/done/2026-10-03-2321-defect-227-takes-a-new-code-not-text-through-a-sitting.md`):
a new code, `not_text`, through a sitting; the blind seat's paid sessions
capped at **5 USD in all** (the author's words, *"5 dollari"*).

## The proposal to judge

Every verb that reads a compilation tells such a file with `error[not_text]`
at exit 1, at the first byte that is not UTF-8, naming the file, the line and
column, and the byte's value; a `use`d module the same, at its own file.
`not_text` is the name a program already sees for this state, `read_file`'s
failure since 2026-09-03 (F3); no compiler diagnostic carries it today.

## The questions

**Q1. The code.** `not_text`, or another name a reader would read better
(the historian's precedents, the blind arms); and its class: it is a refusal
without which the file has no meaning, so `check --permissive` (Part 11's
control arm) keeps it, unless a seat measures otherwise.

**Q2. The message and its fix.** What it names (the byte, its position, the
encoding the bytes most likely are, such as Latin-1 or Windows-1252, and what
to do), and whether any fix can be `certain` by
`.claude/rules/diagnostics-and-goldens.md` (*a `certain` fix repairs the
defect the diagnostic names*): re-encoding from Latin-1 rewrites bytes the
author may have meant otherwise, and deleting a byte changes a string's
value. A `guess` is a question, not a premise.

**Q3. How many, and what after.** One diagnostic per file (the first bad
byte), one per line holding one, or every bad byte with a bound; and whether
the rest of the file is lexed and checked past it (each bad byte read as one
replacement character keeps lines and columns exact, F6) or the file stops
there. A one-turn repair is the thesis's measure (design.md §4.17).

**Q4. Where it lives and what it costs.** The runtime (a check that reports
the first bad offset and a read that keeps the text: a C-boundary change in
`runtime/`, and whether its ABI number moves), `cli/input.hero` (exit 1, F2),
`modules.hero` (317 of its 320, F7), every verb that reads a compilation
(`check`, `build`, `run`, `test`, `lex`, `parse`, `fmt`, and whatever else
reads a `.hero` file: `measure`, `probe`, `mutate`), and the lines each
costs by the layout suite's unit.

**Q5. The specification.** § 1 says comments and strings are UTF-8 and no
sentence says what a file that is not UTF-8 gets (F4). Is a sentence owed,
at what price (design.md §1.6's payment rule), or is the message the rule's
home (panel 055's precedent, removed at `aab44f9b` because the message states
the rule).

**Q6. The shapes beside.** Every other place a byte that is not UTF-8 can
reach the compiler, and what each does today: argv (spec § 11: *one that is
not UTF-8 aborts* for a program's `args()`; the CLI's own argv), an
environment variable (`HEROES_RUNTIME`), a file name or a directory name, a
`.pc` answer, clang's or the linker's text read back, a golden or a fixture
file. Only a shape with this repair's own cause belongs to 227; any other is
filed apart (`.claude/rules/verification.md` § Bounded discovery).

## Seats convened, and one not

`compiler-engineer` (builds the route), `spec-warden`, `historian`,
`llm-ergonomist` (a blind experiment within the 5 USD cap), and the
completeness critic over the briefs first and the reports after. **Not
convened: `ffi-pragmatist`.** No C binding is at stake; the runtime change
(F2's check gaining an offset) is the compiler-engineer's to build, and Q6's
`.pc` and clang-text shapes are measurements any seat with a shell can run.
Recorded so a later reader can disagree with the choice (CL-023).

## The frozen tree and your copy

`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
Your copy is `<scratchpad>/189-<seat>/`, made with `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 7d9f2e8f | tar -x -C <your copy>`;
build your compiler inside it from the seed, `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (the seed's sha256 begins `3bc3aa8bd2b5ba65`, the
trunk's compiler built from it `958320f39dee0b9e`: check yours). Rebuild from
`selfhost/` after an edit with `./heroes build selfhost/main.hero -o heroes`.
F1's cases are `<scratchpad>/189-facts/` (read only; copy what you need).
Never build, run or read inside another seat's copy or a lane's worktree, and
in the trunk write nothing but your own report. **Never `rm -rf` anything**:
a destructive command waits for the author's permission (it cost 156 minutes
on 2026-10-03); use a new folder name instead. **No paid run**: the
llm-ergonomist's sessions are the coordinator's, within the author's 5 USD; no
seat runs `heroes measure --refresh`. At most three processes at once, no
timing. Docker: one container at a time, `docker ps -q` empty first. **Every
time you write is read from `date` at that moment**, never estimated. Your
report is `docs/panel/189-reports/<seat>.md` in the TRUNK, written as you go:
a verdict per question, what you built and ran, your cost, a falsifiable
prediction, the condition that would change your verdict. English, no em
dashes.
