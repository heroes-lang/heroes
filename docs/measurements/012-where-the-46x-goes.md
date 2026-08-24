# 012 — Where the 46× goes: two quadratics repaired, and the rest has a name

Date: 2026-08-23/24. This is the measurement panel 088 R5 asked for: emission
scaled 9.6× source → 46× time, push was measured at 7% of it, and the
superlinearity was somewhere nobody had looked. R5 named
`selfhost/emit_writer.hero:82` (`w.out @ w.out + text + "\n"`, once per output
line) as the leading suspect and marked its ~440 s extrapolation as an
inference. This file replaces the inference with measurements — and adds a
second culprit the sampler found that nobody had suspected.

**Everything below was run in the session that writes this file**, serially —
no other load during a timed run — with `/usr/bin/time -p` or the shell's
`time`, on the same machine as measurement 011. Two timed runs were discarded
and re-taken; § The discarded numbers says why, because the failure mode will
recur.

## Repair 1 — the writer accumulates in chunks

`emit_writer.hero` no longer accumulates into a `str`. It could not switch to
a flat `[str]` either: design.md §4.10 records that `push` copies every
element on every push, so the flat array is O(n²) too (1M pushes = 806 s,
panel 037) and `join` would be reached through exactly the cost it removes.
The form that lands is the one §4.10 already names — **accumulate in
chunks**: each line is pushed onto a short `chunk: [str]`, every `CHUNK_LINES`
(128) lines the chunk is joined into one `str` and pushed onto
`chunks: [str]`, and `finish` joins once. Inner cost n·K/2 element copies,
outer (n/K)²/2; K = 128 sits within 2× of the balance point ∛(2n) from 100k
lines to 1M. No runtime change, no ABI, no new surface, no spec token.

Two tests were added at the seam a regenerator cannot fake: one crosses the
flush boundary twice with blank lines interleaved (a smuggled join separator
would show), one ends exactly on the boundary.

Measured, same batch, serial: `build selfhost/main.hero --emit-c` went
**944.76 s → 537.60 s (1.76×)**, output **byte-identical** on 764,180 lines
(`cmp`). R5's ~440 s inference measures at **407.16 s — 43.1%** of the build.
The suspect was the right one and the estimate was honest.

## Repair 2 — loading stops re-reading the whole program per file

