# Panel 164 — historian report (verbatim; written to disk by the coordinator, the seat has no write tool)

## verdict

**object** (advisory, no veto) to **route 1**, and to **route 4** as written.
**approve route 3** as the spine, with **route 2 admitted only when the binding itself declares the field terminated** (Cyclone's `@zeroterm`, Zig's `[N:0]u8`).

Route for finding the rooms: the author's own *Heroes of code* / *Gli eroi del codice* pointed me at Zurich (Wirth) and at the safe-C dialects. It is named as a route only. Every claim below rests on a page or PDF I fetched in this session, and the ones I could not confirm say so in those words.

## precedents

### 1. Cyclone: the exact ancestor of route 3, and the only project that recorded reversing on this question — VERIFIED

Cyclone made NUL-termination a **type qualifier**, not a convention:

- "`@zeroterm`: This qualifier is only present by default for char pointers." / "`@nozeroterm`: This qualifier is present by default on all pointer types except for char pointers." The compiler "inserts code to ensure that the index does not step over the final zero", and "when updating a zero-terminated array, the compiler also ensures that the final zero is not overwritten with a non-zero value." — https://cyclone.thelanguage.org/wiki/Pointers/
- **And the decisive sentence for this sitting**: "by default, char arrays are *not* considered zero-terminated. To make them so, you must add the `@zeroterm` qualifier following the size of the array", and then "the size of the array must include the zero terminator" (`char s[4] @zeroterm = "bar";`). — https://cyclone.thelanguage.org/wiki/Cyclone%20for%20C%20Programmers/

**Cost, from the authors' own "Design history" section** (USENIX ATC 2002 paper, PDF fetched and read, §4):

> "We didn't understand the importance of NUL-terminated strings. NUL termination isn't guaranteed in C, so, for safety, we were committed to using explicit array bounds from the beginning. The NUL seemed pointless, and our first string library ignored it. As we programmed more in the language and ported C code, we came to understand how important NUL is to efficiency (memory reuse), and we changed our string library to match up with C's."

Other measured costs in that same paper: porting diffs (Table 3) total **18627 C LOC → 18847 Cyclone, 1452 lines changed, 8% of lines**, of which the `*`→`?` change was **31%**; the text says "Usually fewer than 10% of the lines needed to be changed" and that the simple pointer change "accounted for 20–50% of changed lines". Runtime (Table 4): checked `grobner` **2.85×** C, `cfrac` 2.42×, most I/O-bound programs 1.00×. Compiler size: "approximately 35,000 lines of Cyclone". Future work already names the thin zeroterm pointer that later shipped: "a pointer to a zero-terminated array can be safely represented as just an address, as long as the pointer only moves forward inside the array, and the zero terminator is never overridden." — https://www.usenix.org/legacy/publications/library/proceedings/usenix02/full_papers/jim/jim.pdf

**What it means here.** Cyclone is CLAUDE.md §5's named ancestor and it landed precisely on route 3's discipline: a plain `char[N]` is not a C string; the author may *declare* it one. It also refused NUL entirely first and reversed, which is the warning against route 4's purity.

### 2. Zig: the terminator in the type, and the escape hatch it had to keep — VERIFIED, with one sub-claim from community docs

