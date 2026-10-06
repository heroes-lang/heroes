# M-buildable-structs — the structs a program can build

**Scheduled 2026-09-18** out of panel 164, which recorded *a real five-field
`utsname` needs 1280 literal zeros*. **This file opened 2026-09-25**, with
panel 178, while the milestone's row stays `scheduled`: one `**OPEN**` row at a
time is what `site/src/lib/chain.ts` accepts, and M-agreed-retention holds it.

**What the milestones before it left standing.** Panel 163 found there was no
wall for a struct C fills: a function that returns the record, and `partial` to
declare only what is read. What it did not do is make a struct with long arrays
writable in the program's own construction, and it left `[0; 256]` unpriced.

**Why it is a milestone and not a note.** The repository was the wrong sample:
its longest fixed array is 8, so nothing here ever hit the wall. The headers
M-core-packages will bind are the right one, and panel 178's census counts 42
public structs on Darwin, 29 on Linux arm64 and 23 on Linux x86-64 with an
array longer than 8, in three kinds: bytes the program never touches, buffers C
fills, and bytes the program writes. The same struct differs by platform
(`utsname` is five of `[256]` on Darwin and six of `[65]` on Linux), and an
all-zero value is not valid for every type on every platform (a zeroed
`pthread_mutex_t` locks on Linux and is `EINVAL` on Darwin). **Why here, before
M-core-packages**: the argument M-readable-bytes made for its own position, a
binding written before the wall moves is a binding rewritten after it.

**What panel 178 decided, provisionally**: `rest: zero` on a group record's
construction, zero admitted as bytes and never as a claim, the header's own
initialiser as the way C says which value is valid, C's `char` spelled once,
and defects 091 to 094 filed. What the landing owes and what the sitting left
open are the open `feature` and `task` issues naming M-buildable-structs.

**This page moved 2026-10-06** from `docs/work/milestones/`, where panel 178's
branch wrote it on 2026-09-25, when that branch was merged: the page keeps its
reasoning, and its six items became issue files of `issues/2026-09/25/` on
2026-10-06 (`.claude/rules/records.md` § A live list), each as the page held it.
