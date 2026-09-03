# The Linux machine — a container, built from one file, reachable in seconds

**There is a Linux x86-64 for this project on the author's own Mac, and it is
not the CI.** Until 2026-09-03 every Linux fact in this repository was learned
through the Linux leg of GitHub Actions, seven minutes per question, and the
day before, the program that is now `examples/ctime/` — then called filestat,
a directory that no longer exists — shipped with a comment that named three
platforms and had been run on one. This machine answers a Linux question in
the time it takes to compile the seed, so a platform fact is **measured before
the commit** instead of read red after it.

**What it is and what it is not.** It is the **hunting instrument**, exactly as
`docs/environment/windows/WINDOWS-MACHINE.md` is for Windows: it finds where something
dies, it prices a hypothesis, it lets a fix be tried before it is committed. It
is **not the judge**. The Linux leg of `.github/workflows/ci.yml` stays the
acceptance criterion, and a green run here is evidence, never the criterion.
The section *How it differs from the judge* below says why that distinction is
not a formality on this machine in particular.

## It runs on Docker Desktop, which is usually off

Docker Desktop is installed on the Mac and **its resting state is stopped**:
`docker info` fails until the author starts it, and the author is the one who
does (2026-09-03, *"docker is on now"*, after the assistant had said there
was no Linux instrument to hand). When it is off, the assistant asks for it
in so many words and does the machine-free parts of the work while waiting,
which is the same protocol as the Windows box.

Measured 2026-09-03 on the author's Mac (`venus`, arm64, macOS 26.6.2):

| | |
|---|---|
| Docker Desktop | client and server 29.7.2 |
| Emulation | Rosetta (`UseVirtualizationFrameworkRosetta: true`); `uname -m` inside says `x86_64` |
| Visible to a container | 8 CPUs, 7.8 GB |
| Base image | `silkeh/clang:22`, the same digest as `:latest` that day (`sha256:ca3544b0…`), 1.82 GB |
| Built image | `heroes-linux`, 1.93 GB |
| Inside | Debian 13.6 (trixie), Debian clang 22.1.8, lld, lldb 22.1.8, glibc 2.41, pkg-config, sqlite 3.46.1, libcurl 8.14.1 |

## The one file, and how to build it

The `Dockerfile` beside this file is the whole machine. It names the base
image by major version, so that a `latest` moving to clang 23 cannot move this
instrument in silence, and it installs the four things the bare image lacks and
the CI leg has: `lldb-22`, `pkg-config`, `libsqlite3-dev`, `libcurl4-openssl-dev`
(the same list as the CI's Linux install step, minus `clang`, which the image
is). raylib is left out on purpose, as in CI, so `examples/raylib/` goes on
exercising the skip rule. From the repository root:

```
docker build -t heroes-linux docs/environment/linux
```

**16.5 s the first time**, apt layer included; a rebuild with nothing changed
is served from the cache. The file's first line is a parser directive that
switches off one lint, `FromPlatformFlagConstDisallowed`: Docker warns that
the `FROM` names a constant platform, and it does, because the CI leg is
x86-64 and nothing else is the instrument. The comment above the directive
says the same thing in the file itself.

Nothing of Heroes is built by the Dockerfile. The compiler inside is the one
clang line from CLAUDE.md § Commands, typed by hand at run time, so the file is
an environment and not a build script (CLAUDE.md §10).

## How to run something on it

The repository is **mounted read-only and copied inside**. Every run starts
from a fresh copy and a fresh seed build, and both are cheap here:

```
docker run --rm -v "$PWD":/src:ro heroes-linux bash -c '
  tar -C /src --exclude=.git --exclude=build -cf - . | tar -xf -
  clang -I runtime seed/heroes.c runtime/runtime.c -o heroes
  ./heroes test examples/ctime/main.hero'
```

The copy is **3.8 s for 273 MB**; the seed build is **6.1 s** (3.5 s on the Mac,
7.7 s on the Windows box). `--rm` discards the copy with the container, which
is the point: nothing this machine builds survives into the Mac's tree.

**Why it is copied and not mounted writable, measured.** The first run of the
day, before this file existed, mounted the repository at `/w` and ran
`heroes test` there. It passed, and it left **eight x86-64 ELF files in the
Mac's own `build/` cache**: a `runtime-<hash>.o`, three program directories
with a Linux `main` and `main.c.o`, two `tu-<hash>` objects. The Mac's compiler
did not trip on them, because the cache key told the two apart, but a cache
holding objects for a machine it does not run on is a cache that will confuse
the next person who reads it. The six entries were deleted the same hour and
the run line above cannot recreate them.

