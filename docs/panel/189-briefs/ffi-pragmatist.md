# Panel 189, the ffi-pragmatist's brief

Read `00-shared.md` and `00-facts.md` in this directory first; they bind you.
You judge the founding constraint (your charter,
`.claude/agents/ffi-pragmatist.md`): everything comes from C, and the C the
compiler and its runtime are built from must hold on every platform. Convened
after the completeness critic's first pass, which found the platform runs of a
`runtime/` change assigned to no seat.

## What to measure, in `<scratchpad>/189-ffi-pragmatist/` (`git archive 7d9f2e8f`)

1. **The platforms' own text** (Q6): on the Windows box (`ssh win`, Git Bash,
   clang 23.1.1, your folder `/c/w/189-fp-win/`), what the shell, PowerShell
   (`Out-File`, `>`, `Set-Content`) and Notepad write by default (UTF-8 with
   or without a BOM, UTF-16, Windows-1252), and what the trunk's compiler,
   built there from the seed with seed/README.md's stack flag, does with each;
   in the Linux arm64 container (one at a time, `docker ps -q` empty first, on
   the container's own filesystem and not the `/w` bind mount, which is this
   Mac's through FUSE), a file and a directory whose names are not UTF-8,
   through the trunk's compiler.
2. **F8's two readers on both platforms**: a runtime directory whose
   `runtime.c` holds a Latin-1 byte (`cli/toolchain.hero:85`), and a C header
   holding one (`cli/deps.hero:100`, its digest `absent`): what each does, and
   what clang itself does with a Latin-1 byte in a header and in a source
   (`-Winvalid-source-encoding`, its default and its text).
3. **The runtime's C, once the compiler-engineer has a diff** (the coordinator
   sends you its path): compile it on Apple clang, Debian clang 22.1.8 and 18
   in the container, and the box's clang 23.1.1, under the compiler's own flag
   list, and run its runtime tests where they exist; whether it belongs in the
   runtime (its ABI number) or in the compiler's own extern groups (F3), from
   the C side.

Report per item: what you ran, per platform; your verdict on Q4 and Q6 from
the C side; a falsifiable prediction; the condition that would change it.
