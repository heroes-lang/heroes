# Panel 207, historian

Copied by the coordinator at 12:10 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 207, historian

I resumed at 12:03 on 2026-10-10. My first run left no notes. I read only and wrote nothing outside my notes folder. I found 20 precedents, each checked by a web search or fetch this session. Every quotation below comes through the fetch tool's summary, not my own reading of the page, and is marked *(fetch)*.

## verdict

**object (advisory).** The objection is to the brief's lead route: a step bound written into the language, refusing a program when the bound is reached. Every system that shipped one has drawn complaints, and four are now softening it. I **approve** the structural routes: refuse `while` and keep `for` (route A), extend the walk on the straight path (route D), or reword C4b (spec § 4's sentence).

## precedents

### A step bound (the brief's route)

1. **Rust: verified.** This checks design.md's claim *"Rust shipped one and removed it in 1.72"*.
   - **Added:** PR #67260, *"const limit for CTFE"*, merged 2020-03-05, default 1,000,000 *(fetch)*. https://github.com/rust-lang/rust/pull/67260
   - **A loop detector before it, removed:** PR #70087, *"Remove const eval loop detector"*, merged 2020-03-24 *(fetch)*. https://github.com/rust-lang/rust/pull/70087
   - **Exact repeat detection was weighed and set aside:** RFC 2344 (2018-02-18) left the state-hashing detector as an open question: it *"will slow down const evaluation enormously and for complex iterations is essentially useless"* *(fetch)*. https://rust-lang.github.io/rfcs/2344-const-looping.html
   - **Loops enter `const fn`:** Rust 1.46, 2020-08-27. https://blog.rust-lang.org/2020/08/27/Rust-1.46.0/
   - **The hard limit goes:** PR #103877, merged 2023-06-01, closing tracking issue #67217. https://github.com/rust-lang/rust/pull/103877
   - **Why, in the 1.72 announcement (2023-08-24):** *"whether code hit the limit could vary wildly based on libraries invoked by the user"* *(fetch)*. https://blog.rust-lang.org/2023/08/24/Rust-1.72.0/
   - **Nuance design.md omits:** the limit became a **deny-by-default lint**, `long_running_const_eval`, that a program can switch off. https://doc.rust-lang.org/stable/nightly-rustc/rustc_lint/builtin/static.LONG_RUNNING_CONST_EVAL.html
   - The blog post spells the lint `const_eval_long_running`. The rustc docs use `long_running_const_eval`.

2. **Zig: verified.** This checks design.md's *"bypassed by generic types into a compiler segfault"*.
   - **The quota:** default 1000, error *"evaluation exceeded 1000 backwards branches"* *(fetch)*. https://ziglang.org/documentation/master/
   - **The bypass:** issue #21324, opened 2024-09-06 on Zig 0.13.0, still open. https://github.com/ziglang/zig/issues/21324
     - A maintainer (mlugg) wrote *"I have absolutely no idea how to solve this"* and *"Non-determinism in whether a given piece of Zig code compiles is not acceptable"* *(fetch)*.
   - **Legality depends on analysis order:** issue #22410, opened 2025-01-05, still open. https://github.com/ziglang/zig/issues/22410
   - **Where to put the quota:** issue #1767, opened 2018-11-21, closed 2019-08-19. https://github.com/ziglang/zig/issues/1767
   - **Exempting `for` loops:** issue #16983, *"exempt known-finite operations (`for` loops) from eval branch quota"*, opened 2023-08-27, labelled accepted on 2026-04-28 *(fetch)*. https://github.com/ziglang/zig/issues/16983

3. **C++: verified.**
   - **C++11 had no loops in constexpr:** a C++11 constexpr body held *"exactly one return statement"*. N3652 (Richard Smith, 2013-04-18) added the loops *(fetch)*. https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3652.html
   - **The standard's limits are informative only:** Annex B gives 512 recursive calls and 1,048,576 full-expressions. https://eel.is/c++draft/implimits
   - **gcc defaults:** loops `1<<18`, operations `1<<25`. https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html
   - **clang default:** 1,048,576 steps.
   - **A raise was refused:** clang PR #143785 proposed 20,000,000. It showed libc++ tests overriding the limit at up to 200,000,000. It was closed unmerged on 2025-09-24 *(fetch)*. https://github.com/llvm/llvm-project/pull/143785
   - **An opt-out was added instead:** PR #160440 (2025-10-13) lets `-fconstexpr-steps=0` turn the limit off. Its message says users pick *"some arbitrary high number"* *(fetch)*. https://llvm.googlesource.com/llvm-project/+/ec2d6add367acbc03dba038b7d4e519b11bbadec

4. **D: verified, no bound at all.** The spec says *"If the function goes into an infinite loop, it may cause the compiler to hang"* *(fetch)*. https://dlang.org/spec/function.html
   - Bug 21044 (2020) asked for loop detection and moved to dmd#19747. Its status comes from a search summary only, because the bugzilla page returned HTTP 522: **unverified**.

