# 020 — The probe: the lexer ported, to measure what self-hosting lacks

## Goal

Port the Rust lexer to Heroes **for real** — before the port, not during it —
to find out what self-hosting still lacks (Principle 0's checkpoint). The
milestone opened on the SCHEDULED item that names it: mark every
ordering-sensitive map walk `// ORDER:` and fix the design.md §4.9 note that
pointed at the wrong file, which took panel 065 (soundness lane, two
approve-with-conditions) because the note is design.md Part 4. Then the port:
eleven files in `selfhost/`, 1,888 lines, 55 test blocks green under
`heroes test selfhost/lexer.hero`, and the cross-check — `heroes lex
--dump-tokens` and the port produce the same stream for the same program.
Acceptance artifact: `docs/measurements/009-selfhost-readiness.md` (the
ROADMAP's pointer said 004; 004 was taken by `error-codes` before this
milestone opened).

## What surprised

- **The wall the milestone was aimed at no longer exists.** The ROADMAP's
  premise — `as_bytes()` at seven sites, "the closure list has no form that
  replaces byte access" — described the lexer of 2026-08-12. M-sized-integers
  filled it before the probe ran: `s[i]` is a `u8`, a character literal takes
  its width from context, and `b - '0'` is ordinary arithmetic. The probe's
  gaps are accordingly small, which is itself the measurement — and a lower
  bound, since the lexer has zero `BTreeMap` and zero closures.
- **The language refused its bootstrap's own recorded defect.** Rust's
  `is_line_ender` forgot `nullptr` when the keyword arrived (layout.rs's own
  comment records the trap: silent for a line, loud on the next). In Heroes
  the bug is inexpressible — `_` is forbidden on a variant match, so the
  ported predicate names all 61 kinds and a 62nd is a compile error on that
  very function until somebody decides its enderness.
- **The thesis features fired on the compiler's own port, unprompted.**
  `needs_label` forced named arguments on the port's first two-`str`
  constructor; the unused-`use` rule pruned a dependency the port did not
  have; `declaration_in_arm` refused a no-op and forced a better shape (the
  match yields the fixes array). Every one of the porter's own mistakes
  arrived back with a `certain` fix attached.
- **The one gap with a real cost is a testability gap.** `\r` has no
  spelling (the escape set is five and frozen, panel 008), which the scanner
  absorbed as `constant CR: u8 = 13` — but the `stray_carriage_return` test
  cannot be written at all: it needs a CR inside a test string. 33 of the 34
  Rust lexer tests translated; that one is named where it cannot be written.
- **No `i128`, and no form wanted**: every literal is unsigned by
  construction, so `u64` holds every one a program can write, and the signed
  lower bound moves to where negation lives — the checker port. The decoder
  guards each accumulation step instead, because overflow aborts rather than
  failing politely.

## What broke and why

Nothing in the compiler. Five porting mistakes, all caught by the language at
compile time with the repair attached (`u @ t` for a declaration; missing
commas in a multi-line construction; two missing `use` lines; unlabelled
same-typed arguments), and two wrong test expectations, both mine and both
instructive: `base_prefix_case` lowers only the *prefix* — the digits are
case-blind by design, so `0X1F` repairs to `0x1F` and my `0x1f` was the bug —
and a block header ending in an identifier earns its terminator before the
indent, which the Rust dump confirmed. One false alarm: `heroes test`
appeared to exit 0 on a failure, and the 0 was `tail`'s exit code in my own
pipeline.

## What landed, and what carried forward

**Closed 2026-08-15, tag `m-selfhost-probe`.** Landed: panel 065's thirteen
`// ORDER:` marks with the rewritten §4.9 port note and two order-pinning
goldens (the per-option function order and the `fn{N}` numbering had no
checked-in pin at all — the double-emit test pins stability, not order);
`selfhost/` with the lexer end-to-end (token, keywords, escape, digits, diag,
state, layout, bytes, number, literals, scan, lexer — 55 tests); measurement
009 with twelve gap entries, every one workaround-compiles, none qualifying
for the closure list under §1.0; `hero_spawn` priced at **+39 spec tokens**
(3374 → 3413 measured and reverted) with signature
`spawn(cmd: str, args: [str]) -> i64?` — a price, not a landing; `private`
stays Part 7 item 14 (no blockage, and the rule says a wish is not one);
predictions scored in panels 037, 039, 041, 057, 064.

Carried forward to M-selfhost-port, with reasons in measurement 009 § What
carries: the second file (maps, recursive variants, generics — it opens the
port), the multi-file `lex` wrapper (needs the Source record), the two
diagnostic builders of `digits.rs` (need render-in-base — the first real
string-building want), and the `stray_carriage_return` test (the one deferral
with a named cost). Chain: M-complete-structs → **M-selfhost-probe** →
M-selfhost-port.
