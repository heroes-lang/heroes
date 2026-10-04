# Clang already knew

2026-09-19. Panel 166, at M-declared-extents, on the two defects panel 165 found
under route 6.

## The decision

| | |
|---|---|
| date | 2026-09-19 |
| decision | **route H is adopted** — the lend emits `const void *` and clang refuses the write with the flag this project already ships — and **route C in its C-emitted form**, a `_Static_assert` the probe writes. Route G is refused on three vetoes |
| reason | the engineer's objection to refusing an immutable lend was that *the language cannot see whether C writes*. True of the language, false of the toolchain: clang reads it from the header, and `emit/field_lend.hero` is discarding that `const` today |
| design.md § | §1.12 (rank 3, memory corruption), §4.19, §1.6 |
| panel | 166, provisional — author ratification pending; supersedes panel 165's resolution 3 |

## The route no seat listed, measured end to end

```
$ clang -std=gnu11 -c h.c -Werror=incompatible-pointer-types-discards-qualifiers
h.c:6:39: error: passing 'const void *' to parameter of type 'void *' discards
  qualifiers [-Werror,-Wincompatible-pointer-types-discards-qualifiers]
./immut.h:3:34: note: passing argument to parameter 'p' here
```

The read compiles clean, the write is refused, and the note points at the
header's own declaration. `selfhost/cli/flags.hero:106` already carries the flag
and the net asserts it at `:213`. **Zero spec tokens, no new mark, no grammar, no
parameter mode** — and it leaves green the two `.ptr()` sites the engineer
measured route A would over-refuse.

`emit/field_lend.hero:17-22` rejected this on a dichotomy its own comment states,
always-const or never-const. The critic measured the third answer: **the header
decides, per call.**

## Four contradictions, all settled by running something

- **Route B's veto stands and is wider than it stated**: `@p: ptr` already emits
  a `void **` out-parameter, and `@` is occupied at the call site too. **But the
  veto kills the spelling, not the ground** — and route H reaches the ground
  without the spelling.
- **`__has_attribute(access)` is 0** (the historian predicted it; the coordinator
  settled it), **and the check does not need it**: plain C11 gives
  `note: expression evaluates to '64 <= 8'`. So *"route E is load-bearing"* does
  not follow — the assertion's right-hand side is `sizeof(…)`, C's own number.
  And a group `constant` extent lowers to a **function call**, so a Heroes-side
  check would refuse the careful reader and accept the careless one's literal.
- **`spec § 3` is not false.** The lent binding reads 65; a copy, a record field
  and an array element all read 72. Exactly one sentence is false, § 5's, and
  § 13 licenses what falsifies it — the document is contradictory rather than
  incomplete. And the hole is **wider than `=`**: an immutable parameter's field
  is written too.
- **`getcwd` is writable today**, and the coordinator's brief said it was not.

## A fourth defect, which no brief and no route touched

**The lend has no lifetime rule.** C parks the address in a static, the frame
dies, and a later call reads it: `1` at exit 0, `AddressSanitizer:
stack-use-after-scope` under the sanitizer. The compiler refuses the two escapes
*Heroes* can see and nothing looks at the C side;
`examples/ledger/db/sqlite.hero:107` is the same shape with a real library behind
it. Panel 164 opened the address route and did not carry the lifetime rule
across, though `spec § 13`'s lease already owns its shape. **Defect 066.**

## And the corrections the sitting made to its own coordinator

Five of them, all run by a seat or the critic: *"leaves `getcwd` unwritable"* is
false; the denominator was wrong three times over and all three measured the
wrong population, which is **4 call sites in 1 file**; *"exit 133"* does not
reproduce at any overshoot from 9 to 4096 bytes, so **every lie corrupts in
silence** rather than a big one trapping; `_LIBC_COUNT` expands to nothing only
*without* `-fbounds-safety`, and with it the SDK enforces against the real
`getcwd`; and defect 065's **one-line summary** still claimed § 3 after its body
had been corrected — the half every instrument reads.
