# Panel 158 — historian report

Written to disk by the coordinator, verbatim from the seat's final message,
because this seat has no write tool. **All web sources checked 2026-09-16.** The
seat ran six retrievals and stopped, per brief, and states that **every
repository fact in this report is taken from the shared brief and is UNRUN by
it.**

## verdict

**approve** (advisory, no veto) — approve keeping the nesting, in the sense that
the strongest precedent available refuses exactly the flatten this sitting
already vetoed. I do not recommend which of options 1/3/4 to take; that is not
my seat.

## 1. Swift SE-0230, "Flatten nested optionals resulting from `try?`" — VERIFIED

Source:
https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0230-flatten-optional-try.md
(fetched 2026-09-16). Author **BJ Homer**, review manager **John McCall**, status
**Implemented (Swift 5.0)**.

Verbatim, motivation:

> "Swift's `try?` statement currently makes it easy to introduce a nested
> optional. Nested optionals are difficult for users to reason about, and Swift
> tries to avoid producing them in other common cases."

Verbatim, the shape that produced it:

```swift
let q = try? harbor.boat()           // q is of type 'Boat?'
let r = try? harbor.optionalBoat()   // r is of type 'Boat??'
```

**(a) Swift flattened the OPERATOR, not the container.** SE-0230 changed `try?`
only. Verbatim from Alternatives considered:

> "**Do nothing.** It is possible to write correct code under the current model.
> We are not proposing to eliminate nested Optionals from the language entirely,
> so we could just expect users to figure them out."

So Swift's precedent is: the *sugar* stops manufacturing the nesting; the *type*
stays legal and writable. Heroes repair 2 (flatten `m[k]`) is the container
flatten, which Swift did **not** do. Heroes repair 3 (make `T??` writable) is
what Swift has always had.

**(b) Swift shipped the collapse only after COUNTING that nobody used it.**
Verbatim from Source compatibility:

> "There are **613** total instances of `try?` in the compatibility suite."
> "There are **4** instances of `try? ... as?`. All four of them wrap the `try?`
> in parentheses to get the flattening behavior of `as?`, and are
> source-compatible with this change."
> "There are **12** cases of `try? foo?.bar()` across 3 projects. **10** of those
> assign it to `_ = try? foo?.bar()`, so the resulting type does not matter."
> "There are **6** instances of `try? somethingReturningOptional()`. They all
> flatten it manually using `flatMap { $0 }`."
> "As far as I can tell, there are **zero** cases in the entire suite where a
> double-optional is actually used to distinguish between the error case and the
> nil-as-a-value case."
> "As far as I can tell, there are **zero** cases of source incompatibility found
> in the compatibility suite."

**This is the sentence the sitting should read twice.** Swift's veto-equivalent —
"flattening destroys the error/nil distinction" — was overcome not by argument but
by an enumeration over a corpus finding zero users of the distinction. That is
CLAUDE.md § RUN IT's *the list is a measurement too* performed by another language
ten years ago. It is also the reason Swift's flatten is *not* precedent for
flattening `m[k]`: nobody measured a corpus for the container case, and Swift
never flattened it.

**(c) The source-breaking bar was named explicitly.** Verbatim:

> "The bar for including source-breaking changes in Swift 5 is high, but I
> believe it passes the bar."

with three stated criteria: the syntax "must be shown to actively cause problems
for users", the replacement "must be clearly better", and "there must be a
reasonably automated migration path". Criterion 3 was met: "Automated migration
is implemented for the double `if/guard let` and `case let value??:` patterns."

That last clause carries a fact relevant to R3: **`case let value??:` is legal
Swift.** Swift can always *write* the type and pattern it *prints*. The Heroes
condition — prints `i64??`, refuses to parse `i64??` — has no Swift analogue.

**UNVERIFIED:** no search was made for a later proposal reversing or amending
SE-0230.

## 2. Swift's `Dictionary` subscript over an Optional `Value` — the same shape, KEPT

Source: https://forums.swift.org/t/dictionary-with-optional-values/2675, thread
dated **2016-05-18** (fetched 2026-09-16).

Verbatim, Jeremy Pereira:

> "The type of a dictionary subscript is Optional<V> where V is the value type.
> If V is itself Optional<T> the type of the subscript is Optional<Optional<T>>…
> When you have a nested optional type combined with the implicit conversion, the
> meaning of nil becomes ambiguous."

Verbatim, Jens Alfke:

> "What's the difference between 'x has no value' and 'x has a value of nil'? I
> guess it's that when you iterate the keys you see x."

**What happened to them.** The READ side kept the nesting — `[String: Int?]`
subscript yields `Int??`, absent key is the outer `.none`, stored-nil is
`.some(.none)`. The WRITE side collapsed, and *that* is where it hurt:
`dict["key"] = nil` deletes the key rather than storing a nil value. The thread is
a bug report about exactly that collapse.

This is the precedent asked for under item 2: a shipped collapse of "absent"
against "present but empty". It was shipped, it generated a thread of confusion,
and **Swift's response was to keep the read-side nesting rather than extend the
collapse.**

