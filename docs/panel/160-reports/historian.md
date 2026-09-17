# Panel 160 — historian report

Written to disk by the coordinator, verbatim from the seat's final message,
because this seat has no write tool. The seat's text follows unchanged from its
first heading.

---

# Panel 160 — historian's report

**Seat:** historian (precedent, advisory, no veto). **Fetched:** all sources on 2026-09-17. **Tooling caveat:** every fetch went through a summarising fetch tool; where I mark a string VERIFIED it is the string the tool returned as a quotation from the page named. The one place I saw a whole file reproduced as code (clippy's `option_option.rs`) I treat as exact. Every repository fact below comes from `docs/panel/160-briefs/00-shared.md` and is **UNRUN by me**: I ran no command in the tree. *Heroes of code* was not consulted; nothing here rests on it.

## verdict

**approve (advisory)** — for a resolution that refuses the predicate on a nested fallible while keeping `match` as the way to name both levels, **provided the sitting records it as a deliberate departure with no shipped precedent**. I **object (advisory)** to option B being justified by clippy: the lint it would cite is opt-in, fires on the type, and its own help text protects the three-case use B would forbid.

## precedents

### 1. Clippy `option_option` (rust-clippy) — VERIFIED

- Sources: `https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/types/mod.rs` and `.../clippy_lints/src/types/option_option.rs` (both fetched 2026-09-17).
- Group: **pedantic** (opt-in; not warn-by-default). VERIFIED from `mod.rs`.
- Doc comment, as returned: *"Checks for usage of `Option<Option<_>>` in function signatures and type definitions"*; *"`Option<_>` represents an optional value. `Option<Option<_>>` represents an optional value which itself wraps an optional. This is logically the same thing as an optional value but has an unneeded extra level of wrapping."* Example: `fn get_data() -> Option<Option<u32>>`.
- Implementation, exact: `pub(super) fn check(cx: &LateContext<'_>, hir_ty: &hir::Ty<'_>, qpath: &QPath<'_>, def_id: DefId) -> bool` — it takes a **`hir::Ty`**, i.e. a type node, never an expression. Message: `"use of `Option<Option<T>>`"`. Help: `"consider using `{inner_opt}`, or a custom enum if you need to distinguish all 3 cases"`.
- What it means for the ballot: this is a precedent for **B's shape only** (the declaration), and a weak one: the Rust project did not make it default, and the help text itself says the three-case need is legitimate and should be spelled as a custom enum, not refused.

### 2. Swift, the read side of a double optional — partly VERIFIED

- `https://www.avanderlee.com/swift/optionals-in-swift-explained-5-things-you-should-know/` (secondary, fetched 2026-09-17): `let nameAndAges: [String:Int?] = ["Antoine van der Lee": 28]`; `print(antoinesAge)` prints `Optional(Optional(28))`, `antoinesAge!` prints `Optional(28)`, `antoinesAge!!` prints `28`. VERIFIED. That `if let x = dict["k"]` binds `x` as `Int?` is the same one-level rule; I found **no primary sentence stating it** — UNVERIFIED as a quotation, an inference from the `!` example.
- `https://forums.swift.org/t/double-optionals-let-and/51184` (primary; first post 2021-08-13T10:39Z by JetForMe, 7 posts; dates from `/t/51184.json`, fetched 2026-09-17). VERIFIED: on `let hashtags: [String?]`, `foo.hashtags.first ?? "<none>"` printed `hashtags: Optional("#foo")`. The OP: *"The compiler is even fooled by it; if you write `print("\(foo.hashtags.first ?? "<none>")`, the compiler warns the expression produces an optional and offers a fix-it to add `?? ""` to the end. But if you do that, it doesn't fix the problem!"* Reply (Alexis Schultz, as returned): *"If you are interested in the inner optionality this make sense. Defaulting to unwrapping both optional would make some code impossible to write."* sveinhal (2021-08-13T14:54Z) proposed: *"Maybe the compiler should generate a warning when optional promotion and nested optionals are combined?"* JetForMe (2021-08-13T20:48Z): *"I'd prefer an error, or at least a warning that my non-optional RHS was wrapped."*
- **Documented warning for reading a `T??` one level:** none found in either source. Whether Swift later added the warning proposed in 2021 is **UNVERIFIED** (not searched beyond these pages).
- `https://developer.apple.com/forums/thread/55025` (Aug 2016, write side; fetched 2026-09-17). VERIFIED: an Apple DTS Engineer wrote *"Honestly, I think folks who are new to Swift should steer clear of dictionaries with optional values. Such a setup is always going to be confusing."* No diagnostic mentioned. That is advice against the DECLARATION, given as prose, never shipped as a rule.
- False lead, for the record: `https://github.com/swiftlang/swift/pull/85883` (merged 2025-12-15) is an unrelated `as!`-cast warning suppression. Not about nested optionals.

### 3. Kotlin, `T??` is `T?` by construction — VERIFIED

- `https://kotlinlang.org/spec/type-system.html`, § Nullable types (fetched 2026-09-17): *"Redundant nullability specifiers are ignored: T?? ≡ T?."* VERIFIED. A **reason** in the spec or design notes: **not found** — UNVERIFIED (search vocabulary: "Breslav", "design rationale", "nested nullability", "T??").
- The shipped consequence, `https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.collections/-map/get.html` (fetched 2026-09-17), `operator fun get(key: K): V?`: *"Returns the value corresponding to the given key, or `null` if such a key is not present in the map. Note that for maps supporting `null` values, the returned `null` value associated with the key is indistinguishable from the missing key, so containsKey should be used to check if the map actually contains the key."* VERIFIED.
- What it means: Kotlin is panel 158's vetoed flatten, shipped. Its cost is written in its own stdlib doc: the outer question loses its type and has to be asked through a **distinct spelling** (`containsKey`) and a **documented sentence**. That is options C and D arriving together as the price of B-by-construction.

### 4. Rust, `.is_some()` on `Option<Option<T>>` — negative, VERIFIED as far as the enumeration reaches

- `https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/declared_lints.rs` (fetched 2026-09-17): the fetch tool counted **1,174** lint entries, **15** with `OPTION` in the name (`OPTION_OPTION`, `OPTION_MAP_OR_NONE`, `OPTION_AS_REF_CLONED`, `OPTION_AS_REF_DEREF`, `OPTION_FILTER_MAP`, `OPTION_ZIP_NONE`, `OPTION_ENV_UNWRAP`, `OPTION_IF_LET_ELSE`, `REF_OPTION`, `REF_OPTION_REF`, `MANUAL_OPTION_AS_SLICE`, `MANUAL_OPTION_ZIP`, `UNNECESSARY_OPTION_AS_DEREF`, `UNNECESSARY_OPTION_MAP_OR_ELSE`, `UNNECESSARY_OPTION_TAKE`). Counts are the tool's, not mine.
- `.../clippy_lints/src/methods/mod.rs` (fetched 2026-09-17): **zero** occurrences of `Option<Option`, "nested option", "double option"; `is_some` appears only in plain-`Option` lints.
- Plainly: after a decade of `Option<Option<T>>` being legal, **no clippy lint fires on a predicate applied to a nested option**. The only nested-option lint is the type lint in item 1. A GitHub-issue search for a rejected proposal turned up nothing (UNVERIFIED either way; search engine vocabulary only). This is the precedent **against E** the brief asked the sitting to hear — with the caveat that Rust has no thesis that a plausible mistake must be a compile error, so its silence is a choice about Rust, not a finding about Heroes.

### 5. Haskell, `Maybe (Maybe a)` and `isJust` — negative, VERIFIED as far as the enumeration reaches

- `https://raw.githubusercontent.com/ndmitchell/hlint/master/hints.md` (fetched 2026-09-17). VERIFIED: hlint's only nested-Maybe hints recognise a hand-written FLATTEN and suggest the library one — *Use join* (suggestion): `case m of Nothing -> Nothing; Just x -> x` ⇒ `Control.Monad.join m`; `maybe Nothing id` ⇒ `join`. Every `isJust`/`isNothing` hint (`not (isNothing x)` ⇒ `isJust x`, `x == Nothing` ⇒ `isNothing x`, `isJust (f <$> x)` ⇒ `isJust x`, …) is about a plain `Maybe`. None fires on `isJust` applied to a `Maybe (Maybe a)`.
- `https://downloads.haskell.org/ghc/latest/docs/users_guide/using-warnings.html` (GHC **9.14.1**, fetched 2026-09-17): no `-W` flag concerns nested `Maybe` or `isJust` on one (searched "Maybe", "isJust", "nested", "redundant"; the redundancy flags are constraints, wildcards, bangs, strictness).

### 6. My choice: Go's comma-ok map read, the outer question with its own spelling — VERIFIED

- `https://go.dev/ref/spec`, *Language version go1.27 (May 26, 2026)*, § Index expressions (fetched 2026-09-17). VERIFIED verbatim: *"An index expression on a map a of type map[K]V used in an assignment statement or initialization of the special form `v, ok = a[x]` / `v, ok := a[x]` / `var v, ok = a[x]` / `var v, ok interface{} = a[x]` yields an additional untyped boolean value. The value of ok is true if the key x is present in the map, and false otherwise. If the key is not present, a[x] yields the zero value for the map's element type."*
- What it means: Go gives the question *was the key there* a spelling that cannot be confused with the value — the shape of **option C**. But note the second half: the one-value form `a[x]` still compiles and silently hands back the zero value. Go added the spelling; it refused nothing. How long the form has existed (Go 1.0, 2012, from memory) is **UNVERIFIED** here.

## which option class the evidence supports

- **A and E (refuse the predicate): no precedent found.** Not one shipped compiler, default lint or opt-in lint in items 1, 4, 5 refuses or warns on a one-level predicate over a nested optional; Swift users asked for one in 2021 (item 2) and, as far as I found, did not get it. If the sitting adopts A or E it is, on this evidence, the first — which the mandate allows, as a **deliberate** departure. The argument the other languages give for NOT refusing (Swift reply: *"would make some code impossible to write"*; clippy help: *"if you need to distinguish all 3 cases"*) is answered in Heroes by `match` naming both levels (brief, UNRUN by me).
- **B (refuse the declaration): one precedent, and it argues against a hard error.** Clippy made it pedantic, on the type, with a help text preserving the three-case use. Kotlin did B by construction and paid with `containsKey` plus a stdlib sentence. Apple's advice against the declaration stayed advice.
- **C (distinct spelling for the outer question): two shipped precedents**, Go's `v, ok` and Kotlin's `containsKey`, both as ADDITIONS beside a form that still compiles silently. Neither is a refusal.
- **D (document only): one precedent**, Kotlin's `Map.get` note — and it exists because Kotlin had already lost the type distinction. A language that keeps `T??` in the type has less need of D than Kotlin did.

## argument

No shipped compiler or lint I could find refuses a one-level predicate on a nested optional: clippy's 1,174 lints (tool's count), GHC 9.14.1's warnings and hlint's hints all leave `is_some`/`isJust` on `Option<Option<T>>` alone, and Swift users asked for exactly this warning in 2021 without, as far as I found, getting it. So A and E are departures, to be made deliberately and written down as such. B's only precedent, clippy `option_option`, is opt-in pedantic, fires on the TYPE, and its own help text protects the three-case use. Kotlin flattened by construction and then had to document `containsKey`; Go named the outer question `v, ok`. Precedent's shape: keep both levels nameable, give the outer question a spelling, write the sentence.

