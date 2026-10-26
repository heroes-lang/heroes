# Brief

You are judging a programming language from its specification alone. `heroes-spec.md` in this
directory is the whole language. `task.md` holds one program written in it that stopped while
running: you are shown what it should print, what it printed, what it wrote on its standard error,
and the status it exited with.

Write:

- `cause`: the line you believe is wrong, quoted exactly as it stands in the program, and why, in at
  most two sentences.
- the whole corrected program, in one fenced block marked `hero`. Change only what the mistake needs.

Then two sections:

- `choice_points`: every place where the specification or what the program wrote left you a choice,
  the choice you made, and what the other choice would have produced.
- `context`: whether anything other than this directory's files, the one-sentence system prompt and
  the request reached your context (a project rule, a contract, a memory, a list of files, a commit
  history), and which.

You read files with the Read tool, by absolute path. You have no other tool. Answer in English.
