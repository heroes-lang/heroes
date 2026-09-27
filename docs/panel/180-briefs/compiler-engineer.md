# Panel 180, brief for the compiler-engineer

Read `00-shared.md` first. Your seat judges implementation cost and
core-versus-sugar (design.md §1.1, §1.7, Part 5), with a veto on soundness.
Your directory is `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/compiler-engineer/`.

## What you decide

1. **The whole map, not the table's sample.** Enumerate every bracket context
   the grammar has (a group, a call's arguments, an index, an array literal, a
   map literal, a record construction, a parameter list, a type's brackets, an
   interpolation's hole, a `match` or `if` value inside brackets, a Place's
   index on the left of `@`, an `extern` member's parameters) against every
   break position (after the opener, before the closer, before and after `,`,
   `:`, `.`, `=`, `@`, `->`, `=>`, a binary operator, a unary one). Run each
   with `heroes parse`, and say where the list of contexts came from (the
   spec's productions, `grep` of the parser's `expect` calls), because the
   list is a measurement too.
2. **State the rule the compiler implements** in the fewest words that are
   true of the whole map, and name every exception it has (the index's `]`,
   the call's `,` in `00-shared.md` are two; there may be more).
3. **Price the routes**, each in files and lines of `selfhost/`, prototyped
   in your own copy far enough that the number is real:
   - **(a)** the spec states the rule as it is, exceptions included, and the
     compiler does not move;
   - **(b)** the compiler is made uniform where it is irregular (for example
     an index's `]` crossing a terminator as every other closer does, and one
     answer for a break before `,` in every list), and the spec states the
     uniform rule;
   - **(c)** the spec's sentence is made true: inside brackets a NEWLINE falls
     between any two tokens except where a production writes one, which moves
     the lexer or the parser; say what it breaks (§4.9's newline-separated
     literals, `Sep = "," | NEWLINE`, the formatter's reliance named in
     `00-shared.md`), and whether it reverses §4.15's deferral or is a
     different question;
   - any route none of these names.
   For (b) and (c), run the compiler's own tests and the net's `surface`,
   `canonical`, `check`, `annotations` and `grammar` suites in your copy on the
   prototype, and say which break.
4. **What the formatter does with each shape**: `heroes fmt` on the accepted
   shapes of your map, since a rule the parser accepts and the formatter
   cannot print is not a rule a program can keep.

## Prediction

Register one falsifiable prediction with an instrument that exists today,
scored at this milestone's close (M-agreed-retention).

Write your report to `<your directory>/report.md`. English, no em dashes.
