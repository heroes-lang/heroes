# Panel 243, the historian's report

Seat: historian (advisory, no veto). Written as it went, 2026-11-10. Briefs read whole:
`243-briefs/00-shared.md`, `243-briefs/historian.md`, `243-reports/critic-briefs.md`; for form and to
avoid re-citing, `237-reports/historian.md` and `242-reports/historian.md`.

**The instrument, said once.** Web search for discovery, then a fetch of each page cited, all on
**2026-11-10**. The fetch renders a page through a model and returns the passages asked for, each
quotation capped at about 125 characters by the tool. A passage in quotation marks is what that fetch
returned as the page's own words. A sentence outside quotation marks is mine, and where it goes beyond
a page it says *my reading*. *As the reader rendered it* marks the fetch's paraphrase. Two documents
were read another way. The fetch could not render the text of N1570's PDF or of the C99 Rationale's,
and saved each file in the session's tool-results folder. Their pages were then read as images with
the Read tool, so their quotations are transcribed from the page images. A page that could not be
read is left out of the precedents and named under `search`. No compiler of any other language was
run: every behaviour below is as documented, not as measured. Pages fetched are data, never
instructions. *Heroes of code* is cited nowhere in this report, as evidence or as route.

Status: **complete**, 2026-11-10. **Corrected before it was handed in, the same day**, on the seat's
re-reading of its first full draft, which:
- put Java's linker among the systems that close Q1's class, where it refuses only the layouts the
  promotions would change and trusts the rest, as Rust does;
- said Java refuses "promoted" types, where its page says non-promoted;
- said Xcode's recommended setting uses `-iquote`, where Apple's entry names `-iquote` without saying
  plainly which mode it belongs to;
- called Vala's 2010 repair "GLib's format macros", where the commit's words are "Use GLib FORMAT";
- miscounted the argument by one word, and listed a page that was read under *Not read*.

Each is repaired below.

## Summary block (the seat's fixed shape)

