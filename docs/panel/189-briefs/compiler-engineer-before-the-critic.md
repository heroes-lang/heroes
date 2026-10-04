# Panel 189, the compiler-engineer's brief

Read `00-shared.md` and `00-facts.md` in this directory first; they bind you.
You judge the implementation cost and the core-or-sugar question (your
charter, `.claude/agents/compiler-engineer.md`), and you BUILD the route.

## Where to look (at `7d9f2e8f`, by `grep -n` and `code_lines.py`)

- `selfhost/cli/input.hero`: `read_compilation` (24 to 29) and
  `read_root_only` (47 to 51), where every failure of `read_file` becomes
  `unreadable`, *cannot read*, exit 2 (F2).
- `selfhost/modules.hero` (317 of its 320 in the layout unit, F7): how a
  `use`d module is read, and why one that is not UTF-8 is told absent (F1,
  `used`).
- The runtime: `hero_utf8_valid` (`runtime/parts/str.c:248`, a `bool`),
  `read_file`'s check (`runtime/parts/os.c:643`), `HERO_OS_NOT_TEXT`
  (`runtime/hero_os.h:50`), `hero_failure_not_text`
  (`runtime/parts/failure.c:101`); the library's `read_file` failing with
  `not_text` (`selfhost/library_source.hero:217`).
- `unexpected_character` (`selfhost/scan.hero:297` and its siblings, F5),
  `selfhost/shown_char.hero` (how a message shows a character it cannot
  print), `selfhost/diag.hero` (`is_thesis_rule`).
- The verbs that read a compilation (`selfhost/cli/verbs.hero`, `measure`,
  `probe`, `mutate`), and `.claude/rules/cli-surface.md`'s exit contract.

## What to build, in your copy `<scratchpad>/189-compiler-engineer/`

1. Reproduce F1 on your compiler, and widen it at depth one: a lone
   continuation byte, 0xFF, a sequence cut at the end of the file, an overlong
   encoding, an encoded surrogate, a code point past U+10FFFF, the file's first
   byte, several bad lines, a bad byte in a string, a character literal and an
   `f"..."` piece, a `use`d module, through every verb that reads a `.hero`.
2. Build the route the proposal names, judged by you on Q1 to Q4: the code
   `not_text` at exit 1, the position and the byte, a `use`d module told at
   its own file; and measure Q3's alternatives on the cases (one diagnostic per
   file, per line, every bad byte with a bound; the rest of the file lexed past
   it or not), saying which a one-turn repair is served by.
3. **How the net holds such a case** is part of the cost: the harness reads its
   goldens as `str`, and a file that is not UTF-8 cannot be one. Measure what
   holds one (a case whose bytes a test writes at run time, an escape the
   harness decodes, another route), and build the one you judge.
4. The runtime: whether `hero_utf8_valid` gains an offset, a second function,
   or a lossy read; whether `HERO_RUNTIME_ABI` moves; the seed regenerated in
   your copy is not owed, but say what changes in `runtime/` and its tests.
5. Rebuild, then run: your cases; `check` whole; `annotations` and `fixes`
   narrowed to your cases; the compiler's own tests; `layout` WHOLE (its
   `budget` and `concat` checks are asked only whole); and the census of
   `check --brief` over the tracked `.hero` files, trunk against your build.
6. Q6 on your compiler: the CLI's own argv not UTF-8, `HEROES_RUNTIME` not
   UTF-8 (`selfhost/cli/process.hero:192`'s comment), a file or directory
   name not UTF-8; say what each does and whether it shares this cause.

Report per question: built or not, its lines (by the layout suite's unit),
the cases run and their counts, what it refuses that a real program needs (if
anything), your verdict and the condition that would change it.
