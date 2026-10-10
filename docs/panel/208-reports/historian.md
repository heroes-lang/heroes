# Panel 208, historian (advisory, no veto)

Written as I went. My only instruments were web search and fetch, plus `Read`
on one local SDK header. I have no shell, so I could not take a `date`
reading and give no clock times. Every precedent below carries a URL fetched
in this session. Anything a fetched page did not confirm is marked
`unverified`. A small summarising model answered each fetch, so a quotation
below is that model's rendering of the page, not a byte copy of it.

Route into the record: the brief's own list (Zig, Go, Rust, Swift, Nim, Odin,
C3, Hare, V, and C under `-Werror`). Search found nothing more, so the list is
still a lower bound. *Heroes of code* was not used as evidence.

## Precedents

### Zig: refusing deprecated code by default, behind a flag. Merged, then reverted a day later, and never released

- **Proposal**: the `@deprecated()` builtin, issue #22822, opened by kristoff-it
  on 2025-02-09. andrewrk accepted it with changes on 2025-02-11: the flags are
  `-fallow-deprecated` / `-fno-allow-deprecated`; *"Modules created by the root
  package default to `-fno-allow-deprecated`"*, while dependency packages default
  to allow; *"Build system does not expose an option to set this flag."* Users
  objected. nektro said it would fit better as a library. jeffective called
  `-fno-deprecated` *"viral"*. castholm wrote *"how can we let users know that
  they are using a deprecated API?"*, saying discoverability is the real
  problem. mlugg raised re-exports and `@deprecated()` inside function bodies.
  **verified**, https://github.com/ziglang/zig/issues/22822
- **Implementation**: PR #22898, merged 2025-02-27 as `dea72d1` with milestone
  0.14.0. **Reverted 2025-02-28** in `6b6c1b1`, whose message reads *"The
  changeset does not work as advertised and does not have sufficient test
  coverage."* **verified**, https://github.com/ziglang/zig/pull/22898
- **Not in 0.14.0**: neither half of the release notes mentions `@deprecated`
  or `-fallow-deprecated` (both halves read). **verified**,
  https://ziglang.org/download/0.14.0/release-notes.html
- **Still unsettled, open since 2025-02-09**: it moved to Codeberg as #36703,
  opened 2026-08-30, open, with no milestone. One commenter says *"folks werent
  happy"* that the first implementation raised an error when a deprecated
  symbol was used only inside the module that defined it. Making the builtin
  *"dependency aware would be a lot more complicated so it was tabled."*
  **verified**, https://codeberg.org/ziglang/zig/issues/36703
- **What the Zig standard library does instead**: an API is turned into
  `@compileError("deprecated; ...")` at a release boundary. I saw this only
  in search-result summaries and fetched no page for it: **unverified**.
- **`@cImport` and a C header's `deprecated` attribute**: I found no page that
  says whether translate-c carries the attribute over (search: *Zig translate-c
  cImport __attribute__((deprecated)) ignored warning*). This is a question,
  **unverified**.

What it means here: the brief cites `-fallow-deprecated` as "the shape" for
(F). By the pages above, that shape never shipped in a Zig release, and the
part users objected to was the one (R) would copy: refusing by default in the
root package.

### Go: the compiler has no warnings, and C's warnings pass through cgo raw

- **The no-warnings rule**, from the Go FAQ entry *Can I stop these complaints
  about my unused variable/import?*: *"compiler options should not affect the
  semantics of the language and because the Go compiler does not report
  warnings, only errors that prevent compilation."* It gives two reasons. The
  first is *"if it's worth complaining about, it's worth fixing in the code.
  (Conversely, if it's not worth fixing, it's not worth mentioning.)"* The
  second is that warnings make compilation noisy, *"masking real errors"*. The
  way out of the refusal is written in the source: `var _ = unused.Item`, the
  blank identifier. **verified**, https://go.dev/doc/faq
- **A deprecation is a comment, not a diagnostic**: it is a doc-comment
  paragraph that begins `Deprecated:`. The wiki says *"Some tools will warn on
  use of deprecated identifiers"*, linking staticcheck's SA1019, and that under
  Go 1 compatibility a deprecated feature is not removed. **verified**,
  https://go.dev/wiki/Deprecated
- **staticcheck SA1019**, *"Using a deprecated function, variable, constant or
  field"*: available since 2017.1 and on by default. **verified**,
  https://staticcheck.dev/docs/checks/#SA1019
- **The gopls analyzer `deprecated`**, which *"looks for deprecated symbols
  and package imports"*: on by default. Its package sits under
  `gopls/internal/analysis`, so it is an editor's check and not `go vet`'s
  (that last step is my reading of the path). The entry itself is
  **verified**, https://go.dev/gopls/analyzers