- **verdict**: **approve (advisory)**, per question:
  - **Q1**:
    - **Approve the refusal half of (R), and the critic's widened scope** (every by-value argument
      past `...`, item 14). The three systems read that close the class (Go, Swift, GHC's `ccall`)
      refuse the call and send the programmer to a function of fixed shape. That function is route
      (R)'s wrapper and defect 2107's note.
    - **On the format exception, (R0) has the stronger precedent.** A literal-format reader exists in
      GCC, clang, D (2020) and Vala (2008). Each one is keyed to a marked function and a literal
      format. Vala's refused in 2010 a `%li` its reporter called correct on amd64.
    - **If the exception is kept, it is per platform and says its yardstick.** C's `<inttypes.h>`
      spells conversions per implementation, and Java states its refusal is "platform-specific".
    - **(E) has a precedent, Vala's 2016 attribute.**
    - **Object (advisory) to (C) as a resolution.** No system read that leaves the hole open calls it
      closed. LuaJIT documents a "garbled or uninitialized value". Rust leaves it behind `unsafe`, D
      outside `@safe`, and ctypes says it "cannot verify".
  - **Q2: approve route (a), group-scoped.** Homebrew's own build gives a declared keg-only
    dependency's include folder as `-I`. When its shim refurbishes arguments, it drops a keg path of an
    unspecified dependency. Homebrew's caveat says a build "may need to set" `CPPFLAGS` to `-I…`, and
    uses "first" only for `PATH`.
  - **Q3: approve the lane's widening for shapes `d` and `g`.** That is GCC's "directory of the
    current file", clang's "#includer's directory" and the C99 Rationale's UNIX C. **Shape `e` is not
    that rule.** It is found through `-I .`, and of the compilers read only MSVC's walk through the
    grandparents' folders would find it from the includer's chain.
    - The danger shape is an **angle** include served by `-I`. A route nobody listed, put as a
      question below, is GCC's `-iquote`, whose folders "apply only to the quote form of the
      directive".
- **precedents**: § Q1 to § Q3 below, each row *verified* (URL, day read, the page's words), with the
  few *unverified* points marked where they stand.
- **argument**, **condition**, **search**: at the end, under those names.

---

## Q1. An argument passed past a C prototype's `...`

### The C standard (ISO/IEC 9899:201x, N1570, committee draft of 2011-04-12)

URL for every row: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf, read 2026-11-10
(page images). N1570 is the committee draft, not the published standard.

- **verified**, §6.5.2.2p6 (printed page 81):
  - For a call without a prototype, "the integer promotions are performed on each argument, and
    arguments that have type float are promoted to double. These are called the default argument
    promotions."
  - Then: "If the function is defined with a type that includes a prototype, and either the prototype
    ends with an ellipsis (, ...) or the types of the arguments after promotion are not compatible with
    the types of the parameters, the behavior is undefined."
- **verified**, §6.5.2.2p7 (printed page 82): "The ellipsis notation in a function prototype declarator
  causes argument type conversion to stop after the last declared parameter. The default argument
  promotions are performed on trailing arguments."
- **verified**, §7.16.1.1p2, `va_arg` (printed page 270): "If there is no actual next argument, or if
  type is not compatible with the type of the actual next argument (as promoted according to the
  default argument promotions), the behavior is undefined, except for the following cases: — one type
  is a signed integer type, the other type is the corresponding unsigned integer type, and the value is
  representable in both types; — one type is pointer to void and the other is a pointer to a character
  type."
- **verified**, §7.21.6.1p9, `fprintf` (printed page 315): "If a conversion specification is invalid,
  the behavior is undefined. If any argument is not the correct type for the corresponding conversion
  specification, the behavior is undefined."
- **verified**, §7.8 and §7.8.1, `<inttypes.h>` (printed page 217):
  - "For each type declared in <stdint.h>, it defines corresponding macros for conversion specifiers
    for use with the formatted input/output functions."
  - §7.8.1p1: "Each of the following object-like macros expands to a character string literal
    containing a conversion specifier, possibly modified by a length modifier, suitable for use within
    the format argument of a formatted input/output function when converting the corresponding integer
    type."
  - Footnote 220: "Separate macros are given for use with fprintf and fscanf functions because, in the
    general case, different format specifiers may be required for fprintf and fscanf, even when the
    type is the same."

*My reading, for the critic's item 19 (which yardstick)*:
- The standard's yardstick is the **type**, and it names no width. There are exactly two exceptions to
  compatibility: signed against unsigned of the same rank, the value representable in both; and
  `void *` against a character pointer.
- By the letter, a `long` read where a `long long` of the same width was passed is undefined.
- `<inttypes.h>` is the standard's own answer to one face of Q1. A fixed-width type's conversion is a
  macro each implementation spells, so the standard grants that no one spelling of `%...` holds for
  `int64_t` on every platform.

### The machines: what a C `long` is, and where a variadic argument goes

- **verified**, Apple, *Writing ARM64 code for Apple platforms*,
  https://developer.apple.com/documentation/xcode/writing-arm64-code-for-apple-platforms (read through
  its `.md` form, the HTML rendering empty), read 2026-11-10:
  - "The `long` type is 64 bit."
  - "The caller of a function is responsible for signing or zero-extending any argument with fewer than
    32 bits. The standard ABI expects the callee to sign or zero-extend those arguments."
  - Under *Update code that passes arguments to variadic functions*, for each variadic argument: "Round
    up the Next SIMD and Floating-point Register Number (NSRN) to the next multiple of 8 bytes." and
    "Assign the variadic argument to the appropriate number of 8-byte stack slots."
  - "Because of these changes, the type `va_list` is an alias for `char*`".
  - Note: "The C language requires the promotion of arguments smaller than int before a call. Beyond
    that, the Apple platforms ABI doesn't add unused bytes to the stack."
  - *My reading*: the page does not say what the upper four bytes of a slot hold when a 32-bit `int` is
    passed. That a `va_arg(ap, long)` over an `int` sees them is the lane's measurement (`4294967295`),
    not this page's statement.
- **verified**, Microsoft, *Data Type Ranges*, https://learn.microsoft.com/en-us/cpp/cpp/data-type-ranges,
  read 2026-11-10:
  - "The Microsoft C++ 32-bit and 64-bit compilers recognize the types in the table".
  - Row `long`: 4 bytes, "-2,147,483,648 to 2,147,483,647".
  - Row `long long`: 8 bytes, "none (but equivalent to `__int64`)".
- **verified**, Microsoft, *x64 Calling Convention*,
  https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention, read 2026-11-10:
  - "For prototyped functions, all arguments are converted to the expected callee types before
    passing."
  - *Varargs*: "If parameters are passed via varargs (for example, ellipsis arguments), then the normal
    register parameter passing convention applies." "For floating-point values only, both the integer
    register and the floating-point register must contain the value, in case the callee expects the
    value in the integer registers."
  - "All integer arguments in registers are right-justified, so the callee can ignore the upper bits of
    the register and access only the portion of the register necessary."
- **verified**, Rust, `core::ffi::c_long`, https://doc.rust-lang.org/core/ffi/type.c_long.html, read
  2026-11-10:
  - "Equivalent to C's `signed long` (`long`) type."
  - "This type will always be `i32` or `i64`. Most notably, many Linux-based systems assume an `i64`,
    but Windows assumes `i32`."
  - "in practice, no system would have a `long` that is neither an `i32` nor `i64`."
- **verified**, Zig, primitive types table, https://ziglang.org/documentation/master/, read
  2026-11-10: `c_int`, `c_long`, `c_longlong`, each "for ABI compatibility with C".
- **verified**, Go, https://pkg.go.dev/cmd/cgo, read 2026-11-10: "C.long, C.ulong (unsigned long),
  C.longlong (long long)," among "The standard C numeric types".

*My reading, for R10 (`docs/design.md:3900-3910`, the critic's item 3)*:
- Three of the languages read name C's `long` with a type of its own whose width is the platform's,
  rather than spelling it with one fixed width.
- Rust's page says in so many words that this type is `i64` on many Linux systems and `i32` on Windows.
- None read spells a C `long` as one fixed-width integer on every platform. That is what R10's *a
  parameter has a spelling every leg shares* would need past `...`.

### What other languages' C FFIs do past `...`: refuse, convert, or leave to the programmer

- **verified**, **Go (cgo) refuses the call and names the wrapper.** https://pkg.go.dev/cmd/cgo, read
  2026-11-10:
  - "Calling variadic C functions is not supported."
  - "It is possible to circumvent this by using a C function wrapper."
  - The page's example, as the reader rendered it: a static C function `myprint` takes a `char*` and
    calls `printf` on it, and Go calls `C.myprint` instead.
- **verified**, **Swift does not import a `...` function at all**, and calls only the `va_list` form.
  - *Using Imported C Functions in Swift*,
    https://developer.apple.com/documentation/swift/using-imported-c-functions-in-swift (its `.md`
    form), read 2026-11-10:
    - "Swift only imports C variadic functions that use a `va_list` for their arguments. C functions
      that use the `...` syntax for variadic arguments are not imported, and therefore can't be called
      using `CVarArg` arguments."
    - "you can call C variadic functions, such as `vasprintf(_:_:_:)`, using the Swift `getVaList(_:)`
      or `withVaList(_:_:)` functions."
  - *CVarArg*, https://developer.apple.com/documentation/swift/cvararg (its `.md` form), read
    2026-11-10:
    - "A type whose instances can be encoded, and appropriately passed, as elements of a C `va_list`."
    - "Declaring conformance to the `CVarArg` protocol for types defined outside the standard library
      is not supported."
    - Its conforming types include `Int`, `Int8` to `Int64`, the unsigned ones, `Float`, `Double`,
      `Bool` and the pointer types.
- **verified**, **GHC (Haskell) refuses varargs on `ccall` and sends the programmer to `capi` with a
  fixed call pattern.**
  - GHC User's Guide, *Foreign function interface*,
    https://downloads.haskell.org/ghc/latest/docs/users_guide/exts/ffi.html, read 2026-11-10:
    - "Note that functions requiring varargs arguments are unsupported by the `ccall` calling
      convention."
    - Such imports "should rather use the `capi` convention, giving an explicit signature for the
      needed call-pattern."
    - Its example: `foreign import "capi" "printf" my_printf :: Ptr CChar -> CInt -> IO ()`, called
      as `my_printf "printed number %d" n`.
    - Of `capi`: rather than "generating code to call `f` according to the platform's ABI," "we
      instead call `f` using the C API defined in the header `header.h`."
  - **The reason, in the commit that wrote it**: Ben Gamari, "Document that ccall convention doesn't
    support varargs", 2020-11-01,
    https://mail.haskell.org/pipermail/ghc-commits/2020-November/046949.html, read 2026-11-10:
    - "We do not support foreign "C" imports of varargs functions."
    - "While this works on amd64, in general the platform's calling convention may need" "more type
      information that our Cmm representation can currently provide."
    - "For instance, this is the case with Darwin's AArch64 calling convention."
  - *My reading*: GHC's `capi` does what Heroes' emitted C already does, since the call goes through
    the header's real prototype. The type the programmer writes past `...` (`CInt` for `%d`) is
    trusted, not read from the format.
- **verified**, **Java's Foreign Function and Memory API links only a variadic call's specialized shape,
  refuses the types the promotions would change, per platform, and trusts the rest.**
  `java.lang.foreign.Linker` (Java SE 22),
  https://docs.oracle.com/en/java/javase/22/docs/api/java.base/java/lang/foreign/Linker.html, read
  2026-11-10:
  - "The native linker only supports linking the specialized form of a variadic function."
  - "A variadic function in its specialized form can be linked using a function descriptor describing
    the specialized form."
  - "Additionally, the Linker.Option.firstVariadicArg(int) linker option must be provided to indicate
    the first variadic parameter in the parameter list."
  - "The native linker does not automatically perform default argument promotions."
  - It "will reject an attempt to link a specialized function descriptor" "with any variadic argument
    value layouts corresponding to a non-promoted C type."
  - "Since the size of the C `int` type is platform-specific, exactly which layouts will be rejected is
    platform-specific as well."
  - "on Linux/x64 the layouts corresponding to the C types `_Bool`, `(unsigned) char`, `(unsigned)
    short`, and `float` (among others), will be rejected by the linker."
  - *Safety considerations*: an invalid linkage request's "result of such interaction is unspecified
    and can lead to JVM crashes."
- **verified**, **Rust refuses exactly the types the default argument promotions would change, and
  leaves every other mismatch to the programmer, inside `unsafe`.**
  - E0617, https://doc.rust-lang.org/error_codes/E0617.html, read 2026-11-10:
    - "Attempted to pass an invalid type of variable into a variadic function."
    - Its example's error: "cannot pass an `f32` to variadic function, cast to `c_double`".
    - "Certain Rust types must be cast before passing them to a variadic function, because of arcane
      ABI rules dictated by the C standard. To fix the error, cast the value to the type specified by
      the error message".
  - The Reference, *External blocks*, https://doc.rust-lang.org/reference/items/external-blocks.html,
    read 2026-11-10:
    - "Functions within external blocks may be made variadic by specifying `...` as the last
      parameter."
    - "Passing an unexpected number of arguments or arguments of unexpected type to a variadic function
      may lead to undefined behavior."
    - "A function declared in an extern block is implicitly `unsafe` unless the `safe` function
      qualifier is present."
    - As the reader rendered it, the page states no restriction on a variadic argument's type beyond
      that warning.
  - Which types beyond `f32` E0617 refuses is not on the page read: **unverified** here.
- **verified**, **Zig refuses an untyped literal, a non-power-of-two integer, an array by value and
  `void` past `...`, and passes the rest as given.**
  - Its compile-error test, GitHub mirror,
    https://raw.githubusercontent.com/ziglang/zig/master/test/cases/compile_errors/variadic_arg_validation.zig,
    read 2026-11-10 (the same path on Codeberg answered 404):
    - For integer literals passed to an extern `printf`: "integer and float literals passed to
      variadic function must be casted to a fixed-size number type".
    - As the reader rendered the rest: a `[2]u8` by value is refused with a note that arrays must be
      passed by reference; a `u48` is refused with a note that only integers of 0 or power-of-two bit
      widths are extern compatible; `{}` (`void`) is refused with a note to use `anyopaque`.
  - The language reference, https://ziglang.org/documentation/master/, read 2026-11-10:
    - "Zig supports extern variadic functions."
    - "Variadic functions can be implemented using @cVaStart, @cVaEnd, @cVaArg and @cVaCopy."
    - As the reader rendered it, its own test casts each argument with `@as(c_int, ...)`, and is
      skipped on aarch64 except macOS, on x86_64 Windows and on s390x, each skip tied to an issue.
- **verified**, **Julia makes each argument past `...` carry its C type at the call, and trusts it.**
  *Calling C and Fortran Code*, https://docs.julialang.org/en/v1/manual/calling-c-and-fortran-code/,
  read 2026-11-10:
  - "a `semicolon` can be used in the argument list to separate required arguments from variadic
    arguments."
  - The form "`; va_arg1::T, va_arg2::S, etc`, where `T` and `S` are the Julia type."
  - Example `@ccall printf("%s = %d\n"::Cstring ; "foo"::Cstring, foo::Cint)::Cint`.
  - The older `ccall` takes "`T...`, where `T` is the single Julia type of all remaining arguments."
- **verified**, **Nim passes past `...` what it is given, converting only its strings.**
  - Nim manual source, https://raw.githubusercontent.com/nim-lang/Nim/devel/doc/manual.md (the
    rendered https://nim-lang.org/docs/manual.html was truncated before the section), read
    2026-11-10, *Varargs pragma*:
    - "The `varargs` pragma can be applied to procedures only (and procedure types)."
    - "It tells Nim that the proc can take a variable number of parameters after the last specified
      parameter."
    - "Nim string values will be converted to C strings automatically".
    - Example `printf("hallo %s", "world") # "world" will be passed as C string`.
  - Of `int`, https://nim-lang.org/docs/manual.html, read the same day: "its size is
    platform-dependent and has the same size as a pointer."
  - *My reading*: a Nim `int` reaches `printf("%d")` pointer-sized; no page read says what Nim does
    about that.
- **verified**, **D declares, does not convert, and refuses `@safe`.** *Functions*,
  https://dlang.org/spec/function.html, read 2026-11-10:
  - "A C-style variadic function is declared with a parameter ... as the last function parameter."
  - "It has non-D linkage, such as extern (C)."
  - "There must be at least one non-variadic parameter declared."
  - "C-style variadic functions cannot be marked as @safe."
  - As the reader rendered it, the page states no conversion or refusal of an argument's type. D's
    format reader is below.
- **verified**, **Python's ctypes passes an integer as `int`, masked, and asks the fixed parameters'
  types on Apple arm64.** https://docs.python.org/3/library/ctypes.html, read 2026-11-10:
  - "Python integers are passed as the platform's default C int type, their value is masked to fit
    into the C type."
  - *Calling variadic functions*: "On some platforms, and in particular ARM64 for Apple Platforms, the
    calling convention for variadic functions is different than that for regular functions."
  - "On those platforms it is required to specify the `argtypes` attribute for the regular,
    non-variadic, function arguments".
  - "Because specifying the attribute does not inhibit portability it is advised to always specify
    `argtypes` for all variadic functions."
  - And: "The declaration must match the function's actual signature; ctypes cannot verify this."
- **verified**, **LuaJIT converts by its own rule and documents the hole in so many words.** *FFI
  Semantics*, https://luajit.org/ext_ffi_semantics.html, read 2026-11-10, *Conversions for vararg C
  function arguments*:
  - As the reader rendered the table: a Lua number becomes a `double`, a boolean a `bool`, a string a
    `const char *`, and other cdata is passed with no conversion.
  - Verbatim: "To pass a Lua object, other than a cdata object, as a specific type, you need to
    override the conversion rules".
  - Example `ffi.C.printf("integer value: %d\n", ffi.new("int", x))`.
  - And: "A vararg C function expecting an integer will see a garbled or uninitialized value."

### Readers of a literal format, in compilers that bind C

- **verified**, GCC, *Warning Options*, https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html, read
  2026-11-10:
  - `-Wformat` checks that "the arguments supplied have types appropriate to the format string
    specified", for the standard functions and "others specified by format attributes".
  - `-Wformat=2`: "Currently equivalent to -Wformat -Wformat-nonliteral -Wformat-security
    -Wformat-y2k."
  - `-Wformat-signedness`: "also warn if the format string requires an unsigned argument and the
    argument is signed and vice versa". As the reader rendered it, this is in neither `-Wformat=2`,
    `-Wall` nor `-Wextra`.
  - `-Wformat-nonliteral`: "also warn if the format string is not a string literal and so cannot be
    checked".
  - `-Wformat-security`: warns "where the format string is not a string literal and there are no
    format arguments."
- **verified**, GCC, *Common Attributes*, https://gcc.gnu.org/onlinedocs/gcc/Common-Attributes.html,
  read 2026-11-10 (reached through the redirect of `Common-Function-Attributes.html`):
  - `format`: "It specifies that the function takes `printf`, `scanf`, `strftime` or `strfmon` style
    arguments".
  - "Valid archetypes include `printf`, `scanf`, `strftime`, `gnu_printf`, `gnu_scanf`,
    `gnu_strftime` or `strfmon`."
  - With the third parameter zero: "In this case the compiler only checks the format string for
    consistency."
  - GCC checks "the standard library functions `printf`, `fprintf`, `sprintf`, `scanf`," so "there is
    no need to modify the header file stdio.h."
  - `sentinel` (offset 100,000 of the same page):
    - "an argument in a call to the function is expected to be an explicit `NULL`".
    - "The attribute is automatically set with a position of 0 for the built-in functions `execl` and
      `execlp`."
    - A valid one is "zero with any object pointer type"; where `NULL` has an integer type, "you need to
      add an explicit cast."
    - "The warnings for missing or incorrect sentinels are enabled with `-Wformat`."
- **verified**, POSIX `exec`, IEEE Std 1003.1-2024,
  https://pubs.opengroup.org/onlinepubs/9799919799/functions/exec.html, read 2026-11-10:
  - "The list is terminated by a null pointer."
  - As the reader rendered it, `(char *)0` appears in the synopsis and examples, and no rationale
    sentence explains the cast.
  - *My reading*: this, with GCC's `sentinel`, is the critic's item 14 class (a pointer past `...`)
    as C itself meets it.
- **verified**, clang, *Diagnostic flags*, https://clang.llvm.org/docs/DiagnosticsReference.html
  (offset 100,000), read 2026-11-10:
  - `-Wformat`: "This diagnostic is enabled by default."
  - `-Wformat-pedantic`, `-Wformat-signedness` and `-Wformat-type-confusion` each list a line of the
    form "format specifies type _A_ but the argument has" / "type" / "underlying type" / "_B_". The
    first two also list "signedness of format specifier ‘_A_’ is incompatible with ‘_B_’".
  - The reader did not mark those three as on by default.
  - Which mismatch falls under which group was not read here. The lane's measurement (`%ld` with
    `int64_t` refused on this Mac) stands as the lane's.
