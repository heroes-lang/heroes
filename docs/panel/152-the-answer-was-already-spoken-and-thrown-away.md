# Panel 152 — the answer was already spoken, and thrown away

**Convened** 2026-09-15 on `docs/work/DEFECTS.md` 037, **three seats under
CL-023**: the compiler-engineer and ffi-pragmatist ruled at panel 151 and their
reports stand as input rather than being re-run. Brief at
`docs/panel/152-briefs/00-shared.md`.

## The question

> May the compiler ask clang ONE BIT about a `tag` — does this word need `struct`
> in front of it? — and keep the author's word as the identity?

## Verdict

| seat | verdict |
|---|---|
| **spec-warden** | **adopt with condition, veto-backed**: yes to the capability, **refuse with veto** on the generated probe |
| **historian** | **object** (advisory) |
| **llm-ergonomist** | **object**, escalating to **veto** on the ambiguous tag |

**The capability is adopted. The mechanism is refused.** And the mechanism is
refused because **the answer already arrives and is discarded.**

## The finding that ends the sitting

**Clang already distinguishes the three cases by message.** Measured by the
coordinator, `-Wall -Wextra -Werror`:

| written | clang says |
|---|---|
| `addrinfo *p;` | **must use 'struct' tag to refer to type 'addrinfo'** |
| `nosuchtype *p;` | **unknown type name 'nosuchtype'** |
| `struct nosuchtype *p;` | **nothing. exit 0.** |

**And the recogniser for the first message already ships.**
`selfhost/emit/ffi_tag.hero:31` reads the exact string *"must use 'struct' tag to
refer to type '"* and builds a Heroes diagnostic with a `certain` fix. **It is
discarded for one line**: `selfhost/emit/ffi_lookup.hero:69` compares
`decl.name` and never the tag, so `record AI tag addrinfo` is invisible to a
lookup for `addrinfo`.

> **The disagreement §4.19 exists to hear is already spoken, already
> unambiguous, and already thrown away.** — the spec-warden

**A failing build already prints it five times.**

## Why the probe is refused, and two seats found it independently

**The one bit measures two conditions and resolves the ambiguity by
fabricating.** The historian predicted it without a shell; the warden measured
it; the coordinator ran it:

```
addrinfoo *p;          → error: unknown type name
struct addrinfoo *p;   → exit 0, zero diagnostics
```

**A misspelled tag gives the same bit as a correct struct-only tag.** The route
would emit `struct addrinfoo *` against a type C fabricates on the spot. The
warden simulated it on the compiler's own output: **exit 2 and 23 error lines
today, exit 0 under the probe.**

And the route's advertised strength — *an answer for a handle that appears in no
signature at all* — is **exactly the case where nothing downstream refuses it**.
`selfhost/emit/ffi_tag.hero:95` already names the hazard in the compiler's own
words: *"the tag may be MISSPELLED, in which case C has invented an empty type of
that name."*

## The precedent is unanimous, and it goes against guessing

The historian found **six independent systems that make the author write `struct
addrinfo`**: Nim, Go's cgo, Zig, CMake, Meson, autoconf. **Nim's own posix module
does it thirty-two times in one file**, and CLAUDE.md § 6 binds Heroes to Nim's
surface.

**cgo refuses a misspelled name rather than guessing it** — issue #31390,
`C.CStirng`, a plain typo, and nobody in the thread proposed correction.

**And the one tool that ever supplied a spelling the author omitted was
removed.** .NET's `DllImportAttribute.ExactSpelling=false` probed for `A`/`W`
suffixed entry points for twenty years at Windows scale; its successor
`LibraryImport` dropped it, and the code-fix now **makes the author choose**.
*"Panel 151's measurement generalises."*

**What the instrument itself is not, is novel.** Autoconf moved header checks off
the preprocessor and onto the compiler across 2.56, 2.64 and 2.70, in its own
words because *"incompatibilities between headers went unnoticed during
configuration"*. cgo's `guessKinds` infers from *"the presence or absence of an
error on a specific line"* on every Go build since 2009. **Reading clang's
disagreement is that instrument. Generating a probe to guess a spelling is not.**

## What the ergonomist found, and it is bigger than the question

**§ 13 cannot bind the shape C uses most.** A struct you both **read** and
**point at** needs two records on one tag — the fielded one for `ai_family`, the
handle for `struct addrinfo **` — and *"two records may not name one tag"*.
`netdb.h`, `struct stat` under `lstat`, and every linked list in every C header
are that shape. **Filed as defect 042.**

**And the specification's only FFI example teaches the omission.** Its
`sqlite3_open`/`sqlite3_close` pair — an acquire and a release — carries
**neither** `acquires` nor `consumes`. The seat predicts **≥ 8 in 10** models omit
them following that example, and the omission *"compiles, runs, leaks, and
produces no diagnostic and no abort"*, because the rule only arms where a
`consumes` exists. **Filed as defect 043.**

