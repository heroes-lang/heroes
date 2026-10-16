# Panel 217, the shared brief: a read-only lend, a parameter whose place C may read and the function promises not to change

Written 2026-10-16 from 22:00 by the coordinator. Convened on issue 2098
(https://github.com/heroes-lang/heroes/issues/2098), task of M-buildable-structs, the sitting panel
215's question 2 recommended and the author ratified (*3a*, issue 2070); the author's answer as they
left the session that evening convened it now, inside this milestone, with up to 10 USD of paid runs
shared with the evening's other sittings. **Lane: full**: the answer may be a new form of the
language, a sentence of the specification and a diagnostic.

**The tree the seats copy is lane `lane-land215` at `f57af53dae65fedb66d8915311738b8bb05dd38e`**
(M-buildable-structs step 7, panel 215's R2 landed: a copy of a group record whose C struct holds a
lock is refused at `build` as `ffi_lock_copied`), not the trunk: half of this sitting's question is
what a reader does once that refusal tells them. Copy it with `git -C
/Users/joseph/Temp/heroes/heroes-lang worktree add --detach <your folder> f57af53da`. **The
specification a reader reads is `m-g-at`**, panel 215's R3, ratified and landing beside R2
(`docs/panel/215-reports/llm-ergonomist-work-2/heroes-spec-m-g-at.md`, sha256 `ac4ff3e2…`), whose
§ 9 says *the callee works on the caller's place itself*; the copy at `f57af53da` still has the old
§ 9 sentence.

## The question

Heroes has two kinds of parameter (`Param = [ "@" ] ident ":" Type`, `m-g-at` line 124): a place
the function may change, `@c`, written `@` at the call too, and a value. C's `const struct counter *`
reads a place and changes nothing, and there is no way to write a Heroes function that takes such a
record's place without declaring that it changes it. `lent` belongs to a C function's parameter
alone (`m-g-at` line 462), where it means *C does not keep it* (line 415), not *nothing changes it*.
**Should the language have a read-only lend, and in which form?**

## What was measured while this brief was written (every number is a command's)

- **Panel 215's paid re-run, task C** (`docs/panel/215-reports/llm-ergonomist-rerun.md`, the
  sitting's record; its numbers are that file's): ten blind readers of `m-g-at` all printed the
  right answer, and all ten wrote the read-only helper `doubled(c: Counter)` by value and copied the
  counter, lock and all, to lend it to C. That report read seven of ten notes saying `@c` would claim
  a change the task forbids (r01, r03, r04, r05, r06, r07, r09); a `grep` tonight for `@c` beside
  *change*, *modify* or *mutate* in the ten `notes.md` finds four (r03, r05, r06, r08), so the
  count is a reading, not a measurement.
- **The same ten programs, built tonight** (`docs/panel/215-reports/llm-ergonomist-work-2/r*/c.hero.txt`,
  each beside the kit's `counter.h`, in the coordinator's scratchpad `readers-r2/`): with the
  trunk's compiler, **10 of 10 build at exit 0 and print `6`, `4`, `5`**; with the lane's compiler
  at R2, **10 of 10 are refused**, each with 4 to 7 `ffi_lock_copied` (r02 7, r05 5, the rest 4).
  r01's four, read: its parameter `c` *takes by value a `Counter` … a copy C forbids*, with the
  `guess` fix *take it by place, and lend it at each call with `@`*; the copy into its local cell;
  and each of the two calls `doubled(stats.first)`, `doubled(stats.second)`. The note every one
  carries: *lend the place instead: a function taking `@c` receives the caller's own cell, `f(@c)`*.
