# Panel 220, the shared brief: C2 as built costs more than the 450 code lines its seat set

Written 2026-10-17 from 11:50 by the coordinator, on the trunk at `b2c81211d` and lane
`lane-land215` (its committed head `bc978b09d`, C2 uncommitted in its working tree while its gate
runs). Convened by the compiler-engineer's own condition of panel 218
(`docs/panel/218-reports/compiler-engineer.md` § The seat's answer, *condition*): *I turn to
veto if the reader grows into parsing C text rather than slicing clang's offsets, or if the cost
crosses 450 code lines without a sitting*. Panel 218's file, ratified 2026-10-17, is read first:
`docs/panel/218-a-header-constants-initialiser-is-read-from-clangs-dump-and-run-under-the-probes-checks-at-o0-and-every-probe-asks-the-c-standards-meaning.md`.
**Seat**: compiler-engineer alone, the ceiling being the one question (CLAUDE.md § 4: the seats
whose input differs).

## The question

C2, panel 218's R1, is built and measures about 785 code lines where its seat predicted at most
450. Does it stand as built, and if not, what is cut or reshaped, with the cost of each cut
measured?

## What was measured (the coordinator's commands; the seat re-runs what it relies on)

Code lines by `tests/harness/suite_layout.hero`'s rule (non-blank, non-comment, outside `test`
blocks; the `awk` the coordinator used: `awk '/^test "/ { t = 1; next } t && /^[^ \t#]/ { t = 0 }
!t && !/^[[:space:]]*#/ && !/^[[:space:]]*$/' <file> | wc -l`), in
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-land215/selfhost/`:

| module | code lines | what it is |
|---|---|---|
| `cli/clang_tree.hero` (new) | 170 | an iterative reader of `-ast-dump=json`: nodes of `inner` arrays, the fields the walk asks, a `referencedDecl` stub never a node |
| `cli/value_init.hero` (new) | 216 | the walk: accessors' bodies, references followed, the predecessor rule, brace lists leaf by leaf, references spliced at their dump type, the probe's functions |
| `cli/value_text.hero` (new) | 122 | before the dump, `reaches` (the pre-filter); after it, `presumed`, a byte's file and line from `clang -E`'s line markers |
| `cli/value_c2.hero` (new) | 131 | the stage: key, pre-filter, dump, walk, probe at `-O0`, verdict, the dump's canary |
| `cli/value_told.hero` | 214, from 124 at `760a9035b` | C2's lines told, the header's line read |
| `emit/value_probe.hero` | 136, from 119 | a dispatcher returning the guard's answer, `disagree` read |
| `cli/value_run.hero` (new) | 177 | panel 216's running machinery moved out of `cli/value_ask.hero` (217 to 78) plus a factored compile, about 38 new |

Behaviour, measured on this Mac with the lane's compiler: the compiler's own cold `heroes build
selfhost/main.hero` asks 4 units and runs 0 dumps (`ls build/init-*/base.i`: none); the reader
reads SDL3's 12,033,929-byte dump in 0.31 s `user` with the node, declaration and reference counts
Python's `json` gives (13,502; 1,174 enumerators plus 13 objects; 223); a study unit's 12 questions
trap on the 5 undefined values and none of the 7 defined; the case
`tests/golden/unsupported/fixedbugs-2109-a-value-clang-folds-in-an-initialiser-is-refused.hero`
refuses its 8 constants each naming the declaration and line that traps, and
`tests/golden/run/fixedbugs-2109-a-value-c-defines-in-an-initialiser-builds.hero` prints C's 14
values at `-O0` and `-O2`. The lane's gate on all of it is running as this is written.

## What the seat is asked

Read the seven modules. Rule: **stand** (the cost is what the conditions of panel 218 cost, and
the ceiling holds), **reshape** (name the cut, build it far enough to measure the lines it saves
and the behaviour it keeps), or **veto** (§1.1's ceiling, or the reader grown into parsing C).
Say which of panel 218's conditions each module pays for, and whether any is paid twice.

## Rules

- **Read only**: the lane's tree is owned by its gate; read files, never write, build or run a
  harness there. Build in your own copy if you build: a detached worktree under
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/220-compiler-engineer` from `bc978b09d`,
  the lane's working files copied in by `cp`, `rm -rf build`, your compiler from its seed, then
  from its sources. Remove it when you finish.
- **Write your report as you go** to `docs/panel/220-reports/compiler-engineer.md` in the trunk,
  the one file you write there; every number names its command; what you could not run says
  *unrun*.
- No paid run, no credential printed, a program that may not end bounded in time and bytes,
  nothing outside the repository's root but your scratchpad and the Linux containers.
