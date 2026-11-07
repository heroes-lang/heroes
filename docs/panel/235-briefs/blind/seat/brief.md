# Your brief

You judge one thing: whether a proposed part of a programming language makes
it more or less likely that a language model writes a correct program on the
first try. Your verdict is an experiment, not an opinion: write the code.

## Your inputs, and only these

All of them are files in this directory, and you read nothing else:

- `heroes-spec.md`, the language's specification as it stands, the whole
  language. It has no section on packages.
- `variant-1.md`, `variant-2.md` and `variant-3.md`: three competing versions
  of one new section, `## 14. Packages`, which would be appended to the
  specification. They state the same facts and differ in how a `use` line
  reaches a package's module. Their numbering says nothing about them.
- `task-1.md` and `task-2.md`, two programming tasks; `task-2.md` names a
  module the program's own folder holds, `geom/area.hero`, which is in this
  directory at that path.

## What you do

1. For each task and each variant, write what the task asks: the catalogue
   the variant describes and the program, as you would hand them in, reading
   the specification plus that one variant. Six answers.
2. Record every place where the specification and the variant left you a
   choice, the choice you made, and what the other choice would produce: a
   message from the compiler or the catalogue's reader, or a program that
   builds and does something else.
3. Judge **locality**. A construct is local, in this judgement, when its
   meaning can be determined from its line plus the enclosing signature (the
   specification says *inference is local only*, section 5; the rest of this
   definition is your brief's). Under each variant, say
   what a reader of one `use` line can tell from the line alone, what needs
   another file, and which file; and whether that is the same as, or
   different from, what a `use` of the program's own module needs today.
   You hold a **veto** on a construct that is not local; use it only where
   the meaning of a line cannot be determined without a file the line does
   not name or mark.

## Your report

Write `report.md` in this directory, in English, with exactly these
sections:

- `verdict`: for each variant, approve, object or veto, and one line why.
- `experiment`: the six answers, each the catalogue and the program.
- `choice_points`: every place the text left you a choice, the choice you
  made, and what the other would produce, a compile-time message or a
  silently different program.
- `locality`: the judgement of step 3, per variant.
- `argument`: at most 150 words.
- `prediction`: for each variant, how many of 30 first tries you expect two
  smaller, faster language models, fifteen each, to get right on three tasks
  like these, and what their most frequent mistake will be; this is checked
  against a measured run.
- `condition`: the measured result that would change each verdict.
- `context`: whether anything other than the files of this directory reached
  you: a project's rules, a contract, a memory, another document; name it if
  so. A yes voids this reading.
