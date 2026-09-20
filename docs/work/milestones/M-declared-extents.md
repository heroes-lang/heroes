# M-declared-extents — the extent a C header spells, and whether the compiler should check it

**Opened 2026-09-18**, as `docs/ROADMAP.md` row 61, on panel 164's route 6. It is
the only row in the chain that was scheduled **behind a measurement rather than
behind a decision**, in the sitting's own words: *"Its whole value is a number
nobody has measured: how many real headers spell the parameter as an array.
Queued, not adopted, because adopting an unmeasured route is the cheap move."*

**What the route is.** `function arr_len(s: i8[8])` inside an `extern` group: the
parameter carries the extent exactly as the C header spells it, and the Heroes
compiler **checks** what is passed against it. Panel 164 found it needs no new
expression, no lend, no built-in and no position rule — one widening of §4.19's
parameter list, with ordinary type identity doing the matching. It is the only
one of the seven routes where the extent is checked rather than trusted to the
author or to the callee.

**What warrants it.** design.md §1.11 — everything comes from C — and §1.12,
robustness, which is what a checked extent buys over an unchecked one.

## The measurement is run, and it is `docs/measurements/035-not-one-byte-array-is-spelled-the-same-way-on-both.md`

Four numbers, because the question had a platform in it that the sitting had not
noticed:

| | Darwin arm64 | Linux arm64 | Linux x86-64 |
|---|---|---|---|
| headers walked | 3120 | 5404 | 5411 |
| parameters spelled as an array | **82** | **182** | **182**, the same set |
| of those, with a **fixed** extent | 31 | 59 | 59 |
| fixed extent, in **base** system headers | **20** | **25** | 25 |
| fixed extent **and byte-typed** | 12 | 31 | 31 |
| byte-typed and fixed **on both platforms** | **0** | **0** | **0** |

**The finding is not the count.** `L_tmpnam` is **1024 on Darwin and 20 on
glibc**, and glibc is the only one of the two that spells `tmpnam`'s parameter as
an array. An author who mirrors the header in front of them writes
`function tmpnam(s: i8[20])`, and the compiler checks the 20 — soundly about the
program and falsely about the world. `if_indextoname` is the same shape reversed:
glibc spells the extent, Darwin writes `char *`.

**And the language already holds the portable answer.** `spec § 13`'s header
constant was run on both platforms, by a Heroes program built from the seed on
each, and gives 1024 and 20 respectively. Route 6 would put a literal where a
working mechanism already puts the header's own value.

**Principle 0, from this tree.** 172 `extern` functions are declared here.
Exactly one is spelled with an array parameter by a real header — `tmpnam`, glibc
only — and that declaration is a diagnostic fixture that declares `buffer: ptr`.
No working binding in the repository would change.

## What the measurement does not decide, and why this milestone stays open

The number prices the route; it does not rule on it, and the ruling is a panel's
because it changes `spec § 13` and what the checker refuses (CLAUDE.md § 4).
Three things the sitting will have to weigh, each of them measured above rather
than argued:

- route 6 **does** serve `pipe(int[2])`, `futimens`/`utimensat`'s
  `struct timespec[2]` and the `drand48` family, which both platforms spell
  alike — none of them bytes;
- it serves **no byte parameter that both platforms spell as an array**, and
  bytes are what panel 164 sat about;
- where it is most tempting, it invites a **platform-varying literal**, which is
  the failure the header constant exists to prevent.

**The route that nobody has listed** is also recorded here rather than left to be
rediscovered: Darwin states extents in an **annotation on a pointer**,
`_LIBC_COUNT(L_tmpnam)`, nine times in the census corpus. Whether Heroes should
read that annotation is a different question from route 6, and no sitting has
named it.

**Windows was unrun when this file was opened and was walked on 2026-09-19**, on
the box the author powered on: the C library spells **zero** array parameters and
Win32 spells 89, of which 2 are byte-typed with a fixed extent. `L_tmpnam` is
**260** there, against 1024 and 20. The record is
`docs/records/done/2026-09-19-1030-the-fourth-platform-spells-none-in-its-c-library.md`,
and it states that the Windows leg used a weaker instrument than the other three.