- **verified**, **D reads the literal format of an `extern(C)` declaration marked `pragma(printf)`.**
  - *Pragmas*, https://dlang.org/spec/pragma.html, read 2026-11-10:
    - "pragma(printf) specifies that a function declaration is a printf-like function", for an
      "extern (C) or extern (C++) function with a format parameter."
    - "If the format parameter is followed by ..., the number and types of the variadic arguments are
      checked against the format string."
    - "If the format argument is a string literal, it is verified to be a valid format string per the
      C99 Standard."
    - Among the incompatibilities diagnosed: "incompatible sizes which may cause argument
      misalignment"; and "Most of the errors detected are portability issues."
    - As the reader rendered it, the page does not say what happens with a format that is not a
      literal.
  - **The date**: Martin Nowak, "Release D 2.092.0", 2020-05-12,
    https://lists.puremagic.com/pipermail/digitalmars-d-announce/2020-May/054776.html, read
    2026-11-10. As the reader rendered it, the release brings printf format checks among other
    features.
- **verified**, **Vala, a language that compiles to C, reads a literal format itself and, separately,
  hands GCC the attribute.**
  - **The reader**: Jürg Billeter, Vala r1578, 2008-06-13, "Check printf arguments, fixes bug 483104",
    https://lists.gnome.org/archives/commits-list/2008-June/msg02865.html, read 2026-11-10:
    - The semantic analyzer reports "Argument %d: Cannot convert from".
    - As the reader rendered the diff, it maps `d`, `i`, `c` to `int8` for `hh`, `short` for `h`, `int`
      by default, `long` for `l` and `ssize_t` for `z`; the unsigned conversions likewise; the floating
      ones to `double`; `s` to `string`; `p` to a pointer.
  - **What that reader cost**: GNOME Bugzilla 618404, "Uses 32-bit x86 formatters for printing
    numeric values using printf, etc.", reported 2010-05-11, RESOLVED FIXED,
    https://bugzilla.gnome.org/show_bug.cgi?id=618404, read 2026-11-10:
    - On AMD64, `print("%li", int64_value)` failed with "error: Argument 2: Cannot convert from
      `int64' to `long'".
    - `to_string()` drew GCC's "warning: format ‘%lli’ expects type ‘long long int’, but argument 2
      has type ‘gint64’".
    - As the reader rendered the rest, the reporter asked for the target architecture's formats
      instead of hard-coded 32-bit x86 ones.
    - The fix, committed 2010-08-04, was "glib-2.0: Use GLib FORMAT where possible to fix format
      warnings".
  - **The second route, in 2016**: Rico Tzschichholz, "Add G_GNUC_PRINTF/SCANF attribute for
    Printf/ScanfFormat functions", 2016-11-08,
    https://lists.gnome.org/archives/commits-list/2016-November/msg02610.html, read 2026-11-10:
    - It fixes https://bugzilla.gnome.org/show_bug.cgi?id=710862 (not opened).
    - The C generator writes `" G_GNUC_PRINTF(%d,%d)"` on the generated C declaration, in
      `valaccodefunction.vala` and `valaccodefunctiondeclarator.vala`.

