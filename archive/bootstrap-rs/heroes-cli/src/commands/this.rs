//! `heroes this` — print The Zen of Heroes (panel 080).
//!
//! An easter egg in the tradition CPython's `this.py` began (2002) and
//! `zig zen` carried into a compiled toolchain. It is the surface's one
//! author-ratified exception to CLAUDE.md §10's stopping rule, admitted in
//! the class `--version` occupies — zero input, constant output, exit 0 —
//! and never citable for a capability that reads a file, a flag, or the
//! environment. Bowie is here under CLAUDE.md §11's clause: prose and
//! packaging, never error text or library names. Python stores its Zen
//! ROT13-encoded; this one is plain text, because its own law 4 says what
//! the eye can see is all there is.

use crate::cli::Exit;

/// The twenty laws, byte-exact: `tests/surface.rs` pins this text verbatim,
/// so a change here is a deliberate change in two places (CLAUDE.md §9's
/// redundancy rule — and law 15's).
const ZEN: &str = "The Zen of Heroes

 1. First of all and freely, this language is a tribute to the great David Bowie.
 2. A modern language made for LLMs, yet plain to human eyes.
 3. The machine has read everything, yet sees only the page before it.
 4. What the eye can see is all there is, nothing but sound and vision.
 5. One way to say each thing, and every program sings it in the same voice.
 6. Although that way is not obvious at first, the Starman waiting in the sky already knows it.
 7. The semantics you already know, a syntax nobody has seen before.
 8. Turn and face the strange, for every strangeness here is deliberate.
 9. Nothing changes behind your back, every change signs its name where it happens.
10. Every plausible mistake becomes a compile error before the program ever runs.
11. Unless you confess it honestly and write ??? in its place.
12. An honest hole in the program beats a confident guess every time.
13. A compile time error is an answer, a run time error is an ambush.
14. An error tells you how to fix the program, anything less is a complaint.
15. A repeated word is cheap, a forgotten one costs the whole program.
16. The specification is small and lives under pressure, every word must earn its place.
17. Nothing crashes and nothing leaks, ashes to ashes, what it takes it returns.
18. Where the pretty things and the robust disagree, the robust wins every time.
19. There is no standard library, everything comes from C, the man who sold the world.
20. We can be heroes, just for one day.
";

pub fn run() -> Exit {
    print!("{ZEN}");
    Exit::Ok
}
