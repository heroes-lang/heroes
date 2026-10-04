# Panel 192, the compiler-engineer's brief

Read `00-shared.md`, `00-facts.md` and the critic's first pass
(`docs/panel/192-reports/completeness-critic-briefs.md`) first; they bind
you. You judge implementation cost and core against sugar (your charter,
`.claude/agents/compiler-engineer.md`), and you have a veto on soundness. You
take **Q1, Q2, Q3 and Q7**, and you **build** what you recommend: a route
that does not build is not adopted (`/panel` § 3c). *Repaired after the
critic's first pass: the text it read is `compiler-engineer-before-the-critic.md`.*

## Where to look, in your copy `<scratchpad>/192-compiler-engineer/`

- **The string literal**: `selfhost/literals.hero` (`raw_carriage_return` at
  `:83`, called at `:119` for a string and `:162` for a character literal)
  and `selfhost/lex_interp.hero` (`:104`, an f-string's text).
- **The comment**: find its reader from the lexer, by a command, and say
  where it is.
- **The group head's refusals**: `selfhost/head_names.hero:155`
  (`unwritable_byte`, the NUL and the line end, not a thesis rule) and `:191`
  (`unshowable_character`, a thesis rule); `selfhost/shown_char.hero`
  (`DEFAULT_IGNORABLE` at `:204`, `UNSEEN` at `:95`); `diag.hero:129-153`,
  the thesis list (F6).
- **The escapes**: `selfhost/escape.hero`; the `unknown_escape` message and
  its `guess` fix (F10); design.md `:993` to `:1024`.
- **Every decoder of a string literal**, which a new escape meets beyond the
  re-printers (the critic's list): `selfhost/escape.hero`,
  `selfhost/emit/externs.hero:69` (`unquoted`, a group head's string),
  `selfhost/check/contextual.hero:140` (`literal_value`),
  `tests/harness/suite_lines.hero:327`, `suite_records.hero:1919` and
  `suite_spec.hero:1343` (which reads every escape the compiler accepts out
  of `escape_text`'s single-letter arms, a pattern `\u{...}` does not fit).
- **The compiler's own raw characters**: `selfhost/cli/compile.hero:83`
  (F5), and the 13 places it writes a control character by `[u8]` with
  `validated_bytes()` (F10).

## What to do

1. **Count by context first, with `heroes lex`**, not a regular expression:
   which tracked files hold each class of F5's characters inside a string,
   a comment, a character literal or elsewhere. F5's split is the critic's
   approximation.
2. **Q1 and Q2, built.** The refusal you recommend in a string and in a
   comment, in your copy:
   - its code or codes;
   - its class, against F6's three precedents;
   - its fix;
   - **which list**: panel 188's or `UNSEEN`'s, and the tab's class whole
     (NBSP, U+2000 to U+200A, U+202F, U+205F, U+3000);
   - **real text**: what a program printing ZWJ, ZWNJ, a variation selector
     or a tag character writes under your route, run;
   - each shape of F1 refused or accepted as you rule, at `check`, `build`
     and `check --permissive`.
   - **Then run the net over your tree**, all 27 suites and `cache`
     (`./heroes run tests/harness/main.hero -- ./heroes <suite>`, three at a
     time at most), and report what moves.
   - F5's cases that hold these characters on purpose: what each needs.
   - `compile.hero:83`: what replaces its two raw U+0001, and that the cache
     key keeps its meaning (a separator no path holds). Route (a′) works on
     today's seed.
3. **Q3, built.** Whichever route you recommend, build it. Then show:
   - a program printing a terminal's ESC under it;
   - the NUL kept out;
   - `fmt` printing it back byte for byte;
   - what `--dump-tokens` shows;
   - what it does in a character literal and in a group head's string;
   - **what `unknown_escape` says** for `\x1b`, `\u{1b}`, `\u001b`, `\e` and
     `\033` under it, and whether its fix can be `certain`;
   - **the landing order**: escape, seed and refusal, given the seed reads
     only the escapes it knows.

   Weigh route (d), an escape only for what a literal refuses raw, against
   design.md `:994-995`'s one spelling, and route (a′), which already exists.
4. **Q7, the shapes beside**, each run on the base and on your route:
   - a character literal (a raw ESC there checks at exit 0);
   - an f-string's text;
   - a `test` title and `heroes test`'s output (F11);
   - a doc comment;
   - a diagnostic quoting a line that holds one;
   - `fmt --in-place` over a file holding one.

   Name what shares the cause and what does not.
5. **The tools that re-print a program**
   (`.claude/rules/diagnostics-and-goldens.md` § A new surface form, if Q3
   adds one): the formatter **and its own self-check**
   (`selfhost/cli/syntax_cmds.hero`), the dumps, `mutate`, `probe`'s reader,
   the fixes, `measure`, the two highlighters. List what each owes, built or
   not.
6. **Cost**:
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
