# The Linux machines — two containers, one file each, reachable in seconds

**There are TWO since 2026-09-17** (M-arm-platform), and the second is the
reason to read this heading twice: `Dockerfile.amd64` is x86-64 and runs through
Rosetta on the author's arm64 Mac, `Dockerfile.arm64` is arm64 and runs
natively. They differ on **one line**, the `FROM`'s platform — same Debian
13.6, same Debian clang 22.1.8, same glibc 2.41, same sqlite 3.46.1, same
libcurl 8.14.1, same git 2.47.3, all read off the two images on the day the
second was built. One axis, so a divergence between them has one candidate
cause and not two; § How it differs from the judge is where that discipline was
bought, by a finding that looked like a platform fact and was a clang version
fact.

**The x86-64 file was named `Dockerfile` until 2026-09-17.** The bare name had
become the one that lies — `docker build <this directory>` would have handed a
reader x86-64 without their choosing it — so there is now no default and the
architecture is a thing you type. `tests/harness/suite_records.hero`'s `MOVED`
table carries the rename so a citation in a record still resolves.

**And the arm64 one earned its place in its first hour**, which is § What the
arm64 machine measured on its first day: a golden case this repository has
shipped since 2026-08-16 does not compile there, and the compiler's own
diagnostic tells the author to write the spelling it has just refused. Defects
058 and 059.

**There is a Linux x86-64 for this project on the author's own Mac, and it is
not the CI.** Until 2026-09-03 every Linux fact in this repository was learned
through the Linux leg of GitHub Actions, seven minutes per question, and the
day before, the program that is now `examples/ctime/` — then called filestat,
a directory that no longer exists — shipped with a comment that named three
platforms and had been run on one. This machine answers a Linux question in
the time it takes to compile the seed, so a platform fact is **measured before
the commit** instead of read red after it.

**What it is and what it is not.** It is the **hunting instrument**, exactly as
`docs/ref/environment/windows/WINDOWS-MACHINE.md` is for Windows: it finds where something
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
| Built image | `heroes-linux`, 1.93 GB; 1.99 GB since git joined (2026-09-09) |
| Inside | Debian 13.6 (trixie), Debian clang 22.1.8, lld, lldb 22.1.8, glibc 2.41, pkg-config, sqlite 3.46.1, libcurl 8.14.1, git 2.47.3 |

## The two files, and how to build them

The two `Dockerfile`s beside this file are the whole of both machines:

```
docker build -f docs/ref/environment/linux/Dockerfile.amd64 -t heroes-linux       docs/ref/environment/linux
docker build -f docs/ref/environment/linux/Dockerfile.arm64 -t heroes-linux-arm64 docs/ref/environment/linux
```

**`-f` is not optional and that is the design.** Neither file is named
`Dockerfile`, so a build that does not say which architecture it wants fails
instead of choosing one. The image names carry the same distinction, so a `docker
run` cannot pick the wrong machine either.

`silkeh/clang:22` publishes both architectures under the one tag — measured
2026-09-17, `docker manifest inspect` gives amd64 `sha256:42cebd4a…` and arm64
`sha256:d315ac7f…` — which is what makes the two images siblings rather than
cousins, and what makes the major-version pin cost nothing on the new leg.
**The arm64 image built in 145 s including the base-image pull** (19.4 s of
that in Docker's own layer export; the rest was the network, so the figure is a
first-build number and not a rebuild one) and comes to **3.1 GB** against the
x86-64 image's 1.99 GB.

Each file names the base
image by major version, so that a `latest` moving to clang 23 cannot move this
instrument in silence, and it installs the five things the bare image lacks and
the CI leg has: `git`, `lldb-22`, `pkg-config`, `libsqlite3-dev`,
`libcurl4-openssl-dev` (the CI's Linux install step, minus `clang`, which the
image is, plus `git`, which the CI runner ships and the bare image does not).
raylib is left out on purpose, as in CI, so
`examples/raylib/` goes on exercising the skip rule. `git` was the fifth and
arrived last (2026-09-09): the net's own tests have asked `git check-ignore`
since 2026-09-07 and one of them was red here, unreported, until a run read the
count instead of the goldens; and the `records` suite reads commits and tags, so
a copy that carries `.git` can now run all three suites on this machine. Both
lines are run from the repository root, and both took a first build of the same
shape: **16.5 s for x86-64** on 2026-09-03, apt layer included, and the arm64
figure above. A rebuild with nothing changed is served from the cache.

