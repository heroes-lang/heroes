# Panel 261, the historian's report: a function type carries its parameters' modes

Seat: `historian` (advisory, no veto). Base: `df92f9281`. Written as it went, 2026-11-26, sections 1 to 4 first.
Every page below was read on **2026-11-26** with the session's web tools (search and page fetch); no paid run, no GitHub
write, no file saved by this seat but this report. **One thing to report**: fetching Wirth's Oberon-07 report, a PDF,
made the fetch tool keep the PDF's bytes in its own results folder outside the repository (the tool's doing, not a
request of this seat; the text was unreadable to it and nothing here rests on it); every later source was an HTML or
text page for that reason. A line marked **verified** quotes the page it names; **unverified** could not be confirmed
on a page read that day and carries no weight. Each line says what the page states, never what Heroes would do about
it (`.claude/rules/records.md` § And the record says whose idea it was). The fetcher cuts a quotation at about 125
characters, so several quotations are fragments of one sentence, each fragment the page's own words.

## 1. Rust

- **(type)** A function pointer's parameter is a type, with no mode beside it: the Reference's grammar is
  `MaybeNamedParam → OuterAttribute* ( ( IDENTIFIER | _ ) : )? Type`
  ([function pointer types](https://doc.rust-lang.org/reference/types/function-pointer.html)). So `fn(&mut T)` carries
  `&mut` as part of the parameter's **type** `&mut T`, not as a mode. *"Function pointers can be created via a coercion
  from both function items and non-capturing, non-async closures."* (same page).
- **(call)** At a call by name the Book writes the borrow at the call: *"we create a mutable reference with `&mut s`
  where we call the `change` function"*, `change(&mut s)`
  ([the Book, 4.2](https://doc.rust-lang.org/book/ch04-02-references-and-borrowing.html)). No page read states a rule
  for a call through a `fn` value separately: **unverified** as a statement about calls through a value.
- **(identity, converts)** *"Subtyping is restricted to two cases: variance with respect to lifetimes and between
  types with higher ranked lifetimes. If we were to erase lifetimes from types, then the only subtyping would be due to
  type equality."* ([subtyping](https://doc.rust-lang.org/reference/subtyping.html)). The coercion list has *"`&mut T`
  to `&T`"* for values, and for functions only *"Function item types to `fn` pointers"* and *"Non capturing closures to
  `fn` pointers"* ([coercions](https://doc.rust-lang.org/reference/type-coercions.html)): the list names no coercion
  from `fn(&T)` to `fn(&mut T)` or back. The brief's recollection (*"it does not, as recalled"*) is consistent with
  the list; the list is the evidence, not a sentence saying so.
- **(alias)** *"If you have a mutable reference to a value, you can have no other references to that value."* and
  `error[E0499]: cannot borrow `s` as mutable more than once at a time` (the Book, 4.2).
- **(a callee kept in the place the call lends)** rustc's test suite pins that calling a closure borrows it:
  `borrowck-call-is-borrow-issue-12224.rs`, *"Ensure that invoking a closure counts as a unique immutable borrow"*,
  with `f.f.call_mut(())` refused, *"cannot borrow `f.f` as mutable, as it is behind a `&` reference"*
  ([the test file](https://fuchsia.googlesource.com/third_party/rust/+/2fcea9fb68c8e04f10e5cb15bbfb486de9800afa/tests/ui/span/borrowck-call-is-borrow-issue-12224.rs)).
  On the users' forum, 2022-05-08, for a struct that wants a list of handlers each taking the struct: steffahn,
  *"you could just collect fn(&mut Foo) function pointers"*; for `Box<dyn FnMut(&mut Foo)>` calling them conflicts with
  the struct's own borrow, and the advice is to take the vector or one item out of `self` and put it back
  ([thread 75385](https://users.rust-lang.org/t/vector-of-pointers-to-struct-impl-methods/75385); the second half is
  the fetcher's summary of the post, the first the post's words). The Reference's call-expression page does not state
  whether the function operand is evaluated before the arguments
  ([call expressions](https://doc.rust-lang.org/reference/expressions/call-expr.html)).
- **verified**: the pages above, read 2026-11-26; the call-through-a-value line **unverified**.

## 2. Swift

- **(type)** *"To specify an in-out parameter, prefix the parameter type with the `inout` keyword."* and *"Argument
  names in functions and methods aren't part of the corresponding function type."* (The Swift Programming Language,
  Types, Function Type; source
  [Types.md](https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/ReferenceManual/Types.md); the
  rendered docs.swift.org page needs a script and returned nothing to the fetcher).
- **(history of the spelling)** **SE-0031**, *"Adjusting `inout` Declarations for Type Decoration"*, **Implemented
  (Swift 3.0)**: *"We propose to move the `inout` keyword to the right side of the colon to decorate the type instead of
  the parameter label."*; *"In Swift 2, the `inout` parameter lives on the label side rather than the type side of the
  colon although the keyword isn't modifying the label but its type."*; its examples *"(x: inout T) -> U // => (inout
  T) -> U"* and *"func foo(inout x: T) // foo(_:), type (inout T) -> Void"*
  ([SE-0031](https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0031-adjusting-inout-declarations.md)).
- **(labels left the type)** **SE-0111**, *"Remove type system significance of function argument labels"*,
  **Implemented (Swift 3.0)**: *"Function types may only be defined in terms of the types of the formal parameters and
  the return value."*; *"Removing this feature simplifies the type system."*
  ([SE-0111](https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0111-remove-arg-label-type-significance.md)).
- **(call)** The argument to an in-out parameter *"must be prefixed with an ampersand (`&`)"* (Declarations, In-Out
  Parameters, [Declarations.md](https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/ReferenceManual/Declarations.md)),
  stated of in-out parameters, not of calls through a value in particular.
- **(the model)** The same section: *"When the function is called, the value of the argument is copied."*, *"In the
  body of the function, the copy is modified."*, *"When the function returns, the copy's value is assigned to the
  original argument."*; *"This behavior is known as copy-in copy-out or call by value result."*; the optimisation
  *"is known as call by reference"*; *"you can't pass the same value to multiple in-out parameters"*; and *"A closure or
  nested function that captures an in-out parameter must be nonescaping."*
- **(a read-only and an owning mode in the type, and their conversion)** **SE-0377**, *"`borrowing` and `consuming`
  parameter ownership modifiers"*, **Implemented (Swift 5.9)**, writes them in function types, `let f: (borrowing Foo)
  -> Void`, beside `(inout Foo) -> Void`, and rules: *"Function values can also be implicitly converted to function
  types that change the convention"* *"of parameters of copyable types among unspecified, `borrowing`, or
  `consuming`:"*, with `let g: (borrowing Foo) -> Void = f`; and *"These implicit conversions for protocol conformances
  and function values are not available"* *"for parameter types that are noncopyable, in which case the convention must
  match exactly."* `inout` is not in the list of interconvertible conventions; the modifiers *"are mutually exclusive
  with each other and with `inout`"*; `inout` gets *"exclusive access"*, `borrowing` *"shared access"*
  ([SE-0377](https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0377-parameter-ownership-modifiers.md)).
- **(alias)** **SE-0176**, enforce exclusive access to memory, **Implemented (Swift 4.0)**: *"Passing 'x' as an inout
  argument is a write access for the duration of the call."*; `swap(&array[0], &array[1])` is a *"CONFLICT"* since
  *"This call makes two such accesses to the same array variable, which therefore conflict."*; and of `x += x`, the
  inout access *"does not begin until immediately before the call, after the right-hand side is fully evaluated"*
  ([SE-0176](https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0176-enforce-exclusive-access-to-memory.md)).
  On the callee: the fetcher found no sentence of SE-0176 on a function value read out of the variable passed `inout`,
  nor on the callee's evaluation relative to the access; **unverified** that Swift rules on it.
- **verified**: every quotation above, read 2026-11-26; the callee half of (alias) **unverified**.

## 3. C#

- **(type)** A delegate type is declared with a parameter list whose parameters carry `ref`, `out`, `in` or `ref
  readonly`; *"Delegate types in C# are name equivalent, not structurally equivalent."*, `D1` and `D2` of one signature
  being *"two different types"* (C# standard, §21.2,
  [Delegates](https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/delegates)).
- **(identity)** §21.4, delegate compatibility: *"each parameter in `D` has the same by-reference parameter modifier as
  the corresponding parameter in `M`"*, and *"For each by-reference parameter, the parameter type in `D` is the same as
  the parameter type in `M`"*, while value parameters admit *"an identity conversion ... or implicit reference
  conversion"* (same page). And *"Members of a class can't have signatures that differ only by `ref`, `ref readonly`,
  `in`, or `out`."* ([Method parameters](https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/method-parameters),
  dated 2026-01-22 on the page).
- **(call)** *"An argument for a `ref` parameter must include the `ref` modifier."*; *"An argument for an `out`
  parameter must include the `out` modifier."*; *"An argument for an `in` parameter can optionally include the `in`
  modifier."*; *"An argument for a `ref readonly` parameter should include either the `in` or `ref` modifiers ... If
  neither modifier is included, the compiler issues a warning."* (Method parameters). A delegate's `Invoke` has a
  parameter list that *"matches the parameter_list in the delegate declaration"* (§21.3), and §21.6: *"If such a
  delegate invocation includes reference parameters, each method invocation will occur with a reference to the same
  variable"*.
- **(converts: a read-only method where a read-write type is asked)** The C# 12 proposal *"ref readonly parameters"*:
  for method group conversions *"these modifiers are considered compatible (but any allowed conversion between
  different modifiers results in a warning)"*; *"`in` parameter of the target method is allowed to match `ref readonly`
  or, gated on LangVersion, `ref` parameter of the delegate."*; *"`ref` parameter of the target method is *not* allowed
  to match `in` nor `ref readonly` parameter of the delegate."*; examples `DRef dRef = (in int p) => { }; // warning:
  mismatch between `in` and `ref`` and `DIn dIn = (ref int p) => { }; // error`. And for function pointers, in the same
  language: *"implicit function pointer conversions are disallowed if there is a mismatch between reference kind
  modifiers, and explicit casts are always allowed without any warnings."*
  ([proposal source](https://raw.githubusercontent.com/dotnet/csharplang/main/proposals/csharp-12.0/ref-readonly-parameters.md);
  the learn.microsoft.com copy answered 404).
- **(history)** The same proposal's motivation: *"C# 7.2 introduced `in` parameters as a way to pass readonly
  references."*; *"`in` parameters allow both lvalues and rvalues and can be used without any annotation at the
  callsite."*; *"However, APIs which capture or return references from their parameters would like to disallow
  rvalues"* *"and also enforce some indication at the callsite that a reference is being captured."*; *"`ref readonly`
  parameters are ideal in such cases as they warn if used with rvalues"* *"or without any annotation at the
  callsite."* And on a mode inside a delegate's signature: *"Note that the rules outlined above apply to method
  signatures, but not to delegate signatures."* *"For example, changing `ref` to `in` in a delegate signature can be a
  source breaking change"* *"(if a user is assigning a method with `ref` parameter to that delegate type,"* *"it would
  become an error after the API change)."*
- **(alias)** None of the pages read states a rule refusing two `ref` arguments that name one variable; that is a
  sentence about what four pages do not say, not about C#.
- **verified**: the quotations above, read 2026-11-26.

## 4. Ada

- **(type)** *"Ada 95 provides access-to-subprogram types"*, *"useful for installing call-backs in a separate
  subsystem"* and *"for table-driven programming, using, for example, a state machine model"*; its example `type
  Integrand is access function(X: Float_Type) return Float_Type;` (Ada 95 Rationale §3.7.2, mirror
  [rat2_3](https://www.aise.ics.saitama-u.ac.jp/~cheng/ADA/ada95rat/rat2_3.htm)). The profile of such a type is the
  subprogram's parameter profile, modes included (below).
- **(identity)** RM 2012 3.10.2(32/3), for `P'Access`: *"The profile of P shall be subtype conformant with the
  designated profile of S"* ([RM 3.10.2](https://www.adaic.org/resources/add_content/standards/12rm/html/RM-3-10-2.html));
  6.3.1(16.2/3), mode conformance: *"corresponding parameters have identical modes"*; 6.3.1(17): *"Two profiles are
  subtype conformant if they are mode conformant, corresponding subtypes of the profile statically match"*
  ([RM 6.3.1](https://www.adaic.org/resources/add_content/standards/12rm/html/RM-6-3-1.html)). The Ada 95 Rationale
  §6.2: *"Subtype conformance. This is the matching required for access to subprogram types"*
  ([rat2_6](https://www.aise.ics.saitama-u.ac.jp/~cheng/ADA/ada95rat/rat2_6.htm)). So an `in` profile and an `in out`
  one do not conform, and no conversion between them is stated.
- **(call)** No mark at any call: RM 6.4(6), *"explicit_actual_parameter ::= expression | variable_name"*; and 6.4(8/2),
  *"the prefix can be an implicit_dereference of an access-to-subprogram value"*
  ([RM 6.4](https://www.adaic.org/resources/add_content/standards/12rm/html/RM-6-4.html)).
- **(alias)** RM 2012 6.4.1(6.15/3 to 6.16/3): *"If a call C has two or more parameters of mode in out or out that are
  of an elementary type"*, it is legal only if *"there is no other name among the other parameters of mode in out or out
  to C that is known to denote the same object."* The AARM: *"these rules are intended to be conservative."* and
  *"These rules prevent obvious cases of dependence on the order of evaluation of names or expressions."*
  ([AARM 202y 6.4.1](https://ada-rapporteur-group.github.io/ARM/Ada_202Y/AA-6-4-1.html)). The rule is written of *"a
  call C"*; neither page says anything particular of a call through an access value, so whether Ada applies it there
  is **unverified** (the text names no exception). It covers **elementary** types only.
- **(history)** Ada 2012 let functions take `in out` parameters, with new order-dependence rules beside it (AI05-0143
  and AI05-0144): **unverified** as a quotation, the rationale's PDFs answering 404 to the fetcher; the search engine's
  summary of John Barnes's *Rationale for Ada 2012* says so, which is a route and not evidence.
- **verified**: the RM, AARM and Ada 95 Rationale quotations, read 2026-11-26 (ada-auth.org refused the connection;
  the AdaIC and ARG mirrors answered).

## 5. Go: Gin, Echo, net/http (no modes; how a handler reaches the program's state)

- **net/http**: *"The HandlerFunc type is an adapter to allow the use of ordinary functions as HTTP handlers."*, `type
  HandlerFunc func(ResponseWriter, *Request)`, whose `ServeHTTP` *"calls f(w, r)"*
  ([server.go](https://go.dev/src/net/http/server.go)). The function type has no slot for the program's state.
- **Gin**: *"HandlerFunc defines the handler used by gin middleware as return value."*, `type HandlerFunc
  func(*Context)` ([gin.go](https://raw.githubusercontent.com/gin-gonic/gin/master/gin.go)); the per-request store:
  *"Keys is a key/value pair exclusively for the context of each request."*, `Keys map[any]any`; *"Set is used to store
  a new key/value pair exclusively for this context."*; *"MustGet returns the value for the given key if it exists,
  otherwise it panics."* ([context.go](https://raw.githubusercontent.com/gin-gonic/gin/master/context.go)). The store's
  values are `any`: the state's type is erased there.
- **Echo**: v4, `type HandlerFunc func(c Context) error`, *"HandlerFunc defines a function to serve HTTP requests."*
  ([v4.13.4 echo.go](https://raw.githubusercontent.com/labstack/echo/v4.13.4/echo.go)); its `master` branch now reads
  `type HandlerFunc func(c *Context) error` ([master echo.go](https://raw.githubusercontent.com/labstack/echo/master/echo.go)),
  so the brief's `func(c echo.Context) error` is v4's. v4's context: *"Get retrieves data from the context."*, `Get(key
  string) interface{}`; *"Set saves data in the context."* ([v4.13.4 context.go](https://raw.githubusercontent.com/labstack/echo/v4.13.4/context.go)).
  Echo's guide: *"Share data between middleware and handlers with `Get`/`Set`:"* and `u, _ := c.Get("user").(*User)`,
  the type restored by a type assertion ([guide/context](https://echo.labstack.com/guide/context/)). The guide's
  *Extending Context* section (a custom context embedding `echo.Context`) answered 404 at `echo.labstack.com/docs/context`;
  **unverified**.
- **The idiom for shared state is a closure**: Mat Ryer, *"How I write HTTP services in Go after 13 years"* (Grafana
  blog, dated 2024-02-10 on the page, 2024/02/09 in its URL), section *"Maker funcs return the handler"*, `func
  handleSomething(logger *Logger) http.Handler`; *"This pattern gives each handler its own closure environment."*; and
  *"My handlers used to be methods hanging off a server struct, but I no longer do this."*
  ([the post](https://grafana.com/blog/2024/02/09/how-i-write-http-services-in-go-after-13-years/)).
- **verified**: the quotations above, read 2026-11-26; Echo's *Extending Context* **unverified**; that Gin's own docs
  recommend a closure for shared state **unverified** (its custom-middleware page answered 404; Ryer writes of
  `net/http`, not of Gin).

## 6. Nim

- **(type)** *"Parameters can be declared mutable and so allow the proc to modify those arguments, by using the type
  modifier var."*; *"The type of a parameter may be prefixed with the var keyword:"*; *"A procedural type is internally
  a pointer to a procedure."*; *"procedural types are only compatible if they have the same calling convention."*
  ([manual](https://nim-lang.org/docs/manual.html)). The fetcher found no example `proc (x: var int)` and no sentence
  on whether `var` is part of a procedural type's identity: **unverified**.
- **(call)** No mark: `divmod(8, 5, x, y) # modifies x and y`; *"The argument passed to a var parameter has to be an
  l-value."*; *"Var parameters are implemented as hidden pointers."* (manual).
- **(overloading)** *"var T matches better than just T then."* (manual, *Overloading based on 'var T'*).
- **(the word `lent`)** Nim's `lent` is a result annotation: *"a proc's return type can be annotated as lent T."*;
  *"lent T is like var T a hidden pointer. It is proven by the compiler that the pointer does not outlive its origin."*
  ([destructors](https://nim-lang.org/docs/destructors.html)); the section does not say whether `lent` stands on a
  parameter.
- **verified**: the quotations above, read 2026-11-26; `var` in a procedural type's identity **unverified**.

## 7. D

- **(type)** A function pointer type carries `ref`: the spec's example `void f2(ref scope int*); void function(ref
  int*) fp2 = &f2;` ([functions](https://dlang.org/spec/function.html), *Ref Contravariance*).
- **(identity, converts)** *"**Note:** There is no variance between ref and non-ref types, so those entries are omitted
  from the table for simplicity."*; while *"A function pointer/delegate type implicitly converts to a more-loosely
  qualified type."* for attributes such as `pure` (same page).
- **(call)** No mark: the example calls `inc(z);`; *"A ref parameter takes an lvalue argument, so changes to its value
  will operate on the caller's argument."*; `ref` is *"an input/output parameter, passed by reference"*; *"Unlike ref
  parameters, in parameters can bind to both lvalues and rvalues (such as literals)."* (same page).
- **verified**: read 2026-11-26.

## 8. Pascal, Modula-2, Oberon, Modula-3 (Wirth's line and its neighbour)

- **ISO 7185 Pascal**, 6.6.3.6 *Parameter list congruity*: two lists are congruous *"if the formal-parameter-sections
  in corresponding positions match"*, a section matching another when both are value-parameter specifications, item
  a), *"They are both value-parameter-specifications containing the same number of parameters,"*, or both variable ones,
  item b), with the same type; a procedural actual's list must be congruous *"or neither formal-parameter-list shall
  occur."*; 6.6.3.1: *"procedural-parameter-specification = procedure-heading ."*. Its *Language history*: *"The
  syntax used to specify procedural and functional parameters has been changed"* *"to require the use of a procedure
  or function heading, as appropriate."*; *"this change was introduced to overcome a language insecurity."*
  ([ISO 7185, standardpascal.org's copy](https://www.standardpascal.org/iso7185.html)). What the earlier syntax was the
  page does not say; that Jensen and Wirth's Pascal left a procedure parameter's own parameters unspecified is
  **unverified** (the search engine attributes it to Lecarme and Desjardins, *More comments on the programming language
  Pascal*, Acta Informatica 4, 1975, whose page answered with a login redirect).
- **Oberon-2** (H. Mössenböck, N. Wirth, ETH Zürich, October 1993), 6.5 *Procedure types*: *"If a procedure P is
  assigned to a variable of type T, the formal parameter lists"* *"of P and T must match"*; *"P must not be a predeclared
  or type-bound procedure nor may it be local to another procedure."*. Appendix A, *Matching formal parameter lists*:
  *"they have the same number of parameters"*, *"they have either the same function result type or none"*,
  *"parameters at corresponding positions have equal types"*, *"parameters at corresponding positions are both either
  value or variable parameters"*; *Equal types*: *"Ta and Tb are procedure types whose formal parameter lists match."*
  Section 6's type forms are 6.1 basic, 6.2 array, 6.3 record, 6.4 pointer, 6.5 procedure
  ([the report, HTML](https://cseweb.ucsd.edu/classes/fa00/cse131a/oberon2.htm)).
- **Modula-3** (DEC SRC's language definition, release 3.6 HTML): a procedure value *"is a member of the type T if it
  is NIL or its signature is covered by the signature of T"*; covering needs *"They have the same number of parameters,
  and corresponding parameters have the same type and mode."*; *"The parameter names and defaults affect the type of a
  procedure, but not its value."* ([procs](https://softwarepreservation.computerhistory.org/modula3/archive/DEC_SRC/release-3.6-html/m3defn/html/procs.html)).
  Its modes include a **read-only by reference** one: the tutorial's grammar *"<formal> = [ VAR | READONLY | VALUE ]
  <id> ..."*, READONLY *"generally "call by reference," but the procedure may not modify the formal parameter."*
  ([tutorial](https://www.cs.columbia.edu/graphics/modula3/tutorial/www/m3_23.html)); at a call, *"A VAR formal is
  bound to the variable designated by the corresponding actual; that is, it is aliased."*, *"A READONLY formal is
  treated as a VAR formal if the actual is a designator"* *"otherwise it is treated as a VALUE formal."*, the call
  being one whose *"P is a procedure-valued expression"*
  ([calls](https://softwarepreservation.computerhistory.org/modula3/archive/DEC_SRC/release-3.6-html/m3defn/html/calls.html)).
  The report's dates (SRC RR 31, 1988; RR 52, November 1989) are the search engine's, **unverified**.
- **Modula-2**: the procedure type's syntax with `VAR` in its formal type list **unverified** (no fetchable page of
  Wirth's report or PIM4 found; the GNU Modula-2 pages point to ISO 10514-1 and PIM4 without quoting them).
- **(call)** None of the four marks a mode at a call (ISO 7185's and Oberon's actual parameters are expressions or
  variables; no quoted page names a mark); this is the reading of the grammars above, not a sentence of theirs.
- **verified**: ISO 7185, Oberon-2 and Modula-3 quotations, read 2026-11-26; Modula-2 **unverified**.

## 9. Rust's axum; Hylo

- **axum 0.8.9**: `pub struct Router<S = ()>`; *"`Router<S>` means a router that is _missing_ a state of type `S` to be
  able to handle requests."*, *"It does _not_ mean a `Router` that _has_ a state of type `S`."*; `with_state`: *"Provide
  the state for the router."*, *"State passed to this method is global and will be used for all requests this router
  receives."* ([Router](https://docs.rs/axum/latest/axum/struct.Router.html)). The `State` extractor: *"Extractor for
  state."*, handlers taking `State(state): State<AppState>`; *"As state is global within a `Router` you can't directly
  get a mutable reference to the state."*; *"The most basic solution is to use an `Arc<Mutex<_>>`."*; every example
  derives `Clone` on the state, and `FromRef`'s blanket implementation is bound `where T: Clone`
  ([State](https://docs.rs/axum/latest/axum/extract/struct.State.html)). So axum's generic router hands a handler a
  value of the state, not an exclusive reference to it.
- **Hylo**: *"In Hylo, there are four: `let`, `inout`, `sink` and `set`."*; *"arguments to `inout` parameters must be
  mutable and marked with an ampersand (`&`) at the call site:"*, `offset_inout(&v, by: (x: 1, y: 1))`; a function type
  carrying the convention: *"the type of `Float64.infix+=` is `(inout Float64, Float64) -> Void`."*; a closure whose own
  state changes: *"The signature of the closure must be annotated with `inout` because calling it modifies its own
  state"*, *"a call to `counter` must be prefixed by an ampersand to signal mutation."*; and `offset_inout(&v, by: v)`
  is *"error: overlapping `inout` access to `v`;"* *"pass `v.copy()` as the second argument instead."*
  ([Functions and methods](https://docs.hylo-lang.org/language-tour/functions-and-methods)). Whether a lambda **type**
  can be written with `inout` beside other conventions, beyond the operator's type above, the page does not show.
- **verified**: read 2026-11-26.

## 10. The route nobody listed: a language with neither closures nor generic types

Two found, each restoring the state's type at the boundary, and the Erlang family beside them.

- **C**: libevent, `int evhttp_set_cb(struct evhttp *http, const char *path, void(*cb)(struct evhttp_request *, void
  *), void *cb_arg)`, *"Set a callback for a specified URI."*, `cb_arg` *"an additional context argument for the
  callback"* ([http.h](https://libevent.org/doc/http_8h.html)): a route bound to a function pointer and an untyped
  pointer the program owns. What it cost, said by Rusty Russell, *"Typesafe callbacks in C (and gcc)"*, 2010-05-24:
  *"A classic pattern in C is to hand a generic callback function around which takes a "void *priv" pointer"*; *"The
  problem with this pattern is that it breaks type safety completely"*; *"there's no compiler warning that I'm now
  handing my callback something it doesn't expect."*; his answer became CCAN's `typesafe_cb`
  ([the post](https://rusty.ozlabs.org/2010/05/24/typesafe-callbacks-in-c-and-gcc.html)). **The critic's candidate is
  a route of its own, and its cost is on record.**
- **Oberon** (no closures: *"nor may it be local to another procedure"*, § 8; type forms without a generic one, § 8):
  ETH Oberon's gadgets keep the handler **in the object** and call it with the object itself, the message by `VAR`:
  `Handler = PROCEDURE (obj: Objects.Object; VAR M: Objects.ObjMsg);`, set as `F.handle := MyHandler;`, sent as
  `obj.handle(obj, msg)`, the object's own type restored by a checked guard, `WITH F: MyGadget DO`, the message's by
  `IF M IS Display.FrameMsg THEN` ([ETH Oberon tutorial, GadgetsProg](https://en.wikibooks.org/wiki/Oberon/ETH_Oberon/Tutorial/GadgetsProg));
  `MsgProc* = PROCEDURE (VAR M: FrameMsg);` in `Display.Mod`
  ([S3Trio.Display.Mod](https://en.wikibooks.org/wiki/Oberon/ETH_Oberon/2.3.7/S3Trio.Display.Mod)). The type is
  erased to a base record and restored by a guard checked at run time, which needs record extension.
- **Erlang, the state passed and answered by value (§ 7 (f), (g))**: `gen_server`'s `handle_call` returns
  `{reply, Reply :: term(), NewState :: term()}`; *"`State` is the internal state of the `gen_server` process, and
  `NewState` a possibly updated one."*; *"The `gen_server` process continues executing with the possibly updated
  internal state `NewState`."* ([gen_server, OTP 27](https://www.erlang.org/docs/27/apps/stdlib/gen_server.html)).
  Cowboy binds a route to a module **and per-route data**: `%% {HostMatch, list({PathMatch, Handler, InitialState})}`
  ([routing guide source](https://raw.githubusercontent.com/ninenines/cowboy/master/doc/src/guide/routing.asciidoc));
  `init(Req, State) -> {ok, Req, State}.`, the callback *"receives the Req object and the initial state defined in the
  router configuration"*, and *"The last value of the tuple is a state that will be used in every subsequent callbacks
  to this handler."* ([handlers guide source](https://raw.githubusercontent.com/ninenines/cowboy/master/doc/src/guide/handlers.asciidoc)).
  Erlang has closures, so it answers (f) and (g) and not the question's *"neither"*.
- **verified**: read 2026-11-26. Searched and **not found**: a router of a language with neither closures nor generic
  types that hands handlers an **exclusive in-out** state through a stored function value (the search named below).

## 11. The table

| precedent | mode in the function type | mark at a call through a value | two types or one | converts | alias rule |
|---|---|---|---|---|---|
| Rust | none; `&mut T` is a parameter **type** | `&mut x` at a call by name (Book); through a value unverified | two (`fn(&T)`, `fn(&mut T)`) | no coercion listed | one `&mut` excludes every other reference (E0499); invoking a closure is a unique borrow (rustc test) |
| Swift | `(inout T) -> U` (SE-0031, 3.0); `borrowing`, `consuming` (SE-0377, 5.9) | `&` required for in-out (stated of in-out parameters) | not stated as such; `inout` is outside SE-0377's interconvertible set | `borrowing` / unspecified / `consuming` interconvert for copyable types; `inout` not in the list | SE-0176 (4.0): inout is a write access for the call; overlapping accesses conflict |
| C# | `ref`, `out`, `in`, `ref readonly` in a delegate's parameter list | `ref`, `out` required; `in` optional; `ref readonly` warns without one | delegate compatibility needs the same by-reference modifier (§21.4) | C# 12: `in` method to `ref` / `ref readonly` delegate with a warning; `ref` method to `in` an error; function pointers exact | none found on the pages read |
| Ada (95, 2012) | profile modes, `in out` | none at any call | subtype conformance needs identical modes | none stated | 6.4.1: two `in out` / `out` elementary actuals *known to denote the same object* illegal, *"conservative"*; through an access value unverified |
| Nim | `var` is a *type modifier* of the parameter | none | unverified for procedural types | unverified | none found |
| D | `void function(ref int*)` | none | *"no variance between ref and non-ref types"* | attributes loosen; `ref` does not | none found |
| ISO Pascal | procedural parameter written as a full heading | none | congruity: value with value, variable with variable | none | none quoted |
| Oberon-2 | `PROCEDURE (VAR ...)` in a procedure type | none | match: *"both either value or variable parameters"* | none | none quoted |
| Modula-3 | `VAR`, `READONLY`, `VALUE` in a signature | none | covering needs *"the same type and mode"* | none | VAR *"is aliased"*; no inter-argument rule quoted |
| Hylo | `(inout Float64, Float64) -> Void` | `&` for `inout` | unverified | unverified | *"overlapping `inout` access"* refused |
| Go (Gin, Echo, net/http) | no modes; pointer parameters | none | n/a | n/a | n/a |

## 12. Which precedent each route of § 6.2 and § 7 rests on (what the pages show, not what Heroes should do)

- **The proposal's core, a mode in the type and the call through a value writing it**: Swift (`inout` in the type and
  `&` at the call), C# (`ref`/`out` in the delegate and required at the call), Hylo (`(inout ...) -> Void`, `&`).
  Ada, ISO Pascal, Oberon-2, Modula-3 and D carry the mode in the type and mark nothing at any call.
- **Sketch (A), the program keeps the table**, and **(k), a name-keyed map**: Rust's forum advice (2022) for handlers
  over the struct that holds them is a list of `fn(&mut Foo)` pointers; Oberon's gadgets keep the handler in the
  object it is called with (`obj.handle(obj, msg)`). Neither page rules on a callee read out of the place the call lends
  beyond rustc's closure test.
- **Sketch (B), a generic face over the program's table**, and **(C), `Router<S>`**: axum is C's precedent, a router
  generic over its state; it hands a handler a value, *"you can't directly get a mutable reference to the state"*, and
  points to `Arc<Mutex<_>>`. No router found hands a stored handler an exclusive in-out state.
- **(a) a handler record kind**: C#'s delegates are declared, name-equivalent types (§21.2); a near precedent, not the
  same form.
- **(c) closures**: Go's idiom (Ryer), Echo and Gin's closures-or-store; Swift requires a closure capturing an in-out
  parameter to be non-escaping.
- **(e) a variant of routes**: no precedent sought beyond the record (unsearched; named so the absence is not read as a
  finding).
- **(f) state by value, (g) an owner thread**: Erlang's `gen_server`; Cowboy's per-route `InitialState` beside the
  handler (the shape of registry's scope and limiter, § 5.3).
- **(h) `lent` coerced to `@`**: C# 12, the one language found that admits a read-only-by-reference method where a
  read-write delegate is asked, **with a warning and gated on LangVersion**, and refuses the reverse; Swift does not
  include `inout` among its interconvertible conventions; Ada, ISO Pascal, Oberon-2, Modula-3 and D require the modes
  to be identical.
- **(m) `lent` adapted to a by-value type** (panel 217's spec-warden): Swift SE-0377, *borrowing* and *unspecified*
  interconvertible for copyable types, not for noncopyable ones.
- **The C half (question 7)**: libevent's `cb_arg`, Russell's account of its cost.
- **The route nobody listed**: C's `void *` and Oberon's guarded extension are the two found with neither closures nor
  generic types; each erases the state's type at the boundary, C restoring it unchecked, Oberon by a checked guard that
  needs record extension. Cowboy's per-route datum is the third shape, a datum the router stores beside the handler.

## 13. What the history says went wrong

- **A procedure parameter that did not carry its parameter list was an insecurity**: ISO 7185 changed procedural and
  functional parameters *"to require the use of a procedure or function heading"*, *"to overcome a language
  insecurity"*, and its congruity rule pairs value with value and variable with variable. The page does not describe
  the insecurity itself.
- **The mode's place in the spelling was corrected once**: Swift moved `inout` from the label to the type in 3.0
  (SE-0031), *"although the keyword isn't modifying the label but its type."*
- **Labels left the function type in the same release** (SE-0111): *"Removing this feature simplifies the type
  system."* The panel's chosen precedent for identity, a parameter's name in the type, is the thing Swift removed.
- **An unmarked read-only mode was revisited**: C# 7.2's `in`, usable *"without any annotation at the callsite"*,
  needed C# 12's `ref readonly` for APIs wanting *"some indication at the callsite"*; and **a mode in a delegate
  signature is a source-breaking change to alter**: *"changing `ref` to `in` in a delegate signature can be a source
  breaking change"*.
- **Conversions between modes stayed narrow where they exist**: C# 12's arrive with a warning and a LangVersion gate;
  Swift's exclude `inout` and noncopyable types; C#'s function pointers admit none implicitly.
- **The untyped context pointer's cost is on record** (Russell, 2010): *"it breaks type safety completely"*.
- **Ada's alias rule is conservative by its own words** and covers elementary types only; its extension in Ada 2012
  beside `in out` for functions is **unverified** here.
- Searched and **not found**: a language with by-reference parameter modes and procedure values that refused a
  procedure with such a parameter as a value, or that removed modes from its procedure types after carrying them.

## Verdict, argument, condition, search

- **verdict**: **approve** (advisory).
- **precedents**: §§ 1 to 10 above; the table is § 11. Verified: Rust, Swift (SE-0031, SE-0111, SE-0176, SE-0377), C#
  (standard §21, C# 12 proposal), Ada (RM 3.10.2, 6.3.1, 6.4, 6.4.1; Ada 95 Rationale), D, ISO 7185 Pascal, Oberon-2,
  Modula-3, Hylo, axum, Gin, Echo v4 and master, net/http, Ryer's post, libevent, Russell's post, ETH Oberon's gadgets,
  `gen_server`, Cowboy. Unverified: Modula-2's procedure types; Nim's `var` in a procedural type's identity; Ada's
  6.4.1 through an access value and Ada 2012's history; Swift's rule for a callee read out of an in-out place; the
  pre-ISO Pascal syntax; Echo's *Extending Context*; Modula-3's report dates.
- **argument** (112 words): Every language found with by-reference parameter modes and procedure values carries the
  mode in the procedure type: ISO Pascal, Oberon-2, Modula-3 (a read-only mode included), Ada, D, Swift, C#, Hylo. None
  found refuses such a procedure as a value, as Heroes does today: the present rule is the departure. Swift, C# and
  Hylo mark the in-out argument at the call; Wirth's line and Ada mark nothing. Where precedent narrows: modes must
  match exactly nearly everywhere; only C# 12 lets a read-only method stand where a read-write delegate is asked, with
  a warning; changing a published delegate's mode can break callers; axum, the generic router, hands handlers a value,
  never an exclusive reference.
- **condition**: my reading turns to **object** on any of: a language with by-reference modes and procedure values
  that refused such procedures as values and kept the refusal, with its reason published; a documented class of bugs
  traced to a call-site mark (`&`, `ref`) through a function value in Swift, C# or Hylo; a router that handed stored
  handlers an exclusive in-out state and withdrew it; or a language that carried modes in its function types and
  removed them, with its reason.
- **search** (2026-11-26; the session's web search and page fetch; CLAUDE.md § RUN IT, CL-018):
  - **Languages and documents swept**: the Rust Reference (function pointer types, subtyping, coercions, call
    expressions), the Rust Book 4.2, rustc's UI test `borrowck-call-is-borrow-issue-12224.rs`, users.rust-lang.org
    thread 75385; The Swift Programming Language (Types, Declarations, from `swiftlang/swift-book`), Swift Evolution
    SE-0031, SE-0111, SE-0176, SE-0377; the C# standard §21 (Delegates), Microsoft Learn's *Method parameters*, the C#
    12 *ref readonly parameters* proposal (`dotnet/csharplang`); Ada RM 2012 3.10.2, 6.3.1, 6.4, 6.4.1 (AdaIC mirror),
    the ARG's AARM 202y 6.4.1, the Ada 95 Rationale §3.7.2 and §6.2 (Saitama mirror); `net/http`'s `server.go`, Gin's
    `gin.go` and `context.go`, Echo's `echo.go` and `context.go` (v4.13.4 and master) and its context guide, Mat Ryer's
    post; the Nim manual and its destructors page; the D spec's *Functions*; ISO 7185 (standardpascal.org); the
    Oberon-2 report (UCSD copy); Modula-3's DEC SRC definition (procs, calls) and Columbia's tutorial; Hylo's language
    tour; axum 0.8.9's `Router` and `State`; libevent's `http.h`; Rusty Russell's post; ETH Oberon's gadgets tutorial
    and `S3Trio.Display.Mod`; Erlang/OTP 27's `gen_server`; Cowboy's routing and handlers guides.
  - **Queries** (web search): Rust closure in a struct field called with `&mut self`; C# delegate compatibility and
    parameter modifiers; Ada 2012 in out for functions and AI05-0144; Ada 95 access-to-subprogram rationale; Echo's
    *Extending Context*; Nim procedural types with `var`; Welsh, Sneeringer and Hoare's *Ambiguities and insecurities
    in Pascal*; Habermann's *Critical comments*; Wirth's *Recollections* on procedure parameters; Project Oberon's
    `Handler` and `FrameMsg`; Modula-2 procedure types with `VAR`; Modula-3 covering and READONLY; ISO 7185 against
    Jensen and Wirth; Cowboy's `{PathMatch, Handler, InitialState}`; libevent's `evhttp_set_cb`; the cost of `void *`
    callback data.
  - **Could not be read**: ada-auth.org (connection refused), AdaCore's Ada 2012 Rationale PDFs (404), Springer's
    Acta Informatica pages (login redirect), the Lazarus wiki (certificate mismatch), Echo's `docs/context` and
    `echox` sources (404), Gin's custom-middleware page (404), Wirth's Oberon-07 report PDF (compressed, unreadable to
    the fetcher), the Purdue Modula-3 pages (404).
  - **Not searched**: a precedent for § 7 (e), a variant of routes matched exhaustively; Vapor, Hummingbird and other
    Swift routers; Zig, Odin, Cyclone and Idris for modes in function types. A sentence above about something not
    found is about these sources and queries alone.
  - **The project's book, *Heroes of code***, was used as no source: no line above rests on it.
