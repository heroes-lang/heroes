# Panel 214, the shared brief: who is told when clang refuses a header of the program's own

Written 2026-10-14 from 00:45 by the coordinator. **Every number, path and count below is followed
by the command that produced it, run while this brief was written** (`.claude/skills/panel/SKILL.md`
§ 2, CL-077); a sentence that says the tree lacks something names what was searched. Read your own
brief after this one.

## The sitting

Convened by the author's choice at M-typed-inspection's close, 2026-10-13, through the question
widget: the option *repair them, then tag*, whose words included *and I open a panel on the general
rule of who to blame, because six repairs in a row case by case say a rule is missing, not a case*
(the coordinator's wording of the option, recorded in decision issue 2055, item 4). A **full panel**: who is told decides a diagnostic's class and
its exit code (CLAUDE.md § 4: a diagnostic class is a trigger), and the specification may owe a
sentence. Not asked again (§ 4: once per milestone, and the author asked for it by name).

**The proposal, as a question with its routes** (the routes are the coordinator's list and not the
option set; a route nobody listed is worth more than a vote):

When clang refuses a unit that holds a header a group of the program names, the compiler must say
whose mistake it is: the program's header, told `ffi_header_refused` at exit 1 on the group's
string, or the compiler's own, told `internal error` at exit 2 (`.claude/rules/c-boundary.md:49`:
*a clang failure is normally exit 2 and says the compiler is wrong*, with twelve named members
of the exit-1 class; and `c-boundary.md:133`: *the narrowing is `declaration()`, not whose text it
is*, which every route must be read against). Defect 360 (issue 1687, 2026-10-06) ruled the line: *a stack or note reaching
the runtime's or the build's files stays the compiler's*. From the night of 2026-10-12 to the evening
of 2026-10-13 that line was carved repair after repair, each finding the next shape (2049, 2052, 2051
and 2053 landed at noon and in the evening of 10-13, not overnight; the critic's pass 1, by the
commits' dates):

| defect | commit | what it moved to the header | module it added or changed |
|---|---|---|---|
| 2034 | `2190ff8a3` | (kept `-Wformat` an error in a header's region; the start of the chain) | `emit/header_region.hero` |
| 2036 | `4dcc1e197` | (passed over the five parts of `-Wformat` C defines; its other shapes a known cost by the author's ruling, `5a92815d3`, decision 2048) | `emit/header_region.hero` |
| 2035 | `afd34b4c2` | a program kept under `build/` or `runtime/` (the compiler's files known by exact values, not a folder name) | `cli/group_file.hero`, `cli/runtime_key.hero` |
| 2040 | `1e888df16` | a macro-expansion note (`NULL` defined in a system header the runtime reads) | `cli/clang_first.hero` |
| 2044 | `0c7eae194` | a note at the runtime's identical re-declaration of a `<stdint.h>` type | `cli/standard_name.hero` |
| 2045 | `31a112ae5` | a note at the runtime's declaration of the identifier the error names | `cli/runtime_name.hero` |
| 2049 | `801e51069` | a C library name a later standard header declares; a name the emitter writes; an undeclared runtime call; a typo whose suggestion is a runtime name | `cli/declared_after.hero`, `cli/runtime_name.hero` |
| 2051 | `d7fd9c111` | notes at the generated unit's include lines (`int fabs;`) | `cli/builtin_name.hero` |
| 2052 | `602071cc4` | (kept `-Wimplicit-function-declaration` an error in a header's region), and a call's *did you mean* note read as the runtime's name | `emit/header_region.hero`, about 100 lines of reader code in `cli/clang_first.hero` and `cli/runtime_name.hero` (the critic's pass 1) |
| 2053 | `de6f0f3e1` | (kept `-Wmicrosoft-redeclare-static` an error; Windows only) | `emit/header_region.hero` |

(`git log --oneline 5d3b96606 -n 60 | grep -E "Defect (2035|...|2053)"`.) Routes:

- **A, shape by shape**: today's, each shape a reader of clang's words in `selfhost/cli/`.
- **B, the first error's own location decides**: an error located in the program's header, or a file
  it reaches that is neither the compiler's nor the C library's, is the header's whatever its notes
  say; one located in the compiler's own files is the compiler's, except where the sitting names
  why not (2049's emitter name had its error in the generated C, its note in the header).
- **C, the compiler's C names reserved in the language**: the specification says which identifiers
  a header must not declare (the runtime's `hero_*`, `Hero*`, `HERO_*`, the emitter's `h_*`), so a
  collision with one is the header's by rule (open issue 2046 names this silence).
- **D, isolation**: the groups' headers compiled in units that never hold the runtime's or the
  emitter's names (a shim per group), so the collisions cannot happen at all.
- **E, a discriminating probe**: compile the header with the C library's headers the unit reads
  (`<stdint.h>`, `<math.h>` and what else: the route must say) and without the runtime, then with
  the runtime; refused without it is the header's, refused only with it is a collision told as one,
  naming both sides. **Corrected after the critic's pass 1**: `cli/header_alone.hero` is not this
  probe: it compiles the header after `heroes_unit.h` and `hero_os.h`, both of which include
  `heroes_runtime.h`, and with no `-W` flag, so on `hs.h` it answers *does not compile alone*; and
  `i64.h` and `e_std.h` compile bare and are refused only once `<stdint.h>` or `<math.h>` is read.
- **F, the compiler's names in C's reserved space**: rename the runtime's and the emitter's C names
  into identifiers C11 7.1.3 reserves to the implementation, so a header that collides has used a
  reserved name.
- **G, clang's diagnostics read as structure**: locations and notes from a structured output
  (`--serialize-diagnostics`, SARIF) instead of its text (the critic's pass 1: on this Mac clang's
  SARIF is flagged unstable, leaves every note unlinked from its error and carries no include stack).
- **Named by the critic's pass 1, two of them run**: **H**, the runtime read after the groups'
  headers, as the tree already reads the C library's math after them, so every collision takes one
  shape (an error in a later file, its note at the header's line); **I**, the program's unit
  compiled without the group's header, a hand-written prototype in its place, which compiled `hs`,
  `e_std` and 2049's emitter-name case at exit 0: the one probe that separates a compiler bug from a
  header's mistake; **J**, the header's declared names scanned before any unit compiles; **K**, a
  message that says what was established without assigning blame.

**What the sitting must rule**: the rule (one of these, a combination, or another); whether the
specification states it and at what cost in tokens; how much of today's readers it replaces, and
with what witness that nothing told today changes for the worse; what a false blame costs either
way (a compiler bug told as the author's is a false message, and so is an author's mistake told as
the compiler's); Windows (defect 2053's words differ there); and what happens to the open
`adjacent` issues 2047 (absolute paths in notes) and 2054 (the `-Werror` note's list).

## The tree, frozen

- The sitting's tree: worktree `.claude/worktrees/lane-panel-214`, branch `lane-panel-214`, at
  `8c8783ac7` (`git log --oneline -1`): the trunk `5d3b96606` plus panel 213's ratification.
- **Your copy**: `git worktree add --detach .claude/worktrees/scratch-panel-214/<your seat> 8c8783ac7`
  from the repository's root, then inside it `clang -I runtime seed/heroes.c runtime/runtime.c -o
  heroes` (seconds; never another seat's compiler or the trunk's) and `mkdir -p build`. You read,
  build and run only there; you write your report into the sitting's tree at
  `.claude/worktrees/lane-panel-214/docs/panel/214-reports/<your seat>.md`, as you go and not only at
  the end. Rebuilding the compiler from `selfhost/` in your copy: `./heroes build selfhost/main.hero
  -o build/heroes-next`, about a minute; regenerating its seed is not owed for a prototype.
- **Another long run is on this machine**: the release v0.4.0's full net in
  `.claude/worktrees/lane-release`. Do not touch that worktree; time nothing (a timing beside it is
  discarded by the ratio, `.claude/rules/verification.md`).
- **No paid run** (no `claude -p`, no API call, no cloud run), **no credential printed** (`gh auth
  status` if you must know, never `gh auth token`), **no GitHub write**, **no push**, no `git add
  -A`, no `git commit -a`, no `git stash`, nothing written outside the repository's root. Never pipe
  a suite into `head` or `tail`: write it to a file under your `build/` and read it.

## The evidence, measured

- **The readers**: `wc -l` over `selfhost/cli/{clang_first,header_refused,standard_name,runtime_name,declared_after,builtin_name,group_file,header_order,header_alone,headers_together,runtime_key}.hero`
  and `selfhost/emit/header_region.hero`: 332, 650, 181, 418, 292, 213, 133, 334, 61, 125, 197 and
  211 lines. `clang_first.hero`'s module doc: *what clang's words say of a refused unit's first
  error: its row, its place and its message, the includes above it, the notes under it, the group
  whose header it lies in and another group a note names*.
- **The cases**: 55, the `.hero` files marked `#~ ffi_header_refused` (`grep -rl "#~ ffi_header_refused"
  tests/golden | grep -c "\.hero$"`: 54 under `unsupported/`, 1 under `fixedbugs/`); the 125 the
  first version of this brief gave counted their `.expected` files and the recovery corpus's copies
  too. Five `run` cases that must keep building (defect 2052's and 2044's) are cases too. By defect,
  `ls tests/golden/unsupported tests/golden/run | grep -c "fixedbugs-<n>-.*\.hero$"`: 360 six, 2036
  nine, 2044 six, 2045 five, 2049 eight, 2051 two, 2052 three, 2053 two; 2035 and 2040 have no golden
  case (their witnesses are tests inside `cli/header_refused.hero`, the golden forms running every
  case from the repository's root).
- **A unit's order**, `grep -n "#include" tests/emission/examples-sqlite-main.c`: `heroes_unit.h`,
  `hero_os.h` and `heroes_runtime.h` first (lines 2 to 5), the group's header between
  `heroes_guard_open.h` and `heroes_guard_close.h` (lines 22 to 24), `heroes_standard.h` after
  (line 48). So every header of the program's own is read after the runtime's declarations and
  before `<math.h>`.
- **The specification** (`spec/heroes-spec.md` § 13 FFI, read): it says a group names its header,
  that clang checks every result type, constant and record field against it, and says nothing of
  who is told when the header itself does not compile (`grep -n -i "internal error\|blame"` over the
  spec: nothing). Its budget: `heroes measure spec/heroes-spec.md`, 7655 on the vendored ranks and
  10,048 real against 10240 (`docs/ROADMAP.md`'s row, re-measured 2026-10-13 by the claim's lane);
  the real headroom is 132 once the FFI floor's 60 is held back, and a sentence's real cost needs a
  `--refresh`, a paid run no seat starts.
- **Through `#line`, clang places an error in the compiler's own emitted code inside the program's
  `.hero` file** (the critic's pass 1): a location is not a party.
- **The witnesses before and after the night**: six reproducers built with the compiler of
  `08230cbfa` (before defects 2044 to 2053) and with this tree's (`build/old/heroes` and `./heroes`
  in the sitting's tree, `build/samples/<name>-old.txt` and `-new.txt`): `hs` (2045), `i64` (2044),
  `e_fabs` (2051), `imp` (2052), `e_std` and `j_typo` (2049), each exit 2 before and exit 1 after.
  Their sources are in `.claude/worktrees/scratch-typed-inspection/rep/` (`hs`, `i64`),
  `rep/after2049/` (`e_fabs`, `imp`) and `rep/2045-shapes/` (`e_std`, `j_typo`).
- **The tracked programs with an `extern`**: `git grep -l -E '^[[:space:]]*extern ' 8c8783ac7 --
  '*.hero' ':!docs' | wc -l` is 878 (1,583 with `docs/`, 705 of them under `docs/panel/`; 313 under
  `tests/golden/run`, 281 `unsupported`, 142 `check`). The witness a rule change owes is that
  population BUILT with this tree's compiler and with the route's, exit code AND message compared
  (`.claude/rules/verification.md`'s census is of `heroes check`, which exits 0 on all six
  reproducers and so cannot see this question).
- **Other units hold a group's header too** (the critic's pass 1): `two_ways_ask`, `label_dump`,
  `defined_twice`, `reads` and `header_alone`, each with its own include order and no `-W` flags;
  and ten examples name the runtime's own `hero_os.h` as a group's header. A rule stated on the
  program's unit must say what it does in these.
- **The names C and F would reserve** are more than four prefixes: the runtime's ten integer
  typedefs, `HEROES_RUNTIME_H`, `main`, and the names of the C library and clang's builtins the
  unit reads (the critic's pass 1).
- **Windows**: the box is on tonight (`ssh win`, Git Bash, clang 23.1.1 for the MSVC target). Defect
  2053's cases are judged there alone because no `static` shape reads alike on the three platforms;
  `tests/harness/platform_skips.hero` names them.

## What every report holds

The structure your agent definition gives (`.claude/agents/<seat>.md`), plus: the route you would
adopt and why, the route you refuse and the program or fact that refuses it; every number with its
command; a **prediction** naming the instrument that scores it and when; and what you could not run.
