# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item here asks **what should be true**, and until it
is answered the compiler goes on behaving some way by default. That default is
the cost of leaving an item open, so each item names it.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/work/DONE.md`. A file that keeps its
own closed items stops being a list of what is owed — this one held **138 ticked
items and zero open ones** on 2026-08-26, under eighteen headings still titled
`## Open`, which is why the rule now lives in `/decide` itself instead of in
prose about `/decide`.

**One notation: `- [ ]`.** No section headings, no prose sections, no
strikethrough. A finding written as a bare bullet is invisible to every count in
this project, and nine of them sat in exactly that shape under two panel
headings here until 2026-08-26 — **five already closed** and two of those
measurably stale, while the file reported itself empty.

Two rules bind this file, both learned the hard way:

- **Verify before asking.** An entry is a claim from the day it was written, and
  entries outlive their causes. Asking the author about a settled question is the
  one cost this list cannot pay. Of the nine findings recovered on 2026-08-26,
  five were closed and were ticked with what closed them rather than asked.
- **Rank by what it blocks**, never by age, and say the blocker in the question.

Format: `- [ ] <origin> | <what> | <where to look> | <why it matters>`

- [ ] **Ratify: the FFI pointer verdict goes strict, and the compiler learns the header's parameter types** (`panel 103`, 2026-09-03, M-robustness-guards step 3; provisional under CLAUDE.md §4) | **Five seats, none for doing nothing; the two that compile corrected the brief on one point and the resolution took it.** What is provisionally live and lands in ONE step: `-Werror=incompatible-pointer-types` as the 14th flag (never a `-Wno-`); `(void *)&x` on **every** `@` argument to an `extern` — call, probe and `_Static_assert` — because the flag alone refuses `time(@t: i64)` and `getline(@n: u64)` on macOS only (`long *` against `long long *`, measured by both compiling seats); the compiler's own pointee check, width AND sign, by one clang process per cold unit (`-ast-dump=json` on `extern __typeof__(name) hero_ty_name;`, 0.02 s, same shape on clang 18, 21, 22) and two `_Static_assert`s at the declaration's `#line`, so `@n: i32` and `@n: i64` for `size_t *` are `ffi_parameter_type` at exit 1 everywhere and `@n: u64` is accepted; spec w5 at **3718 (+33 net)**, landing in the same commit as the check and never before; Q3 as the 21st `ffi_*` code with two texts; Q4 (`-Werror` on all C) **not** adopted; and, by the author's mid-sitting instruction, a **clang 18 floor** enforced by `heroes doctor` and the build. Recommendation: **ratify as resolved** | docs/panel/103-the-ffi-pointer-verdict-goes-strict.md · selfhost/cli/flags.hero:28-40 · selfhost/emit/ops.hero:213-260 · selfhost/emit/extern_probe.hero · selfhost/emit/assert_spelling.hero:191-197 · spec/heroes-spec.md:209-211 | a program's FFI verdict differs by host clang today, and a write out of bounds is waved through with a warning that `--sanitize` cannot see

- [ ] **Ratify: deep recursion stops with a word — the guard-page handler lands, the depth counter does not** (`panel 104`, 2026-09-03, M-robustness-guards step 4, soundness lane; provisional under CLAUDE.md §4) | **Both seats approve (a) with conditions and reject (b).** What is provisionally live: `runtime/parts/stack.c` installed from `hero_args_set` (no ABI change for this step), `SIGSEGV` and `SIGBUS` on a 256 KiB `mmap`ed alternate stack with a `PROT_NONE` page, **two witnesses** (`si_addr` in the guard region AND `sp` within 4 KiB of the bound — one witness gave a false abort on a wild store), the previous disposition **saved and chained** (the brief's *restore and return* killed a library's write-barrier at 138, measured), nothing installed under `--sanitize`, a frame walk that names the function (`panic: stack exhausted in d100000.down`, exit 134), `_GNU_SOURCE` and `-rdynamic` on Linux, a Windows VEH arm measured on the box before the commit, threads recorded as a live hole. (b) rejected: +55–60% per call at `-O0`, and at `-O2` it blocks the recursion-to-loop transformation (0.81 s against 0.000 s). Recommendation: **ratify as resolved** | docs/panel/104-deep-recursion-stops-with-a-word.md · runtime/parts/panic.c · runtime/parts/os.c:80 (`hero_args_set`) · site/src/html/why.html:197 | a language whose goal is that programs do not crash, crashing without a word on a five-line program on all three platforms
