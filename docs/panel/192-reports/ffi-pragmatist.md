# Panel 192, the ffi-pragmatist: Q4 (a NUL at the C boundary, the runtime's doors) and Q5 (`args()` on Windows)

Started 2026-10-04 18:16:20 (from `date`). Written as I go; a section that
says *in progress* is not finished.

**My copy**: `<scratchpad>/192-ffi-pragmatist/`, from `git -C <trunk>
archive 4c3524fb | tar -x`. Seed sha256 begins `c79ffd5ad005c301`,
35,206,983 bytes. Compiler built from it 18:16:35 to 18:16:39:
`shasum -a 1 heroes` reads `8084f018f53875362048cc0230d23207976d8b00`. All
three match `00-facts.md`'s base. Programs and probes live in
`<scratchpad>/192-ffi-pragmatist-work/`; each route is its own copy,
`<scratchpad>/192-ffi-pragmatist-r<X>/`, from the same archive.

## Status

- Q4: in progress (measured facts below; routes being built).
- Q5: waits on the Windows box (held by batch 10's leg, then lane
  b11-windows). The Mac and Linux arm64 halves are done here first.

## Q4, measured on the base (18:26 to 18:30)

### F4 reproduces

`192-ffi-pragmatist-work/f4-base/`, F4's four programs copied and built with
my base compiler, each `build` exit 0:
- `nulc` prints `1`, exit 0;
- `rf` prints `3`, `1`, exit 0;
- `qi-read` prints `3`, `false`, `the file named a`, exit 0;
- `qi-write` prints `11`, `false`, exit 0, and writes `out-a` holding
  `written`.

### Where a program's `str` gets a NUL: one more door than the critic's

`192-ffi-pragmatist-work/sources/sources.hero` with a header of my own,
`nulsrc.h`, whose C hands back the bytes `61 00 62`:

| constructor | what the program holds |
|---|---|
| `nul_three().validated()` (a `cstr` over `a\0b`) | length 1: `strlen` |
| `[97, 0, 98].validated_bytes()` | length 1: `memchr`, to the first zero |
| `nul_owned()`, `-> cstr owned free` | length 1 |
| `nul_str()`, `-> str`, whose C is `return hero_str_from_bytes("a\0b", 3);` | **length 3** |

So **C itself is a third door**, beside a raw literal and `read_file`: a
binding's own C returning `str` through `hero_str_from_bytes` with an
explicit length, which design.md §4.20 names as the constructor that makes
§4.19's ladder step 3 writable. The critic's *"after Q1, a program's NUL
comes from `read_file`"* is false by this door.

**The constructors are a closed set, by the code.** `runtime.c` includes
every part into one translation unit (`:117` to `:200`), so every heap `str`
is born in the static `hero_str_alloc` (`str.c:46`), whose callers are 11
lines: `str.c` 155 (`repeat`), 165 (`concat`), 225 (`slice`), 306
(`from_bytes`), 347 (`try_from_cstr`), 387 (`try_from_bytes`); `f64.c` 222,
236, 256, 268 (number to text); `text.c:120` (`join`). The two static forms
are the literal macro `HERO_STR_STATIC` (`heroes_runtime.h:186`, length
`sizeof(text) - 1`, which is how a raw NUL in a literal is kept) and
`hero_empty_block` (`str.c:24`). Of these, the ones that copy bytes from
outside a `str` with a length the caller states are `from_bytes` alone;
`try_from_cstr` and `try_from_bytes` stop at the first zero; `repeat`,
`concat`, `slice`, `join` copy only bytes already in a `str`; the numbers
write digits. **So the doors through which a NUL enters a `str` are three:
the literal (compile time), and at run time every caller of
`hero_str_from_bytes` with a length not from `strlen`**: `hero_file_read`
(`os.c:814`), `hero_bytes_shown` (`os.c:649`, `:703`, the compiler's shown
read of a file and of the environment), `chars()` (`text.c:87`, a piece of
a `str` that already holds it), `hero_run_win_command_line` (`run.c:210`,
Windows, words already `strlen`ed), and any binding's C.

### The doors, enumerated at the parameter rather than by vocabulary

A grep of 120 call names (POSIX, C and Win32 calls that take a name),
comment lines dropped, prints 75 lines in six parts; mapped to their
enclosing functions (a script over the grep), every one sits under a
`hero_os.h` door or a static helper of one. **And it still missed one**:
`hero_fs_deny_delete` (`replace.c:737`) reaches the system through
`acl_get_file` and `acl_set_file`, which no vocabulary of mine named. So the
enumeration that is complete by construction is the door's parameter list:
a C function receives a name only through a parameter or a global, and the
globals here are `hero_run_words` (filled by `hero_run_arg`) and `hero_argv`.

**`hero_os.h` declares 27 doors whose 36 name parameters are
`const char *`**, and one that takes the `str`:

| door | name parameters | reaches |
|---|---|---|
| `hero_file_read`, `hero_file_read_shown` | `path` | `fopen` (`os.c:747`) |
| `hero_env_shown` | `name` | `getenv` |
| `hero_file_write` | `path` | `fopen` (`os.c:857`) |
| `hero_fs_exists`, `hero_fs_is_directory` | `path` | `stat`, `GetFileAttributesA` |
| `hero_fs_mkdir_all` | `path` | `mkdir`, `_mkdir` |
| `hero_fs_remove` | `path` | `remove`, `RemoveDirectoryA` |
| `hero_fs_newer_than` | `path`, `reference` | `stat`, `GetFileAttributesExA` |
| `hero_fs_rename` | `from`, `to` | `rename`, `MoveFileExA` |
| `hero_fs_landing` | `path` | `lstat`, `readlink`, `CreateFileA`, `FindFirstFileA` |
| `hero_fs_kind`, `hero_fs_links`, `hero_fs_writable` | `path` | `lstat`, `stat`, `open`, `CreateFileA` |
| `hero_file_stage` | `staged`, `like` | `open`, `stat`, `unlink`, `CreateFileA`, `DeleteFileA`, `Get`/`SetFileAttributesA` |
| `hero_fs_replace` | `staged`, `path` | `rename`, `open` (the directory), `MoveFileExA` |
| `hero_file_unstage` | `staged` | `unlink`, `DeleteFileA` |
| `hero_fs_mode`, `hero_fs_set_mode` | `path` | `stat`, `chmod`, `Get`/`SetFileAttributesA` |
| `hero_fs_link`, `hero_fs_symlink` | two each | `link`, `symlink`, `CreateHardLinkA`, `CreateSymbolicLinkA` |
| `hero_fs_deny_delete` | `path` | `acl_get_file`, `acl_set_file` (macOS) |
| `hero_fs_flagged`, `hero_fs_set_flag` | `path` | `stat`, `open`, `chflags`, `Get`/`SetFileAttributesA` |
| `hero_dir_scan`, `hero_dir_remove_tree` | `root`, `path` | `opendir`, `FindFirstFileA`, then `hero_fs_remove` |
| `hero_run_go` | `program`, `in_path`, `out_path`, `err_path` | `execvp`, `open` (`run.c:378` to `:411`), `CreateFileA`, `CreateProcessA` |
| **`hero_run_arg`** | **`HeroStr word`** | `execvp`'s words, `CreateProcessA`'s line |

A program can reach every one: an `extern "hero_os.h"` group in a program's
own module is accepted (the compiler's own tests declare `hero_file_read`
that way, `check/contracts.hero:324`), and the prelude reaches two,
`read_file` and `write_file` (`library_source.hero:208`, `:222`).