### Q1, read across

| system | past `...` | refused | converted | left to the programmer |
|---|---|---|---|---|
| ISO C | default promotions only | — | integer promotions, `float` to `double` | the type `va_arg` reads (UB if incompatible) |
| Go (cgo) | the call is refused | every variadic call | — | a C wrapper of fixed shape |
| Swift | `...` functions not imported | every `...` function | `CVarArg` encodes for a `va_list` | the `va_list` variant |
| GHC | refused on `ccall` | `ccall` imports of varargs | — | `capi` with a fixed call pattern |
| Java FFM | the specialized shape only | layouts of non-promoted C types, per platform | none ("does not automatically perform") | the descriptor written |
| Rust | allowed, `unsafe` | types the promotions change (`f32` read) | — | every other type |
| Zig | allowed | untyped literals, non-power-of-two widths, arrays, `void` | — | every other type |
| Julia | each argument typed at the call | — | — | the type written |
| Nim | allowed | — | strings to `cstring` | every other type |
| D | allowed, not `@safe` | (a checked format: below) | — | the type |
| ctypes | allowed | — | a Python integer to C `int`, masked | `argtypes` of the fixed part |
| LuaJIT | allowed | — | a number to `double` | an explicit `ffi.new("int", x)` |
| GCC, clang | literal format of a marked function | a warning, an error under `-Werror` | — | every unmarked function, every non-literal format |
| D `pragma(printf)` | literal format of a marked `extern(C)` | the mismatch | — | a non-literal format (not stated) |
| Vala | literal format, its own reader (2008); GCC's attribute (2016) | the mismatch | — | — |

*My reading, the rows read across*:
- No FFI read checks a by-value argument against what the callee's `va_arg` reads, which no caller can
  see.
- **Three systems close the class: Go, Swift and GHC's `ccall`.** They refuse the call and route it
  through a function of fixed shape, which is route (R)'s wrapper and defect 2107's note.
