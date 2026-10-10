# Panel 206, historian

Copied by the coordinator at 09:22 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 206, historian

## verdict

**approve (advisory).** `check` should refuse an operator tree whose operands are only literals and constants when its value cannot fit its position. The refusal should be defined as a class of syntax in the spec, not as whatever a folding pass manages to see. It should stop at any call or variable operand, which keep the run-time abort (panel 203 R3). On precedent alone, the round accepting `y: u8 = 200 + 100` departs from every surveyed language that types literals by context or keeps constants exact. Precedent does not decide `rep`; it needs its own judgement under panel 054.

## precedents

All pages were fetched on 2026-10-10. Every quotation below is the fetch tool's rendering of the page, not my own copy of the page text, so each counts as summarised.

**1. Go: an error defined in the spec, with exact evaluation for untyped constants and per-step checking for typed ones.** Verified, [go.dev/ref/spec](https://go.dev/ref/spec).
- *"Constant expressions are always evaluated exactly; intermediate values … may require precision significantly larger than supported by any predeclared type."*
- *"The values of typed constants must always be accurately representable"*, with `Four * 100 // product 400 cannot be represented as an int8`.
- *"If the divisor is a constant, it must not be zero."*
- Where it stops: a non-constant operand wraps silently, *"Overflow does not cause a run-time panic."*
- The complaint it drew: issue [#20108](https://github.com/golang/go/issues/20108), opened 2017-04-25. `fmt.Println(math.MaxUint64)` is refused, and `math.MaxUint32` is refused on 386 only. It was closed the same day as working as intended: *"The spec is the spec is the spec."* Verified.

**2. Rust: three deny-by-default lints that can each be allowed.** `arithmetic_overflow`, `unconditional_panic` and `overflowing_literals` are all listed as deny-by-default ([lint listing](https://doc.rust-lang.org/rustc/lints/listing/deny-by-default.html)). Verified.
- The first two were split out of `const_err` in Rust 1.43.0, released 23 April 2020 ([releases.rs](https://releases.rs/docs/1.43.0/)). The change came through PR [#69185](https://github.com/rust-lang/rust/pull/69185) by Ralf Jung, merged 2020-02-20. Verified.
- RFC [560](https://rust-lang.github.io/rfcs/0560-integer-overflow.html) (start date 2014-06-30) recommended a lint *"defaulting to warn"*. Verified (summarised).
- The lint runs on const propagation in the MIR, a form of folding. Issue [#117949](https://github.com/rust-lang/rust/issues/117949) (2023-11-15) is a false negative: `format_args!("{}", 1 << 32)` was missed because of promotion. It was fixed by PR #119432 and closed 2024-02-17. Verified.
- The lint is independent of the `overflow-checks` setting ([users forum, 2022-12-16](https://users.rust-lang.org/t/rust-arithmetic-overflow/85999)). Verified.
- Whether it reaches `let x: u8 = 255; x + 1` is unverified.

**3. Swift: an error produced by mandatory constant folding in its SIL intermediate form.** Chris Lattner wrote on 2017-11-23 that `1+127 as Int8` is diagnosed *"through constant folding at the SIL level"* ([swift-evolution](https://lists.swift.org/pipermail/swift-evolution/Week-of-Mon-20171120/041604.html)). Verified (summarised).
- The false positive: SR-5964 / [swiftlang/swift#48523](https://github.com/apple/swift-issues/issues/5964), opened 2017-09-22. `Int.min.dividedReportingOverflow(by: -1)` fails to compile inside `do {}` and compiles at top level, even though the API is documented as not an error. A comment reads *"Constant folding only gets smarter."* The issue was still open when fetched. Verified.
- That Swift checks each step at the width (so `2 - 3 + 5` at `UInt8` is refused) is unverified.

**4. Zig: an error for anything known at compile time.** The 0.14.0 language reference shows *"error: overflow of integer type 'u8' with value '256'"* and a compile-time *"division by zero"* error, and says *"Integer literals have no size limitation"* (the `comptime_int` type) ([langref](https://ziglang.org/documentation/0.14.0/)). Verified. Whether Zig skips functions that are never called (lazy analysis) is unverified.

**5. C#: constant expressions are checked by default.** *"Constant expressions are evaluated by default in a checked context and overflow causes a compile-time error"*, and `unchecked` is the escape ([checked and unchecked](https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/checked-and-unchecked); CS0220 at [errors page](https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/compiler-messages/overloaded-operator-errors)). Verified.
- Where it stops: non-constant expressions run unchecked unless the `CheckForOverflowUnderflow` option is set. Verified.

**6. Ada: static expressions are evaluated exactly, so only the final value counts.** RM 2012 4.9(33/3): evaluation is *"performed exactly, without performing Overflow_Checks"*. 4.9(35/2): the value *"shall be within the base range of its expected type"* ([adaic RM](https://www.adaic.org/resources/add_content/standards/12rm/html/RM-4-9.html)). Verified. The run-time `Constraint_Error` for a value outside a subtype, and GNAT's warning about it, are unverified.

**7. C: a constraint in the standard, warnings in the compilers.** C11 6.6p4: *"Each constant expression shall evaluate to a constant that is in the range of representable values for its type"* ([N1570](https://port70.net/~nsz/c/c11/n1570.html)). Verified.
- GCC's documentation has `-Wno-overflow`, *"Do not warn about compile-time overflow in constant expressions"* ([GCC](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)). Verified. That the warning is on by default is my inference from the `-Wno-` form.
- clang's `-Winteger-overflow` and `-Wconstant-conversion` are both *"enabled by default"*, and both are warnings ([clang](https://clang.llvm.org/docs/DiagnosticsReference.html)). Verified.

**8. Java: int arithmetic wraps, and only the narrowing is refused.** JLS SE21 §5.2 allows narrowing a constant only when *"the value … is representable in the type of the variable"* ([JLS](https://docs.oracle.com/javase/specs/jls/se21/html/jls-5.html#jls-5.2)). Verified. That int constant arithmetic wraps silently (§15.18.2) and javac's exact message are unverified.

**9. Kotlin: a warning only.** The `INTEGER_OVERFLOW` warning name and the claim that it is warning-only come from a tutorial and Rosetta Code, not from the spec. Unverified.

**10. Nim.** The manual (2.2.12) says *"int literals are implicitly convertible to a smaller integer type if the literal's value fits"* and *"Unsigned operations all wrap around"* ([manual](https://nim-lang.org/docs/manual.html)). Verified. A compile-time error for signed constant overflow is unverified.

## argument

Precedent splits in two. Go, C#, Ada and C define the refusal over a class of syntax, the constant expression, so the refused set is stable and does not depend on reachability. Their one complaint on record, Go #20108, was closed as intended. Swift and Rust refuse whatever their folding pass sees, so the refused set moves with that pass: SR-5964 has been open since 2017, and Rust #117949 was a miss. Heroes has no warning class and no `allow` (critic's grep), so a refusal here is Go-strict and should be Go-defined.

On semantics, Go's untyped constants and Ada keep only the final value exact. Heroes types literals by their position, like Swift and Rust, and its C aborts at each step. Checking each step makes the refused set exactly the set of evaluations that abort (inference).

## condition

Three findings would change this reading:
- a language that refuses constant expressions by spec rule and later relaxed it for unreachable code after complaints;
- evidence that Zig's or Rust's laziness (not checking code that is never used) was adopted deliberately because refusing dead code drew complaints;
- a language that types literals by position and moved from per-step to final-value checking.

None was searched to exhaustion. The searches were for Rust, Go and Swift complaints only.

## repository files read

- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-206/docs/panel/206-briefs/00-shared.md
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-206/docs/panel/206-briefs/historian.md
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-206/docs/panel/206-reports/completeness-critic-pass1.md
- My notes: /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/206-historian/notes.md

I could not search panel 042, panel 054 or design.md, because I have no shell or grep, and no compiler was run. *Heroes of code* was not used.
