# Panel 216, the shared brief: a header constant whose value C leaves undefined, which clang folds in silence

Written 2026-10-16 from 21:45 by the coordinator, on the trunk frozen at
`6e2690b5058b663a768f06db91abe73cd804793a` (`git log -1`; `git status --short` printed nothing).
Convened on defect 2075 (https://github.com/heroes-lang/heroes/issues/2075), class `systemic`,
under the author's goal given as they left the session that evening: close the open milestones
and the blocking defects. **Lane: soundness** (compiler-engineer and ffi-pragmatist, then a
completeness critic), because every route below extends a check spec § 13 already promises
(*clang checks every result type, constant and record field against that header*) and tells its
refusal with the class that already tells such a value, `ffi_constant_type`
(`selfhost/emit/ffi_constant_value.hero:90`): no surface, no new diagnostic class, no spec token.
A seat that finds a route needing any of the three says so, and the sitting is then re-convened full.

## The question

A group's integer `constant` whose header value C leaves undefined, and which clang folds with no
warning at all, builds at exit 0 and prints whatever the machine held. Two shapes are known:
`INT_MIN / -1`, and a floating value converted to an integer out of its range, `(int)(1e20)`. What
does the compiler do with them, given that the one C11 check the coordinator found that sees them
also refuses values C defines?

## What was measured while this brief was written (every number is a command's)

Header `k.h`: `#define K_DIV (INT_MIN / -1)`, `#define K_FLT ((int)(1e20))`, `#define K_COMMA (1, 2)`,
`#define K_FDIV ((int)(1000.0 / 60))`; one program per name, `extern "k.h"` with `constant K: i32`
and `print(K)`. The compiler is the trunk's, built from its seed. The files are in the
coordinator's scratchpad, `p2075/`.

| | this Mac, Apple clang 21.0.0, `heroes run` (which compiles at `-O2`, see below), three runs | this Mac, `heroes build` (`-O0`), three runs | Linux x86-64 (`heroes-linux`, Debian clang 22.1.8), the emitted C at `-O0`, three runs | the same at `-O2` |
|---|---|---|---|---|
| `K_DIV` | `0 0 0` | `1835574252 1794335708 1840260076` | `-230216 -230328 -230264` | `0 0 0` |
| `K_FLT` | `0 0 0` | `1839260636 1876419548 1839457244` | `-230504 -230168 -230104` | `0 0 0` |
| `K_COMMA` | `2 2 2` | `2 2 2` | — | — |
| `K_FDIV` | `16 16 16` | `16 16 16` | — | — |

So at the `-O0` `heroes build` compiles at, both platforms print a different value on every run,
exit 0, no message: the defect as filed; at `-O2` both print a stable `0`, the same undefined value.

**Corrected 22:05, after the critic's first pass** (`docs/panel/216-reports/critic-briefs.md`): this
table's first column was headed `heroes run (-O0)` and its second `the same at -O2`, both `0 0 0`;
`heroes run` compiles at `-O2` (`selfhost/cli/verbs.hero:62`, and its own help says so), so the
Mac's `-O0` was never measured, and it varies. The second column is re-measured with `heroes build`.

Three questions asked of clang over the same four names (`clang -fsyntax-only`, each on Apple
clang 21.0.0 and on Debian clang 22.1.8 in `heroes-linux-arm64`):

1. **C's integer constant expression rule**, `_Static_assert((K) == (K), "")` under
   `-std=gnu11 -Werror=gnu-folding-constant`: **all four refused**, each *expression is not an
   integer constant expression; folding it to a constant is a GNU extension*, on both. This is the
   check `selfhost/emit/value_check.hero`'s head names (*it also refuses values C defines*).
2. **C23's `constexpr`**, `constexpr long long v = (K);` under `-std=c23`: **`K_DIV` and `K_FLT`
   refused, `K_COMMA` and `K_FDIV` accepted**, on both, each refusal *constexpr variable 'v_div'
   must be initialized by a constant expression*.
3. **C++'s `constexpr`**, the same line under `-x c++ -std=c++17`: the same split on both, with a
   note naming the value, *value 2147483648 is outside the range of representable values of type
   'int'* and *value 1.0E+20 is outside the range …*.

**Unrun, and questions rather than premises**: whether clang 18 accepts C23's `constexpr` (the
compiler's floor is 18, `selfhost/cli/clang_floor.hero:36-37`, and the CI's Linux legs install
the distribution's clang, `.github/workflows/ci.yml:332`); whether the headers of the 932 `.hero`
files with an `extern` line (`git grep -l -E '^[[:space:]]*extern ' 6e2690b50 -- '*.hero' ':!docs'`)
compile under `-std=gnu23` or as C++; what either
check costs a build.

## The routes the coordinator listed, and where the list came from

From `selfhost/emit/value_check.hero`'s head, defect 2075's text and the three measurements above;
nobody has searched further, and a route a seat finds that is not here is the most valuable
thing it can bring.

- **A, nothing more.** The value stays undefined in every build. `--sanitize` does **not** trap
  either: it prints a `runtime error:` line and the program runs on, exit 0 (the critic, measured;
  `value_check.hero:41` says *traps*, which is false as worded).
- **B, C's rule as a refusal.** Refuses `(1, 2)` and `((int)(1000.0 / 60))`, values C defines:
  a correct program refused, which `.claude/rules/verification.md` § Bounded discovery classes
  `blocking`.
- **C, C23's `constexpr` as the question**, asked where the value check already stands or in a
  unit of its own, the refusal read by position as `layout_screen.hero` reads its own.
- **D, C++'s `constexpr` as the question**, the same, the header read as C++.
- **E, the value computed under a trap at run time**, in the accessor: unbuilt and unexamined.

## What the critic's first pass changed (22:05), every line its report's, with the command there

- **Answered**: clang 18.1.3, the compiler's floor and the clang of the CI's two Linux legs, has no
  C23 `constexpr` (*unknown type name 'constexpr'* on all four), so route C refuses everything
  there. And `constexpr long long` refuses correct unsigned values (`ULLONG_MAX`, `SIZE_MAX`,
  `~0ULL`): a route asks in the constant's own type (`typeof`).
- **Routes F to J, not listed above**: **F**, the undefined value read at build time as a
  sanitizer trap: a probe function per constant compiled at `-O1` with UBSan in trap mode leaves
  `llvm.ubsantrap` for all 23 undefined shapes the critic wrote and for none of the 21 defined
  ones, the split identical on clang 18.1.3, 21.0.0 and 22.1.8 (at `-O0` it is not readable);
  **G**, flags that define the behaviour (`-fno-strict-float-cast-overflow` saturates the float
  half; `-fwrapv` does not cover `INT_MIN / -1`); **H**, the ICE rule as a filter and a second
  question only for what it refuses; **I**, clang's folded value as the accessor's literal (stable,
  refuses nothing, accepts the undefined value: the conservative route to record); **J**, the
  compiler's own evaluator (against design.md §1.11, named for completeness).
- **The class is wider than two shapes**: fifteen more undefined neighbours build silent
  (`INT_MIN % -1`, NaN and infinity cast, an undefined value under `* 0`, a ternary or a
  comparison), and the same defect reaches an `f64` constant and a record constant. A route that
  covers two shapes and not these is a partial answer, and says so.
- **Found beside it, filed**: a constant spelt `(__extension__ ({ 3; }))` stops `heroes build` at
  exit 2, *internal error*, clang refusing a statement expression at file scope in the constant's
  return-type assertion: **defect 2101**, `blocking`
  (https://github.com/heroes-lang/heroes/issues/2101), reproduced by the coordinator. The sitting's
  resolution says how that constant is told too.
- **Questions to answer**: spec § 4 line 115 already makes an aborting operator in a *written*
  constant a compile error, *run or not*, which a header constant's route can be held to; a
  run-time abort (route E) changes what a program does and may need a spec sentence, which would
  re-convene this sitting full; raising the clang floor is a change to the tool surface, a panel
  trigger of its own; a unit of questions needs `-ferror-limit=0` (clang stops at twenty errors),
  as the trunk's own ask units pass it (`selfhost/cli/header_ask.hero:87` and its siblings
  `switch_ask`, `unswitch_ask`, `two_ways_ask`), the nearest models for a route that asks; and the
  population is about 132 integer and `bool` header constants in 65 files (the critic's `awk`, an
  approximation the seats' corpus run replaces).

## What each seat is asked

Rule on the question and say which route, built far enough that its numbers are real: which
programs it refuses and which it keeps, over the four shapes and over the corpus; what it costs a
cold build; what it does on clang 18 and on the Windows box's clang 23 (the box may be off: say
*unrun* then); and the message it tells, if any. `CLAUDE.md` § Precedence ranks robustness above
everything but the hard stops and the rule of running: a value that changes from run to run at
exit 0 is a robustness question, and a correct program refused is one too.

## The rules every seat keeps

- **Build in your own copy**, `<scratchpad>/216-<seat>/`, made with
  `git -C /Users/joseph/Temp/heroes/heroes-lang worktree add --detach <path> 6e2690b50` or `git
  archive`; then `rm -rf build`, then build your compiler there from the seed, `clang -I runtime
  seed/heroes.c runtime/runtime.c -o heroes` (about 8 s). Never use the trunk's `./heroes`, never
  write in the trunk's tree or in another seat's copy. Remove a worktree you added when you finish.
- **Write your report as you go**, to `docs/panel/216-reports/<seat>.md` in the trunk's tree,
  the one file you write there. Every number names the command that produced it; what you could
  not run says *unrun*.
- **No paid run** (no `claude -p`, no API call, no cloud run): name one in your report with its
  size if you think it is worth it, and the coordinator decides.
- **No credential is printed**: `gh auth status`, never `gh auth token`.
- **A program that may not end** is built and its binary run under `timeout`, output bounded
  (`.claude/rules/verification.md` § A run that may not end).
- **Nothing outside the repository's root** but your scratchpad copy; the Linux containers
  (`heroes-linux` is x86-64 under Rosetta, `heroes-linux-arm64` native) and their own `/tmp` are
  yours to use.
