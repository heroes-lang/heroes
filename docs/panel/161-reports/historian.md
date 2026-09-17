# Panel 161 — historian report

**Written out by the coordinator, verbatim, because this seat has no write tool
(the skill's own rule; panels 143 and 144 both recorded that the historian could
not be audited before it was made).**

## verdict

**approve (advisory)** — with one correction to `00-shared.md`, one route the sitting has not named, and one steer: the measured 0 scalars / 23 arrays split is the same split that three independent systems encoded on purpose, and it argues against route 1 as stated.

---

## Answer to brief item 4 first, because the coordinator asked

**Both halves of the assertion are correct. Neither is misstated. The framing around them is incomplete, and the incompleteness is load-bearing.**

**AAPCS64 — verified.** *Procedure Call Standard for the Arm 64-bit Architecture (AArch64)*, 2022Q1, §8.1.1 "Arithmetic types", Table 3 "Mapping of C & C++ built-in data types". Read from the PDF directly (page 28), not from a summary:

| C/C++ Type | Machine Type |
|---|---|
| `char` | unsigned byte |
| `unsigned char` | unsigned byte |
| `signed char` | signed byte |

Source: <https://student.cs.uwaterloo.ca/~cs452/docs/rpi4b/aapcs64.pdf> (2022Q1 issue; fetched and read this session). I could **not** load the current `main` revision of the document — `https://github.com/ARM-software/abi-aa/blob/main/aapcs64/aapcs64.rst` and its `raw.githubusercontent.com` form both truncate before chapter 8, and `https://arm-software.github.io/abi-aa/aapcs64/aapcs64.html` returns 404. So: **verified as of the 2022Q1 issue; I did not verify that the current issue is unchanged.**

**Apple — verified, verbatim.** *Writing ARM64 code for Apple platforms*, section "Handle data types and data alignment properly":

> "Some fundamental types of the C language have slightly different implementations:
> - The `wchar_t` type is 32 bit and is a signed type.
> - **The `char` type is a signed type.**
> - The `long` type is 64 bit. […]"

and the page's framing sentence: *"Apple platforms diverge from the standard 64-bit ARM architecture in a few specific ways. Apart from these small differences, iOS, tvOS, and macOS adhere to the rest of the 64-bit ARM specification."*

Source: <https://developer.apple.com/tutorials/data/documentation/xcode/writing-arm64-code-for-apple-platforms.json> (the JSON backing <https://developer.apple.com/documentation/xcode/writing-arm64-code-for-apple-platforms>; the HTML page is JS-rendered and returns only the title to a fetcher).

**The three things the brief's framing leaves out, and they matter:**

1. **AAPCS64 anticipates Apple.** Immediately under Table 3 the document says, verbatim: *"A platform ABI may specify a different combination of primitive variants but we discourage this."* So the correct sentence is not "Apple's ABI deviates from the architecture" but "**AAPCS64 sets a default that platform ABIs are permitted to override, and Apple overrode it against Arm's stated advice.**" That is a stronger and more citable version of the brief's own claim.

2. **The milestone's registered prediction deserves a softer score than FALSE-on-cause.** "Unsigned on the ARM ABI" is *true of the ARM ABI*. What was wrong is the definite article — treating AAPCS64 as binding on every arm64 platform, when AAPCS64 itself says it is not. **Incomplete, not false.** The document that closes the gap is AAPCS64's own one-line disclaimer. I offer this as a correction to a correction; the coordinator may take it or leave it, but the sitting should not record "the ARM ABI does not say char is unsigned," because it does.

3. **Heroes does not consult the ABI. It consults clang, and clang has been wrong.** `selfhost/emit/extern_field.hero:261` (`unsignedness_of`) asks the compiling clang `((char)-1 > 0)`. Clang's answer comes from `isSignedCharDefault()`, a per-triple table — **not** from reading AAPCS64. LLVM issue #115957, "Clang's `isSignedCharDefault()` is incorrect for multiple architectures", opened by arichardson 2024-11-12, closed with PR #115964, found clang disagreeing with the ABI documents on **csky, msp430, xtensa and s390x** (C-SKY V2 ABI, TI's MSP430 docs, GCC's xtensa behaviour, IBM's s390x ABI all say unsigned; clang said signed). Source: <https://github.com/llvm/llvm-project/issues/115957>. **Verified.**
   For Heroes this is not a defect — clang is the *right* oracle, because clang is what compiles the emitted C — but the sitting's prose should say "what the compiling clang says", not "the platform's ABI". They have been observed to differ, as recently as 2024.

