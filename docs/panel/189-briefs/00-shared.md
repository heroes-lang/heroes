# Panel 189, the shared brief: a source byte that is not UTF-8, and the code that tells it

Written 2026-10-03 by the coordinator on the trunk at `7d9f2e8f`, frozen from
the briefs to the synthesis, then **repaired on 2026-10-04 from 00:08 after the
completeness critic's first pass** (`docs/panel/189-reports/completeness-critic-briefs.md`;
the text it read is `00-shared-before-the-critic.md`). Every number names the
command or the file that produced it; `00-facts.md` holds the facts, cited as
F1 to F8. What was not run says so.

## Why this sitting

Defect 227 (`blocking`, `docs/work/DEFECTS.md`): a `.hero` file holding a byte
that is not UTF-8 is answered *error: cannot read `p.hero`* at exit 2 by
every verb, where the file was read and the author can be told which line
holds the byte (F1, F2); and a `use`d module holding one is told falsely that
it is not there (F1, `used`). Lane b8-source stopped before writing a code,
because a new code is a diagnostic class (CLAUDE.md § 4). The author's answer
*3a*, 2026-10-03 (`docs/records/done/2026-10-03-2321-defect-227-takes-a-new-code-not-text-through-a-sitting.md`):
a new code, `not_text`, through a sitting. **The recommendation the author
answered attributed the `not_text` precedent to panel 087**; corrected at
23:37, after the answer and told to the author: 087 chose the conservative
`read_failed`, and `not_text` became `read_file`'s code on 2026-09-03 (F3).
The blind seat's paid sessions are capped at **5 USD in all**, the author's
*"5 dollari"*, recorded in
`docs/records/log/2026-10-03-2343-panel-189-convened-for-not-text-the-blind-seat-capped-at-5-usd.md`
(committed with the sitting).

## The proposal to judge

Every verb that reads a `.hero` file tells one that is not UTF-8 with
`error[not_text]` at exit 1, at the first byte that is not UTF-8, naming the
file, the line and column, and the byte's value; a `use`d module the same, at
its own file. `not_text` is the code a program already sees when it reads
bytes that are not text (F3); whether a compiler diagnostic and a program's
failure should share the word is a question (Q1), not a premise.

## The questions

