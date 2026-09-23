# Panel 175 — compiler-engineer

Everything below was run on 2026-09-23, Darwin 25.6.0 arm64, Apple clang 21.0.0,
in `<scratchpad>/175-compiler-engineer/` (the trunk at `64c92654` by `git
archive`, plus the untracked `docs/panel/175-briefs/`). The compiler was built from
the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 3.26 s).
Prototype compilers were built with `./heroes build selfhost/main.hero -o <name>`
(77.7 s real, 72.7 user). Nothing in the trunk was built, run or written except
this file. Linux and Windows are **UNRUN** for every route in this report.

**One interruption, said so nobody reads a gap as a result.** Partway through,
every Bash call was refused by a worktree-isolation guard naming
`/Users/joseph/Temp/heroes-lane-076` (not this seat's directory). It lifted on its
own. No number below comes from that stretch; each one was produced by a command
run after it, except the baseline table in § 1, which was run before it.

## The verdict, per route

| route | verdict | the section it stands on |
|---|---|---|
| **A** set remembers the releaser | **approve**, on four conditions (§ 6) | §1.7 (not core), §1.12 (loud on every path) |
| **B** same-function checker rule | **object** | §1.7's working criterion, Principle 0 |
| **C** two Heroes types over one tag | **object** | §1.12 (the boundary stays complete), §1.7 |
| **D** A+B / A+C | **object** to the B and C halves; A alone | as above |
| **E** handler speaks with no lease | **approve only in the E2 shape** (say it after chaining) | §1.12's falsifier, panel 173 R1 |
| **F** a sentence in § 13 | **approve** | costs no compiler line |
| **G** nothing | **object** | §1.12 |

**No veto.** Not one route adds a core construct (design.md Part 5's seven) and not
one breaches the ceiling: the largest compiler cost here is B's 91 lines.

```
verdict: approve (A, E2, F) · object (B, C, G) · no veto
section: design.md §1.7 (core plus elaboration), §1.1 (the ceiling), §1.12 (robustness)
implementation_cost: A = selfhost 18 code lines (emit/handle_traffic.hero) + 2 (emit/decls.hero stamp),
                     runtime 41 code lines (parts/alloc.c) + 3 (heroes_runtime.h), 0 in lexer/parser/checker/
                     descriptors/ownership; ABI 22 -> 23; 227 + 7 blessed files and the seed move.
                     B = 91 lines (67 code) new check/crossing.hero + 2 in check/walk.hero.
                     C = 1 code line in check/decls.hero. E2 = 19 code lines in runtime/parts/os.c, 0 compiler.
needed_for_self_hosting: no — selfhost/ declares 0 extern lines with acquires/consumes/borrows (grep, below)
```

**argument** (120 words). A is not core: no token, no checker line, one argument
more at two emitter sites that already exist, 18 compiler code lines and 41 runtime
ones. It flips three of the four reproducers before C runs, at `-O0`, `-O2` and under
`--sanitize`. Its price sits outside the compiler: ABI 23, 234 blessed files and the
seed move. The strongest reason it is wrong is measured: `sqlite3_close_v2`, a
releaser SQLite documents for `sqlite3_open`'s handle, now aborts a correct program,
because `acquires` names one function. And no listed route reaches `xacquires`:
nothing crosses there at run time. B buys two of A's three for 91 checker lines; C
is opt-in. E is sound only if it is said after the previous handler has been called.

---

## 1. Baseline, reproduced before any change

`./heroes check <f>`, `./heroes build -O0 <f> -o <bin>`, one run each, in a copy of
the brief directory with its `build/` caches deleted:

| reproducer | check | build | run: exit / stdout / stderr |
|---|---|---|---|
| `popen_darwin.hero` | 0 | 0 | 0 / 28 B / 0 B |
| `oneacq.hero` | 0 | 0 | 0 / 24 B / 0 B |
| `outacq.hero` | 0 | 0 | 0 / 24 B / 0 B |
| `xacquires/main.hero` | 0 | 0 | 0 / 0 B / 0 B |

This matches the brief's table. One run each, not five: the five-run numbers in § 3
are all from after the change.

## 2. Where the code lives today (`wc -l`, measured)

`selfhost/check/acquiring.hero` 278 · `selfhost/check/decls.hero` 367 ·
`selfhost/emit/handle_traffic.hero` 143 · `selfhost/emit/ops.hero` 462 ·
`runtime/parts/alloc.c` 610 · `runtime/heroes_runtime.h` 602 ·
`runtime/parts/os.c` 510 · `tests/harness/suite_runtime.hero` 754 ·
`selfhost/check/*.hero` 10805 in total · `selfhost/check/consuming.hero` 129 ·
`selfhost/resolved.hero` 400 · `selfhost/check/walk.hero` 2292.

`handle_traffic.for_call` and `before_call` run only on the extern-call path of
`emit/ops.hero` (`:178`, `:204`), where `extern_name = source.span_text(s,
decls[e.decl].name)` (`:138`) is the C symbol written in the call. spec § 13's
`Member = "function" ident …` has no renaming, so **an extern's Heroes name is its C
symbol in every translation unit**. That fact is what route A's identity rests on.

## 3. Route A, prototyped: `<scratchpad>/175-compiler-engineer/prototype-A.diff`

**What it does.** The set becomes one table of `{ const void *h; const char *by; }`
pairs. `hero_handle_acquired(h, "<releaser>")` stores the name the mark wrote.
`hero_handle_consumed(h, "<consumer>", <transfer-or-NULL>)` aborts **before the C
call** when the address is live and neither name matches, naming both. The emitter
passes the releaser from `fd.value.acquires_result` / `p.acquires` in `for_call`,
and the consumer's own name in `before_call`. Emitted, for `oneacq.hero`:

```
    t1 = h_open();
    hero_handle_acquired(t1, "h_close");
    ...
    hero_handle_consumed(t2, "h_close2", NULL);
    (void)h_close2(t2);
```

### The four reproducers, `heroes-next` + the patched runtime, five runs each

| reproducer | check | build | run (exit/stdout/stderr) ×5 | what it prints |
|---|---|---|---|---|
| `popen_darwin` | 0 | 0 | **134 / 0 B / 219 B** ×5 | `panic: a C handle was given back to `fclose`, and the call that handed it over is marked `acquires pclose` — …` |
| `oneacq` | 0 | 0 | **134 / 0 B / 222 B** ×5 | the same, `h_close2` / `acquires h_close` |
| `outacq` | 0 | 0 | **134 / 0 B / 222 B** ×5 | the same, through the `@out` arm |
| `xacquires/main` | 0 | 0 | **0 / 0 B / 0 B** ×5 | nothing |

Stdout is 0 B on the three that flip, where it was 24 to 28 B: the abort comes before
the wrong releaser runs and before the `print` after it. At `-O2` and under
`--sanitize`, `oneacq` and `popen_darwin` exit 134 with 222/225 B and 219/219 B,
and `grep -c AddressSanitizer` is 0 on all four.

**`xacquires` does not flip, and no route in the brief can flip it.** Each module
keeps the promise it made: `alt` acquires with its own `h_open … acquires h_close`
and gives the handle back to `h_close`; `main` acquires with its own `h_open …
acquires h_close2` and gives it back to `h_close2`. Nothing crosses at run time.
What is wrong is that **two declarations of one C function disagree** about what ends
its result. `i2.h:9-10` makes both releasers `free(x)`, so in C this program is
correct. Only a declaration rule sees that disagreement: *one C name, one mark,
program-wide*, beside `bindings_say_which`. That route is not in the brief's list.
**UNRUN**: I did not prototype it. It would also refuse a correct pair of bindings
where both releasers are valid (the `sqlite3_close_v2` row below), so it depends on
the ffi-pragmatist's measurement.

### Which identity survives separate compilation, measured in C

`work/ident/{a,b}.c`, both including a header of `static` functions, linked into
one binary, at `-O0` and `-O2`:

```
fn address equal across TUs: 0        <- (const void *)&h_close differs per TU
literal address equal across TUs: 1   <- ld64 merges __cstring; C11 6.4.5p7 leaves it unspecified
literal content equal across TUs: 1
```

So a **function address is not sound**: a `static` function in a header (the shape
of `i2.h`, `r1.h` and every `static inline` binding) has one copy per module. Two
spellings merely happen to work on this Mac. A **literal's address** is equal here
only because Apple's linker merges string literals. The ISO C standard does not
promise that, and I did not run another linker (**UNRUN**, Linux and Windows). **A
string compared by content** is sound by construction, and that is what the
prototype does. A program-wide integer id would be baked into a module's cached
`.c`. Whether the per-module cache key would catch a renumbering is a question I did
not run.

### The shapes beside A (three runs each; baseline `./heroes` + stock runtime vs A)

| shape (all in `work/shapes/`) | baseline | A |
|---|---|---|
| `font_whole`: `load_font() -> Font acquires unload_font`, two handles in fields, `unload_font(f)` | 0 | 0 |
| `font_part`: the same, then `unload_texture(t: f.texture)` first | 134, stray, **after** `print("part")` | **134 at the first release**, `given back to unload_texture … acquires unload_font`, 0 B stdout |
| `xmod/`: acquired in `lib` and released in `main`, then acquired in `main` and released through `lib.close_it(@h)`; `static` header functions | 0 | **0** (per-TU C shows `"h_close"` on both sides) |
| `peek_same`: a `borrows` result given to the named releaser | 0 | 0 |
| `peek_other`: the same borrow given to `h_close2` | **0, silent** | **134**, names both |
| `fixed_reissue`: C hands out one static address again after its release (`a == b` prints `true`) | 0 | 0 |
| `fixed_unseen`: the first life ended out of sight (an unmarked `f_drop`), the same address is acquired again under `acquires f_close2` | 0 | **0** |
| `transfer`: `h_grow(x: H consumes) -> H acquires h_close` (the `realloc` shape) | 0 | 0 |
| `freopen`: real `stdio.h`, `fopen … acquires fclose`, then `freopen(… stream: File consumes) -> File acquires fclose` | 0 | 0 |
| `popen_freopen`: `freopen` of a `popen` stream | **0, silent** | **134**, `given back to freopen … acquires pclose` |
| `close_v2`: real `sqlite3.h`, `sqlite3_open(@out: Db acquires sqlite3_close)`, closed with `sqlite3_close_v2` | 0 | **134 / 8 B / 236 B** — a correct C program aborted |

Three choices in the prototype are measured, not argued:

- **The newest mark wins at re-acquisition.** With a runtime that keeps the first
  mark (`runtime-keepfirst/`), `fixed_unseen` goes to **134, 222 B** and names
  `f_close`, the releaser of a life that is already over. The newest mark is right:
  an address C hands out again belongs to the life that just began.
- **A transfer pays only its own mark.** A naive A, where the consumer must be the
  named releaser (`heroes-naive`), aborts `freopen` and `transfer` at **134, 220 B**:
  it refuses correct C. Exempting every transfer (the first draft, `NULL`) gets both
  right but lets `popen_freopen` through silently. The shipped rule is: *a consumer
  that also acquires may pay what its own mark names*. It keeps both correct
  programs at 0 and refuses `popen_freopen`.
- **Pairs, not a parallel array.** The first draft added a second table
  (`hero_handle_by`), and `runtime` went **6 passed, 2 failed**:
  `runtime/alloc: allocation calls … 9 against a floor of 7`, and
  `runtime/threads: runtime/parts/alloc.c:174: static const char **hero_handle_by`.
  Folding the name into the entry type keeps the object count and the allocation
  count, and the suite reads **8 passed, 0 failed** with no new
  `SHARED_BY_DECISION` entry.

**The `close_v2` row is the strongest reason A is wrong**, and it is measured.
`acquires` takes one `ident` (spec § 13 grammar, the `Member` and `CParam` lines), so
a module whose `sqlite3_open` names `sqlite3_close` cannot also use
`sqlite3_close_v2`, and `declared_twice` stops it from declaring `sqlite3_open`
twice. The program aborts loudly, naming both functions, and it does not corrupt
memory. So the loss is to §1.12's *complete* clause, not its *must not crash*
clause. How often a shipped library has two releasers that are both valid is the
ffi-pragmatist's question. If the answer is *often*, the repair is a list,
`acquires a | b`. That is a surface change (lexer, parser, `check/acquiring.hero`'s
`releaser_reads`, the formatter, the `grammar` suite), and I would price it before
approving.

### What A costs, by file (`diff -u` against the untouched copies)

| file | + / − | added code lines (comments and blanks excluded) | `wc -l` after |
|---|---|---|---|
| `selfhost/emit/handle_traffic.hero` | +25 / −5 | 18 | 167 (was 143) |
| `selfhost/emit/decls.hero` | +2 / −2 | 2 (the stamp and its test) | — |
| `runtime/parts/alloc.c` | +50 / −17 | 41 | 644 (was 610) |
| `runtime/heroes_runtime.h` | 3 lines changed | 3 | — |

**0 lines** in the lexer, parser, checker, descriptor pass or ownership pass.
**`HERO_RUNTIME_ABI` 22 → 23**, because both declarations change shape.

**What moves with the stamp.** These counts come from the `emission` suite before
and after an `UPDATE_EMISSION=1` run *in the scratch copy*, and from `diff` against
the trunk's files:

- `tests/emission/`: **227 red, 257 green** before blessing, **484/0** after. Of the
  227, **211 differ only in the stamp line**. **16 also differ on handle lines**
  (the three `examples/`: curl, ledger, sqlite, plus 13 `run-*` cases), **556 diff
  lines in total**.
- `tests/golden/emit/`: **7 red** (`emit: 1 passed, 7 failed`). Each `.expected`
  differs by exactly the one stamp line (`diff … | grep -c '^[<>]'` = 2 for all 7).
  **None of them carries a mark.**
- `tests/golden/ir/`: **0**, and `ir: 24 passed, 0 failed`. No ir case carries a
  mark; the only grep hit, `regression-wildcard-binds-nothing.hero:7`, is the prose
  word *consumes*.
- **The seed must be regenerated in the same commit.** `clang -I runtime
  seed/heroes.c runtime/runtime.c` against the new header fails:
  `static assertion failed due to requirement '23 == 22'`. The regenerated seed
  (`heroes-next build selfhost/main.hero --emit-c`) compiles with 0 clang
  diagnostics, and the compiler built from it re-emits it byte-for-byte (`cmp`
  silent).

**Suites run on A** (`heroes-next run tests/harness/main.hero -- ./heroes-next
<suite>`):

- `run` **136 passed, 0 failed**, both runtime drafts (339 s real, 132 user: the run
  was waiting on its children, and no timing is claimed from it);
- `runtime` 8/0; `ir` 24/0; `canonical` 2/0; `layout` 2/0; `order` 3/0;
- the compiler's own tests: **675 tests, all passed**.

**Speed.** A emits nothing new at an unmarked call. `selfhost/` declares no mark:
`grep -rnE '^\s*function .*(acquires [a-z_]|consumes|borrows)' selfhost/`, minus
comment and string lines, finds **0**. So A was not timed, per brief item 6.

## 4. Route B, prototyped: `prototype-B.diff`

`check/crossing.hero`, **91 lines, 67 of them code**, plus 2 lines in
`check/walk.hero` beside `consuming.refuse_borrowed` (`:1323`). An argument in a
`consumes` position is refused when it is a `.bind` local whose `resolved.Local.value`
(`resolved.hero:100`) is a call to an extern marked `acquires`, and the consumer is
neither that releaser nor a transfer paying it.

**Reach, measured with `heroes-b check`**:

- **2 of 4**: `popen_darwin` and `oneacq` get `error[crossed_releaser]`.
- `outacq`: check 0. The handle arrives in a `.cell` through `@h`, and `Local`
  records how many writes a cell has (`writes`, `:102`) but not where they are. So
  reaching `@out` needs a walk of the function body. I estimate that at about 40
  more lines. **UNRUN.**
- `xacquires`: check 0, because nothing crosses.
- From § 3's shapes, B catches `popen_freopen` and misses `font_part` (a field is not
  a name) and `peek_other` (a borrow is not a binding).
- B refuses 0 of the 136 run goldens and 0 of the examples, and `corpus` reads
  **55/0**.
- **`check` goes 134/1.** In `fixedbugs-a-crossed-free-between-two-void-families`,
  B reports a second error on the same span as `type_mismatch`. A landing owes a rule
  that yields to `type_mismatch`: more lines.

**Time**, `/usr/bin/time -p ./<c> check selfhost/main.hero`, alternated, three runs
each, real time within 0.1 s of user plus sys:

- A only: 19.28, 19.40, 19.44 s;
- A plus B: 19.39, 19.28, 19.55 s.

No change is measurable.

**Why I object.** A catches everything B catches, at run time, on every path, for
fewer lines. B's only extra value is moving two shapes from run time to compile
time. Principle 0 admits that only on a measured design.md Part 11 effect, and
nobody has measured one. §1.7's working criterion asks whether a change removes a
special case from the compiler; B adds one.

## 5. Route C, prototyped: `prototype-C.diff`

It costs one code line: `&& !handles.is_handle(decl)` at `check/decls.hero:338`.
Measured with `heroes-c`:

- `croute/split.hero` (`record Pipe tag __sFILE` beside `record File tag __sFILE`):
  `fclose(stream: f)` on a `popen` stream is `error[type_mismatch]: expected File,
  found Pipe`. **But the brief's own `popen_darwin.hero`, with one `File`, still
  checks at 0**, so C closes the class only for an author who already knew to split
  the types.
- `croute/split_fgetc.hero`: calling one stdio function on both types is
  `error[declared_twice]` inside one module, so every `FILE *` function costs one
  module per Heroes type.
- The `check` suite goes **134/1**: `ffi-handle-refusals.expected:1` (`Db2` and
  `Db`, both handles over `sqlite3`) stops being refused. That undoes the author's
  decision of 2026-09-13 at a real tag, which `decls.hero:285-289` records.
  `acquiring.hero:77` and `:118` also rest on *one tag is one handle declaration*.

## 6. Route E, prototyped: `prototype-E1.diff` and `prototype-E2.diff`

**E1**, 15 lines in `os.c`: an `else if (!hero_runtime_spoke)` beside the lease line.
It says `panic: the process died of SIGTRAP|SIGABRT, which this runtime did not
raise, in <module>.<function>`, then names C's usual reasons as *most often*.
**E2** is E1 with **both** lines moved after the previous disposition has been
called: **+24 / −6, 19 code lines**, `os.c` 510 → 528. Zero compiler lines, no ABI
change. Programs are in `work/eroute/`, built with the seed-built `./heroes`
against each runtime; three runs each:

| program (no lease live unless named) | stock | E1 | E2 |
|---|---|---|---|
| `p10` (double free, `ptr`) | 133 / 0 B | 133 / 322 B, true | same as E1 |
| `b_out` (a `cstr` freed twice) | 133 / 0 B | 133 / 323 B, true | same |
| `e_abort` (C `abort()`) | 134 / 0 B | 134 / 325 B, true | same |
| `e_assert` (failed C `assert`) | 134 / 66 B, C's line | C's line, then ours, 392 B | same |
| `e_trap` (`__builtin_trap()`, SIGTRAP on arm64) | 133 / 0 B | 133 / 324 B, true | same |
| `e_kill` (`kill(getpid(), SIGABRT)`) | 134 / 0 B | 134 / 324 B, true (the *most often C* clause holds) | same |
| `e_panic` (Heroes index out of range) | 134 / 32 B | 32 B, one line | 32 B, one line |
| `e_recover` (a C handler installed before `main` that `siglongjmp`s out of `abort`) | **0** / 0 B | **0 / 327 B: "the process died" in a process that exits 0** | **0 / 0 B** |
| `e_recover_lease` (the same, with a lease live) | **0 / 277 B: "the process died with 1 lease(s) still live"** | the same | **0 / 0 B** |

**The last row is a defect in what ships today**, and I am reporting it rather than
filing it. Panel 173's lease line is written *before* the handler it found is
called (`os.c:249-262`, then `:264-269`). So when that handler recovers, the process
exits 0 under a sentence saying it died. That breaks panel 173's R1 (*no path prints
a sentence measured false*). E1 would add a second false path. E2 closes both.

