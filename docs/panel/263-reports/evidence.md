# Panel 263's evidence: the programs the sitting ran

Every folder of the sitting that holds a `.hero` program is kept whole in `evidence.tar.xz` beside this file,
its path inside the archive the one the briefs and reports cite, so a citation reads the same once unpacked:
`tar -xJf docs/panel/263-reports/evidence.tar.xz` from the repository's root. They are archived rather than
tracked because a program a seat wrote to fail is no program the tree's checks should judge.

- archive: 49512 bytes, sha256 `527594bb1e55492439d5982edb26c17af83f2a6d695363dbe5bc6789fcb394c7`
- left out of the archive, since they are rebuilt from the sources it keeps: every `build/` folder and 0
  further build outputs and binaries (`.o`, `.a`, Mach-O and ELF files)

| folder | files |
|---|---|
| `docs/panel/263-briefs/reproducer` | 11 |
| `docs/panel/263-reports/programs/compiler-engineer/cost` | 10 |
| `docs/panel/263-reports/programs/compiler-engineer/proto-cc` | 3 |
| `docs/panel/263-reports/programs/compiler-engineer/proto-d` | 2 |
| `docs/panel/263-reports/programs/compiler-engineer/proto-eq` | 1 |
| `docs/panel/263-reports/programs/compiler-engineer/proto-helper` | 2 |
| `docs/panel/263-reports/programs/compiler-engineer/proto-stop` | 2 |
| `docs/panel/263-reports/programs/critic` | 40 |
| `docs/panel/263-reports/programs/ffi-pragmatist` | 23 |
