# Panel 176: historian report (advisory seat, no veto)

Written out by the coordinator on 2026-09-23 from the seat's final message; the
seat has no file-write tool. Verbatim except for two mechanical changes: the
HTML entities the transport added (`&lt;`, `&gt;`) are written as the
characters they stand for, and the absolute paths into this repository are
written relative to its root.

**Marks.** **verified** means I fetched the primary page, file or commit and the words are quoted from it. **verified-weak** means I read it only through a search engine's summary. **UNVERIFIED** means I could not confirm it. **unrun** means it needs a shell, and this seat has none.

**What this seat did not do.** I have no shell, so I built no compiler and ran none of the reproducers. Every Heroes number below is the shared brief's or panel 175's, and I did not re-run any of them.

*Heroes of code* was not used as a route or as evidence. I used panel 171's and panel 175's historian reports only as an index, and re-verified or marked every claim taken from them.

---

## `verdict`: approve V1 on one condition, approve R1, approve A2; object to V2; object to V3 as the first move

The condition on V1: the transfer stays checked against the handle's releasers (route V1c, below). Without that check, V1 drops a check that its closest precedent keeps.

---

## `precedents`

### 1. Marks that say a callee takes ownership, as against ending the life

**Clang Static Analyzer: `ownership_returns`, `ownership_takes`, `ownership_holds`. Verified.**
- **What each marks** ([Annotations](https://clang.llvm.org/docs/analyzer/user-docs/Annotations.html)):
  - `ownership_returns(type)` marks an allocator.
  - `ownership_takes(type, i)` marks a deallocator.
  - `ownership_holds(type, i)` marks *"functions that take ownership of memory and will deallocate it at some unspecified point in the future"*. Its example is `store_in_table(int key, record_t *val)`.
- **Takes against holds:** *"using taken memory is a use-after-free error, while using held memory is assumed to be legitimate."*
- **Holds is still checked.** The OpenSSF guide lists two checks ([OpenSSF](https://best.openssf.org/Compiler-Hardening-Guides/Compiler-Annotations-for-C-and-C++.html)):
  - mismatched deallocation *"if … the result of an allocation call of type allocation-type is passed to a function annotated with `ownership_takes` or `ownership_holds` with a different allocation type"*;
  - double free *"if … a value is passed more than once to a function annotated with `ownership_takes` or `ownership_holds`."*
- **Who writes it:** the header author. **What enforces it:** the analyzer, keyed on a family name, not on a function.
- **This is the one precedent that keys releasers per allocation family and also marks a transfer.** It spent two consumer words, and its transfer word still takes part in the family check.

**CoreFoundation and ARC: `CF_CONSUMED` / `ns_consumed`, `CF_RETURNS_RETAINED`, `CF_RETURNS_NOT_RETAINED`. Verified.**
- `ns_consumed`/`cf_consumed`: *"a `release` message is implicitly sent to the parameter upon completion of the call"* ([Annotations](https://clang.llvm.org/docs/analyzer/user-docs/Annotations.html)).
- `cf_returns_not_retained` exists for functions that *"may appear to obey the Core Foundation or Cocoa conventions"* but do not return an owning reference.
- ARC says `ns_consumed` is *"part of the type of the function or method, not the type of the parameter"* ([ARC](https://clang.llvm.org/docs/AutomaticReferenceCounting.html)).
- **The releaser is universal (`CFRelease`), so one consumer word covers both releasing and storing.**
- Apple's rules: *"An object may have one or more owners; it records the number of owners it has using a retain count"*. The Create Rule covers "Create"/"Copy"; the Get Rule says you do not own the result and must `CFRetain` it to keep it ([Ownership Policy](https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFMemoryMgmt/Concepts/Ownership.html)).

**GObject Introspection: `(transfer none|container|full|floating)`. Verified.**
- `"full": the recipient owns the entire value. For a refcounted type, this means the recipient owns a ref on the value.` In-parameters default to `(transfer none)` ([GI annotations](https://gi.readthedocs.io/en/latest/annotations/giannotations.html)).
- **Who writes it:** the library author, in the C doc comments; `g-ir-scanner` produces the GIR (verified-weak, [GI](https://gi.readthedocs.io/en/latest/writingbindableapis.html)).
- The releaser comes from the type. Copy/free function annotations for plain records arrived in **1.75.2, 2023-01-09** ([changelog](https://gi.readthedocs.io/en/latest/changelog.html)).
- **GI does not separate release from transfer, or a life from a reference:** one mark, discriminated by the type.

**Vala: `owned`, and `free_function` / `ref_function` / `unref_function` on the class. Verified.**
- *"All parameters are, by default, unowned, unless marked with the `owned` keyword. All return values … are, by default, owned"*, and *"If ownership semantics are not correct, either a memory leak has been written or a double-free has been written"* ([Vala ownership](https://docs.vala.dev/guides/bindings/writing-a-vapi-manually/05-00-fundamentals-of-binding-a-c-function/05-02-ownership.html)).
- Singly-owned classes carry `free_function`; reference-counted ones carry `ref_function`/`unref_function` ([compact classes](https://docs.vala.dev/guides/bindings/writing-a-vapi-manually/04-00-recognizing-vala-semantics-in-c-code/04-05-compact-classes.html)).
- **What happened: Vala shipped defect 075's exact shape in its standard binding.** Issue #645, "Closing POSIX.FILE.popen", was opened 2018-06-25 and closed 2018-11-21 ([API record](https://gitlab.gnome.org/api/v4/projects/GNOME%2Fvala/issues/645)). It quotes `[CCode (cname = "FILE", free_function = "fclose", …)]` and says the child process *"stays zombied"*.
  - It was found by a user from the symptom, not by any tool.
  - Today `popen` returns `CommandPipe`, which is `cname = "FILE"`, `free_function = "pclose"` and inherits `Posix.FILE` ([valadoc](https://valadoc.org/posix/Posix.CommandPipe.html)). **That `CommandPipe` was #645's fix is UNVERIFIED**, an inference from the two facts.

**Swift `consuming` (SE-0377). Verified.**
- Status *"Implemented (Swift 5.9)"*; the prior spellings were *"`__shared` and `__owned`"*.
- `consuming`: *"The callee becomes responsible for either releasing the parameter or passing ownership of it along somewhere else"* ([SE-0377](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0377-parameter-ownership-modifiers.md)).
- **One word, by design, for both of V1's meanings**, because the release is the type's own.

**Swift C++ interop. Verified, with gaps.**
- `SWIFT_SHARED_REFERENCE` *"takes two arguments, naming the retain and release operations"*, on the type ([swift.org](https://www.swift.org/documentation/cxx-interop/)).
- `SWIFT_RETURNS_RETAINED`/`SWIFT_RETURNS_UNRETAINED` mark +1/+0 on each function. *"Calling unannotated functions that return a shared reference type currently triggers a compiler warning."*
- PR #82488, merged 2025-07-15, says those warnings *"were previously disabled in Swift 6.2 … due to … false positives"* and puts them back behind a default-off flag ([PR #82488](https://github.com/swiftlang/swift/pull/82488)). The documentation page and the PR disagree about today's default; I record both.
- **UNVERIFIED:** the version that introduced these annotations. The summariser said 5.9, and I rejected that as unsupported.

**Rust `Box::into_raw`/`from_raw`. Verified.**
- `into_raw`: *"After calling this function, the caller is responsible for the memory previously managed by the `Box`."*
- `from_raw`: *"a double-free may occur if the function is called twice on the same raw pointer"* ([Box](https://doc.rust-lang.org/std/boxed/struct.Box.html)).
- Counted pointers are a different type, `Rc::increment_strong_count` (verified-weak, [Rc](https://doc.rust-lang.org/std/rc/struct.Rc.html)).

**SWIG `%delobject` and the `DISOWN` typemap. Verified.**
- `%delobject` *"instructs SWIG that the first argument passed to the method will be destroyed … This is similar to use the DISOWN typemap"* ([SWIG 3.0](https://swig.org/Doc3.0/Customization.html)).
- The `disown.i` test stores the disowned pointer in `B::_a` ([disown.i](https://github.com/swig/swig/blob/master/Examples/test-suite/disown.i)).
- **Two spellings, one effect, because the proxy only drops its flag and checks no releaser.**

**pybind11. Verified.**
- `take_ownership`: *"Undefined behavior ensues when the C++ side does the same"*.
- `keep_alive<Nurse, Patient>` names the receiver by index, and is *"required when the C++ object is any kind of container and another object is being added"* ([functions](https://pybind11.readthedocs.io/en/stable/advanced/functions.html)).
- **What happened:** issue #1132, opened 2017-10-09, said accepting a `unique_ptr<T>` from Python *"is not presently possible"* ([#1132](https://github.com/pybind/pybind11/issues/1132)). 3.0.0, on 2025-07-10, shipped *"disowning a Python object being passed to `std::unique_ptr<T>`"* ([changelog](https://pybind11.readthedocs.io/en/latest/changelog.html)). **A transfer with no spelling cost that binding system about eight years.**

**OpenSSL and BoringSSL. Verified.**
- OpenSSL: *"In the 0 version the ownership of the object is passed to … the parent object … In the 1 version … a copy or "up ref" of the object is performed"* ([OpenSSL guide](https://docs.openssl.org/3.6/man7/ossl-guide-libraries-introduction/)).
- BoringSSL: *"These names originally referred to the effect on a reference count"*, and ***"Callers should also take note of whether the function is documented to transfer pointers unconditionally or only on success. Unlike C++ and Rust, functions in BoringSSL typically only transfer on success"*** ([BoringSSL](https://boringssl.googlesource.com/boringssl/+/HEAD/API-CONVENTIONS.md)).

**cJSON. Verified.**
- `add_item_to_object` returns `false` on NULL arguments or `object == item`, and does not free the item ([cJSON.c](https://raw.githubusercontent.com/DaveGamble/cJSON/master/cJSON.c)).
- The README says *"Adding it to an array or object transfers its ownership"*.
- **Its own example adds `resolutions` to `monitor` and then keeps adding items into `resolutions`** ([README](https://raw.githubusercontent.com/DaveGamble/cJSON/master/README.md)).

**json-c. Verified.** `json_object_object_add`: *"The reference count of `val` will *not* be incremented, in effect transferring ownership"* ([json_object.h](https://raw.githubusercontent.com/json-c/json-c/master/json_object.h)). What happens to `val` on failure is not documented there.

**SQLite. Verified.** The destructor *"is called to dispose of the BLOB or string even if the call to the bind API fails"* ([bind_blob](https://www.sqlite.org/c3ref/bind_blob.html)). This transfer is unconditional.

**talloc. Verified.**
- `talloc_steal()` *"changes the parent context"*, and `talloc_free()` frees *"all its children"*.
- *"From version 2.0 and onwards … `talloc_free()` is refused on pointers that have more than one parent"* ([talloc](https://talloc.samba.org/talloc/doc/html/group__talloc.html)).

**The pattern, with its sources, and the departure it explains.**
- **One consumer word.** In Swift, GI, Vala, CF and SWIG the releaser belongs to the type, and one consumer word does both jobs.
- **Two consumer words.** Clang keys releasers per allocation family and needed two: takes and holds.
- **Heroes' choice.** Heroes refused the type key on purpose: ROADMAP row 56, M-cleanup-verdict, *"a releaser keyed on the TYPE refused to Part 6, because ownership is a property of the CALL"* (`docs/ROADMAP.md:142`). So the second consumer word is the price of a deliberate departure, and Clang paid the same price.

### 2. Reference counts at a boundary

- **GLib. Verified.** `g_object_ref` returns *"The same `object`"* ([docs](https://docs.gtk.org/gobject/method.Object.ref.html)). Its count lives inside the object. Over-release shows up as `g_object_unref` / `g_object_ref` assertion criticals (verified-weak, issue titles: [Mozilla 704032](https://bugzilla.mozilla.org/show_bug.cgi?id=704032), [Homebrew #38138](https://github.com/Homebrew/homebrew-core/issues/38138)).
- **CoreFoundation.** `CFRetain`'s return value is **UNVERIFIED**.
- **Objective-C runtime: a count per address. Verified.** `typedef objc::DenseMap<DisguisedPtr<objc_object>,size_t,true> RefcountMap;` sits inside `SideTable` beside the weak table ([NSObject.mm](https://github.com/opensource-apple/objc4/blob/master/runtime/NSObject.mm)). On mismatch it prints *"overreleased while already deallocating; break on objc_overrelease_during_dealloc_error"* (verified-weak, [WeScan #116](https://github.com/WeTransfer/WeScan/issues/116)).
- **Linux `refcount_t`. Verified.**
  - It was *"introduced for 4.11"*, with overflow and underflow checks (Corbet, 2017-03-29, [LWN](https://lwn.net/Articles/718275/)).
  - On a bad event it saturates and warns: `"addition on 0; use-after-free"`, `"underflow; use-after-free"`, `"saturated; leaking memory"` ([refcount.c](https://raw.githubusercontent.com/torvalds/linux/master/lib/refcount.c)).
  - The header's rationale: saturating *"avoids wrapping the counter and causing 'spurious' use-after-free issues"* ([refcount.h](https://raw.githubusercontent.com/torvalds/linux/master/include/linux/refcount.h)).
  - That `refcount_inc` is the call that emits "addition on 0" is my reading of the enum name `REFCOUNT_ADD_UAF`: verified-weak.
- **No system I found separates "begins a life" from "adds a reference" in a function mark.** CF, GI (`full` = *"owns a ref"*) and OpenSSL ("up ref") all count one unit. Whether something is counted is declared on the type (Vala, Swift).

### 3. Two declarations of one function

- **C. Verified.**
  - *"If two declarations refer to the same object or function and do not use compatible types, the behavior of the program is undefined"*, and this applies across translation units.
  - Compatible declarations merge into a composite type (C17 6.2.7, [cppreference](https://en.cppreference.com/c/language/type)).
  - Same scope: *"All declarations in the same scope that refer to the same object or function shall specify compatible types"* (verified-weak, [shape-of-code 6.7](https://c0x.shape-of-code.com/6.7.html)). **UNVERIFIED** that it is a constraint.
  - Parameter lists *"shall agree in the number of parameters and in use of the ellipsis terminator"* (verified-weak, [cppreference](https://en.cppreference.com/c/language/compatible_type)).
- **C++ `[[noreturn]]`. Verified.** The first declaration must carry it. Declared with it in one translation unit and without it in another, the program is *"ill-formed, no diagnostic required"* ([eel.is](https://eel.is/c++draft/dcl.attr.noreturn)).
- **Clang `noescape`. Verified.**
  - The mark is part of the function type: `void noescapeFunc2(__attribute__((noescape)) int *);` followed by `void noescapeFunc2(int *);` gives `conflicting types for 'noescapeFunc2'` ([noescape.mm](https://raw.githubusercontent.com/llvm/llvm-project/main/clang/test/SemaObjCXX/noescape.mm)).
  - It landed as r313722 on 2017-09-19 ([D32210](https://reviews.llvm.org/D32210)).
  - It was reverted because `tsan_libdispatch_mac.cc` *"cannot be compiled because some of the functions declared in the file do not match the ones in the SDK headers (which are annotated with 'noescape')"* ([revert](https://github.com/llvm-mirror/clang/commit/8385a295e2d9a3f7ea56000826056d669d562dab)).
  - **The revert's own date is UNVERIFIED by this seat.**
- **ARC. Verified.** A block pointer redeclared without `ns_returns_retained` is `redefinition … with a different type` ([test](https://github.com/microsoft/clang/blob/master/test/SemaObjC/attr-ns_returns_retained.m)). A dynamic dispatch whose `ns_consumed` set differs from the static one is undefined behaviour ([ARC](https://clang.llvm.org/docs/AutomaticReferenceCounting.html)).
- **Rust `clashing_extern_declarations`. Verified.**
  - Issue #69390 (2020-02-23) was a miscompilation from exactly this shape ([#69390](https://github.com/rust-lang/rust/issues/69390)).
  - PR #70946 was opened 2020-04-09 and merged 2020-06-21. The lint runs *"within a single crate"*, and *"does not run between crates because a project may have dependencies which both rely on the same extern function, but declare it in a different (but valid) way"* ([PR](https://github.com/rust-lang/rust/pull/70946), [lint](https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html)).
  - *"1.46 received a beta backport simply allowing this lint by default"*, and 1.47 had it on (Mark-Simulacrum, 2020-08-24, [#75885](https://github.com/rust-lang/rust/pull/75885)).
  - False negatives were still being fixed on 2024-09-13 ([#130301](https://github.com/rust-lang/rust/pull/130301)).
- **cffi. Verified.** `if prevobj is obj and prevquals == quals: return  # ignore identical double declarations`; otherwise `FFIError("multiple declarations of …")`, unless `override=True` ([cparser.py](https://raw.githubusercontent.com/python-cffi/cffi/main/src/cffi/cparser.py)). The check is per `FFI` instance.
- **GCC and Clang across units. Verified.** GCC's `-Wlto-type-mismatch` warns about *"type mismatches between duplicate global declarations found in different compilation units"*, only under LTO (Clifton, 2015-06-22, [gcc list](https://gcc.gnu.org/legacy-ml/gcc/2015-06/msg00199.html)). Clang has no equivalent: [#56487](https://github.com/llvm/llvm-project/issues/56487), open since 2022-07-12.
- **Variadics. Verified.** cgo: *"Calling variadic C functions is not supported"* ([cgo](https://pkg.go.dev/cmd/cgo)). **So Heroes' one legal repeat, `printf` at two fixed arities, is a case C itself calls incompatible. It has no precedent I found.**
- **A binding generator that refuses two bindings differing only in ownership: none found.** Searched: cffi redeclaration, `clashing_extern_declarations`, noescape and `ns_returns_retained` redeclaration, GCC dealloc merging. The closest is Clang, where the ownership mark is part of the type. GI avoids the question by construction: one description per namespace.

### 4. One call, a contract chosen by an argument

- **SQLite. Verified.** It offers three options, `#define SQLITE_STATIC ((sqlite3_destructor_type)0)` and `#define SQLITE_TRANSIENT ((sqlite3_destructor_type)-1)` ([c_static](https://www.sqlite.org/c3ref/c_static.html)).
- **CPython. Verified.** `bind_param` passes `SQLITE_TRANSIENT` for both text and blob ([cursor.c](https://raw.githubusercontent.com/python/cpython/main/Modules/_sqlite/cursor.c)).
- **rusqlite. Verified.** It uses `SQLITE_TRANSIENT()`, and `SQLITE_STATIC()` only for `""`, commented *"Return a pointer guaranteed to live forever"* ([lib.rs](https://raw.githubusercontent.com/rusqlite/rusqlite/master/src/lib.rs)).
- **mattn/go-sqlite3. Verified.** C helpers hard-code `SQLITE_TRANSIENT` ([sqlite3.go](https://raw.githubusercontent.com/mattn/go-sqlite3/master/sqlite3.go)). **UNVERIFIED:** why it goes through a C helper.
- **None of the three lets its user choose the mode, and none exposes both modes through one function.** That is a claim about three bindings, not about the world.

---

## What the precedents predict, variant by variant

**V1, `transfers`.**
- **The shape has precedent:** Clang's `holds` beside `takes`, which is what a per-family releaser key forced.
- **Task 2, the wrong closer.** `fclose` stays a `consumes` outside `popen`'s set, so it aborts before C runs. Vala's #645 shows the bug ships silently otherwise.
- **Risk 1, the transfer that checks nothing.** V1 as worded checks nothing on a transfer, so an author meeting route A's abort can relabel `fclose` as `transfers` and the abort disappears. SWIG shows the two words collapsing when nothing checks. Clang does not allow it: a `holds` of the wrong family is still a mismatched deallocation. That is **V1c**, the route nobody listed: the transfer names the releaser it hands the life towards, checked against the handle's set before C runs. It costs one mark per transfer function, not V2's producers × consumers.
- **Risk 2, using the child after the transfer.** cJSON's own README uses the child after adding it, and Clang calls held memory legitimate to use. If `transfers` ends the Heroes value as `consumes` does, the README's order is refused and must be reversed. That is a question for the spec seats.
- **Task 1's later delete.** Under V1 the child left the set at the add, so a second delete is caught before C. That matches Clang's "passed more than once" double-free rule.
- **Risk 3, a reference rather than a life.** json-c's add hands over one reference. With R1 in place, *"hands that handle's life"* is false when the program holds two references.
- **Risk 4, transfers only on success.** cJSON and BoringSSL transfer only on success; SQLite transfers unconditionally. *"Owes nothing more"* is then false on the failure path, but in the leak direction, which is the direction the kernel chose on purpose.

**V2, transfers inside the set.**
- This is GCC's shape: its analyzer lists `malloc (freopen, 3)` beside `malloc (fclose)` and tracks *"the expected deallocator_set"* ([sm-malloc.cc](https://raw.githubusercontent.com/gcc-mirror/gcc/master/gcc/analyzer/sm-malloc.cc)).
- **What happened:** glibc's shipped `fopen` carries only `__attr_dealloc_fclose`, and `freopen` is not in the set ([stdio.h](https://raw.githubusercontent.com/bminor/glibc/master/libio/stdio.h)). The largest adopter listed releasers and left out the consumer that is not a plain release.
- **Prediction:** incomplete sets, so a correct transfer aborts. That is a §1.11 failure, at the brief's scale of 12 json-c creators × up to 6 calls and 124 OpenSSL `set0`/`add0`/`push0`/`own0` names.

**V3, `consumes into obj`.**
- This is talloc's shape (`talloc_steal`), and pybind11's `keep_alive<Nurse, Patient>`. It is the only variant with a natural reading of cJSON's README: the child is then lent from `obj`.
- **What happened:** talloc needed a refusal when a pointer has two parents (2.0). V3 combined with R1 meets that at json-c, where a child with a second reference makes *"obj's own release then ends both"* false.
- **Why it is not the first move:** it asks the runtime to model C's containment as a tree, a claim no header states.

**R1, `retains`.**
- The precedents count a created object and a retained one as one unit, and declare counting on the type. Heroes refused the type key at row 56, so R1 moves that declaration onto the function. That is consistent with Heroes' own ruling.
- The kernel's `"addition on 0; use-after-free"` is R1's precondition: a `retains` on an address that is not live should abort before C runs.
- **Prediction:** `getter_wrong_repaired` stays caught, whereas counting every address (panel 175's `runtimeA2`) turns it into a raw double free.
- **Cost warning:** Swift's warning on every unannotated +1/+0 return was disabled in 6.2 for false positives. Requiring R1 everywhere would bring the same noise.

**A2, every declaration agrees.**
- Within the unit a compiler sees whole, precedent is nearly unanimous: Clang `conflicting types`, cffi refuses, Rust warns in a crate. Heroes' checker sees one resolved program, so A2 is that same rule applied to the unit Heroes has.
- The 2017 revert shows where the cost lands: at a second declaration someone else wrote.
- **Rust's exemption between crates** arrives at M-core-packages and M-package-manager (ROADMAP rows 64 and 65, *"bindings instead of a standard library"*).
- **Two-arity `printf`:** A2 must define *"the parameters they share"* so it stays legal (ergonomist task 4).
- **`sqlite3_bind_text`:** A2 removes the one-module-per-mode route, which none of the three surveyed bindings uses. Each one fixes the argument instead.
- **Route nobody listed, for the SQLite case:** a Heroes wrapper that always passes TRANSIENT. **Unrun.**
- **Route nobody listed, for A2 itself:** C's composite type, a merge where one declaration is only more conservative than the other. Listed; I do not recommend it.

---

## `argument` (115 words)

Where the releaser belongs to the type (Swift `consuming`, GI `(transfer full)`, Vala `owned`), one consumer word suffices; Swift defines it as "releasing the parameter or passing ownership of it along somewhere else". Heroes refused the type key at M-cleanup-verdict. Clang's analyzer, which keys releasers per allocation family as Heroes does per call, needed two words, `ownership_takes` and `ownership_holds`, and *holds* still counts toward mismatched deallocation and double free. So V1 has precedent if the transfer stays checked against the handle's releasers; unchecked, `transfers` silences any wrong release renamed as one. R1 matches the kernel's "addition on 0" refusal. A2 is Clang's `conflicting types` rule over a whole program; Rust exempts only separately written crates.

## `condition`: what would change my reading

1. **Drop the V1c condition** if someone finds a shipped system that keys releasers per call or per family, leaves its transfer mark unchecked, and did not see wrong releases relabelled into silence.
2. **Lift the objection to V2** if a major adopter of GCC's `malloc (dealloc)` is shown listing transfer-like consumers in its producers' sets in practice.
3. **Scope A2 to a package rather than a program** if a Heroes program is found with two valid but different declarations of one C function that no single declaration can replace. That is Rust's reason between crates.
4. **Reconsider R1** if a runtime is found that counts references per address with no separate increment mark and does not turn a wrongly marked getter into a raw double free.

## Falsifiable prediction

**The first C function that a Heroes program in `examples/`, or a core package, marks as a transfer will transfer only on success**, as `cJSON_AddItemToObject` does (it returns `false` and leaves the item) and as BoringSSL's conventions say is typical.
- **Consequence:** a V1 or V3 sentence saying the program *"owes nothing more"* will be false for that function on its failure path, in the leak direction.
- **Checkable at:** the first milestone whose `examples/` or packages gain a transfer-marked parameter, plausibly M-core-packages (row 64).
- **Falsified if:** that function's C documentation or source says it takes ownership even when it fails, as SQLite's destructor argument does.

## Named as unverified or unrun

- When Clang's ownership attributes shipped, and whether the analyzer honours them by default (the checker page's sentence is ambiguous).
- Which Swift version introduced `SWIFT_RETURNS_RETAINED`/`SWIFT_RETURNS_UNRETAINED`.
- The date r313722 was reverted.
- That `CommandPipe` was Vala #645's fix.
- `CFRetain`'s return value.
- That `refcount_inc` is what emits "addition on 0".
- That C11 6.7p4 is a constraint.
- When pybind11's `keep_alive` was introduced.
- Why go-sqlite3 uses a C helper.
- Any binding beyond the three surveyed that exposes both STATIC and TRANSIENT.
- Whether GCC merges `malloc (dealloc)` across redeclarations (searched, not found).
- **Unrun:** every Heroes reproducer, and V1c, which is a question rather than a built compiler.

Repository files read: `docs/panel/176-briefs/historian.md`, `docs/panel/176-briefs/00-shared.md`, `docs/panel/176-briefs/llm-ergonomist.md`, `docs/panel/175-a-consuming-call-is-three-things-and-the-runtime-speaks-after-it-listens.md`, `docs/panel/175-reports/historian.md`, `docs/panel/171-reports/historian.md`, `docs/ROADMAP.md`.

Sources:
- [Clang Annotations](https://clang.llvm.org/docs/analyzer/user-docs/Annotations.html) · [Clang checkers](https://clang.llvm.org/docs/analyzer/checkers.html) · [OpenSSF annotations](https://best.openssf.org/Compiler-Hardening-Guides/Compiler-Annotations-for-C-and-C++.html) · [ARC](https://clang.llvm.org/docs/AutomaticReferenceCounting.html) · [ns_returns_retained test](https://github.com/microsoft/clang/blob/master/test/SemaObjC/attr-ns_returns_retained.m) · [noescape.mm](https://raw.githubusercontent.com/llvm/llvm-project/main/clang/test/SemaObjCXX/noescape.mm) · [D32210](https://reviews.llvm.org/D32210) · [revert 8385a29](https://github.com/llvm-mirror/clang/commit/8385a295e2d9a3f7ea56000826056d669d562dab) · [llvm #56487](https://github.com/llvm/llvm-project/issues/56487)
- [CF Ownership Policy](https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFMemoryMgmt/Concepts/Ownership.html) · [objc4 NSObject.mm](https://github.com/opensource-apple/objc4/blob/master/runtime/NSObject.mm) · [WeScan #116](https://github.com/WeTransfer/WeScan/issues/116)
- [GI annotations](https://gi.readthedocs.io/en/latest/annotations/giannotations.html) · [GI changelog](https://gi.readthedocs.io/en/latest/changelog.html) · [GI bindable APIs](https://gi.readthedocs.io/en/latest/writingbindableapis.html) · [g_object_ref](https://docs.gtk.org/gobject/method.Object.ref.html) · [Mozilla 704032](https://bugzilla.mozilla.org/show_bug.cgi?id=704032) · [Homebrew #38138](https://github.com/Homebrew/homebrew-core/issues/38138)
- [Vala ownership](https://docs.vala.dev/guides/bindings/writing-a-vapi-manually/05-00-fundamentals-of-binding-a-c-function/05-02-ownership.html) · [Vala compact classes](https://docs.vala.dev/guides/bindings/writing-a-vapi-manually/04-00-recognizing-vala-semantics-in-c-code/04-05-compact-classes.html) · [Vala #645](https://gitlab.gnome.org/GNOME/vala/-/issues/645) · [#645 API](https://gitlab.gnome.org/api/v4/projects/GNOME%2Fvala/issues/645) · [Posix.CommandPipe](https://valadoc.org/posix/Posix.CommandPipe.html) · [Posix.FILE.popen](https://valadoc.org/posix/Posix.FILE.popen.html)
- [SE-0377](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0377-parameter-ownership-modifiers.md) · [Swift C++ interop](https://www.swift.org/documentation/cxx-interop/) · [Swift PR #82488](https://github.com/swiftlang/swift/pull/82488)
- [Rust Box](https://doc.rust-lang.org/std/boxed/struct.Box.html) · [Rust Rc](https://doc.rust-lang.org/std/rc/struct.Rc.html) · [clashing_extern_declarations](https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html) · [PR #70946](https://github.com/rust-lang/rust/pull/70946) · [#69390](https://github.com/rust-lang/rust/issues/69390) · [#75885](https://github.com/rust-lang/rust/pull/75885) · [#130301](https://github.com/rust-lang/rust/pull/130301)
- [SWIG Customization](https://swig.org/Doc3.0/Customization.html) · [disown.i](https://github.com/swig/swig/blob/master/Examples/test-suite/disown.i) · [pybind11 functions](https://pybind11.readthedocs.io/en/stable/advanced/functions.html) · [pybind11 #1132](https://github.com/pybind/pybind11/issues/1132) · [pybind11 changelog](https://pybind11.readthedocs.io/en/latest/changelog.html) · [cffi cparser.py](https://raw.githubusercontent.com/python-cffi/cffi/main/src/cffi/cparser.py)
- [OpenSSL libraries guide](https://docs.openssl.org/3.6/man7/ossl-guide-libraries-introduction/) · [BoringSSL API conventions](https://boringssl.googlesource.com/boringssl/+/HEAD/API-CONVENTIONS.md) · [cJSON.c](https://raw.githubusercontent.com/DaveGamble/cJSON/master/cJSON.c) · [cJSON README](https://raw.githubusercontent.com/DaveGamble/cJSON/master/README.md) · [json_object.h](https://raw.githubusercontent.com/json-c/json-c/master/json_object.h) · [talloc](https://talloc.samba.org/talloc/doc/html/group__talloc.html)
- [LWN refcount_t](https://lwn.net/Articles/718275/) · [refcount.c](https://raw.githubusercontent.com/torvalds/linux/master/lib/refcount.c) · [refcount.h](https://raw.githubusercontent.com/torvalds/linux/master/include/linux/refcount.h)
- [cppreference compatible type](https://en.cppreference.com/c/language/type) · [cppreference compatible_type](https://en.cppreference.com/c/language/compatible_type) · [shape-of-code 6.7](https://c0x.shape-of-code.com/6.7.html) · [dcl.attr.noreturn](https://eel.is/c++draft/dcl.attr.noreturn) · [GCC 2015-06 update](https://gcc.gnu.org/legacy-ml/gcc/2015-06/msg00199.html) · [GCC sm-malloc.cc](https://raw.githubusercontent.com/gcc-mirror/gcc/master/gcc/analyzer/sm-malloc.cc) · [glibc stdio.h](https://raw.githubusercontent.com/bminor/glibc/master/libio/stdio.h) · [GCC 11.4 attributes](https://gcc.gnu.org/onlinedocs/gcc-11.4.0/gcc/Common-Function-Attributes.html) · [cgo](https://pkg.go.dev/cmd/cgo)
- [SQLite bind_blob](https://www.sqlite.org/c3ref/bind_blob.html) · [SQLite c_static](https://www.sqlite.org/c3ref/c_static.html) · [CPython cursor.c](https://raw.githubusercontent.com/python/cpython/main/Modules/_sqlite/cursor.c) · [rusqlite lib.rs](https://raw.githubusercontent.com/rusqlite/rusqlite/master/src/lib.rs) · [go-sqlite3](https://raw.githubusercontent.com/mattn/go-sqlite3/master/sqlite3.go)
