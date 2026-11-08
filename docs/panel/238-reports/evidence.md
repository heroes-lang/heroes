# Panel 238, the evidence kept

`evidence.tar.xz` beside this file, written 2026-11-08 at 22:28 by the coordinator: 3,932 files,
7,599,648 bytes before compression, 544,572 after, read back whole (`tar -xJf` into a scratch
folder and every file compared by `cmp` with its source: 0 differing; `tar -tJf` lists 3,932 entries,
no AppleDouble entry). It holds every file under 2 MiB whose type `file -b --mime-type` reads as
text, empty or JSON, of the folders the sitting's reports cite:

- `238-compiler-engineer/build/238/` (2,897 files): the census rows (`census/rows.txt`,
  `rule-moves.txt`), the emission sweep's rows (`esweep/rows.txt`, `list.txt`, `walk-moves.txt`), the
  permutation files (`perm/`), the shapes (`shapes/`), the old cases and the renamed 560 case
  (`oldcases/`), the index move (`idx/`, `move-index.sh`, `idxmove.diff`, `idxmove.stat`, the map's
  output before and after), the suites' outputs (`suites/`), the build logs, and the diffs:
  `prototype.diff` (the rule and the route, as recommended) and `heading-refinement.diff` (built and
  not taken);
- `238-ffi-pragmatist/build/238/` (684 files): the binding C and its scripts (`link/src/`, `c-*.sh`,
  `hero-*.sh`), the Heroes programs and their link words and outputs on both platforms
  (`link/hero/`, `link/hero-pkg/`, `link/hero-sqlite/`, `link/out-darwin/`, `link/out-linux/`),
  `units/two_others/`, the census rows (`census-run/`, `census-ex/`, `roots.txt`, `reads.txt`,
  `multi-extern.txt`), the Linux leg's build script and log, and, of its variant compilers, **only
  the modules that differ from the tree at `27829fbba`** (18 files under `walk/selfhost/` and
  `group/{base,walk}/selfhost/`), with `group/group.diff`, the group words as built;
- `238-critic/build/238/` (204 files): both passes' programs (`probe1/`, `cside/`, `c453/`, `c560/`,
  `spell/`, `second/`: the link, nesting, `two_others`, outputs, characters and r6241 shapes),
  `census.txt`;
- `238-spec-warden/build/238/` (86 files): the drafts it priced (`drafts/`, `make.py`, `make2.py`,
  `measured.txt`, `measured2.txt`), the eight comment shapes (`docs/`), `baserate.txt`, `onemod/`;
- `p238-scratch/probe1/` (61 files): the coordinator's probes of the briefs' F1, F3 and F4.

**Left out**: every binary (compilers, objects, archives, built programs, ELF and Mach-O alike, by
their type), every `build/` cache inside a program's folder, every placed `heroes.packages/` tree of
a copy; **the copies of the repository**: the formatter's output for every tracked file
(`238-compiler-engineer/build/238/census/out/`, `hcensus/out/`, `hcensus2/out/`), the reformatted
tree of the heading refinement (`headtree/`), the emissions of every blessed program
(`esweep/out/`, 255 MB of C), the copies of `tests/golden/run` and `examples` the ffi-pragmatist
formatted and built (`census-run/{sorted,written}/`, `census-ex/{sorted,written}/`), its copy of the
repository sent to the container (`linux/tree/`), the critic's copy of the route's sources
(`second/walk/`), and the unchanged modules of every variant compiler. Each is reproducible from the
tree at `27829fbba` and the diffs above. Scanned before writing: no string shaped like an Anthropic
or GitHub key, no `ANTHROPIC_API_KEY=` with a value, no address at the author's domain. The seats'
worktrees (`.claude/worktrees/238-<seat>/`) hold no committed change. The folders are the author's to
remove once the worktrees are.
