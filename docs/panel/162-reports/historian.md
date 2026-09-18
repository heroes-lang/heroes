# Panel 162 — historian (advisory, no veto)

**Written out by the coordinator, verbatim, because this seat has no write tool.**

*Every URL below was fetched in this session, 2026-09-18. Claims with no fetched URL are marked `unverified` in those words.*

---

## verdict

**approve, with one correction to the brief's framing** — the sitting is choosing **two** failure modes, not one, and the precedent splits differently on each. Route 1 (a fallible inbound conversion answering `str?`) is the majority precedent and the one the last decade has moved *toward*; the brief's "repeat literal" and "zero default" build routes are the near-unanimous precedent and Rust's counter-example is the strongest single warning in this report.

---

## precedents

### Item 6 (priority 1) — what each language does when the bytes are not valid text

Two orthogonal axes, and the brief's §"What the answer must not contradict" only names one. **Termination** (where does the run stop?) and **validity** (is the run text?). Every precedent below answers both, separately.

| language | termination of a `char[N]` | validity of the resulting bytes | verified? |
|---|---|---|---|
| **Rust** `str::from_utf8` | n/a (caller slices) | **fallible**: `pub const fn from_utf8(v: &[u8]) -> Result<&str, Utf8Error>`, stable since 1.0.0, const since 1.63.0 | verified — [doc.rust-lang.org/std/str/fn.from_utf8.html](https://doc.rust-lang.org/std/str/fn.from_utf8.html) |
| **Rust** `String::from_utf8_lossy` / `CStr::to_string_lossy` | n/a | **lossy**, U+FFFD substitution, returns `Cow<str>`; `to_string_lossy` since 1.4.0 | verified — [doc.rust-lang.org/std/ffi/struct.CStr.html](https://doc.rust-lang.org/std/ffi/struct.CStr.html) |
| **Rust** `CStr::from_bytes_until_nul` | **fallible**: `Result<&CStr, FromBytesUntilNulError>`; errors when there is no NUL | separate step (`to_str` → `Result`, or `to_string_lossy`) | verified — same page; stabilized **1.69.0, 2023-04-20** ([blog.rust-lang.org/2023/04/20/Rust-1.69.0](https://blog.rust-lang.org/2023/04/20/Rust-1.69.0/)) |
| **Go** `string(b)` | none — the whole array, NUL bytes included | **unchecked**: "A string value is a (possibly empty) sequence of bytes… Strings are immutable"; no UTF-8 validation on `[]byte`→`string` | verified — [go.dev/ref/spec](https://go.dev/ref/spec) |
| **Go** `unix.ByteSliceToString` | **infallible truncation**: `if i := bytes.IndexByte(s, 0); i != -1 { s = s[:i] }; return string(s)`; no NUL → whole run | **unchecked** | verified — [raw.githubusercontent.com/golang/sys/master/unix/syscall.go](https://raw.githubusercontent.com/golang/sys/master/unix/syscall.go) |
| **Zig** `std.mem.sliceTo` | **infallible**: "If `end` is not found, the full length of the array/slice/sentinel terminated pointer is returned" | **no string type at all** — the result is `[]u8`; validity is a separate, opt-in call | verified — [lib/std/mem.zig](https://raw.githubusercontent.com/ziglang/zig/master/lib/std/mem.zig) |
| **Ada** `Interfaces.C.To_Ada` | **exception**: `function To_Ada (Item : in char_array; Trim_Nul : in Boolean := True) return String;` — "propagates `Terminator_Error` if `Trim_Nul` is True and `Item` does not contain nul" | no encoding validity concept (`char` ↔ `Character`, 1:1) | verified — [ARM B.3, adaic.org](https://www.adaic.org/resources/add_content/standards/05rm/html/RM-B-3.html) |
| **Swift** `init?(validating:as:)` | n/a | **fallible**, `String?`, shipped **Swift 6.0**; SE-0405 chose fallible over repair explicitly, because "such a transformation is often not desirable, especially when dealing with untrusted sources" | verified — [SE-0405](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0405-string-validating-initializers.md) |
| **Python 3** `bytes.decode` | n/a | **fallible by default**: `errors='strict'` raises `UnicodeError`; `'replace'`/`'ignore'` are opt-in | verified — [docs.python.org/3/library/stdtypes.html](https://docs.python.org/3/library/stdtypes.html#bytes.decode) |
| **Pony** (RFC 107, still open, filed 2017-10-23) | n/a | proposed **partial** (fallible) constructor from untrusted byte arrays | verified — [ponylang/rfcs#107](https://github.com/ponylang/rfcs/issues/107) |
| **nix** (the dominant Rust Unix binding) | infallible scan | **refuses to decide**: `pub fn sysname(&self) -> &OsStr` — hands back an OS string, caller picks `to_str()` (fallible) or lossy | verified — [docs.rs/nix 0.31.3](https://docs.rs/nix/latest/nix/sys/utsname/struct.UtsName.html) |

**The direction of travel is one-way.** Swift shipped lossy-by-default first and added the failable initializer in Swift 6.0 (SE-0405, verified above). I found **no** case in this set of a language moving from fallible to lossy-by-default. That is a claim about the eleven rows above and not about the world (CL-057).

**What nobody does: panic/abort on invalid text at this boundary.** Ada aborts on *missing terminator*, never on encoding. Not one row aborts on invalid bytes. If Heroes picks abort it is alone, and being alone is admissible — it just has to be said out loud.

### Item 3 (priority 2) — Go's `unix.Utsname`, nine years on

- **`golang.org/x/sys/unix` today**: `type Utsname struct { Sysname [65]byte; Nodename [65]byte; Release [65]byte; Version [65]byte; Machine [65]byte; Domainname [65]byte }` — **verified**, [unix/ztypes_linux.go](https://raw.githubusercontent.com/golang/sys/master/unix/ztypes_linux.go).
- **The one-liner exists**: `unix.ByteSliceToString(uts.Sysname[:])` — **verified**, body quoted above. Doc comment: *"returns a string form of the text represented by the slice s, with a terminating NUL and any bytes after the NUL removed."*
- **But the standard library never got fixed.** `syscall.Utsname` on linux/amd64 is **still** `[65]int8`, today: **verified**, [src/syscall/ztypes_linux_amd64.go](https://raw.githubusercontent.com/golang/go/master/src/syscall/ztypes_linux_amd64.go) (header: `// Code generated by cmd/cgo -godefs; DO NOT EDIT.`).
- **And the Go toolchain itself still casts through `unsafe`.** `cmd/internal/osinfo/os_uname.go`: `s := uts.Sysname[:]` then `writeCStr(*(*[]byte)(unsafe.Pointer(&s)))`, with `writeCStr` doing `bytes.IndexByte(b, '\000')` by hand — **verified**, [tip.golang.org/src/cmd/internal/osinfo/os_uname.go](https://tip.golang.org/src/cmd/internal/osinfo/os_uname.go).
- Origin issue [golang/go#20753](https://github.com/golang/go/issues/20753), filed 2017-06-22, closed, `FrozenDueToAge` — **verified**.

**The finding: the ergonomic fix was reachable only by changing the field's element type, and the half of Go that could not change its types (the frozen `syscall` package) is still awkward in 2026 — its own compiler reaches for `unsafe.Pointer` to read a machine name.** For Heroes that argues against any answer whose ergonomics depend on the FFI author having declared `u8[256]` rather than `i8[256]`; the header decides the sign, not the binding author.

Swift is the same story with a worse bill: the Clang importer turns `char[256]` into a **256-tuple of `Int8`**, and the recommended incantation is

```swift
let machine = withUnsafeBytes(of: &utsInfo.machine) { rawPtr -> String in
    let ptr = rawPtr.baseAddress!.assumingMemoryBound(to: CChar.self)
    return String(cString: ptr)
}
```

— **verified**, [Ole Begemann, 2017-12-29](https://oleb.net/blog/2017/12/swift-imports-fixed-size-c-arrays-as-tuples/) (his measured ~8 s compile and ~750 KB binary growth were for *his own* `_FixedArray256` workaround type, not for the importer — I state that narrowly on purpose). The repair is **still a pitch**: "[Pitch] Modernize imported C arrays", dated **2026-07-29**, proposing C arrays import as `InlineArray` instead of tuples — **verified**, [forums.swift.org/t/pitch-modernize-imported-c-arrays/88633](https://forums.swift.org/t/pitch-modernize-imported-c-arrays/88633). **Eight and a half years from the blog post to the pitch.**

### Item 1's second half (priority 3) — the BUILD wall

- **Rust const generics MVP: 1.51.0, 2021-03-25** — verified, [blog.rust-lang.org/2021/03/25/Rust-1.51.0](https://blog.rust-lang.org/2021/03/25/Rust-1.51.0/). *"This was most notable in arrays which include their length in their type definition (`[T; N]`), which previously you could not be generic over."*
- **And it did not fix `Default`. Today, 2026-09-18, the std docs still read:** *"Arrays of sizes from 0 to 32 (inclusive) implement the `Default` trait if the element type allows it. As a stopgap, trait implementations are statically generated up to size 32."* — verified, [doc.rust-lang.org/std/primitive.array.html](https://doc.rust-lang.org/std/primitive.array.html). Tracking issue [rust-lang/rust#61415](https://github.com/rust-lang/rust/issues/61415), opened **2019-05-31**, **still open**, because `impl<T> Default for [T; 0]` exists without `T: Default` and rewriting it generically needs specialization.
- **What Rust users write instead**: the repeat expression `[0u8; 256]`, legal because *"If the length operand has a value greater than 1 then this requires the repeat operand to have a type that implements `Copy`, to be a const block expression, or to be a path to a constant item"* — verified, [Rust Reference, array expressions](https://doc.rust-lang.org/reference/expressions/array-expr.html). So Rust's array literal has *always* had the brief's route "a repeat literal"; what it lacks after seven years is the *derived* one.

**Does any language let you write a struct literal that omits a large array field? Four do.**

| language | what you write | verified? |
|---|---|---|
| **C** | `struct utsname u = {0};` — members with no initializer are empty-initialized (zero) | partially verified — [cppreference, struct initialization](https://en.cppreference.com/w/c/language/struct_initialization.html), which cites C11 6.7.9/12-38. **I could not fetch the C11 paragraph text itself**, searching for "6.7.9p21" at open-std.org (PDF, unparseable), port70.net and iso-9899.info (both truncated before clause 6). Treat the paragraph *number* as unverified; the rule as verified via a secondary source. |
| **Ada** | `(others => nul)` — *"the components are given in increasing-index order, with a final **others**, if any, representing any remaining components"* | verified — [ARM 4.3.3](https://www.adaic.org/resources/add_content/standards/12rm/html/RM-4-3-3.html) |
| **Zig** | `std.mem.zeroes(T)` — *"Zero initializes the type… Structs will be initialized recursively"*; or `= undefined`, where *"in debug and safe mode, Zig writes `0xaa` bytes to undefined memory… this behavior is only an implementation feature, not a language semantic"* | verified — [lib/std/mem.zig](https://raw.githubusercontent.com/ziglang/zig/master/lib/std/mem.zig), [ziglang.org/documentation/master/#undefined](https://ziglang.org/documentation/master/#undefined) |
| **Go** | `var uts unix.Utsname` — *"If a variable has not yet been assigned a value, its value is the zero value for its type"* | verified — [go.dev/ref/spec](https://go.dev/ref/spec). (I could not fetch the "The zero value" section body itself; the Variables section quote above is what I have.) |

**Ada also answers wall two the way the brief's out-parameter route wants**, thirty years early: `procedure To_Ada (Item : in char_array; Target : out String; Count : out Natural; Trim_Nul : in Boolean := True);` and the symmetric `procedure To_C (…; Target : out char_array; Count : out size_t; Append_Nul : in Boolean := True);`, `Constraint_Error` if `Target` is too short — verified, ARM B.3 above. Caller-supplied buffer, explicit count out, explicit termination flag, explicit exception. That is the shape route 3 ("a slice-shaped answer") is groping for, standardised in 1995.

### Item 5 — the refusal case

**I could not source a language that binds C and refuses outright**, leaving the program to read `char[N]` element by element. Searched for *"language refuses convert byte array to string type FFI no conversion must iterate elements design rationale"* on the open web, and inspected the Zig, Ada, Go, Rust and Swift precedents above directly; Zig is the closest and it is not a refusal — it has **no string type to refuse into**, and `sliceTo` hands back `[]u8` unconditionally. Zig's answer is therefore *not available* to Heroes, which has an `str` with an invariant (`spec § 3`). Report this as a negative result resting on my vocabulary, not on the world (CL-018).

---

## argument

Precedent splits cleanly. On **validity** the last decade moves one way — Swift added a failable initializer in 6.0 *after* shipping lossy, Python and Rust were fallible from the start, nix refuses to decide — so `str?` is the defensible choice and lossy is the one later regretted. On **termination** precedent disagrees (Ada raises, Go and Zig truncate silently, Rust errors), so Heroes must state it separately and not let one answer cover both. On the build wall, C, Ada, Zig and Go all let you omit the array; Rust's per-length stopgap is **still** capped at 32 after seven years. Do not ship a stopgap: they ossify.

---

## condition

Three findings would change my reading.

1. **`read_file`'s actual failure mode.** The brief calls it "the cheapest possible answer" because the choice is "already made somewhere in this compiler." **I did not verify it** (no shell in this seat). If `read_file` answers `none` only for open/IO failure and passes invalid bytes through unchecked, then Heroes' existing choice is *Go's*, it already contradicts `spec § 3`, and consistency-with-what-exists argues for the opposite of what the brief implies. Settle it with a program that writes `\xFF\xFE` to a file and reads it back, before resting anything on that sentence.
2. **A language that shipped lossy-at-the-C-boundary and kept it deliberately** after a documented review — that would break the one-way-street claim, which rests on eleven rows and Swift's SE-0405 alone.
3. **A `derive`-style zero default for large arrays that a language later withdrew.** I found the reverse (Rust wants one and cannot get it); a withdrawal would be the counterweight to my "do not ship a stopgap" warning.

*Route, not evidence: the author's* Heroes of code *is what sent me to the Wirth/Ada line and to Swift's importer for this boundary. Nothing above is cited to it, and no `unverified` above became `verified` because of it.*

**Misstatements found in `00-shared.md`:** none factual that I could check. One framing gap, stated above — the document treats "what happens to invalid bytes" as one decision; every precedent I fetched treats termination and validity as two, and three of them (Ada, Go, Zig) give *different* answers on each axis.
