# Panel 198, completeness critic, second pass

Copied by the coordinator at 11:08 on 2026-10-09 (`date`) from the critic's
reply, verbatim apart from this header: a subagent's Write of a report file is
refused, and its running notes and probes are
`.claude/worktrees/scratch-b15/198-critic/` (ignored by git).

---

Clock (`date`): read 10:52, 10:57, 11:00, 11:06 CEST, 2026-10-09. My copy of the frozen tree (`56def9b4`, excluding `.claude/worktrees`, `.git` removed), its compiler built from the seed in 5.6 s, and route K rebuilt from the compiler-engineer's prototype (115 s), all under `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/198-critic/`; notes in `notes.txt`, probes in `probe/`.

## The question the sitting did not ask (and a route nobody listed)

**The admitted words' VALUES are an unfiltered `@file` channel, on both platforms, in the frozen compiler and in K alike.** The sitting debates *which words* the list admits; it never asks what an admitted word's *value* may carry. Measured, with a stand-in `pkg-config` answering a `.pc`'s line to the seed-built frozen compiler:

- `-framework @rsp` (admitted next-word flag; `rsp` held `Foundation -Wl,-map,…`): exit 0, the map file was written — clang expanded the response file.
- `-Wl,-framework,@rsp` (`rsp` held `X --ld-path=/…/fake-ld.sh`): exit 0 on macOS, and the program named inside the response file ran as the linker at build time.
- `-Wl,-rpath,@rsp` (`rsp` held `/x/lib -map …`): exit 0, map written, `LC_RPATH /x/lib`. On Debian 13 (GNU ld 2.44, clang 22.1.8) `-Wl,-rpath,@rsp` likewise wrote the map; `-framework @rsp` ran the external ld then exited 2.

Control: `-I@rsp` (joined) is **not** expanded — the door is only the separate-token value slot of the admitted `-framework` / `-Wl,-framework,` / `-Wl,-rpath,` forms. `clean_tail` and the next-word test reject only a leading `-` and a comma; `@` passes. **This falsifies `docs/design.md:2639-2642`'s claim that "a `.pc` answering `@…` is `ffi_package` at exit 1 … Panel 050's allow-list holds against it."** It holds for `-Wl,@file` standalone (refused), not for `@file` inside an admitted value. **Route nobody listed — (V): complete the value check on the words already admitted** (reject a `@`-prefixed value; reject the `-l:<file>` spelling, below), independent of the dtags question. This is the most robust fix and it is orthogonal to every route A–J.

## Claims asserted and not measured

- **compiler-engineer:** "All seven headers under Debian's `/usr/include/mit-krb5` compile under `-I`." There are **nine** `-isystem` packages, all naming that one directory, and ~40 headers in it. Read literally the claim is false: `gssapi/gssapi_alloc.h` is clean under `-isystem` (exit 0) but fails under `-I` (exit 1, implicit `malloc`/`free`). The CE did not state the saving fact (below).
- **compiler-engineer:** `--enable-new-dtags`/`--export-dynamic` give "byte-identical binaries on … Ubuntu GNU ld 2.42." The CI's leg is **native Ubuntu 24.04 x86-64**; that was measured on arm64 and emulated x86-64, never natively. The CE itself lists the CI leg under "could not run."
- **spec-warden:** "the note's reason clause is false." **Checkable and correct:** the clause "everything else is a flag a package file could use to run code during the build" is false for the refused words (`--disable-new-dtags`, `--as-needed`, `-z,relro`, `-Wno-*` run no build-time code), while the measured code execution comes from the *admitted* words' `@file` values. The list's stated rationale is inverted by the measurements, and K keeps the same clause.

## The contradictions named in the brief

**ffi vs compiler-engineer is not a contradiction on dtags.** ffi recommends dropping `--enable-new-dtags` silently and keeping `--disable-new-dtags` refused; K does exactly that. They diverge only in scope: K also admits `-pthread` and `-isystem`-as-`-I`, which ffi explicitly leaves "open … not settled by this seat." The checkable side is K's behaviour, which I tested.

## Can K's `-isystem`→`-I` break a correct library?

**Not for the measured packages.** The six entry headers a binding actually names (`gssapi.h`, `gssapi/gssapi_krb5.h`, `krb5.h`, `kdb.h`, `kadm5/admin.h`, `gssrpc/rpc.h`) compile exit 0 under **both** `-I` and `-isystem` with the compiler's full flag set. The failing `gssapi_alloc.h` is unreachable (`gssapi.h` does not include it, `-E` count 0). The only warnings that differ (`-Wvisibility`, `-Wimplicit-int`, `-Wimplicit-function-declaration`) are **not** in the compiler's `-Werror=` allowlist, so none becomes a hard error. The residual risk — a package on some platform whose *entry* header trips a `-Werror` category under `-I` — was not found in the measured set; the Mac census has 0 `-isystem` packages and the CI's own set (curl, sqlite3) keeps `curl/curl.h` in a `-I` directory already.

## K's dropped words inert on the CI's leg?

**Unrun on the native runner.** No Ubuntu 24.04 x86-64 image is local (`ubuntu:24.04` resolves arm64 here). RUNPATH-by-default is confirmed on Debian 13 x86-64 (emulated) and on Ubuntu 24.04's x86-64 cross-ld (ffi); both make `--enable-new-dtags` a no-op, but neither is the native `ubuntu-latest` toolchain. The CE's prediction already flags this as the falsifier.

## The two exit-2 packages

`libiodbc` (`-liodbc` absent) and `libuv-static` (`-l:libuv.a`) exit 2 — *"internal error: linking failed"* — **identically under the frozen compiler and under K.** K touches neither: `-l` names pass through unchanged. Note `-l:libuv.a` is GNU ld's file-naming spelling admitted via the `-l` prefix — a word that *names a file* yet passes the list, contradicting the list's stated property, orthogonal to 198 and worth filing as its own defect (a package `-l` the platform's linker cannot resolve should be a Heroes diagnostic, not exit 2).

## What the blind seat can and cannot carry

**Can:** on both tasks today's message (M) already leads readers to the safe `link` way out; (G)'s `--allow-word`, offered beside `link`, was **not** taken (both R readers chose `link`, one citing CVE-2018-6574); no reader edited a `.pc` or wrapped `pkg-config`. **Cannot:** both tasks sit on default-search-path worlds with `ldconfig` already run, so `link` works with no rpath cost — the hard case (a machine where `link` loses a needed path, the exact case both R readers named `--allow-word` as the *fallback* for) is unmeasured; n=6, one model, one shape each; and the scoring's "K" column is route (A)'s note (K builds silently, so **nobody ever reads K's own refusal note** in the experiment). It measures a refusing-build repairer, not a package author or a later reader of a silently-building program.

## What I could not run

Native Ubuntu 24.04 x86-64 (no image); the full net, census, fixpoint, Windows (not needed for these questions); and I did not re-verify the historian's Go commit dates on the web (sourced in its report).