- **cgo lets C's warnings through.** In golang/go#14696 (Rob Pike, 2016-03-07,
  *os/user: warning building on mac*), Brad Fitzpatrick wrote *"C warnings are
  basically the only warnings that exist and are allowed to spam in a Go
  build."* **verified**, https://github.com/golang/go/issues/14696
- **A user's build printed exactly this sitting's kind of text through cgo**:
  `'IOMasterPort' is deprecated: first deprecated in macOS 12.0
  [-Wdeprecated-declarations]`, building gopsutil, 2022-01-18. The issue
  itself was about a linker-flag mistake. **verified**,
  https://github.com/golang/go/issues/50662
- **Go holds its own C to `-Werror` and leaves a user's C alone**: #14698,
  *should C warnings cause make.bash to fail?*, opened 2016-03-07, closed
  2016-08-30 with milestone Go1.8Early; and #63903, *runtime/cgo: should not set
  -Werror*, filed from Gentoo 2023-11-02 and still open. In #63903 cherrymui
  wrote *"The `-Werror` in the runtime/cgo package only affects the C code in
  the runtime/cgo package."* **verified**,
  https://github.com/golang/go/issues/14698,
  https://github.com/golang/go/issues/63903
- **Go's builders turn C warnings into errors on their own runs**: *"C compiler
  warning promoted to error on Go builders"*, crypto/x509, 2018-12-06.
  **verified**, https://go.googlesource.com/go/+/8390781ca35ac5874eb5b136cfc29bb47adee94b
- **Go's own tree silenced an Apple deprecation rather than migrate, because an
  older macOS was still supported**: x/exp `56b785ea`, 2019-04-08,
  *shiny/driver/gldriver: fix 2 warnings on macOS 10.14*, used a targeted
  `#pragma clang diagnostic ignored` because Go still supported older macOS
  versions. **verified**,
  https://go.googlesource.com/exp/+/56b785ea58b286fc6c055da6155916719832a3a2

What it means here: Go is the closest precedent to Heroes' position, a language
with no warnings sitting on a C boundary. Go stands where Heroes stands after
defect 571: strict on the language's own lines, while the boundary's C warnings
print raw at exit 0. Go has stood there for ten years, with its own C under
`-Werror` and the user's not. Go neither refuses deprecated names nor silences
them. It leaves the advice to a separate tool that is on by default.

### Rust: deprecation is a warn-level lint, and `deny(warnings)` is the documented trap

- **`deprecated` is warn by default**: *"rustc will issue warnings on use of
  `#[deprecated]` items"*, and the attribute applies to an *"external block
  item"* too, so an `extern "C"` declaration can carry it. `allow` / `warn` /
  `deny` / `forbid` / `expect` are entity-level attributes. **verified**,
  https://doc.rust-lang.org/reference/attributes/diagnostics.html and
  https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html
- **`#![deny(warnings)]` is listed as an anti-pattern**: *"By disallowing the
  compiler to build with warnings, a crate author opts out of Rust's famed
  stability"*, and *"sometimes APIs get deprecated, so their use will emit a
  warning where before there was none."* The page recommends
  `RUSTFLAGS="-D warnings"` outside the code, or naming the lints, and says it
  *"explicitly did not add the `deprecated` lint"*. **verified**,
  https://rust-unofficial.github.io/patterns/anti_patterns/deny-warnings.html
- **RFC 1193, `--cap-lints`** (2015-07-07): it exists because a change to a
  lint broke crates that denied it. Cargo passes `--cap-lints allow` to every
  dependency, and the RFC notes that a new lint warning *"will never represent a
  memory safety issue"*. So a flag that changes the verdict did ship, and it
  is aimed at code the user did not write. **verified**,
  https://rust-lang.github.io/rfcs/1193-cap-lints.html and
  https://doc.rust-lang.org/rustc/lints/levels.html
- **bindgen drops C's `deprecated` attribute, which is (S) in practice**:
  rust-bindgen #2675 (opened 2023-11-01, still open, no maintainer reply) shows
  `foo` deprecated in the header and the generated `extern "C"` block carrying
  no marker. On 2025-04-18 nwellnhof, a libxml2 maintainer, wrote *"We have many
  deprecated functions which new Rust users are suddenly starting to call."*
  **verified**, https://github.com/rust-lang/rust-bindgen/issues/2675

What it means here: this is the one measured cost of silence in the record. A
library's maintainer watched its deprecations stop reaching the users of a
newer language.

