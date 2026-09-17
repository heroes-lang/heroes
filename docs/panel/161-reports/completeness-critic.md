# Panel 161 — completeness critic

I give no verdict on the question. My job is what is missing.

Every number below was produced by a command run on 2026-09-17/18 in a COPY of
the tree at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/4cbbc5a6-a98e-4b44-80b5-b6957110cd39/scratchpad/cc/tree`
(seed built there from C alone, `/usr/bin/time -p clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` → `real 3.13`), or in
`…/scratchpad/abi`. The repository was read and not written outside
`docs/panel/161-reports/`. **No number here is carried from a brief or from a
report**; where I restate a seat's figure I say it is theirs.

---

## Contradictions

### C1. Route 3 and route 5 are both priced at an implementation nobody measured — the compiler-engineer is right, and the spec-warden's sketch has the same hole

The shared brief prices route 3 as a change at `extern_field.hero:154` "and the
corresponding scalar path". The compiler-engineer called that **measured false**
and reported six clang errors. The ffi-pragmatist says its route-3 build was made
by taking `--emit-c` output and rewriting it, *"the four temporaries change type
and the field `_Static_assert` goes, which is **exactly and only** what route 3
relaxes"*.

**Both cannot be true, and I settled it.** The emitter types the array literal's
elements and the field read as `int8_t`, independently of the member's C type:

```
./heroes build tests/golden/run/ffi-a-char-array-member.hero --emit-c \
    --include tests/golden/run > /tmp/cm161.c
grep -n "int8_t t" /tmp/cm161.c        # int8_t t1, t2, t3, t4 (the literal), int8_t t22 (the read)
```

and compiled with the compiler's own fifteen flags (`selfhost/cli/flags.hero:53-69`,
read this session) plus `-funsigned-char`, which is what the Debian arm64 ABI
looks like to clang:

```
clang -std=gnu11 -g -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -Wall \
  -Werror=return-type -Werror=uninitialized -Werror=format \
  -Werror=conditional-uninitialized -fno-strict-aliasing \
  -fno-delete-null-pointer-checks -Werror=shorten-64-to-32 \
  -Werror=sign-conversion -Werror=incompatible-pointer-types-discards-qualifiers \
  -Werror=incompatible-pointer-types -funsigned-char \
  -I runtime -I tests/golden/run /tmp/cm161.c runtime/runtime.c -o /tmp/cm161
```

**6 errors, mine, counted with `grep -c "error:"`**, and by kind:

| count | error |
|---|---|
| 4 | `implicit conversion changes signedness: 'int8_t' to 'char'` — the construction |
| 1 | `implicit conversion changes signedness: 'char' to 'int8_t'` — the read |
| 1 | `static assertion failed … heroes-ffi-field Tag name` |

**The compiler-engineer's measurement is confirmed and the ffi-pragmatist's
scoping is falsified — by the ffi-pragmatist's own method.** Retyping "the four
temporaries" *is* the emitter change (`emit/storageless.hero`, `emit/aggregate.hero`,
`emit/container.hero`); they hand-performed it and then priced route 3 as though
only the assertion moved. Their § 10 condition names four sites —
`extern_field.hero:154`, `:261`, `extern_assert.hero:72-78`, `c_spellings.hero:59`
— and **not one of them is the code that types those temporaries**. A coordinator
who implements route 3 from that condition ships the compiler-engineer's
prediction 1: five `-Wsign-conversion` errors on a `#line` pointing at an ordinary
statement, i.e. **exit 2, internal error**.

**And the same hole is in the spec-warden's route 5.** Its § "Where the mechanism
lands" names two places — the sign conjunct at `extern_field.hero:154` and
`c_spellings.hero:59`, *"defect 059 closed in one line"*. Against my six errors
that ships five of them. The warden's **+21 real is a correct price for the
sentence and is not a price for the change**; the compiler-engineer's ~60-90
lines across five emit modules is the price for the same route. The two seats are
not disagreeing about a number, they are measuring different quantities and only
one of them says so. **Nothing in this sitting has measured route 5's
implementation except the compiler-engineer, and its own estimate is an estimate.**

Settling command: the two above, on any leg, with `-funsigned-char`.

### C2. The spec-warden's stated condition FIRES, and the population it was tested against was selected to exclude the counterexample

The warden: *"My objection to route 3 lifts if a seat shows a program whose
correct spelling is `i8` against a plain `char` on some leg… **Nothing in the 39
measured fields is such a case.**"*

