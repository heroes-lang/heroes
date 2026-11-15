# os: `use <core>/os`

The process's surroundings read as values: the folder it works in, its environment, and what stands at a path, each a failure the caller answers where the system cannot say, and never an abort.

## Types

`record FileInfo` with `kind: Kind` and `mode: i64`: What `stat` and `lstat` answer about a path: its kind, and its permission bits, Go's `Mode().Perm()`, 0 for a link, whose own bits the runtime does not read, and on Windows 0444 for a read-only file and 0666 for any other.

`variant Kind`, one of `.regular`, `.directory`, `.link` or `.other`: What a path holds: Go's file modes, the kinds the runtime can tell apart.

## Functions

- `getwd() -> str?`, called `os.getwd()`: The absolute path of the folder the process works in, every link on the way followed: Go's `os.Getwd`.
- `env(key: str) -> str?`, called `os.env(key)`: The value of the environment variable `key`.
- `stat(name: str) -> FileInfo?`, called `os.stat(name)`: What `name` names, a link followed to what it names: Go's `os.Stat`.
- `lstat(name: str) -> FileInfo?`, called `os.lstat(name)`: What stands at `name`, the link itself where it is one: Go's `os.Lstat`.

## Codes

- `file_not_found`: where the folder is gone (`getwd`); where nothing stands there (`stat`); where nothing stands there (`lstat`).
- `not_found`: where the variable is unset (`env`).
- `not_text`: where its path is not UTF-8 (`getwd`); where its value is not UTF-8 (`env`).
- `read_failed`: where the system could not be asked (`stat`); where the system could not be asked (`lstat`).
