# twoarity — one C symbol, two bindings, no new syntax

**This fixture exists because the same fact was rediscovered three times in one
sitting, after a brief asserted it impossible** (panel 094, 2026-08-26). It is
here so that the fourth person reads a program instead of re-deriving it.

## The claim it refutes

> *"One C name cannot carry two arities, so a variadic function can be bound
> exactly once… without a rename there is no way to spell the C name apart from
> the Heroes name."*

That entry sat in `docs/work/DECIDE.md` for weeks and became the whole stated
case for adding a rename clause to the language. **It is false.** What was
actually measured is that `function f(…) = "printf"` does not **parse** — a fact
about syntax — and the *capability* was inferred from it. CLAUDE.md §1 calls that
shape by name: **a failed search written as an impossibility.**

During panel 094 it was falsified independently by the spec-warden (on `printf`),
by the ffi-pragmatist (on `curl_easy_setopt`, against the real `curl/curl.h`),
and by the coordinator. Three files, exit 0.

## Why it works

- **Two `extern` groups in two modules may name one C symbol.** The probes are
  mangled per module (`hero_ffi_probe_h_asint_printf` and
  `hero_ffi_probe_h_astext_printf`), which is what panel 053 bought when it found
  `redefinition of 'hero_ffi_probe_getenv'` at exit 2.
- **The call goes through the header's own prototype.** §4.19 refuses to
  re-declare, so C sees one `printf` — the real one — and Heroes sees two
  bindings of it. Nothing about the two Heroes declarations reaches the C
  compiler except as type checks.
- **Each module routes through a Heroes function**, because acceptance row 1
  forbids reaching an `extern` across a module boundary (panel 033 R5).

## What it costs, measured

Eight lines across two files, against the three lines in one file a rename
clause would take. That five-line saving, priced against **+23 spec tokens** at
the cheapest honest wording, is what panel 094 refused under Principle 0.

## The pair of verdicts

- `main.hero` runs, exit 0, printing `n = 42` and `s = forty-two`. Pinned by a
  `suite_surface` row.
- Nothing here is refused, and that is the point: the fixture's whole content is
  that this compiles.