Two things, and the second is the one that matters.

1. The ffi-pragmatist measured exactly one: `struct elf_prpsinfo.pr_nice`, a nice
   value, `−20…19`, read as `−20` under route 3 on every leg and as `236`
   under a byte rule. **The condition as written is met. Say so plainly in the
   synthesis: the warden's objection to route 3 lifts on the warden's own terms.**
2. **The 39 fields are 23 + 16 ARRAY fields** (the brief's own table reports 0
   scalars in both walks). The warden tested a condition about scalars against a
   population containing no scalars. That is not a near miss; it is a set that
   could not have contained a counterexample.

**The half no seat stated, and it decides route 5.** Route 5 fixes plain `char`
at `u8` on every target, so `pr_nice` reads `236` and `i8` is refused on all four
legs — which is the property the warden calls the route's whole gain. Getting
`−20` back then needs a bit-preserving reread, and **the specification does not
have one**:

```
grep -n "to_i8\|to_u8" spec/heroes-spec.md        # § 11, lines 294, 305-306
```

> `to_i8` … `to_u64` give a `T?`, **because the number may not fit**.

`to_i8(236)` therefore fails; the value comes back only through hand-written
two's-complement arithmetic (`to_i64`, subtract 256, unwrap). That is precisely
the *"bit-preserving reread that does not exist"* the warden charged **R1-full
+104 real** to add. **R5-min at +21 is priced for text buffers and does not
answer the one real field the sitting measured.** Whoever adopts route 5 owes
either that draft's real price or a sentence saying the scalar position is out of
scope.

Settling commands: the grep above (run); and, unrun, a two-program compile
reading `pr_nice` at `u8` and at `i8` on the arm64 leg.

### C3. The brief's "0 plain-char scalar fields" is right about its nine headers and wrong as the heading three seats read

The brief's number is **not** wrong: `sys/procfs.h` is not one of its nine
headers, and it does not exist on this Mac at all (`ls /usr/include/sys/procfs.h`
→ *No such file or directory*, run this session). What is wrong is the brief's own
heading over the number — *"The scale, measured — and it is arrays, not
scalars"* — which generalises a nine-header walk into a claim about the world.

**By how much: 0 → at least 4 scalars and 1 → at least 2 parameters**, on the
ffi-pragmatist's wider walk (their number; I could not re-run it, see § What I
could not check). The correction moves three seats' reasoning, and I name where:

- **compiler-engineer**, scope of route 5: *"The scale says that is the right
  scope: the brief's own AST walk counts **1** plain-`char` parameter and **0**
  results against **23** array fields."* The decision to leave the parameter and
  `@`-pointee positions refused rests on a count taken over a population that
  excluded the scalars — and the scalar position is exactly where the historian's
  precedent says the cheap answer is unsafe.
- **historian**, the central steer: *"the brief measured **0 scalars, 23
  arrays**… Rust's discussion closed on precisely that seam."* The seam is still
  there — arrays dominate — but it is not clean, and an argument that rests on a
  zero is weaker when the zero is a few.
- **spec-warden**: C2.

Three of five seats carried a brief number without re-measuring it. **CLAUDE.md
§ RUN IT says a number is measured in the session that writes it**; the shared
brief's own § warns that its first walk returned a wrong answer and was caught by
re-reading the instrument. It was right to warn.

Settling command: the ffi-pragmatist's `ffi/walk.py` re-run over the widened
header set on each leg, with the header list printed beside the count.

### C4. The two seats that predict the net disagree about what is being landed

The warden predicts 1825/3 → **1828/0 with the golden case's field changed to
`u8[4]`** and panel 081's recorded refusal corrected underneath. The
ffi-pragmatist predicts 1825/3 → **1828/0 with the golden case unchanged at
`i8[4]`**. Both cite the same three failing tests. They cannot both be the
resolution, and `determinism`/`emission` goldens are byte-compared, so the
synthesis must say which file lands. Settling command: whichever patch is chosen,
`./heroes run tests/harness/main.hero -- ./heroes run determinism emission` on
the arm64 leg. **Unrun by anybody, including me.**

---

## Unmeasured claims

### U1. The historian's handed probe was NOT answered by the ffi-pragmatist. I ran it

The historian handed one question to "whichever seat owns the C boundary":
**whether AAPCS64 §6.4.2's parameter-extension rules make the sign of a ONE-BYTE
ARGUMENT observable across a TU boundary**, and called it *"the single
highest-value thing another seat could measure"*.

