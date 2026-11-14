# slices: `use <core>/slices`

Arrays put in an order the caller writes, where the built-in `sort` puts numbers, text and booleans in the order of their values alone.

## Functions

- `sort_by<A>(xs: [A], less: (function(a: A, b: A) -> bool)) -> [A]`, called `slices.sort_by(xs, less)`: The elements of `xs` in the order `less` writes, stably: `less(a: x, b: y)` answers whether `x` goes before `y`, and two elements neither goes before keep the order they had in `xs`.
