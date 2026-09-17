# Panel 161 — shared brief: C's third `char` has no portable spelling in Heroes

**Every number in this file was produced by a command run on 2026-09-17 while
this brief was being written, and the command is named beside it** (CL-077).
Nothing here is copied from a document, from a milestone file or from an earlier
sitting.

## The question

`spec § 13` says a field and a parameter are declared at **the header's own
width and sign**. C has three distinct `char` types, and the sign of the plain
one is chosen by the platform's ABI. Heroes has eight integer types and every
one of them carries a fixed sign.

**So: what does a Heroes program write to bind a header's plain `char`, such
that ONE source file compiles on all four legs?**

## The fact, measured

Probe: a three-line C program printing `CHAR_MIN`, `CHAR_MAX` and the sign of
`(char)-1`, compiled and run on each machine.

| machine | plain `char` | |
|---|---|---|
| Linux arm64 | **UNSIGNED**, `CHAR_MIN 0`, `CHAR_MAX 255` | |
| Linux x86-64 | SIGNED, `-128` … `127` | |
| Darwin arm64, the author's Mac | SIGNED, `-128` … `127` | |
| Windows x86-64 | **UNRUN** — the box is off and only the author starts it | |

The two Linux machines are **one axis apart and that was verified rather than
assumed**: `docker run … bash -c 'clang --version; ldd --version; …'` on both
images gives Debian 13.6, Debian clang 22.1.8, lld and lldb 22.1.8, glibc 2.41,
pkg-config 1.8.1, git 2.47.3, sqlite 3.46.1, libcurl 8.14.1 on each. Only
`uname -m` differs.

The generic AAPCS declares plain `char` unsigned and Debian arm64 follows it;
Apple's arm64 ABI deviates and declares it signed. **The divergence is the
platform's ABI, not the architecture** — which is why three legs could not show
it and why the milestone's own registered prediction ("unsigned on the ARM ABI")
is scored FALSE on its stated cause while true on its consequence.

## What it costs today

Probe programs written against a local header declaring `char scalar;` and
`char arr[4];`, built with `heroes build` inside each container:

| shape | Linux arm64 | Linux x86-64 |
|---|---|---|
| `scalar: i8` | refused `ffi_field_type` | accepted |
| `scalar: u8` | accepted | refused `ffi_field_type` |
| `arr: i8[4]` | refused `ffi_field_type` | accepted |
| `arr: u8[4]` | accepted | refused `ffi_field_type` |
| `function probe_take(c: i8)` | refused `ffi_parameter_type` | accepted |
| `function probe_take(c: u8)` | accepted | refused `ffi_parameter_type` |
| `function probe_give() -> i8` | refused `ffi_return_type` | accepted |
| `function probe_give() -> u8` | accepted | refused `ffi_return_type` |

Every shape inverts. `signed char` and `unsigned char` bind portably at `i8` and
`u8` and are unaffected; the hole is C's **third** `char` type alone.

**The full net on arm64**, `./heroes run tests/harness/main.hero -- ./heroes`:
**1825 passed, 3 failed**. All three failures are one program,
`tests/golden/run/ffi-a-char-array-member.hero`, in `run`, `determinism` and
`emission`. `corpus` reads **53 passed, 0 failed**, so none of the corpus's 20
`extern` programs of 55 binds a plain-`char` member. The compiler's own tests
read **654, all passed** there, the same 654 measured on the Mac this session;
the net's own tests read **157, all passed**.

## The scale, measured — and it is arrays, not scalars

A clang JSON AST walk over nine headers this project binds or could bind
(`time.h`, `stdio.h`, `dirent.h`, `sys/stat.h`, `sys/utsname.h`, `pwd.h`,
`termios.h`, `sqlite3.h`, `curl/curl.h`), counting `FieldDecl` nodes whose
`qualType` is exactly `char` or matches `char[N]`:

| | Debian arm64 | Darwin arm64 (8 headers, no curl) |
|---|---|---|
| total `FieldDecl` | 489 | 330 |
| plain-`char` **scalar** fields | **0** | **0** |
| plain-`char` **array** fields | **23** | **16** |
| plain-`char` parameters | 1 (`sqlite3_str_appendchar`) | — |
| plain-`char` results | 0 | — |

**The first version of this walk returned 0 arrays and the number was wrong**:
it tested `qualType.startswith('char [')` and clang writes `char[14]` with no
space. It is named here because a brief's numbers are the one thing a sitting
does not otherwise check, and this one was caught by re-reading the instrument
rather than by any seat.

