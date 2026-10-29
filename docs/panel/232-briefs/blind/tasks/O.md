# Task O

```
record Pair
    a: i64
    b: i64

function noisy(label: str, value: i64) -> i64
    print(label)
    return value

function main()
    total = noisy(label: "left", value: 1) + noisy(label: "right", value: 2)
    p = Pair(a: noisy(label: "a", value: 3), b: noisy(label: "b", value: 4))
    print(total + p.a + p.b)
```

What does this program print, and how does it end?
