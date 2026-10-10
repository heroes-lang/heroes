# Panel 205, ffi-pragmatist's brief

Read `00-shared.md` first. Your seat writes and runs the C: which real
libraries need a macro before every header (`_GNU_SOURCE`, `_DEFAULT_SOURCE`,
`_FILE_OFFSET_BITS=64`, `_POSIX_C_SOURCE`, `WIN32_LEAN_AND_MEAN`,
`NOMINMAX`) and what a binding of them must see; which installed package
headers fail under this compiler's warning flags in their own code, on this
Mac and in Docker `heroes-linux-arm64:latest` (a census of the packages
`pkg-config --list-all` names, each entry header compiled as a group's unit
compiles it, with the flags and without them, with `-I` and with
`-isystem`); and for each route of `00-shared.md`, which real binding it
lets through, which it still refuses, and whether a wrong binding's type
check survives it (a `-isystem` header still refusing a wrong parameter
type, measured).
