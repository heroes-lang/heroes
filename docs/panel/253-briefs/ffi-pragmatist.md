# Panel 253, ffi-pragmatist

Read `00-shared.md` first, whole: the frozen commit (`181555e9a32d0b18878f9f10e48d2eadd939c285`), the six
questions, the measurements and the rules. You judge the founding constraint (design.md §1.11 and §4.19:
`grep -n -E '^### 1\.11|^### 4\.19' docs/design.md`), and **question 1 rewrites it**: your own definition
quotes the sentence the author's decision 9 replaces (`.claude/agents/ffi-pragmatist.md:3`, *"there is no
standard library; everything comes from C"*). Veto on anything that breaks the C ABI or makes bindings
categorically harder. **Compile, don't opine.**

**Your folder** is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/253-ffi-pragmatist/`,
made by you as `00-shared.md` § Where each seat works shows; binaries and containers carry `fp`. **Your
report** is `.claude/worktrees/rebel-round/docs/panel/253-reports/ffi-pragmatist.md`, written as you go, in
`.claude/agents/ffi-pragmatist.md`'s structure, **one block per question**: 1, 3, 5. `swiftc` and every
tool with `HOME` and `TMPDIR` inside your folder. One build or run of yours at a time; the Windows box is
not yours, and a Windows fact you need is named **owed**. **The sitting is whole, six questions** (the
coordinator's decision (a), `00-shared.md` § One sitting, six questions): write each question's block
whole before the next; **question 3's first measurement is the constant-time function already in the tree**
(below), which may decide route (H) before any primitive is written; and **you take the Linux arm64 rows
of question 5's frame walk**, which the compiler-engineer builds on this Mac only.

**No wall clock** (the coordinator's decision (b)): five agents share this Mac, and CLAUDE.md
§ Verification says *"while a clock runs the machine stays still"*. Report costs in **instructions
retired** (`/usr/bin/time -l`, best of N stated, the first discarded) and **bytes**; **a timing-based
constant-time test (dudect's kind) is a clock too**, so it is named **owed to a still machine**, and your
constant-time evidence now is what a reading of the emitted C and machine code and a taint run show.

## Question 1: your charter, rewritten

The two resolutions are `bw/arms/design-r.diff` (+333 vendored on design.md) and `design-c.diff` (+88),
`00-shared.md` § Question 1. **Judge them as the seat whose mandate they move**: does *"comes from a
package it carries or from a C library through the FFI"* demote the FFI from *"FFI ergonomics rank
alongside comprehension, not below it"* (design.md §1.11, `grep -n 'FFI ergonomics rank' docs/design.md`),
and should that sentence stand unchanged under either arm? **Decision 24** (*"What reads data from
outside is written in Heroes, never bound from a C library"*, `M-rebel-library.md`, decisions of
2026-11-16) is the strongest departure from the old box: name what it costs a binding today (Rebel's
`sql/sqlite` and the other packages over C: count them in Rebel's faces by their `extern` groups, `grep
-rln '^extern ' packages/rebel/0.1.0/files`, and say which a program would otherwise have bound itself),
and, **as a question** (the critic's finding 17), where every system call Rebel makes stands:
`selfhost/cli/layer.hero:1-23` says the layer's members are the capabilities *"whose C differs by
platform"* and that *"`hero_os.h`'s process, files, directories, spawning and threads, stays there"*, in
the runtime (`:5-6`, `:21-23`), and Rebel's 23 files with an `extern` group (`grep -rln '^extern '`) name
the runtime's and the layer's headers and four of the system's, `math.h`, `sqlite3.h`, `string.h` and
`stdlib.h` (`grep -rh '^extern '`, counted by the writer). design.md already names *"Swift `@_cdecl`"* among
the producers of the C ABI Heroes targets (`docs/design.md:499`), the record question 3's route (S) rests
on. Say in one paragraph how your own definition's first sentence should read under the robust arm; the
definition is the author's to amend.

## Question 3: the cryptography on macOS

**What stands**: `runtime/platform/hero_crypto.c` (3,545 lines, its table at 17-55, its arms after),
`runtime/hero_crypto.h` (263), Rebel's `crypto/seal`, `crypto/keypairs` and their floors
(`packages/rebel/0.1.0/files/crypto/hero_crypto.hero`), the 15 ledger rows of `bw/q3/macgap.txt`
(`00-shared.md` § Question 3), the layer's build (`selfhost/cli/layer.hero`, 472 lines: compiled with
clang on the machine that builds the program, keyed, archived, linked after the program's objects).
**The writer's probe**: `bw/q3/swift/` (`swp.swift`, `swp.h`, `main.hero`; copy it, never build in it):
one CryptoKit function through `@_cdecl`, a static library from `swiftc` 6.4.2 in 2.31 s, a Heroes
`extern "swp.h" link "swp"` group, RFC 8032's first public key printed, exit 0; plain build only.

**What you are asked**:

1. **Route (S), a member in Swift**: build `crypto/seal`'s AES-GCM and ChaCha20-Poly1305 and
   `crypto/keypairs`' Ed25519 and X25519 over CryptoKit as a member of the platform layer's archive in your
   copy (the layer compiles C alone today: say what `layer.hero`, the key, `heroes doctor` and the
   `--sanitize` link owe for a `.swift` member, and build it); run Rebel's own tests of the two packages
   on this Mac (`heroes test` of the two faces alone, through `examples-rebel/`'s `path` entry or a copy
   of it), at both levels and under `--sanitize`; and RFC 8032's and RFC 8439's vectors. **The Command
   Line Tools carry `swiftc`, reached through `xcrun` or a named SDK** (`00-shared.md` § Question 3: the
   critic built the writer's probe under the CLT alone with `xcrun swiftc -sdk "$(xcrun
   --show-sdk-path)"`, bare `swiftc` failing to load the standard library): build the member's `swiftc`
   call that way, and say what the CI's `macos-26` leg holds (its image's documentation;
   `.github/workflows/ci.yml:368`, `:1434`). **What it leaves**: Windows' Ed25519 (CNG has
   none, `hero_crypto.c`'s note 3), X25519 there unmeasured.
