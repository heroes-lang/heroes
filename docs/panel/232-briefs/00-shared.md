# Panel 232, shared brief: what `1.0.0` will promise, said before it is promised

**Convened 2026-10-29** in M-compatibility-promise (row 74), on task 1066 (the
paragraph `1.0.0` rests on, the suite that makes a broken promise red, the
sentence CLAUDE.md § 10 forces), task 2245 (three sentences of the
specification the compiler does not keep), decision 2217's *A* (the order of
evaluation, the first item of the next sitting on the specification), and
what the milestone's steps 1 to 3 found
(`docs/roadmap/milestones/M-compatibility-promise.md`, its last section). By
the author's yes of that day, asked once for the milestone (CLAUDE.md § 4),
recorded here and in the milestone's journal at its close: **the full
panel**, and **the blind seat as short sessions within 12 USD**. Five seats
and a completeness critic, which read these briefs before any seat started
(`docs/panel/232-reports/critic-briefs.md`, 34 findings, cited below as F1 to
F34) and reads the reports after.

## A frame no seat argues past

**The author's rule of 2026-10-29** (CLAUDE.md § 2), meant as: *I do not
want to complicate the language for backward compatibility I do not need
now; an absolute rule*. Before `1.0.0` no form enters the language to keep
an older program compiling: no declared version, no mode, no old spelling
kept beside the new, no deprecated alias. A route that adds one is refused
before it is weighed, and a seat that finds such a route the only one says
so and stops there. And **the promise starts at `v1.0.0`** (the author's
ruling of the same day): no item asks what `0.x` owes an older `0.x`
program, which is nothing. **The rule binds before `1.0.0` alone** (F14):
what `1.x` owes a program is what this sitting's paragraph says.

Each item is one of three kinds: a sentence that says what the language
already does; a sentence the compiler does not keep, and one of the two
repaired; or a promise about `1.0.0` itself.

## The tree

The seats work at **`afe983265`**, steps 0 to 3 committed, whose compiler
says `sources 3e3c95dc246b73b3`. The lane `compat-promise` has since moved to
`89ebb0035`, which re-blesses two emissions on Linux arm64 and changes
nothing the compiler reads (F25). **Each seat that builds works in its own
detached worktree at `afe983265`**, `.claude/worktrees/232-<seat>/`, made by
the coordinator, and builds its own compiler there: `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes-stage1`, then `./heroes-stage1
build selfhost/main.hero -o heroes`, then `./heroes build selfhost/main.hero
-o heroes-next`; `heroes-next --version` says the tree's key (a second
generation, because the library moved at step 1 and a compiler's key covers
the library compiled into it; the critic measured the first at `sources
00577d2a5957da82`, F3). It reads nothing of another seat's worktree, writes
nothing outside the repository's root (CLAUDE.md § Hard stops; its scratch
is its worktree or `.claude/worktrees/232-<seat>-scratch/`), and writes its
report as it goes, `docs/panel/232-reports/<seat>.md`, in the lane. **No
seat prints a credential** (`gh auth status`, never `gh auth token`),
**none starts a paid run** (the blind sessions are the coordinator's;
`heroes measure --refresh` is the token-counting endpoint the ledger's
landings have called since panel 123, named in 34 of its 121 rows, F30, and
the spec-warden is named for it), and **none starts a clock**.

## The items, each with what was measured before this brief

