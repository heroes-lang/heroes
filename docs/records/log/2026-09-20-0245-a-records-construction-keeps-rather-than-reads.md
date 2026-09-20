# A record's construction keeps rather than reads

2026-09-20. M-declared-extents step 8, repairing defect 067 — which panel 167
filed and which its own resolution requires be repaired FIRST, because the
field lease it adopts rests on this mechanism.

## The decision

| | |
|---|---|
| date | 2026-09-20 |
| decision | **a lend and a lease name may not stand as an argument of a record's CONSTRUCTION**, which is the one call whose argument outlives it. One predicate beside `argument_of`, read by both sweeps; no new diagnostic code; a second note on each of the two existing ones |
| reason | panel 122's clause blesses an argument position because a call READS the bytes and returns, and a constructor does not return, it KEEPS. Measured before the clause was written: the record constructor is the **one** door — a variant case, an array literal, a map literal and `ok(x)` are all refused today — so the rule names that shape rather than every construction, which would have been the tidy rule and the wrong one |
| design.md § | §4.19, §4.20, §1.12 |
| panel | 167, provisional; this is clause 3 of its resolution |

## The defect was filed as half of itself, and the shapes beside it said so

It was filed on the LEASE, because that is what the critic ran. Attacking the
shapes beside it before repairing found the **lend** in the same door:
`return Box(p: s.cstr())` parks a pointer into a `str` that dies with the frame
— `check` exit 0, the honest 72 read back as 0, `heap-use-after-free` under the
sanitizer.

And `check/lending.hero`'s own module doc **claims it closes that shape**: *"That
is what closes `xs.push(s.cstr())` and `Rec(field: s.cstr())`."* It does — for a
record OUTSIDE a group, through the clause refusing a `cstr` field there. Inside
a group that clause steps aside on a premise that is true of a header's field and
false of a lend: *there C owns the bytes*. **A true sentence in a module doc,
about a rule that had a hole the sentence could not see.**

## No new code, and what was added instead

The class is one — a pointer outliving the bytes it points into — and
`cstr_escapes`, `field_lend_escapes` and `lease_escapes` already say it. A second
name for one question is a panel path, which is `ffi_parameter_position`'s own
reasoning one module over.

**What each diagnostic gained is a second note, because the first one blesses
the shape now refused.** *"Pass it directly as an argument of the call that reads
it"* sends a reader straight into `Box(p: ...)`. The new sentence says a group's
record is not such a call, and names the two routes out.

## The cut, along the seam the host names in its own first paragraph

The repair put `check/lending.hero` at 331 lines against §11's 300.
`no_cstr_out_of_heroes` and `no_cstr_in_a_record` went out to
`selfhost/check/lend_decls.hero`: they read a declared RESULT and a declared
FIELD and walk the declarations, so they were never position questions — the
seam that first paragraph names, and the third thing to leave along it after the
typing went to `check/lend_types.hero` at panel 164.

**And it is where the growth is going**: panel 167's clause 2 widens both from
`cstr` to `ptr`, without which the field lease is not sound.

## Twice too wide, and both narrowings were measured rather than reasoned

**The `ptr` half is redundant.** `Box(p: s.b.ptr())` reported two codes on the
SAME span before the narrowing, because a `ptr` lend has stood only as an
argument of an extern call since this afternoon and a constructor is never one.
The clause is the `cstr` lend's and the lease's.

**And a record already condemned by its own field does not get a second
message.** `holds_a_refused_field` steps aside there, which is `emit/ffi.hero`'s
direction at defect 062. A record whose fields are legal — `record Box { p: ptr }`
checks clean today — is still refused, so the stepping-aside is about a condemned
record and not about whose record it is.
