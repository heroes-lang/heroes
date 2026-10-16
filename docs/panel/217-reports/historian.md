# Panel 217, historian seat (advisory, no veto)

Written 2026-10-16, from about 22:40. **Every source below was read this session, 2026-10-16**, at
the URL given.

- **verified** means I re-read the primary text, or the project's own documentation, at that URL
  tonight.
- **unverified** means I could not confirm it.
- *(inference)* marks my reasoning from a source, not a repetition of it.

No claim rests on *Heroes of code*. I used it only to know which rooms to search: Zurich for Wirth,
Bell Labs for Ritchie.

This seat has no shell. Its tools are Read, Write, WebSearch and WebFetch. It ran no probe and built
nothing. Every fact about Heroes below comes from the shared brief or from the critic's report, and
is cited as theirs. No paid run, no credential.

Files read: `docs/panel/217-briefs/historian.md`, `docs/panel/217-briefs/00-shared.md`,
`docs/panel/217-reports/critic-briefs.md`, `m-g-at` lines 118-129, 265-278 and 362-464
(`docs/panel/215-reports/llm-ergonomist-work-2/heroes-spec-m-g-at.md`), and, for the format,
`docs/panel/215-reports/historian.md`.

## verdict

**Advisory, per form.** The thirteen languages read are Ada, C++, Cpp2, Swift, Rust, Hylo, Nim, D,
Oberon-07, Modula-3, Free Pascal, Fortran and C.

- **F0, nothing: object.** Each of the thirteen has a read-only parameter, apart from its mutable
  one, that reaches the caller's object or that the compiler may pass that way. In none of them does
  the writer have to claim a change to avoid a copy. That rests on the thirteen, not on the world.
- **F1, `lent` on a Heroes parameter, no mark at the call: approve, on two conditions.**
  1. A read-only lend beside an `@` lend of the same place is **refused**, as Hylo refuses
     `offset_inout(&v, by: v)` and names `v.copy()` as the fix. The alternative is Swift's: the
     caller copies wherever exclusivity cannot be proved.
  2. The C half is settled apart from the Heroes half (critic U1).

  The word fits outside precedent. Swift's `borrowing` and Nim's `lent` each mean, in one word, *not
  kept* and *not written*. An unmarked read-only lend beside a marked mutable one is Swift's and
  Hylo's exact surface.
- **F2, no word, the compiler passes by place: approve only as how a by-value parameter is lowered,
  under condition 1 above. Object as the answer.** It is the most common precedent: Oberon-07, Nim,
  D, Cpp2, Swift's default, Hylo and Ada. It is also the one with the documented failures (§ The
  aliasing table). It does not reach C.
- **F3 as written equals F0** (the critic measured this): no reading.
- **U5, an `@` parameter that is only read, admitted: object.** Rust allows its form of this shape,
  a `&mut` that is only read, and Clippy flags it. But Rust has `&T` to switch to. Under U5 a Heroes
  writer keeps a signature that claims a change.
- **U1, a new word: approve as F1's equal.** Precedents: Ada's `in`, Modula-3's `READONLY`, D's and
  Cpp2's `in`, Pascal's `const`.
- **U2, a mark at the call for the read-only lend: neutral, leaning object.** Of the thirteen, only
  Rust marks it, and Rust drops the mark on a method's receiver.
- **U3, decided by type for a record that cannot be copied: approve as a complement.** Ada and D do
  it with no word. Swift does the opposite: for a noncopyable type it demands the word.
- **U4 and U6: no reading.** I did not research U4. U6 leaves the language as F0 does.

## precedents

For each precedent: the spelling at the call, whether the callee may hand the value on, aliasing
beside a mutable lend of the same object, and whether the language later amended it.

### Ada, `in` parameters: verified

**Call site.** No mode is written at the call. RM 6.4.1(4): an `in` actual is read as an
expression, and the other modes take a name.

**Hand on.** An `in` formal is *"a constant view ... it cannot be updated within the
subprogram_body"* (RM 6.2(13)). It cannot be the actual of an `in out`, because *"If the mode is in
out or out, the actual shall be a name that denotes a variable"* (RM 6.4.1(5)).

