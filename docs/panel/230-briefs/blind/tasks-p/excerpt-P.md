# A program that stopped

The program below was built and run as it stands. You are shown what it should print, what it printed on standard output, what it wrote on standard error, and the status it exited with.

## `excerpt.hero`

It should print:

```
Le | Le petit caf...
Short | Short
Über | Über alles u...
```

It printed nothing on standard output.

It wrote on standard error:

```
panic: string slice splits a character
  at excerpt.hero:15:12, in excerpt.preview
```

It exited with status 134.

The program:

```hero
# Prints each title's first word and a preview of at most thirteen bytes,
# cut only between two characters and followed by "..." when it was cut.

function first_word(title: str) -> str
    i @= 0

    while i < title.len() && title[i] != ' '
        i @ i + 1

    return title.slice(from: 0, to: i)

function preview(title: str, limit: i64) -> str
    if title.len() <= limit
        return title
    return title.slice(from: 0, to: limit) + "..."

function main()
    titles = ["Le petit café du coin", "Short", "Über alles und mehr"]

    for t in titles
        print(first_word(t), " | ", preview(t, limit: 13))
```
