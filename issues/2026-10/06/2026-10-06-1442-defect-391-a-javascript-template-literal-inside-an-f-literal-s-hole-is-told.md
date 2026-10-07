---
kind: defect
area: compiler
milestone: none
filed: 2026-10-06
commit: 7092f6667ae42b15737b6c25eb101f9fbb511657
github: none
---

- [x] **391 — a JavaScript template literal inside an f-literal's hole is told one message per character** | panel 187's R2 mutant `1980e403a92d6cb3` (`interp-js`) writes `` m[`ключ${x}`] `` inside a hole of `print(f"...")` in `tests/golden/run/fixedbugs-an-interpolated-string-holds-characters-above-ascii.hero`: the trunk's compiler at `0f48f9f9` told one message, `hole_not_renderable`, about the hole's type; batch 12's round at `43e6be50` tells seven `unexpected_character`, the opening backtick, each of the four Cyrillic letters, the `$` and the closing backtick, under `check` and under `check --permissive` alike (R2's differential at batch 12's gate, the round arm run by the coordinator from 13:54:11, the differential written at 14:26:36 on 2026-10-06, `FINDINGS: 2`, the two arms of this one mutant) | the lexer's reading of a hole's text, `selfhost/lex_interp.hero`, and its `unexpected_character` told per character where a run is one mistake · panel 187 R2 · **class: adjacent**

    **Origin:** panel 187's R2 instrument, rebuilt after the restart of 2026-10-06, its round arm at batch 12's gate; the mutant's lines read by the coordinator at 14:42.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): seven true messages for one mistake, a second message for one mistake; the trunk's one message named the hole's type rather than the backticks. Into batch 13 under the author's instruction of 2026-10-05.

    Repaired at `7092f666`, 2026-10-06 (lane b13-front), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `7092f666`, 2026-10-06 (lane b13-front), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

**Closed 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.
