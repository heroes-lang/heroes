# The number had a platform in it

2026-09-18. M-declared-extents step 1, the milestone `docs/ROADMAP.md` scheduled
**behind a measurement rather than behind a decision**: panel 164 queued its
route 6 with the words *"its whole value is a number nobody has measured."*

## The decision

| | |
|---|---|
| date | 2026-09-18 |
| decision | M-declared-extents **opens**, its measurement is run and recorded as `docs/measurements/035-not-one-byte-array-is-spelled-the-same-way-on-both.md`, and route 6 stays **unadopted**: the ruling is a panel's, and this milestone's first open item convenes it |
| reason | the number is not one number. 82 array-spelled parameters on Darwin, 182 on Linux, and of the byte-typed ones with a fixed extent **the set both platforms spell alike is empty** |
| design.md § | §1.11 (everything comes from C), §1.12 (robustness is what a checked extent buys) |
| panel | 164, route 6, queued — this entry does not rule on it |

## The instrument had to be found before the number could be

**C deletes the extent before clang builds a tree.** `T a[N]` is adjusted to
`T *a` (C11 §6.7.6.3p7), so `void f(char b[8])` and `void f(char *b)` are the
same declaration. Measured first, because it decides everything after:

```
f_ptr    {"qualType": "char *"}        f_arr8   {"qualType": "char *"}
f_arr2d  {"qualType": "char (*)[8]"}   f_static {"qualType": "char *"}
```

Only the 2D case keeps a bound, because only the outermost dimension decays. **An
AST-shaped instrument would have answered zero to a question it was not asking.**
What survives is the parameter's **source range**, so the census reads the
header's own bytes back through it — a text screen over every `.h`, then clang
per candidate.

Coverage was reported rather than assumed, and the first run was **40 of 127**.
The 87 failures were diagnosed instead of excused: **80 wanted a prerequisite
header**, not C++ and not Objective-C. A thirteen-header prelude took it to
127 of 127, and to 333 of 333 and 334 of 334 on the two Linux legs.

## What it found

| | Darwin | Linux arm64 | Linux x86-64 |
|---|---|---|---|
| parameters spelled as an array | 82 | 182 | 182, **the same set** |
| with a fixed extent | 31 | 59 | 59 |
| fixed, in base system headers | 20 | 25 | 25 |
| fixed **and byte-typed** | 12 | 31 | 31 |
| byte-typed and fixed **on both platforms** | — | **0** | **0** |

The two Linux legs were differenced as sets and not compared as counts: 0 rows on
each side. Two thirds of Linux's lead is Debian's package list — 140 OpenSSL
headers against the Xcode SDK's **0**, and 20 of the byte cases are one file's
`unsigned char ivec[16]`.

## The finding is the divergence, not the count

```
Darwin  _stdio.h:289   char *_LIBC_CSTR tmpnam(char *_LIBC_COUNT(L_tmpnam));
glibc   stdio.h:211    extern char *tmpnam (char[L_tmpnam]) __THROW __wur;

L_tmpnam = 1024 on Darwin,  20 on glibc          (compiled and run on each)
```

One function, one C standard, two spellings, and **two different numbers**. An
author mirroring the header in front of them writes `function tmpnam(s: i8[20])`,
and route 6's compiler checks the 20 — sound about the program, false about the
world. `if_indextoname` is the same shape reversed: glibc spells
`char __ifname[IF_NAMESIZE]`, Darwin writes `char *`.

**And the portable mechanism already ships.** `spec § 13`'s header constant, run
on both platforms by a Heroes program built from the seed on each, gives 1024 and
20. Route 6 would put a literal where a working rule already puts the header's
own value.

**Principle 0, from the tree:** 172 `extern` functions are declared here, exactly
one is spelled with an array parameter by a real header, and that one is a
diagnostic fixture declaring `buffer: ptr`. No working binding would change.

## And the ROADMAP said two things that were not true

Found by opening row 61, which is the act the chain's own preamble says catches
this: **§ Where we are named the next row `M-core-packages`**, which is row 63 —
row 61 is `M-declared-extents`, entered at M-readable-bytes's close. And the
preamble said **the table is 79 long** when it is **81**, because that same close
entered two rows and restated a sentence it had not recounted.

Both are corrected with their date rather than quietly rewritten. It is the
failure that preamble describes about itself — *the one number in this file that
no instrument reads* — arriving for the third time, and the third time it was an
OPENING that found it.

## And the gate was judged by a compiler older than the tree

The full net, run before the push, read **1862 passed, 23 failed** against a
ROADMAP claiming 1891 and 0. The tree was right and the **binary** was wrong:
`heroes` is `.gitignore:15`, this one was built at 02:08 against a HEAD of 18:17,
and `git status` cannot report a file it is told to ignore. Rebuilt from the seed
in **2.97 s**, the same net read **1891 passed, 0 failed**, `real 948.31`.

**Not one of the 23 lines named the cause.** The `spec` pair said `STALE: the
recorded count is for 6bdb9b497a141864 and this file is 3c065c560426eb07` — the
old binary carrying the old pin and concluding, correctly from where it stood,
that the DOCUMENT had moved; `selfhost/measure/pinned.hero:64` had held the new
digest all along. A golden said `no function named validated_bytes`, which reads
as a missing built-in. Both were true sentences pointing away from the cause.
The rule now lives in `.claude/rules/verification.md` § The compiler that judges
is a build artifact, because a lesson written only in a record is performed by
nothing.