### Swift: `deprecated` warns, `unavailable` refuses, and Apple's own header chooses per language

- **SE-0443, *Precise Control Flags over Compiler Warnings***, implemented in
  Swift 6.1. Its motivation: `-warnings-as-errors` blocks users from upgrading
  the compiler or SDK until every new warning is resolved, and *"deprecation
  warnings for certain APIs"* are the clearest case. Its example group is
  `DeprecatedDeclaration`. It adds the flags `-Werror <group>` and
  `-Wwarning <group>`. **verified**,
  https://github.com/swiftlang/swift-evolution/blob/main/proposals/0443-warning-control-flags.md
- **The levels**: *"deprecated ... the first version when using the API
  generates a compiler warning"*; *"obsoleted ... generates a compiler error"*;
  `unavailable` *"generate[s] a compiler error when used"*. NSHipster, Mattt,
  2019-12-10. A secondary source; the TSPL page I fetched names no level.
  **verified** against that page, https://nshipster.com/available/
- **This Mac's SDK, read locally**:
  `/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/_stdio.h:275-280`.
  `sprintf` carries `__swift_unavailable("Use snprintf instead.")`
  unconditionally, and `__deprecated_msg(...)` only under
  `#if !defined(_POSIX_C_SOURCE)` (line 277). XNU's `cdefs.h` defines
  `__swift_unavailable(_msg)` as
  `__attribute__((__availability__(swift, unavailable, message=_msg)))`, with
  the comment *"unavailable in Swift, regardless of any other availability in
  C."* **verified** (local read; the definition at
  https://github.com/apple-oss-distributions/xnu/blob/main/bsd/sys/cdefs.h).
  So Apple's header refuses `sprintf` to the newer language and only warns C,
  and it does so with an explicit attribute, not by inferring a refusal from
  `deprecated`.
- **Whether ClangImporter turns a plain C `deprecated` into a Swift warning**:
  I found no page saying so. **unverified**.

**A lead for the brief's unexplained question** (why a call of `sprintf` stays
quiet). The deprecation at `_stdio.h:278` exists only when `_POSIX_C_SOURCE` is
not defined. If the runtime header or the compile flags define it, the
attribute is never declared in that translation unit and clang has nothing to
say. **I did not run this**: the brief forbids reading the trunk's checkout,
and I have no tree of my own. It is the compiler-engineer's question to run.
Whether or not it explains the silence, it shows that a C header's deprecation
is not a fixed property of a name on one platform: it can depend on a
feature-test macro.

### C under `-Werror`: what projects did when Apple's macOS 13 SDK deprecated `sprintf`

- **protobuf (Flutter/Dart)**, 2023-05-17, Eric Seidel: *"Mac has deprecated
  sprintf, combined with -Werror this makes the protoc build fail."* They
  suppressed the warning in BUILD.gn. **verified**,
  https://flutter.googlesource.com/third_party/protobuf-gn/+/ca669f79945418f6229e4fef89b666b2a88cbb10
- **Ceres Solver**, 2023-09-11: *"Replace sprintf by snprintf to avoid this
  deprecation warning"*, citing `SDKs/MacOSX13.1.sdk/usr/include/stdio.h`.
  They migrated to the replacement. **verified**,
  https://ceres-solver.googlesource.com/ceres-solver/+/863db948f381f74aea58a08a4b559facaf165ef0%5E%21
- **VLC (turned `-Werror` off for Breakpad), MacPorts #66091 (assimp,
  `-Werror,-Wdeprecated-declarations`), Emacs (`-Wno-deprecated-declarations` on
  darwin)**: these appeared in search summaries only, and the VLC and Emacs
  fetches failed (Anubis, 502). **unverified**.
- **Android forcing deprecations to stay warnings under `-Werror`**: I saw
  this in a search summary only, and the fetch failed. **unverified**.

What it means here: the verified record shows a header deprecation landing on
one platform's SDK and breaking strict builds that had compiled everywhere.
Projects answered with either silence or migration, and none kept the
refusal.

### Nim, Odin, C3, Hare, V

- **Nim silences every C compiler warning by default**: `config/nim.cfg` has
  `clang.options.always = "-w -ferror-limit=3 -fno-strict-aliasing"`, and `-w`
  appears in `gcc.options.always` on each branch. So a deprecated `importc`
  function's C warning never reaches a Nim user. **verified**,
  https://raw.githubusercontent.com/nim-lang/Nim/devel/config/nim.cfg. Nim's own
  `Deprecated` warning, which `--warningAsError:X:on|off` can turn into an
  error, is **verified**, https://nim-lang.org/docs/nimc.html. I found no
  complaint about the C-side silence (search: *Nim issue C compiler warnings
  hidden -w clang.options.always importc deprecated*); that is a question, not
  a finding.
