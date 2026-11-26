# Panel 261's evidence: the programs the sitting ran

Every folder of the sitting that holds a `.hero` program is kept whole in `evidence.tar.xz` beside this file,
its path inside the archive the one the briefs and reports cite, so a citation reads the same once unpacked:
`tar -xJf docs/panel/261-reports/evidence.tar.xz` from the repository's root. They are archived rather than
tracked because a program a seat wrote to fail is no program the tree's checks should judge.

- archive: 158348 bytes, sha256 `e5b5b23676dcd1cb601b4adeae7c9b4c711828362017bc98e53bdb1a30a1b463`

| folder | files |
|---|---|
| `docs/panel/261-briefs/blind/reference-t1` | 2 |
| `docs/panel/261-briefs/blind/reference-t2` | 2 |
| `docs/panel/261-briefs/blind/reference-t3` | 2 |
| `docs/panel/261-briefs/ground/probe` | 74 |
| `docs/panel/261-reports/programs/compiler-engineer` | 94 |
| `docs/panel/261-reports/programs/critic/ffi/nftw` | 54 |
| `docs/panel/261-reports/programs/critic/ffi/phdr` | 4 |
| `docs/panel/261-reports/programs/critic/ffi/shapes` | 63 |
| `docs/panel/261-reports/programs/critic/ffi/sqlite` | 18 |
| `docs/panel/261-reports/programs/critic/ffi/threads` | 6 |
| `docs/panel/261-reports/programs/critic/ffi/values` | 11 |
| `docs/panel/261-reports/programs/critic/leg/arm64out/extra/nftw` | 5 |
| `docs/panel/261-reports/programs/critic/leg/arm64out/extra/phdr` | 3 |
| `docs/panel/261-reports/programs/critic/leg/x86out/extra/nftw` | 5 |
| `docs/panel/261-reports/programs/critic/leg/x86out/extra/phdr` | 3 |
| `docs/panel/261-reports/programs/critic/mine` | 41 |
| `docs/panel/261-reports/programs/ffi-pragmatist/census` | 11 |
| `docs/panel/261-reports/programs/ffi-pragmatist/nftw` | 49 |
| `docs/panel/261-reports/programs/ffi-pragmatist/phdr` | 4 |
| `docs/panel/261-reports/programs/ffi-pragmatist/shapes` | 57 |
| `docs/panel/261-reports/programs/ffi-pragmatist/sqlite` | 18 |
| `docs/panel/261-reports/programs/ffi-pragmatist/threads` | 6 |
| `docs/panel/261-reports/programs/ffi-pragmatist/values` | 11 |
| `docs/panel/261-reports/programs/synthesis/ext` | 13 |
| `docs/panel/261-reports/programs/synthesis/mw` | 9 |
| `docs/panel/261-reports/programs/synthesis/probe` | 10 |

## Added 2026-11-26, after the sitting's commit: the precedent probes

The two folders `programs/critic/prec/` and `programs/synthesis/prec/` (the Swift, Nim and Rust programs the critic and the synthesis ran on the three compilers of the author's Mac, with their outputs, 18 files) are kept whole in `evidence-precedent.tar.xz` beside this file, paths kept, so a citation reads the same once unpacked: `tar -xJf docs/panel/261-reports/evidence-precedent.tar.xz` from the repository's root. They were tracked in the tree by the sitting's commit (`e74a3ed7f`) and left it the same afternoon, because the records check `records/citations` told three of them, `fit.rs`, `q2.rs` and `readonly_mut.rs`, a file of Rust outside the Rust bootstrap (CI run 45826172756 of `1e56b5da5`, job *Linux x86-64, the net 5 of 7*). Their text is unchanged.

- archive: 2904 bytes, sha256 `d5f8a3ef53cebae3a16d93c0ed61919d46f532d232a1fe890b8210e68865c4bf`
