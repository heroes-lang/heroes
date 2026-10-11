---
kind: defect
area: site
milestone: none
filed: 2026-10-11
commit: a1ec2eb995098507b4d60523e82acfb367befccb
github: none
---

- [ ] **669 — site/serve.py leaves its TLS floor to the Python and OpenSSL it runs on** | CodeQL alert 3 (`py/insecure-protocol`, high): `ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)` states no `minimum_version`, so TLS 1.0 and 1.1, deprecated by RFC 8996, are allowed wherever the library's default allows them. On this Mac the default floor already reads 771, TLS 1.2 (`python3 -I`, OpenSSL 4.0.3), so nothing older was offered here; the floor rested on the machine | `site/serve.py:195` · **class: improvement**

    **Origin:** the author, 2026-10-11, sending the repository's code-scanning page at 12:47 with *check these too*; the coordinator read the eight open alerts the same minute with `gh api 'repos/heroes-lang/heroes/code-scanning/alerts?state=open'`, opened every location, and wrote each repair in lane b22-codeql.

    **Class: improvement**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): no old protocol was offered on the machine measured; the floor rested on the library's default.

    Repaired at `a1ec2eb995098507b4d60523e82acfb367befccb`, 2026-10-11 (lane b22-codeql). `ctx.minimum_version = ssl.TLSVersion.TLSv1_2` before the certificate is loaded, with the reason. Run with `python3 -I`: the file parses, and a context so set reads 771. The local preview server only; nothing deployed runs it.
