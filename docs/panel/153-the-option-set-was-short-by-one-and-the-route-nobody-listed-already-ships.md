# Panel 153 — the option set was short by one, and the route nobody listed already ships

**Convened** 2026-09-15 on `docs/work/DEFECTS.md` 042 and 043, without asking,
under CLAUDE.md § 4's *ask once per milestone then convene again*. **Full panel**,
five seats plus the completeness critic. Briefs at `docs/panel/153-briefs/`,
reports at `docs/panel/153-reports/`, all six briefs written before any seat
started. Both defects were filed by panel 152's llm-ergonomist and neither had
been ruled on.

## The two questions

> **Q1, defect 042.** § 13 cannot bind a struct that is both READ and POINTED
> AT. `getaddrinfo` hands back `struct addrinfo **`, which needs a handle, and
> reading `ai_family` needs a record with fields; both would carry
> `tag addrinfo`, and *two records may not name one tag*. How does a program
> read the fields of a struct C hands back by pointer, walk a list of them, and
> hand one to C by pointer?
>
> **Q2, defect 043.** The specification's only FFI example is an
> acquire-and-release pair carrying neither mark. What pays for correcting it,
> and may a section's one worked example be incomplete on the section's own rule?

## The verdict table

| seat | Q1 Route A (fields on a handle) | Q1 Route B (two records, a `read`) | Q2 |
|---|---|---|---|
| **compiler-engineer** | **object** | **adopt**, four conditions | — |
| **ffi-pragmatist** | **object** | **adopt**, one condition: not named `read` | **adopt** |
| **spec-warden** | **object** | **adopt** as B2, the dearer wording | **adopt**, paid by a named removal |
| **historian** (advisory) | **approve** by precedent | **object** as the only read path | **approve** |
| **llm-ergonomist** | **object** | **adopt** | **adopt** the corrected fence |

No veto was cast on anything.

## What the seats measured, and each contributed one that mattered

**The `hints` leg already ships.** A record with fields handed to C by pointer
through an `@` parameter compiles and runs today, against `const struct tm *`
and `const struct timespec *` — the ffi-pragmatist and the spec-warden found it
independently. So the brief's *"both routes owe an answer to handing a record
VALUE to C by pointer"* was owed by neither.

**`lstat` is not the shape the defect says it is.** `record FileStat tag stat
partial` with `@buf` builds, runs and prints a file's size. The ffi-pragmatist
and the llm-ergonomist reached it independently, and the coordinator re-ran it:
defect 042's sentence *"`struct stat` under `lstat`, and every linked list in
every C header, are the same shape"* is **false of its first half**. The caller
owns that struct; only the pointer C hands back is the problem.

**Route A does not reach the third leg.** Under A, `AI` with fields is still a
handle, so `@hints: AI` is `struct addrinfo **`, and A's own text forbids
writing its fields. Real hints therefore need a by-value record beside the
handle on one tag — the pair the shipped compiler refuses. **A needs B's
widening anyway**, which the spec-warden argued and the critic measured twice.

**The precedent is for the implicit dereference and against nothing.** The
historian found five of eight designs reading fields through a pointer with no
operator, and **objected to Route B as the ONLY read path**, since no language
in the lineage ships copy-out as the sole way to read a pointed-at struct.

**The document teaches the omission, measured on programs.** The forgetful
reader exits 0 with an empty stderr under the fence as it stood and is stopped
at 134 under the corrected one. That is Q2, and every seat that ruled on it
adopted the correction.

## What the completeness critic found, and it moved the resolution twice

Report at `docs/panel/153-reports/critic.md`.

**A. The narrowing is not a third route; it is the first clause of Route B, and
Route A needs it too.** The coordinator had prototyped it, measured it green and
offered it as Route C. The critic reproduced the central claim — the shipped
compiler refuses `getaddrinfo`, the narrowed one builds and runs it — and then
placed it correctly: it is a **precondition common to both routes**, so Q1 was
never the A-or-B question the brief posed.