## prediction (falsifiable)

If the sitting adopts E (or A) and nothing else, the Go and Kotlin stories say the next request will be for **C**: a refused `.is_err()` on `m[k]` leaves a program only `match` to ask a yes/no *was the key there*, and both languages that kept that question askable gave it a spelling of its own. **Prediction:** before the next `m-*` tag, an item asking for a distinct outer-level spelling on a map read appears in `docs/work/DECIDE.md` or `docs/work/DEFECTS.md`. Falsified if none does. Second, weaker prediction: the sitting's R3 measurement of `selfhost/` finds **zero** reader applications to a nested fallible (the brief counts `{K: V?}` in two files, one a comment — UNRUN by me); falsified if it finds one, in which case the compiler itself will want C before it wants A.

## condition

My reading changes if any seat produces: (1) a shipped compiler or default-on lint that refuses or warns on `is_some`/`isJust`/`if let`-style peeling of a nested optional — that gives A/E a precedent and drops "first of its kind"; (2) evidence that Swift added the warning proposed in thread 51184 after 2021 — that makes E "Swift's road, taken late"; (3) a rust-clippy or GHC issue where such a lint was proposed and **rejected with reasons** — that is a discoverable failure and a stronger argument against E than mere silence; (4) a Kotlin design note giving the confusion, rather than JVM interop or erasure, as the reason for `T?? ≡ T?` — that strengthens B.

## sources

- https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/types/mod.rs
- https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/types/option_option.rs
- https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/declared_lints.rs
- https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/methods/mod.rs
- https://www.avanderlee.com/swift/optionals-in-swift-explained-5-things-you-should-know/
- https://forums.swift.org/t/double-optionals-let-and/51184
- https://developer.apple.com/forums/thread/55025
- https://github.com/swiftlang/swift/pull/85883 (false lead)
- https://kotlinlang.org/spec/type-system.html
- https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.collections/-map/get.html
- https://raw.githubusercontent.com/ndmitchell/hlint/master/hints.md
- https://downloads.haskell.org/ghc/latest/docs/users_guide/using-warnings.html
- https://go.dev/ref/spec