What E2 gives up: when a library's handler ends the process itself (panel 173's
`libabrt.h`, `_exit(77)`), E2 prints nothing where the shipped handler prints the
lease line. That is silence, not a falsehood. `grep` finds no golden with
`LIB_CTOR` or `!exit: 77`.

**"This runtime did not raise it" is true on every path I could enumerate.**
`grep -rn '[^_a-z]abort('` over `runtime/parts/*.c`, `runtime/*.h` and
`selfhost/**/*.hero` finds one raw `abort()`, `panic.c:36`, inside `hero_abort`
after the flag is set. All 227 blessed emissions contain 0. The runtime's only
`kill(` calls send SIGKILL to a child's process group (`run.c:320`, `:777`).

**Golden impact.** `run` with the E2 runtime: **136 passed, 0 failed**. No run golden
dies of a C signal with no lease live (the only `!exit:` expectations are `3` and
`0`). New goldens for E can use the `!sanitizer:` form that `suite_run.hero:32`
already has, so no harness change is needed.

**Not reached, and never false:** `__builtin_trap` on x86-64 raises SIGILL, which
no handler catches. On Windows the arm sees only `0xC0000374` (`os.c:207`). Both
are **UNRUN** here.

## 7. Question 2's route nobody listed, and it costs zero lines

