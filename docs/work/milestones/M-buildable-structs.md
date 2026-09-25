# M-buildable-structs — the structs a program can build

**Scheduled 2026-09-18** out of panel 164, which recorded *a real five-field
`utsname` needs 1280 literal zeros*. **This file opened 2026-09-25**, with
panel 178, while the milestone's row stays `scheduled`: one `**OPEN**` row at a
time is what `site/src/lib/chain.ts` accepts, and M-agreed-retention holds it.

**What the milestones before it left standing.** Panel 163 found there was no
wall for a struct C fills: a function that returns the record, and `partial` to
declare only what is read. What it did not do is make a struct with long arrays
writable in the program's own construction, and it left `[0; 256]` unpriced.

**Why it is a milestone and not a note.** The repository was the wrong sample:
its longest fixed array is 8, so nothing here ever hit the wall. The headers
M-core-packages will bind are the right one, and panel 178's census counts 42
public structs on Darwin, 29 on Linux arm64 and 23 on Linux x86-64 with an
array longer than 8, in three kinds: bytes the program never touches, buffers C
fills, and bytes the program writes. The same struct differs by platform
(`utsname` is five of `[256]` on Darwin and six of `[65]` on Linux), and an
all-zero value is not valid for every type on every platform (a zeroed
`pthread_mutex_t` locks on Linux and is `EINVAL` on Darwin). **Why here, before
M-core-packages**: the argument M-readable-bytes made for its own position, a
binding written before the wall moves is a binding rewritten after it.

**What panel 178 decided, provisionally**: `rest: zero` on a group record's
construction, zero admitted as bytes and never as a claim, the header's own
initialiser as the way C says which value is valid, C's `char` spelled once,
and defects 091 to 094 filed in `docs/work/DEFECTS.md`. The items below are
what the landing owes and what the sitting left open.

*******************************************************************************
**OPEN: 6**

- [ ] **M-buildable-structs** | land panel 178's resolution: `rest: zero` for a group record, § 13's `char` sentence, the header initialiser as a constant (defect 094's repair) and the fixed-element write (defect 091's) | `docs/panel/178-the-rest-is-zero-where-it-is-written-and-c-says-which-value-is-valid.md` · `docs/panel/178-reports/compiler-engineer-work/r1-and-091.diff`

    **Origin:** panel 178's resolution, items 1 to 5, 2026-09-25, provisional.
    **The order is a condition of the sitting, not a preference**: a golden for
    `missing_fields` on a Heroes record and on a group record lands first,
    because that diagnostic has no test anywhere today (the compiler-engineer,
    confirmed by the critic); the three DECIDED rows of
    `tests/harness/suite_layout.hero` the prototype moves (`ast.hero`,
    `check/walk.hero`, `print/fmt.hero`) are named before the build, and the
    construction check leaves `check/walk.hero` for its own module; the
    emission is the compound literal, never member stores into a bare cell.
    The spec text is the +32 sentence the critic priced (8393 real, digest
    `36a1c34f7eaceaba`, on a base of 8361) plus the `char` sentence, priced on
    the real instrument at the landing. The base read **8805** when this file
    was written (`517b8e25`), and the record paragraph was unchanged, so
    +32 ± 2 above it is the prediction, unrun on that base.

- [ ] **M-buildable-structs** | a string into a fixed field waits behind its measurement, and the landing is where it is taken | `docs/panel/178-the-rest-is-zero-where-it-is-written-and-c-says-which-value-is-valid.md` item 6

    **Origin:** panel 178, 2026-09-25. Route T, `s.copy_into(@field)` (+61
    real, unbuilt, untested), returns if either is measured on the landed
    `rest: zero` and the repaired element write: a reader test in which
    writing a path into `sun_path` fails first try at least half the time, or a
    run in which the byte loop or C writing through the field's counted lend
    hands C an unterminated field at exit 0 on some leg. The value form,
    `s.to_fixed()`, is refused on the compiler-engineer's ground: it needs
    storage for `T[N]`, reversing `selfhost/emit/ctype.hero:380`.

- [ ] **M-buildable-structs** | a record passed by value loses defined padding on Linux x86-64, and C can write those bytes out | `docs/panel/178-reports/completeness-critic.md` § 1 finding 1 · `docs/panel/178-reports/completeness-critic-work/w/pad/`

    **Origin:** panel 178's completeness critic, 2026-09-25, under
    MemorySanitizer with a positive control: a `{char c; double d}` record
    passed by value to a Heroes function and handed to `write()` carries
    uninitialised bytes at offset 1, at `-O0` to `-O3` on x86-64; clean on
    Linux arm64; the same with today's `partial` construction. Not a defect,
    because nothing in the spec promises padding and Heroes' own `==` and
    `hash` never read it. The question is whether records should cross calls
    by address or by `memcpy`, so that C never receives uninitialised bytes:
    the information-leak class CERT DCL39-C names, and a change to how every
    record crosses a call.

- [ ] **M-buildable-structs** | `m2 = m` copies a `pthread_mutex_t`, which POSIX does not allow, and nothing refuses it | `docs/panel/178-reports/completeness-critic.md` § 5 question 2

    **Origin:** panel 178's completeness critic, 2026-09-25: 19 of 50 blind
    readers raised copy-in/copy-out of the mutex on their own. The emitted C
    passes the cell's address (`pthread_mutex_lock(&h0_m)`), so a local cell
    does not move between calls, but a value copy of a locked mutex is
    **unrun**. Whether a group record can say it is not a value is the question.

- [ ] **M-buildable-structs** | `ffi_unknown_tag`'s note sends glibc's `pthread_mutex_t` to a handle, and following it aborts; the untagged record works | `docs/panel/178-reports/ffi-pragmatist.md` § 2 · `docs/panel/178-reports/completeness-critic.md` finding 8

    **Origin:** panel 178, 2026-09-25. The ffi-pragmatist bound
    `tag pthread_mutex_t`, which spells `struct pthread_mutex_t`, got
    `ffi_unknown_tag`, followed the note's advice to a handle, and got a null
    read inside `pthread_mutex_lock`, exit 134. The critic bound it under its
    typedef name with no `tag`, `record pthread_mutex_t partial`, and it locks
    and unlocks with 0 on Linux arm64 (x86-64 **unrun**). A diagnostic whose
    fix leads to an abort owes the spelling that works (design.md §4.17).

- [ ] **M-buildable-structs** | count which census structs M-core-packages constructs, and in how many bindings, because panel 178's payment rests on it | `docs/panel/178-reports/spec-warden.md` § prediction · `docs/panel/178-briefs/census.sh`

    **Origin:** panel 178's spec-warden and completeness critic, 2026-09-25.
    The sentence is paid by a registered prediction: at M-core-packages'
    close, `rest: zero` appears in 3 or more `.hero` files outside `tests/` and
    `docs/`, and no literal of more than eight zeros remains; if fewer than 3,
    the ledger row lapses and the sentence is argued again. No seat counted how
    many bindings will construct such a struct; the landing counts it before
    the sentence is paid for.

*******************************************************************************
