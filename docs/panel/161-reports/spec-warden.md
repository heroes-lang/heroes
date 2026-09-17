# Panel 161 — spec-warden

Every number below was produced by a command run on 2026-09-17 in a COPY of the
tree at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/4cbbc5a6-a98e-4b44-80b5-b6957110cd39/scratchpad/lang`,
seed built there with `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
The repository's `spec/heroes-spec.md` was never touched; `git status --porcelain`
in the repository reads only the two untracked `docs/panel/161-*` directories.

- `verdict`: **object** — no budget veto; **veto on routes 1 and 2 as presented**
  (Principle 0's burden unmet); **object to 3** (§1.2) and **to 4** (§1.12);
  **approve route 5**, a fifth route, at **+21 real**.
- `section`: **design.md §1.12** (completeness of the boundary, and the tie-break
  above tokens), with §1.6 reporting *no breach*, §1.2 deciding against route 3,
  Part 7 item 10 standing against routes 1 and 2, and §1.0 unmet by all five on
  the compiler-need branch.
- `spec_token_delta`: **measured**, `heroes measure spec/heroes-spec.md --refresh`,
  `claude-opus-5`, 2026-09-17. Baseline **7984**.

| draft | real | delta | ceiling check (delta + 60 FFI floor) |
|---|---|---|---|
| baseline | 7984 | — | 8044 / 10240 |
| R1 ninth type `c8`, minimal | 8066 | **+82** | 8126 |
| R1-full (`c8` + a bit-preserving reread, which `to_u8` is not) | 8088 | **+104** | 8148 |
| R2 contextual `cchar` inside a group | 8033 | **+49** | 8093 |
| R3 either `i8` or `u8` | 8005 | **+21** | 8065 |
| R4 refuse plain `char` | 8040 | **+56** | 8100 |
| R5 byte, `u8` on every target (full wording) | 8031 | **+47** | 8091 |
| **R5-min**, *"C's plain `char` is a byte: `u8`, on every target."* | **8005** | **+21** | **8065** |

  Vendored cross-check on R5-min: 6015 against the ledger's 5997, **+18** — the
  offline instrument understates this draft by 14%, which is why it is not a price.

- `removal`: **nothing — and that is a problem, so the payment is a registered
  prediction.** I grepped § 13 for text any route makes redundant and found none:
  every clause in it governs a different operation (the conversion exception, the
  `ptr` clause, `tag`, `partial`, handles, leases, `owned`/`consumes`/`acquires`,
  packages), and R5 merges into the existing width-and-sign sentence rather than
  appending — which is why it costs 21 and not 47. The prediction below is the
  payment, and its instruments exist today.
- `needed_for_self_hosting`: **no**, measured. The compiler's only `extern`
  groups are `hero_os.h` and `stdlib.h`
  (`grep -rn "^extern " selfhost/` → `cli/process.hero:37,49`, `cli/io.hero:34`,
  `emit/literal.hero:41`), and `runtime/hero_os.h` declares **no** plain-`char`
  scalar, array or parameter — every `char` in it is `char *`, which binds as
  `cstr`. So **no route is compiler-need**; all five stand or fall on the thesis
  branch of §1.0, and "it would be safer" is not an entry ticket (§1.12's own
  second consequence).

## Question 1, the one that decides it: does § 13's sentence become FALSE?

§ 13 says a parameter and a field *"are declared at the header's own width and
sign … and one that disagrees is refused"*.

| route | the sentence | why |
|---|---|---|
| **R1** | TRUE but **underivable** | `c8` is the header's sign, but nothing tells the reader that spelling exists. A spec that cannot derive the program is the same failure as a false one. |
| **R2** | TRUE but **underivable** | ditto for `cchar`; the `Type` production admits it as `ident`, so the grammar needs nothing — the *word* needs a home. |
| **R3** | **FALSE** | A `u8` against a signed plain `char` disagrees on sign and is accepted. Route 3 with no spec text is not free: it is the spec ceasing to describe the compiler, which is what this seat exists for. Its honest wording costs **+21 real**, the same as R5. |
| **R4** | TRUE | nothing is accepted that the sentence refuses. |
| **R5** | **FALSE** unless amended | on Darwin and x86-64 plain `char` is signed and the spelling would be `u8`. Hence the sentence, at +21. |

So **only R4 leaves § 13 true unamended, and R4 is the one route design.md
forbids on other grounds.** No route is free of the document.

## Question 2: what routes 1 and 2 cost in § 3

R1 puts a ninth row in § 3's table, which `site/src/lib/tables.ts` reads and the
`named`/`rejected` checks police, and it imports a **target-dependent sign into
the type table every program reads** — the one table whose job is that a width is
a number. It also does not stop there: `to_u8` is a value conversion that aborts
out of range (`selfhost/check/walk.hero:2183`), so a `c8` needs a *bit-preserving*
reread that does not exist. That is R1-full: **+104 real**, and two new names in
§ 11's `Built-ins:` sentence that the `offered` check then has to carry.

R2 keeps § 3 clean, which is right — a type usable only inside a group does not
belong in the table of what values do — but a contextual word is a reserved name,
and `.claude/rules/spec-shape.md` then binds it to a code span, `heroes grammar`'s
output and `suite_grammar.hero`'s keyword comparison. **+49 real**, for a spelling
whose own answer to *"what does a program read it as?"* is `u8` — at which point
it is R5 wearing a keyword.

**Both are Part 7 item 10's form**: a C-vocabulary type whose meaning is a
question about the machine. That row has been argued twice (panels 041 and 052)
and closes with *"A Heroes width is a number; a C width is a question about the
machine."* Its stated falsifier names `long`/`size_t`/`unsigned long`
**parameters** and not plain-`char` fields, so it is **not** literally falsified
by defect 058 — the row still stands, and a sitting that wants a ninth type or a
tenth word must amend **that row**, in design.md, not add a sentence to § 13.
Neither R1 nor R2 came with a measured argument that a form does anything the
21-token sentence does not. That is Principle 0's burden, unmet, and it is where
my veto sits.

## Questions 3 and 4: §1.2, and what actually decides this

**The budget does not decide this sitting, and I say so plainly.** The worst
draft measured is R1-full at 8148 against 10240 — **2092 free**. Every route fits.
A seat that ranked these five on tokens would be using the wrong ruler, and
**the cheapest route and the most robust route cost the same 21 real tokens**.
That measurement is the most useful thing I have: route 3's case was *"cheapest"*,
and it is not cheaper than route 5 by a single token.

**§1.2 against route 3.** Today's rewrite rate on a plain-`char` binding is worse
than 1: defect 059 makes it **unbounded** — the note names the spelling it has
just refused, so a model that obeys the compiler loops. Route 3 ends the loop by
deleting the diagnostic, and buys an ambiguity in exchange: two legal spellings
for one field, in a document that is a prompt, with **no compile error that can
ever catch the wrong one**. §1.2's formula is explicit that a construct which
saves tokens and raises error probability is a net loss; here it does not even
save tokens.

**§1.12 against route 4.** *"The boundary is **complete**: any C library must be
bindable, because a library Heroes cannot reach is C code the author has to keep
writing by hand."* Route 4 makes 23 fields on Debian arm64 and 16 on Darwin
unbindable — `struct dirent`'s `d_name`, `struct utsname`'s six, `sockaddr`'s
`sa_data`. Design.md Part 6 asks a refusal to name the program that would make it
wrong, and **that program is already in the repository**:
`tests/golden/run/ffi-a-char-array-member.hero`, whose own header records that
`uname()` was unreachable at any price until M-complete-structs repaired it. A
refusal born falsified is not a refusal.

## The fifth route, and it is where I land

> **C's plain `char` is a byte, not a signed or an unsigned integer: it is `u8`
> on every target.**

One sentence merged into § 13's existing width-and-sign sentence. **+21 real,
+18 vendored, measured.** No new type, no new word, no § 3 row, no § 11 name, no
grammar production, nothing for `site/src/lib/tables.ts` to colour.

What it buys that no other route does: **one spelling, one meaning, four legs.**
R1, R2 and R3 all make the binding *compile* everywhere; R5 is the only one that
also makes the wrong spelling a *compile error* everywhere — `i8` against plain
`char` is refused on all four legs instead of on one — which is what turns defect
059's note into a `certain` fix (`.claude/rules/diagnostics-and-goldens.md`)
rather than a target-dependent guess.

Where the mechanism lands, from the shared brief's own citations:
`selfhost/emit/extern_field.hero:154` drops the sign conjunct **for the `char`
row only** (the size conjunct and the `(*)[N]` length stay, and `signed char` and
`unsigned char` keep both, so `u8` against `signed char` remains refused);
`selfhost/emit/c_spellings.hero:59` splits the two cases, `"signed char"` → `i8`,
`"char"` → `u8`, which is defect 059 closed in one line.

**Two costs of R5 I will not hide.** On a signed-`char` platform a byte C wrote
as −1 reads as 255 — a deterministic, documented reinterpretation, identical on
all four legs, and the right reading for the 39 fields measured, every one of
which is a text buffer. And it **relaxes exactly what panel 081 wrote down as not
to be relaxed**: `tests/golden/run/ffi-a-char-array-member.hero`'s header says
*"u8[4] against char[4] → refused, `char` is signed here"*. That sentence was true
where it was measured and its *here* is the whole of defect 058. `tests/golden/`
is append-only (`.claude/rules/records.md`), so the correction is **added beneath
it with its date** and the case's field becomes `u8[4]`; it is not rewritten in
silence.

**Instruments, run against the R5-min draft in the copy:** `spec` 16 passed,
4 failed — and the four are `budget`, `spendable`, `real`, `ledger`, i.e. exactly
the pinned-number bookkeeping that `.claude/rules/spec-shape.md` § How a change to
the document is made requires any amendment to repaste in one commit. `grammar`
7/7 and `special` 10/10 passed. So the wording breaks no textual instrument:
`shape`, `anchors`, `offered`, `named`, `rejected`, `inventory` all green.

## What design.md does not cover, said explicitly

**design.md is silent on C's third `char`**: `grep -n "plain \`char\`\|signed char"
docs/design/design.md` returns nothing. §4.19 does not name it, Part 6 has no row
for it, and Part 7 item 10 is about *width*, not sign. So no seat may cite a
ruling here; the nearest binding texts are §1.12's completeness clause and Part 7
item 10's closing sentence, and I have said how far each reaches. Whatever this
sitting resolves owes a `docs/records/log/` entry saying so, because the next
sitting will grep for it and find the silence I found.

## Prediction, registered as payment

Instruments that exist today: the four-leg CI, the net's `run`, `determinism`,
`emission` and `corpus` suites, and `heroes measure`. Scored at **M-arm-platform's
close**:

1. With R5 landed and `tests/golden/run/ffi-a-char-array-member.hero`'s field at
   `u8[4]`, **one source file** compiles, runs and emits byte-identically on all
   four legs — Darwin arm64, Linux x86-64, Linux arm64, Windows x86-64 — taking
   the net from **1825/3** on arm64 Linux to **1828/0**, and `corpus` stays
   **53 passed, 0 failed**.
2. **No `ffi_field_type`, `ffi_parameter_type` or `ffi_return_type` naming a
   plain `char` appears in any leg's log**, and `heroes` proposes `u8` for a C
   `char` on every leg — the same string on all four, which is defect 059's test.
3. `heroes measure spec/heroes-spec.md --refresh` reads **8005 ± 0** real; if it
   reads anything else, the sentence that landed is not the sentence I priced and
   the ledger row is wrong.

Falsified if any leg refuses that file, if a second `extern` shape needs a
different spelling per platform, or if the real count is not 8005.

## Condition

My **veto on routes 1 and 2 lifts** the moment a seat produces a *measured*
program — a real header, compiled — that needs a plain-`char` binding which
`u8`-as-a-byte cannot express, or a measured error-rate argument that a ninth
type or a contextual word prevents a mistake the sentence does not. Until then
the form waits (§1.0), and Part 7 item 10 is the row to amend, not § 13.

My **objection to route 3 lifts** if a seat shows a program whose correct
spelling is `i8` against a plain `char` on some leg — i.e. that fixing the byte
at `u8` loses something real. Nothing in the 39 measured fields is such a case.

My **objection to route 4 lifts** only if §1.12's completeness clause is amended
by the author, which is not this panel's to do.

**No budget veto in any case**: 10240 stands, the spec is 7984, and the most
expensive route on the table leaves 2092 free. Say it in the synthesis, so
nobody chooses route 3 believing the budget asked them to.
