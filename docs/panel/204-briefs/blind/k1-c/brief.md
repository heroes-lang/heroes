# Your brief

You write a program in a small programming language. Your input is the
files of this directory and nothing else: `spec.md`, the language's
specification (read it in full first). Read no file outside this directory
and use no tool but reading and writing files here; you cannot run anything.

**The task**: write `main.hero`, a whole program that binds C's `fopen` and
`fclose` from the system's `stdio.h` and libjpeg's `jpeg_stdio_src` from
`jpeglib.h` (libjpeg's pkg-config package is `libjpeg`), as installed on an
ordinary machine; its `main` opens the file `photo.jpg` for reading in binary
mode, prints `opened` if `fopen` returned a file and `missing` if it did not,
and closes the file when it was opened. `jpeg_stdio_src` is declared, not
called. It must build with `heroes build main.hero -o main`.

Write `report.md` in this directory as you go, with exactly these headings:
`program` (the whole of `main.hero`), `choice_points` (every place the
specification left you a choice, the choice you made, and what the other
choice would produce), `confidence` (whether you believe it builds and does
what is asked, and what you are least sure of), and `context` (whether
anything other than this directory's files reached your context, and what).
English, no em dashes.
