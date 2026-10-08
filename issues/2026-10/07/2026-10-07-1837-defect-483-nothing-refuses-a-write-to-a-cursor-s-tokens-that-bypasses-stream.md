---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: 0e3bdd7758748f4622ad4739f8e52705fe4cead6
github: none
---

- [x] **483 — nothing refuses a write to a cursor's tokens that bypasses `stream_tables.relaid`** | defect 258's kept walk rests on every token rewrite going through `relaid`, which asserts the new kind is invisible; a `layout` rule refusing other writes to `c.tokens[...]` would hold the premise (lane b14-parse) | `tests/harness/suite_layout.hero` · defect 258 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-parse's final report (*decisions* 1); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a premise held by one assertion, no rule.

    Repaired at `0e3bdd77`, 2026-10-08 (lane b15-harness), gated by its cases and the net's own tests; the net is owed at the batch's close. `layout/streams` (`tests/harness/stream_writes.hero`) refuses any store into, or lend of, a place running through a `tokens` field other than `stream_tables.relaid`'s, whatever the holder is named, the lexer's own state exempt by its declared type: a write planted in place of `parse/orphans.hero:54`'s `relaid` reads `layout` 5 passed, 0 failed on the base's suite and red here naming the line; the tree holds no such write.

    Widened at `8943002f`, 2026-10-08 (lane b15-harness), the same gate: a `relaid` call broken over lines, one argument a line as `heroes fmt` breaks a long call, was refused as a write, since its argument line names no callee; the check now reads an argument line's lend as the innermost open call's, so of a broken `relaid`, a broken other call and a `relaid` nested in one only the other's lends are told, where the first version told the two `relaid` lines too.

## The repair

Repaired at `0e3bdd77`, 2026-10-08 (lane b15-harness), gated by its cases and the net's own tests; the net is owed at the batch's close. `layout/streams` (`tests/harness/stream_writes.hero`) refuses any store into, or lend of, a place running through a `tokens` field other than `stream_tables.relaid`'s, whatever the holder is named, the lexer's own state exempt by its declared type: a write planted in place of `parse/orphans.hero:54`'s `relaid` reads `layout` 5 passed, 0 failed on the base's suite and red here naming the line; the tree holds no such write.

**Closed 2026-10-08** with batch 15 (lanes b15-parse, b15-check, b15-emit, b15-runtime, b15-harness, b15-deepc, b15-hooks, b15-mutate, b15-box and b15-print, and batch 14's lanes b14-p409 and b14-land197 carried, merged into one round made from the trunk at `56def9b4`; the batch closed on the round as it stood when a reboot at about 11:18 stopped every lane and the author asked at about 16:20 to resume in order, fewer lanes at a time, the round's finished repairs gated and pushed first), its closing gate run on the round at `c02f0840` and the seven suites read red there re-run after their repairs: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 44,867,177 bytes, SHA-256 beginning `dc4079f9bb2026d2`, its fixpoint by `cmp`; the compiler's own tests 1,478, all passed; the net's own tests 318, all passed; the full net 6,971 passed over 29 suites, 0 failed, after three floors were raised to the merged tree's counts (`layout` 527, `permissive` 14, `lines` 398), nine new emissions blessed (defects 451's and 462's cases), defect 462's read check moved `run/fixedbugs-c-writes-over-a-strings-header`'s output (its panic now comes at the read, before the print), a citation in defect 477's case corrected to spec § 9, and `docs/platforms/` made a home of C for defect 463's probe; the census of `check` over 3,038 tracked files moving 15 verdicts and of `--emit-c` moving the C of 2 and 16 refusals, every move attributed (in `check`, 12 to defects 445's and 446's cases and the cases they moved, 3 to defect 504's; the C, defects 219's and 468's per-type touches on the two chains past 32 deep; the refusals, the same 15 and `selfhost/main.hero`, whose emission by the trunk's compiler passed the census's 120 s bound under six parallel jobs, exit 124, not a refusal). The site's build read 188 pages and its claims held. As batch 14's, the formatter's probe by hand and panel 187's R2 replay run after the push, and Linux arm64 and Windows are the CI's legs.
