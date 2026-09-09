# DEFECTS — the compiler defects that are still open

Read by whoever attacks a defect. Every item is a **measured** failure of the
compiler on a program — a crash, a wrong answer at exit 0, a silence where a
message is owed — carrying its reproducer, its cause where known, and what is
owed. The file exists by author instruction 2026-09-03: one file inside
`docs/work/`, so that everything is tidy.

**Only open defects live here.** The moment one is repaired its entry is ticked,
gains a *The repair* section with the measurements that prove it, and moves to
`docs/work/DONE.md`, the record (CLAUDE.md §3). A repair is owed at the class
and not at the witness, with a `tests/golden/fixedbugs/` case per shape.

**The shape.** One line per item, then the body indented four spaces — a
reproducer, a cause and a measurement are the entry, not decoration. Nothing
lives outside the two banners, and this file is why `records/lists` exists: it
had grown 2753 bytes of prose about five already-repaired defects, every one of
them already in the record.

**Numbers are never reused, and 014 was issued twice** — `docs/work/DONE.md`
carries a Windows-diagnostic defect and an FFI-boundary defect both numbered
014, filed a day apart. A record is not rewritten (CLAUDE.md §14), so the
collision stands there; the next number to issue is **022** — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **022 — a `cstr` built from an expression outlives nothing, and reading it is a use-after-free at exit 0** | `return ("a" + n.to_str()).cstr()` compiles with zero diagnostics and hands C a pointer into freed memory, where the same line with a literal is sound | `spec/heroes-spec.md:253` · `selfhost/emit/operator.hero` · `runtime/parts/alloc.c:151` · `docs/panel/121-the-brace-was-already-taken.md`

    **Origin:** the ffi-pragmatist, panel 121, 2026-09-08, and confirmed by the
    coordinator the same hour on its own 16-line reproducer. It predates the
    sitting: interpolation neither causes it nor is needed to reach it. The
    sitting owns it because a hole would put the sound and the dangling spelling
    one brace apart.

    **The reproducer**, whole, and it needs `HEROES_RUNTIME` set when built
    outside the repository root:

        extern "string.h"
            function strlen(s: cstr) -> u64

        function from_literal() -> cstr
            return "static-and-immortal".cstr()

        function from_built(n: i64) -> cstr
            return ("heap-and-doomed-" + n.to_str()).cstr()

        function main()
            a = from_literal()
            print(strlen(s: a).to_str())
            b = from_built(n: 7)
            print(strlen(s: b).to_str())

    **Measured 2026-09-08 on this Mac.** `heroes build` exits 0 and prints no
    diagnostic under all fourteen flags. Run: **19**, which is right, then
    **0**, three runs of three, where **17** is the answer. Under `--sanitize`:
    `AddressSanitizer: heap-use-after-free`, READ of size 18 at `esc.hero:14`,
    `freed by hero_release_block alloc.c:151`.

    **The cause, as far as it is measured.** The owner slot that keeps a built
    `str` alive is decref'd at function exit, and `spec:253` promises `.cstr()`
    lends the string *"for that call"* only. Inside one function that promise is
    kept and longer; **returned**, the `cstr` outlives its owner. A literal
    escapes it because `HERO_STR_STATIC` gives it static storage and an immortal
    refcount, so the two spellings differ in soundness and not in shape.

    **What is owed:** a refusal at the class rather than at the witness. A
    `cstr` derived from anything but a literal may not leave the function that
    built it, and the diagnostic says which of the two spellings the program
    used. With `tests/golden/fixedbugs/` cases per shape: a returned `cstr`, one
    stored in a record, one pushed into a `[cstr]`, and the literal case that
    must stay legal.

- [ ] **023 — the fix for `mixed_arithmetic` on a `str` reproduces the same error, and never names `to_str`** | following the compiler's own advice on `"count " + n` yields `mixed_arithmetic` again, with `str` and `f64`, and the repair that works is not offered | `selfhost/value_errors.hero` · `tests/golden/check/mixed-arithmetic.expected` · `docs/panel/121-the-brace-was-already-taken.md`

    **Origin:** the spec-warden's steelman at panel 121, 2026-09-08, run out by
    the coordinator the same hour. The seat's point was that `"x" + n` is a
    plausible mistake; the run found the advice for it is a loop.

    **Measured 2026-09-08.** `return "count " + n` with `n: i64` gives
    `error[mixed_arithmetic]` and `fix (guess): convert one side: `to_f64(x)`
    cannot fail, and `to_i64(x)` or another `to_<width>(x)` gives a `T?`
    because the number may not fit`. Following it, `return "count " +
    to_f64(n)` gives the identical diagnostic reading `str` and `f64`, with the
    identical fix. `return "count " + n.to_str()` exits 0, and `to_str` is a
    built-in the spec lists at § Strings, arrays, maps.

    **The class, not the witness.** The fix text is written for two numbers of
    different widths, which is the common case, and is emitted unchanged when
    one operand is a `str`, where no `to_<width>` can help. It is correctly
    tagged `guess`, so it breaks no rule about `certain` fixes; what it breaks
    is §4.17, a diagnostic carrying what is needed to fix the program. This is
    the class `docs/journal/039-closures-verdict.md` names as the one none of
    the three suites watches: a wrong MESSAGE is not a wrong answer, so the
    goldens are content.

    **What is owed:** the `str` case gets its own fix naming `to_str`, and a
    `tests/golden/fixedbugs/` case per operand order — `"a" + n` and `n + "a"`
    — pinning the fix text and not only the message, since `suite_fixes` tests
    `certain` fixes through `.fixed` and nothing watches a `guess` fix's words.

*******************************************************************************