The ffi-pragmatist's § 5 answers a different question — whether the *field's*
Heroes spelling is an ABI question (it is a read, not a layout) — and its § 6
answers which *spelling* is accepted at a parameter. **Neither is the extension
question.** It stood unanswered.

Measured, `clang -O2 -S`, two triples, this Mac:

| leg | callee of `int callee(char c) { return (int)c; }` | argument sign observable across TUs? |
|---|---|---|
| `aarch64-unknown-linux-gnu -funsigned-char` | `and w0, w0, #0xff` then `ret` | **no** — the callee re-narrows |
| `aarch64-unknown-linux-gnu -fsigned-char` | `sxtb w0, w0` then `ret` | **no** |
| `arm64-apple-macos11`, either flag | `ret` alone | **yes** — the callee trusts the caller |

and at the return position (`char give(void); return (int)give();`): Linux
aarch64 emits `and w0, w0, #0xff` **after** `bl give`; Darwin arm64 emits nothing.

Executed, not only read, on Darwin arm64 — one callee object, two callers:

```
clang -O2 -c -funsigned-char caller.c && clang -O2 -c -fsigned-char callee.c
clang caller.o callee.o -o mix1 && ./mix1     # 200
clang -O2 -c -fsigned-char caller.c && … ./mix2  # -56
```

**The same machine code returns 200 or −56 according to how the OTHER
translation unit was compiled.** So: on the generic AAPCS64 leg the receiving
side re-imposes its own type in both directions and the sign is *not* observable
across a TU boundary; on the Apple leg it *is*.

