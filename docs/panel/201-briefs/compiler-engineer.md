# Panel 201, compiler-engineer's brief

Read `00-shared.md` beside this file first. Per question a verdict with its
cost in files and lines (`layout`'s unit), and **the route you recommend
built in your copy and run on its cases** before you report (a route that
does not build is not adopted). `./heroes build selfhost/main.hero -o heroes`
rebuilds your copy's compiler.

- **Q1 (488)**: where `cannot_infer` is decided for a function value
  (`selfhost/check/function_value.hero` and what calls it); build the
  inference the spec's sentence reads as (a generic function value's type
  parameters unified with the parameter type the call asks for, including
  through another argument such as `x: 20`); the shapes: `app(f: ident, x:
  20)`, `ns.map(ident)`, `ns.fold(19, keep)`, a generic value stored in a
  typed record field, returned under a declared type, and one that still says
  neither (must stay refused); the C it emits for each (a generic function
  value must be monomorphised at the type the call fixes); `check` on the
  compiler and the census of `check` over the tracked `.hero` files, before
  and after.
- **Q3 (520)**: extend panel 199's R4 (`selfhost/check/self_call.hero`,
  `may_end.hero`, `endless.hero` as landed) to a cycle of named functions (a
  strongly connected component of the call graph whose every function's every
  path reaches a call inside the component before it can end) and to a
  self-call through a function value whose callee the checker knows (`f =
  go`, then `f(n)` in `go`); the correct programs at risk (a cycle with a way
  out in one function); its cost on `check` of the compiler; the message.
- **Q2 (467)**: what `check` does with a `main` in a module that is used
  (`use m`) and never compiled alone: accepted, warned, refused? Run it.

Report per question: verdict, design.md section, cost, cases, prediction,
what you could not run.