**Two things Docker prints that are not errors.** The run warns that *the
requested image's platform (linux/amd64) does not match the detected host
platform*: that is the instrument telling the truth about itself and it is
expected on every run. And `--platform linux/amd64` on `docker run` is
deliberately **absent** from the line above: with it, Docker Desktop answered
*Unable to find image 'heroes-linux:latest' locally* and tried to pull, while
the same image without the flag runs and reports `x86_64` (both measured
2026-09-03). The platform is fixed by the `FROM`, so the flag has nothing to
add and one thing to break.

## What it measured on its first day

Everything below was run on 2026-09-03, inside the image as built that day.

| What | Here (Rosetta) | Mac (arm64) |
|---|---|---|
| Seed build, `clang -I runtime seed/heroes.c runtime/runtime.c` | 6.1 s | 3.5 s |
| `heroes doctor` | all five rows `ok`, `arch x86_64` | |
| `heroes test examples/ctime/main.hero` | 6 of 6, also under `-O0` and `--sanitize` | 6 of 6 |
| `heroes test selfhost/main.hero`, 536 tests | 97.9 s | 53.6 s |
| `heroes build selfhost/main.hero -o heroes-next` | 176.9 s, 17,633,024 bytes | 43.9 s |

The compiler's own tests run at 1.8× the Mac's time and the self-build at 4.0×;
those ratios are Rosetta's, not the code's, and a timing taken here is not
comparable to one taken on the Mac or in CI. The glibc widths `examples/ctime/`
rests on were read here: `struct tm` 56 bytes, `tm_year` 4, `time_t` 8,
`size_t` 8. **The full net was not run here** and no figure for it is offered.

**And the instrument caught something on its first afternoon.**
`heroes test examples/sqlite/main.hero` is accepted on the Mac and on the CI's
Linux leg and is **refused here**, twice, as `ffi_parameter_type`:

```
error[ffi_parameter_type]: `out` of `sqlite3_open` is declared a different kind of thing from the header's `struct sqlite3 **` — clang read the header, and the two do not convert
  at examples/sqlite/main.hero:34:5
error[ffi_parameter_type]: `error` of `sqlite3_exec` is declared a different kind of thing from the header's `char **` — clang read the header, and the two do not convert
  at examples/sqlite/main.hero:36:5
```

The header is not the difference: Debian's `sqlite3.h` 3.46.1 declares
`sqlite3_stmt **ppStmt` exactly as Apple's does. **The compiler is.** A
two-line C probe, `void **` passed where `struct S **` and `char **` are
declared, gets **two errors and no warning** from Debian clang 22.1.8 and **two
warnings and no error** from Apple clang 21.0.0, both under default flags, both
under the same name, `-Wincompatible-pointer-types`; with
`-Wno-error=incompatible-pointer-types` clang 22 falls back to zero errors.
clang 22 made that diagnostic an error by default, as GCC 14 did before it.
Heroes' own flag list (`selfhost/cli/flags.hero`) names the sibling,
`-Werror=incompatible-pointer-types-discards-qualifiers`, and not this one, so
on a clang that only warns the program passes and on a clang that errors the
`ffi_narrowed` class reads the error and reports the `.hero` line. An `@`
parameter is a pointer parameter (CLAUDE.md §7), so `@out: ptr` is `void **`
against the header's `sqlite3 **`, and clang 22 is right that C does not
convert those. `examples/curl/main.hero` passes here.

**What follows is a decision, not a repair**, and it is filed in
`docs/work/DECIDE.md` rather than made in this file: the FFI verdict on a
program should not depend on which clang reads it, and the two directions
(name `-Werror=incompatible-pointer-types` everywhere, which refuses the sqlite
example on every platform until it declares the structs; or
`-Wno-error=incompatible-pointer-types`, which keeps clang 22 as permissive as
the others) each change a flag on panel 047's list. Until it is decided, the
CI's Linux leg is green on Ubuntu 24.04's clang 18.1.3 and will stop being
green on the day `ubuntu-latest` carries a clang that errors.

## How it differs from the judge

The CI's Linux leg is `ubuntu-latest`, which on 2026-09-03 was **Ubuntu 24.04
with Ubuntu clang 18.1.3** (read from the run's own `heroes doctor` step). This
machine is Debian 13 with clang 22.1.8. Same architecture, same libc family,
**not the same compiler**, and the sqlite finding above is the measured proof
that the difference is not academic. So:

- a fact about **the platform** (a struct's size, a field's width, a libc
  function's presence, an exit code) measured here is a fact about the judge's
  platform too;
- a fact about **clang's verdict** on a program is a fact about clang 22, and
  the judge's clang 18 may say otherwise in either direction — such a finding
  goes to CI to be read, and is written down as clang 22's until then.

This is the same stance the Windows document takes toward its own box, one
compiler version apart instead of one machine apart.
