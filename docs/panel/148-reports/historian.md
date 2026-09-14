# Panel 148 — historian

**Written to disk by the coordinator**, verbatim from the seat's returned
report: this seat has Read, WebSearch and WebFetch and no write tool, and panels
143 and 144 both recorded that it could not be audited for that reason.

## verdict

**approve A** (the explicit word `acquires`). **object to C** — advisory, no veto.

## precedents

Every URL was fetched by the seat on **2026-09-14**.

### 1. Clang ARC — ownership inferred from the method name. The purest shipped option C.

| # | claim | source | status |
|---|---|---|---|
| P1 | ARC infers ownership from the selector: `alloc`/`copy`/`mutableCopy`/`new` *"implicitly return a retained object as if they were annotated with the ns_returns_retained attribute"*; `init` methods *"implicitly consume their self parameter and return a retained object"*. | clang.llvm.org/docs/AutomaticReferenceCounting.html | **verified** (verbatim) |
| P2 | **Cost 1 — the inference needed an escape hatch on day one.** The implicit `ns_returns_retained` *"can be overridden by annotating the method with either of the `ns_returns_autoreleased` or `ns_returns_not_retained` attributes."* And `objc_method_family` exists because *"as these conventions have not heretofore been enforced by an unforgiving mechanical system, they are only imperfectly kept, especially as they haven't always even been precisely defined."* | same | **verified** (verbatim) |
| P3 | **Cost 2 — false positives against shipped APIs.** *"There are a fair number of existing methods with `init`-like selectors which nonetheless don't follow the `init` conventions… Therefore we identify a small number of legitimate-seeming return types and call everything else an error."* | same | **verified** (verbatim) |
| P4 | **Cost 3 — it took a piece of the identifier namespace from every programmer.** *"You cannot give an accessor a name that begins with `new`."* | developer.apple.com — Transitioning to ARC Release Notes | **verified** (verbatim) |
| P5 | **Cost 4 — where the name is not statically known it degrades to a warning**: `-Warc-performSelector-leaks`, *"performSelector may cause a leak because its selector is unknown"*. | clang.llvm.org/docs/DiagnosticsReference.html | **verified** |
| P6 | Why `CF_RETURNS_RETAINED` had to exist, in Apple's own header comment: *"Marks functions which return a CF type that needs to be released by the caller but whose names are not consistent with CoreFoundation naming rules. The recommended fix to this is to rename the functions… This macro is ONLY to be used in exceptional circumstances."* | Apple `CFBase.h`, third-party mirror | **verified as text, secondary hosting** |
| P7 | The rule it patches is pure name inference: *"if a function name contains the word 'Create' or 'Copy', you own the object. If a function name contains the word 'Get', you do not own the object."* | developer.apple.com — CF Ownership Policy | **verified** (verbatim) |
| P8 | The analyzer's own statement of the fallback, and *"the analyzer cannot always tell that a pointer return value refers to a Core Foundation object."* | clang.llvm.org/docs/analyzer/user-docs/Annotations.html | **verified** |
| P9 | Swift's CF importer: annotation switched on per-region by `CF_IMPLICIT_BRIDGING_ENABLED` → `_Pragma("clang arc_cf_code_audited begin")`. | same CFBase.h fetch | **pragma verified; the "unannotated ⇒ Unmanaged" consequence is secondary and dropped** |

**What happened to ARC, in one line:** the inference shipped, was kept, and is still shipping fifteen years later — *and it never removed the annotation.* It relocated it to the exceptions, then had to add four attributes, a fifth (`objc_method_family`), a namespace prohibition and a warning class to carry them.

### 2. Other inferred-obligation systems, and their default

