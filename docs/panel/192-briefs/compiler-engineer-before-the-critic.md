# Panel 192, the compiler-engineer's brief

Read `00-shared.md` and `00-facts.md` in this directory first; they bind you.
You judge implementation cost and core against sugar (your charter,
`.claude/agents/compiler-engineer.md`), and you have a veto on soundness. You
take **Q1, Q2, Q3 and Q7**, and you **build** what you recommend: a route that
does not build is not adopted (`/panel` § 3c).

## Where to look, in your copy `<scratchpad>/192-compiler-engineer/`

- **The string literal**: `selfhost/literals.hero` (`raw_carriage_return` at
  `:83`, called at `:119` for a string and `:162` for a character literal)
  and `selfhost/lex_interp.hero` (`:104`, an f-string's text).
- **The comment**: find its reader from the lexer, by a command, and say where
  it is.
- **The group head's refusal**: `selfhost/head_names.hero:191`
  (`unshowable_character`) and `selfhost/shown_char.hero` (`DEFAULT_IGNORABLE`,
  `UNSEEN`).
- **The escapes**: `selfhost/escape.hero` and design.md `:993` to `:1024`.
- **The compiler's own raw characters**: `selfhost/cli/compile.hero:83` (F5).

## What to do

1. **Q1 and Q2, built.** The refusal you recommend in a string and in a
   comment, in your copy:
   - its code or codes, its class, and its fix;
   - each shape of F1 refused or accepted as you rule, at `check`, `build`
     and `check --permissive`.
   - **Then run the net over your tree**, all 27 suites and `cache`
     (`./heroes run tests/harness/main.hero -- ./heroes <suite>`, three at a
     time at most), and report what moves.
   - F5's cases that hold these characters on purpose: what each needs.
   - `compile.hero:83`: what replaces its two raw U+0001, and that the cache
     key keeps its meaning (a separator no path holds).
2. **Q3, built if you recommend an escape.** Whichever route you take, build
   it, and refuse `\u{0}` and a surrogate in it if it is `\u`. Then show:
   - a program printing a terminal's ESC under it;
   - the NUL kept out;
   - `fmt` printing the escape back byte for byte;
   - what `--dump-tokens` shows.

   If you recommend (a), say by a command whether the language can build
   such a character at run time.
3. **Q7, the shapes beside**, each run on the base and on your route:
   - a character literal;
   - an f-string's text;
   - a `test` title;
   - a doc comment;
   - a diagnostic quoting a line that holds one;
   - `fmt --in-place` over a file holding one.

   Name what shares the cause and what does not.
4. **The tools that re-print a program**
   (`.claude/rules/diagnostics-and-goldens.md` § A new surface form, if Q3
   adds one): the formatter, the dumps, `mutate`, `probe`'s reader, the fixes,
   `measure`, the two highlighters. List what each owes, built or not.
5. **Cost**:
   - lines added and removed, by `git diff --stat` against your base;
   - the files that pass the layout ceiling (`layout`);
   - the seed's fixpoint after your edit (two generations, `cmp`).

## Your report

`docs/panel/192-reports/compiler-engineer.md` in the trunk, written as you
go. A verdict per question, with:
- what you built and ran;
- your cost;
- a falsifiable prediction;
- the condition that would change your verdict.
