# Task W

```
function noisy(label: str, value: i64) -> i64
    print(label)
    return value

function main()
    ys: [i64] @= [0, 0]
    ys[noisy(label: "index", value: 5)] @ noisy(label: "value", value: 1)
    print(ys[0])
```

What does this program print, and how does it end?
