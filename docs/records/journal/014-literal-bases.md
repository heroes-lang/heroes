# 014 — M-literal-bases: four ways to write a number, and one that stopped lying

## Goal

Give an `int` the notations every C header uses — `0x1f`, `0o37`, `0b11111` —
with `_` between any two digits, and settle what a leading zero means. One
milestone, because the author's ratification of panel 041 split the sitting's two
questions and put this one first: the notation is independent of the type system
and much cheaper, and folding it into `M-sized-integers` would have made the
cheap question hostage to the expensive one for the third time in one day.

The warrant is panel 040's, arriving one milestone late. That sitting landed
`& | ^ ~ << >>` on the argument that a flag union spelled `+` is a silent wrong
answer — and then a mask had to be written in decimal, which is the one notation
no header uses. A reader comparing `curl.h` to a `.hero` file converted by hand,
and no compiler could check the arithmetic.

## What surprised

**The measured defect was not the one the milestone was about.** `print(0700)`
printed `700`. Not a diagnostic, not a wrong type — a wrong *value*, from the
most-copied constant form in Unix, in a language whose entire thesis is that
every plausible mistake is a compile error. It was found by a reviewer that had
been given only the specification and asked to write a file-permissions task; it
reached for `0700` the way anybody would, and flagged that the document did not
say what it meant. Nobody who could see the compiler had ever asked.

Two repairs existed and only one had been proposed. The narrow one is to refuse
the bare leading zero and say so. The other is to give octal a real spelling, so
`0o700` is 448 *and* the bare form becomes an error — which closes the defect and
delivers the notation with the same three lines. It is what Python 3, Rust and Go
each arrived at independently, and always for the same reason.

**The interesting design question was not "which notations" but "where case is
settled".** §4.15 says there is exactly one correct way to write any program, and
`0xFF`, `0xff`, `0Xff` and `0XFF` are four spellings of one value. Refusing
uppercase outright is the obvious reading of the rule and it is wrong: a mask
arrives in a program by being copied out of a C header, where it is written
`0xFF`, and a language that refuses the paste has refused the reason the notation
exists. So the rule is split. The **prefix** is refused uppercase, because there
is no header to copy it from and `0O` is genuinely misreadable as `00`. The
**digits** accept either case and `fmt` lowercases them. That division is not
invented here: no language's lexer enforces hex digit case, and the one tool that
canonicalises it is a formatter.

**A premise that had not been written down was one milestone from expiring
silently.** The mutation harness's twelfth operator moves one digit of a
`constant`'s value, and it found the last digit by testing `is_ascii_digit`. That
was correct while `9` was the only top digit in the language. The day hexadecimal
arrived it would have mutated `0x10` and skipped `0xff` — coverage depending on
which character a mask happened to end in, in the operator whose *site count* is
the number the previous milestone made its deliverable. Nothing would have gone
red. A sibling operator had the mirror of it: appending `.0` to make an
implicit-conversion mutant would have produced `0xff.0`, which dies in the lexer,
so an operator named for a type mistake would have been scoring the scanner.

Both repairs move nothing today, because the corpus has no based literal in it
yet. That is the shape CLAUDE.md §11 asks for and the reason it asks: the fix is
cheap while the premise is visible and invisible once it is not.

## What broke and why

**Nothing broke; one thing could not be written the way it was planned.** The
adversarial case was drafted as six lines in one file, the sixth being an
out-of-range literal — and it reported five diagnostics. The stages are gated, so
a lexical refusal on the first line means the type checker never runs, and
`int_out_of_range` is the frontend's rather than the lexer's. The case is two
files now, and the reason is written in both.

**The change had one hard precondition that was not on the plan.** Two passes
decoded integer literals independently — the frontend, to decide whether to
report an out-of-range one, and the lowering, to get the value — each calling
`parse::<i64>()` on the source slice with the out-of-range sentence spelled out
by hand in each. With one base, drift between them costs a duplicated message.
With four it costs a **wrong value**: the two could disagree about what `0x10` is
and nothing in the compiler would say so. There is one decoder now, and it also
builds the diagnostic, which is how the message came to answer in the notation
the question was asked in — a reader who wrote sixteen `f`s is told the largest
`int` is `0x7fffffffffffffff` rather than `9223372036854775807`.

**The one veto the ratification did not overturn was the one that was right.** A
based literal could have been read as a *bit pattern*, making
`0xffffffffffffffff` a legal way to write `-1`; that reading closes the all-ones
mask with no unsigned type at all, and it is what `1 << 63` already does. It was
refused because it deletes a compile error: sixteen `f`s is `-1` and fifteen is
1152921504606846975, so a mask copied one digit short would have become a
silently wrong number instead of a diagnostic. `~0` has been the all-ones mask
since the previous milestone anyway, which is the fact that made the refusal
cheap.


---

## The closing block, as `docs/ROADMAP.md` § Status carried it

Moved here at `M-sized-integers` close (2026-08-12), by `/step`'s rule that the
ROADMAP says what is *next* and a closed milestone's record is its journal. The
wording is the milestone's own; `Next:` names what was next **then**.

**M-literal-bases closed 2026-08-12, tag `m-literal-bases` — an `int` is written
in the base the header uses, and `0700` stopped meaning seven hundred.**

    $ heroes run tests/golden/run/literal-bases.hero
    hex 255 · oct 448 · bin 10 · sep 1000000 · mix 3735928559

`0x1f` `0o37` `0b11111` `31`, `_` between any two digits, by the author's
ratification of panel 041: a base creates no value that did not exist, so panel
035's exponent precedent does not reach it. The reading is by **value**, and
octal also closed a live silent defect — `print(0700)` printed `700`.

**529 tests · 49 CLI · 13 harnesses**, clippy clean, spec **2745** of 4096,
`mutate` 93% / 78% over 1252. Record: `docs/journal/014-literal-bases.md` ·
`docs/panel/041`, ratified and scored. Next: **M-sized-integers**.
