# A program that stopped

The program below was built and run as it stands. You are shown what it should print, what it printed on standard output, what it wrote on standard error, and the status it exited with.

## `report.hero`

It should print:

```
first 4
median 16
window 27
window 39
window 54
window 81
```

It printed on standard output:

```
first 4
median 16
```

It wrote on standard error:

```
panic: array index out of range
  at report.hero:13:44, in report.windows
```

It exited with status 134.

The program:

```hero
# Readings arrive one per hour. The report prints the first reading, the
# median, and the sum of every window of three consecutive readings.

function median(xs: [i64]) -> i64
    sorted = sort(xs)
    return sorted[sorted.len() / 2]

function windows(xs: [i64]) -> [i64]
    out: [i64] @= []
    i @= 0

    while i + 2 <= xs.len()
        out @ out.push(xs[i] + xs[i + 1] + xs[i + 2])
        i @ i + 1

    return out

function main()
    readings = [4, 8, 15, 16, 23, 42]
    print("first ", readings[0])
    print("median ", median(readings))

    for w in windows(readings)
        print("window ", w)
```
