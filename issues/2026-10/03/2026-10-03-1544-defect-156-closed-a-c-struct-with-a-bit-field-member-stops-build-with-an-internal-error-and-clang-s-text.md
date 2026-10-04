---
kind: defect
area: emit
milestone: none
filed: 2026-10-02
commit: 826ddc2f06a582675137e6cdf427e97eddb4df68
github: none
---

# Defect 156 closed: a C struct with a bit-field member stops `build` with an internal error and clang's text

- [x] **156 — a C struct with a bit-field member stops `build` with an internal error and clang's text** | `extern "bf.h"` over `typedef struct { int32_t kind; uint32_t flag : 1; uint32_t rest : 31; } BF;` with `record BF` naming `kind: i32`, `flag: u32`, `rest: u32`, reading `make_bf().kind`: `check` exit 0, `build` exit 2, *internal error: compiling the generated C failed: ... invalid application of 'sizeof' to bit-field* at the field assertions and *address of bit-field requested* in the generated hash | `selfhost/emit/` (the field assertion, `heroes-ffi-field`, and the record's descriptor) · panel 073's item 3 (bit-fields filtered before the assertion) · **class: blocking** · **closed 2026-10-03**

    **Origin:** panel 186's completeness critic, 2026-10-02, in its first
    pass over the briefs (`docs/panel/186-briefs/probes/critic/bf.h`, `bf.hero`);
    reproduced by the coordinator at 11:02 on `ae08ed93`.

    **Why it is a defect.** Exit 2 is the compiler blaming itself for a
    binding the author can be told about, and clang's text reaches the
    author (`.claude/rules/c-boundary.md`). Panel 073 resolved that a
    bit-field is filtered before its assertion; what such a field binds to,
    if anything, is panel 186's question beside defect 151.

    **2026-10-02, lane land186, a bit-field refused on its own line at
    build, and one left out sent to `partial`** (panel 186 R5): repaired at
    `afd07f00`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close, and the platform legs before the push.

    **2026-10-02, lane land186, spec § 13's *A bit-field is none of these:
    leave it to `partial`.*** (panel 186 R6): at `b7c510c5`, gated by its
    own cases; the rest is owed at the round's gate.

    **2026-10-03, its cases read one by one, held for the Windows box**: all eight, `run/fixedbugs-156-records-beside-bit-fields-build` and `unsupported/fixedbugs-156-*` 7 of 7, passed on this Mac (00:30 to 00:42 by `date`) and in the Linux arm64 container (one run, 00:36 to 00:54, `libcurl` 8.14.1 answering there), with `docs/panel/186-briefs/probes/critic/bf.hero` told `ffi_field_type` on each bit-field at exit 1 on both. On the Windows box `unsupported/fixedbugs-156-curl-s-hsts-entry` binds `curl/curl.h`: whether it ran there is unread, the box not answering on 2026-10-03, and the leg of `6bec7c8c` (`unsupported` 113 at 0 failed against 119) leaves exactly one skip between it and defect 158's fourteen cases, if the box has neither `pkg-config` nor raylib as its install list says. The other seven name only their own header over `stdint.h` and passed there, by the same totals. The item waits for the box to say whether the curl case ran.

    **2026-10-03, read one by one at `02e507bc` with the Windows box answering, and the author's answer *A***: all eight passed on this Mac and in the Linux arm64 container, under clang 22.1.8 and again under 18.1.8; on the Windows box seven passed and `unsupported/fixedbugs-156-curl-s-hsts-entry` was skipped, built alone there reading `ffi_missing_header` on `curl/curl.h`; `bf.hero` was told `ffi_field_type` on each bit-field at exit 1 on all three. The author's answer *A* (`docs/records/log/2026-10-03-1123-the-author-answers-a-a-case-a-platform-cannot-run-is-judged-where-its-header-is.md`): the curl case is judged where curl is, and the item gains a twin, a case on a header beside the program carrying curl's bit-fields, closing once that twin has run on all three.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): exit 2 and clang's
    text for a binding the author can be told about.

    **Closed 2026-10-03** after the push of `e339ece9`. Repaired as the lines above record, and given its twin of the curl case by lane twin156 at `3a3b6521`, it entered the trunk at the round of 2026-10-03's sixth gate, `da3e29af` (lanes warn, ffimsg, depth, twin156 and win214): the seed regenerated once, 39,690,755 bytes, SHA-256 beginning `2c809845ed0f7ba8`, its fixpoint by `cmp`; the compiler's own tests 1,099 and the net's own 200, all passed; the full net, 26 suites, 5,033 passed and 0 failed; the censuses of `check --brief` over 1,905 files and of `--emit-c` over 595, every move attributed to its lane. Its cases read one by one at `da3e29af`, whose compiler `e339ece9` carries unchanged (no file under `selfhost/`, `seed/`, `runtime/` or `tests/` moved between them): on this Mac (Apple clang 21), in the Linux arm64 container under Debian clang 22.1.8 and again under 18.1.8, and on the Windows box (clang 23.1.1): `run/fixedbugs-156-*` 1 of 1 and `unsupported/fixedbugs-156-*` 8 of 8 on this Mac and in both Linux arm64 runs; on the Windows box 1 of 1 and 7 of 8, `fixedbugs-156-curl-s-hsts-entry` skipped there for `curl/curl.h` and its twin `fixedbugs-156-a-tagged-struct-shaped-as-curl-s-hsts-entry` passing there. By the author's answer *A*, the curl case is judged where curl is, green on this Mac and under both Linux clangs. The push's legs: Linux arm64, its suites four at a time, the compiler's own tests 1,099 all passed and 20 suites at 0 failed under each clang; the Windows box, the compiler's own tests 1,099 all passed and nine of its 20 suites at 0 failed before it went offline at about 14:47 (Tailscale, read at 15:41: last seen 54 minutes before), every case above already read there; and the CI's run 37124159403 on `e339ece9`: Linux x86-64 (Ubuntu clang 18.1.3), the compiler's own tests 1,099 all passed and 26 suites, 4,993 passed and 0 failed; Windows x86-64 (clang 20.1.8), 1,099 and 26 suites, 4,932 passed and 0 failed, the box's eleven unrun suites among them; Linux arm64 and Darwin arm64 green. Which golden cases those jobs ran their logs do not say, so the readings above are the measurement of them, and x86-64's an inference from Linux arm64 under the same clang major. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
