# Panel 208, shared brief: a program's own call of a deprecated C function

Convened 2026-10-10 at 15:08 by the coordinator, for defect 584
(`issues/2026-10/10/2026-10-10-1356-defect-584-a-program-s-own-call-of-a-deprecated-c-function-prints.md`
on `main`), under the author's deadline of 18:00 for the next step. **Lean
sitting by the author's choice at 15:07** (the question widget: *lean, free*):
the compiler-engineer, the ffi-pragmatist and the historian, one completeness
critic pass after the reports, no blind seat and no paid run. What that gives
up: a measurement of what a reader of the spec expects; the synthesis says so.

## The tree

Frozen at **`391628b6`**, branch `lane-b18-guard` (batch 18's lane that landed
defect 571), read clean by `git status --short` (0 lines) at 15:07. Each seat
has its own copy made by `git archive 391628b6`, under
`.claude/worktrees/scratch-b15/208-<seat>/tree/`; build your own compiler there,
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed && ./heroes-seed build selfhost/main.hero -o heroes`
(the seed predates batch 18's `selfhost/` changes, so the second step is
needed; 31 to 61 s measured 2026-09-24). Never read, build or run in another
seat's directory, the trunk's checkout or a lane. No file outside
`/Users/joseph/Temp/heroes/heroes-lang`; set `TMPDIR` inside your directory.

## What is measured (the coordinator, 15:00 to 15:07, each by the command named)

- **The reproducer**, `.claude/worktrees/scratch-b15/208-repro/` (copy it into
  your own directory): `dep.h` is a header of the program's own,
  `__attribute__((deprecated("use twice2"))) static inline int64_t twice(int64_t x)`,
  plus a deprecated function `version`, a deprecated enumerator `OLD_LIMIT`, a
  deprecated `struct old_pair` and a function under GCC's
  `__attribute__((warning("careful")))`; `dep_call.hero` binds `twice` and
  prints `twice(x: 21)`. With lane b18-guard's compiler, `heroes build
  dep_call.hero` exits 0 and prints clang's raw text,
  `dep_call.hero:5:10: warning: 'twice' is deprecated: use twice2 [-Wdeprecated-declarations]`,
  an excerpt of the emitted C (`t2 = twice(t1);`) under the `.hero` line, a
  note naming `./dep.h:2:16`, and *1 warning generated.*; the binary prints
  `42`.
- **A system header's deprecated function did not warn**, unexplained: a
  program binding `sprintf` from `stdio.h` (deprecated in this Mac's SDK,
  `__deprecated_msg(... use snprintf(3) instead.)`, `_stdio.h:278`) and calling
  it, `heroes build` and `heroes run` with the same compiler exit 0 with no
  warning (`scratch-b15/r584/call.hero`). The trunk's compiler at `9743597b`,
  before defect 571, printed the warning at the binding's probe line even with
  no call (`r584/dep.hero`). Why the call is silent is a question, not a
  premise: a system header's deprecation may be quiet by clang's own
  system-header rule, or by the guard's region, or by something else.
- **Defect 571** (`aeed52ec`): the warning is ignored from the groups' close,
  spoken again over the program's own definitions, quiet over the compiler's
  own lines; *the program's own call or signature naming a deprecated name is
  still told by clang at its line, the verdict unchanged.* That sentence is
  this sitting's question.
- **design.md `:3744`**: *this language has no warning level: a diagnostic is
  exit 1 or nothing, so "just warn" is not available*. **No sentence of
  `spec/heroes-spec.md` or `docs/design.md` names `deprecated`**
  (`grep -n -i deprecat` over both: no line). A silence is often a ruling:
  grep `docs/panel/` and `issues/` for it before reading it as a gap.
- **The class list** (`.claude/rules/verification.md` § Bounded discovery):
  `blocking` covers *a clang warning on a correct program* and *a correct
  program refused* alike, so neither silence nor a refusal is free by the
  class list alone; which one the program IS (correct or not) is the question.
- **Scale**: 31 of this Mac's SDK top-level headers carry the word
  `deprecated`, 2,495 lines (`grep -c deprecated $(xcrun --show-sdk-path)/usr/include/*.h`,
  a count of lines, not of declarations); 21 files under `examples/` declare
  an `extern` (`grep -rl --include='*.hero' '^extern' examples/`).

## The routes the coordinator listed, and how that list was made

From design.md's no-warning sentence, the class list and the reproducer; no
search of other trees was made, so the list is a lower bound. **Ask what would
have to be true for a route nobody listed to exist.**

- **(R) refuse**: a call (or signature, constant read, record use) naming a
  name its header marks deprecated is exit 1, a new class (say
  `ffi_deprecated`) on the `.hero` line, the header's own message in a note.
  It refuses a program the C compiler builds, and a function deprecated on one
  platform's SDK only refuses there.
- **(S) silence**: the program's own lines quiet too, as 571 made the
  compiler's; the program builds at exit 0 and the header's advice is lost.
- **(M) refuse with a way out**: (R), and the binding can say it knows, a word
  on the `extern` declaration; a new surface form and spec tokens.
- **(F) a flag**, `--allow-deprecated` (Zig's `-fallow-deprecated` is the
  shape): `.claude/rules/cli-surface.md`'s stopping rule decides whether a flag
  may change a verdict.
- **(N) a note at exit 0** in the compiler's own words: a warning level by
  another name, against design.md `:3744`.

The shapes beside the reproducer, depth one: a deprecated function called, one
bound and not called (571's), a deprecated record in a signature, a deprecated
enumerator read, `__attribute__((warning(...)))` and `unavailable`, Apple's
`availability(macos, deprecated=...)`, `#pragma clang deprecated(NAME)` on a
macro, C23's `[[deprecated]]`, a deprecated function reached through a `link`
library's header (OpenSSL 3 deprecates many).

## Rules every seat follows

- Every number you write is produced by a command you ran, named beside it.
  A negative sentence (*X cannot*, *nobody does*) goes out as a question
  naming what you searched.
- Write your report as you go to `docs/panel/208-reports/<seat>.md` (the
  coordinator's checkout, that file only), so a stopped seat leaves what it
  had.
- No paid run of any kind. No `pkill` by pattern. A program that may not end
  is built and its binary run under `timeout`, output bounded.
- Long commands under `caffeinate -i`. Times from `date`. English. No em
  dashes.
- Verdict: **approve**, **object** or **veto** (veto only on soundness, your
  seat's ground) for each route, and the one you recommend, with what would
  make it wrong.
