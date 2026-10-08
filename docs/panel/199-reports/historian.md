# Panel 199, historian

Copied by the coordinator at 09:56 on 2026-10-08 (`date`) from the seat's reply,
verbatim apart from this header and the HTML entities of the notification
turned back into their characters: the harness refused the seat's Write of its
notes file (*subagents should return findings as text*), so it wrote none, and
the whole report is its reply. The seat was stopped once by the session limit at
about 08:25 and resumed at 09:50 on a new account; it has no shell, so it could
not grep the repository, and every source was read through the web tools, whose
summarising model stands between the seat and the page: a quotation below is
what the tool returned, not a byte-checked copy. Re-read any load-bearing one at
source before it lands in a record.

---

## Panel 199, historian: report

**The notes file was never written.** My first Write of `<scratchpad>/199-historian/report.md` was refused ("subagents should return findings as text"). Resuming from it meant nothing, so the whole report is below for you to copy. I did not retry the Write.

All sources were read on 2026-10-08. Every WebFetch passes through a summarising model, so a "quote" below is what that tool returned, not a byte-checked copy. Re-read any load-bearing one at source before it lands in a record. This seat has no shell, so I read the repository files named in the brief and could not grep. *Heroes of code* was not consulted and no claim rests on it.

### verdict
**approve (advisory)** for a narrow checker error: (A) or (H), with (F) first, (G), and (L) beside them. **Object** to (C), (K), (D) and to a new warning kind.

### precedents

**1. How other compilers tell a function whose every path calls itself**

