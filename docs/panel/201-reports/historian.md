# Panel 201, historian

Copied by the coordinator at 20:11 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule. The seat has no shell and wrote no file; every web source passed through the fetch tool's summarising model, which the seat says where it matters: re-read any load-bearing quotation at its source before it lands in a record.

---

## Panel 201: the historian's report

All sources were read on 2026-10-09 through WebSearch and WebFetch. Every fetch goes through a summarising model, so a quotation below is what that tool returned, not a byte-checked copy. One fetch made up a "Limitations" paragraph for rustc's lint page. A second read of the same page did not reproduce it, so I dropped it. Re-read any quotation at its source before it goes into a record.

I have no shell. I ran no compiler and no `date`, and I wrote no file. Where I say what a compiler does with a program, that comes from its documentation or its own test files, not from a run. I did not consult *Heroes of code*, and no claim below rests on it.

---

### Q1 (488): a generic function passed to a generic parameter

**verdict: approve (advisory)** the reading that infers the passed function's own type parameter together with the call's. `cannot_infer` should stay only for a type variable that nothing fixes.

**precedents**
- **ML and Haskell (Hindley–Milner).** Damas and Milner, "Principal type-schemes for functional programs", POPL '82: every use of a polymorphic name gets a fresh instance, which is then unified with its context. **Verified** (paper metadata). That `app id 20` type-checks follows from that rule. **Unrun.**
  - The limit is rank 1. OCaml's manual ("Polymorphism and its limitations") shows a polymorphic argument used at two types inside the callee failing ("This expression has type 'a list but … int"). The fix there is a polymorphic record field `{ f : 'elt. … }`. **Verified.**
  - GHC rejects `id runST`, which would need instantiation with a polytype, unless `ImpredicativeTypes` is on (Quick Look, GHC 9.2.1, Serrano et al., ICFP 2020). **Verified.**
  - 488's case needs neither of these: `A := i64` is an ordinary monotype.
  - Cost: Mairson (POPL 1990) proved ML typability DEXPTIME-complete, but his hardness proof uses nested polymorphic `let`. **Verified** (search summary of the paper). Heroes has no local polymorphic binding; that is my reading of spec § 9 ("no anonymous functions", generics on functions).
- **Swift.** `docs/TypeChecker.md` says that when the constraint generator meets a reference to a generic function, it "immediately replaces each of the generic parameters … with a fresh type variable" (called "opening"). It adds that this is "only valid because Swift does not support first-class polymorphic functions". **Verified.**
  - `test/Generics/deduction.swift` accepts `acceptFnFloatFloat(identity)`. **Verified.**
  - I found no test passing a generic function to a generic parameter. **Unverified.**
- **Rust.** The Reference says a function item's type carries its type arguments, and an item coerces to a function pointer. **Verified.** The docs for `std::convert::identity` (stable since 1.33.0) show three shapes, all **verified**:
  - `iter.filter_map(identity)`: generic passed to generic, `T` taken from the receiver.
  - `&[identity, manipulation]`: in an array literal.
  - `if condition { manipulation } else { identity }`: in a branch.

  What is left over is E0282 "type annotations needed". **Verified** from the error index, whose example is `Vec::new()`, not a function value.
- **Go.** Go is the closest precedent, because it started where Heroes' compiler is now.
  - Go 1.18 (2022-03-15) shipped generics. Go 1.21 (2023-08-08) says a generic function "may **now** be called with arguments that are themselves (possibly partially instantiated) generic functions". It also says one "may **now** be used without explicit instantiation when it is assigned to a variable or returned as a result value". **Verified.**
  - Issue #59338 (Griesemer, 2023-03-30, closed 2023-07-25) added the assignment case. **Verified.**
  - Griesemer's blog post of 2023-10-09: "the same types are inferred irrespective of the order of the function parameters". **Verified.**
  - Issue #77245 (Griesemer, 2026-01-20): `S{f: g}` is still refused with "cannot use generic function g without instantiation", while `s.f = g` is accepted. It is labelled Proposal-Accepted with milestone Go1.27 and is closed. **Verified.** Whether the change has shipped is **unverified**.
