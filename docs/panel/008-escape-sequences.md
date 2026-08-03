# Panel 008 — escape sequences in string and char literals

Date: 2026-08-04. Trigger: M1 step 3 implemented string/char literals by the
letter of a design.md that never mentions escapes, so `\` became an ordinary
byte — and `print("a\nb")` compiles and silently prints four characters.
A plausible LLM mistake that is *not* a compile error: the anti-thesis.

Differentiated inputs: the engineer built the patch and measured it against
the real tree; the ergonomist wrote six programs from the spec alone; the
warden did the token arithmetic and audited its own methodology; the ffi
judge compiled the C of ladder rungs 1 and 3; the historian verified every
precedent at the source.

## Proposal (as put to the judges)

- **Amendment C** — a minimal C-style escape set in string AND char
  literals: `\n \t \r \\ \" \'`; any other character after `\` is a
  compile error.
- **Amendment D** — the backslash stops being an ordinary byte; today's
  silent trap becomes loud even where no escape is intended.
- Open spec question: whether spec v1 carries one line about escapes.

## Verdicts

| Judge | Verdict | Cost/delta | Prediction | Condition |
|---|---|---|---|---|
| llm-ergonomist | approve | V1 = 1 certain silent-wrong of 3 tasks, and `"` is *unwritable* from the spec; V2 = 0 of 3 | on tasks whose stdout embeds a newline/tab/quote inside a string: V1 compile ≥80% but tests-pass ≤35% (gap ≥45pp); V2 closes it to ≤5pp | the unknown-escape clause ships with the set; bad-escape diagnostic carries a `certain` fix |
| ffi-pragmatist | approve C, **object to D alone as an endpoint** | none — ABI, mangler, `cstr`/NUL, emitted C all untouched | rungs 1–3 need zero shim for any string argument | set frozen at these escapes; **veto reserved on `\0`/`\xNN`/`\u{}`** (interior NUL silently truncates every C call, voids §4.20's free `.cstr()`) |
| compiler-engineer | approve-with-changes | +104 lines, one file; `token.rs`/`mod.rs` byte-identical; `TokenKind` unchanged | `backslash_is_an_ordinary_byte` will NOT flip by itself — `dump()` prints raw source slices and is blind to escape semantics | **drop `\r`**; no new `TokenKind` variant or `LexState` field; `scan.rs` ≤400 lines; decoder lands in the same commit as validation |
| spec-warden | approve-with-changes (provisional) | C+D = **0 spec tokens**; candidate spec line +31 est → breach; shrunk 17-word form +23 with two named removals −21 → ~1496 | (1) a real tokenizer puts spec v0 at 1650–1900, not ~1496; (2) at baseline ≥60% of multi-line-output completions use a C escape, <5% outside the set | design.md C+D land now; the spec line waits for a **measured** count |
| historian (advisory) | approve | n/a | the heaviest consumer of escapes will be the compiler's own C emitter (`\\`), not user programs — checkable at the fixpoint | record the Pascal asymmetry; adopt Go's `\'`/`\"` split or state why not |

## Disagreements, unsmoothed

1. **`\r`.** The engineer struck it (§1.0: on neither the closure list nor
   the thesis; the self-hosted lexer *tolerates* CR but never writes one,
   and a program emitting CRLF is precisely the silent-output class
   Principle 0 exists to close). The warden kept it only conditionally, on
   compiler-need for CRLF rejection. Zig's and Rust's sets both include it.
   Resolved by cutting it — the burden of proof was never discharged.
2. **Amendment D alone.** The ffi judge would accept D as a *transition*
   but objects to it as an endpoint: with the backslash merely reserved,
   `printf("%d\n")` — rung 1 of §4.19's ladder — stays unexpressible, and
   §4.19's own acceptance test fails. D ships with C, never instead.
3. **Two judges converged independently on the same wart.** The engineer
   found `"\'"` and `'\"'` would both be legal — two spellings for one
   program, against §4.15's "exactly one correct way"; the historian
   arrived from Go's spec, which forbids exactly those two. Same fix.

## Resolution — RATIFIED by the author, 2026-08-04

1. **Five escapes, split by context (Go's rule).** In strings:
   `\n` `\t` `\\` `\"`. In char literals: `\n` `\t` `\\` `\'`. So `"` needs
   no escape inside a char literal and `'` needs none inside a string —
   each character has exactly one spelling, and §4.15's canonical-form rule
   survives. `\r` is **cut** (unproven under §1.0).
2. **The backslash is reserved** (Amendment D): any other character after
   `\` is a compile error, whose diagnostic names the legal escapes and
   ships the `certain` fix `\\`. This is the half that converts
   `"C:\temp"` and `"\d+"` from silent bytes into diagnostics — the
   settled modern position (Go, Rust, Zig all error from day one; C's
   permissiveness is what Python has been unwinding since 2016).
3. **Set frozen.** `\0`, `\xNN`, `\u{...}` and octal escapes reconvene this
   panel; the ffi judge holds a standing veto on them (interior NUL).
4. **Implementation constraints** (the engineer's conditions, adopted): no
   new `TokenKind` variant, no new `LexState` field; the token keeps its
   `Str`/`Char` kind on a bad escape — poisoning it to `Error` would delete
   the line's `Terminator` (panel 007's ender set) and turn one mistake
   into two errors; decoding is a pure `unescape(src, span) -> String`
   landing in the same commit as validation, with tests asserting byte
   values (the snapshot format cannot see them).
5. **The char contract is restated**: exactly one character means one ASCII
   byte **or** one escape sequence — `'\n'` is four source bytes and one
   character. Source length stops being the rule.
6. **Spec untouched**: v0 frozen (standing); the escape line waits for the
   v1 package and a measured count.

## Recorded, not resolved

- **The token-count methodology is suspect.** The warden's second estimator
  (BPE proxy) puts spec v0 at ~1859, not the ~1496 every budget verdict
  since panel 002 has assumed; the 1.33 tok/word heuristic is a prose rule
  applied to a document that is ~14% punctuation by character. No tokenizer
  or API key exists on this machine. Author's decision (2026-08-04):
  `heroes measure` stays at M3 as planned — the v1 package is blocked on
  the baseline anyway. Until it runs, every budget figure in this repo is
  explicitly provisional. On the watch list.
- **The Pascal asymmetry** (historian): design.md cites `WriteLn` as
  `print`'s fifty-year precedent but took only its newline half. Pascal
  pairs it with `write` (no newline) and out-of-string `#10`; Oberon-07
  uses `0AX`. Heroes has neither, which made it strictly weaker than its
  own cited precedent — escapes close the gap. Recorded as a known wart.

## Correction, found during implementation (2026-08-04)

The ergonomist's finding 5 and the resolution's rationale both claimed
Amendment D "turns `"C:\temp"` and `"\d+"` from silent bytes into
diagnostics". **Half of that is false, and the half that fails is the
example everyone reaches for.** `\t` is a legal escape, so `"C:\temp"`
produces no diagnostic at all — it silently becomes `C:<TAB>emp`. Only
`"\d+"` and `"C:\Users"` are loud.

This is the residual silent trap that C-style escapes carry in every
language that has them, and it is not fixable by tuning the escape set: the
standard remedy is raw string literals (Go's backquotes, Rust's `r#"…"#`,
Swift SE-0200), which are out of scope for v1 and would need their own
panel. Pinned by `the_residual_windows_path_trap_is_on_the_record` in
`crates/heroes/src/lexer/tests/literals.rs` so the limit stays visible
rather than being rediscovered as a bug.

## Predictions to score

- ergonomist: ≥45pp compile-vs-tests-pass gap on newline-in-string tasks
  under a silent spec — **at the baseline**.
- spec-warden: (1) real count of spec v0 lands at 1650–1900 — **when
  `heroes measure` runs**; (2) ≥60% of multi-line-output completions use a
  C escape, <5% outside the set — **at the baseline**.
- compiler-engineer: `scan.rs` ≤400 lines, `mod.rs` still 130,
  `kind_name` still 59 arms — **at M1's amended close**.
- ffi-pragmatist: ladder rungs 1–3 need zero shim for string arguments —
  **at M7**.
- historian: `\\` occurrences in the self-hosted compiler outnumber all
  escapes in user-facing examples — **at the fixpoint**.
