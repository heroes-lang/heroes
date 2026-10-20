# Panel 226, the ffi-pragmatist's report

Written as the work went, 2026-10-20, in the seat's own copy
`<scratchpad>/226-ffi-pragmatist/` (a `git archive` of `a0a8d1249`, 12,801 files), its own compiler
built from the seed there (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 7.94 s real,
`sources 5f45d6514b22d650`) and stage 2 from it (`./heroes build selfhost/main.hero -o heroes-next`),
which is the compiler every Heroes program below was compiled with. No file of the lane was touched
but this one. No credential was printed, read or sent; the one paid call the brief allows is item 5
and its paragraph says whether it ran.

- `verdict`: (pending, written at the end)
- `section`: design.md §1.11 (the founding constraint: everything a real program needs comes from C
  through the FFI; the harness is such a program), §1.12 (robustness: a boundary that is complete
  and defended; the sandbox and the watchdog are that boundary for the instrument), §4.19 (the
  `extern` groups the harness spawns through), Part 11's provenance sentence.
- `experiment`: below, one numbered section per item of the brief, each command and what it printed.
- `argument`: (pending)
- `prediction`: (pending)
- `condition`: (pending)

## Tools on this Mac, measured first

```
$ curl --version
curl 8.7.1 (x86_64-apple-darwin26.0) libcurl/8.7.1 (SecureTransport) LibreSSL/3.3.6 zlib/1.2.12 nghttp2/1.69.0
Release-Date: 2024-03-27
$ openssl version            # brew's, first on PATH
OpenSSL 4.0.3 29 Sep 2026 (Library: OpenSSL 4.0.3 29 Sep 2026)
$ /usr/bin/openssl version
LibreSSL 3.3.6
$ claude --version
2.1.292 (Claude Code)
$ command -v codex; echo rc=$?
rc=1
$ ls -la /usr/bin/sandbox-exec
-rwxr-xr-x  1 root  wheel  135136 Sep 24 09:10 /usr/bin/sandbox-exec
$ sw_vers
ProductVersion: 27.0.1   BuildVersion: 26A434
```
