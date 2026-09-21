# Panel 172, historian's report

Written to this path verbatim by the coordinator: this seat has no write tool,
so its report reaches the record the way panel 145 settled (`/panel` § 2).

Read date for every URL below: 2026-09-21. `verified` means I opened the page
(or its search snippet where stated) during this sitting; `unverified` means I
could not, and the claim is a lead, not evidence.

## verdict

**approve (advisory)**: give `consumes` the freeing meaning on a `cstr`/`ptr`
parameter. On the DEFAULT the precedents do not decide: route A reproduces the
universal shipping default and its one recorded bug class; route B has no
shipping instance, and the nearest thing to it shipped as opt-in warnings.

## precedents, the eight items in order

### 1. GObject-Introspection transfer annotations

- **Defaults, verified.** "In parameters: (transfer none). Out/inout
  parameters: (transfer full) (or (transfer none) if caller allocates). Return
  values: (transfer full)." Meanings: none = "the recipient does not own the
  value"; container = "the recipient owns the container, but not the elements";
  full = "the recipient owns the entire value".
  https://gi.readthedocs.io/en/latest/annotations/giannotations.html
- **The scanner warns on a missing transfer for a RETURN value**, verified:
  `Warning: Test: test_get_object: return value: Missing (transfer) annotation`,
  from `tests/warn/return-gobject.h`.
  https://github.com/GNOME/gobject-introspection/blob/main/tests/warn/return-gobject.h.
  I found no parameter-side equivalent in that test directory; that is a
  question, not a premise (searched: the `tests/warn` listing surfaced by web
  search, not the directory itself).
- **`--warn-error`: "Make warnings be fatal errors."** verified,
  https://manpages.debian.org/testing/gobject-introspection/g-ir-scanner.1.en.html.
  So a missing transfer on a return can refuse a build; a silent in-parameter
  cannot, because it has a default and raises no warning.
