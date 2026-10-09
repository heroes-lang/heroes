# Panel 203, historian

Copied by the coordinator at 23:22 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 203: historian's report

I read the brief, `00-shared.md` in full, panel 201's historian report, the critic's first pass, panel 199's historian report, spec § 2 (lines 42-46) and § 9 (lines 262-281), and design.md lines 1793-1810. All sources were read on 2026-10-09.

I have no shell. I ran no compiler and no `date`, so where I say what a compiler would do with a program, that is an inference from documents and is marked **unrun**. I read three PDFs myself as page images: Pierce and Turner, Holzmann, and Bagnara et al. Every other quotation went through the fetch tool's summarising model, so re-read it at its source before it goes into a record. I did not consult *Heroes of code*, and no claim below rests on it. I wrote no file: the session's instructions say to return findings as text, and the brief's "if you write anything" made a file optional.

---

## Q1 (defect 541): giving context to an argument that needs it

### verdict
**Approve (advisory) the general route**, under two conditions the precedents set:
- **The result must not depend on argument order** (Pierce and Turner's warning, Go's stated property).
- **Each argument's type is worked out once and never re-checked** (javac's lesson, below).

On repair 17 (does a literal settle a type parameter as `i64`, or wait for the context?), the precedents split. I report the split and give no verdict on it.

### precedents (only what panel 201 lacks)

1. **Pierce and Turner, "Local Type Inference", POPL '98 (pp. 252ff.); TOPLAS 22(1), Jan 2000, pp. 1-44.** Verified: I read the POPL pages; the TOPLAS details come from a search summary.
   - **Panel 105's "Flat only" is their rule.** When a call to a generic function omits its type arguments, "we force the argument expressions to be synthesized" (§4, p. 259). Verified.
   - **They measured what the rule costs.** Generic instantiation occurs "in every third line of code" across "about 160,000 lines of ML" (pp. 252-253). The arguments their rule handles badly occur "only … about once per 100 lines of code" (p. 259). Verified.
   - **What made the rule affordable for them.** "Type operators like List must be made covariant … to allow inference of type arguments to nil and cons" (§7, p. 261). Verified. My inference from that: Heroes has no subtyping, so under the same rule `[]` has no type to work out. The rule costs Heroes a shape it never cost its authors.
   - **Their warning about order.** In Cardelli's greedy algorithm, instantiations "are highly sensitive to the precise order in which constraints are encountered", and the algorithm "lacks monotonicity" (§6, p. 260). Verified. This bears on the critic's `literal2` probe.
2. **Hosoya and Pierce, "How Good Is Local Type Inference?"**, MS-CIS-99-17, 1999-06-22. Extensions "mitigating known expressiveness problems turn out to be unsatisfactory on close examination". Verified only through a search summary; the repository page returned 403 and I did not read the report.
3. **Odersky, Zenger and Zenger, "Colored Local Type Inference", POPL 2001.** It refines Pierce and Turner "by allowing partial type information to be propagated". This is the general route as it appears in the literature. Verified (abstract).
4. **Java: the closest precedent for this whole question.**
   - **JEP 101** (created 2011-02-22, delivered in Java 8) added inference in argument position. Its example: `List.cons(42, List.nil())`, where `List.<Integer>nil()` had been needed before. Verified.
   - **Chained calls were also a listed goal, but did not work when tested.** A jOOQ post of 2013-11-25 got "incompatible types: Object cannot be converted to String" for `List.nil().head()`. Verified. That test ran on a build earlier than Java 8's release, and that the released version kept the limit is **unverified**.
   - My inference: Heroes' `x.f(y)` is `f(x, y)`, so Java's receiver-position gap becomes an ordinary argument position here. The critic's `ufcs` probe is what measures it.
   - **The cost.** JEP 215 (created 2014-07-24, delivered in JDK 9): an argument could be checked "up to N * 3 … + 1 (final check phase)" times, and nesting multiplies that, "leading to a combinatorial explosion". JDK-8078093 (filed 2015-03-06, "with a single overload"): Java 7 took 0.380 s and Java 8 took 34.5 s. JDK-8077247 (2015-02-24): 9 nested calls took 0.418 s on Java 7 and 39.573 s on Java 8. Verified.
   - The fix was to build "bottom-up structural types", once per argument. "Infer first, then re-check against the substituted parameter" is the same check-twice design JEP 215 replaced.
