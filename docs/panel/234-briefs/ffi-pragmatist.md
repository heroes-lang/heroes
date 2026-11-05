# Panel 234, the ffi-pragmatist's brief

Read `00-shared.md` first, then `proposal.md`, `conventions-draft.md` and
`probe/README.md`. You judge the founding constraint (design.md §1.11,
§4.19): the C a real binding needs under each route, which you write and
compile, or show why it cannot be. You have a veto on ABI breakage. A second
instance of your seat writes item D's SQLite shapes
(`ffi-pragmatist-sql.md`); this brief is everything else.

**Your worktree** is `.claude/worktrees/234-ffi-pragmatist/`, detached at the
commit that adds these briefs; build your compiler there from the seed and
check its key (`00-shared.md`, *The tree*). Your scratch is
`.claude/worktrees/234-ffi-pragmatist-scratch/`. Your report is
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/234-reports/ffi-pragmatist.md`,
written as you go. The Linux arm64 container is yours to use: `docker run --rm
-v <your worktree>:/w -w /w heroes-linux-arm64:latest ...`, building the
compiler inside it from the seed. Windows is unrun in this sitting; where a
route's claim is about Windows, say what it would need and mark it unrun.

## Your items

**F, the platform layer (the author's instruction 4), first.** Take two
capabilities the core packages need and that differ between platforms: **a
monotonic clock in nanoseconds** (`clock_gettime` with a `clockid_t`, and what
Windows has; **not `timespec_get`**, which offers only `TIME_UTC`, a wall
clock, on both machines, `TIME_MONOTONIC` defined on neither, the critic's run
F16) and **a TCP listener on loopback
that accepts one connection and echoes one line** (`socket`, `bind`,
`listen`, `accept`, `sockaddr_in` or `sockaddr`; Darwin's `sa_len`). Build
each one under **P2** (a platform layer beside the runtime: a header and a C
file of the project's own, with the `#ifdef`s inside, bound from Heroes with
fixed-width types so no platform typedef reaches a `.hero` file) and under
**P3** (a header of `static inline` functions shipped beside the package,
panel 114 R2's shape), and say what **P1** (the same C as runtime parts behind
`hero_os.h`) would be: compile and run each on this Mac and in the Linux arm64
container. For P2, you link the extra object by hand (the compiler cannot yet:
the compiler-engineer prices that); say exactly what you did. For P3, the
header shipped beside a package below the program's root meets the lookup the
critic measured (F15: found from the compiled file's directory, so
`extern "core/net/net.h"` from the root, `extern "net.h"` alone, or
`--include`): build it under route B2's `core/` and say which spelling you
used and what each placement costs. **Your seat's veto of panel 114 R3**
(F7): R2's two-arm shape does not land without the compiler reporting which
`extern` declarations it did not verify on this machine, since a wrong type in
the arm a machine does not compile is clang exit 0 and a running binary; P2 and
P3 are both two-arm shapes, so run that failure on each (a deliberate wrong
type in the arm this Mac does not compile, built here and in the container)
and say whether your veto binds the route. Then, from the
C you wrote: which of the carried facts of the shared brief's item F still
hold (`sockaddr`'s fields, `clockid_t`'s sign and width, `pthread_t`), each
re-run on both machines; how many `#ifdef` lines each route needed; and what a
Windows arm would add (winsock's `WSAStartup`, `SOCKET` as an unsigned 64-bit
handle, `closesocket`), marked unrun. Judge the routes against §1.12: which
of them can let a wrong width reach C in silence on a platform the CI does not
build first.

**G, the byte buffer.** Bind `read` (or `recv`) into a `[u8]` buffer with
spec § 13's `counted_by` and `lent`, read a file of a few KB and a binary file
through it, and turn the bytes into a `str?` with `validated_bytes()`; say
whether question (iii)'s three routes (per-byte runtime entries, a `[u8]`
result admitted for a group over `heroes_runtime.h`, a built-in) are all
answered by what § 13 now says, or which is still owed, beside the fourth
route already in use, `hero_str_from_bytes(p: ptr, n: i64)`
(`runtime/heroes_runtime.h:358`, called by `examples/checksum/base64.hero`;
F36). Run it on both machines.

**H, a C name spelled as a word Heroes reserves (issue 1)**, re-aimed on the
critic's runs (F11): a contextual word blocks nothing (POSIX `link` binds and
runs; fields named `tag`, `lent`, `when` run), so count **the 21 keywords of
`selfhost/keywords.hero`'s `keyword()` and the 17 foreign words of
`selfhost/foreign_words.hero`'s `declared_word`**, in **function position and
in field position apart**, with a method you write down and a later reader
can re-run, across this Mac's SDK headers (`xcrun --show-sdk-path`), the
installed Homebrew headers (`brew --prefix`), and, separately, the headers of
the libraries this milestone binds (SQLite, libcurl, `stdio.h`, `time.h`,
`sys/socket.h`, `netinet/in.h`, libcrypto). In function position the compiler
already prescribes a shipped header's `static inline` renaming in its own
refusal; build it once for a real symbol you found and confirm. **In field
position there is no route** (`record R` with a field `match`: `expected_field`
and three cascading errors, F11): count how many fields of the structs this
milestone's packages need are spelled so, and price the two forms that would
reach them (F33), an identifier escape (Rust's `r#match`, Zig's `@"match"`)
and a `tag` on a record field, against leaving the field to `partial`, which
spec § 13 offers for a field a record does not name.

**I, a group's header.** Three positions (F9): per group, per module, and
per module plus the seeds (every unit includes `heroes_unit.h` and
`hero_os.h`), which is what the compiler does and which alone explains
`selfhost/cli/process.hero:66-69`. Compile a module with two groups where one
group declares a function only the other's header declares, with no seed
involved, and see what clang and the compiler say; then say whether a
per-group check is possible from what clang can be asked (one probe
translation unit per group, including that group's header alone, with and
without the seeds), and what each would refuse.

**J, the C-width vocabulary (design.md Part 7 item 10).** From the bindings
the core packages need (the page's table: `time.h`, sockets, `stdio.h`,
SQLite, libcurl, libcrypto), list every parameter or field whose C type is
`long`, `unsigned long`, `size_t`, `ssize_t`, `socklen_t`, `time_t`,
`clockid_t`, `off_t` or another typedef whose width or sign is the platform's,
and say for each, on the route F adopts, whether its spelling in a `.hero`
file is the same on **all four CI legs** (Darwin and Linux arm64 run; Windows
and Linux x86-64 marked unrun; the falsifier says three, written when there
were three, F10). Separate a parameter, where a narrower spelling C converts
builds (`labs(x: i32)` against `long labs(long)`, design.md Part 7 item 10's
correction of 2026-10-25), from a struct field, whose layout must match. That
is item 10's falsifier, asked of this milestone.

**B and C, from the C side.** A package over C placed under a program's root
(route B2's `core/`): where does clang look for the header a group in
`core/database/sql/sqlite3.hero` names, when the program is compiled from the
root, and for a header shipped beside that module (`core/net/net.h`)? Run it.
And judge `conventions-draft.md` §1.4 (the C module, named for its header) and
§4.3 (a handle as a group's record with its marks, ended by one function with
an `@` parameter) against the C you write.

## Report shape

Per item: **verdict** (approve, object, or veto, and on which route), **section**
(design.md or spec), **cost** (the C and the Heroes you wrote, in files and
lines), **prediction** (falsifiable, with the milestone where it is checked),
**condition** (what would change your verdict). Then what you could not run,
and why.