A pointer C makes for the program to give back can be declared as a handle **today**,
over `tag void` (panel 170 admitted `void` for this). `eroute/p10_void.hero`:
`record Mem tag void`, `make() -> Mem acquires release`, `release(p: Mem consumes)`,
released twice. Result: **check 0, 134 / 396 B ×5**, the runtime naming the double
release before C runs. So F's sentence has a referent that already works, and E's
line can point the reader to it, as my E prototype's third line does.

## 8. prediction

**Checkable at the commit that lands A, inside M-agreed-retention.**
`docs/panel/175-briefs/xacquires/main.hero` still checks at 0 and runs at **exit 0
with 0 bytes of stderr, five of five**, while `popen_darwin`, `oneacq` and `outacq`
exit 134 with a line naming both functions.

The same commit's `git diff --numstat -- selfhost/` shows **at most 30 added
non-test lines**, and its `emission` run before blessing reads **exactly 227
failed**. If `xacquires` flips in that commit, the landing contains a rule this
report did not price, and it will be in `check/acquiring.hero`, not the runtime.

## 9. condition — what would change my verdicts

- **On A:** the ffi-pragmatist measures that two-valid-releaser libraries are common
  in real bindings, beyond `sqlite3_close`/`sqlite3_close_v2`. Then A-with-one-name
  aborts correct programs routinely, and I would **object** until
  `acquires a | b` is priced on the lexer, parser, formatter and `grammar` suite.
  Or a measured program where two different C functions share one name across
  modules and name identity aborts a correct program.
  Or the Linux or Windows leg running A differently from Darwin.
- **On E:** a path where E2's sentence is measured false. Its four landing
  conditions are the E2 ordering, the two `e_recover` programs as goldens, the
  Windows arm measured on the box, and the shipped lease line repaired in the same
  change.
- **On B:** a measured Part 11 effect, meaning first-attempt compile rate on
  handle-crossing tasks, showing that a compile error here changes what a model
  writes next. Then I would **approve** B beside A, with the yield to
  `type_mismatch` added.
- **On C:** a census showing that splitting the types is what binding authors
  already do. Nothing measured here suggests it.

## Files

- Prototypes: `<scratchpad>/175-compiler-engineer/prototype-A.diff`, `prototype-B.diff`, `prototype-C.diff`, `prototype-E1.diff`, `prototype-E2.diff`
- Programs: `<scratchpad>/175-compiler-engineer/work/shapes/`, `work/eroute/`, `work/croute/`, `work/ident/`
- Suite transcripts: `<scratchpad>/175-compiler-engineer/tmp/suite_*.txt`