**B. The prototype shipped the defect its own source file names.**
`selfhost/handles.hero` gave the diagnostic lookup as a REASON for the old rule:
*"`record_by_tag` answers with the FIRST record carrying the tag, so a message
about the second would carry the first's caret."* The critic built that shape
against the prototype and got exactly it — the caret on the handle's line, the
repair printed being the line it pointed at, and the offending record never
named. Exit 1 both ways, so no suite saw it. **Repaired before landing**: the
lookup is now asked which kind the message is about, with
`tests/golden/fixedbugs/fixedbugs-a-tag-with-two-kinds-names-the-right-record.hero`
as the witness.

**C. THE ROUTE NOBODY LISTED, AND IT SHIPS TODAY.** The brief asserted *"nothing
turns a handle or a `ptr` into the struct it points at"*; the compiler-engineer
reported *"third route: none found"* after searching the IR, the library
mechanism, generics and the `@` path. **All four searches were inside the
compiler.** Nobody searched the FFI, which is where §1.11 says everything in
this language comes from. Three lines of header —

```c
#include <netdb.h>
typedef struct addrinfo AddrInfoValue;
static inline AddrInfoValue ai_read(const struct addrinfo *p) { return *p; }
```

— and the **shipped** compiler reads the struct, walks `ai_next` to the end and
prints output byte-identical to the equivalent C program. No compiler change, no
new word, no built-in, no spec token, and no `duplicate_tag` to narrow, because
the value record reaches the struct by typedef name. Every field is still
verified by clang against the header. **20 headers of the project's own already
ship under test, 16 of them with a `static inline` body**, and a golden already
returns a fielded record by value from one.

**D. Its one cost is a defect, and it is filed.** `ai_read(p: nullptr)` builds
clean and dies at **exit 139**, SIGSEGV, both streams empty. §1.12 forbids that
outright. `docs/work/DEFECTS.md` **045**.

**E. The corruption class the sitting was nominally about is open today and
stays open under everything adopted.** A handle read out of a borrowed node and
then released — `freeaddrinfo(node.ai_next)` before `freeaddrinfo(r)` — builds
at exit 0 with no diagnostic, runs to **exit 133 with empty streams**, and is
`AddressSanitizer: attempting double-free` under the sanitizer. B2 buys a
sentence about it; the sentence does not close it.

**F. The tree was not frozen, and the coordinator is who broke it.** The brief
declared the working tree frozen from the briefs to the synthesis. Q2 was
adopted, landed and recorded at 17:07 while four of the five reports are stamped
16:51, so `SPEC_DIGEST` moved under the spec-warden's twenty-row table and no
seat could have known. Recorded here rather than excused.

**G. The ergonomist's blind A/B was not blind and its ratio is a reading.** The
brief promised a blind comparison and printed § 13 verbatim at its foot, fence
included; and the ten attempts per fence are ten readings narrated in one pass,
not ten runs of a model. The seat's own report names the instrument that would
settle it and it was not run. **Corrected underneath in the done record and in
the ledger row**, both of which had carried the ratio as the thing that paid.

**H. A removal nobody listed.** § 13's prose says `acquires sqlite3_finalize`
and the section declares no such function. Shortening it to `acquires` is
**−4 vendored and −9 real and deletes no rule**, where the removal that paid for
Q2 deletes one. The spec-warden's *"nothing left to remove"* is falsified by it.

## Resolution — provisional, author ratification pending

Adopted under CLAUDE.md § 4: the most robust and complete resolution, never the
cheapest and never a compromise. **It is not what the seats converged on**,
because the critic showed the option set was short by one and a resolution
cannot be the most complete one when it is chosen from an incomplete set.

**R1 — THE NARROWING LANDS, and it is a precondition rather than a route.**
`one_tag_one_type` admits at most one handle and at most one record with fields
per tag. Its premise survives where it was written for and fails where it was
reaching by accident, measured in C alone under `-Wall -Wextra`:

| two Heroes types over one C struct | the swap in C | clang |
|---|---|---|
| two HANDLES, `struct s *` twice | a type where its own type belongs | **silent** — the premise holds |
| two RECORDS WITH FIELDS, `struct s` twice | the same | **silent** — the premise holds |
| a HANDLE and a RECORD, `struct s *` and `struct s` | either direction | **two errors** — the premise fails |