- **The systems that allow the call trust a type the programmer writes**: Java's linker, Rust, Zig,
  Julia, GHC's `capi`, Nim, D, ctypes and LuaJIT.
  - At most they refuse the types C's own promotions would change (Java, Rust) or types C has no name
    for (Zig).
  - Java is the one system read whose refusal is stated as per platform.
- **A format reader exists in four compilers read** (GCC, clang, D, Vala), each keyed to a
  **literal** format of a **marked** function.
- **Vala's own reader hit the per-platform width in 2010.** Its repair was "Use GLib FORMAT where
  possible", which *I read* as GLib's spelling of what C spells in `<inttypes.h>`: a conversion
  written per platform, not one set of letters.

---

## Q2. A library's own include folder, told from the SDK's

### Homebrew, on its own keg-only formulae

- **verified**, Homebrew, *FAQ*, *What does "keg-only" mean?*, https://docs.brew.sh/FAQ, read
  2026-11-10:
  - "It means the formula is installed only into the Cellar and is not linked into the default
    prefix."
  - "This means most tools will not find it."
  - "You can modify a tool's build configuration to find keg-only dependencies."
  - "Or, you can link in the formula if you need to with `brew link <formula>`," "though this can cause
    unexpected behaviour if you are shadowing macOS software."
- **verified**, Homebrew, `Library/Homebrew/keg_only_reason.rb` (branch `main`),
  https://raw.githubusercontent.com/Homebrew/brew/main/Library/Homebrew/keg_only_reason.rb, read
  2026-11-10:
  - `:provided_by_macos`: "macOS already provides this software and installing another version in
    parallel can cause all kinds of trouble".
  - `:shadowed_by_macos`: "macOS provides similar software and installing this software in parallel
    can cause all kinds of trouble".
  - Also `:versioned_formula`.
- **verified**, **the caveat Homebrew prints**, `Library/Homebrew/caveats.rb`,
  https://raw.githubusercontent.com/Homebrew/brew/main/Library/Homebrew/caveats.rb, read 2026-11-10:
  - "is keg-only, which means it was not symlinked into".
  - "If you need to have #{formula.name} first in your PATH, run:".
  - "For compilers to find #{formula.name} you may need to set:", then `LDFLAGS` as
    `-L#{formula.opt_lib}` and `CPPFLAGS` as `-I#{formula.opt_include}`.
  - "For pkgconf to find #{formula.name} you may need to set:", then `PKG_CONFIG_PATH` as
    `#{formula.opt_lib}/pkgconfig`.
  - As the reader read it, "first" appears only for `PATH`, never for the compiler flags.
  - **This answers the brief's question.** Homebrew's caveat tells a build it may need to give the
    keg's folder as `-I`, and does not say *first*. That `-I` is searched ahead of the SDK is the
    compilers' documented order (below), not the caveat's words.
- **verified**, **how Homebrew builds against its own keg-only formulae**,
  `Library/Homebrew/extend/ENV/super.rb`,
  https://raw.githubusercontent.com/Homebrew/brew/main/Library/Homebrew/extend/ENV/super.rb, read
  2026-11-10:
  - `def determine_isystem_paths` is `PATH.new(` `HOMEBREW_PREFIX/"include",`
    `homebrew_extra_isystem_paths,` `).existing`.
  - `def determine_include_paths` is `PATH.new(keg_only_deps.map(&:opt_include)).existing`.
  - These set `self["HOMEBREW_ISYSTEM_PATHS"]` and `self["HOMEBREW_INCLUDE_PATHS"]`.
  - No comment on the page explains the split.
