//! What the generated C is compiled **under**: the dialect, the diagnostics that
//! are errors, and what `--sanitize` adds.
//!
//! Split from `toolchain.rs` by the §11 ceiling, 2026-08-15, and deliberately **not**
//! along the seam the author's 2026-08-14 decision refused — that cut ran through the
//! cache key `link` and `runtime_object` share, and a file split against its own seam
//! is harder to read than a long one. This one runs somewhere else entirely:
//! `toolchain.rs` answers *where is the toolchain and what is cached where*, and this
//! answers *what does clang enforce about the C we hand it*. Nothing here knows a path.
//!
//! It also earns a file for a reason a line count would miss: **this list is quoted by
//! CLAUDE.md §7**, which is the operating contract, and it had already drifted four
//! flags out of step with it. A list two documents claim to state should be easy to
//! find and hard to miss.

/// CLAUDE.md §7's set, in one place. `-Werror=uninitialized` is the net under the
/// emitter's hoisted prologue: a slot read on a path that never wrote it is a
/// *lowering* bug, and this is what makes it a compile error instead of a wrong
/// answer.
///
/// **`-Wconditional-uninitialized` is `-Werror=` because the corpus said so.** Panel
/// 020 adopted it with a falsifiable disposition — zero fires across the whole
/// `run/`+`emit/` corpus at both levels promotes it; one fire is a lowering bug that
/// gets a case named after it (CLAUDE.md §9) — and the measurement came back zero on
/// the first run, with clang's warnings forwarded rather than swallowed so that they
/// could. It is the strictest warning in the set and the one most likely to catch a
/// join slot the lowering forgot to write on one arm.
pub const FLAGS: [&str; 11] = [
    // **`gnu11`, not `c11`, and the difference is one predefined macro** (panel
    // 047, ratified 2026-08-14). `-std=c11` defines `__STRICT_ANSI__`, and on
    // glibc that is the *only* thing it does: it hides `M_PI`, `strdup`,
    // `fileno`, `popen`, `setenv`, `newlocale`, `clock_gettime` — most of what
    // §1.11 says a program binds — while Darwin's headers do not guard them at
    // all. The same `.hero` file compiled here and failed on Linux, which is the
    // failure §4.19 exists to prevent arriving through the flags instead of
    // through the types.
    //
    // What the emitter *writes* is unchanged and still C11. Measured across the
    // two: identical LLVM IR for the runtime and for a generated unit at both
    // levels, 16 glibc struct layouts unchanged, and 63 of 63 `run/` goldens
    // cross-compiling for `x86_64-linux-gnu` — against 62 of 63 under `c11`.
    //
    // The dialect is **named** rather than left to clang's default (`gnu17`
    // today, `gnu11` before Clang 11) because a project whose acceptance is a
    // byte-identical fixpoint cannot let it drift — the same argument that
    // pinned `rust-toolchain.toml` the same day.
    "-std=gnu11",
    // **The third platform hides `M_PI` too, and for a third reason.** Panel 047
    // moved to `gnu11` because glibc guards the `math.h` constants behind
    // `__USE_MISC`, which `-std=c11` switches off. MSVC guards the same constants
    // behind `_USE_MATH_DEFINES`, which nothing switches on by default — so a
    // binding that compiles on Darwin and Linux failed on Windows with the very
    // diagnostic panel 047 was convened about (measured, the third CI leg,
    // 2026-08-14). This is Microsoft's documented switch for exactly that, and it
    // is passed everywhere because it names a macro no other platform reads.
    "-D_USE_MATH_DEFINES",
    // **The second platform switch, and the same argument as the first.** MSVC
    // deprecates `getenv`, `fopen`, `strcpy` and most of what §1.11 says a program
    // binds, in favour of `_s` variants no other platform has — so a binding that
    // is correct everywhere else fails the corpus harness's zero-warning bar on
    // Windows alone. `runtime.c` already defines this for its own compilation; the
    // generated unit needs it for the author's bindings. Named here rather than
    // discovered, like `-std=gnu11` and `_USE_MATH_DEFINES` (panel 047's class).
    "-D_CRT_SECURE_NO_WARNINGS",
    "-Wall",
    "-Werror=return-type",
    "-Werror=uninitialized",
    "-Werror=format",
    "-Werror=conditional-uninitialized",
    "-fno-strict-aliasing",
    "-Werror=shorten-64-to-32",
    "-Werror=sign-conversion",
];



pub fn sanitizers(sanitize: bool) -> Vec<String> {
    if sanitize {
        vec!["-fsanitize=address,undefined".to_string(), "-g".to_string()]
    } else {
        Vec::new()
    }
}