`record_by_tag` learns which kind a message is about in the same change (finding
B). Measured after: `getaddrinfo` binds whole against `netdb.h` alone, builds,
runs and exits 0, plain and under `--sanitize`; both refused pairs stay refused;
the compiler's own tests and every suite are green. **Defect 042's own sentence
is repaired by this and it closes.**

**R2 — THE READ QUESTION IS NOT SETTLED HERE. It is queued as its own sitting,
with Route D on the ballot.** Q1 was put as A-or-B and the critic measured a
third answer that ships. The next sitting prices the difference and nothing
else: *the built-in buys a null guard and a spec sentence; a header of the
author's own buys the same read at zero tokens and no new word. Is the guard
worth +77 real and a new primitive, or is the guard the thing to build, with no
new construct at all?* It drags three sub-questions with it, all unasked here: is
a `.h` beside a `.hero` a supported input class against `.claude/rules/cli-surface.md`'s
refusal of a fourth, what suite judges one, and does it need a spec sentence.

**R3 — THE SEGFAULT IS A DEFECT AND NOT A DESIGN QUESTION**, filed as 045 and
repaired on its own clock. A blanket guard is refused in advance:
`freeaddrinfo(NULL)` and `sqlite3_close(NULL)` are legal C, so the rule must say
which handle arguments are guarded, and that is a diagnostic class.

**R4 — Q2 LANDED, and its record carries its own correction.** The fence gains
the two marks, *Unmarked pointers are never freed.* pays, `design.md` §4.19's
copy is corrected with it, **+0 real and +0 vendored**. Defect 043 closed.

**R5 — THE PROCEDURE FAILED IN TWO PLACES AND BOTH ARE THE COORDINATOR'S.** The
tree was not frozen (F) and the blind task was not blind (G). Both are written
into the record they contaminated rather than into this file alone.

**What conservative would have been, recorded so the author can choose it**
(CL-040): adopt B2 as the five seats converged, land the `read` built-in, the
spec sentence at +77 real and the widened rule together, and close 042 on that.
It is **refused here** because the critic measured that the read it would build
already runs at zero language cost, and §1.2 refuses a construct that buys what
a program can already write — while the one thing the built-in would buy, the
null guard, is now filed as its own defect and reaches every binding rather than
only the ones written through the new primitive.

## What this sitting produced besides its resolution

| defect | what it is |
|---|---|
| **045** | a null handle handed to a C function that reads through it segfaults at exit 139, where a null `cstr` has been guarded since M-robustness-guards |

And one it did not file, deliberately: the double free of finding E is the
documented limit of `borrows`, which
`tests/golden/run/abort-handle-borrows-that-gives-away.hero` already states in
its own words — *a mark that lies, and no rule in this compiler can catch it* —
and design.md Part 8 wart 20 carries the class. It is named here so the author
can disagree, because the critic is right that the adopted sentence does not
close it.

## Predictions to score

| seat | prediction | instrument | when |
|---|---|---|---|
| **spec-warden** | Q2+R1 alone reads 7974 real, 5990 vendored, digest `2ec6dad90753bed2` | `heroes measure --refresh` | **SCORED, SPLIT**: 7974 exact, 5989 vendored, digest `21a9dc541cfa2fa8` — the landed text is not byte-identical to the text priced |
| **spec-warden** | with B2 landed, the `getaddrinfo` golden rewritten to `read()` prints `2` and `check` prints 0 diagnostics on the 042 pair | the golden tree | **lapses with R2**: B2 is not landed. The second half is scored here — `check` on the 042 pair prints **0 diagnostics** under R1 |
| **llm-ergonomist** | a harness task counting both marks in each sample's `extern` group: at most 3 of 10 under the old fence, at least 8 of 10 under the new | a harness task that does not exist yet | at the sitting that builds it |
| **compiler-engineer** | Route A fits three refusals in one module of at most 60 code lines | a compiled Route A | **lapses with R2**: nobody built it, and the sitting compared a measured half of B against an argued A |
| **historian** | 5 of 8 designs surveyed dereference implicitly for field access | the eight URLs | scored in its own report |
| **critic** | R4, shortening `acquires sqlite3_finalize` to `acquires`, is −4 vendored and −9 real | `heroes measure` on the draft | at the sitting that spends it |

## Author's verdict

*Pending. Queued in `docs/work/DECIDE.md` as `panel 153`.*
