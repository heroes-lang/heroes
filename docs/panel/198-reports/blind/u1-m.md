# Panel 198, llm-ergonomist (blind seat): folder `u1-m`, t1, defect 444's SDL3 program, variant M, today's message, the frozen compiler's

Copied by the coordinator at 10:52 on 2026-10-09 from the session's `report.md`,
unchanged below the rule. A fresh `claude -p` session outside the repository
and outside any git tree (the author's exception of 2026-10-09), `claude-opus-5-5`,
`--max-budget-usd 1`, the panel skill's command; started 10:47:53 and ended 10:48:58 by
`date`, exit 0; `run.json`: `success`, 7 turns, 0.2977 USD. Its
`context`: the folder's files and the harness's environment information (the
account's email among it), no project rule; the reading stands. Its inputs are
copied into `docs/panel/198-briefs/blind/u1-m/`. Scored by the coordinator in
`heroes-linux-arm64:sdl3-b14` with the frozen compiler built from `56def9b4`'s
seed, the reader's own build command: `u1-m build 0 run 0: true true true 42 true `.

---

# program

```
extern "SDL3/SDL.h" link "SDL3"
    record SDL_KeyboardEvent
        type: u32
        reserved: u32
        timestamp: u64
        windowID: u32
        which: u32
        scancode: u32
        key: u32
        mod: u16
        raw: u16
        down: bool
        repeat: bool
    record SDL_UserEvent
        type: u32
        reserved: u32
        timestamp: u64
        windowID: u32
        code: i32
        data1: ptr
        data2: ptr
    record SDL_Event
        type: u32
        key: SDL_KeyboardEvent
        user: SDL_UserEvent
    function SDL_Init(flags: u32) -> bool
    function SDL_PushEvent(@event: SDL_Event) -> bool
    function SDL_PollEvent(@event: SDL_Event) -> bool
    function SDL_Quit()

function main()
    print(SDL_Init(flags: 0x4000))
    sent: SDL_Event @ SDL_Event(
        user: SDL_UserEvent(type: 0x8000, reserved: 0, timestamp: 0, windowID: 0, code: 42, data1: nullptr, data2: nullptr)
    )
    print(SDL_PushEvent(@sent))
    found: bool @ false
    code: i32 @ 0
    keyed: u32 @ 0
    tries: i64 @ 0

    while !found && tries < 64
        got: SDL_Event @ SDL_Event(type: 0)

        if SDL_PollEvent(@got) && got.type == 0x8000
            found @ true
            code @ got.user.code
            keyed @ got.key.type
        tries @ tries + 1

    print(found)
    print(code)
    print(keyed == 0x8000)
    SDL_Quit()
```

The only change is on line 1: `package "sdl3"` becomes `link "SDL3"`.

# files

No file is written or changed besides `main.hero`. Commands, in order, run in this directory on the machine:

```
heroes build main.hero -o main
./main
```

Expected output:

```
true
true
true
42
true
```

# choice_points

1. **How to answer the `ffi_package` error.** The compiler refuses the package because `pkg-config` for `sdl3` answers with `-Wl,--enable-new-dtags`, and its note says to name the library directly with `link`. I chose `link "SDL3"`, which needs no flag from pkg-config. This relies on the toolchain's default search paths: on Ubuntu 24.04 clang searches `/usr/local/include` for headers (where `SDL3/SDL.h` is), GNU ld searches `/usr/local/lib` for `libSDL3.so`, and the runtime loader finds it because `/usr/local/lib` is in `/etc/ld.so.conf.d/libc.conf` and `ldconfig` was run. Other choices:
   - Edit `/usr/local/lib/pkgconfig/sdl3.pc` to remove `--enable-new-dtags`, or install a private `.pc` file and point `PKG_CONFIG_PATH` at it. That changes the machine beyond this program, and it affects every other build that uses SDL3. The spec also does not say whether the compiler respects `PKG_CONFIG_PATH`.
   - Keep `package "sdl3"` unchanged. That reproduces the same compile error.
2. **The library name in `link`.** `link "SDL3"` matches `-lSDL3` from pkg-config and the spec's `link "sqlite3"` example. Any other name (`"sdl3"`, `"SDL3.so"`) would fail at the link step, since there is no such library.
3. **Losing the rpath.** The package would have passed `-Wl,-rpath,/usr/local/lib` (the compiler accepts that flag), but `link` has no way to give an rpath. I rely on `ldconfig` instead. If `/usr/local/lib` were not in the loader's configuration, the program would build but fail to start because it could not find `libSDL3.so.0`. The fix for that would be a change to the machine (`LD_LIBRARY_PATH` or an `ld.so.conf.d` entry).
4. **`SDL_Event` as a union naming only three of its members.** The spec says a group's record is "the header's struct: all its fields", but it also says "a record names one or more of each union". I read the second rule as allowing a subset of a union's members, so I left the record without `partial`, as the program had it. The compiler reported no error there. Adding `partial` would probably also build and behave the same, since the program never compares or map-keys an `SDL_Event`. I kept the original so that the behaviour is unchanged.
5. **`@event` without `lent` on `SDL_PushEvent` / `SDL_PollEvent`.** The spec says a parameter not declared `lent` is assumed to keep what it is handed. SDL copies the event and keeps no pointer, so `lent` would be accurate. However, the spec does not say that leaving it out is an error, and the original program did not use it, so I left it out to keep the change minimal. Adding it would not change the output.

# confidence

I believe the program builds and prints `true`, `true`, `true`, `42`, `true` as before. Apart from the build error, I checked the code against the spec and against SDL 3.2.10's declarations:
- the field widths of `SDL_KeyboardEvent` and `SDL_UserEvent` match the header;
- `SDL_INIT_EVENTS` is `0x4000` and `SDL_EVENT_USER` is `0x8000`;
- each `SDL_Event` is built naming exactly one union member.

What I am least sure of:
- Whether `heroes` passes clang no extra sysroot or `-nostdinc`-style flags, so that clang's default `/usr/local/include` and ld's default `/usr/local/lib` are really searched. The spec does not say how `link` resolves directories.
- Whether the record checks behind the first error accept `SDL_Event` without `partial`. The build stopped at the package error, so it never showed the result of those checks.

# context

Only this directory's files reached my context: `brief.md`, `spec.md`, `main.hero`, `machine.txt` and `build-output.txt`. I also used my general knowledge of SDL 3's headers, Ubuntu's default compiler, linker and loader search paths, and pkg-config. I read no other file and ran nothing.
