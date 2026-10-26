# A program that stopped

The program below was built and run as it stands. You are shown what it should print, what it printed on standard output, what it wrote on standard error, and the status it exited with.

## `histogram.hero`

It should print:

```
0-9: 1
10-19: 2
20-29: 1
30-39: 1
40-49: 1
50-59: 1
```

It printed nothing on standard output.

It wrote on standard error:

```
panic: array index out of range
```

It exited with status 134.

The program:

```hero
# Counts how many scores fall in each ten-point bin, from 0 up to the highest
# score, and prints one line per bin.

function bin_count(top: i64, width: i64) -> i64
    return top / width

function tally(scores: [i64], width: i64) -> [i64]
    top @= 0

    for s in scores
        if s > top
            top @ s

    out: [i64] @= []
    i @= 0

    while i < bin_count(top: top, width: width)
        out @ out.push(0)
        i @ i + 1

    for s in scores
        b = s / width
        out[b] @ out[b] + 1

    return out

function main()
    scores = [3, 17, 25, 41, 50, 12, 38]
    counts = tally(scores, width: 10)
    i @= 0

    while i < counts.len()
        print(i * 10, "-", i * 10 + 9, ": ", counts[i])
        i @ i + 1
```
