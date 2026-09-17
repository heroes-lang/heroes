# The generic body that reads the outer level, and what closing it would cost

2026-09-17, M-check-completeness. Defect 056's costing, which panel 160's
compiler-engineer made the condition of its non-veto: *"I move to veto if the
resolution requires closing the generic-body hole, because that is a post-`mono`
check in `selfhost/ir/`, a second layer under §1.7 whose cost nobody has
measured."*

Nobody had. This measures what is measurable today and says plainly which half
is still unrun.

## The hole, reproduced

```
function is_bad<A>(x: A?) -> bool
    return x.is_err()

function main()
    m: {str: i64?} @ {}
    m["a"] @ fail(code: "parse", msg: "no")
    print(f"{is_bad(m["a"])}")
```

`check` **0**, run **0**, prints **`false`** — on a map entry whose stored parse
FAILED. `m["a"]` is an `i64??`; inside the body `x` is an `A?` with `A` still a
type parameter, so the refusal that reads the written type sees nothing to
refuse. The silence of defect 050 passes through a generic intact.

Measured on the compiler at `4d4eec43`, before option E landed. It reads the
same under the prototypes of E and of A, which is the whole point: **no option
panel 160 weighed closes it.**

## THE LIBRARY DOES NOT REACH IT, and that was the open question

Panel 160's synthesis filed *"does the LIBRARY itself contain a reader on `A?`
that instantiates to a nested read?"* as UNRUN, and called it the question that
decides whether this is theoretical or one `find` away. Read, in
`selfhost/library_source.hero`, all three generics that could:

```
function find<A>(xs: [A], f: (function(A) -> bool)) -> A?
    for x in xs
        if f(x)
            return ok(x)
    return fail(code: "not_found", msg: "no element satisfies the test")

function any<A>(xs: [A], f: (function(A) -> bool)) -> bool     # returns f(x)
function all<A>(xs: [A], f: (function(A) -> bool)) -> bool     # returns !f(x)
```

**Not one applies a reader to `A`.** `find` constructs — `ok(x)`, `fail(…)` — and
`any` and `all` hand the element to the caller's own predicate and return its
`bool`. `map`, `filter` and `fold` do the same. So the library cannot
instantiate the hole; it can only hand a caller a nested value, which is the
`find` door option E already closes at the CALL SITE:

```
xs: [i64?] @ [ok(1)]
hit = find(xs, failed)
print(f"{hit.is_err()}")      # option E: check 1, nested_read
```

**The hole is therefore reachable only from a generic the AUTHOR writes**, whose
body reads `A?` with a one-level reader. Live instances: **0** in `selfhost/`
(which declares no generics at all), **0** in the corpus.

## What closing it would cost, and the half that is unrun

**Where it would go.** Not the checker: the body is checked once, with a
`.generic` payload, before any binding exists. `selfhost/check/walk.hero:1802`
already records `c.out.instantiations[span.end] @ resolved_args` keyed by the
CALL-SITE span, and `walk.hero` already walks `keys(c.out.instantiations)` — the
seam panel 155 was convened about, for the same shape one rule over.

So there are two shapes, and they are not the same size:

- **At the call site, from the recorded instantiation.** Per `(decl, type
  parameter)`, record the obligations the body incurs — *read by `.is_err()`* is
  one — and check each against the concrete type at every call. That is panel
  082 R3's shape, never built, and it is the one that fits the existing seam.
- **After `mono`, over the monomorphised IR.** `selfhost/ir/mono.hero` is 368
  code lines against a DECIDED ceiling of 380, so a check of any size goes in a
  new module. This is what the compiler-engineer's veto is aimed at, and it is a
  second layer under design.md §1.7.

**UNRUN, and it is the whole costing.** Neither has been built, so no line count
and no `check`-time figure exists for either. What IS measured: the call-site
table exists and is walked today, so the first shape adds a per-`(decl, param)`
obligation set and a walk, not a new pass. Whoever builds it owes
`/usr/bin/time -p` on `heroes check selfhost/main.hero` before and after, on a
still machine.

## Why it is filed rather than repaired

Zero live instances, in a compiler that declares no generics and a corpus that
declares eight — of which the two that carry an `A?` use `.len()` and `==`. The
rule panel 160 adopted closes every door a program in this repository can reach
today. **What this record exists for is that the next sitting starts from a
measurement instead of from the argument**, and that the veto's condition is on
the record as a question with a shape rather than as a worry.