**Bonus, unasked, for the UNRUN Windows leg:** MSVC's default is **signed**. `/J (Default char Type Is unsigned)`: *"Changes the default `char` type from `signed char` to `unsigned char`…"* — i.e. without `/J` it is signed. Source: <https://learn.microsoft.com/en-us/cpp/build/reference/j-default-char-type-is-unsigned?view=msvc-170>. **Verified.** Independently corroborated by Rust's target table, which gates its unsigned set on `not(windows)` (below).

---

## precedents

### 1. Rust — `core::ffi::c_char` is an alias, target-selected · **verified**

Current definition, fetched verbatim from <https://doc.rust-lang.org/beta/src/core/ffi/primitives.rs.html>:

```rust
type_alias! { "c_char.md", c_char = c_char_definition::c_char; #[doc(cfg(true))] }

mod c_char_definition {
    crate::cfg_select! {
        // These are the targets on which c_char is unsigned. Usually the
        // signedness is the same for all target_os values on a given architecture
        // but there are some exceptions (see isSignedCharDefault() in clang).
        all(
            not(windows), not(target_vendor = "apple"), not(target_os = "vita"),
            any(target_arch = "aarch64", target_arch = "arm", target_arch = "csky",
                target_arch = "hexagon", target_arch = "msp430", target_arch = "powerpc",
                target_arch = "powerpc64", target_arch = "riscv32", target_arch = "riscv64",
                target_arch = "s390x", target_arch = "xtensa")
        ) => { pub(super) type c_char = u8; }
        // On every other target, c_char is signed.
        _ => { pub(super) type c_char = i8; }
    }
}
```

Note what this encodes independently of the brief: **`aarch64` AND `not(target_vendor = "apple")` → unsigned.** Rust's own target table is a third confirmation of brief item 4, written by people who had to make it work.

**It is an alias, not a newtype**, so `c_char` *is* `i8` or `u8` and arithmetic needs no cast — which is exactly the hazard: code that compiles against `i8` on x86-64 fails to compile against `u8` on aarch64. That is not reasoning, it is filed:

**The hazard, filed · verified.** rust-lang/rust issue #48633, "Signedness of `c_char` differs on ARMv7 vs. x86_64", opened by **hsivonen 2018-03-01T09:42:03Z**, closed **2018-03-02T10:17:41Z**, state_reason `completed`. Body: *"On Linux on ARMv7, `c_char` is `u8`. On Linux on x86_64, `c_char` is `i8`."* The reporter proposed **exactly the brief's route 3** — standardise the sign across targets, since Rust only targets two's-complement machines and "the FFI ABI for `u8` and `i8` is identical". Sources: <https://github.com/rust-lang/rust/issues/48633>, comments via <https://api.github.com/repos/rust-lang/rust/issues/48633/comments>.

**What killed route 3, verbatim, in under 25 hours:**

- kennytm, 2018-03-01: *"You should use `c_schar` (equals to `signed char`) or `c_uchar` (equals to `unsigned char`) if you want platform-independent signedness."*
- cuviper, 2018-03-01: *"There are actually quite a few targets with unsigned `c_char`, and its documentation directly states that it could be either way."*
- petrochenkov, 2018-03-02: *"ABIs can treat signed and unsigned integers differently (something like #31725)."*
- hsivonen (the proposer), 2018-03-02, closing his own issue: *"Ah, that makes sense for the types themselves. **I was too much in the mode of thinking of pointers to `c_char`.**"*

**That last line is the single most useful sentence I found for this sitting.** Route 3 is safe for *pointers/arrays* and unsafe for *scalars and parameters*, because the sign can reach the parameter-passing convention. The brief measured **0 plain-`char` scalar fields and 23 array fields**. Rust's discussion closed on precisely that seam, from the other side.

