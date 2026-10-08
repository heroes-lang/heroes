---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: 170db6ba3a83f345022ec9ecb29bbd99c026b10f
github: none
---

- [x] **492 — the emitter's instance lookup scans every function per call** | after defect 418 made it one lookup, it still walks every function for each call (lane b14-emit, unmeasured) | `selfhost/ir/instances.hero` · defect 418 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-emit's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with functions times calls, unmeasured.

    Repaired at `170db6ba`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `instances.Held` holds the program with its functions by declaration, built once per program, and every lookup of a callee in the emitter reads it where it scanned the whole program, first match and last as before: N functions each called once, 500, 1,000 and 2,000, read 8.484, 16.581 and 40.540 billion instructions against 7.183, 11.391 and 19.862, and the compiler's own `--emit-c` 689.013 against 528.883, the C byte-identical.

## The repair

Repaired at `170db6ba`, 2026-10-08 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `instances.Held` holds the program with its functions by declaration, built once per program, and every lookup of a callee in the emitter reads it where it scanned the whole program, first match and last as before: N functions each called once, 500, 1,000 and 2,000, read 8.484, 16.581 and 40.540 billion instructions against 7.183, 11.391 and 19.862, and the compiler's own `--emit-c` 689.013 against 528.883, the C byte-identical.

**Closed 2026-10-08** with batch 15 (lanes b15-parse, b15-check, b15-emit, b15-runtime, b15-harness, b15-deepc, b15-hooks, b15-mutate, b15-box and b15-print, and batch 14's lanes b14-p409 and b14-land197 carried, merged into one round made from the trunk at `56def9b4`; the batch closed on the round as it stood when a reboot at about 11:18 stopped every lane and the author asked at about 16:20 to resume in order, fewer lanes at a time, the round's finished repairs gated and pushed first), its closing gate run on the round at `c02f0840` and the seven suites read red there re-run after their repairs: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 44,867,177 bytes, SHA-256 beginning `dc4079f9bb2026d2`, its fixpoint by `cmp`; the compiler's own tests 1,478, all passed; the net's own tests 318, all passed; the full net 6,971 passed over 29 suites, 0 failed, after three floors were raised to the merged tree's counts (`layout` 527, `permissive` 14, `lines` 398), nine new emissions blessed (defects 451's and 462's cases), defect 462's read check moved `run/fixedbugs-c-writes-over-a-strings-header`'s output (its panic now comes at the read, before the print), a citation in defect 477's case corrected to spec § 9, and `docs/platforms/` made a home of C for defect 463's probe; the census of `check` over 3,038 tracked files moving 15 verdicts and of `--emit-c` moving the C of 2 and 16 refusals, every move attributed (in `check`, 12 to defects 445's and 446's cases and the cases they moved, 3 to defect 504's; the C, defects 219's and 468's per-type touches on the two chains past 32 deep; the refusals, the same 15 and `selfhost/main.hero`, whose emission by the trunk's compiler passed the census's 120 s bound under six parallel jobs, exit 124, not a refusal). The site's build read 188 pages and its claims held. As batch 14's, the formatter's probe by hand and panel 187's R2 replay run after the push, and Linux arm64 and Windows are the CI's legs.
