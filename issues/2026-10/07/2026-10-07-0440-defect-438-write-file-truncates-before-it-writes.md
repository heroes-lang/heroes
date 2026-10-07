---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: 208ea207a56c4af48c13afa606dd659121c08096
github: none
---

- [ ] **438 — `write_file` truncates before it writes** | `write_file` opens its file truncated and then writes, so a concurrent reader can see it empty and a kill between the two leaves it empty, its old contents lost; lane b13-tmpl407's case had to change its witness for it | `runtime/`, `write_file` · **class: improvement**

    **Origin:** filed by the coordinator at 04:40 on 2026-10-07, from lane b13-tmpl407's report (*found beside*); the lane's reading, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): the common behaviour of a write, no program judged wrong; a write through a private name and a rename would keep the old contents until the new are whole.

    Repaired at `208ea207`, 2026-10-07 (lane b14-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `write_file` writes its text under a private name beside the file, flushes it and renames it over the file (`runtime/parts/write.c`, over `parts/replace.c`'s stage), and writes in place as before where a rename cannot make the new file all the old one was (not a regular file, a second name, not writable, an owner, mode, flags, ACL or attributes not carried, no new name in its directory, a refused rename, and on Windows any file already there); measured at `dad2da47` a child under a 4096-byte ceiling left 4096 bytes of a 1 MB text over "OLD" and a 64 MB write killed at the file's first change left 0 bytes in 2 runs of 6, after the repair "OLD" and the whole new text; a write costs 3.16M, 3.37M and 4.18M instructions at 1 KB, 64 KB and 1 MB where it cost 0.42M, 0.55M and 1.36M (200 writes, this Mac), the flushes' wait not counted; cases `run/fixedbugs-438-*`, three, the cut-short one red on the base.
