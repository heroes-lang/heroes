# Your brief

You judge one thing: whether a compiler's message lets a language model
repair a program in one turn. Your input is the files of this directory and
nothing else: `spec.md`, the specification of a small programming language
(read it in full first), and one short program below, with what it should do
and what the compiler printed for it. Read no file outside this directory
and use no tool but reading and writing files here.

Method: write the code, do not opine. In ONE turn, as you would submit it,
write the corrected program, so that it compiles under the specification and
does what it should, to `c.hero` in this directory. You cannot run the
compiler; you have what it printed for the program as written, and the
specification. Do not ask for a second message: one turn is the experiment.

Then write `report.md` with exactly these headings: `experiment` (the
corrected program, as in `c.hero`), `cause` (in one sentence, what you
believe was wrong with `p.hero`), `reading` (which part of what the compiler
printed led you to which edit), `confidence` (whether you expect `c.hero` to
compile and do what it should at once, and why), `argument` (at most 120
words: did what the compiler printed make a one-turn repair likely, and where
did it not), `choice_points` (every place where the specification left you a
choice, the choice you made, and what the other would produce), `prediction`
(a falsifiable rate, over 100 models given this directory, of one-turn
repairs of this program), and `context` (whether anything other than this
directory's files reached your context, and what). English, no em dashes.

## The program

What it should do: print the word `café`.

`p.hero`, as its bytes in hexadecimal (this page is UTF-8 text, so the file
cannot be shown as it is):

```
66 75 6e 63 74 69 6f 6e 20 6d 61 69 6e 28 29 0a
20 20 20 20 70 72 69 6e 74 28 22 63 61 66 e9 22
29 0a
```

Read as text where the bytes allow, it is:

```
function main()
    print("caf\xE9")
```

where `\xE9` stands for the single byte 0xE9, written here as those four
characters only because this page cannot hold the byte itself.

What the compiler printed for it:

`heroes check p.hero` exited 2 and printed:

```
error: cannot read `p.hero`
```

`heroes build p.hero -o p` exited 2 and printed the same line.