**Mechanism.** Elementary types are passed by copy, RM 6.2(3/3). By-reference types are listed in
RM 6.2(4) to (9): tagged, task and protected types, an *"explicitly limited record type"*, and any
composite holding one. For everything else, *"it is unspecified whether the parameter is passed by
copy or by reference"* (RM 6.2(11/3)).

**Aliasing.** Two names can reach one object, one through a formal. RM 6.2(12/3) makes it a
**bounded error** to write through one and read through the other. The outcome is `Program_Error`,
the new value, or an old one.

**Amended.** Ada 2012 made it **illegal** to pass one elementary object to more than one `in out` or
`out` parameter of the same call. This was an incompatibility with Ada 2005 (RM 6.4.1(6.15/3,
6.16/3); AARM 6.4.1(18.d), AI05-0144-2). The rule names only `in out` and `out`. An `in` actual
overlapping an `in out` actual stays a bounded error.

**Sources.**
- https://www.adaic.org/resources/add_content/standards/12rm/html/RM-6-2.html
- https://www.adaic.org/resources/add_content/standards/12rm/html/RM-6-4-1.html
- https://www.adaic.org/resources/add_content/standards/12aarm/html/AA-6-4-1.html

All read 2026-10-16. This is Ada 2012's text: ada-auth.org refused the connection, so Ada 2022's
text was not read.

**Unverified.** Barnes's *Rationale for Ada 2012* sentence on functions gaining `in out`: its PDF
returned 404.

### C++, `const T&`: verified

**Call site.** No mark. LWG 526's own example is `v.insert(v.begin(), v[2])`.

**Guidance.** Core Guidelines F.16: *"For "in" parameters, pass cheaply-copied types by value and
others by reference to `const`"*. The Reason given: *"Both let the caller know that a function will
not modify the argument"*. Its Enforcement warns about two shapes:
- a by-value parameter larger than `4 * sizeof(void*)`;
- a `const&` parameter of size `2 * sizeof(void*)` or less.

So the writer chooses, and a tool polices the choice. Source:
https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines, read 2026-10-16.

**Aliasing.** LWG 526, *"Is it undefined if a function in the standard changes in parameters?"*,
opened 2005-09-14, closed **NAD** (not a defect). Two calls whose `const T&` argument aliases the
container are *"required to work because the standard doesn't give permission for it not to work"*:
- `v.insert(v.begin(), v[2])`;
- `list::remove`.

So `const T&` does not promise that the object stays unchanged during the call. Source:
https://cplusplus.github.io/LWG/issue526, read 2026-10-16.

**Hand on, and handing back.** cppreference's `std::max`: *"Capturing the result of `std::max` by
reference produces a dangling reference if one of the parameters is a temporary and that parameter
is returned"* (https://en.cppreference.com/w/cpp/algorithm/max, read 2026-10-16).

### Cpp2 (cppfront), `in`: verified

**Mechanism.** `in` is the default and is *"always `const`"*. It lowers to `X const x` or
`X const& x`, and *"automatically passes by value if cheaply copyable"*. So the compiler makes F.16's
choice; `in_ref` forces a reference.

**Call site.** `inout` lowers to `X&`, and the page's example calls it as a plain `h( x );`. The
page says nothing on aliasing.

**Source.** https://hsutter.github.io/cppfront/cpp2/functions/, read 2026-10-16.

**Unverified.** Sutter's paper 708 (repository https://github.com/hsutter/708, read 2026-10-16)
could not be read as text, so its content is unverified, and so is its standing in WG21.

### Swift, the default convention, `borrowing` (SE-0377) and the Law of Exclusivity (SE-0176): verified

**SE-0377** is *"Implemented (Swift 5.9)"*, by Michael Gottesman and Joe Groff.
- **Default.** Ordinary functions *"default to borrowing their parameters, since we have found this
  to be more efficient in most situations"*. Initializers and setters consume. So an unmarked
  parameter is already a borrow; this is F2's shape.
- **Call site.** The modifiers *"do not affect the caller-side syntax"*.
- **Access.** *"`borrowing` parameters get shared access to their argument"*, while `inout` gets
  exclusive access.
- **ABI.** The modifier *"affects the ABI-level calling convention"*.
- **Aliasing, the caller's answer.** For *"global or static variables, escaped closure captures, and
  class stored properties"*, the caller **copies** before the call, to *"avoid running afoul of the
  law of exclusivity with mutations"*. Otherwise a callee mutating the global would *"trigger a
  dynamic exclusivity failure (or UB if exclusivity checks are disabled)"*.
