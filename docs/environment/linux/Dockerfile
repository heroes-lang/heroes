# check=skip=FromPlatformFlagConstDisallowed
#
# The Linux machine of this project, as one file.
#
# The first line is a parser directive and has to be the first line. Docker's
# lint warns that the `FROM` below names a constant platform, because a
# portable Dockerfile would let the host choose; this one is not portable on
# purpose (the CI leg is x86-64 and nothing else is the instrument), so the
# warning would say the same true thing on every build and is switched off here
# rather than answered each time.
#
# An x86-64 Debian carrying the clang this project's Linux CI leg compiles
# with, plus the four packages that leg installs and the image does not carry
# (`.github/workflows/ci.yml`, the Linux install step). How it is used, what it
# answers and what it does NOT answer (it is the hunting instrument, never the
# judge) is `LINUX-MACHINE.md` in this same directory, this file's only
# companion.
# Nothing of Heroes is built here: the repository is copied in at run time and
# the compiler inside is the one clang line from CLAUDE.md § Commands, typed by
# hand, so this file stays an environment and not a build script (§10).
#
# `--platform=linux/amd64` is the point, not a detail. The author's Mac is
# arm64, and an arm64 Linux answers different questions from the CI's x86-64
# one: `INT64_MIN % -1` raises SIGFPE on x86-64 and returns 0 on arm64 (the
# leg's own note in ci.yml). Docker Desktop on Apple silicon runs the image
# through Rosetta; `uname -m` inside says `x86_64`, measured.
#
# `silkeh/clang:22` and `silkeh/clang:latest` were one image when this was
# written (both sha256:ca3544b0…, measured 2026-09-03). The major-version tag
# is named so that a clang 23 `latest` cannot move this instrument away from
# the CI leg in silence. Debian 13 (trixie), glibc 2.41, clang and lld from
# apt.llvm.org's `llvm-toolchain-trixie-22`, all read off the bare image.
FROM --platform=linux/amd64 silkeh/clang:22

# What the bare image lacks and the CI leg has, measured: lldb for
# `lldb_breaks_on_a_hero_line`, pkg-config for every `package "…"` binding, and
# the two headers `examples/sqlite/` and `examples/curl/` bind. `lldb-22` comes
# from the LLVM repository the image already trusts, so the debugger matches
# the compiler's DWARF rather than Debian's own lldb, which is another major.
# raylib is left out on purpose, exactly as in CI, so `examples/raylib/` keeps
# exercising the skip rule. LLVM's apt packages install versioned names only
# and the image links its own tools to the bare name; lldb gets the same.
RUN apt-get update \
 && apt-get install -y --no-install-recommends lldb-22 pkg-config libsqlite3-dev libcurl4-openssl-dev \
 && ([ -e /usr/bin/lldb ] || ln -s /usr/bin/lldb-22 /usr/bin/lldb) \
 && rm -rf /var/lib/apt/lists/*

# Where the repository is copied to. It is COPIED, from a read-only mount,
# never mounted writable: the compiler keeps its cache in `build/`, and a
# writable mount would leave Linux objects in the Mac's own cache directory.
WORKDIR /w
