# Your brief

You judge one thing: whether a compiler's behaviour makes it more or less likely
that a language model produces a correct program in one turn. Your input is the
files of this directory and nothing else: `spec.md`, the specification of a
small programming language (read it in full first), `task1.hero`, a program in
that language, and two outputs, `task1-output-P.txt` and `task1-output-Q.txt`.
Each output is what a compiler for this language prints on `task1.hero`; both
were run, and nothing tells you which is the compiler of today. Read no file
outside this directory and use no tool but reading and writing files here.

The program holds mistakes. The question is how a compiler should read a
program past a mistake of a certain kind, and where it should report it; which
kind is for you to find.

Method: write the code, do not opine.

1. Predict, before you open either output. List every mistake you see in
   `task1.hero`, and for each one where you expect the compiler to report it
   (line and column), with the words of the specification you relied on. Say
   what, if anything, the specification tells you about the kind of mistake you
   judge the first to be.
2. Fix from each output. Take each output in turn and write the program you
   would submit after reading that output alone (you may read the program; you
   may not use the other output or your prediction). Count, for each: the
   mistakes your fixed program still has, and the turns you expect it to take
   to reach a program that compiles.
3. Compare. Which output did you fix in fewer turns, and which message, if any,
   sent you to the wrong place or to the wrong fix?

Write `report.md` in this directory as you go, with exactly these headings:
`verdict` (approve the behaviour of P, approve that of Q, or neither), 
`experiment` (the code you wrote), `hesitation_points` (where you guessed, and
what a wrong guess produces: an error, or a program that compiles and does
something else), `argument` (at most 120 words), `prediction` (a falsifiable
rate, one-turn repairs or silent wrong edits, under P and under Q), `condition`
(what result would change your verdict), and `context` (whether anything other
than this directory's files reached your context, and what). English, no em
dashes.
