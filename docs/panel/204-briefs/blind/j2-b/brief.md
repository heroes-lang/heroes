# Your brief

You read programs in a small programming language and say what its tools do
with them. Your input is the files of this directory and nothing else:
`spec.md`, the language's specification (read it in full first), four
programs (`limit_ab.hero`, `limit_ba.hero`, `jpeg_ab.hero`, `jpeg_ba.hero`)
and the two C headers `a.h` and `b.h` that the `limit_` programs name; the
`jpeg_` programs name the system's `stdio.h` and libjpeg's `jpeglib.h`, as
installed on an ordinary machine. Read no file outside this directory and use
no tool but reading and writing files here; you cannot run anything.

**The task**: for each of the four programs, say what `heroes build
<name>.hero -o <name>` does and, if it builds, what `./<name>` prints: the
exit code and everything printed, and why. Be concrete about the values.

Write `report.md` here as you go, with exactly these headings: `limit_ab`,
`limit_ba`, `jpeg_ab`, `jpeg_ba` (each: exit code, output, the sentences of
the specification or the C facts your answer rests on), `choice_points`
(where the specification left you a choice, the choice you made, and what the
other would produce), `confidence`, and `context` (whether anything other
than this directory's files reached your context, and what). English, no em
dashes.
