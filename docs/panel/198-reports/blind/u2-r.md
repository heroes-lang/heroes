# Panel 198, llm-ergonomist (blind seat): folder `u2-r`, t2, libcurl refused for `-isystem`, variant R, route (G)'s note, `--allow-word`

Copied by the coordinator at 10:52 on 2026-10-09 from the session's `report.md`,
unchanged below the rule. A fresh `claude -p` session outside the repository
and outside any git tree (the author's exception of 2026-10-09), `claude-opus-5-5`,
`--max-budget-usd 1`, the panel skill's command; started 10:49:18 and ended 10:49:59 by
`date`, exit 0; `run.json`: `success`, 7 turns, 0.2060 USD. Its
`context`: the folder's files and the harness's environment information (the
account's email among it), no project rule; the reading stands. Its inputs are
copied into `docs/panel/198-briefs/blind/u2-r/`. Scored by the coordinator in
`heroes-linux-arm64:sdl3-b14` with the frozen compiler built from `56def9b4`'s
seed, the reader's own build command: `u2-r build 0 run 0: libcurl/8.14.1 OpenSSL/3.5.7 zlib/1.3.1 brotli/1.1.0 zstd/1.5.7 libidn2/2.3.8 libpsl/0.21.2 libssh2/1.11.1 nghttp2/1.64.0 nghttp3/1.8.0 librtmp/2.3 OpenLDAP/2.6.10 `.

---

# report

## program

```
extern "curl/curl.h" link "curl"
    function curl_version() -> cstr

function main()
    print(curl_version().validated().default("<unnamed>"))
```

## files

No file is written or changed. One command, run in this directory on the machine:

```
heroes build main.hero -o main
```

## choice_points

1. **How to get past `ffi_package`.** The compiler refused the `-isystem /usr/include/mit-krb5` word from `pkg-config` and suggested two ways forward: name the library with `link`, or pass `--allow-word -isystem` for one build.
   - **Chosen:** replace `package "libcurl"` with `link "curl"` (section 13: "`link` a library when the symbols need one"). The only flag the program needs from the package is `-lcurl`. The header `curl/curl.h` is under `/usr/include/aarch64-linux-gnu`, the Debian multiarch directory that clang on Debian searches by default, so the `-I` flag is not needed either. The `mit-krb5` and `p11-kit` paths are there for other parts of the curl development setup, and `curl_version` does not use them.
   - **Other choice, `--allow-word -isystem`:** keeps `package` and would most likely build the same binary. But it means accepting a flag the compiler blocks on purpose as a code-execution risk (CVE-2018-6574), and every future build has to repeat it. It changes the build command, not the program, and I did not want to waive a safety check that this program does not need.
   - **Third choice, editing `libcurl.pc` (or adding a wrapper `.pc` file) to remove `-isystem`:** this would also build. It changes the machine beyond this program and affects every other user of that package file, so I rejected it.

2. **How to call `validated`.** The original writes `validated(c: curl_version())`. The spec only shows the built-in as `c.validated()` and never gives its parameter's name. Argument names are only required when two parameters share a type, and there is just one parameter here, so the name `c:` is a guess about the built-in's signature.
   - **Chosen:** `curl_version().validated()`. By UFCS (section 9) this is the same call, so it does the same thing, and it does not depend on the parameter name.
   - **Other choice, keeping `validated(c: ...)`:** builds the same program if the built-in's parameter really is named `c`. If it is not, the build fails with a compile error. The compiler stopped at the FFI error, so the output does not show whether this line would have been accepted.

3. **Ownership of the returned `cstr`.** `curl_version()` returns a pointer to a static buffer inside libcurl. I kept the result with no `owned` mark, as in the original. Adding `owned <fn>` would build a program that frees static memory with that function, which would crash or corrupt memory at run time.

## confidence

I believe this builds and prints the libcurl version string, the same as before, e.g. `libcurl/8.x.y OpenSSL/... zlib/...`. The `"<unnamed>"` fallback is used only if C returns null or text that is not valid UTF-8, which does not happen in practice.

What I am least sure of: whether this compiler's clang call finds `curl/curl.h` without the `-I/usr/include/aarch64-linux-gnu` flag that the package used to supply. That works if it uses clang's normal Debian include path. If it does not, the build fails with a header-not-found error. The fallback would then be to keep `package "libcurl"` and build with `heroes build main.hero -o main --allow-word -isystem`. My second uncertainty is minor: `validated` is assumed to be a built-in that UFCS can call, which section 13's `c.validated()` shows directly.

## context

Nothing other than this directory's files (`brief.md`, `spec.md`, `main.hero`, `machine.txt`, `build-output.txt`) reached my context, apart from the harness's system context: environment details and the user's account email, which I did not use. My knowledge of Debian's multiarch header layout and clang's default include paths comes from general training knowledge, not from any file I read.