- **The recorded cost of the in-parameter default, verified: GLib issue #1373,
  "Incorrect transfer annotation for g_binding_unbind"** (Tomasz Miąsko). The
  `binding` PARAMETER carried the default `(transfer none)`; the function
  actually releases the reference it is handed; bindings then released it again,
  producing "numerous critical warnings" (Python test case in the issue); fixed
  by writing `(transfer full)` on the parameter.
  https://gitlab.gnome.org/GNOME/glib/-/issues/1373. This is defect 070's shape
  exactly: a parameter that frees, under a default that says it reads. The GLib
  2.58.0 NEWS mention of #1373 was seen only in a search snippet
  (https://abi-laboratory.pro/index.php?l=glib&v=2.58.0&view=changelog),
  unverified.
- **An older alias existed**: the archived wiki lists `floating` as "an alias
  for none, can be used for floating objects", verified,
  https://wiki.gnome.org/Projects/GObjectIntrospection/Annotations/Old.

### 2. Clang's ownership attributes, and the 2026 noescape work

- **Born 2010-07-27**, verified: Andrew McGregor, cfe-dev, "Ownership attribute
  for malloc checking, revised patch", with the test declarations
  `ownership_returns(malloc)`, `ownership_takes(malloc, 1)` on `my_free`,
  `ownership_holds(malloc, 1)` on `my_hold`; written for the malloc checker,
  which "has already found the root cause of a number of real issues".
  http://lists.llvm.org/pipermail/cfe-dev/2010-July/009985.html
- **Two words for the callee side from day one**, verified: `ownership_takes`
  "mark functions that deallocate memory"; `ownership_holds` "mark functions
  that take ownership of memory and will deallocate it at some unspecified point
  in the future".
  https://clang.llvm.org/docs/analyzer/user-docs/Annotations.html. That is route
  B's KEEPS-versus-FREES split, shipped in 2010 by a tool that, like Heroes'
  runtime, models the BLOCK rather than a reference.
- **Analyzer only, and opt-in even there**, verified: documented only on
  2025-01-07 (PR #121759), and the checker reads them only with
  `-analyzer-config unix.DynamicMemoryModeling:Optimistic=true`.
  https://github.com/llvm/llvm-project/pull/121759. I found no statement that
  the compiler proper refuses any call on them; the attributes live in the
  analyzer's documentation, not the compiler's. A 2026 fix for a crash when one
  function carries both `ownership_returns` and `ownership_takes` (PR #183583)
  was seen by title only, unverified.
- **The 2026 noescape work**: I did not find an LLVM Discourse RFC. What exists
  is an inference pipeline in devincoughlin/llvm-project (issue #2 dated
  2026-09-20) that classifies "every use of every parameter alias as benign,
  flows-to-a-callee-parameter, or sink (unrecognized uses are sinks)" and is
  silent on `free`. https://github.com/devincoughlin/llvm-project/issues/2,
  verified. Swift's safe-interop page, which is what consumes `noescape`,
  "contains no discussion of ownership semantics or memory deallocation",
  verified, https://www.swift.org/documentation/cxx-interop/safe-interop/. So
  `noescape` does not touch freeing in either document I read. Correction for
  the brief: panel 171's shared brief, which I read at
  `docs/panel/171-briefs/00-shared.md`, names Clang's `noescape` as an existing
  mark and mentions no 2026 RFC.

### 3. Microsoft SAL

- **Definitions**, verified via the SDK header mirror's search snippet (header
  not opened): `_Frees_ptr_` = `_Pre_notnull_ _Post_ptr_invalid_
  __drv_freesMem(Mem)`, `_Frees_ptr_opt_` = `_Pre_maybenull_
  _Post_ptr_invalid_ __drv_freesMem(Mem)`.
  https://github.com/tpn/winsdk-10/blob/master/Include/10.0.16299.0/shared/specstrings.h.
  In shipping signatures: `HeapFree`'s `lpMem` is `_Frees_ptr_`,
  `CoTaskMemFree`'s is `_Frees_ptr_opt_`, verified via search,
  https://learn.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapfree.
- **Which tool**: "Visual Studio code analysis for C++ uses SAL annotations to
  modify its analysis of functions", verified,
  https://learn.microsoft.com/en-us/cpp/code-quality/understanding-sal. Nothing
  on that page says the compiler refuses a call.
- **What silence costs**, verified: warning C28197 "Possibly leaking memory ...
  is usually due to inadequate annotations on the called function ... Functions
  that free memory should be annotated with `__drv_freesMem`."
  https://learn.microsoft.com/en-us/cpp/code-quality/c28197?view=msvc-170. So an
  unannotated callee is read as neither freeing nor aliasing: a false leak report
  where it frees (stated on the page), and, by inference (not stated), no
  double-free report where the caller frees again.

### 4. Swift's C importer and `consuming`

- **The word is literally "consumed"** at the attribute level, verified:
  `ns_consumed`, "the callee expects to take ownership of a +1 retain count"
  (https://clang.llvm.org/docs/AutomaticReferenceCounting.html); `cf_consumed`,
  "the object reference is implicitly passed to a call to CFRelease upon
  completion of the call"
  (https://clang.llvm.org/docs/analyzer/user-docs/Annotations.html).
- **The macro spelling is RELEASES, not CONSUMED**: swift-corelibs-foundation's
  `CFInternal.h` defines `CF_RELEASES_ARGUMENT_OBJ` as
  `__attribute__((cf_consumed))`, or `ns_consumed` under Objective-C, verified
  via search snippet,
  https://github.com/apple/swift-corelibs-foundation/blob/main/CoreFoundation/Base.subproj/CFInternal.h.
  Apple's own `CFBase.h` returned 404 at two URLs: the `CF_RELEASES_ARGUMENT`
  spelling there is unverified.
- **How Swift imports a `cf_consumed` parameter: not found.** Searched
  `test/ClangImporter/cf.swift` (it shows only that `CCRetain`/`CCRelease`
  import as unavailable, "Core Foundation objects are automatically memory
  managed", https://github.com/swiftlang/swift/blob/main/test/ClangImporter/cf.swift,
  verified), `docs/HowSwiftImportsCAPIs.md` (silent),
  `lib/ClangImporter/ImportType.cpp` lines 1 to 1000 (no match), and PR #28527
  (fetch failed). Unverified.
- **SE-0377, verified**: `borrowing` and `consuming`, implemented in Swift 5.9,
  replacing the compiler's internal `__shared`/`__owned`; `consuming` means "the
  callee becomes responsible for either releasing the parameter or passing
  ownership of it along somewhere else".
  https://github.com/swiftlang/swift-evolution/blob/main/proposals/0377-parameter-ownership-modifiers.md.
  Note the overlap: Swift's one word covers KEEPS and FREES both, because the
  Swift caller never retains a claim; a Heroes lease is a claim the runtime
  retains, which is why one word cannot serve here. A forum thread titled
  "Nobody really seems to know what the `consuming` keyword does"
  (https://forums.swift.org/t/nobody-really-seems-to-know-what-the-consuming-keyword-does/67777)
  was seen by title only, unverified.

### 5. Rust FFI conventions

- **Convention only, no annotation**, verified twice. `CString::into_raw`:
  "Consumes the CString and transfers ownership of the string to a C caller ...
  one should not use the standard C free() function to deallocate this string
  ... Failure to call CString::from_raw will lead to a memory leak."
  https://doc.rust-lang.org/std/ffi/struct.CString.html. `Box::into_raw`:
  "Consumes the Box, returning a wrapped raw pointer."
  https://doc.rust-lang.org/std/boxed/struct.Box.html#method.into_raw.
- The Rust Reference's "External blocks" lists `link`, `link_name`,
  `link_ordinal` as the attributes of an extern block and its items; none
  concerns ownership, freeing or lifetime.
  https://doc.rust-lang.org/reference/items/external-blocks.html, verified. So
  R5 of `docs/measurements/037` has a shipping precedent, and it carries no word
  on the extern declaration.

### 6. Nim `sink`, D `scope`/`return scope`, Vala `owned`

- **Nim `sink` = callee takes ownership (keeps or frees)**, verified: "A
  location that is passed to a sink parameter should not be used afterward ...
  If it cannot be proven to be the last usage of the location, a copy is done
  instead." https://nim-lang.org/docs/destructors.html. Misuse is silently
  repaired by a copy, never refused: a word that fails soft.
- **D `scope` = callee lets go**, verified: "A scope parameter of reference type
  must not escape the function call"; `return scope`: "can only escape those
  indirections via the function's return value".
  https://dlang.org/spec/function.html. Neither says anything about freeing; D's
  words are `lent`-polarity.
- **Vala `owned` on a parameter = callee takes ownership**, verified: "All
  parameters are, by default, unowned, unless marked with the owned keyword";
  "All return values and ref and out parameters are, by default, owned, unless
  marked with the unowned keyword"; and the cost, in Vala's own words: "If
  ownership semantics are not correct, either a memory leak has been written or
  a double-free has been written."
  https://docs.vala.dev/guides/bindings/writing-a-vapi-manually/05-00-fundamentals-of-binding-a-c-function/05-02-ownership.html.
  The binding author's claim is unchecked, and the maintainers say so.

### 7. The word itself

| word | where | means | renamed? |
|---|---|---|---|
| `ns_consumed` / `cf_consumed` | Clang ARC, analyzer | callee releases | no (verified, item 4) |
| `consuming` | Swift 5.9 | callee owns: releases or keeps | from `__owned`, SE-0377 (verified) |
| "Consumes" | Rust std docs | caller loses the value | no (verified) |
| `ownership_takes` / `ownership_holds` | Clang analyzer, 2010 | frees / keeps | no (verified) |
| `(transfer full)` | GI | recipient owns | `floating` alias for none (verified) |
| `owned` | Vala | callee owns | from `#`, thread of 2008-12-19, "very unintuitive as a type modifier" (verified, https://groups.google.com/d/topic/gnome-vala/WZLn2YjD8Xw) |
| `sink` | Nim; Hylo's `let`/`inout`/`sink`/`set` (verified, https://docs.hylo-lang.org/language-tour/functions-and-methods); C++ by-value `unique_ptr`, GotW #91, 2013 (verified, https://herbsutter.com/2013/05/30/gotw-91-smart-pointer-parameters/) | callee owns | none found |

Every renaming I found went TOWARD a plain English word (`#` to `owned`,
`__owned` to `consuming`) and never away from one. Only Clang's analyzer ever
split "frees" from "keeps" in the vocabulary, and it did so at birth.

### 8. The default, directly

**No shipping binding system I found refuses an unannotated pointer parameter.**
Each reads silence as one case: GI as `(transfer none)`; Vala as `unowned`; SAL
as not-freeing-not-aliasing (C28197's wording); Clang's analyzer as unmodelled
unless `Optimistic`; Nim as not-`sink` (copy); Rust has no vocabulary to be
silent in; Swift imports the parameter as `UnsafePointer` with no ownership
claim. Searched: the eight systems above plus Swift's C++ interop, which RENAMES
a method returning a projection to `__…Unsafe` rather than refusing it
(verified, https://www.swift.org/documentation/cxx-interop/).

**The nearest thing to route B's polarity is Swift 6.2's SE-0458**, verified:
`-strict-memory-safety` "will flip the polarity of its safety assumptions. It
will treat all types that are not known to be safe as unsafe"; it is opt-in and
"All of the memory-safety diagnostics ... will be warnings".
https://github.com/swiftlang/swift-evolution/blob/main/proposals/0458-strict-memory-safety.md.
Its cost to users: not found; too new to have a record I could locate.

## what the precedents say about the WORD and about the DEFAULT

**The word.** `consumes` is the right family: the two systems that enforce
anything at the caller (Clang ARC, Swift) both use it, and Rust's documentation
uses the verb. But in every precedent the word means *takes ownership*, KEEPS
included, because the caller there retains no claim once it hands the value
over. Heroes' lease is a claim the runtime keeps, so KEEPS and FREES differ at
the caller, and the only precedent in that position, Clang's malloc checker,
used two words from 2010. The engineer's veto has fifteen years of precedent
behind it. The spec sentence should therefore say *C frees* in those words, not
*C takes ownership*, or readers will import the precedent meaning.

**The default.** "Silence means keeps or reads" is the universal shipping
default, and its one recorded failure is GLib #1373, defect 070's shape found
from the bindings side and repaired by one word on one parameter. Route B has no
shipping instance; the one polarity flip that shipped (Swift 6.2) is opt-in and
warns. That is not an argument against B, since this language's thesis is
exactly the refusal nobody else ships; it is a statement that B's cost has no
record to consult, and A's has one.

## one falsifiable prediction, with its instrument

Of the first five `consumes` marks written on a `cstr` or `ptr` parameter in
`examples/` or `tests/golden/`, **at least one will sit on a parameter whose C
body does not call `free`**: the `strdup` shape or the `keep_label` shape,
because every precedent reader has learned `consumes` as *takes ownership*.
Instruments: `grep -rn 'consumes' examples tests/golden --include='*.hero'` read
beside each paired header; `tests/golden/check/unread-mark.hero:26`
(`function strdup(s: cstr consumes) -> cstr`, read this sitting), which becomes
a LEGAL and FALSE declaration the day route A lands and is judged by the `check`
suite; and `build --sanitize`, which the shared brief shows catching the
forgotten word (070) and which will show nothing for the false one, since a
falsely `consumes`-marked `strdup` leaks rather than crashes. Falsified if five
such marks land and every one sits on a real freer.

## corrections to the shared brief

1. The historian brief calls GI's `none`/`container`/`full` "the closest
   shipping instance of route B's vocabulary". It is not: `container`
   distinguishes elements from container, not KEEPS from FREES. The closest
   shipping three-way for the callee side is Clang's 2010 `ownership_holds` /
   `ownership_takes` / unannotated, analyzer-only and opt-in.
2. The brief offers `borrows` as route B's KEEPS word. In every precedent
   (`borrowing` in Swift, borrow in Rust, D's `scope`) the word means *does not
   keep*. Using it for *keeps* would be the one departure from precedent in this
   sitting that is accidental rather than deliberate.
3. "Clang's 2026 noescape-on-pointers RFC from panel 171's brief": panel 171's
   shared brief does not mention an RFC, and I found none; what exists is
   September 2026 inference work, silent on freeing.
4. Item 4's question "is the word literally consumed?": at the attribute, yes;
   at the macro, the spelling is RELEASES (`CF_RELEASES_ARGUMENT`), verified only
   through its `_OBJ` variant.

## argument

Every enforced ownership word on a C parameter says *the callee takes ownership*
and lets the callee decide whether that is keeping or freeing, because the caller
has already lost its claim. Heroes' lease is different: the runtime still owns
the block after the call, so keeping and freeing diverge at the caller, and the
one precedent in that position, Clang's malloc checker, spent two words on it
from 2010. So: `consumes` for the freer, spelled *C frees* in the spec, and never
`borrows` for the keeper. On the default, route A is what everyone shipped and
GLib #1373 is its recorded bill; route B is unshipped, and Swift 6.2's flip chose
opt-in warnings. Neither fact decides; both should be written down.

## condition

Two findings would change this reading. A shipping COMPILER, not an analyzer,
that refuses an unannotated pointer parameter at the C boundary, with its cost
recorded, would give route B a precedent and me a bill to quote. A shipping
system where one word carries both KEEPS and FREES while the CALLER retains a
claim on the block would weaken the two-words finding; I searched the eight
systems above and found none.
