# Defect 077 closed: a handle given back after C handed its address out again is refused at check through its name, and stopped before C through a copy that was not reused

2026-09-25, M-agreed-retention step 14, panel 177's items 1 to 3 on their
provisional resolution, in the integration lane `a7388f1f`, merged `0e7a61be`.
Found by panel 175's spec-warden (its F1), with the allocator-independent
reproducer from that sitting's completeness critic.

- [x] **077 — a handle given back after C has handed its address out again is `check` 0 and `run` 0, and the release lands on the new handle** | the live set keys on the address, so a stale handle that equals a live one is accepted as the live one, and the one correct release that follows is the call that aborts | `runtime/parts/alloc.c:436` · `spec § 13` · **closed 2026-09-25**

    **Origin:** panel 175's spec-warden, 2026-09-23 (its F1), reproduced by the
    coordinator the same day before filing.

    **The reproducer**, over `r1.h`'s `box_open`/`box_close` (both `noinline`,
    `malloc` and `free`):

        extern "r1.h"
            record Box tag box
            function box_open() -> Box acquires box_close
            function box_close(b: Box consumes)

        function main()
            b = box_open()
            box_close(b: b)
            c = box_open()
            print("same address: ", b == c)
            box_close(b: b)
            print("after the second give-back of b")

    `check` 0; `run` 0, zero bytes of stderr, three of three on Darwin arm64,
    printing `same address: true`. The seat measured five of five at `-O0` and
    `-O2`, and with `box_close(b: c)` added at the end the program aborts at that
    call, the one correct release in it (134, 396 bytes, three of three).

    **That reproducer depends on the allocator, and this one does not** —
    panel 175's completeness critic, 2026-09-23: two binaries from
    byte-identical C, differing only in their UUID and code signature, read 0
    and 134, because the signature decides whether libmalloc hands the freed
    block straight back. The critic's `u1_static.hero`, over a C `f_open` that
    returns one static cell every time and an `f_close` that frees nothing,
    is the one to repair against: `f_open`, `f_close(x: a)`, `b = f_open()`,
    `print(a == b)`, `f_close(x: a)` reads `true` and exits 0 with zero bytes,
    three of three, rerun by the coordinator.

    **Why it is a defect.** Spec § 13 says *the live handles are a set, so
    giving one back twice aborts on its own*, and here a handle given back twice
    does not. `hero_handle_consumed` compares the address (`alloc.c:436`) and the
    comment at `:411-414` already says the set holds one entry per address. What
    it does not say is that the second release of a stale handle then frees a
    live one in silence.

    **Linux, run by the coordinator:** `run` 0, three of three, `same address:
    true`, on arm64 and x86-64 alike. **Under `--sanitize` it is caught**, 134,
    *given back that were never taken*, on both, and that is the sanitizer
    HIDING the defect rather than finding it: ASan's quarantine does not hand a
    freed address straight back, so `c` gets a different one and the stale `b`
    is a stray. **Unrun:** Windows.

    **Added 2026-09-24, with defect 088's correction:** the stale `b` is the
    copy design.md Part 8 wart 20 says outlives a consuming call, and what this
    defect adds to the wart's class is the reuse — the copy is not merely
    dangling, it names a live handle, so no check on the address alone can tell
    them apart. The spec sentence `a747e5a2` landed, *unless C has since reused
    its address*, states that limit; §1.12 is why it cannot be the answer.

## The repair

Three halves, each where panel 177 put it, and none alone is enough.

- **The checker's half, route M (must).** `check/moved.hero` and its four
  neighbours run one flow analysis inside each function: a binding whose life
  a call ended on every path to a read is refused at that read,
  `handle_used_after_end`, naming the call and its line. Both reproducers
  above hand `b` (or `a`) back through the name that was ended, so both are
  refused before the program exists. `==` is a read like any other.
- **The poison.** When the live set's count for an address reaches zero, the
  place the ending call read its argument from is overwritten with a pointer
  to a page the runtime maps with no access, and every crossing of a handle
  into C is checked, callback results included. A binding the checker cannot
  follow, a field or an element ended through a helper, aborts before C.
- **The dead set.** The runtime remembers the last 2^20 addresses whose life
  ended and clears a mark when C hands the address out again. A copy made
  before the end that reaches C while its address is still dead aborts before
  C, with a message that names the copy.

## What stays a limit, as panel 177's item 4 states

A copy made before the end, given back after C has handed the same address
out again, is indistinguishable from the new life, and the release lands on
it. Measured at the close over the critic's static cell with a copy:

| program | exit | what happens |
|---|---|---|
| `a2 = a`, `f_close(x: a)`, `b = f_open()`, `f_close(x: a2)`, `f_close(x: b)` | **134** at the last line | the release through `a2` ended `b`'s life, so the one correct release is the one that stops, with the dead set's message |
| the same without the last line | **0**, zero bytes of stderr | the release through the stale copy ended the new life and nothing says so |

That is the class § 13 now states in words, *unless C has since handed its
address out again or a million other lives have ended*, and design.md Part 8
wart 20 carries it. `limit-a-copy-read-after-c-reused-its-address-is-not-caught`
pins the READ through such a copy at exit 0; **the release through one, the
shape of this defect's own title, is measured above and not yet pinned by a
golden**. Under `--sanitize` ASan's quarantine keeps the address from coming
back, so the dead set catches the copy there.

## The measurements

Measured by the coordinator on the trunk after the merge, `./heroes` rebuilt
from the merged seed, Darwin arm64, each plain and with `--sanitize` alike.

| program | before, as filed on 2026-09-23 | after |
|---|---|---|
| the reproducer over `r1.h`, `box_open`/`box_close` | `check` 0, run 0, `same address: true` | `check` **1**, `handle_used_after_end` at 10:29, the `==` |
| the critic's `u1_static`, one static cell | `check` 0, run 0, `true` | `check` **1**, `handle_used_after_end` at 10:11 |
| `check/fixedbugs-a-handle-given-back-after-its-address-was-handed-out-again` | — | refused at the read, annotated |
| `run/dead-handle-given-back-after-its-address-was-handed-out-again`, `run/dead-handle-compared-after-its-address-was-handed-out-again` | — | **134** before C, the poison's message |
| the 19 `run/dead-*` and 4 `run/limit-*` goldens, with the other 21 run goldens the landing adds or changes | — | 264 runs, plain and `--sanitize`, on Linux x86-64, Linux arm64 and the Windows box: every exit and every stdout as its golden says |

Gate in the lane, the combined compiler as the harness's own binary: the
compiler's 720 tests; the net's 24 suites green; the net's own 167; the seed
regenerated at runtime ABI 25 and the fixpoint held. On the trunk after the
merge: the index equal to the lane's tree, the compiler's 720 tests.