2. **Route (H), the primitives in Heroes. FIRST, the function already in the tree** (the critic's finding
   9): Rebel's `crypto.equals_constant_time` (`packages/rebel/0.1.0/files/crypto.hero:567`, its body
   `crypto/internal/compare.hero:10-20`, the loop `difference @ difference | to_i64(a[at]).must() ^
   to_i64(b[at]).must()`, *"What the C compiler makes of it is not proven here"*), whose package refuses
   every cipher for exactly that reason (`crypto.hero:15-21`). Build a program calling it, at `-O0` and
   `-O2`, on this Mac and in Linux arm64; read its emitted C (`--emit-c`) and its machine code (`otool
   -tv`, `objdump -d`) for a branch or a memory index that depends on a byte's value, the bounds check of
   `a[at]` and the `.must()` of each `to_i64` among them; and run a valgrind taint in Linux arm64 if the
   image has valgrind (`which valgrind`), the secret bytes marked undefined. A secret-dependent branch found
   there is a defect of the frozen tree, filed by the coordinator. **Then**, if the answer leaves the route
   open, write one primitive, X25519 or ChaCha20, in Heroes in your copy, under the spec's rules a
   primitive must live with (*"bitwise & | ^ ~ << >> (i64 only; shift count 0..63 or it aborts)"*, spec
   line 210; *"Overflow aborts at every width ... `>>` copies the sign"*, line 220: every 32-bit add modulo
   2^32 and every limb product written inside `i64` with masks, no wrapping operator existing), and read it
   the same way, the overflow checks (every arithmetic `__builtin_*_overflow`, CLAUDE.md § 7: is an
   overflow check on a secret a secret branch?) and the bounds checks among what you read. **This is the
   question the decision rests on**: whether Heroes as the compiler emits it today can hold a
   constant-time discipline at all, measured. Rebel already holds `argon2`, `bcrypt`, `scrypt`,
   `blowfish` and the hashes in Heroes (`ls crypto/internal`): say whether the password hashes owe the
   same reading.
