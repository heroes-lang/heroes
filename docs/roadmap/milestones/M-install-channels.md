# M-install-channels — the way in, from a package manager


**Scheduled, no warrant** (author instruction 2026-09-03, *"publishing on brew, for
instance … deploying the language"*). Installing Heroes today is a `git
clone` and one clang line (`site/src/html/index.html:135-137`, `seed/README.md`),
and that is the whole reason this milestone is small: **every channel builds from
the seed with that line**, 3.5 s, and no channel ships a prebuilt binary —
because `heroes` without clang compiles nothing (`heroes doctor` says so), so a
binary on its own would be a decoy, and a formula that depends on a C toolchain
is the honest shape.

**What it delivers.** A Homebrew formula in a tap (`heroes-lang/homebrew-tap`),
for macOS and Linux; a winget or scoop manifest for Windows, whichever the box
(`docs/platforms/windows/WINDOWS-MACHINE.md`) measures first; a Nix flake; a
Docker image built from the `Dockerfile` beside
`docs/platforms/linux/LINUX-MACHINE.md`. Each is installed and `heroes doctor`
run on its platform before its commit. **And a version scheme**: `heroes
--version` printed `heroes 0.0.1` on 2026-09-03 and every tag is a milestone's
name (CLAUDE.md §14), so nothing a formula can pin exists yet; what a version
number promises is M-publication-gate's compatibility paragraph, and the two are
written together. **The scheme was decided ahead of this row, on 2026-09-07**
(author question; the `DESIGN-LOG.md` row of that date, and the rule's home is
CLAUDE.md §14 § Release tags): `vX.Y.Z` tags as a third namespace, `v0.1.0`
first, `Y` when the spec moved and `Z` when it did not, and `1.0.0` left to the
gate with its compatibility paragraph. So what a formula pins now exists, the
release tag's source archive and its checksum, and what this row still owes is
the channels themselves, each installed and `heroes doctor` run on its platform,
in private.

**Where the line is.** A formula, a manifest, a flake and a Dockerfile are the
channels' own files, outside `heroes` and outside CLAUDE.md §10's *never a
script*: they invoke the one clang line, they do not replace it. Everything here
is prepared and tested **in private** — a local tap, `brew install
--build-from-source` — and the act that puts a channel where a stranger can reach
it is the gate's, which is why this row sits immediately before it (CLAUDE.md
§14: publishing is a hard stop).

**Corrected 2026-10-14.** *This row sits immediately before the gate* has been
false since 2026-09-06, when M-online-compiler entered between them; after the
re-ordering, M-online-compiler, M-lsp-server and M-vscode-extension stand
between. One channel joins the four: an agent's, a plugin for Claude Code
holding a skill with the specification and a hook that runs
`heroes check --brief` after each write. The site already serves
`site/src/llms.txt`.

**2026-10-26.** M-standalone-compiler entered the chain at row 82, immediately
before this row, which is 83 since. Its delivery would make *a binary on its
own would be a decoy* above stop being true, so the shape of the channels is
read again when that row closes.
