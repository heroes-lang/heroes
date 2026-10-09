# Panel 202, historian

Copied by the coordinator at 23:21 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

**verdict**: object (advisory). I object to route (b), and to any outcome that leaves `clash` at exit 0. The precedent supports (c) for all four verbs (plus `run`), a type comparison per shared C symbol made where a linker stands, and (d)'s true messages for whatever is still refused.

**One correction to the critic's repair 12.** C11 6.9p5 is not the `clash` rule. In `clash` a third module defines `twice` once, so 6.9p5 ("exactly one external definition") holds. Only 6.2.7p2 is broken. 6.9p5 is the rule for `onedef` and `extdef`.

## precedents

**The C standard** (N1570, [port70.net](https://port70.net/~nsz/c/c11/n1570.html), read today; each paragraph re-fetched verbatim after the fetch tool invented one quote)
- **6.2.7p2**: "All declarations that refer to the same object or function shall have compatible type; otherwise, the behavior is undefined." This is `clash` when each module is its own unit. Verified.
- **6.7p4**, a Constraint: "All declarations in the same scope that refer to the same object or function shall specify compatible types." This is `fp` and `clash` inside one unit, so this is why `test` refuses them. Verified.
- **6.9p5**: "…exactly one external definition for the identifier; otherwise, there shall be no more than one." This is `onedef` and `extdef`. Verified.
- **6.2.2p2**: an internal-linkage name is one function only within its own unit. So `fp` built module by module is two legal functions. Verified as the tool's summary only; it refused to quote.
- **What this shows**: C moves the same program between "diagnostic required" and "silent undefined behaviour" depending only on where the units are cut. Each Heroes verb currently cuts them differently.
- **C++** has the same shape: a broken one-definition rule is "ill-formed, no diagnostic required" ([cppreference](https://en.cppreference.com/w/cpp/language/definition)). Verified.

**cfront 2.0, Bell Labs** (Stroustrup, ["Type-safe Linkage for C++"](https://usenix.org/legacy/publications/compsystems/1988/fall.html), *Computing Systems* 1(4), Fall 1988; PDF pages 371–385 read). Verified.
- p.375 describes `fp`'s shape in 1988: file1 includes `glob.h`, file2 includes `widget.h`, and "calls (silently) go to the wrong version… if not, we will simply get wrong results."
- p.376 names `macro`'s order dependence as a defect: the user "must reorder the `#include` directives… irrelevant to the job the programmer is trying to do."
- p.380: the encoding is used to "'trick' the linker into doing type checking of the separately compiled files."
- p.383 explains why `extern "C"` was left unchecked: "the sloppy use of type information in many C programs would make that too painful."
- **What happened**: C++ functions got checked linkage; C names never did. Heroes' `extern` groups sit in that hole.

**GCC**
- `-Wlto-type-mismatch`: "During the link-time optimization… warn about type mismatches in global declarations from different compilation units. Requires -flto… Enabled by default" ([GCC docs](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)). Verified.
- It produced false positives: bug [lto/83954](https://gcc.gnu.org/pipermail/gcc-bugs/2018-March/616647.html), a regression in GCC 6 and 7, fixed 2018-03-06. Verified.
- It finds real bugs: Gentoo bug [940480](https://bugs.gentoo.org/940480), 2024-09-29, under tracker [618550](https://bugs.gentoo.org/618550). Verified. The tracker's size ("hundreds") is unverified.
- `-Wodr` arrived in [GCC 5](https://gcc.gnu.org/gcc-5/changes.html). Verified. The release that introduced `-Wlto-type-mismatch` is unverified.

**Clang**
- [llvm#56487](https://github.com/llvm/llvm-project/issues/56487), "Implement -Wlto-type-mismatch", opened 2022-07-12, is still open today. A maintainer notes that LTO matches types by name, with "no structural examination/comparison." Verified.
- So the critic's "clang's equivalent is unrun" now becomes "no equivalent exists, per the tracker". `clang -flto` on `clash` is still **unrun**.

**wasm-ld** ([lld docs](https://lld.llvm.org/WebAssembly.html)). Verified.
- It checks function signatures at link time.
- By default it gives a warning and replaces the call with a stub that traps at run time. With `--fatal-warnings` it is an error.
- So a mismatch never quietly becomes a wrong value.

**MSVC C4744** ([Microsoft Learn](https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4744)). Verified.
- "'var' has different type in 'file1' and 'file2'", only under `/GL`, and only for variables.

**Rust `clashing_extern_declarations`**
- Warn by default, and deliberately not run between crates: two crates may declare one function "in a different (but valid) way… the compiler can't say that the clashing declaration is incorrect" ([rustc lints](https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html)). Verified.
- Added by [#70946](https://github.com/rust-lang/rust/pull/70946), merged 2020-06-21. Verified.
- False positives on nightly four days later: [#73735](https://github.com/rust-lang/rust/issues/73735), 2020-06-25. Verified.
- Four years on, [#130301](https://github.com/rust-lang/rust/pull/130301) (2024-09-13) found it "missed a *lot* of cases" and said: "We don't currently have a clear spec for what we *want* to consider 'clashing'." Verified.
- This is the precedent for route (e). The release that shipped the lint is unverified.

**Go cgo** ([cmd/cgo](https://pkg.go.dev/cmd/cgo)). Verified.
- A package's C is compiled "as part of the Go package"; "static functions are permitted"; "a C type used in one Go package is different from the same C type used in another."
- Two packages binding `pow` failed to link on Windows: [#8756](https://github.com/golang/go/issues/8756), 2014, fixed in 2016. Verified.
- Generated names collided when one package was vendored twice; fixed by hashing the import path into them: [#23555](https://github.com/golang/go/issues/23555), Go 1.11. Verified.
- I found no check across packages that one C name has one type. That is a question, not a fact.
- `go test` is deliberately stricter than `build`: it runs "a high-confidence subset" of vet and does not run the test binary if vet finds problems ([cmd/go](https://pkg.go.dev/cmd/go)). Verified.

**Zig**
- The docs say there should usually be one `@cImport`, but list as a reason for several: "To avoid a symbol collision, for example if foo.h and bar.h both `#define CONNECTION_COUNT`" ([0.13.0 docs](https://ziglang.org/documentation/0.13.0/)). Verified. That is the `macro` and `nobind` answer.
- Types from separate imports are not equivalent ([zig.guide](https://zig.guide/working-with-c/c-import); [Ziggit, 2025](https://ziggit.dev/t/error-expected-type-t-found-t-with-types-imported-from-c-headers/12200)). Verified.
- One extern function declared in two files made the compiler segfault: [#529](https://github.com/ziglang/zig/issues/529), 2017–2019. Verified.
- Commit [e9fc58ea](https://github.com/ziglang/zig/commit/e9fc58eab77d60dfb02155ff17178b496d75d035) (2022-06-06): "Zig allows multiple extern functions with the same name." Verified.
- Whether Zig refuses differing signatures for one symbol is **unrun**.

**Nim**
- The `header` pragma means the symbol is not declared and the generated code gets an `#include` ([manual](https://nim-lang.org/docs/manual.html)). Verified.
- [#18776](https://github.com/nim-lang/Nim/issues/18776), open since 2021-09-01: two wrappers of the header-only stb_image give "multiple definition" at link (`onedef`'s shape). The maintainers declined to mangle names; the workaround is to make the definitions `static`. Verified.

**Swift**
- [#84948](https://github.com/swiftlang/swift/pull/84948), merged 2025-10-17, made modules giving one `@_extern(c)` symbol "conflicting Swift-level types" stop failing at deserialization. It is now accepted. Verified.

**Fused units**
- Chromium's [jumbo.md](https://chromium.googlesource.com/chromium/src/+/63.0.3239.12/docs/jumbo.md): "symbols that have internal linkage in different `cc` files can collide," and some single-file recompiles cost 10–20 s more. Verified. That it was removed in 2019 is unverified (secondary sources only).
- [SQLite's amalgamation](https://www.sqlite.org/amalgamation.html) runs 5–10% faster. Verified. That it works because one author writes every file is my inference.

**Oberon**
- Crelier, ETH Diss. 10650 (1994): fingerprints check the consistency of separately compiled modules. Unverified: I saw a catalogue listing only, and the page returned 500.

## argument

C draws this sitting's line itself. In one unit, `fp` and `clash` break a constraint and a diagnostic is required (6.7p4). In separate units `fp` is two legal functions and `clash` is silent undefined behaviour (6.2.7p2). The precedents keep C in separate units (Nim, cgo, and Zig, whose docs endorse a second `@cImport` for colliding headers). They catch `clash` where the linker stands, by checking the type each unit gave the symbol (cfront 1988, GCC LTO, wasm-ld, MSVC `/GL`), never by fusing units. Fusing every unit (route b) is the unity build, whose collisions Chromium documented. Comparing Heroes types (route e) is Rust's lint: false positives within a week, and no agreed rule four years later.

## condition

Any of these would change my reading:
- A census showing that fusing every tracked program's units refuses nothing beyond the critic's planted cases. (b)'s cost would then be theoretical.
- llvm#56487 shipping, or `clang -flto` already warning on `clash` (unrun). Route (h) would then cost a flag, not a new check.
- Zig or Swift refusing differing extern signatures at compile time with a clean tracker. That would give `check` (route a) a precedent.
- A language that fused third-party headers into one unit for years and kept doing it. That would weaken my objection to (b).
- Panel 200 R1 for `--emit-c` is not bound by precedent: Nim writes one C file per module.

I could only read and search in this session, so nothing was compiled. I wrote no file; this reply is the report. *Heroes of code* was not used as evidence: nothing above rests on it. Paths read: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-202/docs/panel/202-briefs/{historian,00-shared}.md`, `.../202-reports/completeness-critic-pass1.md`, and `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/{200-critic2/fp,critic-202-203/p202/clash}/a.h`.

Sources: [N1570](https://port70.net/~nsz/c/c11/n1570.html) · [cppreference ODR](https://en.cppreference.com/w/cpp/language/definition) · [Stroustrup 1988](https://usenix.org/legacy/publications/compsystems/1988/fall.html) · [GCC warnings](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html) · [GCC 5](https://gcc.gnu.org/gcc-5/changes.html) · [GCC 83954](https://gcc.gnu.org/pipermail/gcc-bugs/2018-March/616647.html) · [Gentoo 940480](https://bugs.gentoo.org/940480) · [Gentoo 618550](https://bugs.gentoo.org/618550) · [llvm#56487](https://github.com/llvm/llvm-project/issues/56487) · [MaskRay 2022](https://maskray.me/blog/2022-11-13-odr-violation-detection) · [lld wasm](https://lld.llvm.org/WebAssembly.html) · [llvm#91700](https://github.com/llvm/llvm-project/issues/91700) · [MSVC C4744](https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4744) · [rustc lints](https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html) · [rust#70946](https://github.com/rust-lang/rust/pull/70946) · [rust#73735](https://github.com/rust-lang/rust/issues/73735) · [rust#130301](https://github.com/rust-lang/rust/pull/130301) · [cmd/cgo](https://pkg.go.dev/cmd/cgo) · [cmd/go](https://pkg.go.dev/cmd/go) · [go#8756](https://github.com/golang/go/issues/8756) · [go#23555](https://github.com/golang/go/issues/23555) · [Zig 0.13.0](https://ziglang.org/documentation/0.13.0/) · [zig.guide](https://zig.guide/working-with-c/c-import) · [Ziggit](https://ziggit.dev/t/error-expected-type-t-found-t-with-types-imported-from-c-headers/12200) · [zig#529](https://github.com/ziglang/zig/issues/529) · [zig e9fc58ea](https://github.com/ziglang/zig/commit/e9fc58eab77d60dfb02155ff17178b496d75d035) · [zig#20630](https://github.com/ziglang/zig/issues/20630) · [Nim manual](https://nim-lang.org/docs/manual.html) · [nim#18776](https://github.com/nim-lang/Nim/issues/18776) · [swift#84948](https://github.com/swiftlang/swift/pull/84948) · [Chromium jumbo.md](https://chromium.googlesource.com/chromium/src/+/63.0.3239.12/docs/jumbo.md) · [SQLite amalgamation](https://www.sqlite.org/amalgamation.html) · [Crelier 1994 listing](https://research-collection.ethz.ch/handle/20.500.11850/141604?show=full)