## What the milestone has landed, and the prediction it registered

**Three defects the sittings found under route 6 are closed**: 064 (2026-09-19,
the guard was a list), 065 (route H — the lend's `const`, and the temporary's
spelling), 063 (route C — `counted_by`, and the number judged three ways).
**066 stands** and has its own sitting.

**The registered prediction that pays for the spec's +48**, instruments that
exist today, scored at this milestone's close:

> A `.ptr()` lend at a parameter declaring `counted_by n` with an extent the
> call states as a constant emits exactly one `_Static_assert` per lend and
> **no runtime compare**; one whose extent is read at run time emits **no
> assertion and exactly one compare**. Instrument: `./heroes build <program>
> --emit-c` over this step's three goldens, counting `heroes-ffi-extent` and
> `hero_panic(` lines. The falsifier is a lend that emits both or neither.

**Route 6 itself is refused and stays refused** (panel 165, on two vetoes): a
parameter's extent is erased by C, so route 6 would compare the author's
declaration to the author's argument. What the milestone delivered instead is
the relation declared where C can check it.

*******************************************************************************
**OPEN: 3**

- [ ] **M-declared-extents** | two modules may declare one C function with contradictory retention marks, and the compiler accepts both at `check` 0 | panel 171's completeness critic, the historian's B.4, `selfhost/check/marks.hero`

    **Origin:** panel 171's completeness critic, 2026-09-20, attacking the
    `lent` rule at the shapes beside it. **Filed rather than fixed**, because it
    needs a rule ACROSS modules that no sitting has priced, and because the mark
    it concerns lands in commit A of the flip and does not exist yet.

    **What it is.** Module one declares `function keep(s: cstr lent)`, module two
    declares `function keep(s: cstr)`, both against the same header. Each is
    internally consistent, the two disagree about C, and nothing compares them.
    **This is the exact shape that broke upstream Clang's `noescape` on its first
    day** (2017-09-19): a third-party re-declaration disagreed with the SDK
    header's mark and the build failed. Heroes has no header to disagree with,
    only two `.hero` files, so the disagreement is silent.


- [ ] **M-declared-extents** | one Heroes declaration cannot reach all three of `sqlite3_bind_text`'s retention modes, so the shipped ledger and measurement 037 declare the same C function two incompatible ways | panel 170's completeness critic, `examples/ledger/db/sqlite.hero`

    **Origin:** panel 170's completeness critic, 2026-09-20, re-run by the
    coordinator before filing. **It is not a defect and is filed here rather than
    in `docs/work/DEFECTS.md`**, because both directions are LOUD: `d: ptr` takes
    `nullptr` and refuses a function name at `check`, and the null that reaches C
    where C calls it back is a named runtime panic; `d: (function(ptr) -> ())`
    takes the function name and refuses `nullptr` with `error[type_mismatch]` at
    `check`. Nothing is silent and nothing corrupts.

    **What it is** is an expressiveness gap at design.md §1.11's own boundary: a
    real C parameter whose argument may legitimately be a null OR a function has
    no single Heroes spelling, so a binding author must pick one mode and lose the
    other. `SQLITE_STATIC` is a null function pointer, which is why the canonical
    keeps-the-pointer call and the give-away call cannot be written against one
    declaration.

- [ ] **M-declared-extents** | `/panel`'s working rules do not say that a seat's tree copy is its own, and two seats shared one scratchpad in one sitting | `.claude/skills/panel/SKILL.md`, `.claude/rules/verification.md`

    **Origin:** panel 170's completeness critic, 2026-09-20. **Filed rather than
    fixed, because CLAUDE.md § 4 says the skills are amended by author
    instruction and not by a panel.**

    Two seats shared one scratchpad and one rebuilt the other's compiler
    underneath it, which produced an emission divergence a seat reported as a
    question and the critic then traced to a stale binary. Independently, the
    compiler-engineer found its own first copy six commits behind and re-ran
    everything. **Twice in one sitting**, and it is
    `.claude/rules/verification.md` § *The compiler that judges is a build
    artifact* arriving inside a sitting rather than in a gate.

*******************************************************************************