- **TypeScript.** 3.4 (announced 2019-03-29) added higher-order inference (PR #30215, Hejlsberg, merged 2019-03-08). The PR calls it "not a complete unification algorithm" and says it "only delivers the desired outcome when types flow from left to right". **Verified.** What `tsc` says for `app(ident, 20)`: **unrun.**
- **Nim.** Searched; nothing verified.
- **Errors models make here.**
  - Mündler et al., arXiv 2504.09246 (2025-04-12, PLDI 2025): LLMs "frequently generate code with typing errors". **Verified.**
  - "94% of compilation errors result from failing type checks": **unverified**, seen only in a search snippet.
  - I found no study of this particular shape.

**argument**
The compiler's narrowing is the rule Go shipped in 1.18 and dropped in 1.21. Go widened one shape at a time, and a struct-literal field was still refused in 2026, which is the "shapes beside it" lesson. Go programmers had explicit instantiation as a way around the narrowing; Heroes has none, so the narrowing costs Heroes more. Haskell and OCaml (since Damas–Milner, 1982), Swift ("opening") and Rust all solve the passed value's parameters in one system with the call's. What every language refuses is a variable left unsolved, or a polytype; keep `cannot_infer` for those. The critic's restricted route is fine if it means "solve jointly, refuse what is left over". If it means an ordered pass, it brings in TypeScript's documented left-to-right incompleteness, which Go avoided on purpose.

**condition**
My reading would change if a language had shipped inference of generic function arguments and then withdrawn it, over cost or error quality.

---

### Q2 (467): where `main` lives

**verdict: approve (advisory)** keeping the sentence as it is. Precedent treats an extra `main` in a library module as harmless.

**precedents**
- **Go.**
  - The spec (go1.27, 2026-05-26) says a program "contains exactly one `main` package" and execution invokes "the function `main` in the `main` package". **Verified.**
  - Go's own `test/linkmain.go` (2015) is `package notmain` holding `func main() {}`. `linkmain_run.go` compiles it with `go tool compile` and expects only the **link** to fail. **Verified.** So Go accepts the extra `main` as an ordinary function.
  - Go refuses the converse case, importing a program. Issue #4210 (rogpeppe, 2012-10-08) led to Ian Lance Taylor's commit "cmd/go: do not permit importing a main package" (2015-06-11). The reason given was the toolchain confusing a built binary with a library archive. **Verified.**
- **Rust.**
  - The Reference: "A crate that contains a `main` function can be compiled to an executable." **Verified.**
  - Cargo's defaults are `src/main.rs` and `src/lib.rs`. **Verified.** E0601 is "No `main` function was found in a binary crate". **Verified.**
  - rustc's own test `lint-dead-code-2.rs` reports a non-entry `fn main` as "function `main` is never used". **Verified.** That is the generic dead-code lint, not a rule about `main`.
- **Zig.** The langref section added by mlugg on 2025-02-01 says the entry point "is looked up" in the root source file. **Verified** (via a mirror of the commit).
- **The extra `main` as an idiom or a feature.**
  - Nim: `when isMainModule:`. **Verified.**
  - Python: `if __name__ == '__main__'`, "for example to unit test it". **Verified.**
  - Java (JLS 12.1): the JVM invokes "the method `main` of some specified class". **Verified.**
  - Erlang: `erl -s Mod [Func]`, where Func "defaults to `start`". **Verified.**
  - Oberon: "A command is defined by any procedure which is exported and has an empty argument list". Verified only in Wikipedia, a secondary source; I did not read *Project Oberon* itself.
- **Is it documented as a known mistake?** I found no such documentation. I searched Go, Rust, Zig and Nim.

**argument**
Every lineage in my mandate treats an entry function inside a library module as harmless or deliberate. Java, Python, Nim, Erlang and Oberon make it a feature. Go compiles it and simply never calls it. The one refusal with a documented history is the converse: Go refusing to import a program (issue 2012, fix 2015), for toolchain reasons Heroes does not share. I found no documentation calling the addition a mistake. Measurement 040's two additions answered a task that said *compile*, so they are confounded. On precedent, no rewording is owed; whether one is worth its tokens is the spec-warden's question.

**condition**
My reading would change if I found a toolchain that documents an extra `main` in a library as a known error class. It would also change if 040 were re-run with a task that does not say *compile* and the additions still appeared.

---

### Q3 (520): mutual recursion and recursion through a function value

**verdict: approve (advisory)** following calls through a cycle of named functions. Approve the function-value case **only** where it can be decided by syntax. That second part is a departure with no precedent, and it should be taken deliberately.

**precedents** (adding to panel 199's historian, which already covered rustc, clang, GCC, Swift, MSVC, staticcheck and Error Prone)
- **rustc.**
  - Issue #57965 (jonas-schievink, 2019-01-29) is still **open**. Its proposed design, from eddyb: "Build a callgraph where you're only considering calls that are unconditional … Then find cycles in it." On 2020-07-29: "Given how many people run into this, this really needs to be fixed." **Verified.**
  - PR #75067 implemented it (opened 2020-08-02). It was closed unmerged on 2020-10-22. Reviewer bjorn3 called the measured cost "still a significant regression (up to 8%)", and it was closed citing "inactivity" and those benchmarks. **Verified.**
  - rustc's lint test does not flag `if x { loops(x) } else { loop {} }`. **Verified.**
- **clang.**
  - Its own test expects no warning for `void e() { f(); } void f() { e(); }`. **Verified.**
  - D58122 (CodaFi, 2019-02-12) fixed a false positive when a function's exit block is unreachable. **Verified.**
- **GCC.**
  - The manual says the warning "requires optimization in order to detect infinite recursion in calls between two or more functions". In other words, it sees mutual recursion only after inlining. **Verified.**
  - GCC 13 added `-Wanalyzer-infinite-recursion`. It compares state between stack frames, and any change in state "suppress[es] the warning". **Verified.** That it handles mutual recursion explicitly is **unverified**.
- **Mercury**, a language that emits C, on 2022-07-01:
  - GCC 12's warning fired on six of its tests. In one, `higher_order5.m`, GCC saw the recursion only after `id/1` was inlined.
  - Mercury added `-Wno-infinite-recursion` for GCC 12 and later. **Verified.** This is the same move as panel 199's R3.
  - The test's exact shape is **unverified**; I did not read the file.
- **Swift.** In a forum thread of 2020-03-02: "there's no support for identifying mutual infinite recursion". Robert Widmann (codafi) replied that "flow-insensitive, interprocedural analysis … will yield a wealth of false positives". **Verified.**
- **Totality checkers.**
  - Agda's termination checker handles mutual recursion through call graphs. **Verified.**
  - The size-change principle: Lee, Jones and Ben-Amram, POPL 2001. **Verified.**
  - These checkers make the opposite promise to Heroes' rule: they refuse whatever they cannot prove terminates.
- **Cost of finding cycles.** Tarjan (SIAM J. Computing 1(2), 1972) finds strongly connected components in linear time. **Verified.**
- **Through a function value.** I found no checker that follows one. GCC sees one only after inlining. That rustc's original PR left out closures and function pointers is **unverified**.

**argument**
Every shipping lint stops at self-calls. The reasons on record are whole-program cost (rustc, up to 8%) and the false positives of a flow-insensitive analysis (Swift). Neither applies here. A cycle cannot cross modules (`module_cycle`), and the rule can be flow-sensitive: refuse only a set of functions in which every path of every member reaches a call into the set, computed as a greatest fixpoint and reusing R4's "may end". An `isEven`/`isOdd` pair with a base case in only one of the two still passes. The message should name every member and the call that closes the cycle. For function values: decide by syntax, never by value.

**condition**
Two findings would turn me against widening the rule:
- a shipped cross-function check that was withdrawn over **false positives** (cost alone would not be enough);
- a correct program in which every member of a cycle reaches the cycle on every path and the program still ends normally.

---

### Sources
- https://bernsteinbear.com/assets/img/damas-milner-original.pdf
- https://ocaml.org/manual/5.2/polymorphism.html
- https://downloads.haskell.org/ghc/latest/docs/users_guide/exts/impredicative_types.html
- https://static.aminer.org/pdf/20170130/pdfs/popl/o8rbwxmj6h2cxf7qdezlupsbgn5u40wn.pdf
- https://github.com/swiftlang/swift/blob/main/docs/TypeChecker.md
- https://raw.githubusercontent.com/swiftlang/swift/main/test/Generics/deduction.swift
- https://doc.rust-lang.org/reference/types/function-item.html
- https://doc.rust-lang.org/std/convert/fn.identity.html
- https://doc.rust-lang.org/error_codes/E0282.html
- https://go.dev/doc/go1.21
- https://go.dev/doc/devel/release
- https://github.com/golang/go/issues/59338
- https://github.com/golang/go/issues/77245
- https://go.dev/blog/type-inference
- https://www.typescriptlang.org/docs/handbook/release-notes/typescript-3-4.html
- https://github.com/microsoft/TypeScript/pull/30215
- https://arxiv.org/abs/2504.09246
- https://go.dev/ref/spec
- https://go.googlesource.com/go/+/41bc0a1713b9436e96c2d64211ad94e42cafd591/test/linkmain.go
- https://go.googlesource.com/go/+/refs/heads/master/test/linkmain_run.go
- https://github.com/golang/go/issues/4210
- https://go.googlesource.com/go/+/679fd5b4479e0b9936344a33e07a0d1f904c362b%5E%21
- https://forum.golangbridge.org/t/go-run-non-main-package-why-not/38741
- https://doc.rust-lang.org/reference/crates-and-source-files.html
- https://doc.rust-lang.org/cargo/reference/cargo-targets.html
- https://doc.rust-lang.org/error_codes/E0601.html
- https://fuchsia.googlesource.com/third_party/rust/+/HEAD/tests/ui/lint/dead-code/lint-dead-code-2.rs
- https://git.medv.io/zig/commit/cc64295a6313f8697ed02143390caa8fe3a63626.html
- https://nim-lang.org/docs/system.html
- https://docs.python.org/3/library/__main__.html
- https://docs.oracle.com/javase/specs/jls/se21/html/jls-12.html
- https://www.erlang.org/doc/apps/erts/erl_cmd.html
- https://en.wikipedia.org/wiki/Oberon_(operating_system)
- https://github.com/rust-lang/rust/issues/57965
- https://github.com/rust-lang/rust/pull/75067
- https://github.com/rust-lang/rust/pull/20373
- https://raw.githubusercontent.com/rust-lang/rust/master/tests/ui/lint/lint-unconditional-recursion.rs
- https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html
- https://llvm.googlesource.com/llvm-project/+/refs/heads/main/clang/test/SemaCXX/warn-infinite-recursion.cpp
- https://reviews.llvm.org/D58122
- https://clang.llvm.org/docs/DiagnosticsReference.html
- https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
- https://gcc.gnu.org/onlinedocs/gcc/Static-Analyzer-Options.html
- https://gcc.gnu.org/gcc-13/changes.html
- https://lists.mercurylang.org/archives/reviews/2022-July/023110.html
- https://forums.swift.org/t/avoiding-unintentional-infinite-recursion/34225
- https://agda.readthedocs.io/en/latest/language/termination-checking.html
- https://khoury.northeastern.edu/~pete/courses/Formal-methods/2008-Fall/readings/lee01sizechange.pdf
- https://sites.cs.ucsb.edu/~gilbert/cs240a/notes/TarjanDFS.pdf

Repository files read: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-201/docs/panel/201-briefs/{historian,00-shared}.md`, `…/spec/heroes-spec.md` lines 15–53 and 268–283, and `…/docs/panel/199-a-function-that-can-only-call-itself-is-refused-and-recursion-too-deep-aborts-at-every-level.md` and `…/docs/panel/199-reports/historian.md`.
