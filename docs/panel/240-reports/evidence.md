# Panel 240's evidence

`evidence.tar.xz`, packed 2026-11-09 at the synthesis and read back byte for
byte (`diff -r` against the folder it was made from, silent), scanned for keys
before packing (none found). 133 files:

- `evidence/compiler-engineer/compiler-engineer.diff`, the prototype's diff at
  the seat's last save (215,140 bytes), against the base `429464c94`;
- `evidence/compiler-engineer/build/p240/`, every text file under 2 MiB of the
  seat's working folder: its measuring scripts, the suites' outputs it cites
  (`proto0-*.txt` and the rest), the tried programs of § 6;
- `evidence/spec-warden/drafts/`, the 23 drafts of § 13 the seat priced, T1 as
  `pc2-t1.md`, and the base it diffed them against;
- `evidence/synthesis.before-critic.md`, this sitting's synthesis before the
  critic's second pass corrected it.

Left out: the seats' worktrees themselves (`.claude/worktrees/240-*`), every
binary and every `build/` cache. The briefs' drafts before the critic's first
pass are in `240-briefs/` as `*.before-critic.md`. The folders are the
author's to remove.
