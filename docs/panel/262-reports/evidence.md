# Panel 262's evidence: the programs the sitting ran

Every folder of the sitting that holds a `.hero` program is kept whole in `evidence.tar.xz` beside this file,
its path inside the archive the one the briefs and reports cite, so a citation reads the same once unpacked:
`tar -xJf docs/panel/262-reports/evidence.tar.xz` from the repository's root. They are archived rather than
tracked because a program a seat wrote to fail is no program the tree's checks should judge.

- archive: 179368 bytes, sha256 `b0964aee9f001f47a4fb22bbe1926b8c84e49e0f256f0819a1ff6c319725757f`
- left out of the archive, since they are rebuilt from the sources it keeps: every `build/` folder and 70
  further build outputs and binaries (`.o`, `.a`, Mach-O and ELF files)

| folder | files |
|---|---|
| `docs/panel/262-briefs/blind/programs` | 36 |
| `docs/panel/262-briefs/ground/probe` | 60 |
| `docs/panel/262-reports/programs/compiler-engineer` | 26 |
| `docs/panel/262-reports/programs/critic` | 13 |
| `docs/panel/262-reports/programs/ffi-pragmatist` | 1539 |
| `docs/panel/262-reports/programs/spec-warden/instr` | 8 |
