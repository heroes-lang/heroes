# Panel 202, ffi-pragmatist's brief

Read `00-shared.md` first. Your seat writes and runs the C: which real C
libraries put two definitions of one name in headers a program could bind
from two groups (a `static inline` helper in two libraries' headers, a macro
and a function of one name, two versions of one header), with a census of
the headers on this Mac and in Docker `heroes-linux-arm64:latest`; what clang
says in one unit and what the linker says when each module compiles alone
(do two `static inline` definitions of one name ever clash at link?); and
the include graph 550 needs: can clang's own output (`-H`, `-MD`, the note's
`In file included from` chain) give it without a second run?