- "The syntax `[*:x]T` describes a pointer that has a length determined by a sentinel value. **This provides protection against buffer overflow and overreads.**" "The syntax `[N:x]T` describes an array which has a sentinel element of value `x` at the index corresponding to the length `N`." Passing a non-terminated array where a sentinel pointer is declared is a **compile error**: "destination pointer requires '0' sentinel". — https://ziglang.org/documentation/master/#Sentinel-Terminated-Pointers and https://ziglang.org/documentation/0.14.0/
- `std.mem.sliceTo` "takes an array, a pointer to an array, a sentinel-terminated pointer, or a slice and iterates searching for the first occurrence of end, returning the scanned slice" (search result summary of https://github.com/ziglang/zig/blob/master/lib/std/mem.zig). That is `validated_bytes` by another name.

**Cost**: a second pointer kind, `[*c]T`, that coerces freely and reopens the hole for `@cImport`ed code. "C pointers coerce back and forth between integers, and also coerce to single and multi item pointers", and "Outside of automatically translated C code, the usage of `[*c]` is almost always a bad idea, and should almost never be used." — https://zig.guide/working-with-c/c-pointers/ . **Marked partially verified**: I could not fetch the langref's own "C Pointers" section (the page truncated twice), so the "almost always a bad idea" wording is zig.guide, a community document, not the language reference.

### 3. Rust: no decay, and a five-year wait for the read this sitting already has — VERIFIED

- Rust's coercion list contains **no** array-to-raw-pointer coercion; arrays coerce to slices, and `as_ptr` is a slice method reached through that coercion. "Arrays coerce to slices (`[T]`), so a slice method may be called on an array." — https://doc.rust-lang.org/reference/type-coercions.html and https://doc.rust-lang.org/std/primitive.array.html
- The field really is a fixed array: `pub sysname: [c_char; 65]` in `libc` 0.2.189. — https://docs.rs/libc/latest/libc/struct.utsname.html
- `CStr::from_bytes_until_nul` stabilized in **1.69.0, 2023-04-20** — https://blog.rust-lang.org/2023/04/20/Rust-1.69.0/ and https://doc.rust-lang.org/std/ffi/struct.CStr.html#method.from_bytes_until_nul (signature `pub const fn from_bytes_until_nul(bytes: &[u8]) -> Result<&CStr, FromBytesUntilNulError>`; its doc example is literally `let mut buffer = [0u8; 16];` filled by "an unsafe C function").

**Cost**: the motivating issue was opened by Boscop on **2018-03-17** and its first sentence is this sitting's problem word for word: "When dealing with FFI (fixed char buffers in file formats that represent strings and passing fixed char buffers to C code to write chars to them up to a given max len)...", with the boilerplate `let end = s.iter().position(|&b| b == 0)...` repeated at every site. — https://github.com/rust-lang/rust/issues/49107 . **Five years** of hand-written scanning before the standard library owned it.

### 4. Go/cgo: the language that says out loud it cannot do C's adjustment — VERIFIED

- "In C, a function argument written as a fixed size array actually requires a pointer to the first element of the array. C compilers are aware of this calling convention and adjust the call accordingly, **but Go cannot**. In Go, you must pass the pointer to the first element explicitly: `C.f(&C.x[0])`." — https://pkg.go.dev/cmd/cgo
- The two directions get two functions with two different honesty levels: `ByteSliceToString` "returns a string form of the text represented by the slice s, with a terminating NUL and any bytes after the NUL removed"; `BytePtrToString` "assumes that the text sequence is terminated at a zero byte; **if the zero byte is not present, the program may crash**." — https://raw.githubusercontent.com/golang/sys/master/unix/syscall.go

**Cost**: an explicit `&x[0]` at every site, plus the pointer-passing rules ("Go code may pass a Go pointer to C provided the memory to which it points does not contain any Go pointers to memory that is unpinned"). **Unverified**: whether `x/sys/unix`'s `Utsname` fields are `[65]byte` or `[65]int8` today. The request to change them is https://github.com/golang/go/issues/20753; I could not find the current declaration in the generated file I fetched, so the brief's `&uts.Sysname[0]` shape stands but its element type is unconfirmed by me.

### 5. D: C's decay deliberately removed, the pointer made a property — VERIFIED (cost unmeasured)

"Static arrays are value types. They are passed to and returned by functions by value." and "The `.ptr` property will give a pointer to the first element in a static or dynamic array." — https://dlang.org/spec/arrays.html . I did **not** verify what this cost D in porting effort; that is unverified.

### 6. Ada: the missing terminator is a named exception, not undefined behaviour — VERIFIED (partially)

`Interfaces.C`'s `To_Ada` takes a `Trim_Nul` parameter, and "The function propagates `Terminator_Error` if `Trim_Nul` is True and Item does not contain nul." — https://ada-lang.io/docs/arm/AA-B/AA-B.3 (the ada-auth.org original refused the connection this session). **Unverified**: my reading that `To_C`/`To_Ada` always copy rather than lend; I did not confirm that from the standard text.

### 7. Swift: the precedent for what a bad answer costs, measured in years — VERIFIED

Swift imports a fixed-size C array as a tuple: `float[4]` becomes `(Float, Float, Float, Float)`, so `utsname`'s `char[256]` fields become 256-tuples, and reading one requires

```swift
let machine = withUnsafeBytes(of: &utsInfo.machine) { rawPtr -> String in
    let ptr = rawPtr.baseAddress!.assumingMemoryBound(to: CChar.self)
    return String(cString: ptr)
}
```

— https://oleb.net/blog/2017/12/swift-imports-fixed-size-c-arrays-as-tuples/ (published 2017-12-29).

**Cost**: still being repaired. Becca Royal-Gordon posted the pitch "Modernize imported C arrays" on **2026-07-29**, proposing that "Swift import C arrays as `InlineArray` when an appropriate upcoming feature ... is used and when the minimum deployment target is high enough" — https://forums.swift.org/t/pitch-modernize-imported-c-arrays/88633 . That is **eight and a half years** of `withUnsafeBytes` boilerplate in every program that touched a C struct field. SE-0453's acceptance (December 2024) I have from a search summary of https://forums.swift.org/t/accepted-with-modifications-se-0453-inlinearray-formerly-vector-a-fixed-size-array/77678 and **did not fetch**; treat the date as unverified.

### 8. Nim: the one precedent for implicitness, and it is being withdrawn — VERIFIED

The Nim manual: "A Nim string is implicitly convertible to cstring for convenience... **Even though the conversion is implicit, it is not safe**: The garbage collector does not consider a cstring to be a root and may collect the underlying memory. **For this reason, the implicit conversion will be removed in future releases of the Nim compiler.**" — https://nim-lang.org/docs/manual.html#types-cstring-type

And the array case specifically: issue "`ptr char` implicitly converts to cstring, resulting in undefined behavior", opened by timotheecour on **2020-03-28**, noting that "a `ptr char` could come from any source ... with nothing that should guarantee that it's 0 terminated", and that `ptr array[N, char]` and `ptr UncheckedArray[char]` have the same hole. — https://github.com/nim-lang/Nim/issues/13790 . **Unverified**: whether the linked PR #20761 landed and in which Nim release.

This matters twice over, because CLAUDE.md §6 already rules on Nim: copy the surface, never the implementation. Route 1 and unrestricted route 2 are Nim's implementation.

### 9. Odin — UNVERIFIED BY FETCH

Search summaries of https://odin-lang.org/docs/overview/ say a `[16]u8` reaches a `cstring` only via `raw_data(arr[:])` and an explicit `cstring(...)` cast. I did not fetch the page; treat as unverified. If it holds, it is one more explicit-site language.

### 10. What C's own rule has cost, so route 1 is weighed against a bill — VERIFIED

- **STR32-C**: "Do not pass a character sequence or wide character sequence that is not null-terminated to a library function that expects a string or wide string argument." Risk assessment: **Severity High, Likelihood Probable, Priority P12, Level L1**. Related: CWE-119, CWE-123, CWE-125, and **CWE-170 (exact match)**. Its noncompliant example is a `char c_str[32]` filled by `strncpy` and then measured by `strlen`. — https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/characters-and-strings-str/str32-c
- **CWE-170**: "If a null character is omitted from a string, then most string-copying functions will read data until they locate a null character, even outside of the intended boundaries of the string", and the omitted-NUL case "will almost certainly result in information disclosure, and possibly a buffer overflow condition, which may be exploited to execute arbitrary code." Observed instances: CVE-2009-2523, CVE-2003-0143, CVE-2004-1072. — https://cwe.mitre.org/data/definitions/170.html
- **ARR01-C**, the decay's second bill: on an array parameter, "`sizeof(array)` is equal to the `sizeof(int *)`", so the length idiom silently evaluates to 1. — https://wiki.sei.cmu.edu/confluence/display/c/ARR01-C.+Do+not+apply+the+sizeof+operator+to+a+pointer+when+taking+the+size+of+an+array (reached via search; the confluence host redirected me to the GitHub Pages mirror for STR32-C, so treat the ARR01-C quotes as **search-summary, not fetched**)

### 11. The Wirth lineage carries the length, not the pointer — VERIFIED (on a mirror)

Wirth's Oberon report (revision 1.10.2013 / 3.5.2016): "If the formal parameter's type is specified as `ARRAY OF T` the parameter is said to be an open array, and the corresponding actual parameter may be of arbitrary length", with `LEN(v)` giving the length; and "Strings can be assigned to any array of characters, provided the number of characters in the string is less than that of the array. (A null character is appended)." — https://miasap.se/obnc/oberon-report.html . **Caveat**: this is a mirror of the report, not ETH's own copy, which I could not reach this session. The lesson is route 3's: in the lineage design.md claims, an array reaching a procedure carries its **length**, and the 0X is an additional convention on top.

## argument (115 words)

Nine language communities met this exact field, and eight refuse C's decay: Rust, Zig, Go, D, Odin, Swift, Ada, Cyclone. Each makes the crossing visible at the site or in the type. The ninth, Nim, took implicitness and is removing it. Route 1 therefore has one precedent and it is in retreat; adopting it would make Heroes the only language in this survey that lets a `char[N]` field silently become a C string, in a project whose first precedence rule is robustness. Route 3 is the mainstream, and Cyclone is the exact ancestor: char arrays are not zero-terminated by default, `@zeroterm` is declared. Cyclone also warns against route 4's purity: it refused NUL and reversed.

## falsifiable prediction

**Prediction (option-set claim, CL-057).** No language in {Rust, Zig, Go, D, Odin, Swift, Ada, Nim after its deprecation, Cyclone} accepts a fixed-size byte array at a position declaring a NUL-terminated C string parameter **without** either (a) an explicit act written at that site, or (b) a type-level declaration that the array is terminated. **Falsified by exhibiting one such language with documentation showing the bare field being accepted.** Cyclone's `char s[4] @zeroterm` and Zig's `[N:0]u8` are (b); everything else surveyed is (a).

**Second prediction, about this repository, if route 3 lands alone.** The brief measures 58 extern functions taking a plain `cstr` against 5 declared fixed byte fields. I predict the first `examples/` or `tests/golden/` program that binds a *terminated* field to one of those 58 will require either a hand-written `static inline` shim or a second construct within two milestones. **Falsified if route 3 ships and no shim and no `cstr`-for-fields construct appears by the second milestone close after it.** That is route 3's honest bill, and it is why I would pair it with route 2 under a declared terminator rather than ship route 3 bare.

## condition (what would change my reading)

1. A fetched, current document showing a language that binds C and accepts a bare fixed byte array where a NUL-terminated string parameter is declared, with no site-level act and no type-level termination claim. That would give route 1 a living precedent, and I would withdraw the objection to it.
2. A measurement showing that of the 5 fixed byte fields in this repository, all 5 are bound to headers whose *specification* guarantees termination (as POSIX does for `utsname`). Then route 2's widening is safe for the corpus that exists, and the question becomes about the corpus that does not.
3. Evidence that Cyclone's `@zeroterm`-on-arrays cost measurably more than `@zeroterm`-on-pointers in the porting tables. I could not separate the two in Table 3; if someone can, it prices the declared-terminator route directly.

## what I found misstated in `00-shared.md`

**(a) Line 72 overstates line 43.** The scale section says panel 162 measured "**only 13 reliably NUL-terminated**" of 50; route 1's bullet then says "**37 of 50 real fields have no terminator**". Those are different claims. "Not reliably terminated" includes fields that are terminated in practice but not guaranteed by their header's contract. The brief's own rule (panel 163's, adopted the same day) is that a negative sentence is run or marked; this one was arithmetic on a different predicate. It matters in both directions, and here it happens to make route 1 look worse than the measurement supports.

