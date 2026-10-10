# Panel 204, llm-ergonomist (blind seat): the coordinator's scoring

Written by the coordinator at 01:33 on 2026-10-10 (`date`). Six fresh
sessions, `claude-opus-5-5`, from 01:31:51 to 01:33:02, 1.4520 USD (0.2976,
0.3041, 0.1870, 0.1958, 0.2256, 0.2421) of the 10 the author approved. Every
`context` names the folder's files and the harness's environment (the
account's email among it), no project rule; every reading stands. The
readers' repaired programs were built by the coordinator with the trunk's
compiler (`.claude/worktrees/scratch-b15/p204/blindcheck/`).

| folder | task | the reader's answer | today |
|---|---|---|---|
| `h1-a`, `h1-b` | predict four programs | both: `limit_ab` **`3 10`**, `limit_ba` `3 10`, `jpeg_ab` **refused**, `jpeg_ba` refused; both choose *each group compiled against its own header alone*, from § 4's *Declaration order never matters* and § 13's *against that header*, and both write today's behaviour as the rejected reading | `3 50`, `3 10` (with clang's warning), 1, refused |
| `h2-a`, `h2-b` | repair `jpeg_ba` | both move the `stdio.h` group above, reading *as this program's own C includes it* as source order; both doubt it against § 4; both builds print 1 | the right repair |
| `h3-a`, `h3-b` | repair `rec` | both write a wrapper `jpeg_stdio.h` (`#include <stdio.h>`, `#include <jpeglib.h>`) and name it for the jpeg group, both concluding *the order of groups does not decide what clang sees*; both builds print 1 | a working repair resting on a false model |

**Against the prediction** (`llm-ergonomist-prediction.md`): clause 1 held
in its first half (2 of 2 predict `limit_ab` and `limit_ba` print the same)
and was falsified in its second (no reader predicts `jpeg_ba` builds; both
predict `jpeg_ab` refused, the other side of the same reading); clause 2
held; clause 3 held in its first half (neither names the emission order) and
its third (both write a header of their own) and was falsified in its second
(no reader adds a function to the `stdio.h` group); clause 4 held.

**What it says.** Today's spec leads 2 of 2 readers to a model the compiler
does not have: per-group isolation, read off § 4's sentence. Under it they
mispredict 2 of 4 programs, a value (`limit_ab`) and a verdict (`jpeg_ab`),
and both name the compiler's real behaviour as the reading the spec rules
out. Given today's message, readers repair `jpeg_ba` correctly while doubting
the repair against § 4; given the record-only shape, where the written order
is already right, they conclude order does not count and work around it with
a wrapper header, which builds. Two readings per cell; no arm compared two
texts of the spec, which the next arms do with the spec-warden's drafts.

## The second round, scored at 01:53 on 2026-10-10 (`date`)

Twelve sessions from 01:51:21 to 01:52:34, 3.1219 USD; the sitting's blind
seat 4.5739 USD in all of the 10 approved. Every `context` names the folder's
files and the harness's environment only. The writers' programs built by the
coordinator with the trunk's compiler
(`.claude/worktrees/scratch-b15/p204/blindcheck2/`).

| folder | spec | the reader's answer |
|---|---|---|
| `j1-a`, `j1-b` | F7 | both: per-group isolation again, read off F7 and § 13's *against that header*: `limit_ab` **`3 10`**, `limit_ba` `3 10`, `jpeg_ab` **refused**, `jpeg_ba` refused; `j1-b` names a compiler refusing an order-dependent program as a third reading, triggered by neither program |
| `j2-a`, `j2-b` | F7 and G1m | both: `limit_ab` `3 50`, `limit_ba` `3 10` with clang's warning, `jpeg_ab` 1, `jpeg_ba` refused, *C reads `a.h` and then `b.h`*: today's behaviour, 4 of 4 programs; neither predicts the `limit` pair refused, neither names a contradiction between F7 and G1m |
| `k1-a` to `k1-d` | F7 | 4 of 4 write the `stdio.h` group above `jpeglib.h`; `k1-a` and `k1-c` build and print `missing`; `k1-b` and `k1-d` are refused `ffi_unknown_name`, a `record` tagged `_IO_FILE`, glibc's name, on this Mac (not the order) |
| `k2-a` to `k2-d` | F7 and G1m | 4 of 4 write the `stdio.h` group above; 4 of 4 build and print `missing` |

**Against the prediction**: j1 held; j2 held in its first half (both predict
`jpeg_ab` builds and `jpeg_ba` refused) and was falsified in its second (no
reader predicts the `limit` pair refused or names the contradiction); k, the
spec-warden's P3: 4 of 4 in k1 and 4 of 4 in k2 write the `stdio.h` group
above, so by its own rule *if both arms reach 3 or more, G1m comes out*.

**What it says.** For writing, G1m changes nothing measured: 8 of 8 readers
put `stdio.h` first with it and without it. For reading, it changes the
reader's model of the language: under F7 alone 2 of 2 read per-group
isolation and mispredict `jpeg_ab`, a correct program, as refused; under F7
and G1m 2 of 2 read the compiler's real include order and predict all four
programs as today's compiler builds them. No reader of either arm foresees
that a module whose two orders mean different things is refused: a refusal
the detector would add is told by its message, not by any draft read here.
Two readings per predicting cell, four per writing cell.
