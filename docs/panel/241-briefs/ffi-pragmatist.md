# Panel 241, the ffi-pragmatist's brief

Read `00-shared.md` beside this file first (rewritten on the critic's first pass). Your worktree:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/241-ffi-pragmatist` (detached at
87d77dec2); build your compiler there from the seed and nowhere else. Your report:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p241/docs/panel/241-reports/ffi-pragmatist.md`.

## Question 2 is yours, compiled

Write, compile and run each route as the package `crypto/sha256` would bind it, with `Sum256` taking
the file's bytes read through `stdio.h` in the package (fact 1 of `00-shared.md`; reproduce the
critic's probe first), and compare every digest with `shasum -a 256` on three files, one not text,
one empty, one larger than a single read:

- **(a) `pkgconfig "libcrypto"`**: on this Mac (where does `pkg-config libcrypto` resolve, `brew
  --prefix openssl`?), in the Linux arm64 container (`docker run` of `heroes-linux-arm64:latest`,
  `docs/platforms/linux/LINUX-MACHINE.md`), and on the Windows box if you can (`ssh win`, the author
  left it on; `docs/platforms/windows/WINDOWS-MACHINE.md`; a folder of your own under `C:/w/`, no
  other folder touched): the milestone page says *"Mac and Linux"*, so say what Windows does.
- **(b) the package's own C header**: a `sha256.h` beside the package's modules, as `fixedbugs-2351`
  shows a package header works; its lines, and what design.md:571-575 (*"nothing a program could
  carry instead"*, about platform C) says or does not say about it.
- **(c) the runtime's `sha256.c`**: the third door that hashes a `[u8]` (`hero_sha256_bytes` is
  `static`, `runtime/parts/sha256.c:83`), declared where a program may bind it; what moving it, or
  `hero_file_sha256`, `hero_text_sha256` and `hero_env_put`, into `hero_os.h` costs: lines,
  `HERO_RUNTIME_ABI`, `heroes_guard_open.h` and `selfhost/emit/guarded_names.hero`, and the promise
  decision 2228 makes for every `hero_os.h` function (design.md:3641).
- **(d) Heroes alone**: time it at `-O0` and `-O2` on the same 4 MB, against the C routes.

And for question 3: the environment's write the transport needs (`setenv`, `_putenv_s` on Windows)
bound by an `os` package's own header, against `hero_env_put`.

## Your verdict

Per route: approve, object or veto, with the C's lines and what you compiled and ran on which
platform. Veto on ABI breakage or unsoundness at the boundary only, naming the program that would
break. If the route you approve needs the per-capability archive (feature 2295, panel 234's R6), say
so: it owes the spec sentence and so the full panel.
