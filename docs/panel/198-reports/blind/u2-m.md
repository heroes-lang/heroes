# Panel 198, llm-ergonomist (blind seat): folder `u2-m`, t2, libcurl refused for `-isystem`, variant M, today's message, the frozen compiler's

Copied by the coordinator at 10:52 on 2026-10-09 from the session's `report.md`,
unchanged below the rule. A fresh `claude -p` session outside the repository
and outside any git tree (the author's exception of 2026-10-09), `claude-opus-5-5`,
`--max-budget-usd 1`, the panel skill's command; started 10:49:18 and ended 10:49:59 by
`date`, exit 0; `run.json`: `success`, 7 turns, 0.2075 USD. Its
`context`: the folder's files and the harness's environment information (the
account's email among it), no project rule; the reading stands. Its inputs are
copied into `docs/panel/198-briefs/blind/u2-m/`. Scored by the coordinator in
`heroes-linux-arm64:sdl3-b14` with the frozen compiler built from `56def9b4`'s
seed, the reader's own build command: `u2-m build 0 run 0: libcurl/8.14.1 OpenSSL/3.5.7 zlib/1.3.1 brotli/1.1.0 zstd/1.5.7 libidn2/2.3.8 libpsl/0.21.2 libssh2/1.11.1 nghttp2/1.64.0 nghttp3/1.8.0 librtmp/2.3 OpenLDAP/2.6.10 `.

---

# program

```
extern "curl/curl.h" link "curl"
    function curl_version() -> cstr

function main()
    print(curl_version().validated().default("<unnamed>"))
```

# files

None. No file on the machine is written or changed, and the only command run is the build itself:

```
heroes build main.hero -o main
```

# choice_points

1. **How to reach libcurl after the compiler refused the package.** pkg-config's answer for `libcurl` includes `-isystem /usr/include/mit-krb5`. The compiler does not accept `-isystem`, and its note says to "name the library directly with `link`". I chose `link "curl"`, which matches the `-lcurl` that pkg-config prints. The header `curl/curl.h` still has to be found without the package's `-I` flags. On Debian the multiarch directory `/usr/include/aarch64-linux-gnu` (the first `-I` pkg-config printed, and where Debian puts `curl/curl.h`) is on clang's default include path for an aarch64 Linux target. So the header resolves without any flag. The other two `-I` directories (mit-krb5, p11-kit-1) come from libcurl's dependencies, and `curl/curl.h` does not include anything from them.
   The alternatives:
   - Keep `package "libcurl"`. This gives the same compile error.
   - Edit `libcurl.pc`, or put a custom `.pc` file on `PKG_CONFIG_PATH`, so the `-isystem` disappears. That would also build, but it changes the machine beyond this program: either a file the package manager owns, or a new file plus an environment setting.
   - Use `link "libcurl"`. That would likely ask the linker for `-llibcurl`, which does not exist, so the build fails at link time.

2. **How to write the call that copies the C string back.** The original writes `validated(c: curl_version())`. The spec shows this built-in only as `c.validated()`. It never gives the parameter's name, so it does not say that `c:` is a name the call accepts. The original build stopped at the package error, so it never tested this. I chose the UFCS form `curl_version().validated()`, which section 9 makes equivalent (`x.f(y)` is sugar for `f(x, y)`) and which needs no parameter name. If I kept `validated(c: ...)`, it would either build identically (if the parameter really is named `c`) or fail with a compile error about an unknown argument name. Behaviour is unchanged either way: print the version string, or `<unnamed>` if C returns null or bytes that are not UTF-8.

3. **No `owned` on the `cstr` result.** `curl_version()` returns a pointer to a static buffer that libcurl owns, and the caller must not free it. I kept the declaration without `owned`, as in the original. Adding `owned free` would type-check differently (the result would become a `str?`, so `.validated()` would be refused, which is a compile error). If it were made to build, it would free static memory at run time and crash.

# confidence

I believe the program builds and prints the libcurl version string (for example `libcurl/8.x.y OpenSSL/... zlib/...`) as the original was meant to. What I am least sure of is that clang finds `curl/curl.h` without the `-I/usr/include/aarch64-linux-gnu` flag. I rely on Debian's clang adding the multiarch include directory by default, which it normally does for a native aarch64 Linux build. If the compiler invokes clang in a way that drops the default system paths, the header would not be found. The fix would then need a change on the machine (for example a custom `.pc` file without `-isystem`), because the language offers no other way to pass an include directory. I am also not certain that the bare `link "curl"` adds no `-L` path the build needs. `libcurl.so` is in `/usr/lib/aarch64-linux-gnu`, which is on the default linker search path, and pkg-config printed no `-L`, which supports this.

# context

Nothing other than this directory's files (`brief.md`, `spec.md`, `main.hero`, `machine.txt`, `build-output.txt`) reached my context, apart from the harness's own environment information (working directory, platform, date) and an account email address, which I did not use. The facts about Debian's header layout and clang's default include paths come from my general knowledge, not from any file I read.
