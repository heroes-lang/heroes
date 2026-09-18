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

**Windows is unrun.** Two platforms of four were walked.
`.claude/rules/platforms.md` would have the other two run rather than reasoned
about, and the census script takes a header root as its argument, so the cost is
a run and not a rewrite.

*******************************************************************************
**OPEN: 2**

- [ ] **M-declared-extents** | convene the panel on route 6, carrying measurement 035 as the brief's numbers rather than an estimate | `docs/measurements/035-not-one-byte-array-is-spelled-the-same-way-on-both.md`, `docs/panel/164-the-read-had-a-door-and-the-write-had-none.md` § Three routes nobody listed

    **Origin:** panel 164's resolution point 4, 2026-09-18, *"Route 6 is QUEUED
    behind its measurement"*. The measurement is run; the queue is what is left.
    The sitting is a language change, so CLAUDE.md § 4 binds it, and the brief
    carries the four platform numbers above and the `L_tmpnam` divergence.

- [ ] **M-declared-extents** | walk the census on the Windows headers and on the fourth leg, so the route is priced on four platforms rather than two | `scratchpad/extents/census.py`, `docs/ref/environment/windows/WINDOWS-MACHINE.md`

    **Origin:** this milestone's step 1, 2026-09-18. The script takes a header
    root as its argument and reported 127/127 and 333/333 coverage on the two it
    has walked, so what is missing is a run on a box, not an instrument. A
    platform fact is run on a platform or it is an inference.

*******************************************************************************