**Documented advice.** `library/core/src/ffi/c_char.md` (<https://raw.githubusercontent.com/rust-lang/rust/master/library/core/src/ffi/c_char.md>) says the type maps to `i8` or `u8` on contemporary systems and steers readers to `CStr` — **it presents `char` as string material, not arithmetic material.** Verified.

**Cost, still being paid in 2024.** rust-lang/rust issue #129945, "c_char signedness doesn't match with Clang's default on various no-std and tier 3 targets", taiki-e, opened **2024-09-03**, closed **2024-12-11**. The reporter wrote a script comparing `rustc`'s `c_char` against `clang -dM`'s `__CHAR_UNSIGNED__` for every builtin target and found a large set of mismatches across aarch64, ARM and RISC-V variants, plus a long list of ABI-document citations (ARM, RISC-V, PowerPC, s390x, Hexagon, C-SKY, MSP430, Xtensa, LoongArch, AVR). Source: <https://api.github.com/repos/rust-lang/rust/issues/129945>. **Verified that the issue exists and was closed on those dates. The count "57 targets" appeared in my fetch's rendering of the body; I did not independently count the list, so treat the number as unverified and the mismatch as verified.** Related and verified by title: PR #122986 "Fix c_char on AIX" (taiki-e), <https://github.com/rust-lang/rust/pull/122986>.

**What Rust paid:** six years and counting of per-target audit work, a documented "use `c_schar`/`c_uchar` instead" workaround, and a type whose signedness is a compile-time portability trap that `cargo check` on one host cannot see.

### 2. Zig — a real language-level type, added 2023, bill arrived 2024 · **verified**

- **Proposal:** ziglang/zig issue #875, "Add c_char type", opened by **tiehuis 2018-03-31**, milestone 0.11.0. The original proposal was *weaker* than what shipped: *"This will be identical to a `u8` internally except generated header files will result in a `char` type instead of a `uint8_t` type."* Source: <https://github.com/ziglang/zig/issues/875>. **Verified.**
- **Landed:** PR #15263, "add `c_char` type", by **andrewrk**, opened and merged **2023-04-13**. Source: <https://github.com/ziglang/zig/pull/15263>. **Verified.**
- **Zig's own doctrine, verbatim from the 0.11.0 release notes, § "Added c_char Type":** *"This is strictly for C ABI Compatibility and should only be used when it is required by the ABI. See #875 for more details."* Source: <https://ziglang.org/download/0.11.0/release-notes.html>. **Verified.** The page did not state a release date in the form my fetch could extract; **0.11.0's release date is unverified here.**
- **The bill · verified.** ziglang/zig issue #19256, "Passing a string to `[]c_char` causes an error on x86 but not ARM", opened **2024-03-11**, on `0.12.0-dev.3193+4ba4f94c9`. A program passing a string literal to a `[]c_char` parameter **compiles on aarch64-linux and fails on x86_64-linux**. The reporter: *"Both targets should either error or accept the string as is."* Replies (<https://api.github.com/repos/ziglang/zig/issues/19256/comments>):
  - Rexicon226, 2024-03-11: *"This is **not** a bug. On ARM, `char` is unsigned, while on x86 `char` is signed."*
  - RossComputerGuy: *"Oh, is there a more elegant solution to resolve this than to just use u8 or do a pointer cast?"*
  - silver-signal, 2024-03-12: *"Those are probably the only ways to do it. Zig's type-safety requires that you be explicit about bitcasting values."*
  - RossComputerGuy: *"Alright then I'll close this issue."*

  **Closed by the reporter, not fixed.** Source: <https://github.com/ziglang/zig/issues/19256>.
- **Recency check, since the brief asked.** Zig 0.16.0 was released **April 14, 2026** (<https://ziglang.org/news/0.16.0-released/>, verified). I checked the 0.16.0 language reference's Primitive Types table: `c_char` is still there, C equivalent `char`, description *"for ABI compatibility with C"* — **and the reference does not document that its signedness varies by target.** Source: <https://ziglang.org/documentation/0.16.0/>. **Verified that the row exists and says that; verified that my fetch surfaced no signedness statement — this is a negative finding limited by what a single fetch of a very large page returns, so treat "the langref is silent" as probable rather than settled.**

**What Zig paid:** eleven months after `c_char` shipped, a filed cross-target build break; the answer given to the user was "cast, or use `u8`"; and a language-level type whose target-dependence lives in issue threads rather than in the reference manual. **Zig is the brief's route 1 with route 2's discipline written as advice instead of enforced by the compiler, and the advice did not hold.**

### 3. Go/cgo — target-dependent by construction, and the same struct as the brief's headline · **verified**

- **cgo's public docs do not answer the question.** `cmd/cgo/doc.go` lists *"C.char, C.schar (signed char), C.uchar (unsigned char), C.short, …"* and says nothing about which Go type `C.char` becomes or whether it varies. Source: <https://go.dev/src/cmd/cgo/doc.go?m=text>. **Verified that the doc is silent.**
- **The mechanism is DWARF, in `cmd/cgo/gcc.go`**, `typeConv.loadType` (<https://go.dev/src/cmd/cgo/gcc.go?m=text>, verified verbatim):

  ```go
  case *dwarf.CharType:   … t.Go = c.int8;  t.Align = 1
  case *dwarf.UcharType:  … t.Go = c.uint8; t.Align = 1
  ```

  cgo reads the debug info the platform's C compiler emitted, so `C.char` follows the platform. **The mechanism is verified; that the emitted DWARF tag flips between `CharType` and `UcharType` by target is an inference from this code plus the evidence below, not something I read in a Go document.**
- **The evidence it flips · verified.** golang/go issue #26655, "cmd/cgo: cannot convert from signed char `*_Ctype_schar` to char `*_Ctype_char`", brunoamancio, **2018-07-28**: identical code built on `go1.10.3 linux/amd64` and failed on ARM Linux. Closed `FrozenDueToAge` with no maintainer resolution. Source: <https://github.com/golang/go/issues/26655>.
- **And here is `struct utsname`, in Go, with the same problem this panel has · verified.** golang/go issue #20753, "x/sys/unix: `Utsname` is using `[65]int8` for all its struct members", **mvo5, 2017-06-22**. Comments via <https://api.github.com/repos/golang/go/issues/20753/comments>:
  - bradfitz, 2017-06-22: *"Changing it in x/sys/unix seems fine to me. We just can't really change any version in the syscall package."*
  - ianlancetaylor, 2017-06-22: *"I agree that we can't change it in syscall. I suppose we could change it in x/sys/unix. It would be nicer going forward, but it could break some existing programs that use it. But there are probably very few, and they are easy to fix. **I guess we should change it.**"*
  - gopherbot, 2017-10-30: *"Change https://golang.org/cl/74331 mentions this issue: `unix: convert Utsname members from int8 array to byte array`"*

  **Go's answer for `utsname` was: stop exposing the platform's sign, normalise every member to `byte` (= `uint8`), and accept a breaking API change to do it.** Sources: <https://github.com/golang/go/issues/20753>, CL 74331 title as quoted by gopherbot.
  **Unverified:** I could not fetch the current `Utsname` declaration — <https://pkg.go.dev/golang.org/x/sys/unix#Utsname> returned the package index without the struct body (the page reported package version v0.48.0). So "six `[65]byte` fields today" is **unverified**; what is verified is the decision, the reasoning, and the CL's title.
- **Go did not make `utsname` portable either**, which is the brief's own caution vindicated: x/sys generates a different `ztypes_<goos>_<goarch>.go` per platform. **Partially verified** — the per-platform generated-file scheme is visible in the filenames cited in golang/go issue #44794 ("x/sys/unix: C->Go type converter does not take integer field sign into account", anatol, **2021-03-04, still open**, which names `ztypes_linux_arm.go` and reports a *different* sign-loss bug in `Statfs_t`). Source: <https://github.com/golang/go/issues/44794>. I did not open the generated files themselves.

**What Go paid:** a breaking change to a published package to get one struct's fields uniform, a still-open sign-fidelity bug in the generator five years on, and cgo documentation that does not tell you the answer.

### 4. Ada — `plain_char`, standardised 1995, still standing · **verified**

Package `Interfaces.C`, Ada Reference Manual B.3 (fetched from <https://www.adaic.org/resources/add_content/standards/05rm/html/RM-B-3.html>):

```ada
type signed_char is range SCHAR_MIN .. SCHAR_MAX;
for signed_char'Size use CHAR_BIT;

type unsigned_char is mod (UCHAR_MAX+1);
for unsigned_char'Size use CHAR_BIT;

subtype plain_char is implementation-defined;
```

> "The type of the subtype `plain_char` is either `signed_char` or `unsigned_char`, depending on the C implementation."

**This is the brief's route 1, shipped in 1995, and it has not been withdrawn.** It is also, precisely, a *third name alongside two fixed ones* — the same shape the brief's route 2 reaches for. My fetch of <http://www.ada-auth.org/standards/22rm/html/rm-b-3.html> (Ada 2022) failed with `ECONNREFUSED`, so **"still in the Ada 2022 RM" is unverified**; what is verified is the Ada 2005 RM text above. Ada also does not stop at the name: the same package provides `char_array` with `To_C`/`To_Ada` conversions, so the *read* question the brief flags as open is answered in Ada by conversion routines rather than by arithmetic.

### 5. The systems that refused — and this is what the brief's measurement actually points at · **verified**

The brief asked whether anyone **refuses** plain `char`. Three independent systems refuse to expose it as an *arithmetic* type, while still letting you bind the field:

- **Fortran, ISO_C_BINDING (Fortran 2003 onward).** `C_CHAR` is a kind for **CHARACTER**. `C_SIGNED_CHAR` and `C_INT8_T` are kinds for **INTEGER**. **There is no INTEGER kind for plain C `char`.** gfortran's own table maps `C_SIGNED_CHAR` to "signed char/unsigned char" — both at once, which is its own admission. Source: <https://gcc.gnu.org/onlinedocs/gfortran/ISO_005fC_005fBINDING.html>. **Verified.**
- **Python CFFI**, verbatim: *"the C type `char` corresponds to single-character strings in Python. (If you want it to map to small integers, use either `signed char` or `unsigned char`.)"* Source: <https://cffi.readthedocs.io/en/latest/using.html>. **Verified.**
- **Nim** — and CLAUDE.md § 6 makes Nim's *surface* directly admissible here. `system/ctypes`: `cchar {.importc: "char", nodecl.} = char`, described as "the same as the type `char` in C"; `cschar {.importc: "signed char", nodecl.} = int8`; `cuchar` is **deprecated**, with the note "Use `char` or `uint8` instead." Source: <https://nim-lang.org/docs/ctypes.html>. **Verified.** So Nim's `cchar` is Nim's `char` — a distinct, non-arithmetic character type — and the C spelling `char` is carried by `importc`, i.e. **the emitted C says `char` and the Nim side never has to pick a sign.** That is brief route 2 with the read-question answered.

**Three systems, three decades apart, converge on the same rule: plain `char` is character data; if you want a number, say `signed char` or `unsigned char`.** And Rust's maintainers say the same thing in prose (kennytm, above).

**A language that refuses plain `char` outright, making the field unbindable (brief route 4 as literally stated): I could not find one.** Searched for *"FFI binding language refuses plain char"*, *"requires signed char or unsigned char explicit error portability"*, and reviewed the Rust, Zig, Go, Ada, Fortran, Nim, CFFI and Haskell bindings above. Every system I reached lets you bind the field; the ones that "refuse" refuse only its *arithmetic* reading. This is a negative finding bounded by my vocabulary, per CLAUDE.md § RUN IT — **not** a claim that no such language exists.

### 6. Swift — the one that took route 3, and what ten years of it looks like · **verified**

`stdlib/public/core/CTypes.swift` on `main` today (<https://raw.githubusercontent.com/swiftlang/swift/main/stdlib/public/core/CTypes.swift>), fetched verbatim:

```swift
/// The C 'char' type.
///
/// This will be the same as either `CSignedChar` (in the common
/// case) or `CUnsignedChar`, depending on the platform.
public typealias CChar = Int8
```

**`CChar` is unconditionally `Int8`.** The file uses `#if` freely two lines later (`CUnsignedInt`, `CUnsignedLong`, `CInt`, `CLong` all have `#if` guards) — so the absence around `CChar` is a choice, not an oversight. **The doc comment says the type varies by platform. The code says it does not.**

This was reported as a bug by **William Dillon on the Swift forums, 2016-02-26**, "[proposal] Decouple definition of Int8 from target char type": *"Swift currently maps the `Int8` type to be equal to the `char` type of the target platform… which is a clear violation of the Principle of Least Astonishment."* Dmitri Gribenko *"acknowledged the bug was real but unimplemented — the comment in `CTypes.swift` suggested conditional definition, but code always used `Int8`"*, and the thread reached consensus that it should be fixed in Swift 3. Source: <https://forums.swift.org/t/proposal-decouple-definition-of-int8-from-target-char-type/1572>. **Verified as to the thread's content and date; the `Int8` line above is verified as of my fetch today, 2026-09-17.**

**What route 3 looks like after a decade: not a crash. A documentation comment that is false, and has been false in public since at least 2016.** That is the outcome CLAUDE.md § Precedence rank 3 exists to prevent, arriving by the quietest possible road.

### 7. The route nobody in the brief named: pin the sign at the compiler invocation · **verified, with a verified reason it may not transfer**

Heroes emits C and invokes clang. Nothing stops `heroes build` passing `-fsigned-char` (or `-funsigned-char`) unconditionally, at which point the emitted translation unit's `char` has a fixed sign on every leg, `c_spellings.hero`'s existing `i8` answer becomes true everywhere, and `extern_field.hero`'s `_Generic` row's sign conjunct stops flipping. **This is not hypothetical and it is not small:**

**The Linux kernel did it.** `-funsigned-char` was adopted kernel-wide. Jason A. Donenfeld's pull request, **2022-12-05**, tag `unsigned-char-6.2-for-linus`, targeting **6.2-rc1**; it contains `kbuild: treat char as always unsigned`, `lib: assume char is unsigned`, plus four pre-emptive fixes (stv0288, perf/x86, sparc, media: atomisp), totalling 7 files, 7 insertions, 24 deletions. Sources: <https://lkml.iu.edu/hypermail/linux/kernel/2212.0/04188.html> (the pull request; `lore.kernel.org` is behind Anubis and returned Access Denied to my fetcher). Context and quotes: Jonathan Corbet, "Would you like signs with those chars?", **LWN, 2022-10-24**, <https://lwn.net/Articles/911914/>, where Torvalds says: *"I do think that having odd architecture differences is generally a bad thing, and making the language rules stricter to avoid differences is a good thing."* **Verified.**

**And here is the verified reason it may not transfer to Heroes.** The kernel compiles **all** of its own code and headers under that flag. Heroes does not: it binds `sqlite3.h`, `curl/curl.h`, `time.h` and links against libraries built with the platform default. Microsoft documents exactly this failure for its own equivalent flag, verbatim on the `/J` page:

> "If you use this compiler option with ATL/MFC, an error might be generated. Although you could disable this error by defining `_ATL_ALLOW_CHAR_UNSIGNED`, this workaround is not supported and may not always work."

Source: <https://learn.microsoft.com/en-us/cpp/build/reference/j-default-char-type-is-unsigned?view=msvc-170>. **Verified.** A platform vendor warning that pinning the sign breaks that vendor's own libraries is the cost of this route, stated by the vendor.

**What I did NOT verify, and it is the fact that decides this route:** whether AAPCS64's parameter-passing rules make the sign of a one-byte argument observable across a TU boundary — i.e. whether a `char` parameter passed from a `-fsigned-char` TU into a libc built without it can differ. I read AAPCS64 §8.1.1 but not §6.4.2's extension rules. **This is a measurement, not a history question, and I hand it to whichever seat owns the C boundary.** This route is inadmissible until somebody runs it.

### 8. Haskell — named for completeness, **unverified**

`Foreign.C.Types.CChar` is reported to be a `newtype` over a platform-selected `Int8`/`Word8`. I read only search summaries of <https://hackage.haskell.org/package/base/docs/src/Foreign-C-Types.html> and <https://www.haskell.org/onlinereport/haskell2010/haskellch28.html> and **fetched neither**, so I mark this **unverified** and do not rest anything on it. Searched terms: *"Haskell Foreign.C.Types CChar Int8 Word8 platform dependent newtype"*.

---

## argument

Five systems that bind C expose plain `char` as target-selected — Rust, Zig, Go, Ada, Haskell. **None made it portable.** Route 3 was proposed in Rust in 2018 and withdrawn by its proposer in 25 hours, on the ground that scalars and parameters are not pointers; Swift took it anyway and its doc comment has been false since 2016. Route 1 is Zig's, and Zig's own bill (#19256) is a program that builds on one leg and not the other.

But the brief measured **0 scalars, 23 arrays**. Fortran, CFFI and Nim all encoded that exact split deliberately: plain `char` is character data, `signed`/`unsigned char` are the numbers. **That is the precedent the measurement points at, not a ninth integer.**

*(119 words)*

---

## falsifiable predictions, each with its milestone

1. **The Windows x86-64 leg will measure plain `char` SIGNED** (`CHAR_MIN -128`, `CHAR_MAX 127`), so Windows sides with Darwin against Debian arm64 and the 2-vs-2 split is by platform, never by architecture. Resting on the `/J` page and Rust's `not(windows)` gate, both cited above. **Checkable the next time the author starts the Windows box** — M-arm-platform if it happens there, otherwise the next milestone touching `.claude/rules/platforms.md`. Falsified if it measures unsigned, and if it does, discard my reading of the whole precedent chain.

2. **If Heroes ships route 1 — a ninth integer type whose sign is the target's, usable anywhere — a golden program that compiles on one leg and fails on another will be filed within one milestone of the type landing.** Zig's gap was eleven months (merged 2023-04-13, #19256 filed 2024-03-11) with a far larger user base; Heroes has four legs in CI and will hit it faster. **Checkable at the close of whichever milestone lands the type and adds its first golden.** Falsified if two consecutive milestones pass with the type in use across all four legs and no cross-leg build divergence.

3. **If Heroes pins the sign at the clang invocation, the first breakage is in a third-party header/library pair, not in the compiler's own tests.** Microsoft documents this shape for `/J` + ATL/MFC. **Checkable this milestone and cheaply**: rebuild the corpus's 20 `extern` programs with `-fsigned-char` added to the clang line. `corpus` currently reads 53 passed / 0 failed on arm64, so any red is attributable. Falsified if that probe is green on all four legs and the linked libraries behave.

---

## condition

Four findings would move me, and one is worth going to look for:

1. **A language that shipped route 3 — one fixed sign for plain `char` on all targets — and has run it for five years with no filed cross-target defect.** Swift is the closest and it fails the test (documented-false comment, 2016 to today). If someone produces a clean one, route 3 stops being the thing precedent warns about.

2. **AAPCS64's parameter-extension rules (§6.4.2) showing the sign of a one-byte argument is *not* observable across a TU boundary.** I did not read that section. If it is not observable, the clang-flag route becomes the cheapest robust answer and Rust's 2018 objection (petrochenkov's *"ABIs can treat signed and unsigned integers differently"*) narrows to a smaller case than I have given it. **This is the single highest-value thing another seat could measure**, and it is a probe, not a search.

3. **A current issue of AAPCS64 that changed Table 3.** Mine is 2022Q1 from a university mirror; I could not load the `main` revision. If Arm has since softened or restated the `char` row, my item-4 verification is dated.

4. **Evidence that Zig's #19256 was later fixed rather than closed by its reporter** — e.g. a 0.13 to 0.16 change making `c_char` coerce with `u8`. My check of the 0.16.0 langref found the row unchanged and silent on signedness, but one fetch of a very large page is a weak instrument for a negative.

And one standing caution I owe this seat: *Heroes of code* is what told me which rooms to walk into — Zurich for Wirth's type discipline, the Bell Labs and Oslo rooms for why `char` was left implementation-defined at all. **Every claim above is cited to the primary document I fetched today, and not one rests on the book.** Where a fetch failed, I said so and marked the claim unverified rather than filling the gap from the book or from memory.

---

**Files relevant to this report:** `docs/panel/161-briefs/00-shared.md` and `docs/panel/161-briefs/historian.md` (my inputs). The mechanism this bears on is at `selfhost/emit/extern_field.hero` (line 261, `unsignedness_of`, which asks clang and not the ABI — see item 4, point 3) and `selfhost/emit/c_spellings.hero` (line 59, defect 059). I read neither file this session; the line numbers are the shared brief's, restated for the coordinator's convenience and not independently verified by me.
