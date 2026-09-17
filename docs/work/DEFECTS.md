# DEFECTS — the compiler defects that are still open

Every item is a **measured** failure of the compiler on a program — a crash, a
wrong answer at exit 0, a silence where a message is owed — carrying its
reproducer, its cause where known, and what is owed. **Only open defects live
here**: a repaired one is ticked, gains a *The repair* section with the
measurements that prove it, and moves to `docs/records/done/`. A repair is owed
at the class and not at the witness, with a `tests/golden/fixedbugs/` case per
shape.

**The shape** is `.claude/rules/records.md` § The lists, and § A live list is a
preamble, a count and its items is why this preamble is fifteen lines. **The
next number is READ, never remembered** — `records/numbering` takes one above
the highest issued across this file and `docs/records/done/`. Who issued which
number since 2026-09-08, and why 014 exists twice, is
`docs/records/log/2026-09-16-2200-the-defect-register-leaves-the-list.md`.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **058 — a plain `char` field or parameter has no spelling that binds on every platform** | `i8` binds it on two legs and `u8` on the third, so one `extern` cannot be written for all of them, and `tests/golden/run/ffi-a-char-array-member.hero` is accepted on x86-64 and refused on arm64 Linux | `selfhost/emit/extern_field.hero:154` · `selfhost/emit/extern_field.hero:261` · `tests/golden/run/ffi-a-char-array-member.hero`

    **Origin:** M-arm-platform, 2026-09-17, the fourth leg's first hour.

    **The reproducer, measured on two Linuxes from one Mac**, same Debian 13.6,
    same clang 22.1.8, same glibc 2.41 — the architecture is the only axis that
    differs. A local header declares `char scalar; char arr[4];` and four
    programs bind them:

    | shape | Linux arm64 | Linux x86-64 |
    |---|---|---|
    | `scalar: i8` | refused `ffi_field_type` | accepted |
    | `scalar: u8` | accepted | refused `ffi_field_type` |
    | `arr: i8[4]` | refused `ffi_field_type` | accepted |
    | `arr: u8[4]` | accepted | refused `ffi_field_type` |
    | `tests/golden/run/ffi-a-char-array-member.hero` | **refused** | accepted |

    **The checker is not wrong, which is what makes this a language question
    rather than a repair.** C has three distinct `char` types and its plain one
    takes its sign from the platform: unsigned under the generic AAPCS, which
    Debian arm64 follows, signed on x86-64 and signed on Darwin arm64, where
    Apple's own ABI deviates from AAPCS. `spec § 13` says a field and a
    parameter are declared at *the header's own width and sign*, the sign
    conjunct at `extern_field.hero:154` asks exactly that, and it answers
    correctly on both machines. What has no answer is the author's question:
    **which of the eight integer types do I write.** Heroes' eight each carry a
    fixed sign; C's `char` does not.

    **The shapes beside it were attacked and all four invert** (CL-061): scalar
    field, fixed-array field, parameter and result. `signed char` and
    `unsigned char` bind portably at `i8` and `u8` and are not affected — the
    hole is the third type alone.

    **What it costs today, named rather than implied.** The existing golden case
    carries a comment reading *"`char` is signed here"*, a premise about the
    world measured on one machine and true on three of the four legs this
    project now has. `docs/roadmap/production-ready.md`'s FFI promise and
    `M-core-packages`' roughly eighteen headers are what a non-portable `char`
    reaches: the count of plain-`char` members across those headers is **unrun**
    and is the first thing a sitting on this owes.

    **This is a panel question** (CLAUDE.md § 4: the FFI's accepted set and a
    diagnostic class), and the routes are at least three — a ninth integer type
    whose sign is the platform's, a `char`-spelled field the checker maps per
    target, or a refusal that tells the author to declare the header's own
    `signed char`/`unsigned char` instead. Naming three is not a claim that
    three is the set (CL-057).

- [ ] **059 — on a platform where `char` is unsigned, the diagnostic names the spelling it has just refused** | `ffi_parameter_type` on `i8` against a `char` parameter says *"Declare it `i8`"*, so an author who follows the compiler's own note loops forever | `selfhost/emit/c_spellings.hero:59`

    **Origin:** M-arm-platform, 2026-09-17, found by the same probe as 058.

    **Measured on Linux arm64**, verbatim from the run:

    ```
    q-param-i8   REFUSED error[ffi_parameter_type]
       note: §4.19: a result may be wider than C's, and a parameter is declared
       at the header's own width and sign. Declare it `i8`, and convert at the
       call where the value is known to fit
    ```

    On Linux x86-64 the same program is accepted, and the note is correct there.

    **The cause is one table row.** `selfhost/emit/c_spellings.hero:59` reads
    `if c_type == "signed char" || c_type == "char"` and answers `i8` with
    `no_caveat`, on every target. **That file's own header comment records
    having learned this exact lesson one type over**: `long` and `unsigned long`
    were absent until their absence was the defect, and the fix was to stop
    tabling the answer — *"A width is not a constant to be tabled; it is a
    question about the target"*, so `word_bits` arrives as a parameter the
    driver owns. The SIGN of plain `char` is the same kind of question and never
    got the same treatment.

    **It is filed apart from 058 because the repairs are different.** 058 asks
    what the language should offer and needs a sitting; this one asks the target
    a question the file is already shaped to ask, and its repair is bounded by
    whatever 058 resolves — a note cannot name a portable spelling before one
    exists. The `Spelling` record already carries a `caveat` field for exactly
    the case where *"the C type's width is the PLATFORM'S rather than a width
    the header chose"*, and nothing uses it for sign.

*******************************************************************************
