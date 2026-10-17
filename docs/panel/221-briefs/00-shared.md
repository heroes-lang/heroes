# Panel 221, the shared brief: a record constant whose header value is no constant

Written 2026-10-17 from 12:00 by the coordinator, on the trunk at `b2c81211d` and lane
`lane-land215` (committed head `bc978b09d`; panel 218's C2 uncommitted in its working tree while
its gate runs). Convened on defect 2116 (https://github.com/heroes-lang/heroes/issues/2116,
`blocking`), which reaches a hole panel 063 wrote down as a known cost. Read first: panel 063's
file `docs/panel/063-the-direction-nobody-checked.md` (its condition 1), panel 218's file
(ratified 2026-10-17) and `selfhost/emit/assert_spelling.hero`'s `asks_for_a_value` and
`selfhost/emit/record_constant.hero`'s head in the lane. **Seats**: compiler-engineer and
ffi-pragmatist, then the coordinator's synthesis.

## The question

A group record constant whose header value reads something that is not constant builds at exit 0,
and two reads of it differ, where the same read as a scalar constant is refused
`ffi_not_constant`. Panel 063 left the question unasked of a struct, measured: `__builtin_constant_p`
answers 0 for every struct, so asking it would refuse raylib's 26 `CLITERAL(Color)` macros. Does
the compiler now refuse such a record, and how, without refusing a record C builds from constants?

## What was measured (the coordinator's commands)

- **The defect**, lane and trunk compilers, this Mac: `rc.h` with `static inline int32_t
  g_next(void) { static int32_t n; return ++n; }` and `#define R_CNT { g_next(), 2 }`; the program
  binding `R_CNT` as a two-field record reads it twice and prints `1` then `2`, exit 0;
  `#define M_CNT (g_next())` bound as `i32` exits 1, `ffi_not_constant`.
- **What exists now that panel 063 did not have**: panel 218's C2 reads every leaf of a record
  constant's brace list from clang's JSON dump of the probe's text, with its node kind
  (`IntegerLiteral`, `CallExpr`, `DeclRefExpr` to an `EnumConstantDecl` or a `VarDecl`, ...),
  in `selfhost/cli/value_init.hero` (the walk) over `selfhost/cli/clang_tree.hero` (the reader).
  A route that refuses a leaf by its kind (a call, a reference to an object the program may
  write) reads structure, never clang's words, and never asks `__builtin_constant_p` of a struct.
- **Unrun, and questions rather than premises**: how many record constants the real headers bind
  whose leaves are calls or non-const objects (raylib's, SDL3's, Darwin's `*_INITIALIZER`s, glibc's,
  the Windows SDK's); whether a call leaf can be constant (a `static inline` returning a literal,
  `__builtin_*` builtins); what a `static const` object reached through a brace list reads
  (`{ k_obj, 2 }`, `k_obj` a `static const int`, which C11 does not count as a constant
  expression and clang folds); what C's own rule for a static initialiser refuses
  (`static const struct pt x = R;` as a probe line), measured beside the dump route.

## What each seat is asked

Rule on the question, route by route, built far enough that its numbers are real: which record
constants it refuses and which it keeps, over the shapes above, the corpus and the real headers;
what it costs; the message it tells. CLAUDE.md § Precedence, ranks 3 and 4 read together (the
author, 2026-10-16): robustness decides THAT a measured class of wrong programs closes, Principle 0
HOW. A route that needs surface, a new diagnostic class or a spec token says so.

## Rules

- **Build in your own copy**: a detached worktree under
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/221-<seat>` from `bc978b09d`, the
  lane's uncommitted C2 files copied in by `cp` from
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-land215/` where you need them
  (`git -C` that lane `status --short` names them; read only there, its gate owns it); `rm -rf
  build`, your compiler from the seed, then from its sources. Remove the worktree when you finish.
- **Write your report as you go** to `docs/panel/221-reports/<seat>.md` in the trunk, the one
  file you write there; every number names its command; what you could not run says *unrun*.
- No paid run, no credential printed, a program that may not end bounded in time and bytes,
  nothing outside the repository's root but your scratchpad and the Linux containers (the Windows
  box answers `ssh win`; write only under a folder of your own there and remove it).
