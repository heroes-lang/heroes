- [ ] **272 — the layer-0 hook calls a check case *does not parse* when its intended diagnostic makes `fmt` refuse it** | writing `tests/golden/check/fixedbugs-225-a-climb-to-the-root-is-a-known-cost.hero`, whose `machine_locked_path` is annotated: the hook answered *does not parse; the compiler says: ... error: refusing to format a file with diagnostics*, exit 2, while `check`, `annotations` and `fixes` read the case 1 and 0 each (2026-10-04); every case holding a parse-stage refusal, `ffi-a-group-head-names-not-locates` on the trunk's compiler among them, draws the same | `.claude/hooks/fmt_check.py:86` to `:94` · **class: improvement**

    **Origin:** the coordinator, 2026-10-04, writing defect 225's pin.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument's false alarm on every such case, which teaches its reader to pass it by; no program moves.
