# The reader that forgets the level

2026-09-17. Panel 160, on defect 050: `.is_err()` applied to a value that is
fallible twice answers *was the key there* while the line reads *did the stored
value fail*. Five options were on the ballot and the sitting adopted **E** — the
refusal of that one reader, and of no other.

## Why one reader and not four

Of the four readers of a fallible value, three hand back a value that still
carries the second level. `.must()` and `?` give an `i64?`; `.default(v)` types
`v` against the payload. A wrong depth therefore fails at its next use, in the
type. **`.is_err()` returns a `bool`**, and the level vanishes from the line —
it did not even bind the payload in the arm that types it.

So the asymmetry is exactly one reader wide, and that is what the sitting
refused. Refusing all four — option A, the complete form — was measured and
rejected: it refuses correct programs, including a golden that reads two levels
deliberately and a use of `?` on a nested value that the ffi seat wrote itself.

## The dissenting seat's condition was met by the option it dissented from

The ffi-pragmatist approved A over E on one program: a SQLite pool where
`hit.must().default(nullptr)` closes a connection that never existed, at exit 0.
It conditioned approving E on that shape being refused some other way.

**E refuses it.** That seat built one prototype — A — and read E's reach from the
brief rather than from a run. The program's fourth line is `if hit.is_err()` on a
`Db??`. Run over all thirteen of that seat's probes on the compiler-engineer's
own `heroes-E`: **E and A differ on exactly two, and both favour E.** One is a
correct program A refuses. The other is the single mistake E lets compile, and it
is not silent — it aborts at 134 with *given back that were never taken*, the
runtime ledger doing its job.

A seat that builds its own prototype measures its own option precisely and the
alternative from prose. That is a gap in how a sitting is briefed, not a failure
of the seat, and it is the second sitting running where the decisive fact was
about the option nobody built.

## What the sitting found that was not on its ballot

**The specification carries a sentence the runtime falsified two days earlier.**
§ 13 says the owing is counted, *so a handle consumed twice hides one never
consumed*. On 2026-09-15, `2e7d221c` made the counter a set — its own subject
says so — and the golden asserts the set's message. The brief handed that clause
to five seats as fact. Defect 054.

**`design.md` strikes `has(m, k)` on a spelling this resolution removes.** Under
E a `{K: V?}` map has no one-line presence test. Defect 055, and the historian
predicts the request for a distinct outer-level spelling arrives next — Go and
Kotlin both gave that question a name of its own.

**A generic body reads the outer level of whatever it is instantiated with**, and
neither option closes it: the body is checked once with a generic payload and
instantiation happens after the checker. Zero live instances. Defect 056.

## The critic did not sit

It died on a session rate limit after one tool call. What it was convened to
settle — the P2d question above — the coordinator measured instead, and the
answer inverted the only dissent. The four sittings before this one each recorded
the critic finding something no seat had seen, so its absence is a missing
instrument and the synthesis says so rather than passing over it.