**UNVERIFIED:** that `.some(nil)` and `updateValue(_:forKey:)` are the sanctioned
ways to store a nil value today, and that the read-side double optional is still
present in current Swift.

## 3. Rust: `{integer}` and `{float}` — a type the compiler prints and the grammar cannot write

Source: https://github.com/rust-lang/rust/pull/35080, author **sophiajt (Jonathan
Turner)**, merged **2016-07-30**. Verbatim:

> "This PR renames `_` to `{integer}` or `{float}` for unknown numeric types, to
> help people parse error messages that have numeric types that haven't been
> nailed down."

The review debated the *bracketing* precisely because the printed form must not
read as a type: `@nrc` suggested `<float>`, `@eddyb` objected — verbatim — "Small
problem is with generics, e.g. `Option<<integer>>`" — and the author preferred
braces as clearer in `Thing<{numeric}, {numeric}>`, adding "Not perfect, but we
don't have a lot to work with."

Prior art within Rust: https://github.com/rust-lang/rust/pull/18264, merged
**2014-10-31**, which had moved rustc *to* printing `_`. Verbatim rationale:
"leaving out the specific number makes the messages slightly less terrifying."

**The precedent, stated carefully:** a production compiler deliberately prints, in
diagnostics, a type-shaped token the surface grammar rejects, and treats choosing
*how it looks unwritable* as the design question. **CAVEAT:** the claim that this
naming deliberately uses syntax that cannot appear in valid Rust came back as the
fetching model's characterisation, not as a verbatim quote. What is verbatim is
the bracketing exchange. Mark it UNVERIFIED pending
https://doc.rust-lang.org/reference/types.html.

**Difference from Heroes the sitting must not paper over.** `{integer}` denotes a
type that has *no* spelling because it is **not yet decided** — the diagnostic is
honest and the fix is to write a real type. Heroes' `i64??` denotes a type that
*is* decided, that the checker fully knows, and whose spelling the parser refuses.
Rust's precedent legitimises *printing an unspellable type*; it does not
legitimise *refusing to parse a type you fully print*. On the evidence available,
**no precedent was found for the second**.

## 4. Rust `Option<Option<T>>`, Scala `Option.flatten`, Haskell `join` — UNVERIFIED

The brief asserts these; the seat did not search them and declined to launder an
assertion into a citation. What would settle each:
`https://doc.rust-lang.org/std/option/enum.Option.html#method.flatten`;
`scala.Option#flatten` in the 2.13/3.x scaladoc; `Control.Monad.join`.

## 5. An operator that peels exactly one level — UNVERIFIED / THIN

Budget ran out before reaching it. Candidates not verified: Rust `?`, Zig `try`
over `!T`, Go's absence of any such operator. Per CL-018 this is a statement about
the searcher's vocabulary, not about the world.

## argument

Swift is the near-exact precedent and it cuts against flattening the container.
SE-0230 flattened the *operator* `try?` and explicitly declined to eliminate
nested optionals; the `Dictionary` subscript still yields `Value??` and keeps
absent distinct from present-and-empty. Crucially, Swift bought its collapse with
an enumeration — 613 sites, "zero cases … distinguish between the error case and
the nil-as-a-value case" — not an argument. This sitting's vetoed repair 2 has no
such count. Rust's `{integer}` shows printing an unspellable type is deliberate
and defensible; no precedent was found for refusing to *parse* a type the compiler
fully *prints*. That asymmetry, not the nesting, is the departure from precedent.

## prediction

If repair 4 (leave it, qualify the spec) ships, the Heroes analogue of the 2016
Swift Forums thread arrives at the **write** side, not the read side:
`m["a"] @ fail(...)` versus a key never set, exactly where Swift's
`dict["key"] = nil` collapsed. The shared brief's measurements cover reading; no
measurement of the assignment form was seen. **UNRUN** and worth one command.

## condition

1. **A corpus count.** SE-0230's precedent is usable only if the same enumeration
   is performed here. If every Heroes program in `tests/golden/**` and
   `examples/**` that reaches a `V?` through a container is counted and **zero**
   distinguish absent from failed, the flatten veto rests on a hazard nobody uses.
   The seat would still not recommend flattening — that veto is the soundness
   seat's — but the precedent would stop supporting it.
2. **A counter-precedent for R3**: a shipped language that prints a type in a
   diagnostic and rejects that exact spelling in source, with a written rationale.
   OCaml's weak type variables (`'_weak1`) are where to look next; not looked at.
3. **A reversal of SE-0230**, which would invert the first precedent. Not searched.

## sources

- https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0230-flatten-optional-try.md
- https://forums.swift.org/t/dictionary-with-optional-values/2675
- https://github.com/rust-lang/rust/pull/35080
- https://github.com/rust-lang/rust/pull/18264
- https://developer.apple.com/forums/thread/55025 — surfaced in search, **not
  fetched**, listed so a later seat can follow it
