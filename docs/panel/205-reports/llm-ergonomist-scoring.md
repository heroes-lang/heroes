# Panel 205, llm-ergonomist (blind seat): the coordinator's scoring

Written by the coordinator at 03:15 on 2026-10-10 (`date`). Eight fresh
sessions, `claude-opus-5-5`, from 03:14:46 to 03:15:35, 1.7353 USD (0.1861,
0.2358, 0.2196, 0.1905, 0.2279, 0.2376, 0.2135, 0.2244) of the 5.43 the
author approved. Every `context` names the folder's files and the harness's
environment (the account's email among it), no project rule; every reading
stands. The readers' programs are not built: the route they would need (a
first group read before any libc header) exists in no compiler, and on
today's compiler every one of them is refused (the spec-warden's `own` and
`first`, Linux arm64).

| folder | spec | the reader's program |
|---|---|---|
| `m1-a`, `m1-b`, `m1-d` | 203 and 204's sentences | `extern "sched.h"` plainly; all three name `_GNU_SOURCE` as what they are least sure of (*the spec does not say whether the toolchain defines it*, `m1-a`; *the specification has no way to define a macro before the header is read*, `m1-b`), two naming a wrapper header as the rejected choice |
| `m1-c` | the same | `getcpu.h` (`#define _GNU_SOURCE`, then `#include <sched.h>`) named by its one group |
| `m2-a` to `m2-d` | the same and H2f | 4 of 4 write a header of their own defining `_GNU_SOURCE`; `m2-a`, `m2-c`, `m2-d` include `<sched.h>` from it and name it by their one group, `m2-b` names it by its first group and `sched.h` by the second |

**Against the prediction** (`llm-ergonomist-prediction.md`, the spec-warden's
P2): held. With H2f 4 of 4 put the switch in a header of their own named by
the first group; without it 1 of 4 does, and the other 3 bind `sched.h`
plainly while naming the macro as their doubt.

**What it says.** Readers know `sched_getcpu` needs `_GNU_SOURCE` (8 of 8 name
it) and, without a sentence, do not know where Heroes lets them put it: 3 of 4
leave it to the toolchain. H2f's nine vendored tokens move that to 4 of 4 on
the first try. Four readings per arm; the route H2f describes is unbuilt, so
the arm measures what a reader writes, not that it builds.