5. **Swift.** Its exponential cost comes from overloading, which Heroes does not have (spec § 9).
   - Pestov, 2025-10-30: "Without disjunction constraints, a constraint system can almost always be solved very quickly." Verified.
   - Gallagher, 2016-07-12: the problem appears only when two or more features combine in one expression: overloads, literals, untyped closures. Verified.
   - `docs/TypeChecker.md`: a literal gets a fresh type variable and falls back to `Int` only when nothing else fixes it. Verified.
6. **Go, on repair 17.**
   - Griesemer, 2023-10-09: "typed arguments take precedence over untyped arguments. An untyped constant is considered for inference only if the type parameter … doesn't have an inferred type yet." Verified.
   - **Issue #50285** asks for inference from the type the result is assigned to. Opened 2021-12-21, still open, on hold. Verified.
     - Ian Lance Taylor, 2021-12-21: "The rules for when the type is inferred would be unclear to the person reading the program. People already get confused about untyped constants."
     - Griesemer, 2023-05-16: doing it "will require significant re-engineering of the type checker".
   - So Go would type `first(255, b)` with `b uint8` as `uint8`, and refuse `var y uint8 = first(1, 2)` with `T = int`. **Unrun**, inferred from these documents.
7. **Rust, on repair 17.**
   - The Reference: an integer literal takes the type "uniquely determined from the surrounding program context", otherwise `i32`. Overflow panics in a debug build. Verified.
   - **RFC 212** (2014-09-03) brought back the `i32` fallback after it had been removed. Without it, "coding in the small", meaning tests and beginners' programs, was "quite annoying". Verified.
   - **Swift** likewise traps overflow at run time: "Overflow behavior is trapped and reported as an error" (TSPL). Verified.
   - In both, `first(200, 2) + 100` typed as `u8` would be a run-time abort rather than a compile error. **Unrun.**
8. **C# 12 collection expressions.** "The empty literal `[]` has no type", and `var v = [];` is illegal. In generic inference, the elements are inferred one by one: `AsListOfArray([[4, 5], []])` gives `List<int[]>`. Verified, through the fetch tool.
9. **TypeScript.**
   - PR #8944 (Hejlsberg, 2016-06-02) makes `[]` have type `never[]`. Verified. That only works because TypeScript has subtyping, which Heroes lacks.
   - The TS 4.7 playground: older versions inferred `unknown` "because both … would be evaluated at the same time". Verified.

### argument (Q1, 118 words)
"Flat only" is Pierce and Turner's 1998 rule. They could afford it because `nil` worked out to a bottom-typed list under covariance. Heroes has no subtyping, so the same rule leaves `[]`, `ok(...)` and a generic function value with no type to work out. Java tried the general route: Java 8 moved to argument-position target typing and kept it. Its first implementation re-checked nested arguments and went exponential even with one overload, and JDK 9 fixed that by building each argument's type once. Swift's blow-up needs overloading, which Heroes lacks. On literals, Rust and Swift wait for context, as Heroes § 2 already does outside generic calls. Go stops at sibling arguments, for readability. Waiting turns a refusal into a run-time overflow abort.

### condition (Q1)
My reading would change on any of these:
- **A language that shipped argument-position contextual inference and then withdrew it.** I found none: Java kept it and made it faster; Go and TypeScript both widened theirs.
- **The compiler-engineer measuring the general route as worse than linear on nested generic arguments**, such as `ok(ok(ok(1)))`, which is javac 8's shape. I would then ask for JEP 215's build-each-type-once design before approving.
- **A documented incident in Rust or Swift where a literal typed through a generic call's result context hid an overflow.** That would favour Go's line on repair 17.

---

## Q2 (defect 547): a self-call through a parameter

### verdict
- **Object (advisory)** to following a parameter back through its callers to find what it is bound to (whole-program control-flow analysis).
- **Approve** the run-time abort as the answer today, as panel 199's R2 already rules.
- **Not objected to:** the critic's summary route, decided at the call site, where a function's summary says "calls its parameter `f` on every path". Its mechanism has working ancestors, but I found none that uses it to refuse a program, so it is a departure. If taken, it should be taken deliberately and recorded as such.

### precedents (only what panels 199 and 201 lack)

