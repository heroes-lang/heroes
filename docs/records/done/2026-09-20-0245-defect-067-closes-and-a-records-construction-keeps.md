# Defect 067 closes, and a record's construction keeps rather than reads

2026-09-20, M-declared-extents step 8. Found by panel 167's completeness critic,
which asked whether the mechanism three seats proposed to replicate for fields
is itself sound. No seat asked.

- [x] **067 — a lease pointer parked in a group's record survives its own release** | `end_lease` empties the cell and the copy inside an `extern` record keeps pointing at the freed bytes, which C then reads at exit 0 | `selfhost/check/lending.hero`'s `no_cstr_in_a_record`

    **Origin:** panel 167's completeness critic, 2026-09-20, asking whether the
    mechanism three seats proposed to replicate for fields is itself sound. It is
    not. No seat asked.

    **Reproducer**, and the record is a group's:

        x: cstr @ s.lease()
        b = Box(p: x)              # a record constructor IS a call
        end_lease(@x)              # the cell is emptied; the copy is not
        print(to_str(read_it(p: b.p)))

        check   exit 0
        run     prints 0 where 72 is honest
        --sanitize: heap-use-after-free

    **Why the door is open.** Panel 122's clause permits a lease name *"as an
    argument of a call"*, and a record constructor is a call. The other four
    escape shapes are correctly refused; this one is not. And
    `cstr_in_a_record`'s own note blesses it — *"a group's `record` may hold a
    `cstr`, because there the fields are the header's and C owns the bytes"* —
    which is true of a header's field and **false of a lease**, whose bytes the
    program owns and frees.

    **What is owed.** The exemption asks the wrong question: it asks whose record
    it is, and the fact that decides is whose bytes they are. **It is repaired
    before panel 167's route A lands**, because that route rests on this
    mechanism.

    **CORRECTION, 2026-09-20, by the session that repaired it, before the repair
    was written.** The entry above is a LEASE defect and the class is wider: the
    **lend** reaches the same door. Measured on the trunk:

        function make_box() -> Box
            s = "Hi" + " there"
            return Box(p: s.cstr())      # a pointer into a str that dies here

        check   exit 0
        run     prints 0 where 72 is honest
        --sanitize: heap-use-after-free

    `check/lending.hero`'s own module doc claims it closes `Rec(field:
    s.cstr())` — and it does, for a record **outside** a group, through the
    clause that refuses a `cstr` field there. **Inside a group that clause steps
    aside**, on the premise quoted above. So the defect as filed named the half
    the critic happened to run, and the shape beside it is where what it
    actually is became visible (CL-078).

    **Closed by** `M-declared-extents step 8`, at the class: a record's
    construction is not a call a lend or a lease name may stand in.

## The rule, in one sentence

**A record's construction is the one call whose argument outlives it.** The
position rule blesses an argument position because a call READS the bytes and
returns; `Box(p: x)` does not return, it KEEPS — the pointer becomes a field of
a value the statement hands on.

## The four shapes beside it, run BEFORE the clause was written

A repair is attacked at the shapes next to the one that provoked it, and here
that is what decided the rule's width: the record constructor was the **one**
door.

| shape | before the repair |
|---|---|
| `Box(p: x)`, a group's record | **check 0, corrupts** |
| `Box(p: s.cstr())`, the lend into the same | **check 0, corrupts** |
| `V.one(p: x)`, a variant case | already refused |
| `[x]`, an array literal | already refused |
| `{1: x}`, a map literal | already refused |
| `ok(x)` | already refused |

So the clause names a **record's construction** rather than widening to every
construction, which would have been the tidy rule and the wrong one.

## The repair, and it was twice too wide before it was measured

`keeps_its_argument` sits beside `argument_of` in `check/lending.hero`, and both
sweeps read it — the `cstr` lend's and the lease's — because both reach one door
and one rule cannot live in two files. It asks the **callee's own declaration**,
so a construction form the AST gains tomorrow is refused by default rather than
admitted in silence.

**The first draft applied it to the `ptr` lend too, and that was redundant.**
Measured: `Box(p: s.b.ptr())` reported `field_lend_escapes` AND
`field_lend_needs_a_header` **on the same span**, `10:19` twice. A `ptr` lend has
stood only as an argument of an EXTERN call since panel 166's clause landed this
afternoon, and a record's constructor is never one — so the header clause
already refuses every construction, and the new one is the `cstr` lend's alone.
Two messages on one span is exactly what this rule exists to avoid.

**And the first draft fired where the record's own field is already refused.**
`record Holder { c: cstr }` outside a group cannot hold the pointer at all, so
the construction cannot legally exist: reporting both told the reader to fix two
things where there is one, and the second disappears the moment the first is
fixed. `holds_a_refused_field` is what steps aside, and it is `emit/ffi.hero`'s
own direction at defect 062 — *"the type error above is the true one… it waits
for the author to fix the type and run again."* It does **not** reach a record
whose fields are legal: `record Box { p: ptr }` checks clean today, and a lend
parked in one is still refused.

**No new diagnostic code, and that is a decision rather than an omission.** The
class is one — a pointer outliving the bytes it points into — and
`cstr_escapes`, `field_lend_escapes` and `lease_escapes` already say exactly
that. A second name for one question is a panel path, which is
`ffi_parameter_position`'s own reasoning one module over.

**What the two diagnostics gained is a second NOTE**, because the first one
blesses the shape the clause now refuses: *"pass it directly as an argument of
the call that reads it"* sends a reader straight into `Box(p: ...)`. The new
sentence says a group's record is not such a call, and names the two routes out
— hold the cell and pass it to each call, or hold the `str` and lend at each.

## The cut this forced, and it is the seam the host names in its own first paragraph

The repair put `check/lending.hero` at **331 lines** in `suite_layout.hero`'s
unit against §11's 300, and `layout` said so. The two clauses that went out —
no Heroes function answers a lent pointer type, no record outside a group holds
one — **are not position questions at all**: they read a function's declared
result and a record's declared field, and they walk the declarations rather than
the expressions. That is the seam the host's first paragraph names, and it is
the third thing to leave along it, after the typing went to
`check/lend_types.hero` at panel 164.

`selfhost/check/lend_decls.hero` is also the file panel 167's resolution is
about to grow: clause 2 widens both from `cstr` to `ptr`, without which the
field lease it adopts is not sound. So the cut lands where the growth is going.

## Gates

`check` **129**, `run` **129**, `emission` **468**, `determinism` **158**,
`annotations` **168**, `surface` **111**, `warnings` **189**, `lines` **130**,
`corpus` **55**, `fixes` **25**, `unsupported` **15**, `canonical` **2**,
`layout` **2**, `order` **3**, `records` **24**, `spec` **20**, `grammar` **7**,
`descriptors` **220**, the compiler's own **672** and the net's own **158** — on
a compiler built from the regenerated seed, 859,172 lines, the fixpoint verified
byte-identical. The wide set was run because a change to what the compiler
REFUSES is judged by every golden tree, and it earned itself twice: two goldens
in the tree were refused by the new clause and both are amended rather than the
clause loosened.

## The two goldens the clause reached, and why neither is an over-refusal

**`tests/golden/fixedbugs/a-header-field-that-is-not-const.hero`** filled a
group record's `cstr` field from a Heroes string literal. That is defect 067's
own shape, and the literal case was already the author's decision of 2026-09-09
— *the strict rule over the escape-only one*, because `"lit".cstr()` and
`("a" + n.to_str()).cstr()` are one brace apart and what separates them is
invisible at the call site. The case keeps its subject: the `__typeof__` cast it
guards is still emitted three times, measured, identical to the blessed file.

**`tests/golden/unsupported/ffi-a-cstr-field-blames-the-field.hero`** was
refused at CHECK before it could reach the clang verdict it exists to prove.
That is an ordering fact worth keeping: a check-time refusal preempts a
build-time one, so a case about a header disagreement must not reach it through
a lend. Both now fill the field with `nullptr`, which reaches each subject
unchanged.

**What is NOT lost**: a `cstr` C gave the program still fills such a field, so
the capability § 13 describes is untouched. What is refused is parking *Heroes'
own bytes* there, which is the defect.
