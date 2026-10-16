# Panel 216, the ffi-pragmatist's brief

Read `docs/panel/216-briefs/00-shared.md` first, whole: the question, the measurements and the
rules you keep are there.

## Your seat's question

**Whether a correct binding still builds.** A route that refuses a header value C defines makes a
correct binding unbuildable, which is your seat's veto line. So: over the real headers this
project binds (the `examples/` programs and the `tests/golden/` groups; `git grep -l -E
'^[[:space:]]*extern ' 6e2690b50 -- '*.hero' ':!docs'` lists them) and over the system headers a
binding names, which integer `constant`s would each route refuse, and are any of those values C
defines? Write the C each route asks (the C23 unit, the C++ unit), compile it against those
headers on this Mac and in `heroes-linux-arm64`, and say which headers do not compile under
`-std=gnu23` or as C++ at all, and what the route does then. Ask clang 18 too, the compiler's
floor (`selfhost/cli/clang_floor.hero:36-37`): a container with it (`ubuntu:24.04` with its
`clang` package is one way; installing it needs the network, which you have) answers whether
C23's `constexpr` exists there. The Windows box, `ssh win`, if it answers; *unrun* if not.

## What to return

`verdict` (adopt, object, veto) per route, `section`, `experiment` (each command and what it
printed), `prediction` (falsifiable, with its instrument), `condition` (what would turn your
verdict, and what would make it a veto).
