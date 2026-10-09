# Panel 198, llm-ergonomist (blind seat): folder `u1-k`, t1, defect 444's SDL3 program, variant K, route (A)'s note (route K builds with no message on this task's machine, so the brief gives K the (A) note)

Copied by the coordinator at 10:52 on 2026-10-09 from the session's `report.md`,
unchanged below the rule. A fresh `claude -p` session outside the repository
and outside any git tree (the author's exception of 2026-10-09), `claude-opus-5-5`,
`--max-budget-usd 1`, the panel skill's command; started 10:47:53 and ended 10:49:05 by
`date`, exit 0; `run.json`: `success`, 7 turns, 0.3152 USD. Its
`context`: the folder's files and the harness's environment information (the
account's email among it), no project rule; the reading stands. Its inputs are
copied into `docs/panel/198-briefs/blind/u1-k/`. Scored by the coordinator in
`heroes-linux-arm64:sdl3-b14` with the frozen compiler built from `56def9b4`'s
seed, the reader's own build command: `u1-k build 0 run 0: true true true 42 true `.

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
    function SDL_PushEvent(@event: SDL_Event lent) -> bool
    function SDL_PollEvent(@event: SDL_Event lent) -> bool
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

Two changes from the original. Line 1 now says `link "SDL3"` where it said `package "sdl3"`, as the compiler's note suggests. The two event parameters are now marked `lent`.

# files

No file is written or changed apart from `main.hero`, which gets the program above. Commands, in order, run in the directory that holds `main.hero`:

1. `heroes build main.hero -o main --include /usr/local/include --library /usr/local/lib`
2. `./main`

Expected output, five lines:

```
true
true
true
42
true
```

# choice_points

1. **How to get past the `ffi_package` refusal.** I chose `link "SDL3"` and passed the include and library directories as build flags, which is what the compiler's second note says to do. The machine stays as it is.
   - Keeping `package "sdl3"` and editing `/usr/local/lib/pkgconfig/sdl3.pc` to remove `-Wl,--enable-new-dtags` would also build. That changes the machine beyond this program, and it affects every other user of that `.pc` file.
   - Keeping `package "sdl3"` with nothing else changed reproduces the same compile error.
2. **Which build flags.** I used `--include /usr/local/include --library /usr/local/lib`, copied from the note.
   - Leaving out `--include` would most likely give a compile error, because clang cannot find `SDL3/SDL.h`. `/usr/local/include` is often on clang's default search path, but I cannot confirm that here.
   - Leaving out `--library` would probably still link, since `/usr/local/lib` is usually searched. That is not certain, and leaving it out could give a link error.
   - I placed the flags after `-o main`. The note does not say where they go.
3. **How the loader finds `libSDL3` at run time.** `link` writes no rpath, and the note warns about this. I chose to change nothing. `machine.txt` says `ldconfig` was run after SDL was installed into `/usr/local`, and on Ubuntu `/usr/local/lib` is in the default loader configuration (`/etc/ld.so.conf.d/libc.conf`), so the loader should find `libSDL3.so.0`. That last fact comes from my general knowledge, not from this directory.
   - Alternatives are setting `LD_LIBRARY_PATH=/usr/local/lib` when running the program, or adding an rpath. The environment variable changes only how the program is run. Any file added under `/etc/ld.so.conf.d` would change the machine.
4. **Marking `@event` as `lent` on `SDL_PushEvent` and `SDL_PollEvent`.** Section 13 says passing a local with `@` is a lend ("a local lent with `@`, which C receives itself"). It also says "a parameter, `@` or not, is taken to keep what it is handed unless declared `lent`, and a lend reaches only one so declared." The compiler stopped at line 1, so it never checked this.
   - I added `lent`. It is accurate: SDL copies the event in `SDL_PushEvent`, writes into it in `SDL_PollEvent`, and keeps neither pointer.
   - Without `lent`, the program is either accepted the same way, or refused with a compile error because the lend reaches a parameter that is assumed to keep what it gets. That second risk matters most because `got` is a new local on each loop iteration.
   - The sqlite example in the spec leaves out `lent` on its `@out` handle, so the spec can be read either way. Adding the word costs nothing at run time.
5. **The `SDL_Event` union.** `SDL_Event` is a C union, and the record names three of its members: `type`, `key` and `user`. The spec says such a record "names one or more of each union, reads any, and is built naming exactly one". Both constructions name exactly one member (`user:` and `type:`), and reading `got.key.type` after writing through `user` is a read of another member, which is allowed. I left this unchanged. Adding `rest: zero` to the constructions would also build. It would zero the union's remaining bytes, which does not change the five printed lines.
6. **Field widths.** `SDL_KeyboardEvent.type` and `scancode` are C enums. Every value they hold is non-negative, so clang gives them `unsigned int`, which matches `u32`. If clang's check disagreed, the result would be a compile error naming the field, and the fix would be the matching width. I left these unchanged.

# confidence

I believe this builds and prints `true`, `true`, `true`, `42`, `true`, the five lines the program was written to print. On `SDL_Init`, `SDL_INIT_EVENTS` (0x4000) needs no display, so it works on a headless machine.

What I am least sure of:
- Whether the build flags go where I put them (after `-o main`). The note gives the flags but not their position.
- The run-time loader path. The program depends on `/usr/local/lib` being in the loader cache, which is standard on Ubuntu and which the `ldconfig` run supports, but I cannot verify it here. If it fails, the symptom is `./main` exiting with "libSDL3.so.0: cannot open shared object file". Running with `LD_LIBRARY_PATH=/usr/local/lib` fixes that.
- Checks the compiler has not run yet, because it stopped at line 1: the `lent` question and clang's field-width checks against SDL 3.2.10's structs. As far as I recall those structs, the field lists match.

# context

Nothing outside this directory reached my context. I read only `brief.md`, `spec.md`, `main.hero`, `machine.txt` and `build-output.txt`. Some judgments rely on my general knowledge rather than any file: SDL3's struct layouts and constant values (`SDL_INIT_EVENTS`, `SDL_EVENT_USER`), clang's choice of underlying type for enums, and Ubuntu's default loader configuration. My session context also included the user's account email, which I did not use.
