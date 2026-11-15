# filepath: `use <core>/path/filepath`

Paths read and written as text the way the system the program runs on writes them: Go's `path/filepath`.

## Functions

- `clean(path: str) -> str`, called `filepath.clean(path)`: The shortest path naming what `path` names, by text alone: Go's `filepath.Clean`.
- `joined(parts: [str]) -> str`, called `filepath.joined(parts)`: The parts written one after another with the system's separator and cleaned, an empty part passed over, "" where every part is empty: Go's `filepath.Join`, named otherwise since `join` is a built-in's name.
- `dir(path: str) -> str`, called `filepath.dir(path)`: All of `path` but its last part, cleaned, a Windows volume kept: Go's `filepath.Dir`.
- `is_abs(path: str) -> bool`, called `filepath.is_abs(path)`: Whether `path` is absolute: Go's `filepath.IsAbs`.
- `rel(base: str, target: str) -> str?`, called `filepath.rel(base: base, target: target)`: The path that names `target` read from `base`, by text alone, each cleaned first: Go's `filepath.Rel`.
- `abs(path: str) -> str?`, called `filepath.abs(path)`: `path` made absolute, joined to the working folder where it is not and cleaned: Go's `filepath.Abs`.

## Codes

- `not_relative`, `filepath.NOT_RELATIVE`: What `rel` fails with where no path leads from one path to the other.