- Source:
  https://github.com/swiftlang/swift-evolution/blob/main/proposals/0377-parameter-ownership-modifiers.md,
  read 2026-10-16.

**TSPL, Declarations** (https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/ReferenceManual/Declarations.md,
read 2026-10-16).
- *"The `borrowing` modifier indicates that the function does not keep the parameter's value"*.
- A borrowed parameter *"cannot be copied unless you use an explicit `copy` keyword"*.
- So **one word means both *not kept* and *not written***. *(inference)* That is the pair F1 puts in
  `lent`.
- For `inout`: *"Within a function, don't access a value that was passed as an in-out argument"*, and
  *"you can't pass the same value to multiple in-out parameters"*.

**TSPL, Functions**
(https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/Functions.md,
read 2026-10-16). Parameters are constants by default, and *"You place an ampersand (`&`) directly
before a variable's name"* when passing it to an `inout` parameter. So **only the mutable lend is
marked at the call**.

**SE-0390**, *"Implemented (Swift 5.9)"*. For a noncopyable type, a parameter *"must declare whether
the parameter uses the `borrowing`, `consuming`, or `inout` convention"*, because *"the ownership
convention becomes a much more important part of the API contract"*. Borrowers *"are not allowed to
mutate the value"*. Source:
https://github.com/swiftlang/swift-evolution/blob/main/proposals/0390-noncopyable-structs-and-enums.md,
read 2026-10-16.

**SE-0176**, *"Implemented (Swift 4.0)"*
(https://github.com/swiftlang/swift-evolution/blob/main/proposals/0176-enforce-exclusive-access-to-memory.md,
read 2026-10-16).
- *"two accesses to the same variable are not allowed to overlap unless both accesses are reads"*.
- Local variables are checked statically; class properties and globals dynamically.
- An `inout` argument's write access *"does not begin until immediately before the call"*.
- `swap(&x, &x)` is a conflict.

**Unverified.** Whether `f(&x, x)` on a local, with a `borrowing` second parameter, is diagnosed.
None of the four pages gives that example.

### Rust, `&T` beside `&mut T`: verified

**Call site.** The caller writes `&`: *"we pass `&s1` into `calculate_length`"*. The rule: *"At any
given time, you can have either one mutable reference or any number of immutable references."*
Source: https://doc.rust-lang.org/book/ch04-02-references-and-borrowing.html, read 2026-10-16.

**Receiver exception.** For a method's receiver the mark is supplied: *"for each candidate `T`, add
`&T` and `&mut T` to the list immediately after `T`"*. So `x.f()` needs no `&`. Source:
https://doc.rust-lang.org/reference/expressions/method-call-expr.html, read 2026-10-16. This bears
on the critic's UFCS question 3.

**Aliasing.** Refused, E0502: *"A variable already borrowed with a certain mutability ... was
borrowed again with a different mutability"* (https://doc.rust-lang.org/error_codes/E0502.html,
read 2026-10-16).

**Amended.** RFC 2025, two-phase borrows: start date 2017-06-06, issue rust-lang/rust#44100. It
admits `vec.push(vec.len())`: the `&mut` borrow is first *reserved*, with reads allowed, and only
*activated* at the call. A shared borrow still alive at activation stays an error. Source:
https://rust-lang.github.io/rfcs/2025-nested-method-calls.html, read 2026-10-16.

**Not a promise of no change.** *"A type has interior mutability if its internal state can be
changed through a shared reference to it"*, through `UnsafeCell`. Source:
https://doc.rust-lang.org/reference/interior-mutability.html, read 2026-10-16.

**A `&mut` only read.** Clippy's `needless_pass_by_ref_mut`, *"using a `&mut` argument when it's not
mutated"*: group `nursery`, `#[clippy::version = "1.73.0"]`. So in Rust the shape is legal and
linted, not refused. Source:
https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/needless_pass_by_ref_mut.rs,
read 2026-10-16.

### Hylo (formerly Val), `let` parameters: verified

**Mechanism.** *"`let` is the default convention"*; *"`let` parameters are (notionally) passed by
value, and are truly immutable"*. The compiler *"allows arguments to be passed without making any
copies"*, and the page likens `let` to C++'s `const` reference and Rust's immutable borrow.

