- [ ] **228 — the emitter finds each instruction's line by walking its source line by character, so a long line costs its length squared** | lane irverify's profile: `writer.at_span` → `source.locate`/`line_col`, 65% of `concat-chain-2000`'s build after defect 218's repair (`<scratchpad>/lane-irverify/`, 2026-10-03) | `selfhost/emit/writer.hero`, `selfhost/source.hero` · **class: improvement**

    **Origin:** lane irverify, 2026-10-03, profiling the shapes of panel 184's R6 after defect 218's repair, reported to the coordinator; not yet run by the coordinator.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): every one of R6's 25 shapes builds at 2,000 on this Mac and in the Linux arm64 container after 218's repair, so this is a cost past the floor, not a program that fails.

    **2026-10-03, lane b8-emit, a `#line` asks the source for its file and line and walks no line**: repaired at `17322f0e`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Counted on an instrumented copy of each compiler's own C, the characters the column walk covers in `build --emit-c`: concat-chain-2000 24,188,468 to 286, index-chain-2000 92,310,458 to 286, for-nested-1000 80,378,910 to 286; seven more emitter sites built `#line` text through `locate` and ask the same lookup.
