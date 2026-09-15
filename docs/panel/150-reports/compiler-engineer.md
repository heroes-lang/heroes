# Panel 150 — report of the compiler-engineer

Worked in a copy; seed built in 3.44 s. `selfhost/` was never rebuilt, so **every
claim about how a patched rule BEHAVES is unrun and labelled**; every claim about
what ships today was run. Panels 147, 148 and 149 were read in full and are cited
where they bite.

| | verdict |
|---|---|
| **Q1 — the `ptr` producer** | **REFUSE WITH VETO** on the type-keyed rule. **Adopt with condition** the route nobody listed. |
| **Q2 — the per-element release** | **REFUSE WITH VETO** on *WHOLE versus PER-ELEMENT*. **REFUSE** *refuse the element release*. **Adopt the message.** |

## Q1 — the veto, and it is a compelled wrong free

**The type-keyed rule cannot be built soundly**, because `Reached.decl` is a
declaration index and **a `ptr` has no declaration**. Any implementation collapses
every `ptr` to one key, which makes `releaser_reads` vacuous.

**Measured on the shipped compiler**, and this is the veto's ground:

```
extern "stdio.h"
    function fopen(path: cstr, mode: cstr) -> ptr acquires free
    function fclose(p: ptr consumes) -> i32
extern "stdlib.h"
    function free(p: ptr consumes)
```

releasing with `free`: **`heroes check` exit 0**, balance +1/−1, and under
`--sanitize`: **`AddressSanitizer: attempting free on address which was not
malloc()-ed`, exit 134.** Without the sanitizer it is a **silent** exit 134 with
no message at all.

*Coordinator's confirmation, 2026-09-15: re-run independently. Check exits 0;
AddressSanitizer reports the bad free.*

**That is panel 148 R1's compelled-wrong-free re-entering through the key** —
*"under C a mistake compels a release the program must not make"* — and §1.12
with CLAUDE.md § Precedence rank 3 make it a refusal, not a price.

**And the mark on a `ptr` is never read at all today**: `malloc(size: u64) -> ptr
acquires sqlite3_notafunction` beside `free(p: ptr consumes)` **checks at exit 0**.
That is defect 036's class, still open on `ptr`.

**The rule would also be program-wide, proved rather than read.** `duplicate_tag`
fires across a module boundary, so `decls` spans every module. A `ptr consumes`
**anywhere** would arm the rule for every `ptr` producer everywhere. The seat
compiled the nuisance: three modules, `malloc`/`free` in one and `dlopen`/`dlsym`
in another, and it **checks clean and runs today**, aborting 134 on a correct
program with a message naming two causes neither of which happened. Under the
type-keyed rule it would demand **three** marks, two of them in a module that has
never heard of `free` — and the note on `dlopen` would read *"write `acquires
free`"*, because with one key `taker_of` returns the first `ptr`-consumer in
program declaration order.

## The route nobody listed, and it needs no construct at all

```
extern "stdlib.h"
    record Blob tag void
    function malloc(size: u64) -> Blob acquires free
    function free(p: Blob consumes)