**Hand on.** Without a copy, a `let` parameter *"can't be returned, or stored anywhere that outlives
the call"*.

**Call site.** A `let` argument carries no mark. An `inout` argument is *"marked with an ampersand
(`&`) at the call site"*.

**Aliasing, the critic's own case.** Given `fun offset_inout(_ target: inout Vector2, by delta:
Vector2)`, whose `delta` is `let` by default, the call `offset_inout(&v, by: v)` is a compile-time
*"overlapping `inout` access to `v`"*. The fix: *"pass `v.copy()` as the second argument
instead"*.

**Source.** https://docs.hylo-lang.org/language-tour/functions-and-methods, read 2026-10-16.

**Unverified.** The year of the rename from Val; no announcement was re-read.

### Nim, ordinary parameters, `var` and `lent`: verified

**Mechanism.** *"Since non-var parameters cannot be modified the compiler is always free to pass
arguments by reference if it considers it can speed up execution."* This is F2, stated as a rule.

**Call site.** *"Var parameters are implemented as hidden pointers"*, and the page's call
`divmod(8, 5, x, y)` writes no mark.

**Source.** https://nim-lang.org/docs/manual.html, read 2026-10-16.

**`lent T`.** It is a return-type annotation for *"an immutable view into a container"*. It is *"like
var T a hidden pointer. It is proven by the compiler that the pointer does not outlive its
origin"*, and *"Unlike 'var' this hidden pointer cannot be used to mutate the object"*. So Nim's
`lent`, too, means *borrowed and not written*. Whether `lent` may be written on a parameter: neither
page says. Source: https://nim-lang.org/docs/destructors.html, read 2026-10-16.

**Aliasing, as it went.** Nim issue #14983, *"memory corruption for seq.add(seq) with gc:arc and
d:useMalloc"* (https://github.com/nim-lang/Nim/issues/14983, read 2026-10-16):
- opened 2020-07-14 and closed 2020-09-25 by commit e9fa486;
- `s.add(s)` passes one seq as both the `var` and the non-`var` argument;
- cooldome: *"If you pass the same `s` as both arguments, you get undefined behavior"*;
- Araq: *"I'm leaning to the compiler producing a copy instead when the aliasing can cause trouble.
  No need to break code."*

**Still unenforced.** The experimental manual's *"Aliasing restrictions in parameter passing"* says
*"An input and an output parameter should not be aliased"*, and *"The aliasing restrictions are
currently not enforced by the implementation"*. Source:
https://nim-lang.org/docs/manual_experimental.html, read 2026-10-16.

### D, `in` and `-preview=in`: verified

**Mechanism** (spec, https://dlang.org/spec/function.html, read 2026-10-16).
- `in` behaves as `const` and as `scope`, and *"Input parameters may also be passed by reference by
  the compiler"*.
- Types that *"cannot be copied"*, or whose copy would have a side effect, are **always** passed by
  reference. This is U3's rule.
- For other types the choice is implementation-defined.
- The section opens: *"The following requires the -preview=in switch"*.

**When it came.** Changelog 2.094.0, released Sep 22, 2020: *"in parameters will now be passed by
reference when optimal, and will accept rvalue"* (https://dlang.org/changelog/2.094.0.html, read
2026-10-16).

**The word changed meaning more than once.** The D Blog of Oct 15, 2020 says *"The D2 version of
`in` was intended to be equivalent to `const scope`"* but in practice was `const`, and that with
2.092.0 it *"finally became equivalent to `const scope`"*. Source:
https://blog.dlang.org/archive/2020/10/15/d-2-094-0-dconf-online-schedule-and-saoc-2020/, read
2026-10-16.

**Call site, and aliasing: the critic's probe in D, verified.** Steven Schveighoffer, 2020-10-02,
*"-preview=in might break code"* (https://forum.dlang.org/thread/rl7c8t$sml$1@digitalmars.com?page=1,
read 2026-10-16):
- the function is `void foo(T)(in T constdata, ref T normaldata)`;
- it writes `normaldata.data[0] = 1;` and then prints `constdata.data[0]`;
- it is called `foo(smallval, smallval)` and `foo(largeval, largeval)`, with no mark;
- without the switch both calls print `0`; with it, the large struct prints `1`.

This is the critic's `bump_and_read(a: @p, b: p)` going from 1 to 2.

**Unverified.** Walter Bright's reply, and his *"10 releases"* remark: search snippet only.

**Not adopted.** *(inference)* The spec read tonight still puts the by-reference `in` behind a
preview switch, six years after 2.094.0.

### Wirth, Oberon-07: verified

**Mechanism.** Wirth, *Differences between Revised Oberon and Oberon*, 22.03.2008 / 15.7.2011,
§ 5 Parameters: *"This implies making a copy of that value, which is undesirable in the case of
structured types. In order to avoid the necessity of copying, we postulate that if a value parameter
is if a structured type, no assignments are possible. Hence, the parameter can be passed by a
reference like in the case of VAR parameters."* The introduction counts *"constant parameters"*
among the changes to *"be considered as permanent, and as corrections of unsatisfactory properties
of the original Oberon"*. Source: https://people.inf.ethz.ch/wirth/Oberon/Oberon07.pdf, read
2026-10-16.

**The report.** *The Programming Language Oberon*, revision 1.10.2013 / 3.5.2016:
- § 9.1: *"If a value parameter is structured (of array or record type), no assignment to it or to
  its elements are permitted."*
- § 10.1: VAR parameters are *"denoted by the symbol VAR and value parameters by the absence of a
  prefix"*.
- The calls `ReadInt(i)` and `INC(w[k].count)` carry no mark.

Source: https://people.inf.ethz.ch/wirth/Oberon/Oberon07.Report.pdf, read 2026-10-16.

**F2 with no new word, in this mandate's own lineage.** Wirth's note says nothing on aliasing.

**Aliasing, an implementer's warning.** Mike Spivey, writer of the Oxford Oberon compiler, in his
Oberon-07 checklist: *"Presumably this means aggregate value parameters are not copied, though such
an implementation creates aliasing problems."* Source:
https://spivey.oriel.ox.ac.uk/corner/Oberon07_checklist (page last modified 2019-04-07), read
2026-10-16.

### Modula-3, `READONLY`: verified

**Mechanism.** *"A READONLY formal is treated as a VAR formal if the actual is a designator"* of the
formal's type, and otherwise as a VALUE formal. *"A VAR formal is bound to the variable designated by
the corresponding actual; that is, it is aliased."* Source, *Modula-3: Procedure calls*:
https://softwarepreservation.computerhistory.org/modula3/archive/DEC_SRC/release-3.6-html/m3defn/html/calls.html,
read 2026-10-16.

**Call site.** No mark.

**Aliasing.** The page says nothing beyond VAR's own *"it is aliased"*. *(inference)* So a READONLY
formal sees a write made through a VAR alias in the same call.

**Hand on.** *"An identifier is a readonly designator if it is a READONLY parameter"*, and a VAR
actual *"must be a writable designator"*. So a READONLY formal cannot be passed on as VAR. Source:
https://softwarepreservation.computerhistory.org/modula3/archive/DEC_SRC/release-3.6-html/m3defn/html/designators.html,
read 2026-10-16.

### Free Pascal (and Delphi), `const`: verified for Free Pascal

Reference guide § 14.4.4, *Constant parameters*
(https://www.freepascal.org/daily/doc/ref/refsu70.html, read 2026-10-16; the daily docs renumber
their pages):
- *"Contrary to earlier versions of Delphi, no assumptions should be made about how const parameters
  are passed"*;
- `constref` forces a reference, *"available as of version 2.5.1"*;
- `const` is *"a contract between the programmer and the compiler"*;
- *"a const parameter cannot be passed on to another function that requires a variable parameter"*.

**Call site, and aliasing by design.** `DoIt(Const T: String)` assigns the global
`S := 'Something else'` and then prints `T`. The call `DoIt(S)`, with no mark, prints *"Something
else"*, and the page says *"This behavior is by design."*

**Unverified.** Delphi's own page and its `[Ref]` attribute: docwiki.embarcadero.com returned 403.

### Fortran, `INTENT(IN)`: verified, through Flang's documentation

Flang's *Aliasing in Fortran* (https://flang.llvm.org/docs/Aliasing.html, read 2026-10-16) is an
implementer's reading of the standard, not the standard's text:
- *"Fortran famously passes actual arguments by reference"*;
- the rule is that *"a dummy argument by which some particular bit of storage has been modified must
  be the only means by which that storage is referenced"*;
- *"A Fortran compiler is free to assume that a program conforms with this rule"*;
- *"`INTENT(IN)` does not prevent the same variable from also being associated with another dummy
  argument in the same call"*, which is conforming only if the subprogram never references the
  `INTENT(IN)` dummy.

**Unverified.** When `INTENT` entered Fortran, and the call syntax, which was not read tonight.

### C, `const T *`: verified

**Ritchie.** Dennis Ritchie, *"Why I do not like X3J11 type qualifiers"*, comp.lang.c, 20 March 1988
(https://www.lysator.liu.se/c/dmr-on-noalias.html, read 2026-10-16):
- *"Noalias must go. This is non-negotiable."*;
- *"Const has two virtues: putting things in read-only memory, and expressing interface
  restrictions"*;
- avoiding writes through pointers derived from `const` objects is *"the programmer's
  responsibility"*;
- on `strchr`, *"there is no way in X3J11's language for 'strchr' to return the value it promises
  to"*.

**`restrict` and overlap.** `memcpy` gained `restrict` in C99, and *"If the objects overlap (which is
a violation of the restrict contract)(since C99), the behavior is undefined"*
(https://en.cppreference.com/w/c/string/byte/memcpy, read 2026-10-16). This bears on the critic's
`memcpy`-shaped question.

**Amended, for the critic's question 5.** WG14 N3020, *Qualifier-preserving standard library
functions, v4*, Alex Gilding, 2022-06-28. It names *"twelve "qualifier-losing" search functions"*
that *"accept a `const`-qualified pointer ... but return the pointer to the found element without
`const`-qualification"* (https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3020.pdf, read
2026-10-16). cppreference shows `strchr` since C23 as type-generic: given `const T*`, *"the return
type is `const char*`"* (https://en.cppreference.com/w/c/string/byte/strchr, read 2026-10-16).

### One word given a second meaning

- **C++ `auto`, verified.** N2546, *Removal of auto as a storage-class specifier*, Vandevoorde and
  Maurer, 2008-02-28. The new meaning *"results in undesired interpretations of `auto` as a storage
  class specifier in certain contexts"*, so the old one was removed; its use was judged *"Rare"*.
  Source: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2546.htm, read 2026-10-16.
- **C++ `static`, verified.** CWG 1012, *"Undeprecating static"*, 2009-12-09: namespace-scope
  `static`, deprecated since C++98, was undeprecated for C++11, because *"it is unlikely that the
  feature will be removed ... especially in light of C compatibility concerns"*. Source:
  https://cplusplus.github.io/CWG/issues/1012.html, read 2026-10-16.
- **D `in`, verified above.** Its meaning moved: `const scope`, then `const` in practice, then
  `const scope` again in 2.092.0, then by-reference under a preview.
- **Swift `borrowing` and Nim `lent`, verified above.** Each already carries *not kept* and *not
  written* in one word.
- *(inference)* So F1 does not give `lent` a meaning foreign to the word. It does make one word
  carry, on a C parameter, *C receives the place* as well as *C does not keep it*. I found no
  precedent for that pairing on a foreign-function parameter: I searched Swift, Nim, D, Rust, Ada
  and C. It is the critic's U1, and it is a question.

## The aliasing table

The case: a read-only lend and a mutable lend of one object in one call.

| language | read-only lend passed by place? | the answer to the alias | outcome |
|---|---|---|---|
| Ada | unspecified for most types | bounded error; illegal only between two `in out`/`out` (2012) | `Program_Error`, new or old value |
| C++ | `const T&`, yes | allowed | the library must cope (LWG 526, NAD) |
| Free Pascal | the compiler chooses | allowed | prints the new value, *"by design"* |
| Fortran | yes | the program's obligation; the compiler assumes none | non-conforming if the `IN` dummy is read |
| Modula-3 | `READONLY` on a designator, yes | allowed | *(inference)* sees the write |
| Oberon-07 | structured value parameters, yes | silent | Spivey: *"creates aliasing problems"* |
| Nim | the compiler chooses | documented, *"not enforced"* | memory corruption, #14983 (2020) |
| D | `-preview=in` chooses | silent | output 0 becomes 1 (2020); still a preview |
| Swift | default borrow | Law of Exclusivity; the caller copies shared state | dynamic failure or a copy |
| Rust | `&T`, yes | **refused**, E0502 (two-phase reservation aside) | compile error |
| Hylo | `let`, yes | **refused**, *"overlapping `inout` access"* | compile error; fix `v.copy()` |

*(inference)* Every language here that passes a read-only parameter by place and does not refuse
the alias leaves the result implementation-dependent, by design, or wrong. The ones that refuse it
(Rust, Hylo, and Swift statically for locals) were designed with the rule from the start. Heroes
already refuses two `@` arguments of one place (the critic, citing panel 215). Hylo's rule is that
refusal widened by one row.

## Which precedents bear on a reader choosing between a value and a place

- **The precedents that remove the choice.** Swift (*"Other functions default to borrowing"*), Hylo
  (`let` is the default), Cpp2 (`in` picks value or `const&`), Nim, Oberon-07 and D. Each says the
  writer should not have to pick: the language takes the read-only parameter by place when it can.
  *(inference)* Panel 215's ten readers all picked the value: when a reader has to choose, the
  reader chooses the copy.
- **The precedent that keeps the choice.** C++ keeps it, and needed F.16 with a size threshold
  enforced by a tool, then Cpp2's `in` to take it away again.
- **Swift's exception.** For a type that cannot be copied, Swift makes the word mandatory, because
  the convention is *"part of the API contract"*. The counter holding a lock is such a type.
  *(inference)* For it, precedent favours a written word, F1 or U1, over a silent F2 or U3.

## argument (≤120 words)

All thirteen languages read have a read-only parameter apart from the mutable one. Ten, by the
pages read, mark nothing at the call. Swift and Hylo mark only the mutable lend, which is F1's
surface. Swift's `borrowing` and Nim's `lent` already mean *not kept, not written*. So F1 moves
Heroes' word toward its outside meaning. The documented failures are aliasing, not spelling: Nim's
memory corruption, D's output turning 0 into 1, Free Pascal's *"by design"*, Ada's bounded error,
Spivey's warning on Oberon-07. The languages without them refuse the alias (Rust, Hylo) or copy at
the caller (Swift). So F1 is sound only with Hylo's refusal of a read-only lend beside an `@` lend
of one place.

## condition

My reading changes if any of these turns up:

1. **A language that passes a read-only parameter by place, leaves the alias unrefused, and reports
   no failure from it in use.** That would weaken condition 1 to a note. I found none among the
   thirteen.
2. **A census of the tracked programs that build** (the ffi-pragmatist's) **finding a call that
   passes one place both by `@` and as a read-only argument whose meaning depends on the copy.**
   That would make the refusal a break of existing programs rather than a new rule, Ada 2012's
   incompatibility. It is still sound, but it would owe a migration.
3. **Swift's static diagnosis of `f(&x, x)` with a `borrowing` second parameter**, found refused or
   found copied. Either way it settles which of Swift's two answers is the precedent for a local.
   Unverified tonight.
4. **A precedent for one word meaning both *C reads the place* and *C does not keep it* on a foreign
   parameter**, or a language that withdrew such a word. Either would settle U1 against F1 on the
   C half.

## prediction

**The prediction.** In arm F1, at least 7 of 10 readers write `doubled` with the read-only lend and
call it with no mark, and at most 2 of 10 copy the lock.

**Why.** It follows Swift's and Hylo's surface, and Panel 215's notes: nine of ten said `@c` would
claim a change.

**Instrument.** The person's tabulation of each program's helper signature (the critic's A1), not
`judge.sh`.

**When.** If the coordinator runs arm F1. A 7 or more scored on a task whose words say *"without
changing the counter"* still owes the critic's control task (A2) before it counts as reach.

## what I could not run

- No shell: no probe, no census, no build. Every Heroes fact is the brief's or the critic's.
- Left **unverified** above:
  - Barnes's Ada 2012 rationale;
  - Ada 2022's text;
  - paper 708's content;
  - Swift's `f(&x, x)` with `borrowing` on a local;
  - the year of Val's rename;
  - Walter Bright's replies;
  - Delphi's docwiki and `[Ref]`;
  - Fortran's call syntax, and when `INTENT` entered Fortran.
- U4 (a lend method at the call to C) was not researched.
