# M-agreed-retention — one C function, one story about who frees what it is handed

**Opened 2026-09-21**, at M-declared-extents's close, because that milestone's
last four open items had nowhere to live: a closed milestone's file is a place
nobody looks, and `records/homes` says so in as many words. Every one of them
was found by a completeness critic attacking a landed rule at the shapes beside
it, and not one is a defect: each is a shape the language ADMITS and says
nothing about.

**What the milestone before it left standing.** M-declared-extents gave the
boundary two marks and a report. `counted_by` relates the number a lend crosses
with to the field it crosses into; `lent` says a C parameter keeps nothing of
what it is handed, under a default the author flipped so that silence means
*keeps*; and where no word can reach — C frees the bytes from a callback, or
from a later call with no pointer at all — the runtime names the leases that
were live, on all three platforms. What it did NOT do is make two declarations
of one C function agree, or say whose job it is to free a pointer C made.

**Why these are one milestone and not four filings.** Three of the four are the
same question asked at three distances: *whose word about a C function is the
one that counts*. A binding's marks are a claim nothing checks, so two modules
may claim opposite things; one declaration cannot carry a contract chosen per
call; and a pointer C made has no rule at all about who frees it once. The
fourth is a process rule that the closed file could not keep, parked here with
its reason rather than lost.

*******************************************************************************
**OPEN: 4**

- [ ] **M-agreed-retention** | a pointer C made, handed twice to a C function that frees it, is `check` 0 and dies at run time saying nothing, and nothing in § 13 says a pointer C made is C's to free once | panel 172's compiler-engineer and completeness critic, `spec § 13`

    **Origin:** panel 172, 2026-09-21. The compiler-engineer measured it as the
    shape beside the double give-away of a lease (`p10`: `p = make()`,
    `release(p: p)` twice, exit 0 at `check` under both compilers) and the
    critic as `b_out`, an `@out: cstr` C fills and frees and the program then
    frees. **Not this sitting's**, both said, and filed here so it is not
    discovered as new: the runtime's report adopted at 172 covers the LEASE
    class and is silent here, because no lease is live and the pointer is C's.
    Whether the document owes the sentence that a pointer C made is C's to free
    once, whether that is design.md §1.12's business or C's, and whether the
    runtime's handler can reach it, are unmeasured.

- [ ] **M-agreed-retention** | two modules may declare one C function with contradictory retention marks, and the compiler accepts both at `check` 0 | panel 171's completeness critic, the historian's B.4, `selfhost/check/marks.hero`

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


- [ ] **M-agreed-retention** | one Heroes declaration cannot reach all three of `sqlite3_bind_text`'s retention modes, so the shipped ledger and measurement 037 declare the same C function two incompatible ways | panel 170's completeness critic, `examples/ledger/db/sqlite.hero`

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

- [ ] **M-agreed-retention** | `/panel`'s working rules do not say that a seat's tree copy is its own, and two seats shared one scratchpad in one sitting | `.claude/skills/panel/SKILL.md`, `.claude/rules/verification.md`

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
