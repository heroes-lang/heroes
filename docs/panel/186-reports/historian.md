# Panel 186, the historian's report (advisory, no veto)

Written 2026-10-02 as the work went. Evidence is the web only: every precedent
below carries its URL and the version or date the source describes, and is
marked **verified** (read in this session from the source named, a quotation
in double quotes being that source's words) or **unverified** (found only in a
search engine's summary, or inferred). No build, no copy of the tree, no paid
run. *Heroes of code* was not used as a route this sitting: the rooms here are
post-2000 binding tools, and every one was reached from its own repository or
documentation.

**Cost**: about 85 web calls (31 searches, about 55 fetches), counted from this
session's transcript; nothing else.

## 1. Who bound a C struct holding an anonymous union, and how

### rust-bindgen: a synthetic NAMED field, nested

- **verified**. Header `struct foo { union { unsigned int a; unsigned short b; }; };`
  ([bindgen-tests/tests/headers/struct_with_anon_unnamed_union.h](https://raw.githubusercontent.com/rust-lang/rust-bindgen/main/bindgen-tests/tests/headers/struct_with_anon_unnamed_union.h),
  `main`, read 2026-10-02) becomes `pub struct foo { pub __bindgen_anon_1: foo__bindgen_ty_1 }`
  with `pub union foo__bindgen_ty_1 { pub a: c_uint, pub b: c_ushort }`
  ([expectation](https://raw.githubusercontent.com/rust-lang/rust-bindgen/main/bindgen-tests/tests/expectations/tests/struct_with_anon_unnamed_union.rs)).
  So a reader writes `s.__bindgen_anon_1.a`; the anonymous member is given a
  name and reached through it.
- **verified**, the layout is read from clang and checked by the target compiler,
  **one assertion per field, linear**: `["Offset of field: foo__bindgen_ty_1::a"][::std::mem::offset_of!(foo__bindgen_ty_1, a) - 0usize];`
  and the same for `b`, plus size and alignment per type (same expectation file).
- **verified**, `==` and `hash` are withheld from a struct holding an anonymous
  union **even when asked for**: header `derive-partialeq-anonfield.h` carries
  `// bindgen-flags: --with-derive-hash --with-derive-partialeq --with-derive-eq --impl-partialeq`
  over DPDK's `struct rte_mbuf { union {}; }`, and the expectation derives only
  `Copy, Clone` on both the struct and its union, with no `impl PartialEq`
  ([header](https://raw.githubusercontent.com/rust-lang/rust-bindgen/main/bindgen-tests/tests/headers/derive-partialeq-anonfield.h),
  [expectation](https://raw.githubusercontent.com/rust-lang/rust-bindgen/main/bindgen-tests/tests/expectations/tests/derive-partialeq-anonfield.rs)).
  The union alone: `/// Deriving PartialEq for rust unions is not supported.`
  ([derive-partialeq-union.hpp](https://raw.githubusercontent.com/rust-lang/rust-bindgen/main/bindgen-tests/tests/headers/derive-partialeq-union.hpp)).
- **verified**, the pessimism is transitive and documented: *"Blocklisted types
  are pessimistically assumed not to be able to `derive` any traits, which can
  transitively affect other types' ability to `derive` traits or not."*
  ([bindgen book, Blocklisting](https://rust-lang.github.io/rust-bindgen/blocklisting.html), read 2026-10-02).
- **verified**, Rust itself refuses: `tests/ui/union/union-derive.stderr` reports
  *"this trait cannot be derived for unions"* for `PartialEq`, `PartialOrd`,
  `Ord`, `Hash`, `Default`, `Debug`
  ([rust stable branch](https://rust.googlesource.com/rust/+/refs/heads/stable/tests/ui/union/union-derive.stderr),
  read 2026-10-02; since which release, unverified).

### rust-bindgen's bit-fields: storage units plus generated accessors

- **verified**. `struct bitfield { unsigned short a:1, b:1, c:1, :1, :2, d:2; int e; unsigned int f:2; unsigned int g:32; }`
  becomes `_bitfield_1: __BindgenBitfieldUnit<[u8; 1usize]>`, `e`,
  `_bitfield_2: __BindgenBitfieldUnit<[u8; 8usize]>`, with getters `a()`,
  setters `set_a()`, raw forms `a_raw()`/`set_a_raw()`, and
  `new_bitfield_1(a, b, c, d)`
  ([header](https://raw.githubusercontent.com/rust-lang/rust-bindgen/main/bindgen-tests/tests/headers/struct_with_bitfields.h),
  [expectation](https://raw.githubusercontent.com/rust-lang/rust-bindgen/main/bindgen-tests/tests/expectations/tests/struct_with_bitfields.rs)).
  bindgen computes the bit placement itself from clang's offsets.
- **verified (subject and date), detail unverified**: what that cost on a real
  header. The Linux kernel made `alt_instr` an opaque type for bindgen,
  *"[PATCH 1/1] rust: bindgen: Add `alt_instr` as opaque type"*, reply dated
  2023-03-02 ([lkml](https://lkml.iu.edu/hypermail/linux/kernel/2303.0/01794.html)),
  after the error *"packed type cannot transitively contain a `#[repr(align)]` type"*
  ([lkml 2022-12](https://lkml.iu.edu/hypermail/linux/kernel/2212.3/01484.html), title read in search only).
  That is a user routing around a binder's own bit layout by making the type opaque.

### Rust's own unnamed fields, RFC 2102: accepted 2017, retired 2024

- **verified**. RFC 2102, start date 2017-08-05, issue rust-lang/rust#49804:
  *"Allow unnamed fields of `struct` and `union` type, contained within an outer
  struct or union; the fields they contain appear directly within the containing
  structure"* ([RFC text](https://raw.githubusercontent.com/rust-lang/rfcs/master/text/2102-unnamed-fields.md)).
  Its instantiation examples for `S { a, _: union { b, _: struct { c, d }, e }, f }`
  are `S { a: 1, b: 2, f: 3.0 }`, `S { a: 1, c: 2, d: 3.0, f: 4.0 }`,
  `S { a: 1, e: 2.0, f: 3.0 }`: one arm per union, named flat. **The RFC states
  no rule sentence for it** (searched the text for one, absent): the rule lives
  only in its examples.
- **verified**. Its motivation is the cost of nesting: Rust *"cannot represent
  C's unnamed struct and union fields without artificial naming"*, which forces
  *"binding generators (and the authors of manual bindings) to invent new names"*
  (same text). `__bindgen_anon_1` is that cost.
- **verified**. The implementation was **removed**: PR rust-lang/rust#131045,
  *"Retire the `unnamed_fields` feature for now"*, by compiler-errors, merged
  2024-10-11, because *"it represents a compiler implementation burden including
  a new kind of anonymous ADT and additional complication to field selection, and
  is quite prone to bugs today"*, fixing eight ICE issues
  ([PR](https://github.com/rust-lang/rust/pull/131045)). `REMOVED_FEATURES`
  records `(removed, unnamed_fields, "1.83.0", Some(49804), Some("feature needs redesign"), 131045)`
  ([rustc_feature/removed.rs](https://doc.rust-lang.org/beta/nightly-rustc/src/rustc_feature/removed.rs.html)).
  The tracking issue is open with `S-tracking-design-concerns`: *"adds a
  fundamentally new 'kind of type' to the language that's a hybrid of a struct
  and a union"* ([#49804](https://github.com/rust-lang/rust/issues/49804), read 2026-10-02).

### cgo: a union is a byte array, a bit-field is padding, since 2009

- **verified**, the documentation today (go1.27.1): *"As Go doesn't have
  support for C's union type in the general case, C's union types are
  represented as a Go byte array with the same length."* and *"C struct fields
  that cannot be expressed in Go, such as bit fields or misaligned data, are
  omitted in the Go struct, replaced by appropriate padding to reach the next
  field or the end of the struct."* ([pkg.go.dev/cmd/cgo](https://pkg.go.dev/cmd/cgo)).
- **verified**, cgo learns layout **from the C compiler**, not by itself: the
  first public weekly's `gcc.go` opens *"Annotate Crefs in Prog with C types by
  parsing gcc debug output"*, runs `gcc -gdwarf-2`, and its `typeConv` is *"a
  translator from dwarf types to Go types with equivalent memory layout"*; a
  DWARF union becomes `c.Opaque(t.Size)` ([weekly.2009-11-10 gcc.go](https://go.googlesource.com/go/+/weekly.2009-11-10/src/cmd/cgo/gcc.go)).
- **verified**, `-godefs` promotes **only the first member** of an anonymous
  union: *"when cgo -godefs sees a struct with a field that is an anonymous
  union, the first field in the union is promoted to become a field of the
  struct. See issue 6677 for background."* (test file, copyright 2014,
  [anonunion.go](https://golang.google.cn/src/cmd/cgo/internal/testgodefs/testdata/anonunion.go)).
  Issue 6677 (2013-10-28) reports `Rusage` losing its named fields to
  `Anon0` through `Anon13` byte arrays ([#6677](https://github.com/golang/go/issues/6677)).
- **verified**, bit-fields went from translated to omitted because cgo's own
  translation was wrong: Go 1.16, *"The cgo tool will no longer try to translate
  C struct bitfields into Go struct fields, even if their size can be represented
  in Go. The order in which C bitfields appear in memory is implementation
  dependent, so in some cases the cgo tool produced results that were silently
  incorrect."* ([go1.16 notes](https://go.dev/doc/go1.16); release date not on
  the page, unverified); commit *"cmd/cgo: don't translate bitfields into Go
  fields"*, Ian Lance Taylor, 2020-09-16 ([eaa97fbf](https://go.googlesource.com/go/+/eaa97fbf20baffac713ed1b780f864a6fee54ab6)).
- **verified, what the users did** (panel 073's historian's Go precedent, now
  sourced twice):
  - `golang.org/x/sys/unix` rewrites the C struct by hand in its preamble:
    *"The real epoll_event is a union, and godefs doesn't handle it well."*
    over `struct my_epoll_event`, and *"collapsed the unions, to avoid confusing
    godoc for the generated output"* over `struct perf_event_attr_go`, whose
    `union { sample_period; sample_freq; }` is written as one `__u64 sample`
    ([unix/linux/types.go](https://raw.githubusercontent.com/golang/sys/master/unix/linux/types.go), `master`;
    the epoll comment is already in commit 405a2f21, Tobias Klauser, 2019-11-12,
    [diff](https://go.googlesource.com/go/+/405a2f2161b6a12965e7a91bfe5d14b626e176fb%5E%21)).
  - Xen's Go bindings construct a union by filling a C struct defined in the
    preamble and copying its bytes into the Go byte array:
    *"[PATCH v4 3/6] golang/xenlight: implement keyed union Go to C marshaling"*,
    Nick Rosbrook, 2019-12-23 ([xen-devel](https://old-list-archives.xen.org/archives/html/xen-devel/2019-12/msg01910.html)).
  - and the byte array defeated cgo's own pointer check:
    *"cmd/cgo: missing pointer check for struct passed through a union type"*,
    opened 2016-06-02, labels NeedsFix ([#15942](https://github.com/golang/go/issues/15942));
    whether it was ever fixed, unverified.

### Zig: translate-c names the anonymous member `unnamed_N`; bit-fields make the struct opaque

- **verified**, Zig 0.13.0's translate-c test: `typedef struct { union { char x; struct { int y; }; }; } outer;`
  becomes `pub const outer = extern struct { unnamed_0: union_unnamed_1 = ... }`
  with `const union_unnamed_1 = extern union { x: u8, unnamed_0: struct_unnamed_2 }`,
  and the C body `x->y = x->x` becomes `x.*.unnamed_0.unnamed_0.y = ... x.*.unnamed_0.x`
  ([test/translate_c.zig at 0.13.0](https://raw.githubusercontent.com/ziglang/zig/0.13.0/test/translate_c.zig)).
  Source: `if (field_decl.isAnonymousStructOrUnion() or field_name.len == 0) { ... "unnamed_{d}" ...}`
  ([src/translate_c.zig at 0.13.0](https://raw.githubusercontent.com/ziglang/zig/0.13.0/src/translate_c.zig)):
  **the fact "this member is an anonymous union" is read from clang's AST.**
- **verified**, a bit-field anywhere makes the whole record opaque:
  `if (field_decl.isBitField()) { ... warn(c, scope, field_loc, "{s} demoted to opaque type - has bitfield", ...)`
  (same file). The issue *"translate-c: support C bitfields"* was opened
  2018-09-11 and is **open** today, migrated to Codeberg
  ([translate-c#179](https://codeberg.org/ziglang/translate-c/issues/179), read 2026-10-02);
  its comments say users cannot reach libraries such as Graphviz without C
  wrapper functions, and that musl's `timespec` breaks translation (summary of
  the thread, not quoted, so **unverified in wording**). A forum thread dated
  2026-08-28, *"There has got to be a better way to deal with bitfields in C"*,
  lists the routes users take: hand-written packed structs, C helper functions
  ([ziggit](https://ziggit.dev/t/there-has-got-to-be-a-better-way-to-deal-with-bitfields-in-c/17399)).
  **Eight years of refusal, and the users routed around it.**
- **verified**, union construction names exactly one member, two is an error:
  *":14:20: error: cannot initialize multiple union fields at once; unions can
  only have one active field"*, *":14:31: note: additional initializer here"*,
  *":1:12: note: union declared here"*, and zero fields is *"union initializer
  must initialize one field"*
  ([compile_errors/union_init_with_none_or_multiple_fields.zig at 0.13.0](https://raw.githubusercontent.com/ziglang/zig/0.13.0/test/cases/compile_errors/union_init_with_none_or_multiple_fields.zig)).

### Swift's ClangImporter: anonymous members read flat, constructed through the union's own one-member initializers

- **verified**, Apple's current article ([Using Imported C Structs and Unions in Swift](https://developer.apple.com/documentation/swift/using-imported-c-structs-and-unions-in-swift.md), read 2026-10-02):
  - a C union imports as a struct whose members are computed properties over
    one storage, with **one initializer per member**:
    `init(isAlive: Bool)`, `init(isDead: Bool)`, `init()`. There is no
    initializer taking two.
  - *"Swift imports bit fields that are declared in structures, like those found
    in Foundation's `NSDecimal` type, as computed properties."*
  - `struct Cake { union { int layers; double height; }; struct { bool icing; bool sprinkles; } toppings; };`
    is read flat, `simpleCake.layers = 5`, and constructed with the union as an
    **unlabelled** first argument naming one member:
    `Cake(.init(layers: 2), toppings: .init(icing: true, sprinkles: false))`;
    *"Because the first field of the `Cake` structure is unnamed, its
    initializer's first parameter doesn't have a label."*
- **verified (PR text via summary)**: the importer sees these members as clang's
  `IndirectFieldDecl`, *"Fields of anonymous unions are injected to the parent as
  IndirectFieldDecl"* ([swiftlang/swift#83973](https://github.com/swiftlang/swift/pull/83973)).
  The Swift version that first imported anonymous members: **unverified**.

### D: anonymous unions native, the struct literal names one member flat

- **verified**, D specification ([dlang.org/spec/struct.html](https://dlang.org/spec/struct.html), read 2026-10-02, version not stated):
  *"Unions are initialized similarly to structs, except that only one member
  initializer is allowed."*, with the example ``U w = { 2, 3 };    // error: overlapping initialization for field `a` and `b` ``;
  and for a struct: *"If there is a union field in the struct, only one member
  of the union can be initialized inside a struct literal."*; *"An anonymous
  union is useful inside a class or struct to share memory for fields, without
  having to name a parent field with a separate union type."*
- **verified**, users meet it: 2017-08-14 forum thread, *"as soon as an anonymous
  union is present, you can't initialize anything further than the first union
  field"* positionally, answered with named initializers `mess m = { i: 99, x: 3.14};`
  ([digitalmars.D.learn](https://digitalmars.com/d/archives/digitalmars/D/learn/Initialization_of_struct_containing_anonymous_union_95514.html)).
- **verified**, bit-fields: *"Add bit fields to D - They work just like the bit
  fields in ImportC do."*, DMD 2.101.0, 2022-11-14
  ([changelog](https://dlang.org/changelog/2.101.0.html)); the spec: *"The
  address of a bit field cannot be taken."*

### Nim: `{.union.}` and `{.bitsize.}` hand the bits to the C compiler

- **verified**, *"The ``union`` pragma can be applied to any ``object`` type. It
  means all of the object's fields are overlaid in memory. This produces a
  ``union`` instead of a ``struct`` in the generated C/C++ code."* (Nim 0.16.0
  manual, [ffi.txt mirror](https://web.mit.edu/nim-lang_v0.16.0/nim-0.16.0/doc/manual/ffi.txt)).
- **verified**, *"The `bitsize` pragma is for object field members. It declares
  the field as a bitfield in C/C++."*, `flag {.bitsize:1.}: cuint` emitting
  `unsigned int flag:1;` ([manual mirror](https://nim-docs.readthedocs.io/en/latest/manual/pragmas/specific_pragmas/), version not stated).
- **verified, and a correction to panel 073's record**: nim-lang/Nim#22708,
  *"C++ mode generates bad initialization code for union type"*, opened
  2023-09-15, closed 2024-03-03 ([API](https://api.github.com/repos/nim-lang/Nim/issues/22708)).
  The program constructs **one** field, `Union(b:true)`, and the C++ backend
  emitted `{((NI32)0), NIM_TRUE}`, **a positional value for every declared
  member**, which C++ refuses as *"excess elements in union initializer"*;
  bisected to commit `2288188fe` of 2020-10-03, failing from 1.2.18 to devel
  ([timeline](https://api.github.com/repos/nim-lang/Nim/issues/22708/timeline)).
  Panel 073 cited it as *"Heroes' exact bug"* (two-field construction,
  `073...:55`); its shape is in fact **defect 150's**: Heroes' completeness probe
  `W v = {0,0};` is the same positional-zeros-over-a-union initializer, and it
  stood in a production compiler for three and a half years. How Nim's C backend
  wrote the same constant: the reporter says it worked; the emitted line,
  **unverified**. How Nim binds a C struct holding an anonymous union: not
  found (searched `importc`, `c2nim`, the forum), **unverified**.

### Odin, Python ctypes, LuaJIT, cffi

- **Odin, verified**: *"struct #raw_union {...} // all fields share the same
  offset (0). This is the same as C's union"*, constructed `Foo{x = 123}`; and
  *"Odin's `bit_field`s have a well defined layout"* against *"C's bit fields on
  `struct`s are undefined and are not portable"* ([overview](https://odin-lang.org/docs/overview/), read 2026-10-02).
  So Odin's `bit_field` is deliberately not C's (my inference). *"`union`s in
  Odin do not have a compatible data layout to their equivalent C union"*
  ([Binding to C, 2022-07-17](https://odin-lang.org/news/binding-to-c/)).
  An anonymous union inside a bound struct, and a two-field raw-union literal:
  **unverified**.
- **ctypes, verified**: `_anonymous_` is *"An optional sequence that lists the
  names of unnamed (anonymous) fields"*; the union is declared as its own class,
  given a NAME in `_fields_` (`("u", _U)`), then listed in `_anonymous_` so that
  *"`td.lptdesc` and `td.u.lptdesc` are equivalent"* ([Python 2.5 docs](https://docs.python.org/2.5/lib/ctypes-structured-data-types.html)).
  The nested group with a user-chosen name, re-flattened for reading.
- **ctypes bit-fields, verified**: ctypes computes layout itself, and got it
  wrong: *"ctypes: bit field data does not survive round trip"*, opened
  2022-09-27, closed 2024-09-09 ([gh-97588](https://api.github.com/repos/python/cpython/issues/97588));
  Python 3.14: *"The layout of bit fields in Structure and Union objects is now a
  closer match to platform defaults (GCC/Clang or MSVC). In particular, fields no
  longer overlap."* ([What's New 3.14](https://docs.python.org/3/whatsnew/3.14.html)).
- **LuaJIT, verified**: its own C parser supports *"Unnamed ('transparent')
  struct/union fields inside a struct/union"*; *"Initialization of a union stops
  after one field has been initialized."* (silently, not an error); *"Packed
  struct bitfields that cross container boundaries are not implemented."*
  ([ext_ffi_semantics](https://luajit.org/ext_ffi_semantics.html), undated).
- **cffi, verified**, the closest analogue of Heroes' hand-written records
  checked against the compiler:
  - *"Use '...;' as the last 'field' to declare a partial structure ... the field
    offsets, total struct size, and total struct alignment aren't deduced by
    looking at the cdef. Instead they will be corrected by the compiler."*
    ([cdef docs](https://cffi.readthedocs.io/en/latest/cdef.html)): cffi's
    `...;` is Heroes' `partial`.
  - a non-partial declaration is checked field by field against the compiler and
    the message **names the field and the escape**: *"struct foo_s: wrong offset
    for field 'b' (cdef says 0, but C compiler says 4). fix it or use "...;" in
    the cdef for struct foo_s to make it flexible"*, *"struct foo_s: wrong size
    for field 'a' (cdef says 20, but C compiler says 24)"* (test expectations in
    the cffi-1.0 branch, 2015-05-08, [pypy-commit](https://mail.python.org/pipermail/pypy-commit/2015-May/090438.html)).
  - for the C check, anonymous members are expanded: *"When producing C, expand
    all anonymous struct/union fields. That's necessary to have C code checking
    the offsets of the individual fields contained in them."*; and bit-fields are
    **filtered out of `offsetof` and `sizeof`**: `if cname is None or fbitsize >= 0: offset = '(size_t)-1'`,
    `if fbitsize >= 0: op = OP_BITFIELD; size = '%d /* bits */' % fbitsize`
    ([recompiler.py](https://raw.githubusercontent.com/python-cffi/cffi/main/src/cffi/recompiler.py), `main`).
    This is panel 073's item 3, the filter defect 156 shows was never landed.

### C and clang themselves

- **verified (summary of the standard's text)**: C11 says the members of an
  anonymous structure or union *"are considered to have been declared as members
  of the containing structure or union"*, and an initializer list lets *"each
  initializer provided for a particular subobject overriding any previously
  listed initializer for the same subobject"* ([N1570](https://port70.net/~nsz/c/c11/n1570.html),
  §6.7.2.1 and §6.7.9; paragraph numbers unverified). So flat naming is C's own
  design, and two members named is legal C (last wins), which is why clang only
  warns.
- **verified**: no clang warns about a field a DESIGNATED initializer omits in C,
  by decision. llvm/llvm-project#81364, *"[clang] Add
  -Wmissing-designated-field-initializers"*, merged 2024-03-05, keeps the missing
  fields check disabled for *"designated initializers (only in C)"* to match GCC,
  the new group applying to C++ ([PR](https://github.com/llvm/llvm-project/pull/81364)).
  This is the reason behind the critic's § 4 measurement on clang 21 and 22.1.8.
- **verified**: the JSON AST dump disclaims stability: *"There is no implied
  stability for the content or format of the dump between major releases of
  Clang, other than it being valid JSON output. Further, there is no requirement
  that the information dumped is a complete representation of the AST, only that
  the information presented is correct."* ([JSONNodeDumper.h, release_90](https://llvm.googlesource.com/clang/+/refs/heads/release_90/include/clang/AST/JSONNodeDumper.h)).
  It has outside consumers that live with that: dtolnay's `clang-ast` crate,
  0.1.35 of 2026-08-02, *"deserialization logic for efficiently processing
  Clang's `-ast-dump=json` format"* ([docs.rs](https://docs.rs/clang-ast/latest/clang_ast/)).
- **verified**: the only consumer of `-fdump-record-layouts` text I found is
  clang's own `LayoutOverrideSource.cpp`, which searches for
  `"*** Dumping AST Record Layout"` to feed `-foverride-record-layout`
  ([llvm-project main](https://llvm.googlesource.com/llvm-project.git/+/refs/heads/main/clang/lib/Frontend/LayoutOverrideSource.cpp));
  that it is a testing-only facility is in a search summary, **unverified**.
  Searched for a binder that parses that dump: none found (a question, not a
  premise).

## 2. Panel 073's *construction-arity form*, checked against the sources

The record (`073...:119-123`): *a record over a union may declare every member
and be constructed naming exactly one, which is what Rust, D, Zig and Swift all
converged on.*

**For a union TYPE: verified in all four.** Rust: *"it must specify exactly one
field"* ([Reference, items.union.init.intro](https://doc.rust-lang.org/reference/items/unions.html);
unions stable since 1.19.0, 2017-07-20, [blog](https://blog.rust-lang.org/2017/07/20/Rust-1.19/)).
D: *"only one member initializer is allowed"*. Zig 0.13.0: *"cannot initialize
multiple union fields at once"*. Swift: one `init(member:)` per member. Every one
declares every member.

**For a STRUCT holding an anonymous union, the shape of `SA`, the claim needs a
correction.** All four keep "exactly one per union", but only **D** names it
flat in the struct's own literal, which is what Heroes' flat record does. Swift
reads flat and constructs through an unlabelled nested `.init(layers: 2)`;
bindgen and Zig construct through a synthetic name (`__bindgen_anon_1`,
`unnamed_0`; the construction expression itself is my inference from the union
rule, not read). Rust's flat form, RFC 2102, was accepted in 2017 and its
implementation removed in 1.83.0 (2024-10-11) for its compiler burden. So: four
languages converged on the rule, one shipping language on the flat spelling
Heroes needs, and Heroes, unlike rustc, inherits the flat spelling from C11 for
free and owes only the knowledge of which fields share a union.

## 3. Verdict per route (advisory)

| route | verdict | the precedent that decides it |
|---|---|---|
| **(1a)** per-pair range assertions | **object** as written; approve a linear variant | No binder found asserts overlap pairwise. The two that check a declaration against the compiler do it **per field, linearly**: bindgen's `offset_of!` per field, cffi's per-field offset and size. The quadratic form has no precedent and is blind to `SB`. |
| **(1b)** designated probe under `-Winitializer-overrides` as error | **object** as the detector; no precedent either way for arming it at Heroes' own construction | C11 makes overriding legal (last wins), so the signal is a lint, not a fact; no binder found detects unions by it; blind to `SB`, and it refuses `read.hero`. |
| **(1c)** refuse any record over a struct holding an anonymous union | **object** | Go's byte array (since 2009) and `-godefs`' first-member promotion: users rebuilt the structs by hand (`my_epoll_event`, `perf_event_attr_go`, Xen's byte copy), and the rebuild defeated cgo's pointer check (#15942). Zig's refusal of bit-field structs: open eight years, users write C wrappers. And u08 runs today. |
| **(1d)** a nested group in the record's declaration | **object** (mild) | Nesting exists in bindgen, Zig, ctypes because their target language cannot promote a member; RFC 2102 names it as the cost (*"artificial naming"*). C11, D and Swift's reading are flat, and Heroes' target is C. A new surface form buys what the C it emits already gives. |
| **(1e)** the layout read from clang, verdict a C assertion | **approve**, via the JSON AST rather than `-fdump-record-layouts` | Every long-running binder asks the C compiler: cgo by DWARF since 2009, bindgen by libclang with per-field compile-time assertions, Zig by `isAnonymousStructOrUnion`/`isBitField`, Swift by `IndirectFieldDecl`, cffi by compiling per-field checks. bindgen's shape is panel 103's exactly: numbers from clang, verdict asserted in the compiled code. The record-layout dump has no outside consumer found; the JSON dump has a disclaimer and outside consumers, and this project's floor measurement. |
| **(1f)** classify OR sum-of-sizes | **object** | No binder detects unions by a size heuristic; all read the record's kind. Panel 077 already swapped one predicate for another on a padded union; `PADU` and `SB` pass this one. |
| **(1g)** refuse `==`, `hash`, map key unless proven union-free | **approve as the floor** | bindgen withholds `PartialEq`/`Hash` from a struct holding an anonymous union even under `--with-derive-partialeq --impl-partialeq` (DPDK's `rte_mbuf`), assumes it pessimistically and transitively for what it cannot see, and Rust refuses the derive on a union (today's stable branch; since when, unverified). With (1e), *proven* becomes knowledge, as in bindgen. |
| **(1h)** the construction-arity form | **approve**, with (1e) as its knowledge | Rust, D, Zig, Swift on a union type; D's struct-literal rule for `SA`'s flat shape. D's message, *"overlapping initialization for field `a` and `b`"*, is also true of a struct, where *"`T` is a `union`"* is false for `SA`. |

**Q2, the completeness probe.** The precedent is against both instruments it
uses today. A positional initializer over a union is Nim#22708 (three and a
half years in a production compiler); a designated one cannot report an
omission in C by the decision of both clang and GCC (#81364). The one binder that
checks a hand-written declaration's completeness against the compiler, cffi
since its 1.0 branch (2015), compares **layout**, per field, names the field,
and points at the partial marker. That is (1e). On the one-member union record:
cffi's non-partial declaration must match the compiler's layout, which leans to
panel 073's reading (one member named of several is not complete) over the code
comment at `extern_record.hero:91-92`; that cffi refuses exactly that shape is
my inference, **unverified**.

**Defect 156, the bit-field member.** Those who let the C compiler do the bits
(Nim's `bitsize`, Swift's computed properties, D) bound bit-fields as fields;
those who computed the bits themselves shipped wrong values (cgo before 1.16,
ctypes until 3.14) or compile failures on real headers (bindgen, `alt_instr`);
those who refused (Zig, cgo since 1.16) left users writing C. Heroes emits C that
names the field, so it is in Nim's position: bind it, and filter it out of
`sizeof`, `offsetof` and every address-taking site, as cffi does. Approve that;
object to a refusal.

## 4. Falsifiable predictions

1. If (1c), or a declaration rule *a record names exactly one member of each
   union* (candidate N), ships, the first real-header binding of a struct holding
   an anonymous union in the ladder (SDL3 or raylib, both on this Mac) reaches
   the other members through a hand-written C function or a second C type
   written for the purpose, within the milestone that writes it. Under (1h)
   with (1e), that count is zero.
2. Under (1e) on the JSON AST, the union facts of `SA`, `SB`, `PADU` and a union
   two levels deep need no branch by clang version across the seven clangs of
   the shared brief's table. If one is needed, the disclaimer in
   `JSONNodeDumper.h` was the warning and the route needs a version gate.
3. Under cffi's filter, `BF` builds, compares and hashes with no hand-written C
   and reads correctly on the three platforms; a refusal of bit-field records
   would gain an open defect or DECIDE item asking for them within two
   milestones, as Zig's #179 did.

## 5. What would change these verdicts

- **(1c) or N to approve**: a binder that made a struct's anonymous union
  unreachable or single-member at the type level and whose users did not route
  around it after three years (panel 073's historian's condition, carried).
- **(1d) to approve**: the blind seat measuring that flat declarations hide the
  union from a reader (panel 073's ergonomist: *each line right, their
  conjunction wrong*) in a way a construction-time diagnostic does not repair;
  ctypes' named-then-promoted group is then the precedent.
- **(1e) to object**: a seat measuring that the JSON dump omits the anonymous
  union's members or kind on any clang of the table, or a binder found that
  abandoned clang's AST for a reason that applies to a text dump.
- **(1g) to object**: a tracked program, outside `docs/panel/*-briefs/`, that
  compares two group records and is refused by it.
