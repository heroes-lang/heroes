# hero_os: `use <core>/os/hero_os`

The language's own edges of the process, `hero_os.h`, bound thin: one function per C function `os` reads, each answering what C answers, in the header's order.

## From `hero_os.h`

- `hero_os.HERO_OS_OK`, `hero_os.HERO_OS_NOT_FOUND`, `hero_os.HERO_OS_FAILED` and `hero_os.HERO_OS_NOT_TEXT`, each `i64`: What each call answers in its status: the same small numbers on every platform, never the system's own.
- `hero_os.HERO_FS_UNKNOWN`, `hero_os.HERO_FS_ABSENT`, `hero_os.HERO_FS_FILE`, `hero_os.HERO_FS_DIRECTORY`, `hero_os.HERO_FS_LINK` and `hero_os.HERO_FS_OTHER`, each `i64`: What `hero_fs_kind` answers: the thing at a path, a link itself where it is one, or -1 where the system could not be asked.

## Functions

- `env_shown(name: str, @status: i64, @marks: i64) -> str`, called `hero_os.env_shown(name, status: @status, marks: @marks)`: The value of the environment variable `name`, `status` set to `HERO_OS_NOT_FOUND` and "" answered where it is unset, and to `HERO_OS_NOT_TEXT` where the value is not UTF-8, the answer then opening with `marks` bytes that name each byte replaced.
- `fs_why() -> i64`, called `hero_os.fs_why()`: The system's own number for why the last call on files failed on this thread, below zero where the runtime refused before asking it, 0 after one that did not fail.
- `fs_resolved_shown(path: str, @status: i64, @marks: i64) -> str`, called `hero_os.fs_resolved_shown(path, status: @status, marks: @marks)`: The absolute place `path` names, every link on the way followed and the parts not there yet as written, `status` set to `HERO_OS_FAILED` and "" answered where it cannot be resolved; read as `env_shown` reads a value that is not UTF-8.
- `fs_kind(path: str) -> i64`, called `hero_os.fs_kind(path)`: What stands at `path`, the link itself where it is one: one of the `HERO_FS_` constants.
- `fs_mode(path: str) -> i64`, called `hero_os.fs_mode(path)`: The permission bits of what `path` names, a link followed, or -1 where it cannot be read; on Windows 0444 for a read-only file and 0666 for any other.
- `run_exe_suffix() -> str`, called `hero_os.run_exe_suffix()`: `.exe` on Windows, where a program's file needs it to be started, and "" on every other system: the one answer by which a program written in Heroes tells Windows apart, as the compiler does.
