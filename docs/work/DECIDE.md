# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item here asks **what should be true**, and until it
is answered the compiler goes on behaving some way by default. That default is
the cost of leaving an item open, so each item names it.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/work/DONE.md`. A file that keeps its
own closed items stops being a list of what is owed — this one held **138 ticked
items and zero open ones** on 2026-08-26, under eighteen headings still titled
`## Open`, which is why the rule now lives in `/decide` itself instead of in
prose about `/decide`.

**One notation: `- [ ]`.** No section headings, no prose sections, no
strikethrough. A finding written as a bare bullet is invisible to every count in
this project, and nine of them sat in exactly that shape under two panel
headings here until 2026-08-26 — **five already closed** and two of those
measurably stale, while the file reported itself empty.

Two rules bind this file, both learned the hard way:

- **Verify before asking.** An entry is a claim from the day it was written, and
  entries outlive their causes. Asking the author about a settled question is the
  one cost this list cannot pay. Of the nine findings recovered on 2026-08-26,
  five were closed and were ticked with what closed them rather than asked.
- **Rank by what it blocks**, never by age, and say the blocker in the question.

Format: `- [ ] <origin> | <what> | <where to look> | <why it matters>`





- [ ] **SOUNDNESS-LANE BRIEF instructed 2026-08-28, answers `2a` and `3a`, and it now carries THREE questions because they are one subject** | All three are `extern` arguments at a **variadic position**, so they go to one sitting (compiler-engineer + ffi-pragmatist: no surface, no diagnostic class for the first, no spec token). **(1) A literal format string, which is the one that matters.** Measured 2026-08-26 and again 2026-08-28: `printf(format: "%d\n".cstr(), value: 4294967303)` with `value: i64` prints **`7`** at exit 0 — the silent wrong answer this language exists to kill. `-Wformat` cannot see it because the emitter passes the format as `hero_cstr_nonnull(t2)`, never as a C literal; `-Wformat-nonliteral` is NOT the lever (100% false positives, since the format is never a literal by construction). What works, compiled: written `printf("%d\n", v)` the project's own `-Werror=format` says `format specifies type 'int' but the argument has type 'int64_t'` and names the fix. Proposal: where an argument to an `extern` is `.cstr()` applied directly to a string LITERAL, emit the C literal. Blast radius measured: 19 call sites, 18 blessed emissions. Owes two answers: the lifetime of a C literal's pointer against a `HeroStr`'s, and whether any of the 19 is a format function bound wrongly. **(2) A correct binding refused, and the coordinator's first reading of this was WRONG** — it was offered to the author as a message fix and it is not. `function printf(format: cstr, value: f32)` is refused as *"declared narrower than the header's `double`"*; the header says `...`, not `double`. Measured 2026-08-28: `clang -std=gnu11 -Wall -Wextra -Werror` on the equivalent C is **exit 0** and prints `0.500000`, and the warning Heroes reads it from is **`-Wdouble-promotion`, which is not in `-Wall -Wextra`** — the project opts into it. At a variadic position the float-to-double promotion IS the C ABI and is exact, which `emit_ffi_narrowed.hero`'s own test comment already says. So this is a REFUSAL to remove, not a message to reword, and removing a refusal is the sitting's call. **(3) Arity-1 `printf` is exit 2, the compiler blaming itself**: `function printf(format: cstr)` gives `internal error: compiling the generated C failed` from `-Wformat-security` on the PROBE. This one IS a diagnostic class — it belongs in CLAUDE.md §7's exit-1 family beside the four that already recover a name and blame the author's line — so if the lane cannot take it, it needs the full panel. The cheap repair (silencing the pragma inside the probe) was tried 2026-08-26 and REVERTED with the measurement: it moves the error onto the author's own call site where the warning is CORRECT and must stay, because a format string the author does not control is `%n` writing into memory (§1.12's own subject) | selfhost/emit_body.hero · selfhost/emit_literal.hero · selfhost/emit_ffi_narrowed.hero · selfhost/emit_extern_probe.hero:118-125 · selfhost/cli_flags.hero:32 · docs/panel/094 | §1.12 ranks a silent wrong answer above everything else in the contract, and (1) is the only repair that moves this one into the loud column