**Two sentences of § 13 give the qualifier two values**, and the seat quoted
both: *"the header writes it after the word struct, which `tag` gives"* implies
`struct addrinfo`; *"`record Db tag sqlite3` is `sqlite3 *`"* implies the bare
word. **`sqlite3.h` is the one header where both are right**, because it declares
`typedef struct sqlite3 sqlite3;` — so the document's only example sits exactly
where the split is invisible.

## Resolution — provisional, author ratification pending

**R1 — THE QUALIFIER COMES FROM CLANG'S OWN DISAGREEMENT, NEVER A PROBE.**
`ffi_lookup.declaration` learns to match a record's `tag` as well as its name, so
`ffi_tag.hero`'s `missing_tag` — which already exists, already has a `certain`
fix, and already reads the exact string — stops being discarded. **No probe file,
no second stage, no cache key, no external tool**, so §4.19's *"no external tool,
no libclang"* needs no amendment at all.

**R2 — A TAG CLANG CALLS UNKNOWN STAYS REFUSED, as a Heroes `error[…]` and not
an internal error.** The three clang messages are three different answers and the
compiler must keep them three.

**R3 — THE SPEC LOSES A PHRASE AND GAINS A CORRECTION, net +4 real.** *"What the
header leaves opaque"* does not describe `addrinfo`, which `netdb.h` defines in
full, so the prompt would not authorise the binding this sitting enables:
**−3 real** to drop it. Panel 150's unpaid debt is **still false at HEAD**, all
three probes re-run and all three exit 1, and it is **+7**. **The package is
7974 → 7978, digest `7898916124eeea2b`**, against a ceiling of 10240.

**R4 — design.md:2192 stops saying the tag is written VERBATIM**, and says
instead that the tag is the author's word and the identity, that the emitter
writes that word, and that it supplies `struct` only where the header's own
refusal names it — never a name the author did not write. **A design.md row is
free, measured.** Its falsifier, under CLAUDE.md § 12: **the narrowing is wrong
if any binding refused today builds under it.**

**What conservative would have been** (CL-040): refuse outright and leave defect
037 open with the internal error standing. Recorded and not taken, because the
capability is measured to be needed and the safer mechanism is measured to
already exist.

## What this sitting produced besides its resolution

| defect | what it is |
|---|---|
| **042** | § 13 cannot bind a struct that is both read and pointed at: two records would need one tag |
| **043** | the specification's only FFI example carries an acquire/release pair with neither mark, and teaches the omission |

## Predictions scored

**Panel 150's spec-warden — HELD, exactly.** It predicted D1 would read **7981
real**; re-run today it reads **7981**. Its cl100k differs by one from that
sitting's draft, so the reconstruction is not byte-identical and prices
identically on the binding instrument.

**Panel 152's historian — RUN AND CONFIRMED by the coordinator**, having no shell
itself: `addrinfoo` fails, `struct addrinfoo` exits 0 silently.

## Author's verdict

**RATIFIED 2026-09-15**, as adopted: the capability yes, the generated probe no.

**HALF OF R1 LANDED THE SAME DAY**, and it is the half a program author meets.
`emit/ffi_lookup.hero` now finds a record by its TAG as well as by its name, so
clang's *"must use 'struct' tag"* stops being looked up under a name no
declaration holds. A binding that produced `internal error: compiling the
generated C failed` at exit 2 with zero Heroes diagnostics now produces
`error[ffi_tag_needs_struct]` with the caret on the author's own line. **R2
landed with it**: a misspelled tag stays `error[ffi_unknown_name]`.

**OWED.** The other half of R1 — actually SUPPLYING `struct` from that same
refusal — is unlanded, and defect 037 stays open for exactly it. R3, the spec at
+4 real with digest `7898916124eeea2b`, is unlanded. R4, the design.md row that
stops saying *the tag is written verbatim*, is unlanded and free.

**The diagnostic that landed offers no fix, and the yes ratifies that too.** Its
sibling offers a `certain` one because a record with no tag can be given one;
here the tag is already correct and the repair does not exist, so a fix would be
what `.claude/rules/diagnostics-and-goldens.md` refuses — one that leaves the
defect standing however well it compiles. The blessed emission pins the wrong C
in bytes, `addrinfo *` five times, so the day the capability lands a suite says
so.

**What the yes does not settle**: defects 042 and 043, both filed by this sitting
and neither of which it was convened on.

**What a yes settles**: that the qualifier is read from clang's refusal rather
than guessed, that an unknown tag stays refused as a Heroes error, the +4 on the
specification, and the design.md row.

**What it does not settle**: defects 042 and 043, both filed today, and neither
of which this sitting was convened on.
