# A program that stopped

The program below was built and run as it stands. You are shown what it should print, what it printed on standard output, what it wrote on standard error, and the status it exited with.

## `rects.hero`

It should print:

```
all 2 rectangles normalised
```

It printed nothing on standard output.

It wrote on standard error:

```
assert failed: normalized(drawn[i]) == expected[i]
  at rects.hero:35:16, in rects.main
  left:  Rect(x0: 2, y0: 1, x1: 5, y1: 1)
  right: Rect(x0: 2, y0: 1, x1: 5, y1: 4)
```

It exited with status 134.

The program:

```hero
# Normalises rectangles so that (x0, y0) is the lower-left corner and (x1, y1)
# the upper-right, and checks each result against the corners the drawing
# expects before reporting.

record Rect
    x0: i64
    y0: i64
    x1: i64
    y1: i64

function lower(a: i64, b: i64) -> i64
    if a < b
        return a
    return b

function upper(a: i64, b: i64) -> i64
    if a > b
        return a
    return b

function normalized(r: Rect) -> Rect
    return Rect(
        x0: lower(a: r.x0, b: r.x1),
        y0: lower(a: r.y0, b: r.y1),
        x1: upper(a: r.x0, b: r.x1),
        y1: lower(a: r.y0, b: r.y1)
    )

function main()
    drawn = [Rect(x0: 5, y0: 4, x1: 2, y1: 1), Rect(x0: 0, y0: 0, x1: 3, y1: 3)]
    expected = [Rect(x0: 2, y0: 1, x1: 5, y1: 4), Rect(x0: 0, y0: 0, x1: 3, y1: 3)]
    i @= 0

    while i < drawn.len()
        assert normalized(drawn[i]) == expected[i]
        i @ i + 1

    print("all ", drawn.len(), " rectangles normalised")
```