- **Odin**: Barinzaya on the forum, 2025-05-13: *"the Odin compiler follows a
  similar to philosophy to that of Go: no warnings, only errors."* This is a
  user's post, not a maintainer's. **verified** as said there,
  https://forum.odin-lang.org/t/vet-flags-as-warnings-instead-of-errors/971.
  How Odin treats `@(deprecated)` on use: the overview's first 100,000
  characters do not say, and I did not read the rest. **unverified**.
- **C3**: `@deprecated` makes *"use trigger a warning"*, and `@allow_deprecated`
  *"suppresses detection of `@deprecated` for the function's parameters and
  body"*, a per-function acknowledgement written in the source. **verified**,
  https://c3-lang.org/language-common/attributes/
- **Hare**: its blog post on `hare-update` (Drew DeVault, 2025-06-11) describes
  breaking changes shipped with a migration tool, not a warning period.
  **verified**, https://harelang.org/blog/2025-06-11-hare-update/. Whether Hare
  has any deprecation attribute: none found. **unverified**.
- **V uses a schedule**: *"Before that date, calls to the function will be
  compiler notices"*; *"After that date, calls will become warnings, so
  ordinary compiling will still work, but compiling with -prod will not"*;
  *"6 months after the deprecation date, calls will be hard compiler errors."*
  This came from PR #10682 by spytheman, merged 2021-08-04. **verified**,
  https://docs.vlang.io/attributes.html, https://github.com/vlang/v/pull/10682

## The pattern across the set

Across the ten languages above, I found no compiler whose shipped default
refuses a use of a name at the moment it is deprecated. Zig tried it and
reverted within a day. V refuses only 180 days after a date the library's
author chose. Swift and Apple refuse only where the header says `unavailable`.
Everywhere else, a deprecated name is a correct program carrying advice. Where
the advice was silenced (Nim wholesale, bindgen by omission), the one measured
cost is bindgen's: new users calling deprecated functions. Where it was made
fatal (`-Werror`, `deny(warnings)`), the cost is builds broken by an SDK
upgrade, and the projects answered by switching it off.

## Routes, as the record reads them (advisory)

- **(R) refuse: object.** Zig reverted it. Rust documents it as an
  anti-pattern. SE-0443 was written because of it. The macOS 13 `-Werror`
  breakage shows what it costs a program that builds everywhere else. The
  verdict would also turn on one SDK and possibly on `_POSIX_C_SOURCE`. The
  exception the record supports is narrow: a header's own `unavailable` or
  `__attribute__((error))` is already a refusal in C, and telling it on the
  `.hero` line is the c-boundary class's business, not this route's.
- **(S) silence: approve, as the build verdict.** Nim has done it wholesale for
  as long as `nim.cfg` has said `-w`, which I did not date (**unverified**).
  The cost is real (bindgen #2675), so (S) alone loses the advice.
- **(M) refuse with a way out: object** to the refusal half. The way-out half
  has precedent (Rust `allow`, C3 `@allow_deprecated`, Go's `var _ =`), but in
  every one of those languages except Go the default it escapes is a warning.
  Go's `var _ =` escapes a refusal of unused imports, not of deprecation.
- **(F) a flag: object.** The Go FAQ says *"compiler options should not affect
  the semantics of the language"*, and Zig's flag never shipped. Rust's
  `--cap-lints` did ship, but it is aimed at dependencies inside a language
  that has warnings.
- **(N) a note at exit 0: object.** It is Go's cgo pass-through, which is
  Heroes' state today, and it is a warning level by another name, against
  design.md `:3744`.
- **A route nobody listed (Go's split): the build stays silent, and the advice
  goes to a separate question the programmer asks.** In Go that question is
  staticcheck SA1019 since 2017.1 and gopls's `deprecated` analyzer, both on by
  default. In Heroes it would have to be a `heroes` verb or flag, since
  CLAUDE.md § 10 allows no second binary. Whether
  `.claude/rules/cli-surface.md`'s stopping rule admits it is a question for
  the panel, not for me.

## Prediction (scorable)

If (R) lands without (M), a defect or decision in `issues/` asking for a way to
call a deprecated C function on purpose will be filed before the second
milestone tag after the landing. The reason would be a replacement that is
missing on another platform or SDK, which is the case of Go's x/exp
`56b785ea`. Score it by grepping `issues/` for the new class's name between the
landing commit and that tag.
