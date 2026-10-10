# Your brief

You write a program in a small programming language. Your input is the
files of this directory and nothing else: `spec.md`, the language's
specification (read it in full first). Read no file outside this directory
and use no tool but reading and writing files here; you cannot run anything.

**The task**: write `main.hero`, a whole program that declares a constant
`ALL_ONES` of type `u64` whose 64 bits are all set, and a constant
`LOW_BYTE` of type `u8` whose 8 bits are all set, and prints both. It must
build with `heroes build main.hero -o main` and print the two values.

Write `report.md` in this directory as you go, with exactly these headings:
`program` (the whole of `main.hero`), `choice_points` (every place the
specification left you a choice, the choice you made, and what the other
choice would produce), `confidence` (whether you believe it builds and prints
what is asked, and what you are least sure of), and `context` (whether
anything other than this directory's files reached your context, and what).
English, no em dashes.