1. **rustc**, `lints.rs` as of 1.38.0: the comment `// closures can't recur, so they don't matter.` A self-call is recognised by `call_fn_id == def_id`, so a call through a parameter is never treated as one. Verified (a mirror of the source).
2. **Holzmann, "The Power of 10", IEEE Computer, June 2006, pp. 95-97.** Verified (I read the page images).
   - Rule 1 bans direct and indirect recursion so that tools can work on "an acyclic function call graph".
   - Rule 9 bans function pointers: "if function pointers are used, it can become impossible for a tool to prove the absence of recursion".
3. **MISRA C:2012 Rule 17.2.** Bagnara, Bagnara and Hill, arXiv 1809.00821 (2018-09-04). Verified (page images).
   - p. 10: the decidable approximation finds "cycles in the call graph" and flags "all function calls via pointers"; if no pointer calls are used, "there will be no false positives".
   - p. 13: "Bug finders are usually tolerant about false negatives and intolerant about false positives."
   - **Polyspace**, since R2024a, "reports a violation if a function calls itself using a function pointer". Verified. Whether that covers a pointer passed as a parameter is **unverified**.
   - MISRA refuses all recursion, so what counts as a false positive there is not what counts in Heroes.
4. **GCC's analyzer** (`-fanalyzer`).
   - It follows calls made through a function pointer. Saini's GSoC report of 2021-07-05: it was "successfully able to see the calls happening via the function pointer". Verified.
   - `-Wanalyzer-infinite-recursion` (Malcolm, commit dated 2022-11-11) compares memory state between recursion levels. Verified.
   - **It needed false-positive repairs soon after it landed.**
     - PR108935, fixed by a commit dated 2023-03-01. Verified.
     - PR108524, whose test is reduced from qemu 7.2.0's JSON parser (mutual recursion, a correct program). Verified from the test file; its date is **unverified**, because bugzilla blocked the fetch.
   - Whether it fires through a function-pointer parameter is **unverified**. The two main test files contain no such case.
5. **Termination provers.** These make the opposite promise: they refuse what they cannot prove ends.
   - Sereni, talk of 2007-06-22: "the precision of the call graph crucially determines the precision of the resulting analysis". Verified.
   - Van Horn and Mairson, ICFP 2008: control-flow analysis at depth k is EXPTIME-complete for every k > 0. Verified.
   - **Lean 4.18.0** (2025-04-02), PR #6744: a recursive call inside `List.map`'s argument is handled by `wf_preprocess`, which keeps "two rules for each higher-order function of interest" and can be extended by the user. Verified. This is the critic's "summaries for the built-ins", in shipped form.
6. **Nim: a summary composed through a parameter at the call site.** This is the critic's route in shipped form, though for exceptions rather than recursion.
   - Manual, rule 2: an expression "passed to parameter marked as .effectsOf of proc p is assumed to be called indirectly and thus its raises list is added to p's raises list". Verified.
   - Nim 1.6 (2021-10-19) made explicit "what was previously done implicitly", and Nim 2.0 made it the default. Verified. Why they changed it is **unverified**.
   - CLAUDE.md § 6 refuses Nim's effect systems as language features. A summary kept inside the checker is not one; that is my reading.
7. **Dialyzer** (Erlang).
   - "Function X has no local return" is propagated to callers (Lindahl, 2007-08-30). Verified.
   - Aronis, 2011-05-13: "non-terminating recursive functions (e.g. server loops) were internally characterized as 'failing'", which produced unnecessary warnings. Verified, through the fetch tool.
   - Whether it follows a function passed as an argument is **unverified**.
8. **Zig #1006, "safe recursion"** (andrewrk, opened 2018-05-11, open, accepted). bnoordhuis asked: "How does `@recursiveCall` deal with function pointers? … (P-)hard." I found no documented answer. Verified, through the fetch tool.
9. **The run-time answer.** Rust's standard library prints "thread '{name}' has overflowed its stack" and aborts. Verified through a search summary only.

**Searched and found nothing:** a checker that refuses endless recursion passing through a function given as an argument, beyond MISRA's ban on all recursion and GCC's analyzer (unverified for this shape). Koka's `div` effect was searched but not verified.

### argument (Q2, 104 words)
Every compiler recursion check found by three historians identifies a self-call by the callee's definition; rustc compares definition ids and skips closures. Following a function through its callers is control-flow analysis: its precision is that of the call graph, and it is EXPTIME-complete at any depth k > 0. Safety-critical C answered by banning function pointers (Holzmann) or by flagging every pointer call and accepting false positives (MISRA 17.2). GCC's analyzer, which follows values, needed false-positive repairs within four months. Call-site summaries have working ancestors, Nim's `effectsOf` and Lean's rules for `map`, but none refuses a program. The run-time abort stands.

