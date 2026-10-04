# Clock times written without reading the clock, corrected

2026-10-02 at 16:52, by the clock (`date`), the coordinator found that it
had written clock times between 16:31 and 16:49 that it had not read: times
up to 17:45, later than the moment they were written. CLAUDE.md § RUN IT: a
number is measured in the session that writes it (CL-017). The live files
are corrected in place (`docs/work/DEFECTS.md`'s items 157, 158, 159 and 160,
`.claude/rules/verification.md`'s two dated paragraphs); the records, which
are not rewritten, are corrected here, each against a measured time, the
commit's own (`git log --date=format:%H:%M`) or a file's (`stat`):

| where | written | measured |
|---|---|---|
| `6c4da49b`, subject | the author's instruction *at about 16:30* | between 16:23 (the clock read before it) and 16:31 (the commit) |
| `545e0044`, body | the author's answer *of about 17:00* | between 16:31 and 16:39, the commits before and after it |
| `8c02098f`, body | defect 159 *reproduced at 17:00* | before 16:39, the commit |
| `9faf7462`, body | defect 160 *reproduced at 17:25* | before 16:47, the commit |
| `69e65357`, body | 157's widening *reproduced at 17:40* | before 16:49, the commit |
| `bb6f1ece`'s item 158, *reproduced at 15:57* | | before 15:55, the commit; the Windows leg's log reads its exit at 15:52 |
| `docs/panel/186-briefs/00-shared.md`, defect 156 *filed at 11:05* | | `779139d0` at 11:04 |
| `docs/panel/186-reports/coordinator-blind-program-built.md`, *Then measured, 11:13* | | `l_rev.build` at 11:12 |

What the error cost: no verdict, count or hash moved; the lanes were sent
messages carrying two of the false times (17:28 and 17:45), and are told to
read the clock for any time they write. **From now on every clock time the
coordinator writes comes from a `date` run in the same command, or names the
commit or file it was read from.**
