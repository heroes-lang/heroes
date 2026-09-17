# Panel 161 — historian

Read `00-shared.md` in this directory first. This file is your input only. You
have **no veto**; your seat is advisory, and **unsourced precedent is
inadmissible**. Every date, name and claim you return must carry a URL you
fetched in this session. This role is the most hallucination-prone in the panel
and the rule exists because of that.

## The question

C's plain `char` has a platform-chosen sign. Heroes' FFI declares a field and a
parameter at the header's own width and sign, and its eight integer types each
carry a fixed one — so a header's `char name[65]` has no portable spelling in
Heroes today. **How have other languages that bind C directly solved this, and
what did each choice cost them?**

## What to verify, each with a source

1. **Rust's `std::os::raw::c_char` / `core::ffi::c_char`.** Confirm it is a type
   ALIAS whose target varies by target triple, find the current documentation
   saying so, and find what Rust does about the read: is `c_char` usable in
   arithmetic without a cast on both signs? Find the RFC, issue or release note
   where the design was argued, and say what the argued alternative was. The
   `c_char` **portability hazard** is documented somewhere in the Rust
   ecosystem — find where, and quote it.

2. **Zig's `c_char`.** Zig added it relatively recently; find when and why, and
   what it was before. Zig is the closest living comparison to Heroes' position
   — no standard library between the program and C — so what it refused is as
   informative as what it took.

3. **Go's cgo.** `C.char` in cgo maps to a Go type; find the documentation
   saying which, and whether Go's answer is sign-stable or target-dependent. Go
   chose a different tradeoff from Rust and the reason is worth having.

4. **The ABI documents themselves, because the brief asserts them.**
   `00-shared.md` claims the generic AAPCS declares plain `char` unsigned while
   Apple's arm64 ABI declares it signed. **Verify both against the primary
   documents** — ARM's own AAPCS64 and Apple's "Writing ARM64 Code for Apple
   Platforms" — and say if either is misstated. A measured fact and a
   misattributed cause is exactly what this sitting has already found once
   today.

5. **Any language that chose to REFUSE plain `char`** at an FFI boundary, or to
   accept either signedness silently. If you find one, what happened to it? A
   route that has been tried and abandoned is the most useful thing you can
   return; a route with no precedent at all is also a finding, said in those
   words.

## What is NOT wanted

A survey of C history. The sign of `char` being implementation-defined since
K&R is assumed and needs no sourcing. What is wanted is **what the languages
that came after did about it at the binding boundary**, and what each paid.

## Two cautions

- **Recency matters and your knowledge may be stale.** Today is 2026-09-17.
  Check whether any of the above changed in the last two years, particularly
  Zig, which moves fast.
- **Say what you could not find.** A negative claim rests on the searcher's
  vocabulary rather than the world (CLAUDE.md § RUN IT), so *"I could not find
  X, searching for these terms on these sites"* is a good answer and
  *"X does not exist"* is not.

## Deliver

Verdict (advisory) · every claim with the URL you fetched · what each precedent
COST the language that took it · a falsifiable prediction with its milestone ·
and, explicitly, anything in `00-shared.md` you found to be misstated.