- **rustc `unconditional_recursion`: warn by default. Verified.**
  - Source: https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html shows `#[warn(unconditional_recursion)]` on by default.
  - Added by huonw in PR #20373, merged 2015-01-25 (https://github.com/rust-lang/rust/pull/20373), before Rust 1.0 on 2015-05-15 (https://blog.rust-lang.org/2015/05/15/Rust-1.0/).
  - It closed issue #17899, opened 2014-10-09 (https://github.com/rust-lang/rust/issues/17899). That issue's motivating case is `self.bar(x)` inside `impl Foo for Box<Foo>` calling itself through method dispatch. That is the UFCS shape's closest ancestor.
  - Misses: indirect and mutual recursion. Issue #57965, opened 2019-01-29 and still open (https://github.com/rust-lang/rust/issues/57965), is cited in the source as `FIXME(#57965): Make this work across function boundaries` (https://doc.rust-lang.org/nightly/nightly-rustc/src/rustc_mir_transform/check_call_recursion.rs.html).
  - **Hidden aborts (question 2). Partly inference.**
    - Verified: its terminator classifier ends a path on `Return`, `Unreachable`, `UnwindResume`, `UnwindTerminate`, `Yield` and diverging `InlineAsm` only. `Assert`, `Call`, `Drop` and `SwitchInt` continue the search.
    - Unverified inference: that an overflow check is an `Assert` terminator. I did not read that in this sitting. If it holds, rustc would flag p2.
  - A generic self-call `generic::<Option<T>>()` gets the warning plus a monomorphisation recursion-limit error (https://rust.googlesource.com/rust/+/HEAD/tests/ui/recursion/infinite-function-recursion-error-8727.stderr).
  - **Check/build split.** RFC 3477 (start date 2023-08-22) says `cargo check` "only catches some subset of the possible compilation errors" and post-monomorphisation ones need `cargo build` (https://rust-lang.github.io/rfcs/3477-cargo-check-lang-policy.html). That is the precedent for `polymorphic_recursion` (check 0, build 1).
- **clang `-Winfinite-recursion`. Verified.**
  - Review D1864 by rtrieu, 2013-10-08 (https://reviews.llvm.org/D1864): "searches the CFG to determine if every codepath results in a self call". It was `DefaultIgnore` and outside `-Wall` then. The test says no warning for mutual recursion, and it is disabled in template instantiations "due to false positives".
  - Today the diagnostics reference lists it under `-Wmost`, which `-Wall` controls (https://clang.llvm.org/docs/DiagnosticsReference.html).
  - Rewritten by CodaFi (D43737, 2018-02-24; https://reviews.llvm.org/D43737?id=138302). A later follow-up there found a false positive when the exit node is unreachable.
  - Bogus-warning report 41556 (2019-04-22; https://lists.llvm.org/pipermail/llvm-bugs/2019-April/073963.html): a friend `operator<<` called "with a different object".
  - Clang's own test says no warning for `void l(){ while(true){} l(); }` and for a static-guard function (https://llvm.googlesource.com/llvm-project/+/refs/heads/main/clang/test/SemaCXX/warn-infinite-recursion.cpp).
  - Whether clang treats a call to a non-`noreturn` `exit` wrapper as ending a path is unverified from documentation. Defect 507 is the measured fact.
- **GCC `-Winfinite-recursion`: in `-Wall` since GCC 12. Verified for the doc, unverified for the date.**
  - The 12.1.0 doc has it: "effective at all optimization levels but requires optimization in order to detect infinite recursion in calls between two or more functions. -Winfinite-recursion is included in -Wall" (https://gcc.gnu.org/onlinedocs/gcc-12.1.0/gcc/Warning-Options.html). The 11.4.0 doc has no entry (https://gcc.gnu.org/onlinedocs/gcc-11.4.0/gcc/Warning-Options.html).
  - The source credits Martin Sebor and says "A noreturn call breaks infinite recursion", likewise a longjmp, a siglongjmp and a throw (https://gnu.googlesource.com/gcc/+/refs/heads/master/gcc/gimple-warn-recursion.cc).
  - Commit date 2021-11-23, patch v2 2021-11-11, and a false-positive fix for glibc `gnu_inline` wrappers (PR 104633): **unverified**. Those come from search snippets only, because the patchwork and bugzilla pages were access-denied.
  - Fergus Henderson proposed copying Mercury's check into GCC on 2001-07-06 (https://gcc.gnu.org/legacy-ml/gcc/2001-07/msg00401.html), twenty years before it shipped.
  - Defect 507 (`serve`) is the false positive GCC avoids by design.
- **Swift: a warning. Verified.**
  - SR-626 "No diagnostic for unconditional recursion", 2016-01-27, resolved (https://github.com/apple/swift-issues/issues/626). The version it shipped in is **unverified**.
  - The `DiagnoseInfiniteRecursion` header (https://raw.githubusercontent.com/swiftlang/swift/release/5.9/lib/SILOptimizer/Mandatory/DiagnoseInfiniteRecursion.cpp) says it handles "invariant conditions due to forwarded arguments", i.e. `f(x)` with `x` unchanged. That is route (H).
  - Forum threads show:
    - Mutual recursion is missed, and a flow-insensitive version would flag `isEven`/`isOdd` (codafi, 2020-03-02; https://forums.swift.org/t/avoiding-unintentional-infinite-recursion/34225).
    - `#available` is not read as constant (2019-11-23; https://forums.swift.org/t/all-paths-through-this-function-will-call-itself/31068).
    - A property that resolves to itself is warned inconsistently (2020-08-12; https://forums.swift.org/t/inconsistent-behavior-with-all-paths-through-this-function-will-call-itself-warning/39321).
- **MSVC C4717: a level-1 warning. Verified.** "recursive on all control paths, function will cause runtime stack overflow" (https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4717). The doc's example has two recursive paths and no exit.
- **Go: no compiler or vet check. Verified.**
  - The `go vet` analyzer list has nothing about recursion (https://pkg.go.dev/cmd/vet).
  - Its printf analyzer reports "causes recursive String method call". That message is verified in #30441 (2019-02-27), along with a false positive; #23550 (2018-01-25) is another false-positive report, and #55928 (2022-09-28, open) is a generic-method miss (https://github.com/golang/go/issues/30441, https://github.com/golang/go/issues/23550, https://github.com/golang/go/issues/55928).
  - staticcheck SA5007 "Infinite recursive call", available since 2017.1 and enabled by default (https://staticcheck.dev/docs/checks/#SA5007). The critic's name is therefore verified, and it is a third-party tool.
  - The Go compiler itself: unverified, not read.
- **Java.**
  - No `-Xlint` key about recursion in `javac` (https://docs.oracle.com/en/java/javase/21/docs/specs/man/javac.html). Verified.
  - **Error Prone `InfiniteRecursion` is ERROR severity and on by default** (https://errorprone.info/bugpattern/InfiniteRecursion, https://errorprone.info/bugpatterns). Its fixes are "call a different overloaded method" or "call the method on a different instance". It is a javac plugin, not javac.
- **Haskell: nothing in GHC. Verified.**
  - GHC #9207 "Detect obvious cases of infinite recursion", opened 2014-06-23, closed as invalid (https://mail.haskell.org/pipermail/ghc-tickets/2014-June/013637.html).
  - SPJ on 2014-06-24: GHC already does a guaranteed-divergence analysis but "it does not distinguish an infinite loop from a call to `error`" (https://mailman.haskell.org/archives/list/ghc-tickets@haskell.org/message/IQOMNETY525Z2AJ4QPERUIXGOD5336LJ).
  - The users-guide warning list has no recursion warning (https://downloads.haskell.org/ghc/latest/docs/users_guide/using-warnings.html).
- **Mercury, the oldest precedent for (H). Verified.**
  - Fergus Henderson, 1997-02-09 (https://lists.mercurylang.org/archives/developers/1997-February/000038.html): warn when a direct recursive call's input arguments are equivalent to the procedure's own head variables.
  - Zoltan Somogyi, 2019-04-20 (https://lists.mercurylang.org/archives/developers/2019-April/016989.html), asked whether a loosened form that adds false positives is worth it. `--warn-suspicious-recursion` followed on 2019-05-01 (https://lists.mercurylang.org/archives/reviews/2019-May/020354.html).
- **C# and Kotlin: no general check found.** C# has CA2011 (a property assigned inside its own setter), enabled "as suggestion" (https://learn.microsoft.com/en-us/dotnet/fundamentals/code-analysis/quality-rules/ca2011). Kotlin's JetBrains IDE inspection "Recursive property accessor" (https://www.jetbrains.com/help/inspectopedia/RecursivePropertyAccessor.html) is IDE-only. A snippet claiming a Visual Studio 2005 getter warning was not read: **unverified**.
- **Zig: no recursion diagnostic found.**
  - "zig doesn't have warnings, and they are not planned. The reasoning being warnings can be ignored" is a forum user's statement, 2026-04-19 (https://ziggit.dev/t/pedantic-warning/15050). It is not an official source, so it is **unverified as policy**.
  - Zig issue #335 (opened 2017-04-19, open, "accepted") asks for compile errors for unused things (https://github.com/ziglang/zig/issues/335).

**2. Any language that made it an error, and why; any that removed it**

- **Error Prone is the one verified "error".** Sadowski et al., CACM 61(4), April 2018, pp. 58-62 (https://6826.csail.mit.edu/2020/papers/google-analysis-cacm.pdf), read as page images. Their criteria for a compiler check: "easily understood; actionable and easy to fix (whenever possible, the error should include a suggested fix that can be applied mechanically); produce no effective false positives (the analysis should never stop the build for correct code); and report issues affecting only correctness rather than style". Their reason for errors over warnings: the Clang team enabled the diagnostic "as a compiler error (not a warning, which the Clang team found Google developers ignored)". They cleaned the codebase first.
  - Heroes has no warning level, so this is the model. It also makes defect 507 (an error on a correct program) disqualifying until (F) lands.
  - Panel 020's *unsupported* kind is the same posture. It is a diagnostic kind that is not a claim about the program.
- **D `auto` return type gives an error on infinite recursion.** That is a forum statement by Quirin Schroll, 2023-05-25 (https://lists.puremagic.com/pipermail/digitalmars-d/2023-May/335086.html). It is **unverified** against dmd source or spec. The same post says dmd does not detect `int f(int x) => f(x);`.
- **None removed.** I found only narrowing: clang's template exemption (D1864), clang's exit-node fix (D43737), and Go's printf recursive-Stringer check, whose "relax" and "exclude %#v" commit titles appear only as search-result titles (the commit pages were unreadable: **unverified**).

**Tail position and `-O2`**

- GCC: "`-foptimize-sibling-calls` Optimize sibling and tail recursive calls. Enabled at levels -O2, -O3, -Os" (https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html). Verified.
- LLVM `tailcallelim`: it "transforms calls of the current function (self recursion) followed by a return instruction with a branch to the entry of the function, creating a loop" (https://llvm.org/docs/Passes.html, https://raw.githubusercontent.com/llvm/llvm-project/main/llvm/lib/Transforms/Scalar/TailRecursionElimination.cpp). The pass returns early on the `disable-tail-calls` attribute. Clang's own test shows `-fno-optimize-sibling-calls` sets `"disable-tail-calls"="true"` (https://llvm.googlesource.com/llvm-project/clang/+/refs/heads/main/test/CodeGen/attr-disable-tail-calls.c). So (G) has a documented flag in both compilers.
- **Incidents.**
  - CPython, 2015-03-12 (https://bugs.python.org/msg237993): faulthandler's `stack_overflow()` "may become an infinite loop that does not expand the stack ... 100% CPU ... won't respond to SIGINT". The Intel compiler did it, and GCC 4.8.2 and Clang 3.4.2 did not at the time.
  - D forum, 2023-05-24 (https://forum.dlang.org/thread/upfejjbihndplawetjol@forum.dlang.org): the author's wrapper recursed into itself and "was optimised down into a simple infinite loop".
- **Guaranteed by design.**
  - Lua 5.4 §3.4.10: "there is no limit on the number of nested tail calls" (https://www.lua.org/manual/5.4/manual.html).
  - Kotlin `tailrec`: "the compiler optimizes out the recursion, leaving behind a fast and efficient loop" (https://kotlinlang.org/docs/functions.html).
- **No compiler documentation found** that says an unbounded recursion stops aborting and runs for ever at some `-O` level. A Rust users-forum claim and the LLVM `mustprogress` story came from search snippets only: **unverified**.

**What ends a path (question 2)**

- Go fixes the list by syntax: `return`, `goto`, a call to built-in `panic`, blocks, `if` with `else`, `for` with no condition and no `break`, `select`, `switch` with a default. `os.Exit` is not on it (https://go.dev/ref/spec, "Terminating statements").
- Java (JLS 14.22, https://docs.oracle.com/javase/specs/jls/se21/html/jls-14.html#jls-14.22) defines "can complete normally" by syntax. The fetch returned a note that the rules do not treat a never-returning call such as `System.exit` as preventing completion. I could not confirm that note's exact wording.
- Panel 184 cites Java's rule "since 1996". I did not re-verify that date.
- GCC and clang end a path only on a `noreturn` call. Rust continues through its `Assert` terminator (see above).
- Heroes' written ends (spec §8, `exit(code:)`, `assert false`, an unbroken `while true`) are syntactic like Go's and Java's. The overflow, index and `.must()` aborts are not path ends, so (A) as written refuses p2, as rustc seems to and clang does not.

**3. UFCS**

- **D bug 11327** (2013-10-22, https://digitalmars.com/d/archives/digitalmars/D/bugs/Issue_11327_New_Regression_2.059_ICE_in_a_recursive_UFCS_call_57324.html): `S call(S)(S s) { return s.call(); }` was an "accidental recursive call to UFCS function" and compiled. It was closed invalid. UFCS shipped in D 2.059 (2012-04-12; https://dlang.org/changelog/2.059.html). This is the Heroes shape exactly, and D added no diagnostic. I did not read the bug page itself (it returned 522). The code and resolution above come from the digitalmars mailing-list copy.
- D's UFCS rules: a free function is a candidate only when the member does not exist, and local-scope functions are not found, "to avoid unexpected name conflicts" (https://dlang.org/spec/function.html). No documented self-recursion pitfall found there.
- **Nim:** method call syntax is "syntactic sugar" (https://nim-lang.org/docs/manual.html). No documented pitfall found in two searches. Nim has a runtime depth counter, `const nimCallDepthLimit {.intdefine.} = 2000` (https://raw.githubusercontent.com/nim-lang/Nim/devel/lib/system/excpt.nim). Its message says "in a debug build", and which builds compile it in is **unverified**. This is the counter panel 104 rejected.
- **Swift:** the 2019-11-23 and 2020-05-09 forum threads are typo-made self-calls (https://forums.swift.org/t/all-paths-through-this-function-will-call-itself/36330).

**4. This repository's own precedents (frozen tree, read whole)**

- **Panel 020** (ratified 2026-08-12): a report that is not an error is a *kind*, because `#~ <code>` keys off codes. `selfhost/diag.hero:22-27` holds `error_kind` and `unsupported_kind`. If (C) is a new kind, this is its template. The GCC `kinds.def` and GHC `Sorry` claims are carried from that record and **not re-verified here**. GCC's own kinds include `warning`, so Heroes' two-kind set is a deliberate narrowing.
- **`design.md:3725`** is a "Note also" inside wart 15's text, a statement of fact about the compiler. I found no panel that ruled it as policy, but I could not grep.
- **Panel 070** (2026-08-16): "it depends on the optimisation level" was falsified for a shape whose call is followed by work (`array.c:74` inside a `for`, then `hero_release_block`). Its *not permitted* sentence is about non-tail calls only. Its note that the harness runs every `run/` case at `-O0` and `-O2` means a `run` golden of the endless shape would hang at `-O2`, so any case must be a `check` case.
- **Panel 104** (2026-09-03):
  - The counter rejection is at lines 65-69 of the frozen file, not 64-68. Its measurement was `down(n)` returning `1 + down(n - 1)`: non-tail, so its guard fires at `-O2`.
  - The same text records plain recursion costing 0.000 s at `-O2`, "the transformation that turns the recursion into a loop". That was true for that shape. It does not contradict defect 508's tail-position shape.
  - 104 had no historian seat.
- **Panel 097** does not bear. It is about the filesystem platform arm and has no stack, recursion or `-O2` text. The one echo is its condition 7, "a green `doctor` with a red `build` is worse than a red `doctor`", which is the shape of landing (C) or (K) before (F).
- **Panels 184 and 185:** the written path ends are read by syntax, never by value (184 R4; spec §8, `:238-240`). 185 R5 made one predicate for a function's end, a value arm and a value block. A new "ends a path" rule must reuse that predicate or it creates a third.

**Searched, found nothing**

- Kotlin compiler (general self-call).
- Roslyn (general self-call).
- A Zig recursion diagnostic.
- A Nim method-call recursion pitfall.
- A compiler stating the `-O2` loop consequence.
- A language that removed the check.
- Idris, Erlang, Scheme, OCaml, K/APL: not searched.

### argument
Every compiler that ships such a check ships a narrow one: rustc, clang, GCC, MSVC, Swift, staticcheck, Error Prone. None catches mutual recursion, and rustc#57965 has been open since 2019. The one that made it an error, Error Prone, did so under a no-false-positives rule, because warnings get ignored. Heroes has no warning level, so a narrow (A) or (H) is an error or it is nothing. Land (F) first: GCC treats a `noreturn` call as ending a path, and defect 507 is the false positive such checks were narrowed to avoid. No compiler documents the `-O2` loop, but both have a flag. D met this exact UFCS shape in 2013 and added nothing.

### condition
My reading changes if I find:

1. A compiler that shipped an *error* for unconditional recursion and reverted it over false positives. That would turn me against (A) as an error.
2. A compiler that documents tail self-calls becoming jumps at `-O2` as intended behaviour. That favours (L) naming the level over (G).
3. A shipping check that counts an overflow or bounds abort as a path end. That would reopen question 2.
4. Evidence that models actually write the UFCS self-call (question 7, which I cannot supply). Without it, none of this precedent shows the mistake is common in generated code.

Files read are all under `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-199/`: `docs/panel/{020,070,097,104,184,185}-*.md`, `selfhost/diag.hero`, `spec/heroes-spec.md` lines 230-290, `docs/design.md` lines 3700-3750, and the two briefs.

Sources:
- https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html
- https://github.com/rust-lang/rust/pull/20373
- https://github.com/rust-lang/rust/issues/17899
- https://github.com/rust-lang/rust/issues/57965
- https://doc.rust-lang.org/nightly/nightly-rustc/src/rustc_mir_transform/check_call_recursion.rs.html
- https://rust-lang.github.io/rfcs/3477-cargo-check-lang-policy.html
- https://reviews.llvm.org/D1864
- https://reviews.llvm.org/D43737?id=138302
- https://gcc.gnu.org/onlinedocs/gcc-12.1.0/gcc/Warning-Options.html
- https://gnu.googlesource.com/gcc/+/refs/heads/master/gcc/gimple-warn-recursion.cc
- https://github.com/apple/swift-issues/issues/626
- https://raw.githubusercontent.com/swiftlang/swift/release/5.9/lib/SILOptimizer/Mandatory/DiagnoseInfiniteRecursion.cpp
- https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4717
- https://pkg.go.dev/cmd/vet
- https://staticcheck.dev/docs/checks/#SA5007
- https://errorprone.info/bugpattern/InfiniteRecursion
- https://6826.csail.mit.edu/2020/papers/google-analysis-cacm.pdf
- https://mail.haskell.org/pipermail/ghc-tickets/2014-June/013637.html
- https://lists.mercurylang.org/archives/developers/1997-February/000038.html
- https://digitalmars.com/d/archives/digitalmars/D/bugs/Issue_11327_New_Regression_2.059_ICE_in_a_recursive_UFCS_call_57324.html
- https://dlang.org/spec/function.html
- https://go.dev/ref/spec
- https://docs.oracle.com/javase/specs/jls/se21/html/jls-14.html#jls-14.22
- https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
- https://llvm.org/docs/Passes.html
- https://bugs.python.org/msg237993
- https://forum.dlang.org/thread/upfejjbihndplawetjol@forum.dlang.org
- https://www.lua.org/manual/5.4/manual.html
- https://kotlinlang.org/docs/functions.html