**What that does to routes 5b/6 (`-fsigned-char` at the clang invocation), which
the historian called inadmissible until somebody ran this:** the only leg where
the flag disagrees with the platform default is Debian arm64 — and that is
exactly the leg where the receiver re-narrows, so the disagreement is
unobservable at the argument and return positions. On Darwin, x86-64 and (per the
historian's `/J` citation, documented not measured) Windows, `-fsigned-char`
*agrees* with the default, so nothing diverges. **The historian's condition 2
resolves in 5b's favour at the position it was about.** That is the single
largest thing the five reports left on the table.

Two limits I state rather than hide: this is **clang's codegen, not a reading of
AAPCS64 §6.4.2** — the historian asked for the document and I measured the
implementation; and it covers a byte in a register, not a `char` inside a struct
passed by value (layout, where sign has no effect).

### U2. Route 5b's remaining hazard is unrun, and it is now the only thing left

The compiler-engineer named it and did not run it: whether any bound header has
`static inline` code whose behaviour depends on plain `char`'s sign, compiled
into the Heroes TU under the flag while the shipped library's copy was compiled
the other way. **Cheapest instrument, and it is the historian's prediction 3
already written as a command**: rebuild the corpus's 20 `extern` programs with
`-fsigned-char` added to the clang line and diff the suite — `corpus` reads 53
passed / 0 failed today, so any red is attributable. Not run here.

### U3. Two things about 5b that strengthen it and that no seat measured

Both read this session, not inferred:

- **`unsignedness_of` is not a separate clang probe.** `selfhost/emit/extern_field.hero:261`
  builds a `_Generic` row — `char:((char)-1 > 0)` — that is evaluated **inside the
  emitted translation unit**, and I see it in the emitted C (`HERO_C_UNSIGNED`,
  `/tmp/cm161.c:35`). Under `-fsigned-char` it therefore answers consistently with
  the flag by construction. The compiler-engineer's worry about probe flag lists
  applies to `cli/pointee.hero`'s AST-dump unit, not to the field path.
- **Defect 059 closes with zero edits under 5b.** `selfhost/emit/c_spellings.hero:59`
  is a static table: `if c_type == "signed char" || c_type == "char"` → `i8`,
  `no_caveat`. It is not wrong code — it is a sentence that is false on one leg,
  and the flag makes it true on all four. Every other route must *change* it.

Neither fact appears in any report. Whoever prefers route 5 over 5b should do so
knowing 5b closes **058 and 059 with one string**, which is what the milestone
asks for.

### U4. Claims still standing as claims

- **The Windows leg is unrun** and every route on the table is priced against
  three data points. Both the historian and the compiler-engineer say so; the
  synthesis must repeat it, because `/J` is a document and not a measurement.
- **1825 → 1828/0** is predicted twice and run by nobody (C4).
- **The spec deltas** are the warden's; I did not run `heroes measure --refresh`
  and do not restate them as mine.

---

## Routes nobody listed

The brief named four, two seats found the fifth (byte / relax the sign conjunct)
and two the sixth (`-fsigned-char` at the invocation). Naming six is not a claim
that six is the set.

### R7 — split the rule by POSITION. Nobody proposed a non-uniform answer, and the evidence is non-uniform

Every seat proposed one rule for all four positions. The historian's central
finding is that Rust's route-3 proposal was withdrawn on exactly that seam —
*"I was too much in the mode of thinking of pointers to `c_char`"* — meaning
route 3 is safe for pointers/arrays and unsafe for scalars and parameters. My
U1 measurement says **why**, in machine code: at the array/field position the
byte is storage and the sign is a pure read; at the parameter/return position the
sign can reach the calling convention and on one of the four legs it is
observable across a TU boundary.

So the route nobody costed:

- **arrays and fields of plain `char`**: the byte rule. The declaration governs
  the value, with the boundary conversions cast (the compiler-engineer's
  mechanism), and if one spelling is wanted it is `u8` (the warden's sentence).
  This is where 39 of the measured positions are and where no ABI question
  exists.
- **scalars, parameters and results of plain `char`**: a *different* answer is
  admissible here, and it is the position where the measured counterexample lives
  (`pr_nice`, C2), where the precedent warns, and where the sign is not merely a
  read.

It is not a compromise and it should not be adopted as one: it is the shape the
population and the precedent both point at, and it is the shape three of the
historian's own "systems that refused" encode deliberately — Fortran's `C_CHAR`
being a CHARACTER kind with no INTEGER kind for plain `char`, CFFI's *"if you
want small integers use signed char or unsigned char"*, Nim's `cchar` being
Nim's `char`. **Cost: unmeasured. That is the gap, and it is one draft and one
`--refresh` away.**

### R8 — defects 058 and 059 are separable, and nobody asked whether they need one resolution

`docs/work/DEFECTS.md` reads `**OPEN: 2**` (read this session): 058 is *no
portable spelling*, 059 is *the diagnostic names the spelling it just refused*.
Every seat treated 059 as a rider on whichever route wins 058. It is closable
alone, on any route, by making `c_spellings.hero:59` answer the sign the emitted
TU will see instead of the constant `i8` — and under 5b it closes by becoming
true (U3). Worth saying in the synthesis because the milestone's gate is *both
defects closed*, not *one resolution*.

### R9 — a variant worth one measurement: let the GROUP state the assumption

Route 2 moved from the field to the `extern` group: the group says once which
sign it binds plain `char` at, the compiler asserts it against the target, and
the field lines stay `i8`/`u8`. It is the only shape that keeps the property the
llm-ergonomist says route 3 destroys — the binding remains evidence about the
header — without minting a type or a per-field word. I name it unpriced.

---

## The question the sitting should have asked and did not

### Q1. The array→`str` gap is real, it is a LANGUAGE gap and not an unwritten sentence, and it sits under the whole array half of the question

The llm-ergonomist reported that from the specification alone there is no route
from a `char[N]` field to `str`. **It is worse than unstated: it does not exist.**

- `spec/heroes-spec.md` § 11, read: the conversions are `to_f32 to_f64 to_str
  to_i8 … to_u64`; § 13 gives `c.validated()` for a **`cstr`** and says *"outside
  a group nothing answers `cstr` and no record holds one"*; § 10 gives `s[i] ->
  u8` outbound only.
- The compiler agrees: `grep -rn "from_utf8\|from_bytes\|str_from" selfhost/check/builtins.hero
  selfhost/resolve/vocab.hero` returns **nothing**, and `selfhost/check/builtins.hero:123-132`
  refuses `to_str` of anything but *"an integer, a float, `bool` or `str`"*.
- The repository's own only plain-`char`-array program prints **numbers**: read
  `tests/golden/run/ffi-a-char-array-member.hero`, which prints `68` and `68 7`
  and whose header says the real `utsname` was left out because *"a real one
  needs a 256-element literal to construct, which is the separate gap panel 081
  recorded and this case does not claim to close."*

So: **the motivating program — bind `struct utsname`, print the name — is
blocked twice over independently of the sign** (the 65-element construction
literal, and bytes→text), one of those blocks is already a recorded panel-081
gap, and **no seat cited it**. The llm-ergonomist reached both by writing the
program from the spec, which is what that seat is for.

Is the sign question downstream of a larger gap? **At the array position, yes**:
whichever route wins, the 39 text buffers stay unrenderable. **At the scalar and
parameter position, no**: `pr_nice` is a number and is read today. That asymmetry
is a second, independent argument for R7, arrived at from the opposite direction
from the historian's.

What the sitting should have asked: *does the resolution make any program that
does not compile today compile and do something a user wanted?* For the headline
struct the answer is no, for a reason the brief itself flagged (`char[65]` vs
`char[256]`) and for two reasons it did not.

### Q2. The ffi-pragmatist's two defects: one qualifies a claim, one is out of scope and must still be filed

- **`const char[N]` fields refused at every spelling, on all three machines** —
  and it is the `const`, not the `char`. **In scope as a caveat**: any route
  argued as *"it makes headers portable"* must not count const members, which
  bites the warden's §1.12 completeness argument against route 4 and the
  ffi-pragmatist's own list of what route 3 makes portable. It changes no
  verdict.
- **A record with any `const` member cannot cross by value: `internal error:
  compiling the generated C failed`, exit 2** — the compiler blaming itself for
  the author's `extern`, which `.claude/rules/c-boundary.md` says should be exit 1
  on the `.hero` line. **Out of scope for the sign question and it does not change
  the resolution**, but it is a robustness item (§ Precedence rank 3) and it is
  **not** in `docs/work/DEFECTS.md`, whose open list is 058 and 059 only. It
  should be filed as a new defect by this sitting. Filing it mid-milestone is
  legal; the tag gate reads the list at the close.

---

## Admissibility

The llm-ergonomist filed a process finding: three `.claude/rules/*.md` files were
injected into its context unrequested, one stating the specification's measured
token counts — the one number that seat exists not to hold.

**Measured, not assumed.** Over its report:

```
grep -nE "design\.md|selfhost/|runtime/|7984|5997|10240|CL-0|\.claude/rules|docs/" \
    docs/panel/161-reports/llm-ergonomist.md
```

returns **exactly one line: its own process note**. No repository path, no token
count, no design.md citation, no case-law citation appears anywhere in its
reasoning; every authority it cites is a spec section. Its one cost argument
("two names for one thing… a reader will ask why once") is argued in reader
effort and never in tokens — the currency it was handed and did not spend.

**Verdict on admissibility: admissible, with the breach recorded.** The input
discipline was defeated and that is a process defect worth an entry; measured, it
is not load-bearing. I say once what I cannot do: a grep over the output is
evidence, not proof, because a number can steer silently.

And the larger admissibility point, which is not about that seat: **three seats
rested on the shared brief's population count without re-measuring it** (C3).
That is the same failure with a bigger blast radius, and it is the one the
sitting's own brief warned about in writing.

---

## What I could not check

- **The two Linux containers.** I ran nothing in a container this session, so
  every cross-leg figure in the five reports — the 1825/3 net, the inversion
  table, the `elf_prpsinfo` and raylib walks, the three-machine `_Generic`
  compiles — stands as those seats measured it and is unverified by me. My
  `-funsigned-char` compile is a *model* of the arm64 leg on this Mac, not that
  leg.
- **The ffi-pragmatist's header walk.** `sys/procfs.h` does not exist on Darwin
  (checked), so "four plain-`char` scalars in `elf_prpsinfo`" and the raylib
  parameter are theirs. What I could check is that the brief's nine headers do
  not include it, which is what makes C3 a scope error rather than a wrong count.
- **Windows.** Unrun, as it was for everyone.
- **AAPCS64 §6.4.2 itself.** I measured what clang emits for two triples; I did
  not read the document the historian asked for. If the document and clang
  disagree, my U1 is about clang — which is, as the historian's own LLVM issue
  #115957 finding shows, the thing that actually compiles the emitted C, but it
  is not the document.
- **The net, under any patch.** Neither 1828/0 prediction is verified here.
- **The spec token deltas.** I did not run `heroes measure --refresh`; the
  R5-with-bit-preserving-reread draft that C2 says is the honest route 5 has
  **never been priced by anybody**.
- **Whether a `u8[N]` → `str` route exists at run time** through some composition
  I did not think of: I grepped the builtin table and the spec, I did not compile
  a program that tries. The negative in Q1 rests on those two greps and on the
  repository's own golden printing numbers.
