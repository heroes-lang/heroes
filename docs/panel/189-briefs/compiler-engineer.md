# Panel 189, the compiler-engineer's brief

Read `00-shared.md` and `00-facts.md` in this directory first; they bind you.
You judge the implementation cost and the core-or-sugar question (your
charter, `.claude/agents/compiler-engineer.md`), and you BUILD the route.
Repaired after the critic's first pass (the text it read is
`compiler-engineer-before-the-critic.md`).

## Two copies

- `<scratchpad>/189-compiler-engineer/trunk/`, `git archive 7d9f2e8f`: F1's
  reproduction and the census's trunk arm.
- `<scratchpad>/189-compiler-engineer/lane/`, `git -C
  /Users/joseph/Temp/heroes/heroes-lang archive 6ee963e7` (lane b8-source's
  tip, its commits read through git and never inside its worktree): **the
  route is built here**, since it lands on top of defect 236's repair, which
  rewrote `cli/input.hero` and `modules.hero` and added
  `selfhost/module/reading.hero` (F6). Build each copy's compiler from its own
  seed.

## Where to look

- `selfhost/cli/input.hero` (both copies; F2), `selfhost/modules.hero`,
  `selfhost/module/reading.hero` (the lane's), and the three verbs that read
  through their own calls: `cli/mutate.hero:96`, `cli/probe.hero:115` and
  `:319`, `cli/measure.hero:44` (F2).
- The runtime: `hero_utf8_valid` (`runtime/parts/str.c:248`), `read_file`'s
  check (`runtime/parts/os.c:643`), `HERO_OS_NOT_TEXT`
  (`runtime/hero_os.h:50`), `hero_failure_not_text`
  (`runtime/parts/failure.c:101`); the compiler's own extern groups
  (`selfhost/cli/files.hero:18`, `cli/process.hero:37`, F3).
- `unexpected_character` (`selfhost/scan.hero:297`, 291 of 300 lines, F5, F7),
  `selfhost/shown_char.hero`, `selfhost/diag.hero` (`is_thesis_rule`).
- The writers: `check --apply` and `fmt --in-place` (F8).
- The readers F8 found taking *not text* for absence:
  `cli/toolchain.hero:85` (the runtime's own sources), `cli/deps.hero:100` (a
  C header's digest).

## What to build

1. Reproduce F1 on your trunk copy, and widen it at depth one: a lone
   continuation byte, 0xFF, a sequence cut at the end of the file, an overlong
   encoding, an encoded surrogate, a code point past U+10FFFF, UTF-16 with and
   without a byte-order mark, the file's first byte, several bad lines, a bad
   byte in a string, a character literal and an `f"..."` piece, a `use`d
   module, a valid U+FFFD (a control: it must stay legal), through every verb
   that reads a `.hero`.
2. Build the route in your lane copy, judged by you on Q1 to Q4: the code
   (`not_text` as proposed, or what you measure better) at exit 1, the
   position and the byte, a `use`d module told at its own file, every reader of
   F2 covered; measure Q3's alternatives on the cases (one diagnostic per file,
   per line, every bad byte with a bound; lexed past it or not; one replacement
   per bad byte or per run) and say which a one-turn repair is served by.
3. **No writer receives a text with replaced bytes** (F8): show, by a case,
   what `check --apply --in-place` and `fmt --in-place` do with a file that is
   not text under your route.
4. **How the net holds such a case** (F8): `annotations` and `canonical` cannot
   read it, and CLAUDE.md § 9 asks every diagnostic annotated in its source;
   measure what holds one (bytes written through C at run time, as the run
   golden of F8 does; an escape the harness decodes; another route), and build
   the one you judge.
5. The runtime: an offset added to `hero_utf8_valid`, a second function, or a
   read that keeps the text; whether `HERO_RUNTIME_ABI` moves (panel 089 held
   it, F3) or the function is bound in the compiler's own extern groups (F3);
   what changes in `runtime/` and its tests; the C side goes to the
   ffi-pragmatist for its platforms once you have a diff (say where it is).
6. Rebuild, then run: your cases; `check` whole; `annotations` and `fixes`
   narrowed to your cases; the compiler's own tests; `layout` WHOLE (its
   `budget` and `concat` checks are asked only whole); the census of `check
   --brief` over the tracked `.hero` files, trunk against your build (it
   cannot see the new diagnostic fire, F8, so say what it does see).
7. Q6 on your build: F8's two readers (the runtime's sources, a C header), the
   CLI's own argv not UTF-8, `HEROES_RUNTIME` not UTF-8; say whether each
   shares 227's cause.

Report per question: built or not, its lines (by `.claude/hooks/ceiling.py`
and `layout`), the cases run and their counts, what it refuses that a real
program needs (if anything), your verdict and the condition that would change
it; and the patch, against `6ee963e7`, under your copy.