Named fields on Debian arm64 include `struct utsname`'s **six**
(`sysname`, `nodename`, `release`, `version`, `machine`, `__domainname`, each
`char[65]`), `struct dirent`'s `d_name[256]`, `struct sockaddr`'s `sa_data[14]`,
`curl_hstsentry`'s `expire[18]`, and three `_IO_FILE` members. On Darwin:
`struct utsname`'s five at `char[256]`, `dirent`'s `d_name[1024]`, and ten
`_opaque_pthread_*` members.

**`struct utsname` is the headline and it is the same struct twice.**
`tests/golden/run/ffi-a-char-array-member.hero`'s own comment records that
`uname()` was unreachable at any price until M-complete-structs repaired it in
August. That repair works on three legs and **fails on the fourth**.

## One thing the seats should weigh, because it was not obvious

**`struct utsname`'s arrays are `char[65]` on Debian and `char[256]` on
Darwin** — measured, both walks above. So that struct is **already**
non-portable in Heroes for a reason that has nothing to do with sign: the
declared length differs. Any resolution that makes the sign portable still
leaves this struct needing a per-platform source file, and a resolution argued
as *"it makes headers portable"* should say which headers it actually makes
portable and which it does not.

## Where the mechanism is

- `selfhost/emit/extern_field.hero` (363 lines). Line **154** builds one
  `_Generic` row per C integer type: `"<ctype> (*)[N]: (sizeof(<ctype>) ==
  sizeof(<elem>) && (((<ctype>)-1 < 0) == ((<elem>)-1 < 0)))"`. On arm64 the
  `char` row's sign conjunct is `false == true`, so the field lands on
  `default: 0` and the `_Static_assert` fails. Line **261**, `unsignedness_of`,
  asks the compiling clang directly: `char:((char)-1 > 0)`.
- `selfhost/emit/assert_spelling.hero` (279 lines) holds `C_INTEGER_TYPES`, the
  twelve rows: `_Bool`, `char`, `signed char`, `short`, `int`, `long`,
  `long long`, and the five `unsigned` ones.
- `selfhost/emit/c_spellings.hero` (188 lines), line **59**:
  `if c_type == "signed char" || c_type == "char"` answers `i8` with
  `no_caveat`, on every target. This is defect **059**.
- `selfhost/widths.hero` (213 lines) holds `IntKind`, `int_signed`,
  `int_c_type`.

**The checker is not wrong.** Both mechanisms answer correctly on both
machines. What has no answer is the author's question.

## The routes named so far

Naming four is not a claim that four is the set (CL-057), and a seat that finds
a fifth has found the most valuable thing in this sitting.

1. **A ninth integer type** in the language proper whose sign is the target's.
2. **A spelling legal only inside an `extern` group** — a contextual word, as
   `tag`, `partial`, `owned`, `link`, `package` and `as` already are — mapping
   to the target's plain `char`, leaving the eight integers untouched. Open
   sub-question: what a Heroes program READS such a field as, since the sign
   question returns at the read.
3. **Accept either `i8` or `u8` against a plain `char` everywhere.** Cheapest,
   and it permits a silent sign confusion: 200 read back as −56. CLAUDE.md
   § Precedence rank 3 (robustness) is what this has to answer to.
4. **Refuse plain `char` and say so**, making those fields unbindable. Held to
   design.md Part 6's standard: a refusal must name the program or compiler fact
   that would make it wrong.

## Constraints every seat is held to

- **CLAUDE.md § Precedence**: robustness beats elegance, token cost, ergonomics,
  compiler size and speed. Take the most robust and production-ready
  resolution — never the cheapest, never the compromise.
- **Principle 0**: a form enters v1 if the compiler needs it (the closure list)
  or it provably serves the thesis.
- **The spec budget, measured this session** with
  `./heroes measure spec/heroes-spec.md`: **5997** vendored (`cl100k_base`),
  **7984** real (`claude-opus-5`, the binding number), ceiling **10240**,
  headroom **2256**, of which the FFI floor mortgages 60 (panel 030 R3).
- **A vendored delta is not a price** (author instruction 2026-09-16). Any
  spec text is priced on the real instrument or written down as a lower bound
  in those words.
- **Build in a copy.** `cp -r` the tree to your scratchpad, `rm -rf build`, and
  work there. The seed builds in a few seconds:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. Rebuilding from
  `selfhost/` is ~20 minutes and will kill you on the watchdog. **Never
  `archive/bootstrap-rs/`** — nothing builds it.
- The working tree is frozen for the duration of this sitting.

## What the sitting must deliver

A resolution the coordinator can implement **inside M-arm-platform**, which
closes with defects 058 and 059 closed so the fourth CI leg lands green. Each
seat gives: verdict · the section it rests on · cost/delta measured · a
falsifiable prediction with the milestone at which it becomes checkable · any
condition or veto.
