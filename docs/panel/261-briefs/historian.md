# Panel 261, the historian's brief

Read `00-shared.md` first, whole: § 1 (the proposal), § 2 (the questions), § 3.5 (who in this project weighed it
before), § 5 (the router and the programs that drive the sitting), § 6.2 (three sketches of the router) and § 7 (the
routes nobody listed). **Your agent file is the base's** (`.claude/agents/historian.md`): every factual claim carries a
source, verified by a web search or a page you read, and **anything you could not confirm is written `unverified`**.
You have no veto. Your report is `docs/panel/261-reports/historian.md`, written as you go, in the tree the
coordinator's prompt names. You read pages; you start no paid run, download nothing, and save no file but your report.

## The question you answer

**Which languages let a function type carry a parameter's passing mode (in-out, by reference, read-only by reference),
what did a call through such a value write at its call site, and what did that cost or buy them?** And **how do the
routers the author names (Gin, Echo) bind a route to a handler that reaches the program's state**, given that Heroes
has no closure (design.md Part 6, panel 119) and records take no type parameter (panel 252's R3). Every precedent
below is what the coordinator or the clerk recalls, **none verified by this sitting**: each is a question for you, and
a precedent you cannot source is written `unverified` or dropped. For each, say:

- **(type)** how the mode is spelled inside the function type, quoted from the reference or the specification;
- **(call)** whether a call through a value of that type marks the argument at the call site (`&mut x`, `&x`, `ref x`,
  nothing), and whether the mark is required;
- **(identity)** whether two function types differing only in a mode are two types, and whether one converts to the
  other (a read-only one where an in-out one is asked, or the reverse);
- **(alias)** what the language says of two in-out arguments of one call reaching one place through a value
  (exclusivity, Swift's SE-0176, Ada 2012's rule, Hylo), and whether a callee read out of the place the call lends is
  ruled on;
- **(history)** when the form entered, the bugs and complaints, and any proposal that changed it, with dates and
  versions.

1. **Rust**: `fn(&mut T)` as a function pointer type, and the `FnMut` / `Fn` / `FnOnce` traits for closures; whether
   `f(&mut x)` is required through a value; whether a `fn(&T)` coerces to anything taking `&mut T` (it does not, as
   recalled: verify).
2. **Swift**: `inout` in a function type, `(inout Int) -> Void`; `&x` at the call through a value; SE-0176 (exclusive
   access) and whether it reaches a closure or function value read from the same variable it is passed.
3. **C#**: `ref`, `out` and `in` parameters in a delegate type (`delegate void D(ref int x)`), and `ref` at the call;
   whether `in` (read-only by reference) delegates convert.
4. **Ada**: access-to-subprogram types whose profile has `in out` parameters (`type P is access procedure (X : in out
   T)`); the profile conformance rule (mode conformance), and Ada 2012's illegal-overlap rule for a call through such a
   value.
5. **Go**: Gin's handlers, `func(c *gin.Context)`, and Echo's, `func(c echo.Context) error`; how a handler reaches the
   program's state (a closure over it, a method value, the context's store), quoted from each framework's docs; the
   standard library's `http.HandlerFunc`.
6. **Nim**: `var` parameters in a proc type (`proc (x: var int)`), and the call through it.
7. **D**: `ref` in a function pointer or delegate type (`void function(ref int)`).
8. **Also verify, since the language's specification is written in the tradition of Wirth's reports**: Modula-2's and
   Oberon's procedure types with `VAR` parameters (`PROCEDURE (VAR INTEGER)`), and whether a procedure variable's
   type matches by the formal parameters' modes (Oberon's report, *"matching formal parameter lists"*).
9. **Also verify, as the route sketch (C) of `00-shared.md` § 6.2 is theirs**: Rust's axum, whose `Router<S>` is
   generic over the state and whose `State<S>` extractor hands a handler the state (`Router::with_state`); and Hylo's
   `inout` in a function type, if it has one (panel 217 cited Hylo's `inout` call marks at line 107 of
   `docs/panel/217-a-read-only-lend-is-lent-on-a-parameter-the-callers-place-read-and-never-written-refused-beside-an-at-of-that-place-and-past-a-variadic-prototype.md`,
   re-read by the repair with `sed -n 107p`; your tools open a path, never a glob).
10. **The route nobody listed** (`00-shared.md` § 7): is there a language with neither closures nor generic types
    whose routers bind handlers to shared mutable state, and how (a handler table indexed by the route, a state
    parameter the framework threads, an exhaustive variant of routes)? Name it with a source, or say none was found and
    what you searched. **One candidate, a question and not a fact** (the critic's first reading, § 5, which read no
    page of it): C itself, whose routers and callback registries hand the program's state as a `void *` the program
    owns beside a function pointer, the state's type erased at the boundary and restored by a cast in the handler. Is
    that a route of its own (state by an untyped pointer the framework threads), with what its pages say it cost?

**Context, not a question**: panel 138's historian found OCaml refusing constructors as functions for 25 years, quoted
at line 58 of `docs/panel/138-the-item-named-a-parser-that-never-existed-and-the-route-was-already-in-the-grammar.md`
(*"are not functions"*); that sitting also recorded, on 2026-09-13, that `@` in function types was *"a second feature
... that Part 7 does not list and no sitting has proposed"* (its lines 45 to 48, `sed -n`, the repair).

## What you write

One section per precedent, each ending with a line **`verified`** (a page read, quoted, its URL and the day you read
it) or **`unverified`**; then a table: precedent · mode in the type · mark at a call through a value · two types or
one · converts · alias rule. Then which precedent each route of `00-shared.md` § 6.2 and § 7 rests on, and **what the
history says went wrong** where a mode was carried (or not carried) in a function type. **Each line says what the
page itself states**, never what Heroes would do about it (`.claude/rules/records.md` § And the record says whose idea
it was: of 90 lines four researchers once proposed, three re-readers found 33 saying more than their page did).