**Q1. The code and its class.** `not_text`, or another name a reader would
read better (the historian's precedents; the blind arms carry three names);
whether one word for a program's failure and a compiler's diagnostic is one
state or two meanings (F4: the spec names `not_text` at line 385). And its
class: a seat's reading, not a premise. A bad byte inside a comment changes no
program's meaning, yet the file is not text a `str` can hold; is the refusal
a thesis rule (dropped by `check --permissive`, Part 11's control arm) or not.
**Routes to be listed, even to be refused** (CLAUDE.md § 12 holds a refusal to
a feature's standard): accepting a bad byte inside a comment while refusing it
in a string; an encoding declaration or transcoding (PEP 263's route).

**Q2. The message and its fix.** What it names (the byte, its position, the
encoding the bytes most likely are, such as Latin-1, Windows-1252 or UTF-16,
and what to do), and whether a fix is `certain`, a `guess`, or none, by
`.claude/rules/diagnostics-and-goldens.md` (*a `certain` fix repairs the
defect the diagnostic names*), for a byte in a string and for one in a
comment.

**Q3. How many, what after, and what is written.** One diagnostic per file
(the first bad byte), one per line holding one, or every bad byte with a
bound; whether the rest of the file is lexed and checked past it, and if so
whether lines and columns stay exact (F5: columns count characters; one
replacement per bad byte or per run is unrun, F6). And **a requirement the
critic measured (F8): no route may hand a text with replaced bytes to a
writer**; `check --apply --in-place` rewrites a file while an unfixable error
stays in it, so say what every writer (`check --apply`, `fmt --in-place`) does
with a file that is not text. A one-turn repair is the thesis's measure
(design.md §4.17).

**Q4. Where it lives, what it costs, where it lands.** The runtime (a check
that reports the first bad offset, a read that keeps the text; whether
`HERO_RUNTIME_ABI` moves, panel 089 having held it for a new function, F3; or
the new C function bound in the compiler's own extern groups, which keeps it
out of every program's C, F3); `cli/input.hero` (exit 1, F2); `modules.hero`
(317 of 320, F7) and `scan.hero` (291 of 300, F7); **every reader**, the two
of `cli/input.hero` and the three that read through their own calls (`mutate`,
`probe`, `measure`, F2). **The route lands on top of lane b8-source's tip
`6ee963e7`**, which rewrote `cli/input.hero` and `modules.hero` for defect
236 (F6), so it is built and costed there. **How the net holds a case whose
file is not UTF-8**, given that `annotations` and `canonical` cannot read one
and CLAUDE.md § 9 asks every diagnostic annotated in its source (F8; a run
golden already writes such bytes through C at run time). **A change in
`runtime/` runs its cases on each platform** (`.claude/rules/platforms.md`):
Linux arm64 and the Windows box.

**Q5. The specification.** § 1 says comments and strings are UTF-8 and no
sentence says what a FILE that is not UTF-8 gets (F4). Is a sentence owed, at
what price and against what payment (design.md §1.6's rule: a named removal,
or a registered prediction naming an existing instrument **and the milestone
at which it is scored**), or is the message the rule's home (panel 055's
precedent, its sentence removed at `aab44f9b`; panel 188's R10 weighed the
same two standards).

**Q6. The shapes beside.** Every other place a byte that is not text reaches
the compiler, what each does today, and whether it shares 227's cause (*a
read that is not text taken as unreadable or absent*) or another: the
runtime's own sources and a C header (F8, both taken for absence), UTF-16 and
a valid U+FFFD (F8), the CLI's own argv, an environment variable
(`HEROES_RUNTIME`, `selfhost/cli/process.hero:192`), a file or directory name
that is not UTF-8 (Linux only, F8), a `.pc` answer, clang's or the linker's
text read back. Only a shape with this repair's own cause belongs to 227; any
other is filed apart (`.claude/rules/verification.md` § Bounded discovery).

## Seats convened

`compiler-engineer` (builds the route), `ffi-pragmatist` (the runtime's C and
the platforms; **convened after the critic's first pass**, which found the
platform runs assigned to no seat), `spec-warden`, `historian`,
`llm-ergonomist` (a blind experiment within the 5 USD cap), and the
completeness critic over the briefs first and the reports after.

## The frozen tree and your copy

`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
Your copy is `<scratchpad>/189-<seat>/`, made with `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 7d9f2e8f | tar -x -C <your copy>`
(the builder adds a second copy from the lane's tip, its brief says how);
build your compiler inside it from the seed, `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (the seed's sha256 begins `3bc3aa8bd2b5ba65`, the
trunk's compiler built from it `958320f39dee0b9e`: check yours). Rebuild from
`selfhost/` after an edit with `./heroes build selfhost/main.hero -o heroes`.
F1's cases are `<scratchpad>/189-facts/` (read only; copy what you need).
Never build, run or read inside another seat's copy or a lane's worktree, and
in the trunk write nothing but your own report. **Never `rm` anything**, `-f`
or `-rf`: a destructive command waits for the author's permission (it cost
156 minutes on 2026-10-03); use a new folder name instead. **No paid run**: the
llm-ergonomist's sessions are the coordinator's, within the author's 5 USD; no
seat runs `heroes measure --refresh`. At most three processes at once, no
timing. Docker: one container at a time, `docker ps -q` empty first. The
Windows box: `ssh win`, one folder of your own under `/c/w/`, files sent in
1 MB parts as `<scratchpad>/platforms/windows2-u.sh` does (read it, never run
it). **Every time you write is read from `date` at that moment, in its own
command before the text that holds it**, never estimated. Your report is
`docs/panel/189-reports/<seat>.md` in the TRUNK, written as you go: a verdict
per question, what you built and ran, your cost, a falsifiable prediction,
the condition that would change your verdict. English, no em dashes.