- [ ] **FULL-PANEL BRIEF, instructed 2026-08-26 and RE-INSTRUCTED 2026-08-28 (answer `8a`) — the author has said convene it twice, and it has not been.** Re-verified 2026-08-28 rather than recalled: `n: i32 @ 7` then `n.to_i64()` is typed `i64?`, proved by `to_str` refusing it — `error[bad_operand]: to_str takes an integer, a float, bool or str, found i64?` | **A widening that cannot fail should not be fallible.** Measured: `n: i32 @ 7` then `n.to_i64()` is typed `i64?` — an `i32` always fits an `i64`, so the error case cannot occur and the type says it can. Booked by panel 051's ffi-pragmatist at **251 call sites**. The proposal in one line: `to_<wider>` on a value that provably fits returns the value, and only a narrowing returns `T?`. **What the sitting must settle and what makes it a language change rather than a repair**: the spec's own sentence is *"`to_i8` … `to_u64` give a `T?`, because the number may not fit"* (spec:61-64), which is a rule stated by NAME rather than by fit — so the amendment is a spec edit and the warden prices it. The compiler-engineer owes the widening lattice: which of the 64 ordered pairs are infallible (signed to wider signed, unsigned to wider unsigned, unsigned to strictly wider signed — and NOT unsigned to same-width signed). The llm-ergonomist's question is whether a rule that depends on the pair reads worse than a rule that depends on the name. **The cost of leaving it**: §1.3 says locality is the currency and fallibility is what the language asks the reader to read; teaching `.must()` where it is free teaches it where it is not | selfhost/check_builtins.hero · spec:61-64 · docs/panel/051 | the one thing the language asks a reader to notice is the `?`, and 251 sites currently teach them to stop noticing

- [ ] **FULL-PANEL BRIEF, instructed 2026-08-26 and RE-INSTRUCTED 2026-08-28 (answer `9a`), and it carries panel 092's `size_t` question because both spend the same spec budget.** Re-verified 2026-08-28: `grep -i 'round.trip|shortest'` over the spec returns NOTHING, and `runtime/parts/f64.c:26` documents it in full. **ONE CORRECTION TO THIS ITEM, found while verifying**: its witness `5e-324` is NOT writable in Heroes — `error[exponent_literal]: 5.0e-324 is not a number in this language — there are no exponents`. The witness is a fact about the C runtime, not a Heroes program, and the brief must say so or a judge will try to compile it | **The spec never says what guarantee `print` gives a float, and the runtime already documents the answer.** `runtime/parts/f64.c:26` says it in full: *"ROUND-TRIP-EXACT, never shortest: %.*g at increasing precision until strtod() reads back the same bits"* — and names its own witness, `5e-324` rendering as `4.94065645841247e-324`, which round-trips and is **not** the shortest decimal, where Python prints `5e-324`. Java shipped `1.9999999999999998E23` for eighteen years while satisfying its own round-trips spec. The document says only *"A float prints a point or exponent (`1.0`, `1e-06`)"*, from which a reader infers *shortest*, which is false. **The item that asked this had the premise INVERTED** (it said `5e-324` renders shortest) and cited `runtime/runtime.c (hero_f64_render)`, which is neither the file nor the function — corrected here from the source. **What the sitting buys**: one sentence naming round-trip and refusing *shortest*, priced by the warden against headroom 504. **AND IT CARRIES A SECOND QUESTION, because both are spec and the tokens come from one budget**: panel 092's `size_t` mapping row cost **+12** against a prediction that scored FALSE on 2026-08-26 — two llm-ergonomist arms convert at a size argument either way (10 of 10 without the row, 9 of 10 with), so the row did not buy the behaviour it was sold on. What it measurably buys is **declared UNDECIDABLE points dropping 9 to 5** and the misattribution the control arm named in its own words: without the row a wrong width *"would be blamed on the compiler (exit 2), not on me"*. The sitting decides whether that is the same 12 tokens' worth under a different argument, or whether the row goes back out | spec:180-181 · runtime/parts/f64.c:26 · spec:204-205 · docs/measurements/010-spec-budget-ledger.md rows 3560 and 3592 · docs/panel/021, 027, 092 | §12 makes a false spec the compiler's bug, and a reader who infers *shortest* from two examples has been told something untrue by omission