- **The kit's key** (`docs/panel/215-reports/spec-warden-work/q3/kit/key/c.hero`) writes the
  helper as `function doubled(@c: Counter) -> i64`, `return counter_value(@c) * 2`, and declares
  every C function of `counter.h` with `@c: Counter lent`, `counter_value`'s header taking `const
  struct counter *`.

## Panel 215's prediction this sitting scores

The llm-ergonomist's (`docs/panel/215-…-reads.md` § Predictions to score): *with R2 and its message
shown, at least 8 of 10 readers correct on task C's second try; with a read-only lend, at most 2 of
10 copy the lock first try.* The first half is scored by giving each reader its own first program and
R2's messages on it; the second by a spec diff. **Both are paid runs**: the llm-ergonomist's brief
names their size and bound.

## The forms the coordinator listed, and where the list came from

From panel 215's question 2, the readers' notes and the specification's grammar; nobody has
searched further, and a form a seat finds that is not here is the most valuable thing it brings.

- **F0, nothing**: R2's note says `@c`; a reader who takes it writes a signature that says the
  function changes the counter.
- **F1, `lent` on a parameter of a Heroes function too**: `function doubled(c: Counter lent)`, the
  caller's place, read and never written, the call written `doubled(s.first)` with no mark since
  nothing changes; and on a C function's parameter with no `@`, `counter_value(c: Counter lent)`,
  C receiving the place and reading it, which a `const T *` header gives. One word the language has,
  given a second position.
- **F2, no word**: a by-value parameter the function never writes passed by its place in the
  emitted C. It reaches a Heroes helper and not C: lending to `const struct counter *` still needs
  `@`, so task C's copy stays (a reading of the grammar, unrun).
- **F3, `@` admitted on a parameter the function only reads**, panel 210's refusal (`never_rebound`)
  relaxed: the signature still says it changes. (Written until 22:20 as *an `@` parameter only
  lent onward*, which builds today: an `@` argument counts as a write, `written_nowhere.hero`
  line 7, the critic's first pass.)

## What the critic's first pass changed (22:20), each line its report's (`docs/panel/217-reports/critic-briefs.md`)

- **Forms not listed above**: a new word rather than a second meaning for `lent`; F1 with a mark
  at the call; passing by place decided by the type, for a record that cannot be copied; a lend
  method at the call to C, as `.cstr()` and `.ptr()` are; and R2's message changed alone.
- **Questions every form answers**: **aliasing**, since today `f(a: @p, b: p)` reads `p` as a
  copy and prints 1 where a place read through `b` would print 2, so a read-only lend beside an
  `@` argument reaching the same place is refused as two `@` arguments are, or the form says why
  not; whether `s.first.doubled()` dispatches to a read-only first parameter; which types may be
  lent read-only and whether copying out of one is allowed; whether C may return a pointer into
  what it was given as `const`; what R2's message says once the form exists.
- **The notes**, read again: nine of ten (all but r09) say `@c` would claim a change; panel 215's
  report read seven and a `grep` finds four, so the count is a reading.
- **The ffi-pragmatist's `grep`** lists 939 files at `f57af53da`, 900 under `tests/golden/`, 509
  of them programs meant to be refused, and one in `archive/`, which nothing builds: a census of
  bindings reads the programs that build.
- **The spec at `f57af53da` differs from `m-g-at` in § 13 twice as well as § 9**: a seat that
  measures or drafts copies `m-g-at` over `spec/heroes-spec.md` in its own copy first, and starts
  from 10,108 real (the coordinator's `--refresh` in the lane at 21:30; the step that lands R3
  commits it), against the ceiling of 16,384 less the 60-token FFI floor.
- **The arms**: the kit's `judge.sh` scores the key's own F0 answer `doubled(@c)` correct, so every
  program is also tabulated by a person for its helper's signature (a value, `@c`, the form) and
  whether it copies a lock, and the predictions are scored on that table; panel 215's readers ran
  inside the repository (`.claude/worktrees/readers2/`) on `claude-opus-5-5` with no `--model`
  pinned, and these run outside it, in the coordinator's scratchpad, with `--model claude-opus-5-5`
  pinned; arm R2's readers are fresh sessions handed a program, not the same readers, and both arms
  drop tasks A2 and U, which 215's readers did first. **The compiler-engineer builds F1's key, a
  `c.hero` written with the form, and it must build and print `6`, `4`, `5` under the prototype
  before arm F1 runs.**

## The rules every seat keeps

- **Build in your own copy**, `<scratchpad>/217-<seat>/`, from `f57af53da` as above, then `rm -rf
  build`, then your compiler from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o
  heroes`, about 8 s), and **since the seed at `f57af53da` predates R2** (the seed is regenerated
  only at a batch's close), build the compiler that knows R2 from it in two stages: `./heroes build
  selfhost/main.hero -o heroes-r2` (about 75 s). Never the trunk's `./heroes`, never another seat's
  copy, never a write in the trunk's or the lane's tree. Remove a worktree you added when you finish.
- **Write your report as you go**, to `docs/panel/217-reports/<seat>.md` in the trunk's tree
  (`/Users/joseph/Temp/heroes/heroes-lang`), the one file you write there. Every number names its
  command; what you could not run says *unrun*.
- **No paid run your brief does not name**; name one in your report with its size, and the
  coordinator decides. **No credential printed** (`gh auth status`, never `gh auth token`).
- **A program that may not end** is built and its binary run under `timeout`, output bounded.