5. **Nim: verified.** `maxLoopIterationsVM: 10_000_000` and `maxCallDepthVM: 2_000`, read in `compiler/options.nim` *(fetch)*. https://raw.githubusercontent.com/nim-lang/Nim/devel/compiler/options.nim

### Termination guaranteed by structure

6. **Starlark: verified.** Its spec says *"Execution is finite. The language does not allow recursion or unbounded loops"* *(fetch)*. https://raw.githubusercontent.com/bazelbuild/starlark/master/spec.md
   - Changing a collection while a loop walks it is a run-time error.
   - The Go implementation offers `While` and `Recursion` as opt-in flags per file. https://pkg.go.dev/go.starlark.net/syntax

7. **Dhall: verified.** Dhall is total, but *"some short pathological programs ... take longer than the heat death of the universe to evaluate"* *(fetch)*. https://docs.dhall-lang.org/discussions/Safety-guarantees.html
   - So a termination guarantee does not bound the cost. This is the critic's byte bound, seen from outside.

8. **Ada: verified. This is the closest precedent to C4b.** https://ada-lang.io/docs/arm/AA-4/AA-4.9
   - RM 4.9(33/3): static evaluation *"is performed exactly"*.
   - RM 4.9(34/3): an expression whose evaluation fails a language-defined check is **illegal**.
   - Termination is structural. A static expression has no loops, and a static expression function *"contains no calls to itself"* (RM 6.8(5.4/5)). https://ada-lang.io/docs/arm/AA-6/AA-6.8
   - **But** RM 4.9(32.1-32.5/3) exempts *statically unevaluated* code (for example a branch whose condition is a static False). Ratified R1 refuses steps in dead code, so it departs from Ada here. Whether that departure was deliberate is unverified.

9. **Go: verified.** *"Constant expressions are always evaluated exactly"*, integers kept to at least 256 bits, and no loops or user calls *(fetch)*. https://go.dev/ref/spec

10. **Agda and Idris 2: verified.** Both accept structural recursion. Agda: https://agda.readthedocs.io/en/latest/language/termination-checking.html. Idris 2: https://idris2.readthedocs.io/en/latest/tutorial/theorems.html
    - Idris 2's page warns that its totality checker should not yet be relied on.

### Running generated code at build time (route C)

11. **autoconf: verified.** When cross-compiling, `AC_RUN_IFELSE` *"is not run"*. Without a cross-compiling fallback, configure *"prints an error message and exits"* *(fetch)*. https://www.gnu.org/software/autoconf/manual/autoconf-2.72/html_node/Runtime.html
    - `AC_COMPUTE_INT` requires the expression to be computable *at compile time* *"to support cross compilation"* *(fetch)*. https://www.gnu.org/software/autoconf/manual/autoconf-2.72/html_node/Generic-Compiler-Characteristics.html
    - How it does that internally is **unverified**.

12. **CMake: verified.** When cross-compiling, `try_run` runs nothing unless `CMAKE_CROSSCOMPILING_EMULATOR` is set (added in 3.3). Otherwise the user fills the results in by hand. https://cmake.org/cmake/help/latest/command/try_run.html

13. **Go's `go generate`: verified.** Introduced in Go 1.4 (Rob Pike, 2014-12-22). It *"is not part of go build"*, and its output *"must be checked into the source code repository"* *(fetch)*. https://go.dev/blog/generate

### Unverified

- The Henglein and Kfoury–Tiuryn–Urzyczyn (both 1993) citations in `mono_recursion.rs`.
- The C++11 recursion-depth limit.
- Which Ada edition each paragraph suffix marks.

## argument

Every system that shipped a step bound has been pulled toward softening it:
- Rust turned it into a lint a program can switch off, because legality depended on how a library split its statements.
- clang added an opt-out after refusing to raise the default.
- Zig accepted exempting `for` loops, and still has an open segfault and an open analysis-order flaw.

Ada, Go, Starlark and C++11 are the decades-old record of exact constant evaluation with termination guaranteed by structure. Ada even makes a failed check illegal, which is C4b. Zig's accepted #16983 is converging on route A. Route C inherits autoconf's and CMake's cross-compilation failure; whether Heroes cross-compiles is unrun. Two lessons stand. Dhall's caveat says route A still owes a bound on cost. Ada exempts dead code, which R1 does not.

## condition

These would change my reading:
- A language that wrote a fixed step count into its **specification**, not as an implementation-defined limit, and kept it stable across versions without complaint.
- Evidence that Starlark or Ada needed a step quota anyway once their loops were bounded by structure. Starlark's spec only says implementations *"may"* bound computation *(fetch)*.
- A shown Heroes build that never cross-compiles, which would remove route C's precedent problem.

## files read

- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-207/docs/panel/207-briefs/00-shared.md
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-207/docs/panel/207-briefs/historian.md
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-207/docs/panel/207-reports/completeness-critic-pass1.md
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-207/docs/design.md:3150-3172 (the frozen tree's copy; the trunk's line numbers differ)
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-207/archive/bootstrap-rs/heroes/src/ir/mono.rs:1-45
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-207/archive/bootstrap-rs/heroes/src/ir/mono_recursion.rs:1-70
- My notes: /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/207-historian/notes.md
