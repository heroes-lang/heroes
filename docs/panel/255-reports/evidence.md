# Panel 255's evidence

`evidence.tar.xz` (256,156 bytes) holds **4,510 text files**, every one under 2 MiB, from the folders the
sitting's reports cite, at their paths under `.claude/worktrees/p255-scratch/`:

- `compiler-engineer/p255/` (*CE* in its reports): both rounds' routes (`routes/`, `r2/routes/` with
  `C4`, `C4fail`, `C2679`), probes, shapes (`extra/`, `r2/shapes/`), container drivers and logs (`out/`,
  `r2/out/`);
- `ffi-pragmatist/p255/` (*F*): both rounds' routes (`T`, `A`, `C`, `C1`, `C2`, `r2/C4`, `r2/C4r`), the
  Q1 disassembly and decode probe, shapes (`q5/`, `r2/sh/`), gates and logs (`logs/`, `r2/logs/`);
- `critic/p255/` (*K*): the first reading's probes (`untouched.c`, `fakestack*.c`, shape 7, the musl
  log), the second's (`r2/`: shapes `xr.h`, `xf.h`, the `kC4`/`kC5` probes' edited files under each
  `w-*`, logs) and the third's (`r3/`: the seats' files copied and md5-checked, its own shapes `gr`,
  `cv`, `ax`, `cu`, `ub` in `mysh/`, logs);
- `coord/repro/` and `coord/*.log`: the coordinator's runs of the reproducers before any seat.

**Left out**: every `build/` and `tmp/` folder, every binary and object, every copy of the repository's
tree (`runtime/`, `selfhost/`, `tests/`, and the route runtimes copied whole as `rt/` and `rt-mac/`, whose
edited files are kept beside them), and the reproducers' per-run output folders (`mac-out*`, `lin-out`),
whose lines the kept logs carry. The inputs round two was given are in the tree itself,
`255-briefs/round2/`.

**Checked when written** (2026-11-23): the list built by `find ... -size -2048k` over the extensions the
reports cite (`.c .h .hero .sh .py .log .md .txt .dis .s .S .expected .json .png`), minus the paths
above; `COPYFILE_DISABLE=1 tar --no-mac-metadata --no-xattrs -cJf`; extracted into an empty folder and
every one of the 4,510 files compared by `cmp` against its source, **0 differing**; **0** AppleDouble
(`._`) entries; and a scan of every file for `sk-ant-`, `ghp_`, `gho_`, `github_pat_`, `AKIA`, a PEM
private key header and an assigned `ANTHROPIC_API_KEY=`, **no match**.

**Already gone before the sitting**: lane-core's own folder, `.claude/worktrees/lane-core-scratch/2679/`
(its transcript survives as `.claude/worktrees/rebel-scratch/logs/lane-core-2679.log`, outside this
archive; `00-shared.md` § 2). Every container the sitting started was removed by the seat that started
it. The folders under `p255-scratch/` are the author's to remove.
