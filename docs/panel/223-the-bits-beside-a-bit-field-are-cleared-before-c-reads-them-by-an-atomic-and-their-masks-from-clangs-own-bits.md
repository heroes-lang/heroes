# Panel 223: the bits beside a bit-field are cleared before C reads them, by an atomic `and`, their masks from clang's own bits

**Convened 2026-10-17 at about 16:55, synthesised the same evening**, on defect 2099
(https://github.com/heroes-lang/heroes/issues/2099), by the author's answer that afternoon,
*convene the two sittings now*, with M-bounded-seed's tag waiting on it. **Lane: soundness**
(compiler-engineer, ffi-pragmatist, a completeness critic before the seats and after them). **Resolution:
provisional, author ratification pending.** Briefs: `docs/panel/223-briefs/`, repaired after the
critic's first pass (`docs/panel/223-reports/critic-briefs.md`, twelve repairs). Reports:
`docs/panel/223-reports/compiler-engineer.md`, `ffi-pragmatist.md`, `critic.md`. Every number below is
a seat's or the critic's, with its command in its report.

## The proposal, verbatim (the shared brief's question)

> Should those bits be zero where C receives them, and how?

*Those bits*: the bits of a byte a named bit-field holds that no named bit-field holds, which panel
215's R1 leaves as they were, since it writes whole bytes and never a byte a member holds. Routes A
(a mask per shared byte, read-modify-write), B (rebuilt through a zeroed temporary), C (left open),
D (refused) were the coordinator's; five more the critic's first pass (masks from the dump's bit
column, masks from a static object, B over the bit-field run, zeroing at the source, two routes for
the lend and the store).

## What was measured

- **The defect on the landed compiler** (compiler-engineer): MemorySanitizer in
  `heroes-linux-arm64:latest`, 1/0/0/0/0 reports over the five levels on panel 215's `pn.hero`, at
  its line 76, *uninitialized bytes at offset 1* of `struct bf`. **In real headers** (ffi-pragmatist):
  3 records of the macOS SDK and 29 of the Linux image hold such a byte, libcurl's
  `struct curl_hstsentry`, glibc's `struct obstack`, APR's `apr_uri_t` and Linux's `tcp_info` among
  them, floors since many headers do not compile alone.
- **R1 already writes inside C's own access unit** (both seats, the critic's reading run): on the
  Windows box the landed compiler zeroes bytes 6 and 7 of `struct bf`, which clang's store of `c`
  covers as a 32-bit access of bytes 4 to 7; `struct gap`, whose byte 1 only an unnamed bit-field
  holds, lies inside a 24-bit access on all four legs. A zeroing write changes no member's value,
  so no member loses one; the premise *never a member's byte* was never what kept R1 safe.
- **A plain read-modify-write loses C's writes** (compiler-engineer): a Heroes program the landed
  compiler accepts lends the record to a C library that kept its address and writes the bit-fields
  under its own lock; the plain `byte &= mask` changed 1,764 and 6,252 bit-field values on this Mac
  and about 3.85 million on the box, two runs each; an atomic `and` per byte changed 0 on three legs.
  A Heroes program runs threads (`hero_thread_spawn`, `runtime/hero_os.h:414`; the run golden
  `fixedbugs-396-two-threads-lend-at-once`), the critic found, against the ffi-pragmatist's premise
  read from the spec alone.
- **The masks' source** (the critic, all four seats' claims reproduced): `m.F--` on a zeroed object
  compiles with 0 diagnostics under the project's flags for nine bit-field kinds on the four CI
  targets at `-O0` and `-O2`, and its masks equal a check that writes no bit-field, byte for byte, on
  all four legs and the box; the `-1` and `~m.F` spellings are refused by the flags, as the
  compiler-engineer measured; on `powerpc64` the layout dump's bit numbers are wrong, as the
  ffi-pragmatist measured, and the compiler-engineer's little-endian `_Static_assert` fails there.
- **A `const` bit-field** (the critic): `m.F--` refuses to build on it, and an atomic `and` would write
  its byte; both seats missed it.
- **Cost of the route as built** (compiler-engineer, masks from the dump): +92 code lines over five
  modules of `selfhost/emit/`, +58 runtime lines, nothing in the checker, the IR or the lowering; +6 ns
  a lend of a bit-field record on this Mac, about +8 ns on the box (wall clock), 0 on other records;
  its own tests 1720, `determinism` 574/0, one emission moved beyond its key line,
  `run-fixedbugs-156-records-beside-bit-fields-build.c`. `padding.hero` stands at 300 of its 300 code
  lines.

## Verdicts

