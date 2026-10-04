---
kind: decision
area: none
milestone: none
filed: 2026-10-03
commit: f070c4c86556e23b0b0c148341c6fb7c17a57a11
github: none
---

# Panel 188 sat: a group head's string is its value, and what its tool cannot carry as one name is refused on its line

2026-10-03, written at 21:40 by the clock (`date`). The sitting
(`docs/panel/188-a-group-heads-string-is-its-value-and-what-its-tool-cannot-carry-is-refused-on-its-line.md`)
was convened by the author's answer *1a* of 16:27 for defect 216, `blocking`:
a header whose name holds a `>` passed `check` and stopped `build` at exit 2.
Its resolution is `provisional, author ratification pending`
(`docs/work/DECIDE.md`, `panel 188`).

**What it adopts, in one paragraph each** (its R1 to R12):
- **The value**: the five escapes spec § 2 gives a string are decoded where
  the emitters and the front end read a header, a `link` and a `package`; no
  reader did, so `extern "a\\b.h"` asked clang for two backslashes and `build`
  said a header present was missing.
- **What its tool cannot carry as one name**, refused at `check` on the
  value, `unwritable_name`: the empty string, a NUL and a line end in all
  three strings, `>` in a header, whitespace and `,` in a package; C's whole
  `<name>` carried in gets a certain fix. A `\` in a header apart,
  `escape_in_header_name`, because a `\` before the closing `>` makes clang
  bind another header, silently (the critic, measured). C's undefined `'`,
  `"`, `//`, `/*` as the thesis rule `undefined_header_name`, over the
  spec-warden's objection, its conservative form recorded. What no message
  can show, `unshowable_name`: every other control character, every
  Default_Ignorable code point, U+2028 and U+2029. A package names one
  package (`package_comparison`), a leading `-` is refused as well as `--`
  written before it, and a package naming a `.pc` file in any case is panel
  055's `machine_locked_path`.
- **The compiler asks again** in `cli/produce.hero` before any tool reads a
  name, exit 2 on its own defect; the two `-Werror` flags the ffi-pragmatist
  proposed were measured by it and dropped.
- **A leaf outside `parse/`**, which goes from 8,689 to 8,660 lines, so panel
  187's prediction holds.
- **No spec sentence**: 0 of 13 fresh sessions wrote C's brackets inside the
  string, and the three given no header's name chose against them by § 13's
  `sqlite3.h` example; the spec-warden's a6, paid by deleting the
  package-refusal sentence, is the recorded alternative.
- **(1i) vetoed**, (1c), (1g) and (1d) alone not adopted, and Nim's meaning
  for `"<name>"` refused as a second spelling.

**What was built and run**: five stages in the compiler-engineer's copy, the
last, E, adopted: on 98 cases this Mac's trunk exits 2 or 134 on 9 and E on
0, on 154 in the Linux arm64 image the trunk on 14 and E on 1 (a private-use
code point, the reader filed with the batch); the compiler's own tests 1,109,
`check` 450, `run` 261, `corpus` 55, `unsupported` 131, the census 0 of 1,073
moved; +350 lines (0.47%). The critic re-ran stages C and D from their
patches alone in copies of its own, ran the guard over 461 tracked programs
(0 firings), and measured three misses inside D's rules that E repaired.
Building found a crash in the builder's own stages B and C (`extern "é.h"`
aborting `check` with 134), an order fault, and two exits at 2 the trunk
already had (U+2028 and U+2029 in a missing header's name), all repaired
before adoption.

**What it cost**: the blind seat's fifteen sessions 2.6151 USD of the
author's 3, by their `run.json` files; no `heroes measure --refresh` (the
spec moves by 0); the Linux arm64 container one leg at a time; the Windows
box offline throughout, so everything it owes is the landing's. **156
minutes lost to a permission prompt**: the critic's third pass ran a command
beginning with `rm -rf` of its own scratch folder at 18:27:23, the prompt
reached the author's phone by Remote Control while the author was away from
it (the author's account), and its result is stamped 21:03:33; the machine
did not sleep (`sysctl kern.sleeptime`: 2026-10-01), and the sitting held no
`caffeinate -i` until 21:10. A seat that needs a clean folder can name a new
one instead; the `/panel` skill is the author's to amend with that.

**Found beside, and filed** (`docs/work/DEFECTS.md`, each reproduced by the
coordinator on the trunk's compiler): the undecoded escapes, the package read
as an option, the package's list and version grammar, the linker's reader and
the reader of clang's *file not found* line, each `blocking` and landed with
216; the `..` climb past `machine_locked` and `ffi_package`'s span,
`adjacent`; a source byte that is not UTF-8 answered at exit 2, and the
`certain` rename of `sqrt` to `sort`, `blocking`.

**The coordinator's own errors, recorded**: its messages to the seats carried
times not read from `date`, one to five minutes after each message's arrival
by the seats' transcripts, which three reports copied (each corrected
beneath, and one of those corrections first written with a guessed minute
too, corrected at once); and one repaired brief credited the critic with a
reading that was the coordinator's steer (corrected beneath it).
