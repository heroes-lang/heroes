# Panel 214, the ffi-pragmatist's brief

Read `00-shared.md` first. Your question is the C each route implies, compiled.

1. **Route D, isolation, compiled**: write the shim a group's header would need so that the unit
   holding the runtime and the emitted program never includes it: a header's functions,
   `static inline` functions, macros used as constants and as functions, records passed by value,
   `extern constant`s, callbacks, variadics. Which of today's bindings (`examples/` programs with an
   `extern`, the FFI goldens) survive it unchanged, which need a new mechanism, and what it costs at
   run time (a call through a wrapper). If a binding cannot be isolated, show it.
2. **Route F, C's reserved space**: what C11 7.1.3 reserves (read it, cite it), whether renaming the
   runtime's `hero_*`/`Hero*`/`HERO_*` and the emitter's `h_*` into reserved identifiers changes the
   ABI (`HERO_RUNTIME_ABI` in `runtime/heroes_runtime.h`), the FFI headers that already use such
   names (`<stdint.h>`'s `__INT64_TYPE__`, the guards), and whether clang warns on a program's own
   reserved identifiers (`-Wreserved-identifier`).
3. **Routes G, H and I, compiled**: G, clang's structured diagnostics (`--serialize-diagnostics`,
   `-fdiagnostics-format=sarif`) on this Mac's clang, Linux's 22.1.8 and Windows' 23.1.1: do they
   carry the include stack and link each note to its error? H, the runtime read after the groups'
   headers: does every emission still compile (the guards, `hero_os.h` named as a group's header by
   ten examples)? I, the unit compiled without the group's header and a prototype in its place:
   where does the prototype come from, and what does it cost per build?
4. **Every route on the three platforms**: run the six reproducers and defect 2053's two cases on
   this Mac, in `heroes-linux-arm64:latest` (Docker) and on the Windows box (`ssh win`, tree copied
   by `COPYFILE_DISABLE=1 tar --no-mac-metadata --no-xattrs` of `git ls-files`, streamed with
   `ssh win 'cat > /c/w/x.tgz'`, built with `clang -I runtime seed/heroes.c runtime/runtime.c
   -Wl,/STACK:67108864 -o heroes.exe`; keep the ssh call in the foreground, Windows sshd kills a
   closed session's processes; clean `/c/w` of your files at the end). Where clang's words differ by
   platform, say whether the route still decides alike.

Veto on ABI breakage, and on a route that cannot bind a header today's compiler binds.