Found by attribution, not by suspicion. `sample` (macOS, the machine's
instrument — R5's own suggestion) against an untimed run showed the first
~120 s of *every* command inside `source.from_files` → `starts_of`, with
`hero_array_push` + `hero_copy_int` as the hot leaves — **the loader, before
any phase begins**. The cause: `from_files` computed each file's
`lines_before` as `starts_of(text).len() - 1` **over the whole accumulated
text, once per file** — a 153-module compilation re-read its own
concatenation ~76 times on average, growing a fresh line-starts array by
quadratic push inside every pass. Dense sampling along a `lex` run put the
load→lex boundary between t=90 s and t=150 s.

The repair is an incremental newline counter over just the appended segment
(`newlines_in`), byte-equivalent by construction — `starts_of(text).len() - 1`
*is* the newline count — plus one test pinning `lines_before` across a
middle file with no trailing newline. The final `starts_of` over the finished
text stays: it runs once.

Measured, same batch, serial: **531.63 s → 419.04 s (1.27×)**, output
**byte-identical** on 764,307 lines. The loader was costing **112.6 s** of
every build — and of every `lex`, `parse`, `check` and `test` too.

## The headline

| batch | before (2026-08-23) | after (committed compiler) | ratio |
|---|---|---|---|
| `build selfhost/main.hero --emit-c` | **944.76 s** | **424.67 s** | **2.22×** |
| `build tests/harness/main.hero --emit-c` | 21.98 s | 14.59 s | 1.51× |
| `check tests/harness/main.hero` | 8.17 s | 7.43 s | 1.10× |
| `test selfhost/main.hero` (488 tests, green) | 20m35s | **446.08 s** | **2.77×** |
| the net (1132/1132 green) | 6m28s (388 s) | **347.36 s** | 1.11× |

(Intermediate points, each measured at its own step: 537.60 s after repair 1
alone, 419.04 s after repair 2 before the split — the split moves lines, not
time: the fixpoint's two emissions read 424.67/423.44 s.)

Every pair of outputs compared: **byte-identical** — old compiler vs new on
selfhost (764k lines, twice: after each repair) and on the harness, and again
from the compiler rebuilt from the regenerated seed. The identity is also the
fixpoint: the committed seed is the C the new compiler emits for its own
source, and the new compiler is a clang build of that very file.

## The phase profile — the instrument R5 asked for, now readable twice over

`heroes lex|parse|check` run the phases up to their name, so subtraction
gives the profile. Before these repairs the subtraction was doubly poisoned:
the loader's quadratic sat inside every command, and R5's own microbenchmark
shows what printing through the old writer would have added. With the final
compiler, on `selfhost/main.hero`, under `caffeinate`:

| command | total | phase alone | share of build |
|---|---|---|---|
| `lex` | 205.53 s | load+lex **205.53 s** | **49%** |
| `parse` | 330.19 s | parse 124.66 s | 30% |
| `check` | 364.71 s | **check 34.52 s** | **8%** |
| `build --emit-c` | 419.04 s | emit 54.33 s | 13% |

(The same subtraction on the intermediate, writer-only compiler read: lex
316.30 · parse 133.70 · check 27.65 · emit 59.95 — the loader's 112 s was
sitting inside the lex number, which is why the sampler and not the
subtraction found it.)

Two suspects from the R5 brief are exonerated: the check phase ("8m03s and
it does no emitting") was ~90% loading+lex+parse wearing check's name — the
checker itself costs ~35 s — and with it the O(types) type-table walks,
which live in check and emit and together cannot exceed the ~89 s those two
phases now cost.

## The attribution — measured, not inferred

`sample` during the lex phase (t=150/210/270 of a `lex` run): the dominant
leaves are `hero_array_push` with its alloc/release retinue — the token
stream growing one push at a time (`lexer.hero:69` re-pushes every token of
every module into the merged stream). During parse (t≈370 of a `check` run):
`parse_decl` frames over `hero_array_push` and `h_ast_Expr_desc_copy` — AST
arrays growing the same way.

## What this leaves open, and where it points

Load+lex at ~205 s and parse at ~125 s are **79% of the remaining build**,
and both are `p @ push(p, v)` accumulators. **Chunking cannot reach them**:
chunks work for strings because `join` rebuilds in one pass, and the language
deliberately has no array concatenation (`[i64] + [i64]` is `bad_operand`,
panel 043), so a `[[Token]]` flattens at the same quadratic it left. The
repair with a name is the **place store** — adopted by panel 037, prototyped
at panel 088 (100k pushes 22.34 s → 0.00 s), waiting on the author's trigger
(DECIDE R2). This measurement is the number that row was missing: after the
two repairs that needed no decision, the majority of the compiler's build
time sits in the one shape only the place store fixes.

## The net caught the repair growing a frozen file

Repair 2 added ~22 code lines to `source.hero`, and the harness went red —
1131/1132 — on `suite_layout`'s ceiling check: `source.hero` sat in the
frozen list at exactly **304** code lines (a file already past §11's ~300
may not grow further), and the repair took it to 326. The fix the check
itself prescribes is a split along a seam that names a concern, and the
seam was chosen by measuring the users, not the sizes: the file's own
`## The root's own extent` section (`root_end` · `user_text` · `text_of`,
25 lines) is the one section whose five users (`lexer`, `parse`,
`cli_check`, `cli_compile`, `cli_syntax_cmds`) are all under their
ceilings — the tempting "Names" seam would have forced a `use` line onto
`modules.hero` (311/311) and `resolve_state.hero` (338/338), both frozen
at exactly their measured size. `source_extent.hero` is the new module
(39 code lines), `source.hero` lands at **300**, and the migrated tests
went with their functions.

## Suite and net

Final numbers with the committed compiler, under `caffeinate`: **488 tests
green in 446.08 s** (was 20m35s on 2026-08-23: **2.77×**; the count grew from
485 by this work's own tests), **the net 1132/1132 in 347.36 s** (was 6m28s).
The net's modest ratio is expected and matches panel 088: it is many small
compiles plus clang plus process spawning, not one large emission. After
repair 1 alone the suite read 552.14 s (2.24×) and the net 348.45 s.

## The discarded numbers

Two timed runs were thrown away and re-taken, both for the same reason: the
machine slept or downclocked mid-run (this work ran past midnight). The
net's first run reported 2246.96 s wall against 241.46 s CPU; a phase-profile
run reported `check` at 1139.81 s wall / 885.96 s CPU where the caffeinated
re-run reads 364.71/363.85. Recorded because the wrong conclusion was one
glance away each time: a wall clock taken while the laptop naps reads as a
regression at exit 0. The rule this leaves: **a timed run is `real ≈ user`
or it is not a measurement**, and `caffeinate -i` is part of the protocol.
