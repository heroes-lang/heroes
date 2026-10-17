# Panel 219, compiler-engineer: the seed's emission profile

Seat: compiler-engineer (the backend, the tool surface, the ceiling). Written 2026-10-17 from
02:37 as the work ran, in a detached worktree at `cbf724983`,
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/219-compiler-engineer` (removed at the
end), its compiler built from its own seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`, `real 7.53 user 7.41`; `./heroes build selfhost/main.hero --emit-c` then `cmp` against
the seed: silent, so `cbf724983`'s seed is its source's fixpoint). Scripts and derived files in
the scratchpad,
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad/219-ce/`
(`shapes.py`, `attrib.py`, `linecmp.py`, `content.py`). Every number names its command; what was
not run says *unrun*.

## Verdicts in one table

| what | verdict | ground |
|---|---|---|
| s0, the seed as written | object | 48% of its bytes are restores that s4 replaces losslessly |
| s1, no `#line`, as a seed-only profile | object | dominated by s4 in bytes; needs a way of asking that costs surface or a script |
| s2, `#line N` everywhere, no name (the brief's *line number alone*) | **veto, on soundness** (CLAUDE.md § 4; design.md has no rule on a debug location's truth beyond §3.1's *Debug info*, so I stand on that sentence) | it is false: a `-g` build names `s2.c:76` for `lexer.hero:76` (measured below); design.md §3.1 *Debug info* promises the `.hero` line |
| s3, the name only where it changes (C11 §6.10.4) | adopt, as half of s4 | lossless, measured: 0 of 1,234,797 addresses differ |
| **s4, s3 plus one physical C line per anchored source line, for every program, no profile** | **adopt** | 25,649,440 bytes native, line table identical, `lldb` 14/0, `run` 531/0, the fixpoint by `cmp`; no tool surface at all |
| R1, a bootstrap root without `mutate`, `probe`, `measure` | object at this milestone | saves 13.6%, misses 10 MB anyway, and the clang-built compiler stops being the whole one that 34 lines of 24 files print |
| R2, a bootstrap root that only builds | object | saves 19.4%, and breaks the site's start page (`./heroes run` after the clang line) |
| asked by a flag (`--no-line` revived, or `--seed`) | object | a second emission mode (panel 016's own objection), and the argv table's retired-flag message for `--no-line` contradicted |
| asked by a subcommand (`heroes seed`) | object | same artifact class as `--emit-c` (C text of a program), so the stopping rule's first sentence makes it a flag, not a verb |
| asked by a property of the input: a declaration in the root | **veto on the ceiling** | a new surface form, which lexer, parser, checker, formatter and every re-printing tool must carry, for one file |
| asked by a property of the input: the output path `-o seed/heroes.c` | **veto** | design.md §3.1 *Determinism*: the emitted C never depends on the output path, the same bytes on stdout and in `-o` |
| asked by the environment | object | an input argv does not show and the determinism suite cannot see |
| asked by the regeneration command, a pipe through `grep -v '^#line '` | object | zero compiler lines, but a script in the bootstrap's trust path, and it can make s1, never s4 |
| asked by the regeneration command naming a bootstrap root | admissible on the surface (an input, no flag), object on cost | as R1 |

**The 10 MB target is not reached by any shape this sitting names, measured.** The smallest, a
build-only root in s4, is 20,631,868 bytes. Reaching 10 MB means halving the C itself (§ 6 says
what it is made of), which is lowering and emitter work for every program and not a profile; no
seat built it, so it is *unrun*.

## 1. Where the `#line`s come from

One writer, `selfhost/emit/writer.hero` (426 lines at `cbf724983`), emits every body directive
through `directive` and `restore`; `c_text.line_at` (`selfhost/emit/c_text.hero`, 303 lines)
spells it. The other callers of `c_text.line_directive` (`emit/ffi_asked`, `emit/layout_check`,
`cli/header_types` and others) write the probe units clang reads to ask a header, not the
program's C.

The 48% is not *one `#line` per Heroes line*: it is a **restore before every C line after the
first** a Heroes line lowers to, because `#line N` anchors one line and C then auto-increments
(design.md §3.1). From the seed:

```
#line 216 "selfhost/cli/table.hero"
    h1_s0 = t1;
#line 216 "selfhost/cli/table.hero"
    t2 = h1_s0;
#line 216 "selfhost/cli/table.hero"
    t3 = t2.tag;
```

`grep -c '^#line ' seed/heroes.c` 621,943; the names, `grep '^#line ' seed/heroes.c | sed -E
's/^#line [0-9]+ //' | sort -u | wc -l`, 654: 652 `selfhost/` modules (of 653, `find selfhost
-name '*.hero' | wc -l`), `"main.c"` 8,638 times and `"<heroes library>"` 294.

## 2. The seed's size per module

`attrib.py` gives each line of C to the module whose mangled name, `h_<module>_` followed by
`(`, opens the last definition at column 0 (labels and directives skipped):

| directory | bytes | share |
|---|---|---|
| `emit/` | 10,986,277 | 21.3% |
| `cli/` | 9,022,263 | 17.5% |
| top-level modules | 7,361,528 | 14.3% |
| `check/` | 6,791,065 | 13.2% |
| `mutate/` | 5,528,726 | 10.7% |
| `ir/` | 3,080,763 | 6.0% |
| `parse/` | 2,892,462 | 5.6% |
| `print/` | 2,001,279 | 3.9% |
| `resolve/` | 1,978,914 | 3.8% |
| `probe/` | 577,886 | 1.1% |
| `measure/` | 191,814 | 0.4% |

Heaviest modules: `check/walk` 760,039, `ir/flatten` 448,917, `grammar_expr` 385,360,
`print/fmt` 367,420, `mutate/differential` 313,938, `mutate/cut` 299,474, `ir/print` 274,425.
The directories are not what a root saves, since `cli/mutate` sits under `cli/` and a module
two verbs use stays: § 4 measures the roots themselves.

## 3. The `#line` shapes, from the committed seed (`shapes.py`)

| shape | bytes | lines | `#line`s | `xz -9` | object without `-g` | line table under `-gline-tables-only` |
|---|---|---|---|---|---|---|
| s0, as written | 51,514,006 | 1,391,162 | 621,943 | 3,983,808 | 16,310,648 bytes | 1,234,797 rows, 654 files |
| s1, no `#line` | 26,803,431 | 769,219 | 0 | 2,504,280 | **identical to s0** | names the C file only |
| s2, `#line N`, no name | 32,868,681 | 1,391,162 | 621,943 | 3,512,328 | identical | **false** |
| s3, the name where it changes | 33,207,910 | 1,391,162 | 621,943 | 3,541,656 | identical | **identical**, 0 of 1,234,797 addresses |
| s4, s3 plus the restores joined | **25,638,593** | 257,326 | 55,025 | 2,655,336 | identical | **identical**, 0 of 1,335,312 addresses |

Commands: `python3 -I shapes.py seed/heroes.c full`; `wc -lc`; `grep -c '^#line'`;
`xz -9 -k -c <f> | wc -c`; `clang -I runtime -c <f> -o <f>.o` and `cmp s0.c.o <f>.o`, silent for
all four; `clang -I runtime -c -gline-tables-only <f>`, `/usr/bin/dwarfdump --debug-line`, and
`linecmp.py`, which maps every row address of either table to (file, line) under both, columns
ignored, and counts the addresses where they disagree. s4's rule: a directive that re-anchors
**the very line the previous C line sits on** is dropped and its C line joined onto that one,
never onto a directive, never a blank line, a preprocessor line, a line holding a newline or a
`//`. Its first draft re-anchored to the last directive instead, and disagreed at 1,760
addresses: the writer leaves a label's next line to auto-increment on purpose. s4's longest line
is 13,638 bytes.

**What a crash or a stop shows** (`clang -g` of each shape with `runtime/runtime.c`, then `lldb
-b -o 'breakpoint set -n h_lexer_lex' -o 'run lex tiny.hero' -o 'bt 3'`):

| shape | frame #0 | frame #1 | frame #2 |
|---|---|---|---|
| s0 | `lexer.hero:76:35` | `lex.hero:23:10` | `main.hero:230:12` |
| s1 | `s1.c:71760:35` | `s1.c:35127:10` | `s1.c:27659:12` |
| s2 | `s2.c:76:35` | `s2.c:23:10` | `s2.c:230:12` |
| s4 | `lexer.hero:76:85` | `lex.hero:23:21` | `main.hero:230:42` |

s1 names true lines of the C file; s2 names `.hero` line numbers under the C file's name, a
location that does not exist; s4 names what s0 names, its column a C column as s0's is.

**And in the seed's own build line none of it shows.** `seed/README.md`'s command carries no
`-g`, the five objects are one, byte for byte, and a panic prints `panic: <msg>` with no location
(`runtime/parts/panic.c:39-43`; `grep -c '__LINE__\|__FILE__' seed/heroes.c` 0, and `runtime/`
names neither). Under issue 2087's rhythm the clang-built compiler is stage 1, used to build stage
2, and stage 2 is built by `heroes build`, which writes its own `#line`s and its own `-g`
whatever shape the seed is in.

## 4. The bootstrap roots

Built in my worktree as files next to `selfhost/main.hero` (untracked, removed with it):
`boot.hero` (R1) is `main.hero` without `use cli/measure`, `cli/mutate`, `cli/probe`, their three
arms one refusal at exit 2; `boot2.hero` (R2) dispatches `--version` and `build` alone. Both
`heroes fmt` and `heroes check` clean. Emitted with the seed-built compiler, `--emit-c`:

| root | s0 | s1 | s3 | s4 | s4 `xz -9` | modules named | clang `-c` of s4 |
|---|---|---|---|---|---|---|---|
| `main.hero` | 51,514,006 | 26,803,431 | 33,207,910 | 25,638,593 | 2,655,336 | 652 | user 7.48 |
| R1 | 44,501,769 (-13.6%) | 23,141,601 | 28,695,369 | 22,142,500 | 2,317,676 | 593 | user 6.28 |
| R2 | 41,501,075 (-19.4%) | 21,553,777 | 26,731,372 | 20,631,868 | 2,160,844 | 557 | user 6.00 |

**The two-stage rhythm holds with R2 in s4**, by `cmp` each: clang on R2's s4 builds
`heroes-boot2` (10,694,528 bytes against the whole compiler's 13,174,008; `--version` prints
`heroes 0.4.0`; `check` and `mutate` refused at exit 2); `heroes-boot2 build selfhost/main.hero`
from an empty `build/` exits 0 (`real 101.88 user 72.69 sys 16.38` at load 14 to 17, so the
duration is discarded); the compiler it builds emits `selfhost/main.hero` as the committed seed
byte for byte, and `boot2.hero` as R2's emission byte for byte; and `heroes-boot2` emits its own
root byte for byte. R1's stage 2 is *unrun*; R1 is R2 plus code the stage-2 compiler already
holds.

**What a root costs beyond its bytes**, read in the tree: `VERSION` lives in `selfhost/main.hero`
and a second root needs it, so it moves, and `.claude/rules/records.md`, `.github/workflows/ci.yml`
and `editors/README.md` cite it there (`git grep`); the one-line bootstrap appears on 34 lines of 24
files outside the records, one of them under `archive/` (`git grep -n 'clang -I runtime seed/heroes.c runtime/runtime.c' --
':!docs/records' ':!docs/panel' ':!docs/measurements' | wc -l`), the site's start page among
them, which runs `./heroes run examples/gallery/03-fallible.hero` next (`site/src/html/start.html:16-17`),
so R2 breaks a published instruction and R1 keeps it (run: clang on each root's s4, then `run examples/gallery/03-fallible.hero`, R1 prints the page's three lines at exit 0, R2 refuses at exit 2; R1 refuses `probe` at exit 2); `seed/README.md`'s *The compiler it
produces is the real one* becomes false for stage 1; the CI runs the seed-built compiler in 15
places (`grep -n heroes-from-seed .github/workflows/ci.yml`), the net and `mutate examples`
among them, so every leg gains a stage 2 before them (cold on this Mac `real 70.58`, below); and
`trees.rebuild_hint` (`.claude/hooks/trees.py:112-119`) names one clang line where it would owe
two. Saved: 3.5 MB of s4 for R1, 5.0 MB for R2.

## 5. s4 in the writer, built far enough to be real

The two rules went into `selfhost/emit/writer.hero` in my worktree: the last line is held out of
the chunk until the next one comes, so a C line whose restore would re-anchor the held line's own
line joins it; and `named_line` writes `#line N` alone where the name equals the one clang
presumes, the presumption reset after any pushed text that carries a `#line` of its own. Diff:
112 lines added, 11 removed (`git diff --numstat`, the patch kept in the scratchpad as
`s4-writer-and-lines.diff`); by my count of non-blank, non-comment lines outside tests,
152 became 216. **The `layout` instrument reads 355 code lines, past §11's 300**, where the
original passes (`heroes run tests/harness/main.hero -- <compiler> layout emit/writer`, 3 and 1,
then 4 and 0 with the original swapped back): a landing splits the join rule into a module of its
own, a seam that names a concern.

What it measured, with the compiler that writer makes (`heroes-w2`):

- **the emission**: `selfhost/main.hero` in 25,649,440 bytes, 257,497 lines, 55,071 `#line`s of
  which 37,770 bare. Against the old writer's emission of the same source put through
  `shapes.py`, it differs only in the generated file's own numbers, `#line N "main.c"`, which the
  writer counts on the joined file (true) where the script kept s0's: with those numbers
  normalised by `sed`, `diff` prints nothing;
- **the fixpoint**: clang on that emission, the compiler it makes emits it again, `cmp` silent;
  the object without `-g` is identical to the old writer's; the line table under
  `-gline-tables-only` disagrees at 0 of 1,336,288 addresses (151,056 of them on the generated
  file, compared by name since its numbers are the new file's);
- **the suites**, each its own harness process: `lldb` 14 and 0; `determinism` 571 and 0;
  `wholes` 627 and 0; `descriptors` 627 and 0; `full` 39 and 0; `unsupported` 329 and 0; `run`
  531 and 0 (2 skipped by name, `sys/prctl.h`, which this Mac lacks); `emit` 1 and 12 and `emission` 669 and 624, every red *no longer emits what it was blessed to emit* and none of another kind,
  636 files to bless again; `lines` 1 and 531 as the tree has it, every red its own precondition,
  *no function's entry, a line ending `goto bb0;`*, which s4 moves by joining `bb0:` onto the
  prologue: taught a bare `#line N` and an entry ending `goto bb0; bb0:` (17 lines added and 2
  removed in `tests/harness/suite_lines.hero`, `git diff --numstat`), it reads **532 and 0 under s4 and 532 and 0
  under s0**;
- **the compiler's own tests**: 1,689, 6 failed, every one an exact-text expectation of the old
  restore (`writer`'s two, `body`'s *a whole function prints*, the dispatcher's, the laid-out
  constant's, the array field's two assertions);
- **speed**, the machine still (load 1.7 to 3.0), alternating, warm: `--emit-c` of
  `selfhost/main.hero`, old writer user 38.25 and 38.27, new 38.81 and 39.00 (+1.5% to +2.0%;
  a first version re-scanning the held line at every join read +7.5%, 41.13 and 41.33); a cold
  `heroes build selfhost/main.hero`, `build/` removed before each, user 55.23 against 55.51
  (+0.5%, real 70.58 and 71.11). clang on the seed: s0 user 7.82, s1 7.64, s4 7.48 (load 26
  falling to 14 from other seats' work, `real` within 0.33 s of `user` plus `sys` on every run, so
  none waited).

## 6. What the C is made of, and why 10 MB needs more than a profile

`content.py` over s1 (identifiers, temporaries, labels and numbers normalised):
`T = L;` 10.5%, `L = T;` 6.2% (copy chains, 16.7% together), an indexed read's inline bounds check
about 8% in its three shapes, `G(&T)` 4.3%, `G(hero_slot_escape(&L))` 4.2%, `if (T) goto B; else
goto B;` 3.1%, string and array releases 3.9%, leading whitespace 10.0% of s1 (2.4% of s4). Halving
that is copy propagation in the lowering and the bounds check out of line in the runtime header,
each a change to every program's C with its own sitting and its own ABI question; *unrun*.

## 7. History, the cost the milestone was opened for

Three consecutive seeds (`6fe850445`, `6e2690b50`, `bf6a9c4eb`), each shape committed in order to
a throwaway repository in the scratchpad, `git gc --aggressive --prune=now`, `git count-objects
-v`'s `size-pack`:

| shape | first copy | after three | per regeneration |
|---|---|---|---|
| s0 | 6,846 KiB | 14,914 KiB | 4,034 KiB |
| s3 | 6,308 KiB | 12,915 KiB | 3,303 KiB |
| s4 | 5,354 KiB | 10,235 KiB | 2,440 KiB (60% of s0) |
| s1 | 5,257 KiB | 9,901 KiB | 2,322 KiB (58% of s0) |

s1's advantage over s4 on history is 118 KiB a regeneration (4.8%), and on `xz -9` 151,056 bytes;
s4's advantage over s1 in the tree is 1,164,838 bytes (both from the committed seed), and it costs no second mode. Part 2's
rhythm moves the count of regenerations, which weighs more than either.

## 8. How the profile is asked for, under the stopping rule

The rule (`.claude/rules/cli-surface.md`): a capability enters only if the fixpoint invocation,
the golden harness or the Part 11 harness must type it; then a subcommand for a different
artifact class, a flag for another answer about the same input, nothing where two invocations
compose. **Its refusal of `--no-line` rests on a reason this question does not have** (*emitter
debugging is the test helper's job*, panels 016 and 020): a seed profile would be typed by the
fixpoint invocation, so the precondition would hold, and the brief's *§ 10 refuses `--no-line`*
is a question rather than a premise. Panel 016's other objection still binds every way that makes
a second mode: *it doubles the emitter's output space while the determinism diff is green in only
one mode*.

| way | compiler lines | `seed/README.md` | CI's fixpoint step | the hook | verdict |
|---|---|---|---|---|---|
| none: s4 is the emission | writer +112 and -11 lines (+64 code lines), split owed; `suite_lines` +17 and -2; 6 test expectations | its numbers only | unchanged | unchanged | **adopt** |
| a flag | an argv row, the flag threaded to the writer, the retired-flag message (`selfhost/cli/table.hero:188-191`) deleted, a determinism case for the mode | the regeneration line types it | types it | unchanged | object |
| a subcommand `heroes seed` | a `Command(` row, an arm in `main.hero`'s exhaustive match, help; the site's verb count (`site/src/lib/claims.ts:230`) in two languages | new regeneration line | the verb | unchanged | object |
| a declaration in the root | a surface form through lexer, parser, checker, formatter, every re-printing tool | unchanged | unchanged | unchanged | **veto** (ceiling) |
| the output path | a branch on `-o` | unchanged | unchanged | unchanged | **veto** (§3.1 *Determinism*) |
| the environment | an `env` read | a variable before the line | a variable | unchanged | object |
| a pipe, `\| grep -v '^#line '` | 0 | the pipe | the pipe | unchanged | object (s1 only, a script in the trust path) |
| the regeneration names a root | a second root, `VERSION` moved | two-stage bootstrap | `build selfhost/boot.hero --emit-c`, a stage 2 before the 14 later uses | `rebuild_hint` names two stages | admissible, object on cost |

## 9. Prediction, conditions, what would turn me

**Prediction**: if s4 lands as the writer's one rule, the seed regenerated at M-bounded-seed's
close measures between 24,000,000 and 27,000,000 bytes by `wc -c` and under 60,000 directives by
`grep -c '^#line '`, the `lldb` and `lines` suites read 0 failed, and a regeneration costs at most
65% of an s0 one by the method of § 7; **and no seed under 10,000,000 bytes exists at that close
unless a change to the C's content (§ 6) lands first**, which falsifies the sitting's target, not
this seat, if it is held to profiles.

**Conditions on adopting s4**: the join rule split out of `writer.hero` below §11's 300 (the
`layout` instrument); `suite_lines` taught both shapes in the same commit, its own tests included;
the emitter's `--emit-c` of `selfhost/main.hero` timed before and after on a still machine and
reported (+1.5% to +2.0% here); design.md §3.1's `#line` bullet and `.claude/rules/generated-c.md`
amended to say a re-anchoring line joins its anchor and a name is written where it changes;
the 624 blessed emissions under `tests/emission/` and the 12 `emit` goldens blessed again through
the suites' own procedure.

**What would turn me**: a clang message an author is shown whose excerpt or column the joined
line makes wrong or unreadable, or a debugger session the `lldb` suite does not hold that stops
somewhere else (then s3 alone for every program, lossless, and s1 for the seed by a root the
regeneration names); a measurement that 10 MB binds the clone a stranger takes so that R1's 13.6%
is the margin (then R1, asked by the regeneration command, never a flag); or the author ruling
that the clang-built compiler need not be the whole one.
