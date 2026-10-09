# Panel 198, llm-ergonomist (blind seat): folder `u1-r`, t1, defect 444's SDL3 program, variant R, route (G)'s note, `--allow-word`

Copied by the coordinator at 10:52 on 2026-10-09 from the session's `report.md`,
unchanged below the rule. A fresh `claude -p` session outside the repository
and outside any git tree (the author's exception of 2026-10-09), `claude-opus-5-5`,
`--max-budget-usd 1`, the panel skill's command; started 10:47:53 and ended 10:49:14 by
`date`, exit 0; `run.json`: `success`, 7 turns, 0.3405 USD. Its
`context`: the folder's files and the harness's environment information (the
account's email among it), no project rule; the reading stands. Its inputs are
copied into `docs/panel/198-briefs/blind/u1-r/`. Scored by the coordinator in
`heroes-linux-arm64:sdl3-b14` with the frozen compiler built from `56def9b4`'s
seed, the reader's own build command: `u1-r build 0 run 0: true true true 42 true `.

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

The only change is line 1: `package "sdl3"` becomes `link "SDL3"`. Everything else is unchanged.

# files

None. No file on the machine is written or changed. The one command, run in the directory holding `main.hero`:

```
heroes build main.hero -o main
```

and then, to run it:

```
./main
```

Expected output, five lines:

```
true
true
true
42
true
```

# choice_points

1. **How to get past the `ffi_package` error.** The compiler offered two ways: name the library with `link`, or keep `package "sdl3"` and build with `--allow-word -Wl,--enable-new-dtags`. I chose `link "SDL3"`, which keeps the build command exactly as given and does not widen what the compiler accepts from package files.
   - Other choice A (`--allow-word -Wl,--enable-new-dtags` on the build command): a program that builds and does the same thing, with the pkg-config `-I`, `-L` and rpath passed on as well, so it does not depend on the default search paths (see confidence). It changes the build command, not the program or the machine.
   - Other choice B (edit `/usr/local/lib/pkgconfig/sdl3.pc` to drop `-Wl,--enable-new-dtags`): builds and does the same thing, but is a change to the machine beyond this program, affecting every other user of that file. Rejected.

2. **The library name after `link`.** SDL3 installs `libSDL3.so` (pkg-config says `-lSDL3`), so I wrote `"SDL3"`. Writing `"sdl3"` (the package name) would ask the linker for `libsdl3`, which does not exist on a case-sensitive filesystem: a build (link) error.

3. **Where the header and library are found without the package.** With `link`, nothing passes `-I/usr/local/include` or `-L/usr/local/lib`. I rely on clang's default include path on Ubuntu, which contains `/usr/local/include`, on the GNU linker's default search path on Ubuntu, which contains `/usr/local/lib`, and on `ldconfig` having been run (Ubuntu's `/etc/ld.so.conf.d/libc.conf` lists `/usr/local/lib`) so the loader finds `libSDL3.so.0` at run time. If any of those does not hold, the other choice would be choice point 1 A, which supplies the paths itself.

4. **Whether `record SDL_Event` must be `partial`.** `SDL_Event` is a C union with many more members than the three named here. The spec says a record "names one or more of each union", and `partial` is for a record that "names only some" of a struct's fields. I read the union rule as governing here and left the record without `partial`, as it was. Adding `partial` would, as far as I can tell, also build and behave the same, since the program never compares an `SDL_Event` or uses one as a map key; if the compiler instead wanted `partial` and I left it out, the result would be a compile error naming that record.

5. **Constructing the union.** `SDL_Event(user: ...)` and `SDL_Event(type: 0)` each name exactly one union member, which the spec allows. Ending either with `rest: zero` instead would also build; it would make the zeroing of the remaining bytes explicit but changes nothing observable, since only `type`, `user.code` and `key.type` are read and all lie within the named or C-written bytes. I kept the original.

6. **`@event` without `lent`.** `SDL_PushEvent` and `SDL_PollEvent` take `SDL_Event *`. The program passes them as `@` out-parameters on local `@` names, which C receives itself; SDL copies the event and keeps no pointer. I kept them as written. Adding `lent` would declare the same thing more strictly; I do not expect either to change the build.

7. **Field and value meanings.** I kept `0x4000` (`SDL_INIT_EVENTS`) and `0x8000` (`SDL_EVENT_USER`), and the field widths, which match SDL 3.2's `SDL_events.h` as I know it (`SDL_EventType`, `SDL_Scancode` and `SDL_Keycode` 32-bit unsigned, `SDL_Keymod` and `raw` 16-bit, `code` `Sint32`). Changing any width would be refused by clang's check against the header: a compile error.

# confidence

I believe the program builds with `heroes build main.hero -o main` and prints `true`, `true`, `true`, `42`, `true`: `SDL_Init` of the events subsystem needs no display, the pushed user event comes back from `SDL_PollEvent` within the 64 tries, its `code` reads as 42, and `key.type` reads the same first four bytes as `type`, so it equals `0x8000`.

What I am least sure of is choice point 3: that the Heroes compiler's clang invocation uses the system's default include and library search paths (for example, that it does not use a sysroot or `lld`, which has no built-in `/usr/local/lib`). If it does not find `SDL3/SDL.h` or `-lSDL3`, the fallback is to keep `package "sdl3"` and build with `--allow-word -Wl,--enable-new-dtags`. Second, the build stopped at the package step, so no later check of the program (the union record without `partial`, field names such as `repeat` and `mod`) has been seen to pass; I read them against the spec and found nothing it refuses.

# context

Only this directory's files (`brief.md`, `spec.md`, `main.hero`, `machine.txt`, `build-output.txt`) were read. Besides them, my context held the harness's system prompt and an automatically attached note giving the user's account email address; neither informed the answer. My knowledge of SDL 3.2's headers and of Ubuntu's default compiler, linker and loader paths comes from training, not from any file here.