1. **The order of evaluation.** The specification fixes some orders a
   program observes: `&&` and `||` short-circuit, so their left side comes
   first (§ 7); `fold` is left and `find` gives the first it accepts (§ 11)
   (F1). Of an expression's parts it says nothing (`grep -n -i` for
   `order`, `evaluat`, `as written`, `sequenc`, `side.effect`, F1). The
   compiler goes left to right in every position tried: an operator's
   operands, a call's arguments, a record's and a variant case's fields, an
   array's and a map literal's elements, an `f` literal's holes, a call
   through a function value, the UFCS receiver, an object before its index,
   `?` in the middle of a sum (the coordinator's
   `docs/panel/232-briefs/probe/places.hero`; the critic's F5). **Two
   orders a sentence must not hide**: a WRITE evaluates its place's indices,
   then the value, and checks the place as it writes, so
   `ys[noisy(5)] @ noisy(1)` prints the value's effect and then aborts on
   the index, while a READ aborts on the index at once; an `@` argument's
   place is checked after the arguments after it (F4, F26, F31). And **a
   built-in that calls a function value**: `map` calls in order, `any`,
   `all` and `find` stop at the deciding answer, `filter` calls on every
   element; § 11 says only `fold`'s direction and `find`'s result (F5).
   **The labels**: a call, a record or a variant case whose labels stand in
   another order than declared is refused `wrong_label`
   (`docs/panel/232-briefs/probe/order.hero`; F5), so *as written* is the
   declared order; § 9 already says a call through a value names each label
   in its own position, and says nothing of a direct call's (F2).
2. **`<<` and `>>`** (task 2245, 1). `print(3 << 62)` prints
   `-4611686018427387904` and `print(1 << 63)` `-9223372036854775808`, exit 0
   (`docs/panel/232-briefs/probe/shift.hero`); `>>` of a negative copies the
   sign, `-8 >> 1` is `-4` (F6). **design.md § 4.14 rules both, with a
   reason** (F8): *`1 << 63` is `INT64_MIN`, not an abort. The shift is done
   on the unsigned bit pattern ... defined for every input ... Without the
   sign bit reachable, a mask set cannot name its own top flag, which is the
   whole use*; and *`>>` is arithmetic*. § 7 says *(i64 only; shift count
   0..63 or it aborts)* and *Overflow aborts at every width*, and neither
   rule. So the sentence that states design.md's rule is one route; an abort
   on a shift that changes the sign is refused by § 4.14 by name; **an abort
   on a shift that loses a set bit**, which keeps both values above, is a
   route nobody listed (F8, F28), and any abort route amends § 4.14 and must
   answer its reason.
3. **`==` on a handle given back** (task 2245, 2). § 13: *`==` on it
   aborts, and so does a copy made before, unless C has since handed its
   address out again or a million other lives have ended*. `==` on the
   handle ITSELF after it is given back already aborts: the emitted C empties
   the binding to the dead page and `hero_handle_eq`
   (`runtime/parts/alloc.c`) aborts on it; a COPY made before compares by the
   address, and the golden
   `run/fixedbugs-a-freed-object-read-through-a-stale-copy-is-not-a-null-handle`
   pins `null: false` (F9). The runtime already remembers the given-back
   addresses (`hero_dead_mark_locked`, `hero_dead_holds_locked`, a window of
   `1 << 20`, `HERO_DEAD_WINDOW`, the clause's *million*), consulted at a
   crossing into C and not at `==` (F9). design.md Part 8 wart 20, written in
   the same commit as the clause (`c8b1bc878`), reads it otherwise (F13);
   CLAUDE.md § 12: the specification beats the compiler.
4. **A stack exhausted without recursion** (task 2245, 3). § 9 says
   *recursion too deep aborts*; a program of 2,000 functions, none
   recursive, each returning a record holding the one before by value,
   aborts `stack exhausted` (d435ca009), its frames growing with the depth
   (about 16 MB); a chain of small frames, one call whose frame alone passes
   the stack, and a thread's smaller stack (`runtime/parts/stack.c`'s
   `thread_deep`) are the shapes beside it (F10). The issue's draft: *a call
   chain deeper than the stack aborts*.
5. **§ 11's `exit`** (step 1, task 2250, panel 230's fallback): *`exit(code:
   i64)` (ends the program; a code outside 0 to 255 aborts)*, +15 real and
   landed; a literal outside the range is refused at compile time, which the
   sentence does not say. The edges (F7): `-1` is refused, though the grammar
   makes it a minus on a literal; a constant, a `=` name and `255 + 1` abort
   at run time though § 4 calls their values known; the abort's report names
   a `.must()` the program never wrote; `exit` reached through a function
   value aborts at `<heroes library>:232:1` (F25).
6. **The paragraph `1.0.0` rests on** (task 1066): the coordinator's draft,
   repaired after F14, F20, F22 and F27, is
   `docs/panel/232-briefs/paragraph-draft.md`, each sentence with what it
   rests on. Its words, its home, whether it promises one way (a program
   cannot say which version it was written for while CLAUDE.md § 10 admits
   no per-project file, a refusal panel 056 made conditional and not
   permanent, F14), what it excludes (a defect, what the specification
   leaves open, a specification error), and the deployment model.
7. **Whether `1.0.0` promises a diagnostic's code, and not its words.**
   Measurement 060, at `0122c9d1c`, before step 1: of the 242 codes a golden
   pins, none moved its pinned output in the week before; step 1 then
   changed `int_out_of_range`'s pinned words and removed a pinned
   `type_mismatch` line (F11). The 242 are the codes a golden pins: at least
   `not_indexable` and `if_without_else` are reachable and pinned by no test
   (F16). `heroes check --json`'s `"schema"` number (design.md § 4.17) is a
   route nobody listed (F16).
8. **The compatibility suite's corpus** (step 3,
   `tests/harness/suite_compat.hero`): `examples/` at each release tag, 69
   programs at `v0.5.0`, 67 of them with an expected output (F23). Others:
   the golden `run/`, 544 programs at `v0.5.0`, 248 of 622 changed in the 30
   days before (F33); the specification's code blocks, of which the sqlite
   example alone is compiled; the thesis suite's reference programs;
   `selfhost/` at each tag (F23).

## What each seat is for

- **compiler-engineer**: items 1, 2, 4, 5, 7 and 8 in the compiler, with
  files and line counts; what each route costs, built in its own worktree.
- **ffi-pragmatist**: item 3 by compiling and running the C a binding
  needs, and item 6 at the C boundary: `hero_os.h` (57 declarations, decision
  2228) and `heroes_runtime.h` (task 2237's header, which the compiler itself
  binds, F15).
- **spec-warden**: every draft sentence priced on `heroes measure
  spec/heroes-spec.md --refresh` in its own worktree, real and vendored;
  Principle 0's burden for each; the paragraph's home and its cost there.
- **historian**: the precedents, verified by web search, `historian.md`.
- **llm-ergonomist**, the blind seat: short sessions outside every project
  context, `blind/README.md`.