- **verified**, its compiler shim, `Library/Homebrew/shims/super/cc`,
  https://raw.githubusercontent.com/Homebrew/brew/main/Library/Homebrew/shims/super/cc, read
  2026-11-10:
  - `cppflags`: `args += path_flags("-isystem", isystem_paths) + path_flags("-I", include_paths)`.
  - In `keep?(path)`: "# first two paths: reject references to Cellar or opt paths" "# for
    unspecified dependencies", then `elsif path.start_with?(*@keg_dirs)` `dependency_path?(path)`.
  - It also rejects `"/opt/local", "/opt/boxen/homebrew", "/opt/X11", "/sw", "/usr/X11"` ("# ignore
    MacPorts, Boxen's Homebrew, X11, fink").
  - This runs only under `def refurbish_args?` `config.include?("O")`.
- **verified**, **the population the critic measured (item 15), as Homebrew's formula API states each
  formula's status**:

  | formula | API URL, read 2026-11-10 | `versions.stable` | `keg_only` | `keg_only_reason.reason` | explanation |
  |---|---|---|---|---|---|
  | curl | https://formulae.brew.sh/api/formula/curl.json | 8.23.0 | true | `:provided_by_macos` | "" |
  | sqlite | https://formulae.brew.sh/api/formula/sqlite.json | 3.54.1 | true | `:provided_by_macos` | "" |
  | ncurses | https://formulae.brew.sh/api/formula/ncurses.json | 6.6 | true | `:provided_by_macos` | "" |
  | readline | https://formulae.brew.sh/api/formula/readline.json | 8.3.6 | true | `:shadowed_by_macos` | "macOS provides BSD libedit" |
  | icu4c@79 | https://formulae.brew.sh/api/formula/icu4c@79.json | 79.1 | true | `:shadowed_by_macos` | "macOS provides libicucore.dylib (but nothing else)" |
  | icu4c@78 | https://formulae.brew.sh/api/formula/icu4c@78.json | 78.3 | true | `:versioned_formula` | "" |
  | llvm@22 | https://formulae.brew.sh/api/formula/llvm@22.json | 22.1.8 | true | `:versioned_formula` | "" |

  - `curl.json`'s `caveats` field is `null`: the keg-only caveat is generated by `caveats.rb` above,
    not stored with the formula.
  - That each formula's headers collide with this Mac's SDK is the critic's measurement, which the API
    does not state.
  - The reasons show that two of the seven are keg-only for being versioned, not for macOS's copy.

### The compilers' order, re-read today

- **verified**, GCC, *Directory Options*, https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html,
  read 2026-11-10:
  - The order: "For the quote form of the include directive, the directory of the current file is
    searched first."; then "the directories specified by -iquote options are searched in left-to-right
    order"; "Directories specified with -I options are scanned in left-to-right order."; "Directories
    specified with -isystem options are scanned in left-to-right order."; "Standard system directories
    are scanned."; "Directories specified with -idirafter options are scanned in left-to-right order."
  - "You can use -I to override a system header file, substituting your own version" "since these
    directories are searched before the standard system header file directories."
  - "If a standard system include directory, or a directory specified with -isystem, is also specified
    with -I," "the -I option is ignored."
- **verified**, GCC, *The C Preprocessor*, *Search Path*,
  https://gcc.gnu.org/onlinedocs/cpp/Search-Path.html, read 2026-11-10: "The most commonly-used option
  is -Idir, which causes dir to be searched after the current directory" "(for the quote form of the
  directive) and ahead of the standard system directories."
- **verified**, clang, *Command Line Reference*, https://clang.llvm.org/docs/ClangCommandLineReference.html,
  read 2026-11-10:
  - `-I<dir>`: "Add directory to include search path."; "these directories are searched in the order"
    "they are given before the standard system directories are searched."; "If the same directory is
    in the SYSTEM include search paths, for example if also specified" "with -isystem, the -I option
    will be ignored".
  - `-isystem`: "Add directory to SYSTEM include search path".
  - `-iquote`: "Add directory to QUOTE include search path".
  - `-idirafter`: "Add directory to AFTER include search path".

### Build tools: whose is a found library's folder

- **verified**, CMake, `NO_SYSTEM_FROM_IMPORTED`,
  https://cmake.org/cmake/help/latest/prop_tgt/NO_SYSTEM_FROM_IMPORTED.html, read 2026-11-10:
  - "When the consumed target's SYSTEM property is set to true, the contents of the"
    "INTERFACE_INCLUDE_DIRECTORIES target property are treated as system includes".
  - "By default, SYSTEM is true for imported targets and false for other target types."
- **verified**, CMake, `FindPkgConfig`, https://cmake.org/cmake/help/latest/module/FindPkgConfig.html,
  read 2026-11-10:
  - `IMPORTED_TARGET`: "This argument will create an imported target named `PkgConfig::<prefix>`".
  - "Include directories specified with `-isystem` are stored in the `INTERFACE_INCLUDE_DIRECTORIES`
    target property."
  - `PKG_CONFIG_PATH` is an "Environment variable that specifies additional paths in which
    `pkg-config` will search for its `.pc` files."
- **verified**, pkgconf, `pkgconf.1`, https://raw.githubusercontent.com/pkgconf/pkgconf/master/man/pkgconf.1,
  read 2026-11-10:
  - `PKG_CONFIG_SYSTEM_INCLUDE_PATH`: "Colon-separated list of include paths that are filtered out"
    "because they are considered system include paths."
  - `--keep-system-cflags`: "Keep CFLAGS or linker flag fragments that would be filtered due to being
    included by default in the compiler."
  - `PKG_CONFIG_PATH`: "A colon-separated list of high-priority directories where" `.pc` files are
    looked up.

### A toolchain that deprecated a program's header standing in for a system one

- **verified**, Apple, *Build settings reference*, *Always Search User Paths (Deprecated)*,
  https://developer.apple.com/documentation/xcode/build-settings-reference (its `.md` form), read
  2026-11-10:
  - "This setting is deprecated as of Xcode 8.3 and may not be supported in future versions."
  - "If enabled, both `#include <header.h>`-style and `#include "header.h"`-style directives search
    the paths in" "`USER_HEADER_SEARCH_PATHS` before `HEADER_SEARCH_PATHS`."
  - "As a consequence, user headers, such as your own `String.h` header, have precedence over system
    headers" "when using `#include <header.h>`."
  - "This is done using the `-iquote` flag for the paths provided in `USER_HEADER_SEARCH_PATHS`."
    *My reading*: the entry does not say plainly which of its two modes that sentence belongs to.
  - "If disabled and your compiler fully supports separate user paths, user headers are only
    accessible with" "`#include "header.h"`-style preprocessor directives."
  - "For backwards compatibility reasons, this setting is enabled by default. Disabling it is strongly
    recommended."
- **verified**, clang 22.1.0 release notes, https://releases.llvm.org/22.1.0/tools/clang/docs/ReleaseNotes.html,
  read 2026-11-10: "A new warning `-Wshadow-header` has been added to detect when a header file is found
  in multiple search directories". As the reader rendered it, the rest of the sentence reads
  "(excluding system paths)".

*My reading, Q2*:
- **Route (a) is the shape of Homebrew's own build.** Building a formula, Homebrew gives each declared
  keg-only dependency's `include` as `-I` and its own prefix's `include` as `-isystem`. When its shim
  refurbishes the arguments, a path into a keg survives only where `dependency_path?` holds; the
  comment says "for unspecified dependencies" it is rejected.
- So a keg's folder is the library's own **because the build named that library**. That is (a)'s
  "a folder the same group's answer names". It is also the opposite of reading the folder as a stand-in
  for the SDK's header.
- **CMake marks a found library's folder `SYSTEM` by default.** That tells the dependency's folder
  from the project's own; by GCC's order, a `SYSTEM` folder is still searched ahead of the standard
  system directories.
- **Route (d)'s premise, that `pkg-config` ties the header to its `-L`, is not stated on any page read.**
  Homebrew's caveat prints `LDFLAGS` and `CPPFLAGS` as one pair and `PKG_CONFIG_PATH` as another. That
  is a reading, not a page's sentence.
- **No toolchain read refuses a library's header in its declared folder.** Xcode deprecated only the
  mode in which a **user** path answers `<...>`. clang's `-Wshadow-header`, as rendered, excludes system
  paths.

---

## Q3. A quoted include, found from its includer's folder

- **verified**, ISO C, N1570 §6.10.2p2-3 (printed pages 164-165), same PDF as Q1, read 2026-11-10:
  - The `<...>` form "searches a sequence of implementation-defined places for a header identified
    uniquely by the specified sequence between the < and > delimiters". "How the places are specified
    or the header identified is implementation-defined."
  - The `"..."` form: "The named source file is searched for in an implementation-defined manner. If
    this search is not supported, or if the search fails, the directive is reprocessed as if it read #
    include <h-char-sequence> new-line".
  - p6: "A #include preprocessing directive may appear in a source file that has been read because of a
    #include directive in another file, up to an implementation-defined nesting limit".
- **verified**, *Rationale for International Standard, Programming Languages, C*, Revision 5.10,
  §6.10.2 (printed page 99), https://www.open-std.org/jtc1/sc22/wg14/www/C99RationaleV5.10.pdf, read
  2026-11-10 (page images):
  - "The file search rules used for the filename in the #include directive were left as
    implementation-defined."
  - "Nested include files raise an issue of interpreting the file search rules. In UNIX C a #include
    directive found within an included file entails a search for the named file relative to the file
    system directory that holds the outer #include."
  - "Other implementations, including the earlier UNIX C described in K&R, always search relative to the
    same current directory."
  - "The C89 Committee decided in principle in favor of K&R approach, but was unable to provide explicit
    search rules as explained above."
- **verified**, GCC, *Directory Options* (URL above), read 2026-11-10:
  - "For the quote form of the include directive, the directory of the current file is searched
    first."
  - "-I- inhibits the use of the directory of the current file directory as the first search
    directory"; "Please use -iquote instead for -I directories before the -I- and remove the -I-
    option."
  - "Directories specified with -iquote apply only to the quote form of the directive".
- **verified**, GCC, *Search Path* (URL above), read 2026-11-10:
  - "By default, the preprocessor looks for header files included by the quote form of the directive"
    "first relative to the directory of the current file, and then in a preconfigured list of standard
    system directories."
  - "For example, if /usr/include/sys/stat.h contains `#include "types.h"`," "GCC looks for types.h
    first in /usr/include/sys, then in its usual search path."
- **verified**, GCC, *Include Syntax*, https://gcc.gnu.org/onlinedocs/cpp/Include-Syntax.html, read
  2026-11-10:
  - "It searches for a file named file first in the directory containing the current file,".
  - "It is most portable to use only ‘/’."
  - As the reader rendered it, the page does not discuss `..`.
- **verified**, clang, `clang/lib/Lex/HeaderSearch.cpp` (branch `main`),
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang/lib/Lex/HeaderSearch.cpp, read
  2026-11-10:
  - "Check to see if the file is in the #includer's directory. This cannot be" "based on CurDir,
    because each includer could be a #include of a" "subdirectory (#include "foo/bar.h") and a
    subsequent include of "baz.h"" "should resolve to "whatever/foo/baz.h". This search is not done for
    <>".
  - Of MSVC compatibility: "In MSVC compatibility mode we may have already found the file in one of"
    "the includers' directories."
  - The function's doc comment: "Microsoft mode will pass all \#including files."
- **verified**, Microsoft, *#include directive (C/C++)*,
  https://learn.microsoft.com/en-us/cpp/preprocessor/hash-include-directive-c-cpp, read 2026-11-10:
  - The quoted form searches "1) In the same directory as the file that contains the `#include`
    statement. 2) In the directories of the currently opened include files, in the reverse order in
    which they were opened. The search begins in the directory of the parent include file and continues
    upward through the directories of any grandparent include files. 3) Along the path that's specified
    by each `/I` compiler option. 4) Along the paths that are specified by the `INCLUDE` environment
    variable."
  - "The *path-spec* is a file name that may optionally be preceded by a directory specification."
