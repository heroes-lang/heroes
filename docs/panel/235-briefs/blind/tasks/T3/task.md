# The task

Write a Heroes program that prints the area of a square whose side is 5 and
the perimeter of a regular hexagon whose side is 5, as the expected output
shows. Print nothing else.

The area comes from the program's own module `geom/area.hero`, which is in
this directory, at that path: read it. It stands at the same path beside the
program when the program is compiled.

The perimeter comes from a package from git that its authors call `geom`.
Its address: `https://git.example.org/cara/geom`; its tag: `v3.0.2`; its
fingerprint:
`sha256-40400ef59eede9a63c4ebae1e2a9ca0633467bdd4fc652250d3869641abd8214`.
Its module `polygon` declares `function hexagon_perimeter(side: i64) -> i64`:
the perimeter of a regular hexagon whose side is `side`, six times `side`.

The program uses both the module `geom/area` and the package.

## Expected output

```
area: 25
perimeter: 30
```