| # | claim | source | status |
|---|---|---|---|
| P10 | **gobject-introspection** infers from *position and const-ness*: returns and `(out)` default to `(transfer full)`; `(in)` to `(transfer none)`. | gi.readthedocs.io | **verified** |
| P11 | **Vala** does the same in hand-written VAPIs and states the failure mode: *"If ownership semantics are not correct, either a memory leak has been written or a double-free has been written."* | docs.vala.dev | **verified** (verbatim) |
| P12 | **SWIG** went the other way and says why: without `%newobject`, *"SWIG has no idea that the return value is a newly allocated object… SWIG is conservative and will never delete objects unless it knows for certain that the returned object was newly created."* Explicit mark, default = no obligation, default = leak. | swig.org/Doc4.0/Customization.html | **verified** (verbatim) |
| P13 | **Go** hardcodes one inference by name: `lostcancel` knows `context.WithCancel`, `WithTimeout`, `WithDeadline`, `WithCancelCause`. | pkg.go.dev — lostcancel | **verified** |
| P14 | And the Go team's own verdict on generalising it. Alan Donovan, proposal #65682 (12 Feb 2024, open): *"The name is not a reliable clue, and many functions don't name their result variables"*; *"the hardest part of the problem is reliably identifying which functions are cleanup functions."* His fix is an explicit registration. | github.com/golang/go/issues/65682 | **verified** |
| P15 | **Java** infers from the type (`AutoCloseable` + try-with-resources) — the shape panel 147 refused. Google shipped an explicit annotation on top anyway: `@MustBeClosed`. | errorprone.info | **verified** |

### 3. Explicit-mark systems, and what shape the mark took

| # | claim | source | status |
|---|---|---|---|
| P16 | **GCC >= 11** puts the mark on the ALLOCATING function and has it *name its releaser*: `__attribute__ ((malloc, malloc (fclose, 1))) FILE* fopen(const char*, const char*);`. It buys `-Wanalyzer-mismatching-deallocation`, `-Wanalyzer-double-free`, `-Wanalyzer-malloc-leak`. | gcc.gnu.org — Common Function Attributes | **verified** (verbatim + example) |
| P17 | **Splint/LCLint** used one word on both sides, direction read off position: `/*@only@*/ void *malloc(size_t)` and `void free(/*@only@*/ void *ptr)`, with *"There is nothing special about malloc and free — their behavior can be described entirely in terms of the provided annotations."* | splint.org/manual/html/sec5.html | **verified** (verbatim) |
| P18 | What happened to Splint: last release **3.1.2, 12 July 2007**. The technique was right; the tool died of unmaintenance, not of a design fault. | splint.org | **verified** |

### 4. The word itself

| # | claim | source | status |
|---|---|---|---|
| P19 | **Swift SE-0377** is the only shipped deliberation on exactly this naming question found. Final pair `consuming`/`borrowing`, shipped in Swift 5.9; earlier `__owned`/`__shared` and `take`/`taking`. Verbatim: *"We think it is better to describe `borrowing` in terms of what it means, rather than as the opposite of the other convention"*; and *"`consume` reads about as well while being more specific."* | swift-evolution SE-0377 | **verified** (verbatim) |
| P20 | **Hylo** uses four short, deliberately non-symmetric words — `let`, `inout`, `sink`, `set`. | docs.hylo-lang.org | **verified** |
| P21 | **ARC's own pair is asymmetric** (`ns_returns_retained` names a state, `ns_consumed` an action). **SWIG's is symmetric** (`%newobject`/`%delobject`). | clang + swig | **verified** |

