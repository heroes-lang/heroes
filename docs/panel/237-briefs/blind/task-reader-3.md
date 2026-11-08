# Task

Write a complete program, one file `main.hero`, that uses a small C library of journals. The whole
library is its header `journal.h`, with no library to link. Its declarations:

```
#define JOURNAL_OK 0
#define JOURNAL_ERR 5

typedef struct journal journal;

int journal_open(const char *name, journal **out);
int journal_write(journal *j, const char *line);
int journal_close(journal *j);
```

What its documentation says: `journal_open` returns `JOURNAL_OK` and hands back, in `*out`, a journal
that must be closed; `journal_write` appends a line and returns `JOURNAL_OK`; `journal_close` flushes
the journal and closes it. **It always closes the journal and gives back its memory, even when the
final flush fails**, and then it returns `JOURNAL_ERR` (5); a journal must never be closed twice. A
journal holds two lines, so a third makes the final flush fail.

The program:

1. opens the journal `log`;
2. writes the lines `one`, `two` and `three` to it, printing each result code;
3. closes the journal and prints the result code the close returned.

The program gives back everything the library hands it, and exits 0.

Answer with the program in one fenced code block, and nothing after it.