| seat | verdict | condition |
|---|---|---|
| compiler-engineer | **approve A with an atomic `and` per shared byte**; **veto plain A and B** (design.md §1.12, the measured lost updates; B an inference, unbuilt); object C, D, static-object masks, the source route, all-ones masks | `padding.hero` split, no ceiling raised; the byte-printing probe a `run/` case; every moved emission line classified by script; the rule sentence rewritten; and his own objection: an atomic `and` beside C's plain write is a data race by C11's letter, the route resting on the hardware as measured |
| ffi-pragmatist | **approve A** with four conditions; B over the run equivalent; object B, C, static-object masks; **veto D** (§1.11, §4.19: 32 records would need a C shim) and **veto masks from the dump's column** (big-endian) | masks from clang's own bits; a `const` bit-field's byte left alone; the write through `volatile unsigned char *`; the rule sentence names what remains |
| critic | no verdict | the two vetoes are compatible: atomic and `volatile` compose (0 diagnostics, four targets, both levels); MSan cannot judge an atomic route, a value probe and a no-write check can; the question unasked: where the dirty bits come from, counted, since zeroing at the source alone needs no atomic |

## Resolution (provisional: author ratification pending)

**R1, route A as both vetoes allow it.** Before each lend to C and after each store of a whole group
record (R1's two sites, panel 215 and defect 2112), in each byte a named bit-field shares with bits no
named bit-field holds, those bits are cleared by an atomic `and` through `volatile unsigned char *`
(`__atomic_fetch_and`), so a bit a named bit-field holds is never written and a C thread writing the
bit-field under its own lock loses nothing (the compiler-engineer's veto of the plain form). **The
masks come from clang's own bits** (the ffi-pragmatist's veto of the dump's column): for each named
bit-field, `m.F--` on a zeroed object in the unit's constructor before `main`, its name guarded as a
whole member's is, the runtime OR-ing the object's bytes; no endianness is assumed. **A byte any
`const` bit-field holds is left alone** (the critic). A byte no bit-field holds is R1's, unchanged.

**R2, the rule sentence**, in `.claude/rules/generated-c.md` § Arithmetic and memory, says the two
sites, the atomic clear, and what stays: an atomic `and` beside C's plain write to the same byte is a
data race by C11's letter, measured to lose nothing on three legs; R1's zeroing write already lands
inside a bit-field's access unit on the MSVC target (bytes 6 and 7 of `struct bf`) and on all four
legs for a byte only an unnamed bit-field holds, changing no member; a library version that adds a
bit-field where the binding's header had padding has those bits cleared; a `const` bit-field's byte
is left as it was.

**R3, refused**: plain A, B and B over the run (§1.12, the lost updates); D (§1.11, §4.19, and outside
this lane, a diagnostic class); masks from the dump's column (wrong on a big-endian target); C, since
29 records of the Linux image and libcurl's reach C dirty today. **What conservative would have
been**: the compiler-engineer's built route as it stands, masks from the dump's column under a
little-endian `_Static_assert`, sound on every CI leg and refusing on a big-endian one; the author
can choose it.

**R4, the landing owes**: `padding.hero` split at a seam under its ceiling, none raised; the
byte-printing probe as a `run/` case with a `const` bit-field shape in it; every moved emission line
classified by script; the full net, the seed's two-stage build and the platforms by the optimistic
chain.

## Predictions to score

| seat | prediction | instrument | when |
|---|---|---|---|
| compiler-engineer | the re-blessed `emission` moves exactly the emissions whose span tables hold a bit-field's span, 1 of 1,299 (`run-fixedbugs-156-records-beside-bit-fields-build.c`), beyond the key line, and no line outside the classes of his condition 3; and no module of `selfhost/emit/` over its ceiling, no `DECIDED` row added | `emission` before re-blessing against `grep -l -E 'hero_pad_t_[0-9]+\[\] = \{[^}]*\) \+ [0-9]+, [0-9]+' tests/emission/`; `layout` | the landing commit; the critic notes the line classes must be restated for masks from `m.F--` |
| ffi-pragmatist | a Heroes program binding libcurl's `struct curl_hstsentry` (`partial`: `name`, `namelen`, `expire`), stored from a by-value return and lent, runs MemorySanitizer-clean at `-O0` and `-O2` in `heroes-linux-arm64:latest` with no change to its `extern` group and no shim, where R1 reports at offset 16 of 40 | MSan in the container | M-bounded-seed's close or the next batch with libcurl in the image; the critic: MSan cannot judge an atomic route, so a value probe beside it |

## Process notes

- The critic's first pass repaired twelve framing facts before any seat started, among them a
  `records/tagged` sentence that named a tag the coordinator had removed, citations to the wrong
  lines, and a premise, *never a member's byte keeps R1 safe*, that both seats then measured false.
- The seats' copies were inside the repository's root (`.claude/worktrees/223-<seat>/`), the
  critic's first finding being that the scratchpad stands outside it, against CLAUDE.md's hard stop.
- No historian sat; no prior art is claimed.

## Author's verdict

Pending.

Asked for by https://github.com/heroes-lang/heroes/issues/2129, naming the sitting as `panel 223`, with its question and the
recommendation.

**RATIFIED, 2026-10-17**, R1 to R4 as written above, the author answering in conversation that evening, meant as *I ratify everything*, on the coordinator's list of the five open decisions, each with its recommendation: the atomic `and` with masks from
clang's own bits, as landed at `2365c4787`; the conservative route recorded (masks from the dump's
column under a little-endian `_Static_assert`) is not taken. Recorded as a reading (CLAUDE.md § 4).
The author may overturn it.