```

**Checks, builds, runs, exit 0.** The emitted C is `void *` byte for byte. Drop
the mark and it is **already** `error[unmarked_handle_producer]` today. And it is
type-separated: handing a `ptr` where a `Blob` belongs is `error[type_mismatch]`.

*Coordinator's confirmation, 2026-09-15: the marked program prints and exits 0;
the unmarked one is already refused.*

**So the language already carries all three things design.md Part 9 says `ptr`
lacks — pointee type, identity, ownership — and the binding author pays one
line.** The adopted route, **Q1-B, is to REFUSE the mark on a `ptr`** and point
the author at the named form: **+51 lines in one module, and §1.7's subtraction is
positive, because it DELETES a mark position rather than adding one.**

## Q1 on the CONSUMER's side, said explicitly

- **`ptr` already has a consumer-side rule and it fires.**
  `consumed_borrowed_handle` caught the seat's own nuisance binding unaided. **The
  consumer side is not the gap.**
- **The type-keyed rule's consumer side is where it breaks**: one key for all
  `ptr` means the consumer that arms the rule and the producer it refuses need
  have nothing to do with each other, measured across two modules.
- **Q1-B's consumer side is empty, deliberately.** It touches no `consumes`, no
  call site, no emitted C.

## Q2 — both routes vetoed or refused, and the measurement that does it

**Route 5b, *the mark says WHOLE or PER-ELEMENT*, is REFUTED by construction.** A
PER-ELEMENT mark must emit the **declared** array length, and both headers this
sitting cites say the **filled** count is C's runtime choice. The seat built it:
four `@` out-parameters each `acquires`, against a header filling two of four —
the `jpeglib.h` *"or NULL if not defined"* shape:

```
panic: 2 C handle(s) never given back — every call marked `acquires` owes one
marked `consumes`, and this program is missing that many
```

**It turns defect 038's `-3` into a `+2`. Same class, opposite sign, on the exact
headers that motivate it.** Priced from the measured `borrows` landing at
**+47/−12 across 8 modules for the WORD alone**, plus the emitter, plus two
colourers that know neither existing word: **~110-130 lines across 11 files, and
core by §1.7's own test.**

**Route 5a, *refuse the element release*, is REFUSED on cost and on soundness.**
The naive syntactic rule — a `consumes` argument that is not a bare name — is ~25
lines and **refuses four shipped call sites**, two of them in
`examples/ledger/db/sqlite.hero`, so `corpus` goes red. The sound version needs
the provenance of the value the argument is part of, and panel 148 R6 recorded
that this corpus acquires inside **Heroes wrappers one module away**, so the chain
crosses a signature carrying no mark of any kind. `check/leasing.hero:29` and
`check/consuming.hero:22` both state there is no flow analysis.

## Route 6 — the message, and it is RUN

**+9/−3, one file, one `fprintf`, zero compiler passes.** The seat patched
`runtime/parts/alloc.c` and rebuilt both failing programs against the patched
runtime. The element-release program now names *"a value marked once with
`acquires` and released ELEMENT BY ELEMENT, since one mark is one obligation on
the whole value"*, and the nuisance binding names *"a `ptr` producer carrying no
mark beside a `ptr consumes`"*.

**What it fails to catch: everything.** It refuses nothing and closes no class.
*"Its whole value is that it stops blaming the wrong thing, which is exactly what
defect 038 is."* **Its return condition, named rather than left open**: the fourth
cause is deleted the day a static refusal for `ptr` lands, and `tests/golden/run/`
owes one case per cause.

**And the composite release is the working shape**: `four_close(f: Four
consumes)` against `-> Four acquires four_close` **exits 0**.

## What breaks, measured

- **Blast radius of the type-keyed rule, widened beyond the brief's count.**
  Counting producer POSITIONS, result and `@` out-parameter, gives **16** across
  `examples/` and `tests/`, and **exactly one file** also carries `ptr consumes`.
  `selfhost/` and `examples/`: zero.
- **It would take two more goldens red** unless a flag is threaded: a `ptr` map
  key is refused today at the **emit gate** by `unsupported[pointer_element]`, and
  letting the walk see `ptr` unconditionally makes `handle_map_key` fire at CHECK
  time instead. **That is a second diagnostic class and a panel trigger of its
  own.** Eleven of the thirty-eight added lines exist only to avoid it.
- **Q1-B breaks nothing.** Marked `ptr` producers across the whole tree: **one**,
  and it is this sitting's own shared brief.

## Principle 0

**Neither is compiler-need.** `selfhost/` declares **0** `ptr`-returning externs,
**0** `ptr consumes`, and **0** handle records — all twelve `record … tag …` hits
are inside test string literals.

## Does it still fit one person

*"Not a rhetorical question at this point."* `selfhost/` is **209 files, 61,034
lines**, Pascal-P4's ~4000 fifteen times over. Q1-B is +51 in one module and
deletes a mark position; the type-keyed rule is +68/−29 over three and adds a key
the language does not define; Route 5b is ~110-130 over eleven and adds a word to
the core. **On §1.1's hierarchy only the first lowers the ceiling.**

## Prediction

**If the type-keyed rule lands, the `fopen`/`free` program above will `heroes
check` at exit 0 and abort under the sanitizer.** Already measured against today's
compiler. **Falsified** if the landed rule errors on `acquires free` after a `ptr`
result that `fclose` also consumes — *"which is the discriminator I said does not
exist, and producing it is exactly my withdrawal condition."*

**Second-order**: if Q1-B lands instead, the named suites stay green with **zero**
files changed.

## Conditions

- **The Q1 veto withdraws** on a `(key, discriminator)` separating two `ptr`s from
  two different libraries **derivable from what the binding author already
  writes**, with a measured false-acquire rate of zero on the bad-free program and
  the nuisance binding. Or if the panel refuses `ptr consumes` outright, which
  removes the arming clause.
- **The Q2 veto lifts on 5b** for a real shipped header where the FILLED count
  equals the DECLARED length unconditionally and the header says so in its own
  text. `net/route.h`'s bitmask and `jpeglib.h`'s *"or NULL"* both refute it.

**Unrun, stated as such**: no patch above was compiled into a working compiler.
Line counts are exact and parseability is measured; diagnostic behaviour is
reasoned from cited source lines and is a prediction.