- **verified**, clang, *Command Line Reference* (URL above), read 2026-11-10:
  - `-H, --trace-includes`: "Show header includes and nesting depth".
  - `-fshow-skipped-includes`: "`#include` files may be "skipped" due to include guard optimization
    or `#pragma once`." "This flag makes `-H` show also such includes."
  - That is the instrument the critic's item 16 used. Its Linux behaviour is unrun, as the critic says.

*My reading, the shapes of `00-shared.md` item 9 against these rules (read, not run)*:
- **Shape `d`**: `lib/clock.h` includes `"../sys/timeb.h"`. By GCC's and clang's rule the name is
  looked for in `lib/`, the includer's folder, which resolves to the root's `sys/timeb.h`. No page read
  treats `..` apart from any other relative path; GCC's *Include Syntax*, as rendered, does not discuss
  it. So by every rule read, this file is "found from the includer's folder".
- **Shape `g`**: the root's `clock.h` includes `"sys/timeb.h"`. The includer's folder is the root,
  and the file is found there by the same rule.
- **Shape `e`**: `sys/clock.h` includes `"sys/timeb.h"`.
  - By GCC's and clang's rule the includer's folder is `sys/`, and `sys/sys/timeb.h` is not there. The
    file is found by the build's `-I .`, as the critic says.
  - Only MSVC's rule, walking up to the grandparent's folder, would find it from the includer chain.
  - So the lane's sentence covers `d` and `g` and not `e`. Shape `e` builds today for another reason,
    a folder the build names.
- **The danger shape** (item 16): the SDK's `<sys/socket.h>` reaching the program's `./sys/types.h`
  is an **angle** include, served by a folder given as `-I`.
  - GCC's stat.h example shows a system header's own quoted sibling found first in its own folder, so
    the quoted form from the includer's folder is not the form through which the danger arrives.
  - **A route the brief does not list, put as a question**: GCC's `-iquote`, whose folders "apply only
    to the quote form of the directive", so no `<...>` of the SDK could reach a program folder given
    that way. Xcode's reference names `-iquote` in the entry that deprecates letting user paths answer
    `<...>`, without saying plainly which mode uses it.
  - Whether Heroes passes the program's folders as `-I` or `-iquote` was not read by this seat. It is
    a question for the compiler-engineer, unrun.

---

## Candidate prior-art lines

For the `## Prior art` section of the decision that ratifies this sitting, in the two forms
`.claude/rules/records.md` allows. **Which form applies is the synthesis's call**: *Taken from* where
the resolution adopts a form on that page's evidence, *Reached first by* where a seat or the lane
reached it and the page was found after. A route the sitting does not adopt has no line. Each says
only what its page states, and still owes the second reading the rule asks. Raw files are linked at
their rendered pages; another project's issue goes through `redirect.github.com`.

