# Brief for the language-ergonomics reader

You judge one thing: whether a language's rules and messages make it more or less likely that a
language model produces a correct program on the first try, and repairs it on the next. Your inputs
are the files in this directory and nothing else:

- `heroes-spec.md`: the complete specification of a small compiled language. It is the whole
  language and the same text a measured model would receive.
- `task-01.md`, `task-02.md`, `task-03.md`: three programming tasks.
- `feedback/`: two short programs that a compiler refused, each with the compiler's message in two
  forms, a full one and a one-line one.

Do not read anything outside this directory. Do not run anything. Write `report.md` here. Answer in
English.

## Part one: the tasks, blind

For each task, write a complete program in the language of the specification, from the
specification alone, in one go, as a model asked once would. Output it in a fenced code block. Then,
for that task, list every **choice point**: every place where the specification left you a choice
or did not say, the choice you made, and what the other choice would have produced: a program the
compiler refuses (name the rule of the specification it would break), or a program that compiles and
does something else (say what). Then say, for each task, whether any sentence of the task's prose
told you which construct of the language to use, quoting the sentence, and whether the visible
example in the task changed what you wrote.

## Part two: repair under two kinds of message

For each of the two programs in `feedback/`, read `program.hero`, then `full.txt` (the full message)
and write the repaired program; then, as if you had seen only `brief.txt` (the one-line message),
say whether you would have made the same repair, and what the full message's extra lines bought.

## Part three: the protocol, from the model's side

The measurement will run tasks like these through a model, grade the program by hidden tests, and
on a refusal hand the compiler's message back for up to five turns. Answer: (a) which of the two
message forms you would want on turn two, and why; (b) whether a task's visible example (input and
expected output) should be re-run by the grader and its result handed back when the program compiles
but is wrong, or whether that is a hidden test leaking; (c) predict, for each task, the share of
first attempts by a strong model that compiles, and the share that passes hidden tests, as two
numbers each; (d) name the three rules of the specification you expect models to break most often,
in order.

## Output structure, exactly

- `verdict`: approve | object | veto (on the measurement protocol as a way to measure the
  language's effect on a model's first try and repair)
- `experiment`: the three programs and the two repairs
- `choice_points`: the choice points, per task
- `argument`: at most 120 words
- `prediction`: the per-task numbers of part three (c), and the three rules of (d)
- `condition`: what measured result would change your verdict
- `context`: whether anything other than the files in this directory reached your context (an
  instruction file, a memory, a project rule, a git status), and what it was; say *none* if nothing
  did
