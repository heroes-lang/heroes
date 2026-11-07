## catalogue

```json
{
  "dependencies": {
    "geom": {
      "git": "https://git.example.org/cara/geom",
      "tag": v3.0.2,
      "sum": "sha256-40400ef59eede9a63c4ebae1e2a9ca0633467bdd4fc652250d3869641abd8214"
    }
  }
}
```

## program

```hero
use geom/area
use <geom>/polygon

function main()
    print(f"area: {area.square(5)}")
    print(f"perimeter: {polygon.hexagon_perimeter(5)}")
```

## context

none