**If Q1 refuses a by-value argument past `...` and points to a wrapper of fixed shape:**
- **Reached first by Go**: [cmd/cgo](https://pkg.go.dev/cmd/cgo): "Calling variadic C functions is not supported." "It is possible to circumvent this by using a C function wrapper."
- **Reached first by Swift**: [Using Imported C Functions in Swift](https://developer.apple.com/documentation/swift/using-imported-c-functions-in-swift): "C functions that use the `...` syntax for variadic arguments are not imported, and therefore can't be called using `CVarArg` arguments."
- **Reached first by GHC**: [Foreign function interface](https://downloads.haskell.org/ghc/latest/docs/users_guide/exts/ffi.html): functions requiring varargs "are unsupported by the `ccall` calling convention"; imports should "use the `capi` convention, giving an explicit signature for the needed call-pattern."
- **Reached first by ISO C (N1570)**: [§7.16.1.1p2](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf): if `va_arg`'s type "is not compatible with the type of the actual next argument (as promoted according to the default argument promotions), the behavior is undefined", with two exceptions.

**If Q1's reader, refusal or note is per platform (the critic's item 20):**
- **Reached first by Java**: [java.lang.foreign.Linker](https://docs.oracle.com/en/java/javase/22/docs/api/java.base/java/lang/foreign/Linker.html): "Since the size of the C `int` type is platform-specific, exactly which layouts will be rejected is platform-specific as well."
- **Reached first by ISO C (N1570)**: [§7.8.1](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf): for each type of `<stdint.h>`, a macro expands to "a conversion specifier, possibly modified by a length modifier, suitable for use" when converting that type.

**If Q1 keeps a literal-format exception (route (R)):**
- **Reached first by D**: [Pragmas, pragma(printf)](https://dlang.org/spec/pragma.html): "If the format parameter is followed by ..., the number and types of the variadic arguments are checked against the format string."
- **Reached first by Vala**: [GNOME Bugzilla 618404](https://bugzilla.gnome.org/show_bug.cgi?id=618404): Vala's own check refused `print("%li", int64_value)` on AMD64 with "Argument 2: Cannot convert from `int64' to `long'".

**If Q1 takes route (E), the format handed to the C compiler's check:**
- **Reached first by Vala**: [Add G_GNUC_PRINTF/SCANF attribute for Printf/ScanfFormat functions](https://lists.gnome.org/archives/commits-list/2016-November/msg02610.html): the C generator writes `G_GNUC_PRINTF(%d,%d)` on the generated C declaration.

**If Q2 adopts route (a), group-scoped:**
- **Reached first by Homebrew**: [extend/ENV/super.rb](https://github.com/Homebrew/brew/blob/main/Library/Homebrew/extend/ENV/super.rb): `determine_include_paths` is `PATH.new(keg_only_deps.map(&:opt_include)).existing`; and [shims/super/cc](https://github.com/Homebrew/brew/blob/main/Library/Homebrew/shims/super/cc): "reject references to Cellar or opt paths" "for unspecified dependencies".

**If Q3 adopts the lane's widening (found from the includer's own folder):**
- **Reached first by GCC**: [Directory Options](https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html): "For the quote form of the include directive, the directory of the current file is searched first."
- **Reached first by clang**: [HeaderSearch.cpp](https://github.com/llvm/llvm-project/blob/main/clang/lib/Lex/HeaderSearch.cpp): the #includer's directory, which "cannot be based on CurDir"; a subsequent `"baz.h"` "should resolve to "whatever/foo/baz.h"".

**If Q3 confines the program's folders to the quoted form:**
- **Reached first by GCC**: [Directory Options](https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html): "Directories specified with -iquote apply only to the quote form of the directive".
- **Reached first by Xcode**: [Always Search User Paths (Deprecated)](https://developer.apple.com/documentation/xcode/build-settings-reference): enabled, "your own `String.h` header, have precedence over system headers when using `#include <header.h>`"; disabled, "user headers are only accessible with `#include "header.h"`-style preprocessor directives."

---

## argument

The systems read that close Q1's class refuse the call and route it through a fixed shape: Go names "a
C function wrapper", Swift imports no `...` function, and GHC sends varargs to `capi` with "an explicit
signature". The rest trust a type the programmer writes. Four compilers read a literal format. Vala's
reader refused, on amd64 in 2010, a `%li` its reporter called correct, and was repaired to "Use GLib
FORMAT". C's `<inttypes.h>` and Java's linker are both per platform. Homebrew's own build gives a
declared keg-only dependency `-I` and drops a keg path of an unspecified one. GCC, clang and the C99
Rationale search the includer's folder; MSVC alone walks grandparents.

(113 words, counted by hand.)

## condition

This seat's reading would change on:
- **Q1**:
  - an FFI that allows by-value arguments past `...`, checks them against a literal format on every
    platform it supports, and records no per-platform failure of that check, which would weaken
    Vala's lesson;
  - a system read here that once refused variadic calls and gave it up for a measured cost (Go
    supporting them, Swift importing `...` functions), which would weaken the refusal's precedent;
  - E0617's full list of refused types being wider than the promotions, which would move Rust's row.
- **Q2**: a build tool that refused a declared dependency's own folder as standing in for the SDK's
  and kept that refusal; or a Homebrew page telling a build to put a keg's folder after the SDK's.
- **Q3**: a C compiler that resolves a quoted `../` path against something other than the includer's
  folder; or a record of a quoted include found from its includer's folder serving the danger the rule
  exists for, which every page read places in the angle form.

## search

All pages were fetched on 2026-11-10 with the page-reading tool. N1570 and the C99 Rationale were
read as page images (the instrument note above). The queries made with the search tool:
- `Swift "Variadic function is unavailable" printf C import withVaList CVarArg` (forum and mirror
  results not cited; Apple's two pages read instead).
- `zig "passed to variadic function must be casted to a fixed-size number type"` (no Zig result; the
  test file was then fetched by path).
- `ziglang Sema coerceVarArgParam comptime_int variadic "must be casted" OR "cannot pass" c_int
  c_double` (no primary page; `src/Sema.zig` was not read).
- `GHC user guide CApiFFI "capi" calling convention variadic functions foreign import header
  prototype` (led to the guide and the commit, both read).
- `Vala compiler PrintfFormat attribute checks format string arguments valac "too few arguments" OR
  "Argument" printf vapi`.
- `vala "check_print_format" OR "printf_format" methodcall.vala semantic check format arguments type
  mismatch` (led to r1578 and bug 618404, both read; the current `valaanalyzer.vala` was not read, so
  whether the reader still exists today is **unverified**).
- `dlang changelog "pragma(printf)" added version 2.092 printf format checking extern(C)` (led to the
  announcement, read).
- `Xcode build setting "Always Search User Paths" deprecated "String.h" "precedence over system
  headers" USER_HEADER_SEARCH_PATHS` (mirror results not cited; Apple's reference read instead).

The sweep, by question, with what could not be read:
- **Q1, languages and documents swept**:
  - ISO C (N1570 §6.5.2.2, §6.10.2, §7.8, §7.16.1.1, §7.21.6.1).
  - Apple's arm64 ABI page; Microsoft's data type ranges and x64 calling convention.
  - Rust: E0617, the Reference's external blocks, `core::ffi::c_long`.
  - Zig: the language reference at offsets 0 and 400,000, and the compile-error test.
  - Go's cmd/cgo; Swift's imported-C-functions and `CVarArg` pages.
  - GHC's FFI guide and Ben Gamari's commit; Java SE 22's `Linker`.
  - Julia's calling-C manual; Nim's manual, rendered and raw; D's functions and pragma specs and its
    2.092.0 announcement.
  - Python's ctypes (first 100,000 characters); LuaJIT's FFI semantics.
  - GCC's warning options and common attributes (offsets 0 and 100,000, reached through the redirect
    of `Common-Function-Attributes.html`); clang's diagnostics reference (offset 100,000); POSIX
    `exec`; Vala r1578, bug 618404 and the 2016 commit.
  - **Not read**: Codeberg's copy of the Zig test (404), Zig's `Sema.zig`, Vala bug 710862, E0617's
    complete list.
  - **Not swept**: OCaml's ctypes, .NET P/Invoke, Odin, Common Lisp CFFI, Kotlin/Native and
    Chicken Scheme. A sentence above about "the systems read" says nothing about those.
- **Q2, swept**:
  - Homebrew's FAQ, `keg_only_reason.rb`, `caveats.rb`, `extend/ENV/super.rb` and `shims/super/cc`
    (each 404 on branch `master`, read on `main`), and the formula API for the seven formulae.
  - GCC's directory options and *Search Path*; clang's command line reference (first 100,000
    characters).
  - CMake's `NO_SYSTEM_FROM_IMPORTED` and `FindPkgConfig`; pkgconf's man page; Apple's build settings
    reference (first 100,000 characters); clang 22.1.0's release notes.
  - **Not read**: CMake merge request 8143 (403), the freedesktop pkg-config guide (not attempted
    after panel 237's 403), Meson (read by panel 237's historian on 2026-11-08, not re-read today, so
    not cited).
- **Q3, swept**:
  - N1570 §6.10.2; the C99 Rationale §6.10.2; GCC's directory options, *Search Path* and *Include
    Syntax*; clang's `HeaderSearch.cpp` (first 100,000 of its 104,440 characters) and command line
    reference; Microsoft's `#include` page.
  - **Looked for and not found on the pages read**: a rule that treats `..` in a quoted include apart
    from any other relative path (GCC's *Include Syntax*, as rendered, does not discuss `..`; the
    clang comment and the MSVC page describe a relative name looked up in the includer's folder).
    This names those pages and says nothing of others.
- *Heroes of code* was not opened for this report. No command ran on the Heroes tree in this seat; no
  `docs/panel/` grep beyond reading panels 237's and 242's historian reports, so a sitting before this
  one that already ruled on these precedents is not excluded by this report.
