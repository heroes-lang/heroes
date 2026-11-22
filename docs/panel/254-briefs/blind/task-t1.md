# Task

Write `main.hero`, a whole program. `main` holds four documents, each a record of a `name` and a
`text`:

- `a`: `the cat sat on the mat`
- `b`: `a dog and a cat`
- `c`: `to be or not to be`
- `d`: `x`

A document's name and text may hold any character, a newline and a `|` among them; these four are the
ones this run holds, and the program must stay right for any others.

Each document is counted on a thread of its own, and all four threads are started before any is
waited for. A thread is handed its document and the list of words to leave out, `the`, `a` and `to`,
and gives back the document's name, how many of its words are not left out, and how many distinct
words those are. Words are separated by single spaces. Once every thread has answered, `main` prints
one line per document, in the order the documents are given:

    <name>: <words> words, <distinct> distinct

and the program ends normally, not by an abort.

Give `main.hero` whole, then say whether `heroes build main.hero` accepts it, every line the program
writes, and how it ends.
