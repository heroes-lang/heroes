# Panel 242, the ffi-pragmatist's brief

Read `00-shared.md` beside this file first. Your worktree:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/242-ffi-pragmatist` (detached at
1edabb957); build your compiler there from the seed and nowhere else. Your report:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p242/docs/panel/242-reports/ffi-pragmatist.md`.
The Linux arm64 container is `heroes-linux-arm64:latest` (`docs/platforms/linux/LINUX-MACHINE.md`);
the Windows box is `ssh win`, a folder of your own `C:/w/p242-ffi-pragmatist/`, no other touched.

## Question 2 is yours, compiled

- **Route (c), a `net/http` package over libcurl**: write the binding a package would carry
  (`pkgconfig "libcurl"`), the calls an index read needs (an easy handle, a URL, a write callback
  or the fill form, the result code), compile it and fetch a `file://` URL of a local file on this
  Mac, in the container and on the box (no network beyond the machine). Then the seed: what the
  compiler's one clang line becomes if the package manager reads the index through it, and whether
  that line builds where libcurl is not installed (say where it is and is not, by command).
- **Route (b) and (d)**: an `os` package over `hero_os.h` running a program and reading its output,
  bound from a package and not from the compiler; which `hero_os.h` functions it binds, and what
  task 2263 owes each (a stated ownership for every `HeroStr` result).

## Question 3 is yours, run on the box

- `hero_env_put` with its Windows arm over `_wputenv_s`, panel 241's route: does `_wputenv_s(name,
  L"")` remove the variable where `setenv(name, "", 1)` sets it empty? Run it on the box; a caller
  in the tree passes `""` (`tests/harness/case_setting.hero:212-226`). Then the arm that keeps the
  POSIX meaning on every value, built in a scratch copy of the runtime, its cost in lines, and what
  moving the door into `hero_os.h` costs (`heroes_guard_open.h`, `heroes_guard_close.h`,
  `selfhost/emit/guarded_names.hero`, decision 2228's promise).

## Your verdict

Per route: approve, object or veto, with the C's lines and what you compiled and ran on which
platform. Veto on ABI breakage or unsoundness at the boundary only, naming the program that would
break.
