# Panel 193, the ffi-pragmatist's brief

Read `00-shared.md` beside this file first; it holds the question, the
measured facts and your directory, `<scratchpad>/193-ffi-pragmatist/`.

## Your task: the consumer's side, in C

The JSON is read by tools outside the compiler, and the language's world is C
(design.md §1.11). Write, in your copy, a consumer in C that reads a
`check --json` answer carrying a fix's place and applies the `certain` fixes to
the file, compiled with clang; run it on the compiler-engineer's route once its
JSON exists, or on JSON you write by hand from the compiler's own spans
(`selfhost/diag.hero:45-50`) until then, and say which.

The answer comes on **stderr**, and today's diagnostic `col` counts
characters (the shared brief, after the critic's pass). Then measure what a
consumer meets:
- **the unit of a column**: bytes, characters, or UTF-16 code units, as an
  editor's protocol (LSP) counts by default; build a line holding characters of
  two, three and four bytes before a fix and show what each unit gives;
- **line ends**: a file with `\r\n` (does the compiler read one, and what does
  its byte offset count?); a file with no final line end;
- **offsets against lines**: which of the two a C consumer applies with fewer
  ways to be wrong, measured by your consumer on the cases you build;
- **Windows**: the box (`ssh win`, clang 23.1.1, Git Bash) is up tonight and
  shared by two lanes; if you use it, work in `/c/w/193-fp-<pid>` with a fresh
  folder, check `df -k /c` shows at least 10 GB free, and never remove a
  folder.

Your verdict says which fields and unit a consumer in C needs, and what a
field the route leaves out would cost it.