**(b) The emblematic example is drawn from the safe class, not the hazardous one.** Line 26 offers `strlen(u.sysname)` as "the idiom every POSIX program writes", used to motivate the overread. POSIX **requires** those members to be terminated: "The character arrays are of unspecified size, but the data stored in them shall be terminated by a null byte." — https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/sys_utsname.h.html ; Linux's uname(2) repeats it: "The length of the arrays in a *struct utsname* is unspecified ...; the fields are terminated by a null byte ('\0')." — https://man7.org/linux/man-pages/man2/uname.2.html . So the brief's own emblem is the strongest argument *for* letting the field cross to a `cstr`, not against it, and a seat reading only line 26 would weigh route 1 on a case where route 1 is sound. The sitting should pick an emblem from the 37, not the 13.

**(c) Route 1's phrasing "C's own rule, applied at the checker" conflates two rules.** C's array-to-pointer conversion is a *conversion* rule; it makes no claim about termination at all. The hazard lives entirely in what the callee does with the pointer, which is why Cyclone split it into two qualifiers rather than one decay. This is my reasoning rather than a sourced claim: I did not fetch the C standard's text this session, so the clause number stays out and the observation is marked **unsourced**.

**(d) Route 3's premise is the best-sourced sentence in the brief.** "A `cstr` is a promise of a terminator that a `char[N]` field does not make" is, word for word, Cyclone's default and Zig's compile error. It is not a misstatement; it is the sitting's strongest line and it deserves to be cited as such.

**(e) The brief's file references resolve.** `selfhost/check/lending.hero` exists and its module doc confirms the panel 122 R2 position rule, including a note that the historian seat at panel 122 "found none" when asked for a language using call-kind as a lifetime criterion. Today the mirror question has a full answer: **every** language surveyed uses an explicit act or a declared type. The two findings are consistent, and the second is the stronger one.