3. **Route (C)**: AES-CTR from CommonCrypto (public on macOS, `CommonCryptor.h`) with GHASH written
   beside it; say where the constant-time half then stands, and price it without building it if (H)'s
   answer decides it.
4. **Route (V), nobody's in the page**: a small constant-time C implementation of the four primitives in
   the platform layer, as C of the project's own (the member is C already); name a candidate the
   historian can source (its licence, its audit, its size by `wc -l`), build it into the member on this
   Mac and Linux arm64 if one is at hand, and say whether decision 24 reaches it.
5. **The conservative**: the gap kept, 0 lines; what every inheriting package (`jose`, `nats`,
   `keyring`, `web`, `webauthn`, `x509`) answers on macOS today, one call each, run.
6. **The TLS-over-a-stream gap** (`tls/client-over`, `tls/server-over`, `http` client/proxy): not this
   question's, by the brief's reading; say whether any route above closes it, in one sentence.

## Question 5: the platforms of a stack of calls

The compiler-engineer builds the walk; you judge what each platform gives it. **What stands**:
`runtime/parts/stack.c` (1,379: `hero_stack_symbol` 585, `hero_stack_names` 862, Windows' *"no walk"*
858, dbghelp's conditions 1231 and 1300-1301), `runtime/parts/symbols.c` (192, Linux's own symbol table,
since glibc's `dladdr` reads only `.dynsym`, `stack.c:579-583`), the flags (`selfhost/cli/flags.hero:133-153`:
no `-fno-omit-frame-pointer`).

**What you are asked**:

**What the brief settled for you** (`00-shared.md` § Question 5, the critic's findings 7 and 8, re-run by
the writer): this Mac's clang driver (`clang -### -target <t> -O2 -c f.c`) gives `-mframe-pointer=non-leaf`
for `arm64-apple-macos` and `aarch64-linux-gnu` and `none` for `x86_64-linux-gnu` and
`x86_64-pc-windows-msvc`; the images' and the CI's clang are other builds. Every build already carries line
tables (`selfhost/cli/flags.hero:172-175`), and on Darwin they stay in the build's objects: `bw/q5/deep-O0`
has no `__debug` section, a debug map of two `N_OSO` entries and no `.dSYM`.

1. **Frame records at `-O2`, and W's Linux arm64 rows**: build `bw/q5/deep.hero` (copy it) at `-O2` in
   Linux arm64 and in the emulated x86-64 image (the compiler-engineer builds W on this Mac), read each
   image's clang default the same way (`clang -### -O2 -c f.c | grep -o -- '-mframe-pointer=[a-z-]*'`), and
   say whether a walk from the abort reaches `main` through `outer`, `middle` and `pick` on each (lldb's
   `bt` in the binary, or a probe of your own), with and without `-fno-omit-frame-pointer`; what an inlined
   call leaves. Where the compiler-engineer's W prototype is ready in its report, build it in Linux arm64
   from a copy of its files and run the same three programs.
2. **Lines**: the line tables are there on every build (above); whether each frame's line can be had **at
   run time** on each platform without a tool outside the binary (on Darwin the objects of `build/`, a
   `dsymutil` step the link does not take, `atos`; on Linux the ELF's own tables; a table the emitter
   writes), and what each **adds** in bytes over today's binary, whose size already pays for the tables
   (`ls -l` before and after the change, not before and after the tables). And the critic's route (A):
   the runtime writes return addresses only, and `heroes run` or `heroes test`, the program's parent,
   resolve them: what that needs on each platform.
3. **Windows**: what dbghelp's `StackWalk64` and `SymFromAddr` need (a PDB beside the binary, lld's
   `/DEBUG`), read from Microsoft's documentation, the box owed; and whether route (P) of `00-shared.md`
   § Question 5, which needs none of it, is the one the three platforms can hold the same way.
4. **A stop inside a C library** reached by a callback (the guard's case): what each route names there.

## What you are asked to say

Your definition's structure per question; the C (or Swift, or Heroes) you wrote, whether clang and
`swiftc` accepted it, and what it printed, each with its command; the route you would adopt, built and
run; your prediction about a named binding; every premise of these briefs you found false, first.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/253-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
