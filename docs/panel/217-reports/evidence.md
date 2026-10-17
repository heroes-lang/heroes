# Panel 217: the evidence its seats' working folders held

The reports and briefs in this folder cite their seats' working folders as
`<scratchpad>/217-…`, a session's scratch folder under `/private/tmp`, which git
never saw and which the system empties at a reboot. Kept here on 2026-10-18 by
the author's instruction of 2026-10-12, given for panels 198 to 212 and meant
as: *put the evidence in the panel's result, then throw the folders away*. It
was owed here for a reason of its own: the form this sitting ruled lands on a
row of its own, M-readonly-lend, whose starting point is the compiler-engineer's
prototype and the spec-warden's landing draft, and both stood in that folder
alone.

**What is kept** is `evidence.tar.xz`, beside this file: 3,709 files, every
text file under 2 MiB of the sitting's nine scratch entries (outputs, probes,
census rows, diffs, programs, the readers' work), at its path relative to the
scratch folder, so a path a report cites as `<scratchpad>/<path>` still names
its file: `tar -xJf docs/panel/217-reports/evidence.tar.xz -C <folder>` puts
them back under `<folder>`. Each file was read back byte for byte against its
original on 2026-10-18, and none holds a key or a token (scanned for
Anthropic, GitHub and AWS keys and for private keys). The entries, with the
files each keeps:

- `217-ce-before/`, 2: the compiler-engineer's two readings at the top of its
  copy of the repository;
- `217-ce-probes/`, 289: the compiler-engineer's probes and the prototype as
  three patches, `f1d-prototype.patch`, `f1e-prototype.patch` (sha256
  `1d52a86a02d8…`, named by the report's section of 22:54) and
  `f1f-prototype.patch` (sha256 `835578efa8ce…`), the final one, on which that
  seat approved and which the critic built again and found failing the
  ffi-pragmatist's condition (c);
- `217-critic-work/`, 528;
- `217-ffi-files.txt`, 1, and `217-ffi-work/`, 1,969: the ffi-pragmatist's
  census and its C;
- `217-keycheck/`, 2;
- `217-llm-ergonomist/`, 850: the three arms as the readers left them;
- `217-spec-warden/`, 1, and `217-spec-warden-work/`, 67: the drafts the
  spec-warden priced, `drafts/f1-landing.md` being the landing's text at
  10,246 real.

**What is not kept**: the copy of the repository under `217-ce-before/` and
every `build/` folder, regenerable from git at the commit each report names;
the binaries, the prototype compilers among them, which the patches rebuild;
and any file of 2 MiB or more.

**Cited and already gone on 2026-10-18**, found in no scratch folder of this
machine that day:

- `<scratchpad>/217-compiler-engineer/`, which held the prototype compilers
  `heroes-f1` to `heroes-f1f` and the key program `key-f1.hero` (absent
  2026-10-18);
- `<scratchpad>/217-critic/` (absent 2026-10-18).
