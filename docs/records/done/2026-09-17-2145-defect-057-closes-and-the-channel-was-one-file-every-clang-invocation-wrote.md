- [x] **057 — `warnings` blamed one program for another's clang warnings, once, and the reproducer had not been found** | `warnings/curl under build` reported three warnings naming `tests/golden/ir/regression-extern-parameter-types`, a program curl does not touch | `selfhost/cli/units.hero` · `selfhost/cli/toolchain.hero` · `tests/harness/suite_warnings.hero`

    **Origin:** 2026-09-17, the full net run before the CL-078 commit, filed
    without a reproducer on purpose. Closed the same day, by the author's
    instruction to start its repair.

    **The channel was a file name, and the entry named the four lines it was
    on.** `build/clang-stderr.txt` was where EVERY clang invocation in a tree
    wrote its stderr: the four sites are `units.tu_object`,
    `units.link_objects`, `toolchain.runtime_object` and `toolchain.link`. The
    file is opened `O_WRONLY | O_CREAT | O_TRUNC` (`runtime/parts/run.c:248`),
    so one `heroes` process is safe — each compile truncates before its own
    clang writes, and reads its own output back. **Two processes in one tree are
    not.** A second build landing between this clang's exit and the two reads
    that follow substitutes its warnings for this one's, and both readers take
    them: `toolchain.run_clang` prints them as this process's own stderr, which
    is exactly what the suite captures, and `tu_object` then writes them into
    the TU's `warnings.txt`, which the warm-cache replay repeats on every later
    build under this program's name.

    **What was measured, and it is the half the entry called unexplained.**
    `build/tu-08401abe1c35d47d/` — the directory named in the report — is
    created by `heroes build --emit-c` on that fixture, which is the verb the
    `ir` and `determinism` suites run over `tests/golden/ir/`. Reproduced: the
    directory comes back with that exact key, and compiling its `.c` by hand
    with the build's own flags reproduces the warning text word for word,
    `-Wabsolute-value` on the `_Static_assert` the FFI probe emits for `abs`,
    at `:28:29`. So *something compiled that TU* is answered. `heroes build` on
    the same file creates no TU at all: it stops at `no_entry_point`, exit 1,
    measured.

    **What was NOT reproduced**, stated rather than implied: the interleaving
    itself. Two builds were run concurrently against one tree, thirty
    iterations each, one of them a program whose header carries a `#warning` so
    that every compile of it warns and still succeeds; no warning crossed. The
    window is between a child's exit and the read that follows it, and thirty
    tries did not land in it. **The repair does not rest on that measurement**:
    it removes the shared name, and a path no second process writes cannot be
    substituted whatever the timing.

    **The repair.** Every clang invocation writes beside the artifact it is
    about: `dir + "/clang-stderr.txt"` for a translation unit, whose directory
    is already unique per module, per text and per flag set; `object +
    ".clang-stderr.txt"` for the runtime; `c_file + ".clang-stderr.txt"` for
    the single-unit link; and `build/link-<digest of the binary>-clang-stderr.txt`
    for the whole-program link, digested so the name stays inside `build/`
    rather than beside an author's `-o`, which may be anywhere. Measured after
    the repair: `build/clang-stderr.txt` no longer exists after a build, four
    per-invocation files do, the TU's own file carries its own warning, and the
    warning still reaches the building program's stderr. **The one collision
    left is named rather than hidden**: two builds compiling the identical TU do
    share its directory, and their clang output is identical too.

    **And the instrument the entry asked for, which is the other half.**
    `suite_warnings.hero` now tells a misattribution from a finding: a warning
    naming a `.hero` file outside the program's own directory is reported as a
    misattribution, naming both programs and pointing at the shared-file shape,
    instead of being read as that program's defect. The rule it rests on is
    spec § 1's — a `use` starts at the directory of the file being compiled, so
    every module a program reaches is under that program's directory — and a C
    or header path is deliberately not evidence, since a program may warn about
    a header it includes from anywhere. Two test cases, one of each kind.

    **Verified 2026-09-17**: the compiler's own tests 654 passed; the net's own
    tests 157 passed; `canonical` 2, `layout` 2, `order` 3, `records` 24,
    `warnings` **184 passed 0 failed** — the suite that carried the false
    positive — `corpus` 55, and the seed regenerated with the fixpoint holding,
    `cmp` printing nothing. `toolchain.hero` stood at exactly 300 lines of code
    before this and the first draft's comments put it at 304: the explanation
    lives once, in `units.tu_object`, and the other three sites cite it, which
    is the contract's own rule about where a rule is written.