**Tally for item 3:** of the eight shipped systems, three mark only one side; where two words exist they are either asymmetric by construction (ARC) or a deliberate *non-negation* pair (Swift, which argued explicitly against Vala's `owned`/`unowned` negation). `acquires`/`consumes` — two third-person verbs, each describing what it means rather than negating the other — lands in the Swift camp and has precedent. **No shipped precedent found for a rhyming/symmetric pair on a C boundary.**

### 5. Item 2 — the measured silence

Searched, and in each case the answer is **no compiler-inferred foreign-resource obligation**: **Zig** (ownership is a doc-comment convention; enforcement is runtime, `DebugAllocator` with leak detection — verified), **Hare** (*"We use terms like 'borrow' and 'ownership' to reason about memory, but this is not enforced at the language level"* — verified verbatim), **Odin** (runtime tracking allocator — secondary, dropped), **Go** (P13/P14), **Austral** (secondary, dropped), **Vala** (P11, the one that does infer — positionally, with leak-or-double-free as the documented penalty).

### 6. The enforcement half

| # | claim | source | status |
|---|---|---|---|
| P22 | *"A handle nobody consumes aborts when `main` returns, saying how many"* is a shipped shape: LeakSanitizer prints `SUMMARY: AddressSanitizer: 7 byte(s) leaked in 1 allocation(s).` and the process terminates when `exitcode` is non-zero. | clang.llvm.org/docs/LeakSanitizer.html | **verified** |

## The measurement made in the tree, and the one that could not be

Hand-counted from `examples/ledger/db/sqlite.hero:64-101` — **no Bash in this seat, so `grep -c` is unrun**:

- **18** `function` declarations in the group.
- **Handle producers: 2**, both `@`-out-parameters.
- **Functions returning `CDb` or `CStmt` as a result: 0.**

**So on today's shipped SQLite binding, A and C agree on 18 of 18.** Reported because it is the strongest thing anyone can say for C.

Two further in-tree facts, read rather than inferred:

- `sqlite3_close(db: CDb)` is **not** marked `consumes`, so C's trigger is not even present in the shipped SQLite group today.
- **The repository has already chosen A's shape once.** Line 84 reads `@error: cstr owned sqlite3_free` — an explicit mark on the acquiring parameter, naming the releaser, exactly GCC's `malloc(deallocator)` shape (P16). The group also declares `sqlite3_free` and `sqlite3_errmsg(db: CDb) -> cstr`. Under a group-scoped inference of C's form, `sqlite3_free` plus a returned `cstr` would make `sqlite3_errmsg` acquire — and SQLite says it must not: *"Memory to hold the error message string is managed internally. The application does not need to worry about freeing the result."* (sqlite.org/c3ref/errcode.html, verified verbatim). One type-class over from C's stated scope, so an **analogy, not a direct refutation** — but the same rule failing on the same page of the same header.

## argument

Inference by name or position ships and survives — ARC, Core Foundation, Vala, g-i all do it. None of them *removed* the annotation; each relocated it to the exceptions, then paid for the exceptions: four attributes, a namespace prohibition, a warning where the name is unknown, and a header comment begging you to rename your functions instead. Heroes has already made this choice once, correctly: `owned sqlite3_free` is written on the acquiring parameter, not inferred from the group. The eight-token saving C offers buys a rule whose first counterexample is two lines further down the same SQLite header. Take A, and spell the escape hatch never.

## falsifiable prediction

**The next handle-returning SQLite function the corpus binds will be a borrowing one.** `sqlite3_db_handle` *"returns the database connection handle to which a prepared statement belongs"* and `sqlite3_next_stmt` returns statements *"associated with the database connection pDb"* — both fetched 2026-09-14, verified verbatim. Under C each is inferred to acquire, so a connection `sqlite3_open` already acquired is owed a **second** close, and n already-prepared statements are owed n extra finalizes: a double-close, not a leak. Under A both are silent. **The falsifier**: show that Heroes will never bind a foreign function returning a handle it does not own. (Neither is in the tree today — that is the prediction, not a present defect.)

## A third option nobody listed, with two precedents

`acquires <releaser>` — the mark carries the name of the consuming function, exactly GCC's `__attribute__((malloc (fclose, 1)))` (P16) and exactly the `owned sqlite3_free` Heroes already ships. It stays inside panel 147's ruling (keyed on the *call*, and the compiler still never *picks* the release — it only checks the one you named) and it buys GCC's `-Wanalyzer-mismatching-deallocation` class: closing a `CStmt` with `sqlite3_close`. **Its spec cost is unmeasured — no shell in this seat.** Estimate only: more than A's +64.

## condition

1. **A shipped system that infers a foreign-resource obligation from the presence of a releaser in the same module or group — not from a name, not a position, not the type — and that has run for years without adding an override keyword.** Searched and not found; terms named. A negative from one searcher's vocabulary, so: **name one and I withdraw.**
2. **A Heroes-side rule that makes the borrowing return unrepresentable**, so the prediction has no subject.
3. **A measurement that A costs more than +64 once CL-036's formatter walk is counted.**

**Dropped as unsourced or secondary:** `-Wobjc-property-matches-cocoa-ownership-rule` (unverified); "unannotated CF APIs import as `Unmanaged`" (secondary); Odin's tracking allocator (secondary); Austral's FFI trust boundary (secondary); Nim's `=destroy` (unsourced, and type-keyed anyway). One search result looked exactly like the wanted evidence and **for that reason was not used**, the seat being unable to establish that the repository or issue is real. Inadmissible.
