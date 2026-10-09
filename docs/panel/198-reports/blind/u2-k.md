# Panel 198, llm-ergonomist (blind seat): folder `u2-k`, t2, libcurl refused for `-isystem`, variant K, route (A)'s note (route K builds with no message on this task's machine, so the brief gives K the (A) note)

Copied by the coordinator at 10:52 on 2026-10-09 from the session's `report.md`,
unchanged below the rule. A fresh `claude -p` session outside the repository
and outside any git tree (the author's exception of 2026-10-09), `claude-opus-5-5`,
`--max-budget-usd 1`, the panel skill's command; started 10:49:18 and ended 10:49:58 by
`date`, exit 0; `run.json`: `success`, 7 turns, 0.2064 USD. Its
`context`: the folder's files and the harness's environment information (the
account's email among it), no project rule; the reading stands. Its inputs are
copied into `docs/panel/198-briefs/blind/u2-k/`. Scored by the coordinator in
`heroes-linux-arm64:sdl3-b14` with the frozen compiler built from `56def9b4`'s
seed, the reader's own build command: `u2-k build 0 run 0: libcurl/8.14.1 OpenSSL/3.5.7 zlib/1.3.1 brotli/1.1.0 zstd/1.5.7 libidn2/2.3.8 libpsl/0.21.2 libssh2/1.11.1 nghttp2/1.64.0 nghttp3/1.8.0 librtmp/2.3 OpenLDAP/2.6.10 `.

---

# program

```
extern "curl/curl.h" link "curl"
    function curl_version() -> cstr

function main()
    print(curl_version().validated().default("<unnamed>"))
```

# files

No file is written or changed besides `main.hero`. One command is run on the machine, in the directory holding `main.hero`:

```
heroes build main.hero -o main --include /usr/include/aarch64-linux-gnu --include /usr/include/mit-krb5 --include /usr/include/p11-kit-1
```

Then `./main` runs the program. Nothing is installed or reconfigured.

# choice_points

1. **How to get past the `ffi_package` error.** The compiler refuses `package "libcurl"` because pkg-config answers with `-isystem`. I took the compiler's note and replaced it with `link "curl"`. The spec allows `link` or `package` in an `extern` group (section 13), and the program only needs `-lcurl` plus the header.
   - Keeping `package "libcurl"` gives the same compile error again.
   - Editing the system's `libcurl.pc` so it drops `-isystem` would also work, but it changes the machine beyond this program, so I did not do it.

2. **Which include directories to pass.** The note says to build with three `--include` flags, one for each directory pkg-config named. I passed all three, as the note says. `-isystem /usr/include/mit-krb5` becomes an ordinary include, which only changes how warnings are treated in headers from that directory.
   - Passing only `/usr/include/aarch64-linux-gnu`, or none, would most likely also build. Debian puts libcurl's multiarch headers in `/usr/include/aarch64-linux-gnu/curl/`, and clang on Debian searches that directory by default. `curl/curl.h` does not include the krb5 or p11-kit headers.
   - If that default search were missing, though, leaving the flags out would give a compile error (header not found). Following the note costs nothing.
   - `--include` is not in the spec. It comes only from the compiler's message.

3. **How `validated` is called.** The original writes `validated(c: curl_version())`, which names the argument `c`. The spec only shows `c.validated()`, where `c` is the receiver variable in the example, not a documented parameter name. I rewrote the call in UFCS form, `curl_version().validated()`. Section 9 says `x.f(y)` is `f(x, y)`, so this is the same call without depending on a parameter name.
   - Keeping `c:` builds only if the built-in's parameter really is named `c`. Otherwise it is a compile error. Either way the program would do the same thing.

4. **How to declare `curl_version`.** I kept `-> cstr` with no `owned`, `acquires` or other mark. C's `char *curl_version(void)` returns a static buffer that the caller must not free.
   - Adding `owned <disposer>` would make the compiler free libcurl's static string. That is a crash at runtime, and no free function for it exists anyway.

5. **The fallback text.** I kept `.default("<unnamed>")` unchanged. `curl_version` never returns null and its text is ASCII, so the fallback is not reached. `.must()` would behave the same here but would abort instead of printing if validation ever failed.

# confidence

I believe this builds and prints libcurl's version string, for example `libcurl/8.x.y OpenSSL/... zlib/...`, followed by one newline, as the original intended.

What I am least sure of:
- The exact spelling and behaviour of the `--include` build flag. I know it only from the compiler's note.
- Whether the dev package's `libcurl.so` symlink is where the linker looks by default. With Debian multiarch it is in `/usr/lib/aarch64-linux-gnu`, which is a default linker path, and machine.txt says the development files were installed, so I expect `-lcurl` to resolve.

# context

Only this directory's files reached my context: `brief.md`, `spec.md`, `main.hero`, `machine.txt` and `build-output.txt`. Nothing else came in, apart from the harness's automatic environment notes (working directory, date, and the user's account email). None of those were used for the answer. The statements about Debian's header and library locations come from my general knowledge, not from any file.
