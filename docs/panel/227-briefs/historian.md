# Panel 227, historian (advisory)

Read `docs/panel/227-briefs/00-shared.md` first. Your question is precedent, verified by web search
and nothing else; an unsourced claim is inadmissible, and each precedent carries its URL, the day
you read it and `verified` or `unverified`.

1. **How language references write their built-ins**: a list of names, call shapes with the
   receiver (Python's `str.split(sep=None)`), full signatures (Go's `builtin` package), or both;
   and any published evidence on which shape a reader, or a language model, copies more correctly.
2. **How compilers tell a call missing its first, unlabelled argument**: Rust (`E0061`, its
   *argument missing* suggestion), Swift (*missing argument for parameter*), Elm, Kotlin or others;
   what the message names and whether it suggests the call's full shape; and **whether a call to a
   library or built-in function is told as a call to the programmer's own function is** (the
   shared brief's route E), or by a message of its own.
3. **Whether a count typed unsigned by a language's own standard library is told with the
   conversion** (Rust's `usize` count in `str::repeat` and its `E0308` *you can convert* help, for
   one).

Write your report to `docs/panel/227-reports/historian.md`: per question, the precedents with URL,
date read and status, and what each states in its own words; no verdict beyond advisory. Write
nothing else; never write outside the repository's root; start no paid run.