Each file's first line is a parser directive that
switches off one lint, `FromPlatformFlagConstDisallowed`: Docker warns that
the `FROM` names a constant platform, and it does, in both files and for the
same reason — the architecture is the question here, so a portable Dockerfile
that let the host choose would be an instrument that answers whatever it is
asked. The comment above each directive says so in the file itself.

Nothing of Heroes is built by either Dockerfile. The compiler inside is the one
clang line from CLAUDE.md § Commands, typed by hand at run time, so the file is
an environment and not a build script (CLAUDE.md §10).

## How to run something on one of them

The repository is **mounted read-only and copied inside**. Every run starts
from a fresh copy and a fresh seed build, and both are cheap here. The image
name is the only thing that changes between the two machines:

```
docker run --rm -v "$PWD":/src:ro heroes-linux bash -c '
  tar -C /src --exclude=.git --exclude=build -cf - . | tar -xf -
  clang -I runtime seed/heroes.c runtime/runtime.c -o heroes
  ./heroes test examples/ctime/main.hero'

docker run --rm -v "$PWD":/src:ro heroes-linux-arm64 bash -c '…the same body…'
```

**The copy is also what frees the tree**, and that is worth stating because it
is not obvious: the `tar` finishes in seconds and everything after it reads the
container's own copy, so a long run here does not own the Mac's working tree
the way a local suite does (CL-025, `.claude/rules/verification.md`). Editing a
document while the net runs in a container is safe, and waiting for it is the
mistake `.claude/rules/records.md` § Working in lanes names. The clock is still
exclusive: a timing taken while anything else runs is discarded.

The copy is **3.8 s for 273 MB**; the seed build is **6.1 s** (3.5 s on the Mac,
7.7 s on the Windows box). `--rm` discards the copy with the container, which
is the point: nothing this machine builds survives into the Mac's tree.

**Drop `--exclude=.git` when the run is the net's own tests or the `records`
suite.** Both ask git: the citation test for its ignore rules, `records` for
the commits and tags. Without `.git` the net prints *the record checks could
not run* and the own tests carry one red case, and neither is a defect of the
tree. The exclusion stays in the line above because a golden or a single suite
needs none of it and the copy is a fifth smaller without it.

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

## What the x86-64 machine measured on its first day

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

## What the arm64 machine measured on its first day

Everything below was run on 2026-09-17, inside the image as built that day.

**The machine works, and says so in its own words.** The seed built from C
alone; `heroes --version` printed `heroes 0.2.0`; `heroes doctor` reported six
rows `ok` with `arch aarch64`, the clang floor among them (clang 22 against a
floor of 18); and `heroes test selfhost/main.hero` read **654 tests, all
passed** — the same count this tree reads on the Mac.

**And then it refused something the other three accept.** Plain `char` is the
finding, and it is the milestone's own registered prediction coming back half
false:

| machine | plain `char` | |
|---|---|---|
| Linux arm64 | **UNSIGNED**, `CHAR_MIN 0`, `CHAR_MAX 255` | as predicted |
| Linux x86-64 | SIGNED, `-128` … `127` | as predicted |
| Darwin arm64, this Mac | **SIGNED**, `-128` … `127` | **against the prediction** |

The milestone file registered *"plain `char` is unsigned on the ARM ABI"*, and
that sentence is false as written. The generic AAPCS does say unsigned, and
Debian arm64 follows it; **Apple's own arm64 ABI deviates and declares `char`
signed**, so the cause is the platform's ABI and not the architecture. Three
legs were all signed and could not have shown it — which is the whole argument
for a fourth real machine, arriving on the day the machine arrived.

What it costs is **defects 058 and 059**, both measured here and both with
their reproducers in `docs/work/DEFECTS.md`: there is no spelling of a plain
`char` field or parameter that binds on every platform (`i8` on two legs, `u8`
on the third, every shape inverting), so
`tests/golden/run/ffi-a-char-array-member.hero` is accepted on x86-64 and
refused here; and on this machine the `ffi_parameter_type` note tells the author
to *"Declare it `i8`"* about the `i8` it has just refused, because
`selfhost/emit/c_spellings.hero:59` tables the answer instead of asking the
target.

**The sign check itself is correct on both machines**, and that is the part
worth keeping straight: `extern_field.hero:154` compares the header's sign to
the declared one and answers rightly each time. Nothing here is a bug in the
comparison. The hole is that C has three `char` types where Heroes has eight
integers of fixed sign, so the author's question — *which one do I write* — has
no portable answer to give.

`INT64_MIN % -1` and structure padding are the next two shapes this machine is
pointed at, in that order, and both are **unrun** as of this line.

## How they differ from the judge

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
