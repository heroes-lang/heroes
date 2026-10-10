# Panel 205, historian's brief

Read `00-shared.md` first. Precedent, sourced and dated: how languages and
build tools that bind C let a program set a feature-test macro before every
header (cgo's `#cgo CFLAGS: -D`, Zig's `@cDefine` and its 0.16 change,
bindgen's `clang_arg`, Nim's `{.passC.}`, Cython, Meson's and CMake's
`add_compile_definitions`), whether their generated code puts its own headers
first, and what broke when it did; and how C-emitting compilers and build
systems treat warnings inside third-party headers (`-isystem`, GCC's and
clang's `system_header` pragma, MSVC's `/external:I` and `/external:W0`
since 2019, CMake's `SYSTEM` include directories, Bazel's), with the
documented failures on each side (a warning hidden that was a real bug; a
build refused by a library's own warning).
