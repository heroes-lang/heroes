# Panel 175 — ffi-pragmatist

Read `00-shared.md` first. Your seat holds design.md §1.11 and §4.19 — there
is no standard library, everything comes from C — and a veto on ABI breakage.
Work in `<scratchpad>/175-ffi-pragmatist/`, built from the seed. The Linux
containers are `heroes-linux` (x86-64) and `heroes-linux-arm64`, run as
`docs/ref/environment/linux/LINUX-MACHINE.md` § How to run something on one of
them says, and Docker was up when this brief was written.

## What to write and compile

1. **The census the question rests on, from real headers.** In the macOS SDK
   and in a Linux container, find C handle types with MORE THAN ONE function
   that ends them, and for each say whether the releasers are
   - **exclusive** — each acquisition has exactly one right releaser, as
     `popen`/`pclose` against `fopen`/`fclose` on `FILE *`; or
   - **interchangeable** — two releasers both correct for one acquisition, as
     `sqlite3_close` and `sqlite3_close_v2` may be for `sqlite3_open`.
   Say how you searched (which headers, which pattern) and what the search
   cannot see. **The interchangeable class decides route A**: a runtime rule
   that the named releaser and no other ends the life refuses a correct program
   there, unless the mark can name more than one.
2. **Route A's C, compiled against real headers.** Whatever the runtime is
   handed to identify a releaser, write the emitted shape by hand for
   `sqlite3_open`/`sqlite3_close_v2` and for `popen`/`fclose`, and compile it at
   the project's flags in both a Darwin and a Linux build. Say whether it needs
   an ABI change (`HERO_RUNTIME_ABI` is 22) and whether a C library's own
   handle functions see anything different.
3. **Route C against the real `stdio.h`.** Two Heroes records over `FILE`
   (`tag __sFILE` on Darwin, `tag _IO_FILE` on glibc) are refused today by
   `duplicate_tag`. Say what a real binding of `fread`, `fgets`, `fwrite` and
   `ferror` would have to become if a pipe and a file were two types.
4. **Question 2's C side.** For `p10.hero` and `b_out.hero`, what does each C
   library say, and when, with no Heroes line at all: libmalloc on Darwin (the
   brief measured signal 5, zero bytes), glibc (its own line, then SIGABRT),
   and the UCRT (`0xC0000374`, zero bytes, measured on the box at step 1). For
   route E, write the one line the handler could print on each platform with
   what it holds, and run it against a C library that calls `abort()` or fails
   an `assert` on purpose with no lease live, so the line is measured true or
   false rather than assumed.

Give a verdict, the cost, one falsifiable prediction with the milestone at which
it is checkable, and the condition under which you would change your mind.