### condition (Q2)
My reading would change on any of these:
- **A shipped checker that refuses endless recursion through a function argument, with a documented false-positive record.**
- **A correct Heroes program that ends normally** although its function calls a parameter on every path and the function passed in calls back on every path. That would show the summary route refusing a correct program.
- **Evidence that models actually write the parameter shape.** No historical precedent can supply this; it is the blind seat's question, together with repair 19's locality question. Go's Taylor objection in Q1 is the same concern about locality.

---

**Repository files read:**
- `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-203/docs/panel/203-briefs/{historian,00-shared}.md`
- `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-203/docs/panel/{201,199}-reports/historian.md`
- `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-203/docs/panel/203-reports/completeness-critic-pass1.md`
- `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-203/spec/heroes-spec.md`
- `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-203/docs/design.md`

**Sources:**
- https://www.cis.upenn.edu/~bcpierce/papers/lti-popl.pdf
- https://www.cis.upenn.edu/%7Ebcpierce/papers/lti-toplas.pdf
- https://repository.upenn.edu/cis_reports/180
- https://lampwww.epfl.ch/~odersky/papers/popl01.html
- https://openjdk.org/jeps/101
- https://openjdk.org/jeps/215
- https://bugs.openjdk.org/browse/JDK-8078093
- https://bugs.openjdk.org/browse/JDK-8077247
- https://blog.jooq.org/a-lesser-known-java-8-feature-generalized-target-type-inference/
- https://mail.openjdk.org/pipermail/lambda-dev/2013-July/010531.html
- https://github.com/swiftlang/swift/blob/main/docs/TypeChecker.md
- https://forums.swift.org/t/roadmap-for-improving-the-type-checker/82952
- https://cocoawithlove.com/blog/2016/07/12/type-checker-issues.html
- https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/AdvancedOperators.md
- https://go.dev/blog/type-inference
- https://go.dev/doc/go1.21
- https://github.com/golang/go/issues/50285
- https://doc.rust-lang.org/reference/expressions/literal-expr.html
- https://doc.rust-lang.org/reference/expressions/operator-expr.html
- https://rust-lang.github.io/rfcs/0212-restore-int-fallback.html
- https://github.com/dotnet/csharplang/blob/main/proposals/csharp-12.0/collection-expressions.md
- https://github.com/microsoft/TypeScript/pull/8944
- https://www.typescriptlang.org/play/4-7/new-js-features/improved-function-inference.ts.html
- https://www.typescriptlang.org/docs/handbook/type-inference.html
- https://git.dreamy.place/mirrors/rust/plain/src/librustc_mir/lints.rs?h=1.38.0
- https://kcg.chpc.utah.edu/atmos6910/Class_6/power_of_ten.pdf
- https://arxiv.org/abs/1809.00821
- https://www.mathworks.com/help/bugfinder/ref/misrac2012rule17.2.html
- https://gcc.gnu.org/pipermail/gcc/2021-July/236671.html
- https://gnu.googlesource.com/gcc/+/12c583a2a3da798e82737c2d0e11fd686636cfeb
- https://gnu.googlesource.com/gcc/+/070523b9d4c6cfa69060255893006efaf39bf617
- https://gnu.googlesource.com/gcc/+/refs/heads/trunk/gcc/testsuite/c-c++-common/analyzer/infinite-recursion-pr108524-1.c
- https://gnu.googlesource.com/gcc/+/refs/heads/trunk/gcc/analyzer/infinite-recursion.cc
- https://www.talks.cam.ac.uk/talk/index/7624/
- https://arxiv.org/abs/1311.5810
- https://lean-lang.org/doc/reference/stable/releases/v4.18.0/
- https://lean-lang.org/doc/api/Lean/Elab/PreDefinition/WF/Preprocess.html
- https://nim-lang.org/docs/manual.html
- https://nim-lang.org/blog/2021/10/19/version-160-released.html
- https://erlang.org/pipermail/erlang-questions/2007-August/028768.html
- https://erlang.org/pipermail/erlang-patches/2011-May/002117.html
- https://github.com/ziglang/zig/issues/1006
- https://doc.rust-lang.org/src/std/sys/pal/unix/stack_overflow.rs.html
